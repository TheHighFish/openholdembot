#include "StdAfx.h"
#include <io.h>
#include <fcntl.h>

#include "CGlobal.h"

#include "OpenHoldem.h"

#include "CScraper.h"
#include "CSymbols.h"
#include "CHeartbeatThread.h"

#include "resource.h"
#include "grammar.h"
#include "registry.h"
#include "DialogFormulaScintilla.h"

CGlobal				*p_global = NULL;
CRITICAL_SECTION	CGlobal::cs_global;

//  Sanity check: enough disk-space for a replay frame?
//	We assume, 10 MB are enough
//	  - a large table of 1000x1000 pixels a 4 byte
//	  - some KB for the HTML file.
//	  - some space for other processes	
const unsigned int FREE_SPACE_NEEDED_FOR_REPLAYFRAME = 10000000;  


CGlobal::CGlobal(void)
{
	__SEH_SET_EXCEPTION_HANDLER

	__SEH_HEADER

	int			i;
	Registry	reg;

	InitializeCriticalSectionAndSpinCount(&cs_global, 4000);

	ClearFormula(&formula);
	formula_name = "";

	ClearR$Indices();

	trans.map.num_chairs = 10;

	// Get saved preferences
	reg.read_reg();

	// analyzer
	preferences.max_opponents = reg.max_opponents;

	// autoplayer
	preferences.frame_delay = reg.frame_delay;
	preferences.swag_delay_1 = reg.swag_delay_1;
	preferences.swag_delay_2 = reg.swag_delay_2;
	preferences.swag_delay_3 = reg.swag_delay_3;
	preferences.text_selection_method = reg.text_selection_method;
	preferences.text_deletion_method = reg.text_deletion_method;
	preferences.bet_confirmation_method = reg.bet_confirmation_method;
	preferences.button_click_method = reg.button_click_method;
	preferences.ap_auto = reg.ap_auto;
	autoplay_pressed = false;
	preferences.focus_detect = reg.focus_detect;

	// dll extension
	preferences.dll_always_send_state = reg.dll_always_send_state;
	preferences.load_dll_on_startup = reg.load_dll_on_startup;
	preferences.dll_name = reg.dll_name;

	// scraper
	preferences.scrape_delay = reg.scrape_delay;

	// symbols
	preferences.av_time = reg.avtime;
	preferences.handrank_value = reg.handrank_value;
	preferences.disable_caching = reg.disable_caching;

	// poker tracker
	preferences.pt_prefs.ip_addr = reg.pt_ip_addr;
	preferences.pt_prefs.port = reg.pt_port;
	preferences.pt_prefs.dbname = reg.pt_dbname;
	preferences.pt_prefs.user = reg.pt_user;
	preferences.pt_prefs.pass = reg.pt_pass;
	preferences.pt_prefs.disable = reg.pt_disable;
	preferences.pt_prefs.update_delay = reg.pt_updatedelay;
	preferences.pt_prefs.cache_refresh = reg.pt_cacherefresh;

	// icm
	preferences.icm_prize1 = reg.icm_prize1;
	preferences.icm_prize2 = reg.icm_prize2;
	preferences.icm_prize3 = reg.icm_prize3;
	preferences.icm_prize4 = reg.icm_prize4;

	// Replay frames
	preferences.replay_record = reg.replay_record;
	preferences.replay_record_every_change = reg.replay_record_every_change;
	preferences.replay_max_frames = reg.replay_max_frames;

	//  2008.02.27 by THF
	//  Get saved preferences for Perl
	preferences.Perl_default_Formula = reg.Perl_default_Formula;
	preferences.Perl_Editor = reg.Perl_Editor;
	preferences.Perl_load_default_Formula = reg.Perl_load_default_Formula;
	preferences.Perl_load_Interpreter = reg.Perl_load_Interpreter;

	//  2008.02.27 by THF
	//  Get saved preferences for PokerChat
	preferences.Chat_enabled = reg.Chat_enabled;
	preferences.Chat_min_Delay = reg.Chat_min_Delay;
	preferences.Chat_random_Delay = reg.Chat_random_Delay;

	//  Get saved preferences for log$
	preferences.LogSymbol_enabled = reg.LogSymbol_enabled;
	preferences.LogSymbol_max_log = reg.LogSymbol_max_log;

	preferences.Trace_enabled = reg.Trace_enabled;
	for (int i=0;i<nTraceFunctions;i++)
		preferences.Trace_functions[i] = reg.Trace_functions[i];

	// Find the versus data.  First check in the current directory
	// then in the path provided by the registry.  If both fail,
	// disable versus.
	versus_fh = _sopen("versus.bin", _O_RDONLY | _O_BINARY, _SH_DENYWR, NULL);
	if (versus_fh == -1)
	{
		versus_fh = _sopen(reg.versus_path, _O_RDONLY | _O_BINARY, _SH_DENYWR, NULL);
	}

	if (versus_fh == -1)
	{
		MessageBox(NULL, "Could not open versus.bin.\nVersus functions will be disabled.\n", "Versus Error", MB_OK | MB_TOPMOST);
	}

	EnterCriticalSection(&cs_global);
		_attached_hwnd = NULL;

		for (i=0; i<10; i++)
			_flags[i] = false;
	LeaveCriticalSection(&cs_global);

	ppro_is_connected = false;
	ppro_isppro = false;
	ppro_tid = 0;

	state_index = 0;

	mm_network = "";

	autoplay = false;
	autoplay_pressed = false;

	EnterCriticalSection(&cs_global);
		_m_wait_cursor = false;
		_replay_recorded_this_turn = false;
	LeaveCriticalSection(&cs_global);

	__SEH_LOGFATAL("CGlobal::Constructor : \n");
}

CGlobal::~CGlobal(void)
{
	__SEH_HEADER

	DeleteCriticalSection(&cs_global);

	__SEH_LOGFATAL("CGlobal::Destructor : \n");
}

void CGlobal::ClearFormula(SFormula *f)
{
	__SEH_HEADER

	f->dBankroll = f->dDefcon = f->dRake = f->dNit = 0.0;
	f->mHandList.RemoveAll();
	f->mFunction.RemoveAll();

	__SEH_LOGFATAL("CGlobal::ClearFormula : \n");
}


