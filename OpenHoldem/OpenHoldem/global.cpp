#include "StdAfx.h"
#include "debug.h"
#include "resource.h"
#include "grammar.h"
#include "registry.h"
#include "symbols.h"
#include "scraper.h"
#include "tablemap.h"
#include "OpenHoldem.h"
#include "MainFrm.h"
#include "DialogFormulaScintilla.h"
#include "global.h"
#include "threads.h"

class CGlobal	global;

CGlobal::CGlobal(void) 
{
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif

#ifdef SEH_ENABLE
	try {
#endif
		int			i, j;
		Registry	reg;
		FILE		*fp;

		//Initialisation for prwin handrank table
		extern char *prwhandrank169[169];
		extern int pair2ranko[170], pair2ranks[170];
		extern char ctonum[14];
		char *ptr;

		ClearFormula(&formula);
		formula_name = "";

		clear_r$indexes();

		tablemap.num_chairs = 10;

		attached_hwnd = NULL;

		for (i=0; i<10; i++) {
			flags[i] = false;
		}

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
		auto_autoplay_pressed = false;

		// dll extension
		preferences.dll_always_send_state = reg.dll_always_send_state;
		preferences.load_dll_on_startup = reg.load_dll_on_startup;
		preferences.dll_name = reg.dll_name;

		// scraper
		preferences.scrape_delay = reg.scrape_delay;
		preferences.name_scrapes = reg.name_scrapes;
		preferences.balance_scrapes = reg.balance_scrapes;

		// symbols
		preferences.av_time = reg.avtime;
		preferences.handrank_value = reg.handrank_value;
		preferences.disable_caching = reg.disable_caching;

		// poker tracker
		// loaded in PT class

		// icm
		preferences.icm_prize1 = reg.icm_prize1;
		preferences.icm_prize2 = reg.icm_prize2;
		preferences.icm_prize3 = reg.icm_prize3;
		preferences.icm_prize4 = reg.icm_prize4;

		// Replay frames
		preferences.replay_record = reg.replay_record;
		preferences.replay_record_every_change = reg.replay_record_every_change;
		preferences.replay_max_frames = reg.replay_max_frames;

		// Check for versus.bin
		if ((fp = fopen("versus.bin", "rb"))==NULL) 
		{
			MessageBox(NULL, "Could not open versus.bin.\nVersus functions will be disabled.\n", "Versus Error", MB_OK);
			versus_enabled = false;
		}
		else 
		{
			versus_enabled = true;
		}

		state_index = 0;

		//Initialise the handrank tables used by prwin
		for(i=0;i<169;i++)
		{
			ptr=prwhandrank169[i];
			j=(strchr(ctonum,*ptr)-ctonum)*13 + (strchr(ctonum,*(ptr+1))-ctonum);
			if(*(ptr+2)=='s')pair2ranks[j]=i+1;
				else pair2ranko[j]=i+1;
			//end of handrank initialisation
		}

		m_WaitCursor = false;

		replay_recorded_this_turn = false;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGlobal::Constructor : \n"); 
		throw;
	}
#endif
}

CGlobal::~CGlobal(void) 
{
#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGlobal::Destructor : \n"); 
		throw;
	}
#endif
}

void CGlobal::ClearFormula(SFormula *f) 
{
#ifdef SEH_ENABLE
	try {
#endif
		f->dBankroll = f->dDefcon = f->dRake = f->dNit = 0.0;
		f->mHandList.RemoveAll();
		f->mFunction.RemoveAll();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGlobal::ClearFormula : \n"); 
		throw;
	}
#endif
}


void CGlobal::CopyFormula(SFormula *f, SFormula *t) 
{
#ifdef SEH_ENABLE
	try {
#endif
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


#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGlobal::CopyFormula :\n"); 
		throw;
	}
#endif
}

// returns true for successful parse of all trees, false otherwise
bool CGlobal::ParseAllFormula(HWND hwnd, SFormula *f) 
{
#ifdef SEH_ENABLE
	try {
#endif
		sData			data;
		CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

		data.all_parsed = true;
		data.calling_hwnd = hwnd;
		data.f = f;

		CUPDialog		dlg_progress(hwnd, parse_loop, &data, "Please wait", false);

		// Set busy cursor
		global.m_WaitCursor = true;
		pMyMainWnd->BeginWaitCursor();
		if (m_formulaScintillaDlg) m_formulaScintillaDlg->BeginWaitCursor();

		// Start parsing
		dlg_progress.DoModal();

		// Unset busy cursor	
		pMyMainWnd->EndWaitCursor();
		if (m_formulaScintillaDlg) m_formulaScintillaDlg->EndWaitCursor();
		global.m_WaitCursor = false;

		return data.all_parsed;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGlobal::ParseAllFormula :\n"); 
		throw;
	}
#endif
}