void CGlobal::CopyFormula(SFormula *f, SFormula *t)
{
	__SEH_HEADER

	SHandList		list;
	SFunction		func;
	int				from_count, to_count, from_iter, to_iter;
	bool			addit, deleteit;

	// handle deleted udfs
	to_count = (int) t->mFunction.GetSize();
	for (to_iter=0; to_iter<to_count; to_iter++)
	{
		from_count = (int) f->mFunction.GetSize();
		deleteit = true;
		for (from_iter=0; from_iter<from_count; from_iter++)
		{
			if (t->mFunction[to_iter].func == f->mFunction[from_iter].func)
			{
				deleteit = false;
				from_iter=from_count+1;
			}
		}
		if (deleteit)
		{
			t->mFunction.RemoveAt(to_iter, 1);
			to_count = (int) t->mFunction.GetSize();
			to_iter-=1;
		}
	}

	// handle new/changed udfs
	from_count = (int) f->mFunction.GetSize();
	for (from_iter=0; from_iter<from_count; from_iter++)
	{
		to_count = (int) t->mFunction.GetSize();
		addit = true;
		for (to_iter=0; to_iter<to_count; to_iter++)
		{
			if (f->mFunction[from_iter].func == t->mFunction[to_iter].func)
			{
				// changed?
				addit = false;
				if (f->mFunction[from_iter].func_text == t->mFunction[to_iter].func_text)
				{
					// no change
					t->mFunction[to_iter].dirty = false;
					t->mFunction[to_iter].fresh = false;
				}
				else
				{
					// yup, it changed
					t->mFunction[to_iter].func_text = f->mFunction[from_iter].func_text;
					t->mFunction[to_iter].dirty = true;
					t->mFunction[to_iter].fresh = false;
					t->mFunction[to_iter].cache = 0.0;
				}
				to_iter = to_count+1;
			}
		}

		// new
		if (addit)
		{
			func.func = f->mFunction[from_iter].func;
			func.func_text = f->mFunction[from_iter].func_text;
			func.dirty = true;
			func.fresh = false;
			func.cache = 0.0;
			t->mFunction.Add(func);
		}
	}

	// Copy numbers
	t->dBankroll = f->dBankroll;
	t->dDefcon = f->dDefcon;
	t->dRake = f->dRake;
	t->dNit = f->dNit;

	// Copy hand lists
	t->mHandList.RemoveAll();
	from_count = (int) f->mHandList.GetSize();
	for (from_iter=0; from_iter<from_count; from_iter++)
	{
		list.list = f->mHandList[from_iter].list;
		list.list_text = f->mHandList[from_iter].list_text;
		t->mHandList.Add(list);
	}

	__SEH_LOGFATAL("CGlobal::CopyFormula :\n");
}

// returns true for successful parse of all trees, false otherwise
bool CGlobal::ParseAllFormula(HWND hwnd, SFormula *f)
{
	__SEH_HEADER

	sData			data;
	data.all_parsed = true;
	data.calling_hwnd = hwnd;
	data.f = f;

	CUPDialog		dlg_progress(hwnd, parse_loop, &data, "Please wait", false);

	// Set busy cursor
	//EnterCriticalSection(&cs_global);
	//_m_wait_cursor = true;
	//LeaveCriticalSection(&cs_global);

	//CWnd::FromHandle(hwnd)->BeginWaitCursor();
	if (m_formulaScintillaDlg) m_formulaScintillaDlg->BeginWaitCursor();

	// Start parsing
	dlg_progress.DoModal();

	// Unset busy cursor
	//CWnd::FromHandle(hwnd)->EndWaitCursor();
	if (m_formulaScintillaDlg) m_formulaScintillaDlg->EndWaitCursor();

	//EnterCriticalSection(&cs_global);
	//_m_wait_cursor = false;
	//LeaveCriticalSection(&cs_global);

	return data.all_parsed;

	__SEH_LOGFATAL("CGlobal::ParseAllFormula :\n");
}

bool parse_loop(const CUPDUPDATA* pCUPDUPData)
{
	__SEH_HEADER

	int				N, i, j;
	CString			s;
	bool			result;
	int				stopchar;
	int				c, linenum, colnum;
	LARGE_INTEGER	bcount, ecount, lFrequency;
	double			time_elapsed;
	sData			*data = (sData*) (pCUPDUPData->GetAppData());

	pCUPDUPData->SetProgress("", 0, false);

	// init timer
	QueryPerformanceCounter(&bcount);
	QueryPerformanceFrequency(&lFrequency);

	N = (int) data->f->mFunction.GetSize();

	for (i=0; i<N; i++)
	{

		// Update progress dialog
		s.Format("Parsing formula set %s : %.0f%%", data->f->mFunction[i].func.GetString(), (double) i / (double) N * 100.0);
		QueryPerformanceCounter(&ecount);
		time_elapsed = ((double) (ecount.LowPart - bcount.LowPart))/((double) lFrequency.LowPart);
		pCUPDUPData->SetProgress(s.GetString(), (int) ((double) i / (double) N * 100.0), time_elapsed>=3.0);

		// Parse it if it is dirty, and not notes, dll or f$debug
		if (data->f->mFunction[i].dirty == true &&
				data->f->mFunction[i].func != "notes" &&
				data->f->mFunction[i].func != "dll" &&
				data->f->mFunction[i].func != "f$debug")
		{
			p_global->parse_symbol_formula = data->f;
			p_global->parse_symbol_stop_strs.RemoveAll();

			EnterCriticalSection(&cs_parse);
			result = parse(&data->f->mFunction[i].func_text, &data->f->mFunction[i].tpi, &stopchar);
			LeaveCriticalSection(&cs_parse);

			if (!result)
			{
				linenum = colnum = 1;
				for (c=0; c<stopchar; c++)
				{
					if (data->f->mFunction[i].func_text.Mid(c, 1)=="\n")
					{
						linenum++;
						colnum = 1;
					}
					else
					{
						colnum++;
					}
				}
				s.Format("Error in parse of %s\nLine: %d  Character: %d\n\nNear:\n \"%s\"",
						 data->f->mFunction[i].func.GetString(),
						 linenum, colnum,
						 data->f->mFunction[i].func_text.Mid(stopchar, 40).GetString());
				MessageBox(data->calling_hwnd, s, "PARSE ERROR", MB_OK);
				data->all_parsed = false;
			}

			else if (p_global->parse_symbol_stop_strs.GetSize() != 0)
			{
				s.Format("Error in parse of %s\n\nInvalid symbols:\n",
						 data->f->mFunction[i].func.GetString());
				for (j=0; j<p_global->parse_symbol_stop_strs.GetSize(); j++)
				{
					s.Append("   ");
					s.Append(p_global->parse_symbol_stop_strs[j].c_str());
					s.Append("\n");
				}
				MessageBox(data->calling_hwnd, s, "PARSE ERROR", MB_OK);
				data->all_parsed = false;
			}

			else
			{
				data->f->mFunction[i].dirty = false;
			}
		}
	}

	pCUPDUPData->SetProgress("", 100, true);

	return true;

	__SEH_LOGFATAL("::parse_loop :\n");
}