bool parse_loop(const CUPDUPDATA* pCUPDUPData) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int				N, i;
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

				result = parse(&data->f->mFunction[i].func_text, &data->f->mFunction[i].tpi, &stopchar); 
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
				else 
				{
					data->f->mFunction[i].dirty = false;
				}
			}
		}

		return true;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("::parse_loop :\n"); 
		throw;
	}
#endif
}

int cardIdentHelper(const char c)
{
	if (c>='2' && c<='9') { return c - '0' - 2; }
	else if (c=='T') { return 8; }
	else if (c=='J') { return 9; }
	else if (c=='Q') { return 10; }
	else if (c=='K') { return 11; }
	else if (c=='A') { return 12; }
	return -1;
}

void CGlobal::ParseHandList(CString &list_text, bool inlist[13][13])
{
#ifdef SEH_ENABLE
	try {
#endif
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
	while(*pStr) 
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
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("ParseHandList :\n"); 
		throw;
	}
#endif
}

void CGlobal::create_hand_list_matrices(SFormula *f) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int			listnum, i, j, N;
		CString		token;

		for (listnum=0; listnum<=99; listnum++) 
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
			ParseHandList(f->mHandList[i].list_text, f->inlist[listnum]);
		}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGlobal::create_hand_list_matrices :\n"); 
		throw;
	}
#endif
}

void CGlobal::capture_state(const char *title) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int						i, j;
		bool					playing = true;
		unsigned char			card;

		// figure out if I am playing
		if (!symbols.user_chair_confirmed) 
		{
			playing = false;
		}
		else if (scraper.card_player[(int) symbols.sym.userchair][0] == CARD_BACK ||
				 scraper.card_player[(int) symbols.sym.userchair][1] == CARD_BACK ||
				 scraper.card_player[(int) symbols.sym.userchair][0] == CARD_NOCARD ||
				 scraper.card_player[(int) symbols.sym.userchair][1] == CARD_NOCARD) 
		{
				playing = false;
		}


		// Poker window title
		strncpy(global.state[global.state_index&0xff].m_title, title, 64);
		global.state[global.state_index&0xff].m_title[63] = '\0';

		// Pot information
		for (i=0; i<=9; i++)
			global.state[global.state_index&0xff].m_pot[i] = scraper.pot[i];

		// Common cards
		for (i=0; i<=4; i++) 
		{ 
			if (scraper.card_common[i] == CARD_BACK) 
			{
				card = WH_CARDBACK;
			}
			else if (scraper.card_common[i] == CARD_NOCARD) 
			{
				card = WH_NOCARD;
			}
			else 
			{
				card = ((StdDeck_RANK(scraper.card_common[i])+2)<<4) |
					(StdDeck_SUIT(scraper.card_common[i]) == StdDeck_Suit_CLUBS ? WH_SUIT_CLUBS :
					 StdDeck_SUIT(scraper.card_common[i]) == StdDeck_Suit_DIAMONDS ? WH_SUIT_DIAMONDS :
					 StdDeck_SUIT(scraper.card_common[i]) == StdDeck_Suit_HEARTS ? WH_SUIT_HEARTS :
					 StdDeck_SUIT(scraper.card_common[i]) == StdDeck_Suit_SPADES ? WH_SUIT_SPADES : 0) ;
			}

			global.state[global.state_index&0xff].m_cards[i] = card;
		}

		// playing, posting, dealerchair
		global.state[global.state_index&0xff].m_is_playing = playing;
		global.state[global.state_index&0xff].m_is_posting = symbols.sym.isautopost;
		global.state[global.state_index&0xff].m_fillerbits = 0;
		global.state[global.state_index&0xff].m_fillerbyte = 0;
		global.state[global.state_index&0xff].m_dealer_chair = symbols.sym.dealerchair;

		// loop through all 10 player chairs
		for (i=0; i<=9; i++) 
		{
			
			// player name, balance, currentbet
			strcpy(global.state[global.state_index&0xff].m_player[i].m_name, scraper.playername[i].GetString());
			global.state[global.state_index&0xff].m_player[i].m_balance = symbols.sym.balance[i];
			global.state[global.state_index&0xff].m_player[i].m_currentbet = symbols.sym.currentbet[i];

			// player cards
			for (j=0; j<=1; j++) 
			{
				if (scraper.card_player[i][j] == CARD_BACK) 
				{
					card = WH_CARDBACK;
				}
				else if (scraper.card_player[i][j] == CARD_NOCARD) 
				{
					card = WH_NOCARD;
				}
				else 
				{
					card = ((StdDeck_RANK(scraper.card_player[i][j])+2)<<4) |
						(StdDeck_SUIT(scraper.card_player[i][j]) == StdDeck_Suit_CLUBS ? WH_SUIT_CLUBS :
						 StdDeck_SUIT(scraper.card_player[i][j]) == StdDeck_Suit_DIAMONDS ? WH_SUIT_DIAMONDS :
						 StdDeck_SUIT(scraper.card_player[i][j]) == StdDeck_Suit_HEARTS ? WH_SUIT_HEARTS :
						 StdDeck_SUIT(scraper.card_player[i][j]) == StdDeck_Suit_SPADES ? WH_SUIT_SPADES : 0) ;
				}

				global.state[global.state_index&0xff].m_player[i].m_cards[j] = card;
			}

			// player name known, balance known
			global.state[global.state_index&0xff].m_player[i].m_name_known = scraper.name_good_scrape[i] ? 1 : 0;
			global.state[global.state_index&0xff].m_player[i].m_balance_known = scraper.balance_good_scrape[i] ? 1 : 0;
			global.state[global.state_index&0xff].m_player[i].m_fillerbits = 0;
			global.state[global.state_index&0xff].m_player[i].m_fillerbyte = 0;
		}

		global.state_index++;