int cardIdentHelper(const char c)
{
	__SEH_HEADER

	if (c>='2' && c<='9') {
		return c - '0' - 2;
	}
	else if (c=='T') {
		return 8;
	}
	else if (c=='J') {
		return 9;
	}
	else if (c=='Q') {
		return 10;
	}
	else if (c=='K') {
		return 11;
	}
	else if (c=='A') {
		return 12;
	}
	return -1;

	__SEH_LOGFATAL("cardIdentHelper :\n");
}

void CGlobal::ParseHandList(CString &list_text, bool inlist[13][13])
{
	__SEH_HEADER

	for (int i=0; i<=12; i++)
	{
		for (int j=0; j<=12; j++)
		{
			inlist[i][j] = false;
		}
	}

	int	token_card0_rank, token_card1_rank, temp_rank;

	CString list = list_text.MakeUpper();
	const char *pStr = list.GetString();
	while (*pStr)
	{
		if (pStr[0] == '/' && pStr[1] == '/')
		{
			int index = CString(pStr).FindOneOf("\r\n");
			if (index == -1) break;
			pStr += index;
		}

		token_card0_rank = cardIdentHelper(*pStr++);

		if (token_card0_rank == -1)
			continue;

		token_card1_rank = cardIdentHelper(*pStr++);

		if (token_card0_rank == -1)
			continue;

		// make card0 have the higher rank
		if (token_card0_rank < token_card1_rank)
		{
			temp_rank = token_card0_rank;
			token_card0_rank = token_card1_rank;
			token_card1_rank = temp_rank;
		}

		if (*pStr == 'S') // suited
		{
			inlist[token_card0_rank][token_card1_rank] = true;
			pStr++;
		}
		else  // offsuit or pair
		{
			inlist[token_card1_rank][token_card0_rank] = true;
		}
	}

	__SEH_LOGFATAL("ParseHandList :\n");
}

void CGlobal::CreateHandListMatrices(SFormula *f)
{
	__SEH_HEADER

	int			listnum, i, j, N;
	CString		token;

	for (listnum=0; listnum<MAX_HAND_LISTS; listnum++)
	{
		for (i=0; i<=12; i++)
		{
			for (j=0; j<=12; j++)
			{
				f->inlist[listnum][i][j] = false;
			}
		}
	}

	N = (int) f->mHandList.GetSize();
	for (i=0; i<N; i++)
	{
		listnum = atoi(f->mHandList[i].list.Mid(4).GetString());
		if(listnum>=MAX_HAND_LISTS)continue; // matrix 2008-05-14
		ParseHandList(f->mHandList[i].list_text, f->inlist[listnum]);
	}

	__SEH_LOGFATAL("CGlobal::CreateHandListMatrices :\n");
}

void CGlobal::CaptureState(const char *title)
{
	__SEH_HEADER

	int					i, j;
	bool				playing = true;
	unsigned char		card;

	// figure out if I am playing
	int sym_chair = (int) p_symbols->sym()->chair;
	if (!p_symbols->user_chair_confirmed())
	{
		playing = false;
	}
	else if (p_scraper->card_player(sym_chair, 0) == CARD_BACK || 
			 p_scraper->card_player(sym_chair, 1) == CARD_BACK ||
			 p_scraper->card_player(sym_chair, 0) == CARD_NOCARD || 
			 p_scraper->card_player(sym_chair, 1) == CARD_NOCARD)
	{
		playing = false;
	}

	// When using MM, grab i5state for PT network
	bool sym_ismanual = (bool) p_symbols->sym()->ismanual;
	if (sym_ismanual)
	{
		mm_network = p_scraper->button_state(5);
	}

	// Poker window title
	strncpy(state[state_index&0xff].m_title, title, 64);
	state[state_index&0xff].m_title[63] = '\0';

	// Pot information
	for (i=0; i<=9; i++)
		state[state_index&0xff].m_pot[i] = p_scraper->pot(i);

	// Common cards
	for (i=0; i<=4; i++)
	{
		if (p_scraper->card_common(i) == CARD_BACK)
		{
			card = WH_CARDBACK;
		}
		else if (p_scraper->card_common(i) == CARD_NOCARD)
		{
			card = WH_NOCARD;
		}
		else
		{
			card = ((StdDeck_RANK(p_scraper->card_common(i))+2)<<4) |
					(StdDeck_SUIT(p_scraper->card_common(i)) == StdDeck_Suit_CLUBS ? WH_SUIT_CLUBS :
					 StdDeck_SUIT(p_scraper->card_common(i)) == StdDeck_Suit_DIAMONDS ? WH_SUIT_DIAMONDS :
					 StdDeck_SUIT(p_scraper->card_common(i)) == StdDeck_Suit_HEARTS ? WH_SUIT_HEARTS :
					 StdDeck_SUIT(p_scraper->card_common(i)) == StdDeck_Suit_SPADES ? WH_SUIT_SPADES : 0) ;
		}

		state[state_index&0xff].m_cards[i] = card;
	}

	// playing, posting, dealerchair
	int sym_dealerchair = (int) p_symbols->sym()->dealerchair;
	bool sym_isautopost = (bool) p_symbols->sym()->isautopost;
	state[state_index&0xff].m_is_playing = playing;
	state[state_index&0xff].m_is_posting = sym_isautopost;
	state[state_index&0xff].m_fillerbits = 0;
	state[state_index&0xff].m_fillerbyte = 0;
	state[state_index&0xff].m_dealer_chair = sym_dealerchair;

	// loop through all 10 player chairs
	for (i=0; i<=9; i++)
	{

		// player name, balance, currentbet
		strcpy(state[state_index&0xff].m_player[i].m_name, p_scraper->player_name(i).GetString());
		state[state_index&0xff].m_player[i].m_balance = p_symbols->sym()->balance[i];
		state[state_index&0xff].m_player[i].m_currentbet = p_symbols->sym()->currentbet[i];

		// player cards
		for (j=0; j<=1; j++)
		{
			if (p_scraper->card_player(i, j) == CARD_BACK)
			{
				card = WH_CARDBACK;
			}
			else if (p_scraper->card_player(i, j) == CARD_NOCARD)
			{
				card = WH_NOCARD;
			}
			else
			{
				card = ((StdDeck_RANK(p_scraper->card_player(i, j))+2)<<4) |
					   (StdDeck_SUIT(p_scraper->card_player(i, j)) == StdDeck_Suit_CLUBS ? WH_SUIT_CLUBS :
						StdDeck_SUIT(p_scraper->card_player(i, j)) == StdDeck_Suit_DIAMONDS ? WH_SUIT_DIAMONDS :
						StdDeck_SUIT(p_scraper->card_player(i, j)) == StdDeck_Suit_HEARTS ? WH_SUIT_HEARTS :
						StdDeck_SUIT(p_scraper->card_player(i, j)) == StdDeck_Suit_SPADES ? WH_SUIT_SPADES : 0) ;
			}

			state[state_index&0xff].m_player[i].m_cards[j] = card;
		}

		// player name known, balance known
		state[state_index&0xff].m_player[i].m_name_known = p_scraper->name_good_scrape(i) ? 1 : 0;
		state[state_index&0xff].m_player[i].m_balance_known = p_scraper->balance_good_scrape(i) ? 1 : 0;
		state[state_index&0xff].m_player[i].m_fillerbits = 0;
		state[state_index&0xff].m_player[i].m_fillerbyte = 0;
	}

	state_index++;

	__SEH_LOGFATAL("CGlobal::CaptureState :\n");
}