#ifdef SEH_ENABLE
	}
	catch (...)	 {	
		logfatal("CGlobal::capture_state :\n"); 
		throw;
	}
#endif
}


void CGlobal::clear_r$indexes(void) 
{
#ifdef SEH_ENABLE
	try {
#endif

		int i,j,k;


		for (i=0; i<=4; i++) 
		{
			// common card info
			tablemap.r$c0cardfaceX_index[i] = -1;
			tablemap.r$c0cardfaceXrank_index[i] = -1;
			tablemap.r$c0cardfaceXsuit_index[i] = -1;

			// pots
			for (j=0; j<=9; j++) 
				for (k=0; k<=9; k++) 
					tablemap.r$c0potXchipYZ_index[i][j][k] = -1;
		}

		for (i=0; i<=9; i++) 
		{
			// player info
			tablemap.r$pXcardfaceY_index[i][0] = -1;
			tablemap.r$pXcardfaceY_index[i][1] = -1;
			tablemap.r$pXcardfaceYrank_index[i][0] = -1;
			tablemap.r$pXcardfaceYrank_index[i][1] = -1;
			tablemap.r$pXcardfaceYsuit_index[i][0] = -1;
			tablemap.r$pXcardfaceYsuit_index[i][1] = -1;
			tablemap.r$uXcardfaceY_index[i][0] = -1;
			tablemap.r$uXcardfaceY_index[i][1] = -1;
			tablemap.r$pXcardback_index[i] = -1;
			tablemap.r$pXseated_index[i] = -1;
			tablemap.r$uXseated_index[i] = -1;
			tablemap.r$pXactive_index[i] = -1;
			tablemap.r$uXactive_index[i] = -1;
			tablemap.r$pXdealer_index[i] = -1;
			tablemap.r$pXname_index[i] = -1;
			tablemap.r$uXname_index[i] = -1;
			tablemap.r$pXbalance_index[i] = -1;
			tablemap.r$uXbalance_index[i] = -1;
			tablemap.r$pXbet_index[i] = -1;

			for (j=0; j<=9; j++) 
				for (k=0; k<=9; k++) 
					tablemap.r$pXchip_index[i][j][k] = -1;

			// button info
			tablemap.r$iXbutton_index[i] = -1;
			tablemap.r$iXstate_index[i] = -1;
			tablemap.r$iXlabel_index[i] = -1;

			for (j=0; j<=9; j++) 
				tablemap.r$iXlabelY_index[i][j] = -1;

			tablemap.r$iXslider_index[i] = -1;
			tablemap.r$iXhandle_index[i] = -1;
			tablemap.r$iXedit_index[i] = -1;
			tablemap.r$i86Xstate_index[i] = -1;
			tablemap.r$i86Xbutton_index[i] = -1;

			// limits
			tablemap.r$c0limitsX_index[i] = -1;
			tablemap.r$c0handnumberX_index[i] = -1;

			// pots
			tablemap.r$c0potX_index[i] = -1;
		}

		// limits
		tablemap.r$c0limits_index = -1;
		tablemap.r$c0istournament_index = -1;
		tablemap.r$c0sblind_index = -1;
		tablemap.r$c0bblind_index = -1;
		tablemap.r$c0bigbet_index = -1;
		tablemap.r$c0ante_index = -1;
		tablemap.r$c0handnumber_index = -1;

		// player info
		tablemap.r$uname_index = -1;
		tablemap.r$ubalance_index = -1;

		// button info
		tablemap.r$i86state_index = -1;
		tablemap.r$i86button_index = -1;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGlobal::clear_r$indexes :\n"); 
		throw;
	}
#endif
}
		