void CGlobal::ClearR$Indices(void)
{
	__SEH_HEADER

	int i,j,k;


	for (i=0; i<=9; i++)
	{
		// common card info
		trans.map.r$c0cardfaceX_index[i] = -1;
		trans.map.r$c0cardfaceXrank_index[i] = -1;
		trans.map.r$c0cardfaceXsuit_index[i] = -1;

		// pots
		for (j=0; j<=9; j++)
			for (k=0; k<=9; k++)
				trans.map.r$c0potXchipYZ_index[i][j][k] = -1;
	}

	for (i=0; i<=9; i++)
	{
		// player info
		trans.map.r$pXcardfaceY_index[i][0] = -1;
		trans.map.r$pXcardfaceY_index[i][1] = -1;
		trans.map.r$pXcardfaceYrank_index[i][0] = -1;
		trans.map.r$pXcardfaceYrank_index[i][1] = -1;
		trans.map.r$pXcardfaceYsuit_index[i][0] = -1;
		trans.map.r$pXcardfaceYsuit_index[i][1] = -1;
		trans.map.r$uXcardfaceY_index[i][0] = -1;
		trans.map.r$uXcardfaceY_index[i][1] = -1;
		trans.map.r$pXcardback_index[i] = -1;
		trans.map.r$pXseated_index[i] = -1;
		trans.map.r$uXseated_index[i] = -1;
		trans.map.r$pXactive_index[i] = -1;
		trans.map.r$uXactive_index[i] = -1;
		trans.map.r$pXdealer_index[i] = -1;
		trans.map.r$pXname_index[i] = -1;
		trans.map.r$uXname_index[i] = -1;
		trans.map.r$pXbalance_index[i] = -1;
		trans.map.r$uXbalance_index[i] = -1;
		trans.map.r$pXbet_index[i] = -1;

		for (j=0; j<=9; j++)
			for (k=0; k<=9; k++)
				trans.map.r$pXchip_index[i][j][k] = -1;

		// button info
		trans.map.r$iXbutton_index[i] = -1;
		trans.map.r$iXstate_index[i] = -1;
		trans.map.r$iXlabel_index[i] = -1;

		for (j=0; j<=9; j++)
			trans.map.r$iXlabelY_index[i][j] = -1;

		trans.map.r$iXslider_index[i] = -1;
		trans.map.r$iXhandle_index[i] = -1;
		trans.map.r$iXedit_index[i] = -1;
		trans.map.r$i86Xstate_index[i] = -1;
		trans.map.r$i86Xbutton_index[i] = -1;

		// limits
		trans.map.r$c0limitsX_index[i] = -1;
		trans.map.r$c0handnumberX_index[i] = -1;

		// pots
		trans.map.r$c0potX_index[i] = -1;
	}

	// limits
	trans.map.r$c0limits_index = -1;
	trans.map.r$c0istournament_index = -1;
	trans.map.r$c0sblind_index = -1;
	trans.map.r$c0bblind_index = -1;
	trans.map.r$c0bigbet_index = -1;
	trans.map.r$c0ante_index = -1;
	trans.map.r$c0handnumber_index = -1;

	// player info
	trans.map.r$uname_index = -1;
	trans.map.r$ubalance_index = -1;

	// button info
	trans.map.r$i86state_index = -1;
	trans.map.r$i86button_index = -1;

	__SEH_LOGFATAL("CGlobal::ClearR$Indices :\n");
}