void CGlobal::save_r$indexes(void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		// r$tablepointX not indexed, as it is only used for finding tables on green circle-click, and 
		//   this function is not called until a table has been selected by the user

		int		i, N;
		int		cardnum, seatnum, buttonnum, vertstride, horizstride, potnum, limitnum, handnum, indexnum;

		clear_r$indexes();

		N = tablemap.r$.GetCount();
		for (i=0; i<N; i++) 
		{
			///////////////////////////////////////////////////////////////////////////////////////////////
			// Player info
			// Player cards, r$pXcardfaceYrank, r$pXcardfaceYsuit
			if (tablemap.r$[i].name.Mid(0,1)=="p" && 
				tablemap.r$[i].name.Mid(2,8)=="cardface" && 
				tablemap.r$[i].name.Mid(11,4)=="rank") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				cardnum = tablemap.r$[i].name.GetString()[10] - '0';
				tablemap.r$pXcardfaceYrank_index[seatnum][cardnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="p" && 
				tablemap.r$[i].name.Mid(2,8)=="cardface" && 
				tablemap.r$[i].name.Mid(11,4)=="suit") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				cardnum = tablemap.r$[i].name.GetString()[10] - '0';
				tablemap.r$pXcardfaceYsuit_index[seatnum][cardnum] = i;
			}

			// Player cards, r$pXcardfaceY, r$uXcardfaceY
			else if (tablemap.r$[i].name.Mid(0,1)=="p" && tablemap.r$[i].name.Mid(2,8)=="cardface") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				cardnum = tablemap.r$[i].name.GetString()[10] - '0';
				tablemap.r$pXcardfaceY_index[seatnum][cardnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="u" && tablemap.r$[i].name.Mid(2,8)=="cardface") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				cardnum = tablemap.r$[i].name.GetString()[10] - '0';
				tablemap.r$uXcardfaceY_index[seatnum][cardnum] = i;
			}

			// Player card backs, r$pXcardback
			else if (tablemap.r$[i].name.Mid(0,1)=="p" && tablemap.r$[i].name.Mid(2,8)=="cardback") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$pXcardback_index[seatnum] = i;
			}

			// Seated, r$pXseated, r$uXseated
			else if (tablemap.r$[i].name.Mid(0,1)=="p" && tablemap.r$[i].name.Mid(2,6)=="seated") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$pXseated_index[seatnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="u" && tablemap.r$[i].name.Mid(2,6)=="seated") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$uXseated_index[seatnum] = i;
			}

			// Active, r$pXactive, r$uXactive
			else if (tablemap.r$[i].name.Mid(0,1)=="p" && tablemap.r$[i].name.Mid(2,6)=="active") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$pXactive_index[seatnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="u" && tablemap.r$[i].name.Mid(2,6)=="active") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$uXactive_index[seatnum] = i;
			}

			// Dealer button, r$pXdealer
			else if (tablemap.r$[i].name.Mid(0,1)=="p" && tablemap.r$[i].name.Mid(2,6)=="dealer") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$pXdealer_index[seatnum] = i;
			}

			// Player name r$pXname, r$uXname, r$uname
			else if (tablemap.r$[i].name.Mid(0,1)=="p" && tablemap.r$[i].name.Mid(2,4)=="name") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$pXname_index[seatnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="u" && tablemap.r$[i].name.Mid(2,4)=="name") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$uXname_index[seatnum] = i;
			}
			else if (tablemap.r$[i].name=="uname") 
			{
				tablemap.r$uname_index = i;
			}

			// Player balance, r$pXbalance, r$uXbalance, r$ubalance
			else if (tablemap.r$[i].name.Mid(0,1)=="p" && tablemap.r$[i].name.Mid(2,7)=="balance") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$pXbalance_index[seatnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="u" && tablemap.r$[i].name.Mid(2,7)=="balance") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$uXbalance_index[seatnum] = i;
			}
			else if (tablemap.r$[i].name=="ubalance") 
			{
				tablemap.r$ubalance_index = i;
			}

			// Player bet, r$pXbet, r$pXchipYZ
			else if (tablemap.r$[i].name.Mid(0,1)=="p" && tablemap.r$[i].name.Mid(2,3)=="bet") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$pXbet_index[seatnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="p" && tablemap.r$[i].name.Mid(2,4)=="chip") 
			{
				seatnum = tablemap.r$[i].name.GetString()[1] - '0';
				vertstride = tablemap.r$[i].name.GetString()[6] - '0';
				horizstride = tablemap.r$[i].name.GetString()[7] - '0';
				tablemap.r$pXchip_index[seatnum][vertstride][horizstride] = i;
			}

			///////////////////////////////////////////////////////////////////////////////////////////////
			// Common card info
			// Common cards, r$c0cardfaceXrank, r$c0cardfaceXsuit
			else if (tablemap.r$[i].name.Mid(0,10)=="c0cardface" && tablemap.r$[i].name.Mid(11,4)=="rank") 
			{
				cardnum = tablemap.r$[i].name.GetString()[10] - '0';
				tablemap.r$c0cardfaceXrank_index[cardnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,10)=="c0cardface" && tablemap.r$[i].name.Mid(11,4)=="suit") 
			{
				cardnum = tablemap.r$[i].name.GetString()[10] - '0';
				tablemap.r$c0cardfaceXsuit_index[cardnum] = i;
			}
			// Common cards, r$c0cardfaceX
			else if (tablemap.r$[i].name.Mid(0,10)=="c0cardface") 
			{
				cardnum = tablemap.r$[i].name.GetString()[10] - '0';
				tablemap.r$c0cardfaceX_index[cardnum] = i;
			}

			///////////////////////////////////////////////////////////////////////////////////////////////
			// Button info
			// r$iXbutton, r$iXstate, r$iXlabel, r$iXslider, r$iXhandle, r$iXedit, r$i86Xstate, r$i86Xbutton,
			// r$i86state, r$i86button
			else if (tablemap.r$[i].name.Mid(0,1)=="i" && tablemap.r$[i].name.Mid(2,6)=="button") 
			{
				buttonnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$iXbutton_index[buttonnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="i" && tablemap.r$[i].name.Mid(2,5)=="state") 
			{
				buttonnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$iXstate_index[buttonnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="i" && tablemap.r$[i].name.Mid(2,5)=="label" && tablemap.r$[i].name.GetLength()==7) 
			{
				buttonnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$iXlabel_index[buttonnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="i" && tablemap.r$[i].name.Mid(2,5)=="label" && tablemap.r$[i].name.GetLength()==8) 
			{
				buttonnum = tablemap.r$[i].name.GetString()[1] - '0';
				indexnum = tablemap.r$[i].name.GetString()[7] - '0';
				tablemap.r$iXlabelY_index[buttonnum][indexnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="i" && tablemap.r$[i].name.Mid(2,6)=="slider") 
			{
				buttonnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$iXslider_index[buttonnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="i" && tablemap.r$[i].name.Mid(2,6)=="handle") 
			{
				buttonnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$iXhandle_index[buttonnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,1)=="i" && tablemap.r$[i].name.Mid(2,4)=="edit") 
			{
				buttonnum = tablemap.r$[i].name.GetString()[1] - '0';
				tablemap.r$iXedit_index[buttonnum] = i;
			}
			else if (tablemap.r$[i].name=="r$i86button") 
			{
				tablemap.r$i86button_index = i;
			}
			else if (tablemap.r$[i].name.Mid(0,3)=="i86" && tablemap.r$[i].name.Mid(4,6)=="button") 
			{
				buttonnum = tablemap.r$[i].name.GetString()[3] - '0';
				tablemap.r$i86Xbutton_index[buttonnum] = i;
			}
			else if (tablemap.r$[i].name=="r$i86state") 
			{
				tablemap.r$i86state_index = i;
			}
			else if (tablemap.r$[i].name.Mid(0,3)=="i86" && tablemap.r$[i].name.Mid(4,5)=="state") 
			{
				buttonnum = tablemap.r$[i].name.GetString()[3] - '0';
				tablemap.r$i86Xstate_index[buttonnum] = i;
			}


			///////////////////////////////////////////////////////////////////////////////////////////////
			// Pots
			// Pots, r$c0potX, r$c0potXchipYZ
			else if (tablemap.r$[i].name.Mid(0,5)=="c0pot" && tablemap.r$[i].name.Find("chip")==-1) 
			{
				potnum = tablemap.r$[i].name.GetString()[5] - '0';
				tablemap.r$c0potX_index[potnum] = i;
			}
			else if (tablemap.r$[i].name.Mid(0,5)=="c0pot" && tablemap.r$[i].name.Mid(6,4)=="chip") 
			{
				potnum = tablemap.r$[i].name.GetString()[5] - '0';
				vertstride = tablemap.r$[i].name.GetString()[10] - '0';
				horizstride = tablemap.r$[i].name.GetString()[11] - '0';
				tablemap.r$c0potXchipYZ_index[potnum][vertstride][horizstride] = i;
			}

			///////////////////////////////////////////////////////////////////////////////////////////////
			// Limits
			// r$c0limits, r$c0limitsX 
			else if (tablemap.r$[i].name=="c0limits") 
			{
				tablemap.r$c0limits_index = i;
			}
			else if (tablemap.r$[i].name.Mid(0,8)=="c0limits") 
			{
				limitnum = tablemap.r$[i].name.GetString()[8] - '0';
				tablemap.r$c0limitsX_index[limitnum] = i;
			}

			// r$c0istournament
			else if (tablemap.r$[i].name == "c0istournament") 
			{
				tablemap.r$c0istournament_index = i;
			}

			// r$c0sblind, r$c0bblind, r$c0bigbet, r$c0ante, r$c0handnumberX, r$c0handnumber
			else if (tablemap.r$[i].name == "c0sblind") 
			{
				tablemap.r$c0sblind_index = i;
			}
			else if (tablemap.r$[i].name == "c0bblind") 
			{
				tablemap.r$c0bblind_index = i;
			}
			else if (tablemap.r$[i].name == "c0bigbet") 
			{
				tablemap.r$c0bigbet_index = i;
			}
			else if (tablemap.r$[i].name == "c0ante") 
			{
				tablemap.r$c0ante_index = i;
			}
			else if (tablemap.r$[i].name=="c0handnumber") 
			{
				tablemap.r$c0handnumber_index = i;
			}
			else if (tablemap.r$[i].name.Mid(0,12)=="c0handnumber") 
			{
				handnum = tablemap.r$[i].name.GetString()[12] - '0';
				tablemap.r$c0handnumberX_index[handnum] = i;
			}

		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGlobal::save_r$indexes :\n"); 
		throw;
	}
#endif
}

void CGlobal::save_s$indexes(void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		// s$titletextX, s$!titletextX not indexed, as it is only used for finding tables on green circle-click, and 
		//   this function is not called until a table has been selected by the user/*
		// s$hXtype are not indexed, as those records are ignored in OH

		int		i, num;

		// Clear 'em first
		for (i=0; i<=9; i++)
			tablemap.s$ttlimitsX_index[i] = -1;

		tablemap.s$ttlimits_index = -1;

		for (i=0; i<(int) tablemap.s$.GetSize(); i++) 
		{

			// s$ttlimits, s$ttlimitsX
			if (tablemap.s$[i].name=="ttlimits") 
			{
				tablemap.s$ttlimits_index = i;
			}
			else if (tablemap.s$[i].name.Mid(0,8)=="ttlimits") 
			{
				num = tablemap.s$[i].name.GetString()[8] - '0';
				tablemap.s$ttlimitsX_index[num] = i;
			}
			else if (tablemap.s$[i].name=="c0limits") 
			{
				tablemap.s$c0limits_index = i;
			}
			else if (tablemap.s$[i].name.Mid(0,8)=="c0limits") 
			{
				num = tablemap.s$[i].name.GetString()[8] - '0';
				tablemap.s$c0limitsX_index[num] = i;
			}
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGlobal::save_s$indexes :\n"); 
		throw;
	}
#endif
}

void CGlobal::save_s$strings(void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		// s$reseller and s$mechanic are not saved, as they are only comments and not used in OH for any purpose

		int		i;

		// Clear 'em first
		tablemap.num_chairs = 0;
		tablemap.swagtextmethod = 0;
		tablemap.sitename = "";
		tablemap.chairconfig = "";
		for (i=0; i<=3; i++)
			tablemap.ttype[i] = "";

		for (i=0; i<(int) tablemap.s$.GetSize(); i++) 
		{
			if (tablemap.s$[i].name == "nchairs") 
				tablemap.num_chairs = strtoul(tablemap.s$[i].text.GetString(), NULL, 10);

			if (tablemap.s$[i].name == "swagtextmethod")
				tablemap.swagtextmethod = strtoul(tablemap.s$[i].text.GetString(), NULL, 10);

			if (tablemap.s$[i].name == "sitename")
				tablemap.sitename = tablemap.s$[i].text;

			if (tablemap.s$[i].name == "chairconfig")
				tablemap.chairconfig = tablemap.s$[i].text;
	
			if (tablemap.s$[i].name == "t0type")
				tablemap.ttype[0] = tablemap.s$[i].text;

			if (tablemap.s$[i].name == "t1type")
				tablemap.ttype[1] = tablemap.s$[i].text;

			if (tablemap.s$[i].name == "t2type")
				tablemap.ttype[2] = tablemap.s$[i].text;

			if (tablemap.s$[i].name == "t3type")
				tablemap.ttype[3] = tablemap.s$[i].text;

		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGlobal::save_s$strings :\n"); 
		throw;
	}
#endif
}


void CGlobal::create_replay_frame(void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		CString			path, filename, text, fcra_seen;
		FILE			*fp;
		int				i;
		time_t			ltime;
		struct tm		*now_time;
		char			now_time_str[100];
		bool			scrape_running;


		// Wait for scrape cycle to finish before saving frame
		scrape_running = true;
		i = 0;
		while (scrape_running && i<10)
		{
			EnterCriticalSection(&cs_updater);
			scrape_running = global.update_in_process;
			LeaveCriticalSection(&cs_updater);
			Sleep(50);
			i++;
		}

		// Get current time
		time(&ltime);
		now_time = localtime(&ltime);
		strftime(now_time_str, 100, "%Y-%m-%d %H:%M:%S", now_time);

		// Create replay/session dir if it does not exist
		path.Format("%s\\replay\\session%d\\", startup_path, theApp.sessionnum);
		if (GetFileAttributes(path.GetString()) == INVALID_FILE_ATTRIBUTES)
			SHCreateDirectoryEx(NULL, path.GetString(), NULL);

		// Create bitmap file
		path.Format("%s\\replay\\session%d\\frame%03d.bmp", startup_path, theApp.sessionnum, global.next_replay_frame);
		CreateBMPFile(path.GetString(), scraper.entire_window_Cur);
		
		// Create HTML file
		path.Format("%s\\replay\\session%d\\frame%03d.htm", startup_path, theApp.sessionnum, global.next_replay_frame);
		fp = fopen(path.GetString(), "w");

		fprintf(fp, scraper.title); fprintf(fp, "\n");
		fprintf(fp, "<html>\n");
		fprintf(fp, "<style>\n");
		fprintf(fp, "td {text-align:right;}\n");
		fprintf(fp, "</style>\n");
		fprintf(fp, "<body>\n");
		fprintf(fp, "<font face=courier>\n");
		fprintf(fp, "<img src=\"frame%03d.bmp\">\n", global.next_replay_frame);
		fprintf(fp, "<br>\n");
		fprintf(fp, "<a href=\"frame%03d.htm\">PREV</a>\n", 
			global.next_replay_frame-1 >= 0 ? global.next_replay_frame-1 : global.preferences.replay_max_frames);
		fprintf(fp, "<a href=\"frame%03d.htm\">NEXT</a>\n",
			global.next_replay_frame+1 < global.preferences.replay_max_frames ? global.next_replay_frame+1 : 0);
		fprintf(fp, " [%d.%03d] [%s]<br>\n", theApp.sessionnum, global.next_replay_frame, now_time_str);
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
		for (i=0; i<global.tablemap.num_chairs; i++)
		{

			fprintf(fp, "<tr>\n");
			fprintf(fp, "<td>%d</td>", i);  // #
			text.Format("%s%s%s%s%s%s", 
				(int) (symbols.sym.playersseatedbits) & (1<<i) ? "s" : "-",
				symbols.sym.userchair == i ? "f" : "-",
				(int) (symbols.sym.playersactivebits) & (1<<i) ? "a" : "-",
				symbols.sym.dealerchair== i ? "b" : "-",
				(int) (symbols.sym.playersdealtbits) & (1<<i) ? "d" : "-",
				(int) (symbols.sym.playersplayingbits) & (1<<i) ? "p" : "-");
			fprintf(fp, "<td>%s</td>", text.GetString());  // SFABDP
			fprintf(fp, "<td>%s%s</td>", 
				get_card_html(scraper.card_player[i][0]),
				get_card_html(scraper.card_player[i][1]) );  // hand
			fprintf(fp, "<td>%11.2f</td>", scraper.playerbet[i]);  // bet
			fprintf(fp, "<td>%11.2f</td>", scraper.playerbalance[i]);  // balance
			fprintf(fp, "<td>%-15s</td>\n", scraper.playername[i].GetString());  // name
			fprintf(fp, "</tr>\n");
		}
		fprintf(fp, "</table>\n");
		fprintf(fp, "</td>\n");

		// Table for: FCRA
		fprintf(fp, "<td>\n");
		fprintf(fp, "<table align=center border=4 cellpadding=1 cellspacing=1>\n");
		fprintf(fp, "<tr><th>FCRA</th></tr>\n");
		fprintf(fp, "<tr>\n");
		fcra_seen.Format("%s%s%s%s", 
						 ((int)symbols.sym.myturnbits)&0x1 ? "F" : ".",
						 ((int)symbols.sym.myturnbits)&0x2 ? "C" : ".",
						 ((int)symbols.sym.myturnbits)&0x4 ? "R" : ".",
						 ((int)symbols.sym.myturnbits)&0x8 ? "A" : ".");

		fprintf(fp, "<td>%s</td>\n", fcra_seen.GetString());
		fprintf(fp, "</tr>\n");
		fprintf(fp, "</table>\n");

		// Table for: sb, bb, BB
		fprintf(fp, "<table align=center border=4 cellpadding=1 cellspacing=1>\n");
		fprintf(fp, "<tr><th>sb</th><td>%11.2f</td></tr>\n", symbols.sym.sblind);
		fprintf(fp, "<tr><th>bb</th><td>%11.2f</td></tr>\n", symbols.sym.bblind);
		fprintf(fp, "<tr><th>BB</th><td>%11.2f</td></tr>\n", symbols.bigbet);
		fprintf(fp, "</table>\n");

		// Table for: common cards
		fprintf(fp, "<table align=center border=4 cellpadding=1 cellspacing=1>\n");
		fprintf(fp, "<tr><th>commoncard</th></tr>\n");
		fprintf(fp, "<tr>\n");
		fprintf(fp, "<td>%s%s%s%s%s</td>\n", 
			get_card_html(scraper.card_common[0]),
			get_card_html(scraper.card_common[1]),
			get_card_html(scraper.card_common[2]),
			get_card_html(scraper.card_common[3]),
			get_card_html(scraper.card_common[4]) );
		fprintf(fp, "</tr>\n");
		fprintf(fp, "</table>\n");

		// Table for: pots
		fprintf(fp, "<table align=center border=4 cellpadding=1 cellspacing=1>\n");
		fprintf(fp, "<tr><th>#</th><th>pot</th></tr>\n");
		fprintf(fp, "<tr>\n");
		fprintf(fp, "<td>0</td><td>%11.2f</td>\n", scraper.pot[0]);
		fprintf(fp, "</tr>\n");
		for (i=1; i<=9; i++)
		{
			if (scraper.pot[i])
			{
				fprintf(fp, "<tr>\n");
				fprintf(fp, "<td>%d</td><td>%11.2f</td>\n", i, scraper.pot[i]);
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

		// Increment counter
		global.next_replay_frame++;
		if (global.next_replay_frame >= global.preferences.replay_max_frames)
			global.next_replay_frame = 0;


#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGlobal::create_replay_frame :\n"); 
		throw;
	}
#endif
}

CString CGlobal::get_card_html(unsigned int card)
{
#ifdef SEH_ENABLE
	try {
#endif
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

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CGlobal::get_card_html :\n"); 
		throw;
	}
#endif
}