void CGlobal::SaveR$Indices(void)
{
	__SEH_HEADER

	// r$tablepointX not indexed, as it is only used for finding tables on green circle-click, and
	//   this function is not called until a table has been selected by the user

	int		i, N;
	int		cardnum, seatnum, buttonnum, vertstride, horizstride, potnum, limitnum, handnum, indexnum;

	ClearR$Indices();

	N = trans.map.r$.GetCount();
	for (i=0; i<N; i++)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////
		// Player info
		// Player cards, r$pXcardfaceYrank, r$pXcardfaceYsuit
		if (trans.map.r$[i].name.Mid(0,1)=="p" &&
				trans.map.r$[i].name.Mid(2,8)=="cardface" &&
				trans.map.r$[i].name.Mid(11,4)=="rank")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			cardnum = trans.map.r$[i].name.GetString()[10] - '0';
			trans.map.r$pXcardfaceYrank_index[seatnum][cardnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="p" &&
				 trans.map.r$[i].name.Mid(2,8)=="cardface" &&
				 trans.map.r$[i].name.Mid(11,4)=="suit")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			cardnum = trans.map.r$[i].name.GetString()[10] - '0';
			trans.map.r$pXcardfaceYsuit_index[seatnum][cardnum] = i;
		}

		// Player cards, r$pXcardfaceY, r$uXcardfaceY
		else if (trans.map.r$[i].name.Mid(0,1)=="p" && trans.map.r$[i].name.Mid(2,8)=="cardface")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			cardnum = trans.map.r$[i].name.GetString()[10] - '0';
			trans.map.r$pXcardfaceY_index[seatnum][cardnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="u" && trans.map.r$[i].name.Mid(2,8)=="cardface")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			cardnum = trans.map.r$[i].name.GetString()[10] - '0';
			trans.map.r$uXcardfaceY_index[seatnum][cardnum] = i;
		}

		// Player card backs, r$pXcardback
		else if (trans.map.r$[i].name.Mid(0,1)=="p" && trans.map.r$[i].name.Mid(2,8)=="cardback")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$pXcardback_index[seatnum] = i;
		}

		// Seated, r$pXseated, r$uXseated
		else if (trans.map.r$[i].name.Mid(0,1)=="p" && trans.map.r$[i].name.Mid(2,6)=="seated")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$pXseated_index[seatnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="u" && trans.map.r$[i].name.Mid(2,6)=="seated")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$uXseated_index[seatnum] = i;
		}

		// Active, r$pXactive, r$uXactive
		else if (trans.map.r$[i].name.Mid(0,1)=="p" && trans.map.r$[i].name.Mid(2,6)=="active")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$pXactive_index[seatnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="u" && trans.map.r$[i].name.Mid(2,6)=="active")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$uXactive_index[seatnum] = i;
		}

		// Dealer button, r$pXdealer
		else if (trans.map.r$[i].name.Mid(0,1)=="p" && trans.map.r$[i].name.Mid(2,6)=="dealer")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$pXdealer_index[seatnum] = i;
		}

		// Player name r$pXname, r$uXname, r$uname
		else if (trans.map.r$[i].name.Mid(0,1)=="p" && trans.map.r$[i].name.Mid(2,4)=="name")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$pXname_index[seatnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="u" && trans.map.r$[i].name.Mid(2,4)=="name")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$uXname_index[seatnum] = i;
		}
		else if (trans.map.r$[i].name=="uname")
		{
			trans.map.r$uname_index = i;
		}

		// Player balance, r$pXbalance, r$uXbalance, r$ubalance
		else if (trans.map.r$[i].name.Mid(0,1)=="p" && trans.map.r$[i].name.Mid(2,7)=="balance")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$pXbalance_index[seatnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="u" && trans.map.r$[i].name.Mid(2,7)=="balance")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$uXbalance_index[seatnum] = i;
		}
		else if (trans.map.r$[i].name=="ubalance")
		{
			trans.map.r$ubalance_index = i;
		}

		// Player bet, r$pXbet, r$pXchipYZ
		else if (trans.map.r$[i].name.Mid(0,1)=="p" && trans.map.r$[i].name.Mid(2,3)=="bet")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$pXbet_index[seatnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="p" && trans.map.r$[i].name.Mid(2,4)=="chip")
		{
			seatnum = trans.map.r$[i].name.GetString()[1] - '0';
			vertstride = trans.map.r$[i].name.GetString()[6] - '0';
			horizstride = trans.map.r$[i].name.GetString()[7] - '0';
			trans.map.r$pXchip_index[seatnum][vertstride][horizstride] = i;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		// Common card info
		// Common cards, r$c0cardfaceXrank, r$c0cardfaceXsuit
		else if (trans.map.r$[i].name.Mid(0,10)=="c0cardface" && trans.map.r$[i].name.Mid(11,4)=="rank")
		{
			cardnum = trans.map.r$[i].name.GetString()[10] - '0';
			trans.map.r$c0cardfaceXrank_index[cardnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,10)=="c0cardface" && trans.map.r$[i].name.Mid(11,4)=="suit")
		{
			cardnum = trans.map.r$[i].name.GetString()[10] - '0';
			trans.map.r$c0cardfaceXsuit_index[cardnum] = i;
		}
		// Common cards, r$c0cardfaceX
		else if (trans.map.r$[i].name.Mid(0,10)=="c0cardface")
		{
			cardnum = trans.map.r$[i].name.GetString()[10] - '0';
			trans.map.r$c0cardfaceX_index[cardnum] = i;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		// Button info
		// r$iXbutton, r$iXstate, r$iXlabel, r$iXslider, r$iXhandle, r$iXedit, r$i86Xstate, r$i86Xbutton,
		// r$i86state, r$i86button
		else if (trans.map.r$[i].name.Mid(0,1)=="i" && trans.map.r$[i].name.Mid(2,6)=="button")
		{
			buttonnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$iXbutton_index[buttonnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="i" && trans.map.r$[i].name.Mid(2,5)=="state")
		{
			buttonnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$iXstate_index[buttonnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="i" && trans.map.r$[i].name.Mid(2,5)=="label" && trans.map.r$[i].name.GetLength()==7)
		{
			buttonnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$iXlabel_index[buttonnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="i" && trans.map.r$[i].name.Mid(2,5)=="label" && trans.map.r$[i].name.GetLength()==8)
		{
			buttonnum = trans.map.r$[i].name.GetString()[1] - '0';
			indexnum = trans.map.r$[i].name.GetString()[7] - '0';
			trans.map.r$iXlabelY_index[buttonnum][indexnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="i" && trans.map.r$[i].name.Mid(2,6)=="slider")
		{
			buttonnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$iXslider_index[buttonnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="i" && trans.map.r$[i].name.Mid(2,6)=="handle")
		{
			buttonnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$iXhandle_index[buttonnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,1)=="i" && trans.map.r$[i].name.Mid(2,4)=="edit")
		{
			buttonnum = trans.map.r$[i].name.GetString()[1] - '0';
			trans.map.r$iXedit_index[buttonnum] = i;
		}
		else if (trans.map.r$[i].name=="i86button")
		{
			trans.map.r$i86button_index = i;
		}
		else if (trans.map.r$[i].name.Mid(0,3)=="i86" && trans.map.r$[i].name.Mid(4,6)=="button")
		{
			buttonnum = trans.map.r$[i].name.GetString()[3] - '0';
			trans.map.r$i86Xbutton_index[buttonnum] = i;
		}
		else if (trans.map.r$[i].name=="i86state")
		{
			trans.map.r$i86state_index = i;
		}
		else if (trans.map.r$[i].name.Mid(0,3)=="i86" && trans.map.r$[i].name.Mid(4,5)=="state")
		{
			buttonnum = trans.map.r$[i].name.GetString()[3] - '0';
			trans.map.r$i86Xstate_index[buttonnum] = i;
		}


		///////////////////////////////////////////////////////////////////////////////////////////////
		// Pots
		// Pots, r$c0potX, r$c0potXchipYZ
		else if (trans.map.r$[i].name.Mid(0,5)=="c0pot" && trans.map.r$[i].name.Find("chip")==-1)
		{
			potnum = trans.map.r$[i].name.GetString()[5] - '0';
			trans.map.r$c0potX_index[potnum] = i;
		}
		else if (trans.map.r$[i].name.Mid(0,5)=="c0pot" && trans.map.r$[i].name.Mid(6,4)=="chip")
		{
			potnum = trans.map.r$[i].name.GetString()[5] - '0';
			vertstride = trans.map.r$[i].name.GetString()[10] - '0';
			horizstride = trans.map.r$[i].name.GetString()[11] - '0';
			trans.map.r$c0potXchipYZ_index[potnum][vertstride][horizstride] = i;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		// Limits
		// r$c0limits, r$c0limitsX
		else if (trans.map.r$[i].name=="c0limits")
		{
			trans.map.r$c0limits_index = i;
		}
		else if (trans.map.r$[i].name.Mid(0,8)=="c0limits")
		{
			limitnum = trans.map.r$[i].name.GetString()[8] - '0';
			trans.map.r$c0limitsX_index[limitnum] = i;
		}

		// r$c0istournament
		else if (trans.map.r$[i].name == "c0istournament")
		{
			trans.map.r$c0istournament_index = i;
		}

		// r$c0sblind, r$c0bblind, r$c0bigbet, r$c0ante, r$c0handnumberX, r$c0handnumber
		else if (trans.map.r$[i].name == "c0sblind")
		{
			trans.map.r$c0sblind_index = i;
		}
		else if (trans.map.r$[i].name == "c0bblind")
		{
			trans.map.r$c0bblind_index = i;
		}
		else if (trans.map.r$[i].name == "c0bigbet")
		{
			trans.map.r$c0bigbet_index = i;
		}
		else if (trans.map.r$[i].name == "c0ante")
		{
			trans.map.r$c0ante_index = i;
		}
		else if (trans.map.r$[i].name=="c0handnumber")
		{
			trans.map.r$c0handnumber_index = i;
		}
		else if (trans.map.r$[i].name.Mid(0,12)=="c0handnumber")
		{
			handnum = trans.map.r$[i].name.GetString()[12] - '0';
			trans.map.r$c0handnumberX_index[handnum] = i;
		}

	}

	__SEH_LOGFATAL("CGlobal::SaveR$Indices :\n");
}

void CGlobal::SaveS$Indices(void)
{
	__SEH_HEADER

	// s$titletextX, s$!titletextX not indexed, as it is only used for finding tables on green circle-click, and
	//   this function is not called until a table has been selected by the user/*
	// s$hXtype are not indexed, as those records are ignored in OH

	int		i, num;

	// Clear 'em first
	for (i=0; i<=9; i++)
		trans.map.s$ttlimitsX_index[i] = -1;

	trans.map.s$ttlimits_index = -1;

	for (i=0; i<(int) trans.map.s$.GetSize(); i++)
	{

		// s$ttlimits, s$ttlimitsX
		if (trans.map.s$[i].name=="ttlimits")
		{
			trans.map.s$ttlimits_index = i;
		}
		else if (trans.map.s$[i].name.Mid(0,8)=="ttlimits")
		{
			num = trans.map.s$[i].name.GetString()[8] - '0';
			trans.map.s$ttlimitsX_index[num] = i;
		}
		else if (trans.map.s$[i].name=="c0limits")
		{
			trans.map.s$c0limits_index = i;
		}
		else if (trans.map.s$[i].name.Mid(0,8)=="c0limits")
		{
			num = trans.map.s$[i].name.GetString()[8] - '0';
			trans.map.s$c0limitsX_index[num] = i;
		}
	}

	__SEH_LOGFATAL("CGlobal::SaveS$Indices :\n");
}

void CGlobal::SaveS$Strings(void)
{
	__SEH_HEADER

	// s$reseller and s$mechanic are not saved, as they are only comments and not used in OH for any purpose

	int		i;

	// Clear 'em first
	trans.map.num_chairs = 0;
	trans.map.swagtextmethod = 0;
	trans.map.potmethod = 0;
	trans.map.activemethod = 0;
	trans.map.sitename = "";
	trans.map.network = "";
	trans.map.chairconfig = "";
	for (i=0; i<=3; i++)
		trans.map.ttype[i] = "";

	for (i=0; i<(int) trans.map.s$.GetSize(); i++)
	{
		if (trans.map.s$[i].name == "nchairs")
			trans.map.num_chairs = strtoul(trans.map.s$[i].text.GetString(), NULL, 10);

		if (trans.map.s$[i].name == "swagtextmethod")
			trans.map.swagtextmethod = strtoul(trans.map.s$[i].text.GetString(), NULL, 10);

		if (trans.map.s$[i].name == "potmethod")
			trans.map.potmethod = strtoul(trans.map.s$[i].text.GetString(), NULL, 10);

		if (trans.map.s$[i].name == "activemethod")
			trans.map.activemethod = strtoul(trans.map.s$[i].text.GetString(), NULL, 10);

		if (trans.map.s$[i].name == "sitename")
			trans.map.sitename = trans.map.s$[i].text;

		if (trans.map.s$[i].name == "network")
			trans.map.network = trans.map.s$[i].text;

		if (trans.map.s$[i].name == "chairconfig")
			trans.map.chairconfig = trans.map.s$[i].text;

		if (trans.map.s$[i].name == "t0type")
			trans.map.ttype[0] = trans.map.s$[i].text;

		if (trans.map.s$[i].name == "t1type")
			trans.map.ttype[1] = trans.map.s$[i].text;

		if (trans.map.s$[i].name == "t2type")
			trans.map.ttype[2] = trans.map.s$[i].text;

		if (trans.map.s$[i].name == "t3type")
			trans.map.ttype[3] = trans.map.s$[i].text;

	}

	__SEH_LOGFATAL("CGlobal::SaveS$Strings :\n");
}


void CGlobal::CreateReplayFrame(void)
{
	__SEH_HEADER

	CString			path, filename, text, fcra_seen;
	FILE			*fp;
	int				i;
	time_t			ltime;
	struct tm		*now_time;
	char			now_time_str[100];
	ULARGE_INTEGER	free_bytes_for_user_on_disk, 
					total_bytes_on_disk, 
					free_bytes_total_on_disk;
	int				e = SUCCESS;

	//  Sanity check: Enough disk-space for replay frame?	
	GetDiskFreeSpaceEx(
		_startup_path,				  //  Directory on disk of interest
		&free_bytes_for_user_on_disk,  
		&total_bytes_on_disk,	
		&free_bytes_total_on_disk);
	if (free_bytes_for_user_on_disk.QuadPart < FREE_SPACE_NEEDED_FOR_REPLAYFRAME) 
	{
		MessageBox(NULL, "Not enough disk space to create replay-frame.", 
				"ERROR", 0);	
		return;
	}

	// Get current time
	time(&ltime);
	now_time = localtime(&ltime);
	strftime(now_time_str, 100, "%Y-%m-%d %H:%M:%S", now_time);

	// Create replay/session dir if it does not exist
	path.Format("%s\\replay\\session_%lu\\", _startup_path, _session_id);
	if (GetFileAttributes(path.GetString()) == INVALID_FILE_ATTRIBUTES)
		SHCreateDirectoryEx(NULL, path.GetString(), NULL);

	// Get exclusive access to CScraper and CSymbols variables
	// (Wait for scrape/symbol cycle to finish before saving frame)
	EnterCriticalSection(&p_heartbeat_thread->cs_update_in_progress);

		// Create bitmap file
		path.Format("%s\\replay\\session_%lu\\frame%03d.bmp", _startup_path, _session_id, _next_replay_frame);
		CreateBMPFile(path.GetString(), p_scraper->entire_window_cur());

		// Create HTML file
		path.Format("%s\\replay\\session_%lu\\frame%03d.htm", _startup_path, _session_id, _next_replay_frame);
		fp = fopen(path.GetString(), "w");

		fprintf(fp, p_scraper->title());
		fprintf(fp, "\n");
		fprintf(fp, "<html>\n");
		fprintf(fp, "<style>\n");
		fprintf(fp, "td {text-align:right;}\n");
		fprintf(fp, "</style>\n");
		fprintf(fp, "<body>\n");
		fprintf(fp, "<font face=courier>\n");
		fprintf(fp, "<img src=\"frame%03d.bmp\">\n", _next_replay_frame);
		fprintf(fp, "<br>\n");
		fprintf(fp, "<a href=\"frame%03d.htm\">PREV</a>\n",
				_next_replay_frame-1 >= 0 ? _next_replay_frame-1 : preferences.replay_max_frames);
		fprintf(fp, "<a href=\"frame%03d.htm\">NEXT</a>\n",
				_next_replay_frame+1 < preferences.replay_max_frames ? _next_replay_frame+1 : 0);
		fprintf(fp, " [%lu.%03d] [%s]<br>\n", _session_id, _next_replay_frame, now_time_str);
		fprintf(fp, "<br>\n");
		fprintf(fp, "<table>\n");
		fprintf(fp, "<tr>\n");
		fprintf(fp, "<td>\n");

		// Table for: SFABD, hand, bet, balance, name
		fprintf(fp, "<table border=4 cellpadding=1 cellspacing=1>\n");
		fprintf(fp, "<tr>\n");
		fprintf(fp, "<th>#</th>\n");
		fprintf(fp, "<th>SFABDP</th>\n");  //seated, friend, active, button, dealt, playing
		fprintf(fp, "<th>hand</th>\n");
		fprintf(fp, "<th>bet</th>\n");
		fprintf(fp, "<th>balance</th>\n");
		fprintf(fp, "<th>name</th>\n");
		fprintf(fp, "</tr>\n");
		for (i=0; i<trans.map.num_chairs; i++)
		{

			fprintf(fp, "<tr>\n");
			fprintf(fp, "<td>%d</td>", i);  // #
			text.Format("%s%s%s%s%s%s",
				(int) (p_symbols->sym()->playersseatedbits) & (1<<i) ? "s" : "-",
				p_symbols->sym()->userchair == i ? "f" : "-",
				(int) (p_symbols->sym()->playersactivebits) & (1<<i) ? "a" : "-",
				p_symbols->sym()->dealerchair== i ? "b" : "-",
				(int) (p_symbols->sym()->playersdealtbits) & (1<<i) ? "d" : "-",
				(int) (p_symbols->sym()->playersplayingbits) & (1<<i) ? "p" : "-");
			fprintf(fp, "<td>%s</td>", text.GetString());  // SFABDP
			fprintf(fp, "<td>%s%s</td>",
					GetCardHtml(p_scraper->card_player(i, 0)),
					GetCardHtml(p_scraper->card_player(i, 1)) );  // hand
			fprintf(fp, "<td>%11.2f</td>", p_scraper->player_bet(i));  // bet
			fprintf(fp, "<td>%11.2f</td>", p_scraper->player_balance(i));  // balance
			fprintf(fp, "<td>%-15s</td>\n", p_scraper->player_name(i).GetString());  // name
			fprintf(fp, "</tr>\n");
		}
		fprintf(fp, "</table>\n");
		fprintf(fp, "</td>\n");

		// Table for: FCRA
		fprintf(fp, "<td>\n");
		fprintf(fp, "<table align=center border=4 cellpadding=1 cellspacing=1>\n");
		fprintf(fp, "<tr><th>FCRA</th></tr>\n");
		fprintf(fp, "<tr>\n");
		int sym_myturnbits = (int) p_symbols->sym()->myturnbits;
		fcra_seen.Format("%s%s%s%s",
			sym_myturnbits&0x1 ? "F" : ".",
			sym_myturnbits&0x2 ? "C" : ".",
			sym_myturnbits&0x4 ? "R" : ".",
			sym_myturnbits&0x8 ? "A" : ".");

		fprintf(fp, "<td>%s</td>\n", fcra_seen.GetString());
		fprintf(fp, "</tr>\n");
		fprintf(fp, "</table>\n");

		// Table for: sb, bb, BB
		fprintf(fp, "<table align=center border=4 cellpadding=1 cellspacing=1>\n");
		fprintf(fp, "<tr><th>sb</th><td>%11.2f</td></tr>\n", p_symbols->sym()->sblind);
		fprintf(fp, "<tr><th>bb</th><td>%11.2f</td></tr>\n", p_symbols->sym()->bblind);
		fprintf(fp, "<tr><th>BB</th><td>%11.2f</td></tr>\n", p_symbols->bigbet());
		fprintf(fp, "</table>\n");

		// Table for: common cards
		fprintf(fp, "<table align=center border=4 cellpadding=1 cellspacing=1>\n");
		fprintf(fp, "<tr><th>commoncard</th></tr>\n");
		fprintf(fp, "<tr>\n");
		fprintf(fp, "<td>%s%s%s%s%s</td>\n",
				GetCardHtml(p_scraper->card_common(0)),
				GetCardHtml(p_scraper->card_common(1)),
				GetCardHtml(p_scraper->card_common(2)),
				GetCardHtml(p_scraper->card_common(3)),
				GetCardHtml(p_scraper->card_common(4)) );
		fprintf(fp, "</tr>\n");
		fprintf(fp, "</table>\n");

		// Table for: pots
		fprintf(fp, "<table align=center border=4 cellpadding=1 cellspacing=1>\n");
		fprintf(fp, "<tr><th>#</th><th>pot</th></tr>\n");
		fprintf(fp, "<tr>\n");
		fprintf(fp, "<td>0</td><td>%11.2f</td>\n", p_scraper->pot(0));
		fprintf(fp, "</tr>\n");
		for (i=1; i<=9; i++)
		{
			if (p_scraper->pot(i))
			{
				fprintf(fp, "<tr>\n");
				fprintf(fp, "<td>%d</td><td>%11.2f</td>\n", i, p_scraper->pot(i));
				fprintf(fp, "</tr>\n");
			}
			else
			{
				i = 11;
			}
		}
		fprintf(fp, "</table>\n");

		fprintf(fp, "</td>\n");
		fprintf(fp, "</tr>\n");
		fprintf(fp, "</table>\n");
		fprintf(fp, "</body></html>\n");

		fclose(fp);

	LeaveCriticalSection(&p_heartbeat_thread->cs_update_in_progress);

	// Increment counter
	EnterCriticalSection(&cs_global);
		_next_replay_frame++;
		if (_next_replay_frame >= preferences.replay_max_frames)
			_next_replay_frame = 0;
	LeaveCriticalSection(&cs_global);

	__SEH_LOGFATAL("CGlobal::CreateReplayFrame :\n");
}

CString CGlobal::GetCardHtml(unsigned int card)
{
	__SEH_HEADER

	CString suit, color, rank, final;

	suit =	card == CARD_BACK ? "*" :
		   card == CARD_NOCARD ? "&nbsp" :
		   StdDeck_SUIT(card) == Suit_CLUBS ? "&clubs;" :
		   StdDeck_SUIT(card) == Suit_DIAMONDS ? "&diams;" :
		   StdDeck_SUIT(card) == Suit_HEARTS ? "&hearts;" :
		   StdDeck_SUIT(card) == Suit_SPADES ? "&spades;" :
		   "&nbsp";

	color = card == CARD_BACK ? "black" :
			card == CARD_NOCARD ? "black" :
			StdDeck_SUIT(card) == Suit_CLUBS ? "green" :
			StdDeck_SUIT(card) == Suit_DIAMONDS ? "blue" :
			StdDeck_SUIT(card) == Suit_HEARTS ? "red" :
			StdDeck_SUIT(card) == Suit_SPADES ? "black" :
			"black";

	rank =	card == CARD_BACK ? "*" :
		   card == CARD_NOCARD ? " " :
		   StdDeck_RANK(card) == Rank_ACE ? "A" :
		   StdDeck_RANK(card) == Rank_KING ? "K" :
		   StdDeck_RANK(card) == Rank_QUEEN ? "Q" :
		   StdDeck_RANK(card) == Rank_JACK ? "J" :
		   StdDeck_RANK(card) == Rank_TEN ? "T" :
		   StdDeck_RANK(card) == Rank_9 ? "9" :
		   StdDeck_RANK(card) == Rank_8 ? "8" :
		   StdDeck_RANK(card) == Rank_7 ? "7" :
		   StdDeck_RANK(card) == Rank_6 ? "6" :
		   StdDeck_RANK(card) == Rank_5 ? "5" :
		   StdDeck_RANK(card) == Rank_4 ? "4" :
		   StdDeck_RANK(card) == Rank_3 ? "3" :
		   StdDeck_RANK(card) == Rank_2 ? "2" :
		   "&nbsp";

	final.Format("<font color=%s>%s%s</font>", color, rank, suit);

	return final;

	__SEH_LOGFATAL("CGlobal::GetCardHtml :\n");
}