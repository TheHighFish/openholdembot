// DialogFormulaScintilla.cpp : implementation file
//

// menu options, menu edit commands

#include "stdafx.h"
#include "DialogFormulaScintilla.h"

#include <io.h>
#include "CAutoplayer.h"
#include "CFormula.h"
#include "CGrammar.h"
#include "CHeartbeatThread.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSessionCounter.h"
#include "CSymbols.h"
#include "DialogHandList.h"
#include "DialogNew.h"
#include "DialogRename.h"
#include "DialogSettings.h"
#include "MainFrm.h"
#include "MagicNumbers.h"
#include "OpenHoldem.h"
#include "OpenHoldemDoc.h"
#include "../scintilla/include/SciLexer.h"
#include "../scintilla/include/Scintilla.h"
#include "WinMgr.h"


// CDlgFormulaScintilla dialog
CDlgFormulaScintilla	*m_formulaScintillaDlg = NULL;

char * keywords = // Standard functions
				  "f$alli f$swag f$srai f$rais f$call f$prefold f$rebuy f$delay f$chat f$P f$play f$test f$sitin f$sitout f$leave"
				  // General
				  "ismanual isppro site nchairs isbring session handnumber version "
				  // Tablemap
				  "sitename$ network$ swagdelay allidelay swagtextmethod " 
				  // Tablemap, undocumented
				  "potmethod activemethod "
				  // Formula File
				  "rake nit bankroll "
				  // Limits
				  "bblind sblind ante lim isnl ispl isfl sraiprev sraimin sraimax istournament "
				  // Hand Rank
				  "handrank handrank169 handrank2652 handrank1326 handrank1000 handrankp "
				  // Chairs
				  "chair userchair dealerchair raischair chair$ chairbit$ "
				  // Rounds / Positions
				  "betround br betposition dealposition callposition seatposition dealpositionrais betpositionrais "
				  // Probabilities
				  "prwin prtie prlos prwinnow prlosnow random randomhand randomround randomround1 randomround2 randomround3 randomround4 "
				  // F$P Formula
				  "defcon isdefmode isaggmode "
				  // Chip Amounts
				  "balance balance0 balance1 balance2 balance3 balance4 balance5 balance6 balance7 balance8 balance9 "
				  "stack0 stack1 stack2 stack3 stack4 stack5 stack6 stack7 stack8 stack9 "
				  "currentbet currentbet0 currentbet1 currentbet2 currentbet3 currentbet4 currentbet5 currentbet6 currentbet7 currentbet8 currentbet9 "
				  "call bet bet1 bet2 bet3 bet4 pot potcommon potplayer callshort raisshort "
				  // Number of Bets	
				  "nbetstocall nbetstorais ncurrentbets ncallbets nraisbets "
				  // List Tests
				  "islistcall islistrais islistalli isemptylistcall isemptylistrais isemptylistalli nlistmax nlistmin pokerval "
				  // Poker Values
				  "pokervalplayer pokervalcommon pcbits npcbits "
				  // Poker Value Constants
				  "hicard onepair twopair threeofakind straight flush fullhouse fourofakind straightflush royalflush fiveofakind "
				  // Hand Tests
				  "ishandup ishandupcommon ishicard isonepair istwopair isthreeofakind isstraight "
				  "isflush isfullhouse isfourofakind isstraightflush isroyalflush isfiveofakind "
				  // Pocket Tests
				  "ispair issuited isconnector "
				  // Pocket / Common Tests
				  "ishipair islopair ismidpair ishistraight ishiflush "
				  // Players, Friends, Opponents
				  "nopponents nopponentsmax nplayersseated nplayersactive nplayersdealt nplayersplaying nplayersblind "
				  "nopponentsseated nopponentsactive nopponentsdealt nopponentsplaying nopponentsblind "
				  "nfriendsseated nfriendsactive nfriendsdealt nfriendsplaying nfriendsblind "
				  "nopponentschecking nopponentscalling nopponentsraising nopponentsbetting nopponentsfolded "
				  "nplayerscallshort nchairsdealtright nchairsdealtleft playersseatedbits playersactivebits "
				  "playersdealtbits playersplayingbits playersblindbits opponentsseatedbits opponentsactivebits "
				  "opponentsdealtbits opponentsplayingbits opponentsblindbits friendsseatedbits friendsactivebits "
				  "friendsdealtbits friendsplayingbits friendsblindbits "
				  // Flags
				  "fmax f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 f10 f11 f12 f13 f14 f15 f16 f17 f18 f19 "
				  // Common Cards
				  "ncommoncardspresent ncommoncardsknown nflopc "
				  // (Un)known Cards
				  "nouts ncardsknown ncardsunknown ncardsbetter "
				  // nhands
				  "nhands nhandshi nhandslo nhandsti " 
				  // Flushes / Straights / Sets
				  "nsuited nsuitedcommon tsuit tsuitcommon nranked nrankedcommon trank trankcommon nstraight nstraightcommon "
				  "nstraightfill nstraightfillcommon nstraightflush nstraightflushcommon nstraightflushfill nstraightflushfillcommon "
				  // Rank Bits
				  "rankbits rankbitscommon rankbitsplayer rankbitspoker srankbits "
				  "srankbitscommon srankbitsplayer srankbitspoker "
				  // Rank Hi
				  "rankhi rankhicommon rankhiplayer rankhipoker srankhi srankhicommon srankhiplayer srankhipoker "
				  // Rank Lo
				  "ranklo ranklocommon rankloplayer ranklopoker sranklo sranklocommon srankloplayer sranklopoker "
				  // Time
				  "elapsed elapsedhand elapsedauto elapsedtoday "
				  "elapsed1970 clocks nclockspersecond ncps "
				  // Autoplayer
				  "myturnbits ismyturn issittingin issittingout "
				  "isautopost isfinalanswer "
				  // History
				  "nplayersround1 nplayersround2 nplayersround3 nplayersround4 "
				  "nplayersround prevaction didchec didcall didrais didswag nbetsround1 nbetsround2 nbetsround3 "
				  "nbetsround4 nbetsround didchecround1 didchecround2 didchecround3 didchecround4 "
				  "didcallround1 didcallround2 didcallround3 didcallround4 didraisround1 didraisround2 "
				  "didraisround3 didraisround4 didswaground1 didswaground2 didswaground3 didswaground4 "
				  // RON / RUN	
				  "ron$royfl ron$strfl ron$4kind ron$fullh ron$flush ron$strai ron$3kind ron$2pair ron$1pair "
				  "ron$hcard ron$total ron$pokervalmax ron$prnuts ron$prbest ron$clocks run$royfl run$strfl "
				  "run$4kind run$fullh run$flush run$strai run$3kind run$2pair run$1pair run$hcard run$total "
				  "run$pokervalmax run$prnuts run$prbest run$clocks "
				  // Versus
				  "vs$nhands vs$nhandshi vs$nhandsti vs$nhandslo "
				  "vs$prwin vs$prtie vs$prlos vs$prwinhi vs$prtiehi vs$prloshi vs$prwinti vs$prtieti vs$prlosti "
				  "vs$prwinlo vs$prtielo vs$prloslo "
                  "vs$prwinhinow vs$prtiehinow vs$prloshinow vs$prwintinow vs$prtietinow vs$prlostinow "
				  "vs$prwinlonow vs$prtielonow vs$prloslonow vs$nhandshinow vs$nhandstinow vs$nhandslonow "
				  // History	
				  "hi_"
				  //Table statistics 
				  "floppct turnpct riverpct avgbetspf tablepfr maxbalance handsplayed "
				  // Action
				  "lastraised1 lastraised2 lastraised3 lastraised4 "
				  "raisbits1 raisbits2 raisbits3 raisbits4 "
				  "callbits1 callbits2 callbits3 callbits4 "
				  "foldbits1 foldbits2 foldbits3 foldbits4 "
				  "oppdealt "
				  "ac_aggressor ac_agchair_after ac_preflop_pos ac_prefloprais_pos "
				  "ac_postflop_pos ac_pf_bets ac_first_into_pot "
				  "ac_betpos0 ac_betpos1 ac_betpos2 ac_betpos3 ac_betpos4 ac_betpos5 ac_betpos6 ac_betpos7 ac_betpos8 ac_betpos9 "
				  "ac_dealpos0 ac_dealpos1 ac_dealpos2 ac_dealpos3 ac_dealpos4 ac_dealpos5 ac_dealpos6 ac_dealpos7 ac_dealpos8 ac_dealpos9 "
				  // MyHand
				  "mh_3straight00 mh_3straight01 mh_3straight10 mh_3straight11 "
				  "mh_bottomsd mh_nsuitedbetter mh_kickerbetter mh_nouts mh_str_quads mh_str_fullhouse "
				  "mh_str_flush mh_str_straight mh_str_trips mh_str_twopair mh_str_onepair "
				  // Logging
				  "log$ "
				  // ICM calculator
				  "icm icm_fold icm_callwin icm_calllose icm_calltie "
				  "icm_alliwin0 icm_alliwin1 icm_alliwin2 icm_alliwin3 icm_alliwin4 "
				  "icm_alliwin5 icm_alliwin6 icm_alliwin7 icm_alliwin8 icm_alliwin9 "
				  "icm_allilose0 icm_allilose1 icm_allilose2 icm_allilose3 icm_allilose4 "
				  "icm_allilose5 icm_allilose6 icm_allilose7 icm_allilose8 icm_allilose9 "
				  "icm_alliwinSB icm_alliwinBB icm_alliwinUTG icm_alliwinUTG1 icm_alliwinUTG2 icm_alliwinUTG3 "
				  "icm_alliwinUTG4 icm_alliwinUTG5 icm_alliwinUTG6 icm_alliwinCO icm_alliwinD "
				  "icm_allitieSB icm_allitieBB icm_allitieUTG icm_allitieUTG1 icm_allitieUTG2 "
				  "icm_allitieUTG3 icm_allitieUTG4 icm_allitieUTG5 icm_allitieUTG6 icm_allitieCO "
				  "icm_allitieD icm_alliloseSB icm_alliloseBB icm_alliloseUTG icm_alliloseUTG1 "
				  "icm_alliloseUTG2 icm_alliloseUTG3 icm_alliloseUTG4 icm_alliloseUTG5 icm_alliloseUTG6 "
				  "icm_alliloseCO icm_alliloseD "
				  // Hand multiplexor
				  " f$$ "
				  // Memory Symbols
				  "me_st_ me_re_ "
				  // Card Symbols
				  "$$pc0 $$pc1 $$pr0 $$pr1 $$ps0 $$ps1 "
				  "$$cc0 $$cc1 $$cc2 $$cc3 $$cc4 "
				  "$$cr0 $$cr1 $$cr2 $$cr3 $$cr4 "
				  "$$cs0 $$cs1 $$cs2 $$cs3 $$cs4 "
				  // Undocumented
				  "balance_rank0 balance_rank1 balance_rank2 balance_rank3 balance_rank4 balance_rank5 "
				  "balance_rank6 balance_rank7 balance_rank8 balance_rank9 "
				  // Poker Tracker ring symbols
				  "pt_icon pt_hands pt_pfr pt_aggp pt_aggf pt_aggt pt_aggr "
				  "pt_aggtot pt_aggtotnopf pt_floppct pt_turnpct pt_riverpct pt_vpip pt_pf_rfi "
				  "pt_pf_cr pt_pfats pt_wsdp pt_wssd pt_fbbts pt_fsbts "
				  // Poker Tracker ring symbols for the "raischair"
				  "pt_ricon pt_rhands pt_rpfr pt_raggp pt_raggf pt_raggt pt_raggr "
				  "pt_raggtot pt_raggtotnopf pt_rfloppct pt_rturnpct pt_rriverpct pt_rvpip pt_rpf_rfi "
				  "pt_rpf_cr pt_rpfats pt_rwsdp pt_rwssd pt_rfbbts pt_rfsbts "
				  // Poker Tracker tournament symbols
				  "ptt_icon ptt_hands ptt_pfr ptt_aggp ptt_aggf ptt_aggt ptt_aggr "
				  "ptt_aggtot ptt_aggtotnopf ptt_floppct ptt_turnpct ptt_riverpct ptt_vpip ptt_pf_rfi "
				  "ptt_pf_cr ptt_pfats ptt_wsdp ptt_wssd ptt_fbbts ptt_fsbts "
					// Poker Tracker tournament symbols for the "raischair"
				  "ptt_ricon ptt_rhands ptt_rpfr ptt_raggp ptt_raggf ptt_raggt ptt_raggr "
				  "ptt_raggtot ptt_raggtotnopf ptt_rfloppct ptt_rturnpct ptt_rriverpct ptt_rvpip ptt_rpf_rfi "
				  "ptt_rpf_cr ptt_rpfats ptt_rwsdp ptt_rwssd ptt_rfbbts ptt_rfsbts ";

#define ID_SCIN_SIZERBAR 5555

static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

static UINT m_lpaIDToolBar[] = 
{
	ID_FORMULA_TOOLBAR_NEW,
	ID_FORMULA_TOOLBAR_RENAME,
	ID_FORMULA_TOOLBAR_DELETE,
	ID_SEPARATOR,
	ID_FORMULA_TOOLBAR_FONT,
	ID_FORMULA_TOOLBAR_SETTINGS,
	ID_FORMULA_TOOLBAR_HANDLIST,
	ID_SEPARATOR,
	ID_FORMULA_TOOLBAR_LESS_PRECISION,
	ID_FORMULA_TOOLBAR_MORE_PRECISION,
	ID_FORMULA_TOOLBAR_EQUAL_LEFT,
	ID_FORMULA_TOOLBAR_EQUAL_RIGHT
};

static UINT indicators[] = 
{
	ID_SEPARATOR,		   // status line indicator
	ID_INDICATOR_FORMULA_CURPOS,
};

// Window map tells CWinMgr how to position dialog controls
BEGIN_WINDOW_MAP(ScintillaFormulaMap)
BEGINCOLS(WRCT_REST,0,RCMARGINS(4,4))

BEGINROWS(WRCT_TOFIT,0,RCMARGINS(0,4))
RCTOFIT(IDC_SEARCH_ED)
RCREST(IDC_SCINTILLA_FORMULA_TREE)	// tree
RCTOFIT(IDC_FORMULA_TAB)
ENDGROUP()

RCFIXED(ID_SCIN_SIZERBAR,4)			// vertical sizer bar

BEGINROWS(WRCT_REST,0,0)
BEGINCOLS(WRCT_TOFIT,0,0)
RCREST(-1)
RCTOFIT(IDC_SCINTILLA_CALC_RESULT)	// calc result
RCSPACE(8)
RCTOFIT(IDC_SCINTILLA_CALC)			// calc
RCSPACE(8)
RCTOFIT(IDC_SCINTILLA_AUTO)			// auto
ENDGROUP()

RCSPACE(8)

RCREST(IDC_SCINCTRL)				// formula edit

RCSPACE(4)

RCTOFIT(IDC_FUNCTIONS_TAB)			// function tab

RCSPACE(4)

BEGINCOLS(WRCT_TOFIT,0,0)
RCREST(-1)
RCTOFIT(IDOK)							// ok
RCSPACE(8)
RCTOFIT(IDC_SCINTILLA_APPLY)			// apply
RCSPACE(8)
RCTOFIT(IDCANCEL)						// cancel
ENDGROUP()
ENDGROUP()
END_WINDOW_MAP()

CDlgFormulaScintilla::CDlgFormulaScintilla(CWnd* pParent /*=NULL*/) :
		CDialog(CDlgFormulaScintilla::IDD, pParent), m_winMgr(ScintillaFormulaMap) 
{
	in_startup = true;

	m_standard_headings.Add("Autoplayer Functions");
	m_standard_headings.Add("Standard Functions");
	m_standard_headings.Add("Debug Functions");

	m_standard_expand[0] = prefs.expand_auto();
	m_standard_expand[1] = prefs.expand_std();
	m_standard_expand[2] = prefs.expand_debug();

	ASSERT(m_standard_headings.GetSize() == 3);

	m_standard_functions[0].Add("f$alli");
	m_standard_functions[0].Add("f$swag");
	m_standard_functions[0].Add("f$srai");
	m_standard_functions[0].Add("f$rais");
	m_standard_functions[0].Add("f$call");
	m_standard_functions[0].Add("f$prefold");
	m_standard_functions[0].Add("f$delay");

	m_standard_functions[1].Add("notes");
	m_standard_functions[1].Add("dll");
	m_standard_functions[1].Add("f$chat");
	m_standard_functions[1].Add("f$rebuy");
	m_standard_functions[1].Add("f$P");
	m_standard_functions[1].Add("f$play");

	m_standard_functions[2].Add("f$test");
	m_standard_functions[2].Add("f$debug");

	// Copy current doc formula into working set
	m_wrk_formula.ClearFormula();
	m_wrk_formula.CopyFormulaFrom(p_formula);

	m_current_edit = "";
	m_dirty = false;
	m_is_toolbar_visible = true;
	m_are_linenumbers_visible = false;
	m_is_selection_margin_visible = false;
	m_is_folding_margin_visible = false;
	m_is_syntax_colored = true;

	m_precision = 4;
	m_equal = 12;
	m_udf_sort = false;
	m_udf_group = false;

	ok_to_update_debug = false;

	m_pActiveScinCtrl = NULL;
	m_pFRDlg = NULL;

	hUDFItem = NULL;
}

CDlgFormulaScintilla::~CDlgFormulaScintilla() 
{
	for (int i=0; i<m_ScinArray.GetSize(); i++)
	{
		if (&m_EmptyScinCtrl != m_ScinArray.GetAt(i)._pWnd)
			delete m_ScinArray.GetAt(i)._pWnd;
	}
	m_ScinArray.RemoveAll();
}

void CDlgFormulaScintilla::DoDataExchange(CDataExchange* pDX) 
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCINCTRL, m_EmptyScinCtrl);
	DDX_Control(pDX, IDC_SEARCH_ED, m_SearchEdit);
	DDX_Control(pDX, IDC_SCINTILLA_FORMULA_TREE, m_FormulaTree);
	DDX_Control(pDX, IDC_SCINTILLA_CALC, m_ButtonCalc);
	DDX_Control(pDX, IDC_SCINTILLA_AUTO, m_ButtonAuto);
	DDX_Control(pDX, IDC_SCINTILLA_CALC_RESULT, m_CalcResult);
	DDX_Control(pDX, IDC_FORMULA_TAB, m_TabControl);
	DDX_Control(pDX, IDC_FUNCTIONS_TAB, m_FunctionTab);
	DDX_Control(pDX, IDC_SYMBOL_TREE, m_SymbolTree);
	DDX_Control(pDX, IDCANCEL, m_FormulaCancel);
	DDX_Control(pDX, IDC_SCINTILLA_APPLY, m_FormulaApply);
	DDX_Control(pDX, IDOK, m_FormulaOK);
}

BEGIN_MESSAGE_MAP(CDlgFormulaScintilla, CDialog)
	// Menu items
	ON_COMMAND(ID_FORMULA_FILE_APPLY, &CDlgFormulaScintilla::OnBnClickedApply)
	ON_COMMAND(ID_FORMULA_FILE_APPLYANDSAVE, &CDlgFormulaScintilla::OnBnClickedApplySave)
	ON_COMMAND(ID_FORMULA_FILE_OK, &CDlgFormulaScintilla::OnBnClickedOk)
	ON_COMMAND(ID_FORMULA_FILE_CANCEL, &CDlgFormulaScintilla::OnBnClickedCancel)
	ON_COMMAND(ID_FORMULA_EDIT_NEW, &CDlgFormulaScintilla::OnNew)
	ON_COMMAND(ID_FORMULA_EDIT_RENAME, &CDlgFormulaScintilla::OnRename)
	ON_COMMAND(ID_FORMULA_EDIT_DELETE, &CDlgFormulaScintilla::OnDelete)
	ON_COMMAND(ID_FORMULA_EDIT_CUT, &CDlgFormulaScintilla::OnEditCut)
	ON_COMMAND(ID_FORMULA_EDIT_COPY, &CDlgFormulaScintilla::OnEditCopy)
	ON_COMMAND(ID_FORMULA_EDIT_PASTE, &CDlgFormulaScintilla::OnEditPaste)
	ON_COMMAND(ID_FORMULA_EDIT_DELETE_TEXT, &CDlgFormulaScintilla::OnEditDeleteText)
	ON_COMMAND(ID_FORMULA_EDIT_UNDO, &CDlgFormulaScintilla::OnEditUndo)
	ON_COMMAND(ID_FORMULA_EDIT_REDO, &CDlgFormulaScintilla::OnEditRedo)
	ON_COMMAND(ID_FORMULA_EDIT_SELECTALL, &CDlgFormulaScintilla::OnEditSelectAll)
	ON_COMMAND(ID_FORMULA_EDIT_FONT, &CDlgFormulaScintilla::OnFont)
	ON_COMMAND(ID_FORMULA_EDIT_SETTINGS, &CDlgFormulaScintilla::OnSettings)
	ON_COMMAND(ID_FORMULA_EDIT_HANDLIST, &CDlgFormulaScintilla::OnHandList)
	ON_COMMAND(ID_FORMULA_EDIT_FIND, &CDlgFormulaScintilla::OnFindReplaceDlg)
	ON_COMMAND(ID_FORMULA_EDIT_FIND_NEXT, &CDlgFormulaScintilla::OnFindNext)
	ON_COMMAND(ID_FORMULA_EDIT_FIND_PREV, &CDlgFormulaScintilla::OnFindPrev)

	ON_COMMAND(ID_FORMULA_VIEW_TOOLBAR, &CDlgFormulaScintilla::ToggleToolbar)
	ON_COMMAND(ID_FORMULA_VIEW_LINENUMBERS, &CDlgFormulaScintilla::ToggleLineNumbers)
	ON_COMMAND(ID_FORMULA_VIEW_SHOWSELECTIONMARGIN, &CDlgFormulaScintilla::ToggleSelectionMargin)
	ON_COMMAND(ID_FORMULA_VIEW_SHOWFOLDINGMARGIN, &CDlgFormulaScintilla::ToggleFoldingMargin)
	ON_COMMAND(ID_FORMULA_VIEW_SYNTAXCOLORING, &CDlgFormulaScintilla::ToggleSyntaxColoring)
	ON_COMMAND(ID_FORMULA_DEBUG_LOGFDEBUG, &CDlgFormulaScintilla::OnFormulaDebugLogfdebug)

	ON_COMMAND(ID_HELP, &CDlgFormulaScintilla::OnHelp)
	ON_COMMAND(ID_HELP_DOCUMENTATIONWIKI, &CDlgFormulaScintilla::OnHelpWiki)
	ON_COMMAND(ID_HELP_FORUMS, &CDlgFormulaScintilla::OnHelpForums)

	// Keyboard Shortcuts
	ON_COMMAND(ID_TOGGLE_BOOKMARK, &CDlgFormulaScintilla::OnToggleBookmark)
	ON_COMMAND(ID_GOTO_NEXT_BOOKMARK, &CDlgFormulaScintilla::OnNextBookmark)
	ON_COMMAND(ID_GOTO_PREV_BOOKMARK, &CDlgFormulaScintilla::OnPrevBookmark)

	// Toolbar Buttons
	ON_COMMAND(ID_FORMULA_TOOLBAR_NEW, &CDlgFormulaScintilla::OnNew)
	ON_COMMAND(ID_FORMULA_TOOLBAR_RENAME, &CDlgFormulaScintilla::OnRename)
	ON_COMMAND(ID_FORMULA_TOOLBAR_DELETE, &CDlgFormulaScintilla::OnDelete)
	ON_COMMAND(ID_FORMULA_TOOLBAR_FONT, &CDlgFormulaScintilla::OnFont)
	ON_COMMAND(ID_FORMULA_TOOLBAR_SETTINGS, &CDlgFormulaScintilla::OnSettings)
	ON_COMMAND(ID_FORMULA_TOOLBAR_HANDLIST, &CDlgFormulaScintilla::OnHandList)
	ON_COMMAND(ID_FORMULA_TOOLBAR_LESS_PRECISION, &CDlgFormulaScintilla::OnBnClickedLessPrecision)
	ON_COMMAND(ID_FORMULA_TOOLBAR_MORE_PRECISION, &CDlgFormulaScintilla::OnBnClickedMorePrecision)
	ON_COMMAND(ID_FORMULA_TOOLBAR_EQUAL_LEFT, &CDlgFormulaScintilla::OnBnClickedEqualLeft)
	ON_COMMAND(ID_FORMULA_TOOLBAR_EQUAL_RIGHT, &CDlgFormulaScintilla::OnBnClickedEqualRight)
	//ON_COMMAND(ID_FORMULA_TOOLBAR_HELP, &CMainFrame::OnHelp()) !!!

	// Buttons
	ON_BN_CLICKED(IDOK, &CDlgFormulaScintilla::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SCINTILLA_APPLY, &CDlgFormulaScintilla::OnBnClickedApply)
	ON_BN_CLICKED(IDC_SCINTILLA_CALC, &CDlgFormulaScintilla::OnBnClickedCalc)
	ON_BN_CLICKED(IDCANCEL, &CDlgFormulaScintilla::OnBnClickedCancel)

	ON_REGISTERED_MESSAGE( WM_FINDREPLACE, OnFindReplace )

	ON_BN_CLICKED(IDC_SCINTILLA_AUTO, &CDlgFormulaScintilla::OnBnClickedAuto)

	// WinMgr
	ON_REGISTERED_MESSAGE(WM_WINMGR, OnWinMgr)
	ON_WM_SIZE()

	// Scintilla
	ON_NOTIFY(TVN_SELCHANGING, IDC_SCINTILLA_FORMULA_TREE, &CDlgFormulaScintilla::OnTvnSelchangingFormulaTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SCINTILLA_FORMULA_TREE, &CDlgFormulaScintilla::OnTvnSelchangedFormulaTree)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_SCINTILLA_FORMULA_TREE, &CDlgFormulaScintilla::OnTvnExpandedFormulaTree)

	ON_NOTIFY(TCN_SELCHANGE, IDC_FORMULA_TAB, OnTabSelectionChange)
	ON_NOTIFY(TCN_SELCHANGE, IDC_FUNCTIONS_TAB, OnFunctionTabSelectionChange)

	ON_NOTIFY(TVN_GETINFOTIP, IDC_SYMBOL_TREE, OnSymbolTreeTipInfo)
	ON_NOTIFY(NM_RCLICK, IDC_SYMBOL_TREE, OnSymbolContextMenu)

	ON_EN_UPDATE(IDC_SEARCH_ED, OnSearchUpdate)

	// Tooltips
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)

	// Timer
	ON_WM_TIMER()

	ON_COMMAND(ID_FORMULA_DEBUG_MYTURN, &CDlgFormulaScintilla::OnFormulaDebugMyturn)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_FORMULA_VIEW_SORTUDF, &CDlgFormulaScintilla::OnFormulaViewSortudf)
	ON_COMMAND(ID_FORMULA_VIEW_GROUPUDFS, &CDlgFormulaScintilla::OnFormulaViewGroupudf)
END_MESSAGE_MAP()

CScintillaWnd *CDlgFormulaScintilla::SetupScintilla(CScintillaWnd *pWnd, const char *title) 
{
	if (!pWnd) 
	{
		ASSERT(title);
		pWnd = new CScintillaWnd();
		m_ScinArray.Add(CScintillaInfo(pWnd, title));

		m_FunctionTab.InsertItem(m_FunctionTab.GetItemCount(), title);
		m_FunctionTab.SetCurSel(m_FunctionTab.GetItemCount());

		CRect rect;
		m_EmptyScinCtrl.GetWindowRect(rect);
		ScreenToClient(rect);

		pWnd->Create("", WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, IDC_SCINCTRL+m_ScinArray.GetSize());
	}
	pWnd->Init();
	pWnd->SetDisplayLinenumbers(m_are_linenumbers_visible);
	pWnd->SetDisplaySelection(m_is_selection_margin_visible);
	pWnd->SetDisplayFolding(m_is_folding_margin_visible);
	pWnd->SendMessage(SCI_SETINDENTATIONGUIDES, false, 0);
	pWnd->SendMessage(SCI_SETMODEVENTMASK, (SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT), 0);
	pWnd->SetLexer(SCLEX_CPP);
	UpdateScintillaKeywords(pWnd);
	pWnd->EnableWindow(false);
	SetStyleColors(pWnd, m_is_syntax_colored);

	LOGFONT logfont;
	editfont.GetLogFont(&logfont);
	for (int i=0; i<=MAX_STYLE_NUM; i++) {
		pWnd->SetFontname(i, logfont.lfFaceName);
		pWnd->SetFontheight(i, -logfont.lfHeight);
		pWnd->SetBold(i, (logfont.lfWeight==FW_BOLD ? true : false));
		pWnd->SetItalic(i, logfont.lfItalic);
	}

	return pWnd;
}

void CDlgFormulaScintilla::ConstructKeywords(CString &keys)
{
	keys = keywords;
	int i = 0;
	for (i=0; i<m_wrk_formula.formula()->mFunction.GetSize(); i++) {
		keys.AppendFormat(" %s", m_wrk_formula.formula()->mFunction[i].func);
	}
	for (i=0; i<m_wrk_formula.formula()->mHandList.GetSize(); i++) {
		keys.AppendFormat(" is%s", m_wrk_formula.formula()->mHandList[i].list);
		if (m_wrk_formula.formula()->mHandList[i].list.GetLength() > 4) {
			keys.AppendFormat(" vs$%s$prlos", m_wrk_formula.formula()->mHandList[i].list.GetString()+4);
			keys.AppendFormat(" vs$%s$prwin", m_wrk_formula.formula()->mHandList[i].list.GetString()+4);
			keys.AppendFormat(" vs$%s$prtie", m_wrk_formula.formula()->mHandList[i].list.GetString()+4);
		}
	}
	for (i=0; i<10; i++) {
		keys.AppendFormat(" pt_icon%d", i);
		keys.AppendFormat(" pt_hands%d", i);
		keys.AppendFormat(" pt_pfr%d", i);
		keys.AppendFormat(" pt_aggp%d", i);
		keys.AppendFormat(" pt_aggf%d", i);
		keys.AppendFormat(" pt_aggt%d", i);
		keys.AppendFormat(" pt_aggr%d", i);
		keys.AppendFormat(" pt_aggtot%d", i);
		keys.AppendFormat(" pt_aggtotnopf%d", i);
		keys.AppendFormat(" pt_floppct%d", i);
		keys.AppendFormat(" pt_turnpct%d", i);
		keys.AppendFormat(" pt_riverpct%d", i);
		keys.AppendFormat(" pt_vpip%d", i);
		keys.AppendFormat(" pt_pf_rfi%d", i);
		keys.AppendFormat(" pt_pf_cr%d", i);
		keys.AppendFormat(" pt_pfats%d", i);
		keys.AppendFormat(" pt_wsdp%d", i);
		keys.AppendFormat(" pt_wssd%d", i);
		keys.AppendFormat(" pt_fbbts%d", i);
		keys.AppendFormat(" pt_fsbts%d", i);
	}
}

void CDlgFormulaScintilla::UpdateScintillaKeywords(CScintillaWnd *pWnd) 
{
	CString keys;
	CString filter;
	m_SearchEdit.GetWindowText(filter);
	if (filter.IsEmpty()) {
		ConstructKeywords(keys);
		pWnd->SendMessage(SCI_SETKEYWORDS, 0, (LPARAM) keys.GetString());
	} else {
		pWnd->SendMessage(SCI_SETKEYWORDS, 0, (LPARAM) "");
		pWnd->SendMessage(SCI_SETKEYWORDS, 3, (LPARAM) filter.GetString());
	}

	pWnd->Refresh();
}

void CDlgFormulaScintilla::UpdateAllScintillaKeywords() 
{
	CString keys;
	ConstructKeywords(keys);
	for (int iScint=0; iScint<m_ScinArray.GetSize(); iScint++) {
		m_ScinArray[iScint]._pWnd->SendMessage(SCI_SETKEYWORDS, 0, (LPARAM) keys.GetString());
		m_ScinArray[iScint]._pWnd->Refresh();
	}
}

void CDlgFormulaScintilla::DeleteScintilla(CScintillaWnd *pWnd) 
{
	for (int i=0; i<m_ScinArray.GetSize(); i++)
	{
		if (m_ScinArray[i]._pWnd == pWnd) 
		{
			m_ScinArray.RemoveAt(i);
		}
	}
	pWnd->DestroyWindow();
	delete pWnd;
}

// CDlgFormulaScintilla message handlers
BOOL CDlgFormulaScintilla::OnInitDialog() 
{
	int			i = 0, N = 0;
	int			max_x = 0, max_y = 0;

	in_startup = true;

	CDialog::OnInitDialog();

	// Restore font of Scintilla control

	if (!GetProfileFont("Font", "Formula", editfont)) 
	{
		// Use 8pt Courier (monospace) default
		editfont.CreatePointFont(100,"Courier");
	}

	// Save tofit windows as current size
	m_winMgr.InitToFitSizeFromCurrent(this);		// make tofit = current size
	m_winMgr.CalcLayout(this);						  // recalc
	m_winMgr.SetWindowPositions(this);				// set positions

	// Create sizer bar window
	m_winMgrSizerBar.Create(WS_VISIBLE|WS_CHILD, this, m_winMgr, ID_SCIN_SIZERBAR);

	// Menu
	m_Menu.LoadMenu(MAKEINTRESOURCE(IDR_FORMULA));
	SetMenu(&m_Menu);
	m_Menu.Detach();

	// Toolbar
	m_toolBar.Create(this);
	m_toolBar.LoadToolBar(IDR_FORMULA_BAR);
	m_toolBar.SetButtons(m_lpaIDToolBar, sizeof(m_lpaIDToolBar)/sizeof(UINT));
	m_toolBar.SetBarStyle(m_toolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// Status bar
	m_wndStatusBar.Create(this);
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_STRETCH | SBPS_NOBORDERS, 0);
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_FORMULA_CURPOS, NULL, 100);
	m_wndStatusBar.SetPaneText(1, "Line: 1, Col: 1");

	// Resize dialog for control bars
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	ResizeDialogForControlBars();

	// Accelerators
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_FORMULA_EDIT_ACC), RT_ACCELERATOR);
	m_hEditAccelTable = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_FORMULA_EDIT_ACC));
	hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_FORMULA_DIALOG_ACC), RT_ACCELERATOR);
	m_hDialogAccelTable = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_FORMULA_DIALOG_ACC));

	m_TabControl.InsertItem(0, "Formulas");
	m_TabControl.InsertItem(1, "Symbols");

	m_FunctionTab.InsertItem(0, "Empty");
	PopulateFormulaTree();

	SetupScintilla(&m_EmptyScinCtrl, NULL);
	m_ScinArray.Add(CScintillaInfo(&m_EmptyScinCtrl, ""));
	m_pActiveScinCtrl = &m_EmptyScinCtrl;

	// Set dialog title
	::SetWindowText(m_hWnd, "Formula Editor");

	// Set dialog icon
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	this->SetIcon(hIcon, FALSE);

	// Restore window location and size, precision preference
	max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
	max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
	::SetWindowPos(m_hWnd, HWND_TOP, min(prefs.formula_x(), max_x), min(prefs.formula_y(), max_y),
				   prefs.formula_dx(), prefs.formula_dy(), SWP_NOCOPYBITS);
	m_precision = prefs.precision();
	m_equal = prefs.equal();

	// Debug logging preferences
	m_fdebuglog = prefs.fdebuglog();
	m_fdebuglog_myturn = prefs.fdebuglog_myturn();
	m_wrote_fdebug_header = false;

	// Sort UDF?
	m_udf_sort = prefs.udf_sort();
	if (m_udf_sort)
		SortUdfTree();

	HandleEnables(true);

	//  Set auto button state
	m_ButtonAuto.SetWindowText("Auto");

	// Timer to keep menu state updated
	SetTimer(MENU_UPDATE_TIMER, 100, 0);

	// Timer to keep f$debug tab updated
	SetTimer(DEBUG_UPDATE_TIMER, 100, 0);

	in_startup = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return false
}

void CDlgFormulaScintilla::OnCancel()
{
	if (m_dirty)
		if(!PromptToSave())
			return;

	DestroyWindow();
}

void CDlgFormulaScintilla::RemoveSingleItemGroups()
{
	HTREEITEM hUDFChildItem = m_FormulaTree.GetChildItem(hUDFItem);
	HTREEITEM hNextLevelItem = NULL, hNextItem = NULL;
	CString fnName = 0;

	while (hUDFChildItem != NULL)
	{
		if (!m_FormulaTree.ItemHasChildren(hUDFChildItem)) 
		{
			hUDFChildItem = m_FormulaTree.GetNextSiblingItem(hUDFChildItem);
			continue;
		} else {
			m_FormulaTree.SetItemState(hUDFChildItem, TVIS_EXPANDED, TVIS_EXPANDED);
		}

		hNextItem = m_FormulaTree.GetNextSiblingItem(hUDFChildItem);
		hNextLevelItem = m_FormulaTree.GetChildItem(hUDFChildItem);
		if (hNextLevelItem != NULL && m_FormulaTree.GetNextSiblingItem(hNextLevelItem) == NULL) 
		{
			MoveTreeItem(hNextLevelItem, hUDFItem, NULL, false);
			m_FormulaTree.DeleteItem(hUDFChildItem);
		}
		hUDFChildItem = hNextItem;
	}
}

void CDlgFormulaScintilla::GetGroupName(const char *functionName, CString &groupName)
{
	groupName = functionName;
	if (groupName.Find("f$") != 0)
		groupName.Empty();
	else 
	{
		int index = groupName.Find("_");
		if (index < 2)
			groupName.Empty();
		else
			groupName = groupName.Mid(2, index-2);
	}
}

void CDlgFormulaScintilla::GroupUDFs()
{
	HTREEITEM hUDFChildItem = m_FormulaTree.GetChildItem(hUDFItem);
	HTREEITEM hNextItem = NULL;
	CString groupName = "", itemText = "";

	while (hUDFChildItem != NULL)
	{
		hNextItem = m_FormulaTree.GetNextSiblingItem(hUDFChildItem);
		if (!m_FormulaTree.ItemHasChildren(hUDFChildItem)) {
			itemText = m_FormulaTree.GetItemText(hUDFChildItem);
			GetGroupName(itemText, groupName);
			if (!groupName.IsEmpty()) {
				HTREEITEM hExistingGroup = FindUDFGroupItem(groupName);
				if (!hExistingGroup)
					hExistingGroup = m_FormulaTree.InsertItem(groupName, hUDFItem);
				MoveTreeItem(hUDFChildItem, hExistingGroup, itemText, false);
			}
		}
		hUDFChildItem = hNextItem;
	}

	RemoveSingleItemGroups();
}

void CDlgFormulaScintilla::UngroupUDFs()
{
	HTREEITEM hUDFChildItem = m_FormulaTree.GetChildItem(hUDFItem);
	HTREEITEM hGroupedUDFItem = NULL, hNextItem = NULL;

	while (hUDFChildItem != NULL)
	{
		if (!m_FormulaTree.ItemHasChildren(hUDFChildItem)) 
		{
			hUDFChildItem = m_FormulaTree.GetNextSiblingItem(hUDFChildItem);
			continue;
		}

		hGroupedUDFItem = m_FormulaTree.GetChildItem(hUDFChildItem);
		while (hGroupedUDFItem) 
		{
			hNextItem = m_FormulaTree.GetNextSiblingItem(hGroupedUDFItem);
			MoveTreeItem(hGroupedUDFItem, hUDFItem, NULL, false);
			hGroupedUDFItem = hNextItem;
		}
		hNextItem = m_FormulaTree.GetNextSiblingItem(hUDFChildItem);
		m_FormulaTree.DeleteItem(hUDFChildItem);
		hUDFChildItem = hNextItem;
	}
}

void CDlgFormulaScintilla::ConditionallyAddFunction(const CString &name, const CString &content, const CString &filter, HTREEITEM hParent)
{
	HTREEITEM hItem;
	if (filter.IsEmpty() || name.Find(filter) >= 0 || content.Find(filter) >= 0)
	{
		hItem = m_FormulaTree.InsertItem(name, hParent);
		m_FormulaTree.SetItemData(hItem, (DWORD_PTR)FindScintillaWindow(name));
	}
}

void CDlgFormulaScintilla::PopulateFormulaTree()
{
	m_FormulaTree.DeleteAllItems();

	CString filter;
	m_SearchEdit.GetWindowText(filter);

	// Setup the tree
	HTREEITEM	parent = NULL, hItem;

	for (int j=0; j<m_standard_headings.GetSize(); j++)
	{
		if (m_standard_headings[j].IsEmpty()) {
			parent = NULL;
		} else {
			parent = m_FormulaTree.InsertItem(m_standard_headings[j]);
			m_FormulaTree.SetItemState(parent, TVIS_BOLD | (m_standard_expand[j] ? TVIS_EXPANDED : 0), TVIS_BOLD | (m_standard_expand[j] ? TVIS_EXPANDED : 0) );
		}
		for (int i=0; i<m_standard_functions[j].GetSize(); i++)
		{
			ConditionallyAddFunction(m_standard_functions[j].GetAt(i), m_wrk_formula.GetFunctionText(m_standard_functions[j].GetAt(i)), filter, parent);
		}
	}

	parent = m_FormulaTree.InsertItem("Hand Lists");
	m_FormulaTree.SetItemState(parent, TVIS_BOLD | (prefs.expand_list() ? TVIS_EXPANDED : 0), TVIS_BOLD | (prefs.expand_list() ? TVIS_EXPANDED : 0) );
	
	for (int i=0; i<m_wrk_formula.formula()->mHandList.GetSize(); i++) 
	{
		hItem = m_FormulaTree.InsertItem(m_wrk_formula.formula()->mHandList[i].list, parent);
		m_FormulaTree.SetItemData(hItem, (DWORD_PTR)FindScintillaWindow(m_wrk_formula.formula()->mHandList[i].list));
	}

	hUDFItem = parent = m_FormulaTree.InsertItem("User Defined Functions");
	m_FormulaTree.SetItemState(parent, TVIS_BOLD | (prefs.expand_udf() ? TVIS_EXPANDED : 0), TVIS_BOLD | (prefs.expand_udf() ? TVIS_EXPANDED : 0) );
	m_udf_group = prefs.udf_group();

	for (int i=0; i<m_wrk_formula.formula()->mFunction.GetSize(); i++) 
	{
		bool bIsStandardFunction = false;
		for (int k=0;k<4;k++)
		{
			for (int j=0;j<m_standard_functions[k].GetSize(); j++)
			{
				if (!m_standard_functions[k].GetAt(j).Compare(m_wrk_formula.formula()->mFunction[i].func)) {
					bIsStandardFunction = true;
					break;
				}
			}
		}
		if (!bIsStandardFunction)
			ConditionallyAddFunction(m_wrk_formula.formula()->mFunction[i].func, m_wrk_formula.formula()->mFunction[i].func_text, filter, hUDFItem);
	}

	if (m_udf_group)
		GroupUDFs();
}

BOOL CDlgFormulaScintilla::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if (m_hDialogAccelTable != NULL &&  ::TranslateAccelerator(m_hWnd, m_hDialogAccelTable, pMsg))
			return TRUE;
		if (m_pActiveScinCtrl && GetFocus() == m_pActiveScinCtrl) 
		{
			if (m_hEditAccelTable != NULL &&  ::TranslateAccelerator(m_hWnd, m_hEditAccelTable, pMsg))
				return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFormulaScintilla::ResizeDialogForControlBars(void) 
{
	// Figure out how big the control bar(s) are.
	CRect rcClientStart;
	CRect rcClientNow;

	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNow);

	// Move all the controls so they are in the same relative position within the remaining client area as
	// they would be with no control bar(s).
	CPoint ptOffset(rcClientNow.left - rcClientStart.left, rcClientNow.top - rcClientStart.top);
	CRect  rcChild;
	CWnd* pwndChild = GetWindow(GW_CHILD);
	while (pwndChild) 
	{
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcChild, false);
		pwndChild = pwndChild->GetNextWindow();
	}

	// Adjust the dialog window dimensions to make room for the control bar(s)
	//CRect rcWindow;
	//GetWindowRect(rcWindow);
	//rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	//rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	//MoveWindow(rcWindow, false);

	// Position the control bar(s)
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}

void CDlgFormulaScintilla::OnTvnSelchangingFormulaTree(NMHDR *pNMHDR, LRESULT *pResult) 
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	LastChangeToFormula(&m_wrk_formula);

	*pResult = 0;
}

CScintillaWnd *CDlgFormulaScintilla::FindScintillaWindow(const char *name)
{
	for (int i=0; i<m_ScinArray.GetSize(); i++)
	{
		if (!m_ScinArray.GetAt(i)._name.Compare(name))
			return m_ScinArray.GetAt(i)._pWnd;
	}
	return NULL;
}

HTREEITEM CDlgFormulaScintilla::FindFormulaWithWindow(const CScintillaWnd *pWnd, HTREEITEM hFirstItem)
{
	HTREEITEM hItem = hFirstItem;
	while (hItem) {
		if (m_FormulaTree.GetChildItem(hItem)) {
			HTREEITEM hFound = FindFormulaWithWindow(pWnd, m_FormulaTree.GetChildItem(hItem));
			if (hFound)
				return hFound;
		} else if (pWnd == reinterpret_cast<CScintillaWnd *>(m_FormulaTree.GetItemData(hItem)))
			return hItem;
		hItem = m_FormulaTree.GetNextSiblingItem(hItem);
	}

	return NULL;
}

void CDlgFormulaScintilla::OnFunctionTabSelectionChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iSel = m_FunctionTab.GetCurSel();
	HTREEITEM hItem = FindFormulaWithWindow(m_ScinArray[iSel]._pWnd, m_FormulaTree.GetRootItem());
	ASSERT(hItem != NULL);
	if (hItem)
		m_FormulaTree.SelectItem(hItem);
}

void CDlgFormulaScintilla::OnTabSelectionChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	bool bFormulaShowing = (m_TabControl.GetCurSel() == 0);

	if (!bFormulaShowing)
		PopulateSymbols();

	m_FormulaTree.ShowWindow(bFormulaShowing ? SW_SHOW : SW_HIDE);
	m_SymbolTree.ShowWindow(!bFormulaShowing ? SW_SHOW : SW_HIDE);
	m_FormulaTree.EnableWindow(bFormulaShowing);
	m_SymbolTree.EnableWindow(!bFormulaShowing);

	HandleEnables(true);
}

void CDlgFormulaScintilla::OnSymbolTreeTipInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVGETINFOTIP lpGetInfoTip = (LPNMTVGETINFOTIP)pNMHDR;

	if (lpGetInfoTip && lpGetInfoTip->hItem)
	{
		const char *description = (const char *)m_SymbolTree.GetItemData(lpGetInfoTip->hItem);

		if (description)
			strcpy_s(lpGetInfoTip->pszText, lpGetInfoTip->cchTextMax, description);
	}

	*pResult = 0;
}

void CDlgFormulaScintilla::OnSymbolContextMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint cursorPos, clientPos;
	cursorPos.x= GetCurrentMessage()->pt.x;
	cursorPos.y= GetCurrentMessage()->pt.y;
	clientPos = cursorPos;
	m_SymbolTree.ScreenToClient(&clientPos);
	HTREEITEM hItem = m_SymbolTree.HitTest(clientPos);

	if (hItem != NULL && m_SymbolTree.GetChildItem(hItem) == NULL) 
	{
		CMenu contextMenu;
		contextMenu.LoadMenu(IDR_FORMULA_SYMBOL_MENU);
		int ret = contextMenu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, cursorPos.x, cursorPos.y, this);
		if (ret == ID_FORMULA_SYMBOL_COPY) 
		{
			if (OpenClipboard() && EmptyClipboard()) 
			{
				CString symbol;
				symbol = m_SymbolTree.GetItemText(hItem);

				HANDLE hMem = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE , symbol.GetLength()+1);
				if (hMem) 
				{
					LPSTR lpStr = (LPSTR)::GlobalLock(hMem);

					strcpy_s(lpStr, symbol.GetLength() + 1, symbol.GetString());
					::GlobalUnlock(hMem);

					if ( ::SetClipboardData( CF_TEXT, hMem ) != NULL )
						CloseClipboard();
				} 
				else
					CloseClipboard();
			}
		}
	}


	*pResult = 0;
}

void CDlgFormulaScintilla::SelectFunctionTab(CScintillaWnd *pCurScin)
{
	int iSel = -1;
	for (int iWnd=0;iWnd<m_ScinArray.GetSize();iWnd++)
	{
		if (m_ScinArray[iWnd]._pWnd == pCurScin) {
			iSel = iWnd;
			break;
		}
	}
	if (iSel != -1)
		m_FunctionTab.SetCurSel(iSel);
}

void CDlgFormulaScintilla::OnTvnExpandedFormulaTree(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SaveSettingsToRegistry();
}

void CDlgFormulaScintilla::OnTvnSelchangedFormulaTree(NMHDR *pNMHDR, LRESULT *pResult) 
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CString			s = "";
	int				N = 0, i = 0;
	CMenu			*edit_menu = this->GetMenu()->GetSubMenu(1);

	ok_to_update_debug = false;

	s = m_FormulaTree.GetItemText(m_FormulaTree.GetSelectedItem());

	StopAutoButton();

	// A root item was selected or a UDF group item
	if ((m_FormulaTree.GetParentItem(m_FormulaTree.GetSelectedItem()) == NULL && s != "notes") || m_FormulaTree.ItemHasChildren(m_FormulaTree.GetSelectedItem()))
	{
		if (m_pActiveScinCtrl)
		{
			m_pActiveScinCtrl->ShowWindow(SW_HIDE);
			m_pActiveScinCtrl->EnableWindow(false);
		}
		m_pActiveScinCtrl = &m_EmptyScinCtrl;
		m_pActiveScinCtrl->ShowWindow(SW_SHOW);
		m_pActiveScinCtrl->EnableWindow(false);
	}

	// A child item was selected
	else 
	{
		if (s == "notes" || s == "dll") 
		{
			SetWindowText("Formula Editor - " + s);
			N = (int) m_wrk_formula.formula()->mFunction.GetSize();
			for (i=0; i<N; i++) 
			{
				if (m_wrk_formula.formula()->mFunction[i].func == s) 
				{
					CScintillaWnd *pCurScin = reinterpret_cast<CScintillaWnd *>(m_FormulaTree.GetItemData(m_FormulaTree.GetSelectedItem()));
					if (!pCurScin) 
					{
						pCurScin = SetupScintilla(NULL, m_wrk_formula.formula()->mFunction[i].func.GetString());
						m_FormulaTree.SetItemData(m_FormulaTree.GetSelectedItem(), (DWORD_PTR)pCurScin);

						pCurScin->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
						pCurScin->SetText(m_wrk_formula.formula()->mFunction[i].func_text.GetString());
						pCurScin->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
						pCurScin->SendMessage(SCI_EMPTYUNDOBUFFER, 0, 0);
					}

					ASSERT(pCurScin != NULL);
					if (m_pActiveScinCtrl) 
					{
						m_pActiveScinCtrl->ShowWindow(SW_HIDE);
						m_pActiveScinCtrl->EnableWindow(false);
					}
					m_pActiveScinCtrl = pCurScin;
					m_pActiveScinCtrl->ShowWindow(SW_SHOW);
					m_pActiveScinCtrl->EnableWindow(true);
					SelectFunctionTab(pCurScin);

					m_current_edit = m_wrk_formula.formula()->mFunction[i].func;
					i=N+1;
				}
			}
		}
		else if (memcmp(s.GetString(), "f$", 2) == 0) 
		{
			SetWindowText("Formula Editor - " + s);
			N = (int) m_wrk_formula.formula()->mFunction.GetSize();
			for (i=0; i<N; i++) 
			{
				if (m_wrk_formula.formula()->mFunction[i].func == s) 
				{
					CScintillaWnd *pCurScin = reinterpret_cast<CScintillaWnd *>(m_FormulaTree.GetItemData(m_FormulaTree.GetSelectedItem()));
					if (!pCurScin) 
					{
						pCurScin = SetupScintilla(NULL, m_wrk_formula.formula()->mFunction[i].func.GetString());
						m_FormulaTree.SetItemData(m_FormulaTree.GetSelectedItem(), (DWORD_PTR)pCurScin);

						pCurScin->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
						pCurScin->SetText(m_wrk_formula.formula()->mFunction[i].func_text.GetString());
						pCurScin->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
						pCurScin->SendMessage(SCI_EMPTYUNDOBUFFER, 0, 0);
					}

					ASSERT(pCurScin != NULL);
					if (m_pActiveScinCtrl) 
					{
						m_pActiveScinCtrl->ShowWindow(SW_HIDE);
						m_pActiveScinCtrl->EnableWindow(false);
					}
					m_pActiveScinCtrl = pCurScin;
					m_pActiveScinCtrl->ShowWindow(SW_SHOW);
					m_pActiveScinCtrl->EnableWindow(true);
					SelectFunctionTab(pCurScin);

					m_current_edit = m_wrk_formula.formula()->mFunction[i].func;
					i=N+1;
				}
			}
		}
		else if (memcmp(s.GetString(), "list", 4) == 0) 
		{
			// Find proper list and display it
			N = (int) m_wrk_formula.formula()->mHandList.GetSize();
			for (i=0; i<N; i++) {
				if (m_wrk_formula.formula()->mHandList[i].list == s) 
				{
					SetWindowText("Formula Editor - " + s);

					CScintillaWnd *pCurScin = reinterpret_cast<CScintillaWnd *>(m_FormulaTree.GetItemData(m_FormulaTree.GetSelectedItem()));
					if (!pCurScin) 
					{
						pCurScin = SetupScintilla(NULL, m_wrk_formula.formula()->mHandList[i].list.GetString());
						m_FormulaTree.SetItemData(m_FormulaTree.GetSelectedItem(), (DWORD_PTR)pCurScin);

						pCurScin->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
						pCurScin->SetText(m_wrk_formula.formula()->mHandList[i].list_text.GetString());
						pCurScin->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
						pCurScin->SendMessage(SCI_EMPTYUNDOBUFFER, 0, 0);
					}

					ASSERT(pCurScin != NULL);
					if (m_pActiveScinCtrl) 
					{
						m_pActiveScinCtrl->ShowWindow(SW_HIDE);
						m_pActiveScinCtrl->EnableWindow(false);
					}
					m_pActiveScinCtrl = pCurScin;
					m_pActiveScinCtrl->ShowWindow(SW_SHOW);
					m_pActiveScinCtrl->EnableWindow(true);
					SelectFunctionTab(pCurScin);

					m_current_edit = m_wrk_formula.formula()->mHandList[i].list;
					i=N+1;
				}
			}
		}
	}

	if (s == "f$debug") 
	{
		InitDebugArray();
	}

	HandleEnables(true);

	*pResult = 0;
}

HTREEITEM CDlgFormulaScintilla::FindUDFGroupItem(const char *groupName)
{
	CString tempString = "";

	HTREEITEM searchItem = m_FormulaTree.GetChildItem(hUDFItem);
	while (searchItem) 
	{
		if (m_FormulaTree.ItemHasChildren(searchItem)) 
		{
			tempString = m_FormulaTree.GetItemText(searchItem);
			if (!tempString.Compare(groupName))
				return searchItem;
		}
		searchItem = m_FormulaTree.GetNextSiblingItem(searchItem);
	}

	return NULL;
}

HTREEITEM CDlgFormulaScintilla::FindUDFStartingItem(const char *groupName)
{
	CString searchString = "", tempString = "";

	searchString.Format("f$%s_", groupName);
	HTREEITEM searchItem = m_FormulaTree.GetChildItem(hUDFItem);
	
	while (searchItem) 
	{
		if (!m_FormulaTree.ItemHasChildren(searchItem)) 
		{
			tempString = m_FormulaTree.GetItemText(searchItem);

			if (tempString.Find(searchString) == 0)
				return searchItem; 
		}
		searchItem = m_FormulaTree.GetNextSiblingItem(searchItem);
	}

	return NULL;
}

HTREEITEM CDlgFormulaScintilla::MoveTreeItem(HTREEITEM hItem, HTREEITEM hNewParent, const char *name, bool bSelect)
{
	HTREEITEM hMovedItem = NULL;

	if (name)
		hMovedItem = m_FormulaTree.InsertItem(name, hNewParent);
	else 
	{
		CString itemName;
		itemName = m_FormulaTree.GetItemText(hItem);
		hMovedItem = m_FormulaTree.InsertItem(itemName, hNewParent);
	}

	m_FormulaTree.SetItemData(hMovedItem, m_FormulaTree.GetItemData(hItem));
	m_FormulaTree.DeleteItem(hItem);

	if (bSelect)
		m_FormulaTree.SelectItem(hMovedItem);

	return hMovedItem;
}

void CDlgFormulaScintilla::OnNew() 
{
	CDlgNew newdlg;
	SFunction Func;
	SHandList List;
	CString	s = "";
	HTREEITEM	p = NULL, newhtitem = NULL;

	StopAutoButton();

	s = m_FormulaTree.GetItemText(m_FormulaTree.GetSelectedItem());

	if (s == "Hand Lists" || (s.Find("list") != -1 && s.Find("f$") == -1))
		newdlg.type = 0;
	else
		newdlg.type = 1;

	if (newdlg.DoModal() == IDOK) 
	{
		if (newdlg.type == 0) 
		{
			bool bAlreadyExists = false;
			for (int i=0; i<m_wrk_formula.formula()->mHandList.GetSize(); i++)
			{
				if (m_wrk_formula.formula()->mHandList[i].list == newdlg.CSnewname) 
				{
					bAlreadyExists = true;
					break;
				}
			}
			if (bAlreadyExists) 
			{
				MessageBox("Cannot proceed as list already exists");
				return;
			}

			List.list = newdlg.CSnewname;
			List.list_text = "";

			// Add it to working set CArray
			m_wrk_formula.set_list_add(List);

			// Add to tree
			p = m_FormulaTree.GetParentItem(m_FormulaTree.GetSelectedItem());
			if (p == NULL) // Selected item is a parent item
			{  
				newhtitem = m_FormulaTree.InsertItem(newdlg.CSnewname, m_FormulaTree.GetSelectedItem());
			}
			else 
			{
				newhtitem = m_FormulaTree.InsertItem(newdlg.CSnewname, p);
			}
		}
		else 
		{
			bool bAlreadyExists = false;
			for (int i=0; i<m_wrk_formula.formula()->mFunction.GetSize(); i++) 
			{
				if (m_wrk_formula.formula()->mFunction[i].func == newdlg.CSnewname) 
				{
					bAlreadyExists = true;
					break;
				}
			}

			if (bAlreadyExists) 
			{
				MessageBox("Cannot proceed as this function already exists");
				return;
			}

			Func.func = newdlg.CSnewname;
			Func.func_text = "";
			Func.fresh = false;

			// Add it to working set CArray
			m_wrk_formula.set_func_add(Func);

			// Add to tree
			HTREEITEM hNewParent = hUDFItem;
			if (m_udf_group) 
			{
				CString tempString;
				CString groupName;
				GetGroupName(newdlg.CSnewname, groupName);
				if (!groupName.IsEmpty()) 
				{
					// Does a group already exist?
					HTREEITEM hExistingGroup = FindUDFGroupItem(groupName);
					if (hExistingGroup) 
						hNewParent = hExistingGroup;
					else 
					{
						// If a group does not exist, is there another UDF to group together?
						HTREEITEM matchingItem = FindUDFStartingItem(groupName);
						if (matchingItem) 
						{
							hNewParent = m_FormulaTree.InsertItem(groupName, hUDFItem);
							MoveTreeItem(matchingItem, hNewParent, NULL, false);
						}
					}
				}
			}
			newhtitem = m_FormulaTree.InsertItem(newdlg.CSnewname, hNewParent);
			if (m_udf_sort)
				SortUdfTree();
		}

		UpdateAllScintillaKeywords();

		// Select newly inserted item
		m_FormulaTree.SelectItem(newhtitem);
		m_FormulaTree.SetFocus();
		m_dirty = true;
		HandleEnables(true);
	}
}

void CDlgFormulaScintilla::OnRename() 
{
	CDlgRename rendlg;
	CString	s = "";
	char str[512] = {0};
	int N = 0, i = 0;
	bool didRename = false;

	StopAutoButton();

	s = m_FormulaTree.GetItemText(m_FormulaTree.GetSelectedItem());
	strcpy_s(str, 512, s.GetString());

	rendlg.CSoldname = s;

	if (rendlg.DoModal() == IDOK) 
	{
		bool bRenameUDF = false;
		// Renaming a list
		if (memcmp(str, "list", 4) == 0) 
		{
			bool bAlreadyExists = false;
			for (i=0; i<m_wrk_formula.formula()->mHandList.GetSize(); i++)
			{
				if (m_wrk_formula.formula()->mHandList[i].list == rendlg.CSnewname) 
				{
					bAlreadyExists = true;
					break;
				}
			}
			if (bAlreadyExists) 
			{
				MessageBox("Cannot rename to a list that already exists");
				PostMessage(WM_COMMAND, ID_FORMULA_EDIT_RENAME);
			} 
			else 
			{
				// Find proper list
				N = (int) m_wrk_formula.formula()->mHandList.GetSize();
				for (i=0; i<N; i++) 
				{
					if (m_wrk_formula.formula()->mHandList[i].list == rendlg.CSoldname) 
					{
						// Update it in the CArray working set
						m_wrk_formula.set_list_list(i, rendlg.CSnewname);
						didRename = true;
						break;
					}
				}
			}
		}

		// Renaming a UDF
		else if (memcmp(str, "f$", 2) == 0) 
		{
			bRenameUDF = true;

			bool bAlreadyExists = false;
			for (i=0; i<m_wrk_formula.formula()->mFunction.GetSize(); i++) 
			{
				if (m_wrk_formula.formula()->mFunction[i].func == rendlg.CSnewname) 
				{
					bAlreadyExists = true;
					break;
				}
			}

			if (bAlreadyExists) 
			{
				MessageBox("Cannot rename to a formula that already exists");
				PostMessage(WM_COMMAND, ID_FORMULA_EDIT_RENAME);
			} 
			else 
			{
				// Find proper UDF and display it
				N = (int) m_wrk_formula.formula()->mFunction.GetSize();
				for (i=0; i<N; i++) 
				{
					if (m_wrk_formula.formula()->mFunction[i].func == s) 
					{
						// Update it in the CArray working set
						m_wrk_formula.set_func_func(i, rendlg.CSnewname);
						m_wrk_formula.set_func_fresh(i, false);

						didRename = true;
						break;
					}
				}
			}
		}
		if (didRename) 
		{
			UpdateAllScintillaKeywords();
			HTREEITEM hSelectedItem = m_FormulaTree.GetSelectedItem();
			if (!bRenameUDF || !m_udf_group)
				m_FormulaTree.SetItemText(hSelectedItem, rendlg.CSnewname);
			else 
			{
				CString tempString, groupName = rendlg.CSnewname;
				HTREEITEM oldParentItem = m_FormulaTree.GetParentItem(hSelectedItem);
				HTREEITEM hNewParent = hUDFItem;
				CString parentName = m_FormulaTree.GetItemText(oldParentItem);
				if (groupName.Find("f$") == 0) 
				{
					int index = groupName.Find("_");
					if (index > 0) 
					{
						groupName = groupName.Mid(2, index-2);
						if (parentName.Compare(groupName)) 
						{
							// Does a group already exist?
							HTREEITEM hExistingGroup = FindUDFGroupItem(groupName);
							if (hExistingGroup)
								hNewParent = hExistingGroup;
							else 
							{
								// If a group does not exist, is there another UDF to group together?
								HTREEITEM matchingItem = FindUDFStartingItem(groupName);
								if (matchingItem) 
								{
									hNewParent = m_FormulaTree.InsertItem(groupName, hUDFItem);
									MoveTreeItem(matchingItem, hNewParent, NULL, false);
								}
							}
						}
					}
				}
				if (oldParentItem != hNewParent) 
				{
					MoveTreeItem(hSelectedItem, hNewParent, rendlg.CSnewname, true);
					if (oldParentItem != hUDFItem) 
					{
						HTREEITEM hOldSiblingItem = m_FormulaTree.GetChildItem(oldParentItem);
						if (hOldSiblingItem != NULL && m_FormulaTree.GetNextSiblingItem(hOldSiblingItem) == NULL) 
						{
							MoveTreeItem(hOldSiblingItem, hUDFItem, NULL, false);
							m_FormulaTree.DeleteItem(oldParentItem);
						}
					}
				} else
					m_FormulaTree.SetItemText(hSelectedItem, rendlg.CSnewname);
				if (m_udf_sort && memcmp(str, "f$", 2) == 0)
					SortUdfTree();
			}
			SetWindowText("Formula - " + rendlg.CSnewname);
			m_FormulaTree.SetFocus();
			m_dirty = true;
			HandleEnables(true);
		}
	}
}

void CDlgFormulaScintilla::OnDelete() 
{
	int ret = 0, i = 0;
	HTREEITEM hItem = m_FormulaTree.GetSelectedItem();
	CString s = m_FormulaTree.GetItemText(hItem);
	CMenu *file_menu = this->GetMenu()->GetSubMenu(0);

	StopAutoButton();

	if (IDYES != MessageBox("REALLY delete \"" + m_FormulaTree.GetItemText(m_FormulaTree.GetSelectedItem()) + "\" ?", "Confirm Delete", MB_YESNO | MB_ICONWARNING)) 
	{
		HandleEnables(true);
		return;
	}

	bool bDeleted = false;
	// Delete a UDF
	if (s.Find("f$") != -1) 
	{
		for (i=0; i<m_wrk_formula.formula()->mFunction.GetSize(); i++) 
		{
			if (!m_wrk_formula.formula()->mFunction[i].func.Compare(s)) {
				bDeleted = true;
				m_wrk_formula.set_func_remove(i);

				HTREEITEM oldParentItem = m_FormulaTree.GetParentItem(hItem);
				m_FormulaTree.DeleteItem(hItem);
				if (m_udf_group) 
				{
					HTREEITEM hOldSiblingItem = m_FormulaTree.GetChildItem(oldParentItem);
					if (hOldSiblingItem && m_FormulaTree.GetNextSiblingItem(hOldSiblingItem) == NULL) 
					{
						MoveTreeItem(hOldSiblingItem, hUDFItem, NULL, true);
						m_FormulaTree.DeleteItem(oldParentItem);
					}
				}
				break;
			}
		}
	}

	// Delete a list
	else if (s.Find("list") != -1) 
	{
		for (i=0; i<m_wrk_formula.formula()->mHandList.GetSize(); i++) 
		{
			if (!m_wrk_formula.formula()->mHandList[i].list.Compare(s)) 
			{
				bDeleted = true;
				m_wrk_formula.set_list_remove(i);

				m_FormulaTree.DeleteItem(hItem);
				break;
			}
		}
	}
	if (bDeleted) {
		m_dirty = true;
		m_FormulaTree.SetFocus();
		SetWindowText("Formula - ");
		if (m_pActiveScinCtrl)
		{
			m_pActiveScinCtrl->ShowWindow(SW_HIDE);
			m_pActiveScinCtrl->EnableWindow(false);
		}
		m_pActiveScinCtrl = &m_EmptyScinCtrl;
		m_pActiveScinCtrl->ShowWindow(SW_SHOW);
		m_pActiveScinCtrl->EnableWindow(false);
	}

	UpdateAllScintillaKeywords();

	HandleEnables(true);
}

void CDlgFormulaScintilla::OnToggleBookmark()
{
	if (!m_pActiveScinCtrl)
		return;

	int iLine = m_pActiveScinCtrl->GetCurrentLine();

	if (m_pActiveScinCtrl->HasBookmark(iLine))
		m_pActiveScinCtrl->DeleteBookmark(iLine);
	else 
	{
		m_pActiveScinCtrl->AddBookmark(iLine);
		if (!m_is_selection_margin_visible)
			ToggleSelectionMargin();
	}
}

void CDlgFormulaScintilla::OnNextBookmark()
{
	if (!m_pActiveScinCtrl)
		return;

	m_pActiveScinCtrl->FindNextBookmark();
}

void CDlgFormulaScintilla::OnPrevBookmark()
{
	if (!m_pActiveScinCtrl)
		return;

	m_pActiveScinCtrl->FindPreviousBookmark();
}

void CDlgFormulaScintilla::CloseFindReplaceDialog()
{
	if (m_pFRDlg && !m_pFRDlg->IsTerminating() && ::IsWindow(m_pFRDlg->GetSafeHwnd()))
		m_pFRDlg->DestroyWindow();

	m_pFRDlg = NULL;
}

void CDlgFormulaScintilla::DoFind(bool DirDown)
{
	m_pActiveScinCtrl->SetSearchflags((m_FindMatchCase ? SCFIND_MATCHCASE : 0)|
									  (m_FindWholeWord ? SCFIND_WORDSTART : 0));

	// Note: I have confirmed that the TextToFind struct for the search text is incorrectly
	// set to a non-const when in actuality the functions using that variable take a const pointer
	// I did not want to change a Scintilla header files (so I did a dirty dirty cast)
	if (DirDown)
		m_pActiveScinCtrl->SearchForward((char *)m_FindLastSearch.GetString());
	else
		m_pActiveScinCtrl->SearchBackward((char *)m_FindLastSearch.GetString());
}

LONG CDlgFormulaScintilla::OnFindReplace(WPARAM, LPARAM lParam)
{
	LPFINDREPLACE lpFindReplace = (LPFINDREPLACE) lParam;

	if (lpFindReplace && m_pActiveScinCtrl) 
	{
		if (lpFindReplace->Flags & FR_DIALOGTERM) 
		{
			CloseFindReplaceDialog();
		} 
		else if (lpFindReplace->Flags & FR_FINDNEXT) 
		{
			m_FindLastSearch = lpFindReplace->lpstrFindWhat;
			m_FindMatchCase = (lpFindReplace->Flags & FR_MATCHCASE ? true : false);
			m_FindWholeWord = (lpFindReplace->Flags & FR_WHOLEWORD ? true : false);
			DoFind((lpFindReplace->Flags & FR_DOWN ? true : false));
			HandleEnables(true);
		} 
		else if (lpFindReplace->Flags & FR_REPLACE) 
		{
		} 
		else if (lpFindReplace->Flags & FR_REPLACEALL) 
		{
		}
	}

	return 0;
}

void CDlgFormulaScintilla::OnFindNext()
{
	DoFind(true);
}

void CDlgFormulaScintilla::OnFindPrev()
{
	DoFind(false);
}

void CDlgFormulaScintilla::OnFindReplaceDlg()
{
	if ( NULL == m_pFRDlg )
	{
		m_pFRDlg = new CFindReplaceDialog();  // Must be created on the heap
		m_pFRDlg->Create(TRUE, "", "", FR_DOWN, this);
		m_pFRDlg->m_fr.lStructSize = sizeof(FINDREPLACE);
		m_pFRDlg->m_fr.hwndOwner = this->m_hWnd;
	}
}

void CDlgFormulaScintilla::OnFont() 
{
	CFontDialog fontdlg;
	LOGFONT curlf, newlf;
	COLORREF g_rgbText = RGB(0, 0, 0);
	int	i = 0;

	editfont.GetLogFont(&curlf);
	fontdlg.m_cf.lStructSize = sizeof(CHOOSEFONT);
	fontdlg.m_cf.Flags |= CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
	fontdlg.m_cf.hwndOwner = NULL;
	fontdlg.m_cf.lpLogFont = &curlf;
	fontdlg.m_cf.rgbColors = g_rgbText;
	fontdlg.m_cf.Flags ^= CF_EFFECTS;
	fontdlg.m_cf.Flags |= CF_NOSCRIPTSEL;

	if (fontdlg.DoModal() == IDOK) 
	{
		// Create the font using the selected font from CFontDialog.
		memcpy(&newlf, fontdlg.m_cf.lpLogFont, sizeof(LOGFONT));
		editfont.DeleteObject();
		editfont.CreateFontIndirect(&newlf);

		// Save the new font
		WriteProfileFont("Font", "Formula", editfont);

		// Set the new font to all Scintilla windows
		for (int iScint=0; iScint<m_ScinArray.GetSize(); iScint++)
		{
			CScintillaWnd *pCurScint = m_ScinArray[iScint]._pWnd;
			for (i=0; i<=MAX_STYLE_NUM; i++) 
			{
				pCurScint->SetFontname(i, fontdlg.m_cf.lpLogFont->lfFaceName);
				pCurScint->SetFontheight(i, -fontdlg.m_cf.lpLogFont->lfHeight);
				pCurScint->SetBold(i, (fontdlg.m_cf.lpLogFont->lfWeight==FW_BOLD ? true : false));
				pCurScint->SetItalic(i, fontdlg.m_cf.lpLogFont->lfItalic);
			}
		}
	}
}

void CDlgFormulaScintilla::OnSettings() 
{
	CDlgSettings		myDialog;
	CMenu				*file_menu = this->GetMenu()->GetSubMenu(0);
	COpenHoldemDoc		*pDoc = COpenHoldemDoc::GetDocument();

	myDialog.bankroll = m_wrk_formula.formula()->dBankroll;
	myDialog.defcon = m_wrk_formula.formula()->dDefcon;
	myDialog.rake = m_wrk_formula.formula()->dRake;
	myDialog.nit = m_wrk_formula.formula()->dNit;

	if (myDialog.DoModal() == IDOK) 
	{
		m_wrk_formula.set_bankroll(myDialog.bankroll);
		m_wrk_formula.set_defcon(myDialog.defcon);
		m_wrk_formula.set_rake(myDialog.rake);
		m_wrk_formula.set_nit(myDialog.nit);

		m_dirty = true;
		pDoc->SetModifiedFlag(true);
		HandleEnables(true);
	}
}

CString CDlgFormulaScintilla::ExtractCommentFromHandList(CString HandListAsString)
{
	int		length = HandListAsString.GetLength();
	bool	inside_comment = false;
	CString	comment = "";

	for (int i=0; i<length; i++)
	{
		char current_char = HandListAsString.GetAt(i);
		// We check only for a single slash as beginning of a comment,
		// but that should be ok for handlists (not for formulas).
		if (current_char == '/')
		{
			inside_comment = true;
		}
		if (inside_comment)
		{
			comment += current_char;
		}
		if (current_char == '\n')
		{
			// Comments end automatically at the end of the line
			inside_comment = false;
		}
	}
	return comment;
}

void CDlgFormulaScintilla::OnHandList() 
{
	CDlgHandList		myDialog;
	CString				s = m_FormulaTree.GetItemText(m_FormulaTree.GetSelectedItem());
	int					list_index = 0, i = 0, j = 0;
	CMenu				*file_menu = this->GetMenu()->GetSubMenu(0);
	CString				token = "", hand = "", newstring = "";
	
	// Find appropriate list in the internal structure
	list_index = -1;
	for (i=0; i<m_wrk_formula.formula()->mHandList.GetSize() && list_index == -1; i++) 
	{
		if (m_wrk_formula.formula()->mHandList[i].list == s)
		{
			list_index = i;
			break;
		}
	}
	if (list_index == -1)  return;

	m_wrk_formula.ParseHandList(m_wrk_formula.formula()->mHandList[list_index].list_text, myDialog.checked);

	// Window title and static text content
	myDialog.hand_list_num = atoi(s.Mid(4).GetString());

	// Start dialog
	if (myDialog.DoModal() == IDOK) 
	{
		CString old_comment = ExtractCommentFromHandList(m_wrk_formula.formula()->mHandList[list_index].list_text);
		CString new_handlist_without_comment = myDialog.GetHandListAsString();
		CString new_handlist_with_comment = old_comment + new_handlist_without_comment;

		// save it internally
		m_wrk_formula.set_list_text(list_index, new_handlist_with_comment);

		// update scintilla window
		CScintillaWnd *pCurScin = reinterpret_cast<CScintillaWnd *>(m_FormulaTree.GetItemData(m_FormulaTree.GetSelectedItem()));
		ASSERT(pCurScin != NULL);
		if (pCurScin) {
			pCurScin->SetText(m_wrk_formula.formula()->mHandList[list_index].list_text.GetString());
		}

		m_dirty = true;
	}
	HandleEnables(true);
}

void CDlgFormulaScintilla::LastChangeToFormula(CFormula *f) 
{
	CString		s = "", text = "";
	int			N = 0, i = 0;

	HTREEITEM hSelected = m_FormulaTree.GetSelectedItem();
	if (hSelected == NULL)
		return;

	s = m_FormulaTree.GetItemText(hSelected);
	CScintillaWnd *pCurScin = reinterpret_cast<CScintillaWnd *>(m_FormulaTree.GetItemData(hSelected));

	// A root item was selected, do nothing
	if (m_FormulaTree.GetParentItem(m_FormulaTree.GetSelectedItem()) == NULL || m_FormulaTree.GetChildItem(m_FormulaTree.GetSelectedItem()) != NULL )
	{
	}
	else if (!pCurScin) 
	{
		ASSERT(false);
	}
	
	// A child item was selected, copy text from rich edit control to working set CArray
	else 
	{
		// check for function
		N = (int) f->formula()->mFunction.GetSize();
		for (i=0; i<N; i++) 
		{
			if (f->formula()->mFunction[i].func == s) 
			{
				pCurScin->GetText(text);
				f->set_func_text(i, text);
				f->set_func_dirty(i, true);
				f->set_func_fresh(i, false);
				return;
			}
		}

		// check for list
		if (memcmp(s.GetString(), "list", 4) == 0) 
		{
			// Find proper list and load it
			N = (int) f->formula()->mHandList.GetSize();
			for (i=0; i<N; i++) 
			{
				if (f->formula()->mHandList[i].list == s) 
				{
					pCurScin->GetText(text);
					f->set_list_text(i, text);
					return;
				}
			}
		}
	}
}

BOOL CDlgFormulaScintilla::DestroyWindow()
{
	COpenHoldemDoc		*pDoc = COpenHoldemDoc::GetDocument();
	CMainFrame			*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	StopAutoButton();
	SaveSettingsToRegistry();
	CloseFindReplaceDialog();
	m_wrk_formula.ClearFormula();
	editfont.DeleteObject();

	// Uncheck formula button on main toolbar
	pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, false);

	return CDialog::DestroyWindow();
}

void CDlgFormulaScintilla::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
	m_formulaScintillaDlg	=	NULL;
}

BOOL CDlgFormulaScintilla::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR *phDR;
	phDR = (NMHDR*)lParam;
	CMenu *file_menu = this->GetMenu()->GetSubMenu(0);
	CString s = "";

	if (in_startup)  
	{
		return true;
	}

	// does notification come from my scintilla control?
	if (phDR != NULL && phDR->hwndFrom == m_pActiveScinCtrl->m_hWnd) 
	{
		SCNotification *pMsg = (SCNotification*)lParam;
		switch (phDR->code) 
		{
		case SCN_STYLENEEDED:
			break;
		case SCN_CHARADDED:
			break;
		case SCN_SAVEPOINTREACHED:
			break;
		case SCN_SAVEPOINTLEFT:
			// called when the document is changed - mark document modified
			break;
		case SCN_MODIFYATTEMPTRO:
			break;
		case SCN_KEY:
			break;
		case SCN_DOUBLECLICK:
			break;
		case SCN_UPDATEUI:
			// called when something changes and we want to show new indicator state or brace matching

			// update status bar
			s.Format("Line: %d, Col: %d", m_pActiveScinCtrl->GetCurrentLine(), m_pActiveScinCtrl->GetCurrentColumn());
			m_wndStatusBar.SetPaneText(1, s);

			m_pActiveScinCtrl->UpdateUI();
			break;
		case SCN_MODIFIED:
			StopAutoButton();

			m_CalcResult.SetWindowText("");

			m_dirty = true;
			HandleEnables(true);

			break;
		case SCN_MACRORECORD:
			break;
		case SCN_MARGINCLICK:
			// user clicked margin - try folding action
			m_pActiveScinCtrl->DoDefaultFolding(pMsg->margin, pMsg->position);
			break;
		case SCN_NEEDSHOWN:
			break;
		case SCN_PAINTED:
			break;
		case SCN_USERLISTSELECTION:
			break;
		case SCN_URIDROPPED:
			break;
		case SCN_DWELLSTART:
			break;
		case SCN_DWELLEND:
			break;
		}
		return TRUE; // we processed the message
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL CDlgFormulaScintilla::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult) 
{
	// allow top level routing frame to handle the message
	if (GetRoutingFrame() != NULL)
		return false;

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[256];
	CString strTipText;
	UINT_PTR nID = (UINT) pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
			pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND)) 
	{
		// idFrom is actually the HWND of the tool
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	if (nID != 0) 
	{ // will be zero on a separator
		AfxLoadString(nID, szFullText);
		strTipText=szFullText;
	}

	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));

	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,SWP_NOACTIVATE|
				   SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);

	return true;
}

void CDlgFormulaScintilla::ToggleToolbar() 
{
	CMenu *view_menu = this->GetMenu()->GetSubMenu(2);

	if (m_toolBar.IsWindowVisible()) 
	{
		m_toolBar.ShowWindow(SW_HIDE);
		view_menu->CheckMenuItem(VIEW_TOOLBAR, MF_BYPOSITION | MF_UNCHECKED);
		m_is_toolbar_visible = false;
	}
	else 
	{
		m_toolBar.ShowWindow(SW_SHOWNA);
		view_menu->CheckMenuItem(VIEW_TOOLBAR, MF_BYPOSITION | MF_CHECKED);
		m_is_toolbar_visible = true;
	}

	int xadj = 0;
	int yadj = m_is_toolbar_visible ? 26 : 0;  // allow buffer for toolbar on top
	int botadj = 16;  // allow for status bar on bottom
	RECT rect;
	GetClientRect(&rect);
	m_winMgr.CalcLayout(xadj, yadj, rect.right-rect.left, rect.bottom-rect.top-botadj, this);
	m_winMgr.SetWindowPositions(this);
	m_winMgr.CalcLayout(xadj, yadj, rect.right-rect.left, rect.bottom-rect.top-botadj, this);
	m_winMgr.SetWindowPositions(this);
}

void CDlgFormulaScintilla::ToggleLineNumbers() 
{
	m_are_linenumbers_visible = !m_are_linenumbers_visible;
	m_pActiveScinCtrl->SetDisplayLinenumbers(m_are_linenumbers_visible);

	HandleEnables(true);
}

void CDlgFormulaScintilla::ToggleSelectionMargin() 
{
	m_is_selection_margin_visible = !m_is_selection_margin_visible;
	m_pActiveScinCtrl->SetDisplaySelection(m_is_selection_margin_visible);

	HandleEnables(true);
}

void CDlgFormulaScintilla::ToggleFoldingMargin() 
{
	m_is_folding_margin_visible = !m_is_folding_margin_visible;
	m_pActiveScinCtrl->SetDisplayFolding(m_is_folding_margin_visible);

	HandleEnables(true);
}

void CDlgFormulaScintilla::ToggleSyntaxColoring() 
{
	m_is_syntax_colored = !m_is_syntax_colored;
	SetStyleColors(m_pActiveScinCtrl, m_is_syntax_colored);

	HandleEnables(true);
}

LRESULT CDlgFormulaScintilla::OnWinMgr(WPARAM wp, LPARAM lp) 
{
	ASSERT(lp);
	NMWINMGR& nmw = *(NMWINMGR*)lp;

	if (nmw.code==NMWINMGR::GET_SIZEINFO) 
	{
		if (wp==(WORD)GetDlgCtrlID()) 
		{
			// Parent frame is requesting my size info. Report min size.
			m_winMgr.GetMinMaxInfo(this, nmw.sizeinfo);
			return true; // handled--important!
		}
	}

	else if (nmw.code==NMWINMGR::SIZEBAR_MOVED) 
	{
		// User moved a sizer bar: call WinMgr to do it!
		m_winMgr.MoveRect(wp, nmw.sizebar.ptMoved, this);
		m_winMgr.SetWindowPositions(this);

		ResizeScintillaWindows();

		if (::IsWindow(m_FormulaTree.GetSafeHwnd()))
		{
			CRect rc;
			m_FormulaTree.GetWindowRect(rc);
			ScreenToClient(rc);
			m_SymbolTree.SetWindowPos(NULL, rc.left,rc.top,rc.Width(),rc.Height(), SWP_NOZORDER);
		}

		return true;
	}

	return false; // not handled
}

void CDlgFormulaScintilla::OnBnClickedLessPrecision() 
{
	CString s = "";

	if (m_precision > 0) 
	{
		m_precision--;
		CreateDebugTab(&s);
		m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
		m_pActiveScinCtrl->SetText(s.GetString());
		m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
	}
}

void CDlgFormulaScintilla::OnBnClickedMorePrecision() 
{
	CString s = "";

	m_precision++;
	CreateDebugTab(&s);
	m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
	m_pActiveScinCtrl->SetText(s.GetString());
	m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
}

void CDlgFormulaScintilla::OnBnClickedEqualLeft() 
{
	CString s = "";

	if (m_equal > 0) 
	{
		m_equal--;
		CreateDebugTab(&s);
		m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
		m_pActiveScinCtrl->SetText(s.GetString());
		m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
	}
}

void CDlgFormulaScintilla::OnBnClickedEqualRight() 
{
	CString s = "";

	m_equal++;
	CreateDebugTab(&s);
	m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
	m_pActiveScinCtrl->SetText(s.GetString());
	m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
}

void CDlgFormulaScintilla::ResizeScintillaWindows()
{
	if (::IsWindow(m_EmptyScinCtrl.GetSafeHwnd())) 
	{
		CRect rc;
		m_EmptyScinCtrl.GetWindowRect(rc);
		ScreenToClient(rc);

		HDWP hdwp = ::BeginDeferWindowPos(m_ScinArray.GetSize()-1);

		for (int i=1; i<m_ScinArray.GetSize(); i++)
		{
			::DeferWindowPos(hdwp, m_ScinArray[i]._pWnd->GetSafeHwnd(), NULL, rc.left,rc.top,rc.Width(),rc.Height(), SWP_NOZORDER);
		}
		::EndDeferWindowPos(hdwp);
	}
}

void CDlgFormulaScintilla::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	int xadj = 0;
	int yadj = m_is_toolbar_visible ? 26 : 0;  // allow buffer for toolbar on top
	int botadj = 16;  // allow for status bar on bottom

	m_winMgr.CalcLayout(xadj, yadj, cx, cy-botadj, this);
	m_winMgr.SetWindowPositions(this);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	ResizeScintillaWindows();

	if (::IsWindow(m_FormulaTree.GetSafeHwnd()))
	{
		CRect rc;
		m_FormulaTree.GetWindowRect(rc);
		ScreenToClient(rc);
		m_SymbolTree.SetWindowPos(NULL, rc.left,rc.top,rc.Width(),rc.Height(), SWP_NOZORDER);
	}
}

void CDlgFormulaScintilla::OnBnClickedCalc() 
{
	CString					Cstr = "", title = "", s = "";
	double					ret = 0.;
	int						error = 0;
	std::string				str = "";
	struct SDebugTabInfo	debug_struct;
	LARGE_INTEGER			bcount = {0}, ecount = {0}, lFrequency = {0};
	char					format[50] = {0};

	StopAutoButton();

	// Clear result box
	m_CalcResult.SetWindowText("");

	// Caclulate symbols
	//symbols.CalcSymbols();

	// mark symbol result cache as stale
	m_wrk_formula.MarkCacheStale();

	// Make sure we "calc" the latest
	LastChangeToFormula(&m_wrk_formula);

	// calc hand lists
	m_wrk_formula.CreateHandListMatrices();

	// Validate parse trees
	if (!m_wrk_formula.ParseAllFormula(this->GetSafeHwnd(), false))
	{
		s.Format("There are syntax errors in one or more formulas that are\n");
		s.Append("preventing calculation.\n");
		s.Append("These errors need to be corrected before the 'Calc'\n");
		s.Append("button can be used.");
		MessageBox(s, "PARSE ERROR(s)", MB_OK);
		return;
	}

	//
	// f$debug is a special case, handle it first
	//
	if (m_current_edit == "f$debug") 
	{
		LastChangeToFormula(&m_wrk_formula);

		InitDebugArray();
		UpdateDebugAuto();
	}

	// Processing for any other formula
	else 
	{
		// time how long it takes
		QueryPerformanceCounter(&bcount);

		// Execute the currently selected formula
		error = SUCCESS;
		ret = gram.CalcF$symbol(&m_wrk_formula, (char *) m_current_edit.GetString(), &error);

		// time how long it takes
		QueryPerformanceCounter(&ecount);
		QueryPerformanceFrequency(&lFrequency);
		if (error == SUCCESS) 
		{
			// display result
			sprintf_s(format, 50, "%%.%df", m_precision);
			Cstr.Format(format, ret);
			m_CalcResult.SetWindowText(Cstr);

			// display total time for calculation
			title.Format("Formula Editor - %s - [%.8f]", m_current_edit.GetString(),
						 ((double) (ecount.LowPart - bcount.LowPart))/((double) lFrequency.LowPart));
			SetWindowText(title);
		}
		else 
		{
			switch (error) 
			{
			case ERR_INVALID_SYM:
				Cstr.Format("ERROR: Invalid symbol");
				m_CalcResult.SetWindowText(Cstr);
				break;
			case ERR_INVALID_FUNC_SYM:
				Cstr.Format("ERROR: Invalid f$ reference");
				m_CalcResult.SetWindowText(Cstr);
				break;
			case ERR_INVALID_DLL_SYM:
				Cstr.Format("ERROR: Invalid dll$ reference");
				m_CalcResult.SetWindowText(Cstr);
				break;
			case ERR_INVALID_EXPR:
				Cstr.Format("ERROR: Invalid expression");
				m_CalcResult.SetWindowText(Cstr);
				break;
			case ERR_DIV_ZERO:
				Cstr.Format("ERROR: Divide by zero");
				m_CalcResult.SetWindowText(Cstr);
				break;
			default:
				Cstr.Format("ERROR: Undefined");
				m_CalcResult.SetWindowText(Cstr);
				break;
			}
		}
	}
}

void CDlgFormulaScintilla::OnBnClickedAuto()
{
	if (m_ButtonAuto.GetCheck() == 1)
	{
		boost::spirit::tree_parse_info<const char *, int_factory_t>	tpi;
		CString  s;

		ok_to_update_debug = false;
		m_CalcResult.SetWindowText("");

		// cal hand lists
		m_wrk_formula.CreateHandListMatrices();

		// Validate parse trees
		if (!m_wrk_formula.ParseAllFormula(this->GetSafeHwnd(), false))
		{
			s.Format("There are syntax errors in one or more formulas that are\n");
			s.Append("preventing calculation of this formula.\n");
			s.Append("These errors need to be corrected before the 'Auto'\n");
			s.Append("button can be used.");
			MessageBox(s, "PARSE ERROR(s)", MB_OK);

			// All we need to do is remove the Auto Check since the button text hasn't been
			// updated yet and ok_to_update_debug has already been set to false
			m_ButtonAuto.SetCheck(0);

			return;
		}

		m_ButtonAuto.SetWindowText("* Auto *");

		LastChangeToFormula(&m_wrk_formula);

		InitDebugArray();

		ok_to_update_debug = true;
	}
	else 
	{
		ok_to_update_debug = false;
		m_ButtonAuto.SetWindowText("Auto");
	}
}

void CDlgFormulaScintilla::StopAutoButton()
{
	// If auto button is on, turn it off
	if (m_ButtonAuto.GetCheck() == 1) {
		m_ButtonAuto.SetCheck(0);
		OnBnClickedAuto();
	}
}

void CDlgFormulaScintilla::UpdateDebugAuto(void) 
{
	int				i = 0;
	CString			Cstr = "";

	bool			sym_ismyturn = (bool) p_symbols->sym()->ismyturn;

	// mark symbol result cache as stale
	m_wrk_formula.MarkCacheStale();

	// Loop through each line in the debug tab and evaluate it
	for (i=0; i<(int) debug_ar.GetSize(); i++) 
	{
		if (debug_ar[i].valid && debug_ar[i].error==SUCCESS) 
		{
			debug_ar[i].ret = gram.EvaluateTree(&m_wrk_formula, debug_ar[i].tree, NULL, &debug_ar[i].error);
		} 
		else
		{
			debug_ar[i].ret = 0;
		}
	}
	// Format the text
	CreateDebugTab(&Cstr);

	// Write the tab's contents to a log file, if selected
	if (m_fdebuglog && ((m_fdebuglog_myturn && sym_ismyturn) || !m_fdebuglog_myturn)) 
	{
		if (!m_wrote_fdebug_header) 
		{
			WriteFDebugLog(true);
			m_wrote_fdebug_header = true;

		}
		else 
		{
			WriteFDebugLog(false);
		}
	}

	// Display the text in the debug tab
	m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
	m_pActiveScinCtrl->SendMessage(SCI_SETTEXT,0,(LPARAM)Cstr.GetString());
	m_pActiveScinCtrl->SendMessage(SCI_EMPTYUNDOBUFFER);
	m_pActiveScinCtrl->GotoPosition(0);
	m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
}

void CDlgFormulaScintilla::CreateDebugTab(CString *cs) 
{
	int N = 0, i = 0, j = 0;
	CString newline = "";
	char format[50] = {0};

	sprintf_s(format, 50, "%%%d.%df = %%s", m_precision+m_equal, m_precision);

	*cs = "";

	N = (int) debug_ar.GetSize();
	for (i=0; i<N; i++) 
	{
		if (debug_ar[i].valid) 
		{
			if (debug_ar[i].error == SUCCESS) 
			{
				newline.Format(format, debug_ar[i].ret, debug_ar[i].exp.GetString());
				newline.Append("\r\n");
				cs->Append(newline);
			}
			else 
			{
				newline="";
				for (j=0; j<m_precision+m_equal-9; j++)
					newline.Append(" ");


				switch (debug_ar[i].error) 
				{
					case ERR_INVALID_SYM:
						newline.Append("ERROR: invalid symbol     = ");
						break;
					case ERR_INVALID_FUNC_SYM:
						newline.Append("ERROR: invalid function   = ");
						break;
					case ERR_INVALID_DLL_SYM:
						newline.Append("ERROR: invalid DLL-symbol = ");
						break;
					case ERR_INVALID_EXPR:
						newline.Append("ERROR: invalid expression = ");
						break;
					case ERR_DIV_ZERO:
						newline.Append("ERROR: division by zero   = ");
						break;
					case ERR_BAD_PARSE:
						newline.Append("ERROR: invalid syntax     = ");
						break;
					default:
						newline.Append("ERROR: (no detailed info) = ");
						break;				
				}

				//newline.Append("ERROR = ");
				newline.Append(debug_ar[i].exp);
				newline.Append("\r\n");
				cs->Append(newline);
			}
		}
		else 
		{
			cs->Append(debug_ar[i].exp);
			cs->Append("\r\n");
		}
	}

	// Remove last trailing \n so we don't extend field length
	if (cs->GetLength()>=1)
		*cs = cs->Mid(0, cs->GetLength()-1);
}

void CDlgFormulaScintilla::WriteFDebugLog(bool write_header) 
{
	int			N = 0, i = 0;
	CString		temp = "", line = "", header = "";
	char		format[50] = {0}, nowtime[26] = {0};

	sprintf_s(format, 50, "%%0.%df", m_precision);
	line.Format("%s,", get_time(nowtime));
	header.Format("date/time,");

	N = (int) debug_ar.GetSize();
	for (i=0; i<N; i++) 
	{
		if (debug_ar[i].valid) 
		{
			if (debug_ar[i].error == SUCCESS) 
			{
				temp.Format(format, debug_ar[i].ret);
			}
			else 
			{
				temp="";
			}
			line.Append(temp);
			header.Append(debug_ar[i].exp);
			if (i != N-1) 
			{
				line.Append(",");
				header.Append(",");
			}
		}
	}

	// write the line to the log
	CString fn;
	fn.Format("%s\\f$debug_%lu.log", _startup_path, p_sessioncounter->session_id());
	FILE *fp;
	if (fopen_s(&fp, fn.GetString(), "a")==0)
	{
		if (write_header) 
		{
			fprintf(fp, "%s\n", header.GetString());
		}
		fprintf(fp, "%s\n", line.GetString());
		fclose(fp);
	}
}

void CDlgFormulaScintilla::InitDebugArray(void) 
{
	char				buf[1000] = {0}, *eq = NULL;
	SDebugTabInfo		debug_struct;
	CString				Cstr = "", allText = "", strLine = "";
	int					stopchar = 0, pos = 0;
	CString				s = "";
	bool				parse_result = false;

	//
	// Loop through each line in the debug tab and parse it
	//
	debug_ar.RemoveAll();

	m_pActiveScinCtrl->GetText(allText);
	pos=0;
	while (pos<allText.GetLength()) 
	{
		// Get next line from entire buffer
		strLine = "";
		while (allText.Mid(pos, 2)!="\r\n" && allText.Mid(pos, 1)!="\r" && allText.Mid(pos, 1)!="\n" && pos<allText.GetLength()) 
		{
			strLine += allText.Mid(pos, 1);
			pos++;
		}
		if (allText.Mid(pos, 2)=="\r\n") 
		{
			pos += 2;
		}
		else if (allText.Mid(pos, 1)=="\r" || allText.Mid(pos, 1)=="\n") 
		{
			pos += 1;
		}

		// Find the equal sign, if it exists
		strcpy_s(buf, 1000, strLine.GetString());
		if ((eq = strstr(buf, "=")) && buf[0]!='/' && buf[1]!='/') 
		{
			eq++;
			while (*eq==' ' && *eq!='\0') 
				eq++;

			// parse
			debug_struct.exp.Format("%s", eq);

			parse_result = gram.ParseString(&debug_struct.exp, m_wrk_formula.formula(), &debug_struct.tree, &stopchar);

			debug_struct.ret = 0;
			debug_struct.valid = true;
			debug_struct.error = parse_result && gram.parse_symbol_stop_strs()->GetSize()==0 ? SUCCESS : ERR_BAD_PARSE;
		}
		else 
		{
			debug_struct.exp = buf;
			Cstr = "";

			gram.ParseString(&Cstr, m_wrk_formula.formula(), &debug_struct.tree, &stopchar);

			debug_struct.ret = 0;
			debug_struct.valid = false;
			debug_struct.error = ERR_DEBUG_NOEQ;
		}
		debug_ar.Add(debug_struct);
	}
}

void CDlgFormulaScintilla::OnBnClickedApplySave()
{
	OnBnClickedApply();
	CMainFrame			*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	pMyMainWnd->SendMessage(WM_COMMAND, ID_FILE_SAVE, 0);
}

void CDlgFormulaScintilla::WarnAboutAutoplayerWhenApplyingFormulaAndTurnAutoplayerOff()
{
	MessageBox("Editing the formula while the autoplayer is enabled\n"
		"is an extremely insane idea\n"
		"(like changing wheels while driving on the highway).\n\n"
		"We will have to turn the autoplayer off,\n"
		"but nevertheless you might lose your complete formula.\n"
		"Please make a backup and then press ok\n"
		"and never ever do such stupid things again.",
		"Warning", MB_OK | MB_TOPMOST);
	// There might be a race-condition somewhere, nobody really knows.
	// Trying to reduce the risk for evil outcomes...
	Sleep(1000);
	p_autoplayer->set_autoplayer_engaged(false);	
}

void CDlgFormulaScintilla::OnBnClickedApply() 
{
	CMenu				*file_menu = this->GetMenu()->GetSubMenu(0);
	COpenHoldemDoc		*pDoc = COpenHoldemDoc::GetDocument();

	// If autoplayer is engaged, dis-engage it
	if (p_autoplayer->autoplayer_engaged())
	{
		WarnAboutAutoplayerWhenApplyingFormulaAndTurnAutoplayerOff();
	}

	// Save settings to registry
	SaveSettingsToRegistry();

	// Re-calc working set hand lists
	m_wrk_formula.CreateHandListMatrices();

	// Parse working set
	LastChangeToFormula(&m_wrk_formula);

	if (!m_wrk_formula.ParseAllFormula(this->GetSafeHwnd(), false))
	{
		if (MessageBox("There are errors in the working formula set.\n\n"
					   "Would you still like to apply changes in the working set to the main set?\n\n"
					   "Note that if you choose yes here, then the main formula set will \n"
					   "contain errors, will likely not act as you expect, and may cause you\n"
					   "to lose money at the tables.\n\n"
					   "Please only click 'Yes' if you really know what you are doing.",
					   "PARSE ERROR", 
					   MB_YESNO) != IDYES)
		{
			return;
		}
	}

	// Copy working set to global set
	p_formula->CopyFormulaFrom(&m_wrk_formula);

	pDoc->SetModifiedFlag(true);

	// Re-calc global set hand lists
	p_formula->CreateHandListMatrices();

	// Re-parse global set
	p_formula->ParseAllFormula(this->GetSafeHwnd(), false);

	// Re-calc symbols
	p_symbols->CalcSymbols();

	// Rewrite f$debug log header, if required
	m_wrote_fdebug_header = false;

	m_dirty = false;

	HandleEnables(true);
}

void CDlgFormulaScintilla::OnBnClickedOk() 
{
	COpenHoldemDoc		*pDoc = COpenHoldemDoc::GetDocument();
	CMainFrame			*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	// If autoplayer is engaged, dis-engage it
	if (p_autoplayer->autoplayer_engaged())
	{
		WarnAboutAutoplayerWhenApplyingFormulaAndTurnAutoplayerOff();		
	}

	// Re-calc working set hand lists
	m_wrk_formula.CreateHandListMatrices();

	// Parse working set
	LastChangeToFormula(&m_wrk_formula);

	if (!m_wrk_formula.ParseAllFormula(this->GetSafeHwnd(), false))
	{
		if (MessageBox("There are errors in the working formula set.\n\n"
					   "Would you still like to apply changes in the working set to the main set "
					   "and exit the formula editor?\n\n"
					   "Note that if you choose yes here, then the main formula set will\n"
					   "contain errors, will likely not act as you expect, and may cause you\n"
					   "to lose money at the tables.\n\n"
					   "Please only click 'Yes' if you really know what you are doing.",
					   "PARSE ERROR", 
					   MB_YESNO) != IDYES)
		{
			return;
		}
	}

	// Copy working set to global set
	p_formula->CopyFormulaFrom(&m_wrk_formula);

	pDoc->SetModifiedFlag(true);
	m_dirty = false;

	// Re-calc global set hand lists
	p_formula->CreateHandListMatrices();

	// Re-parse global set
	p_formula->ParseAllFormula(this->GetSafeHwnd(), false);

	// Re-calc symbols
	p_symbols->CalcSymbols();

	DestroyWindow();
}

bool CDlgFormulaScintilla::PromptToSave()
{
	COpenHoldemDoc		*pDoc = COpenHoldemDoc::GetDocument();
	CMainFrame			*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	int response = MessageBox("You have made changes to this formula.\n\nDo you want to apply changes?", 
				   "Save changes?", 
				   MB_YESNOCANCEL);
	if (response == IDYES)
	{
		// Re-calc working set hand lists
		m_wrk_formula.CreateHandListMatrices();

		// Parse working set
		LastChangeToFormula(&m_wrk_formula);

		if (!m_wrk_formula.ParseAllFormula(this->GetSafeHwnd(), false))
		{
			if (MessageBox("There are errors in the working formula set.\n\n"
						   "Would you still like to apply changes in the working set to the main set "
						   "and exit the formula editor?\n\n"
						   "Note that if you choose yes here, then the main formula set will\n"
						   "contain errors, will likely not act as you expect, and may cause you\n"
						   "to lose money at the tables.\n\n"
						   "Please only click 'Yes' if you really know what you are doing.",
						   "PARSE ERROR", 
						   MB_YESNO) != IDYES)
			{
				return false;
			}
		}
	
		// Copy working set to global set
		p_formula->CopyFormulaFrom(&m_wrk_formula);

		pDoc->SetModifiedFlag(true);

		// Re-calc global set hand lists
		p_formula->CreateHandListMatrices();

		// Re-parse global set
		p_formula->ParseAllFormula(this->GetSafeHwnd(), false);

		m_dirty = false;

		return true;
	}
	
	if (response == IDNO)
	{
		m_dirty = false;
		return true;
	}

	return false;
}

void CDlgFormulaScintilla::OnBnClickedCancel()
{
	// Prompt for changes if needed
	if (m_dirty)
		if(!PromptToSave())
			return;

	DestroyWindow();
}

void CDlgFormulaScintilla::OnSearchUpdate()
{
	CString filter;
	m_SearchEdit.GetWindowText(filter);
	if (filter.IsEmpty()) {
		UpdateAllScintillaKeywords();
	} else {
		for (int iScint=0; iScint<m_ScinArray.GetSize(); iScint++)
		{
			UpdateScintillaKeywords(m_ScinArray[iScint]._pWnd);
			m_ScinArray[iScint]._pWnd->Refresh();
		}
	}
	LastChangeToFormula(&m_wrk_formula);
	PopulateFormulaTree();
}

void CDlgFormulaScintilla::OnEditCut() 
{
	m_pActiveScinCtrl->Cut();
}

void CDlgFormulaScintilla::OnEditCopy() 
{
	m_pActiveScinCtrl->Copy();
}

void CDlgFormulaScintilla::OnEditPaste() 
{
	m_pActiveScinCtrl->Paste();
}

void CDlgFormulaScintilla::OnEditDeleteText() 
{
	m_pActiveScinCtrl->SendMessage(SCI_DELETEBACK, 0, 0);
}

void CDlgFormulaScintilla::OnEditUndo() 
{
	m_pActiveScinCtrl->Undo();
}

void CDlgFormulaScintilla::OnEditRedo() 
{
	m_pActiveScinCtrl->Redo();
}
void CDlgFormulaScintilla::OnEditSelectAll() 
{
	m_pActiveScinCtrl->SelectAll();
}

void CDlgFormulaScintilla::OnTimer(UINT nIDEvent) 
{
	CMenu *edit_menu = this->GetMenu()->GetSubMenu(1);

	if (nIDEvent == MENU_UPDATE_TIMER) 
	{
		HandleEnables(false);
	}

	// Update debug tab (if auto button is pressed)
	else if (nIDEvent == DEBUG_UPDATE_TIMER)
	{
		if (m_ButtonAuto.GetCheck() == 1 && m_current_edit == "f$debug" && ok_to_update_debug)
			UpdateDebugAuto();
	}
}

void CDlgFormulaScintilla::SetStyleColors(CScintillaWnd *pWnd, bool enabled) 
{
	int i = 0;

	if (enabled) 
	{
		pWnd->SetForeground(0, RGB(0x00, 0x00, 0x00));		// SCE_C_DEFAULT 0
		pWnd->SetForeground(1, RGB(0x00, 0x99, 0x00));		// SCE_C_COMMENT 1	  (//)
		pWnd->SetForeground(2, RGB(0x00, 0x99, 0x00));		// SCE_C_COMMENTLINE 2  (/* */)
		//pWnd->SetForeground(3, RGB(0x00, 0x00, 0x00));	// SCE_C_COMMENTDOC 3
		//pWnd->SetForeground(4, RGB(0x00, 0x00, 0x00));	// SCE_C_NUMBER 4
		pWnd->SetForeground(5, RGB(0x00, 0x33, 0xcc));		// SCE_C_WORD 5			(keywords)
		//pWnd->SetForeground(6, RGB(0x00, 0x00, 0x00));	// SCE_C_STRING 6
		//pWnd->SetForeground(7, RGB(0x00, 0x00, 0x00));	// SCE_C_CHARACTER 7
		//pWnd->SetForeground(8, RGB(0x00, 0x00, 0x00));	// SCE_C_UUID 8
		//pWnd->SetForeground(9, RGB(0x00, 0x00, 0x00));	// SCE_C_PREPROCESSOR 9
		pWnd->SetForeground(10, RGB(0xff, 0x00, 0x00));	// SCE_C_OPERATOR 10	(operators)
		//pWnd->SetForeground(11, RGB(0x00, 0x00, 0x00));  // SCE_C_IDENTIFIER 11
		//pWnd->SetForeground(12, RGB(0x00, 0x00, 0x00));  // SCE_C_STRINGEOL 12
		//pWnd->SetForeground(13, RGB(0x00, 0x00, 0x00));  // SCE_C_VERBATIM 13
		//pWnd->SetForeground(14, RGB(0x00, 0x00, 0x00));  // SCE_C_REGEX 14
		//pWnd->SetForeground(15, RGB(0x00, 0x00, 0x00));  // SCE_C_COMMENTLINEDOC 15
		//pWnd->SetForeground(16, RGB(0x00, 0x00, 0x00));  // SCE_C_WORD2 16
		//pWnd->SetForeground(17, RGB(0x00, 0x00, 0x00));  // SCE_C_COMMENTDOCKEYWORD 17
		//pWnd->SetForeground(18, RGB(0x00, 0x00, 0x00));  // SCE_C_COMMENTDOCKEYWORDERROR 18
		pWnd->SetBackground(19, RGB(250, 240, 0));  // SCE_C_GLOBALCLASS 19
	}
	else 
	{
		for (i=0; i<=MAX_STYLE_NUM; i++) 
		{
			pWnd->SetForeground(i, RGB(0x00, 0x00, 0x00));
		}
	}
}

void CDlgFormulaScintilla::OnFormulaDebugLogfdebug() 
{
	m_fdebuglog = !m_fdebuglog;

	HandleEnables(true);
}

void CDlgFormulaScintilla::OnHelp()
{
	if (_access("OpenHoldem_Manual.chm", F_OK) != 0)
	{
		MessageBox("\"OpenHoldem_Manual.chm\" not found.\nPlease put it into your OpenHoldem folder.", "Error", 0);
	}
	else 
	{
		int RetValue = int(ShellExecute(NULL, "open", "OpenHoldem_Manual.chm", NULL, NULL, SW_SHOW));
		if (RetValue <= 32)
		{
			MessageBox("Error opening help-file", "Error", 0);
		}
	}
}

void CDlgFormulaScintilla::OnHelpWiki()
{
	ShellExecute(NULL, "open", "http://www.maxinmontreal.com/wiki/index.php5?title=Main_Page", "", "", SW_SHOWDEFAULT);
}

void CDlgFormulaScintilla::OnHelpForums()
{
	ShellExecute(NULL, "open", "http://www.maxinmontreal.com/forums/index.php", "", "", SW_SHOWDEFAULT);
}

void CDlgFormulaScintilla::OnFormulaDebugMyturn() 
{
	m_fdebuglog_myturn = !m_fdebuglog_myturn;

	HandleEnables(true);
}

BOOL CDlgFormulaScintilla::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	if (pMyMainWnd->wait_cursor())
	{
		RestoreWaitCursor();
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CDlgFormulaScintilla::OnFormulaViewGroupudf()
{
	m_udf_group = !m_udf_group;
	if (m_udf_group)
		GroupUDFs();
	else
		UngroupUDFs();
	if (m_udf_sort)
		SortUdfTree();

	HandleEnables(true);
}

void CDlgFormulaScintilla::OnFormulaViewSortudf()
{
	m_udf_sort = !m_udf_sort;
	if (m_udf_sort)
		SortUdfTree();

	HandleEnables(true);
}

void CDlgFormulaScintilla::SortUdfTree()
{
	if (hUDFItem != NULL) 
	{
		m_FormulaTree.SortChildren(hUDFItem);
		HTREEITEM hSearchItem = m_FormulaTree.GetChildItem(hUDFItem);
		while (hSearchItem) 
		{
			if (m_FormulaTree.ItemHasChildren(hSearchItem))
				m_FormulaTree.SortChildren(hSearchItem);
			hSearchItem = m_FormulaTree.GetNextSiblingItem(hSearchItem);
		}
	}
}

void CDlgFormulaScintilla::SaveSettingsToRegistry()
{
	WINDOWPLACEMENT wp;
	HTREEITEM		hItem = NULL;
	CString			text = "";
	UINT			state = 0;

	GetWindowPlacement(&wp);
	prefs.set_formula_x(wp.rcNormalPosition.left);
	prefs.set_formula_y(wp.rcNormalPosition.top);
	prefs.set_formula_dx(wp.rcNormalPosition.right - wp.rcNormalPosition.left);
	prefs.set_formula_dy(wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);
	prefs.set_precision(m_precision);
	prefs.set_equal(m_equal);
	prefs.set_fdebuglog(m_fdebuglog);
	prefs.set_fdebuglog_myturn(m_fdebuglog_myturn);
	prefs.set_udf_sort(m_udf_sort);
	prefs.set_udf_group(m_udf_group);

	// Tree expansion settings
	hItem = m_FormulaTree.GetChildItem(NULL);
	while (hItem != NULL)
	{
		text = m_FormulaTree.GetItemText(hItem);
		state = (m_FormulaTree.GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED) ? true : false;
		if (m_FormulaTree.ItemHasChildren(hItem)) {
			if (text == "Autoplayer Functions")
				prefs.set_expand_auto(state);
			else if (text == "Standard Functions")
				prefs.set_expand_std(state);
			else if (text == "Debug Functions")
				prefs.set_expand_debug(state);
			else if (text == "Hand Lists")
				prefs.set_expand_list(state);
			else if (text == "User Defined Functions")
				prefs.set_expand_udf(state);
		}

		hItem = m_FormulaTree.GetNextItem(hItem, TVGN_NEXT);
		if (hItem != NULL)
			text = m_FormulaTree.GetItemText(hItem);
	}

	if (hItem != NULL && text == "User Defined Functions")
		m_FormulaTree.SortChildren(hItem);

	m_standard_expand[1] = prefs.expand_auto();
	m_standard_expand[2] = prefs.expand_std();
	m_standard_expand[3] = prefs.expand_debug();
}

void CDlgFormulaScintilla::HandleEnables(bool AllItems)
{
	// These are what drive the enable/disable logic
	bool bFormulaVisible = true;
	bool bTreeHeadingSelected = false;
	bool bTreeValidLeafSelected = false;
	int  iWhichTypeSelected = -1; // 0=Standard, 1=Hand List, 2=User Defined Function, -1=In_valid
	bool bFindInfo = false;
	bool bDebugActive = false;
	bool bNotesOrDllActive = false;
	bool bEditorWindowActive = false;
	// m_dirty

	// Figure out the state of the dialog
	HTREEITEM curSelected = m_FormulaTree.GetSelectedItem();
	HTREEITEM parentItem = (curSelected == NULL ? NULL : m_FormulaTree.GetParentItem(curSelected));

	bFindInfo = !m_FindLastSearch.IsEmpty();
	bTreeHeadingSelected = (parentItem == NULL);
	bTreeValidLeafSelected = (!bTreeHeadingSelected && curSelected && m_FormulaTree.GetChildItem(curSelected) == NULL);
	bEditorWindowActive = (m_pActiveScinCtrl && GetFocus() == m_pActiveScinCtrl);
	bFormulaVisible = (m_TabControl.GetCurSel() == 0);

	CString headingText;
	if (bTreeHeadingSelected && curSelected)
		headingText = m_FormulaTree.GetItemText(curSelected);
	else if (parentItem)
	{
		headingText = m_FormulaTree.GetItemText(curSelected);
		bDebugActive = (headingText == "f$debug");
		bNotesOrDllActive = (headingText == "notes" || headingText == "dll");
		headingText = m_FormulaTree.GetItemText(parentItem);
	}
	if (headingText == "Autoplayer Functions")			iWhichTypeSelected = 0;
	else if (headingText == "Standard Functions")		iWhichTypeSelected = 0;
	else if (headingText == "Debug Functions")			iWhichTypeSelected = 0;
	else if (headingText == "Hand Lists")				iWhichTypeSelected = 1;
	else if (headingText == "User Defined Functions")	iWhichTypeSelected = 2;
	else {
		HTREEITEM hNextLevelUp = m_FormulaTree.GetParentItem(parentItem);
		if (hNextLevelUp != NULL) {
			headingText = m_FormulaTree.GetItemText(hNextLevelUp);
			if (headingText == "User Defined Functions")
				iWhichTypeSelected = 2;
		}
	}

	// Just used to clean up the code a bit.  Basically, MenuEnableDisable[false] has the flags for disabling, and MenuEnableDisable[true] for enabling
	const int MenuEnableDisable[2] = {MF_BYPOSITION | MF_DISABLED | MF_GRAYED, MF_BYPOSITION | MF_ENABLED };

	// Edit Menu that relate the state of the Scintilla window
	CMenu *edit_menu = this->GetMenu()->GetSubMenu(1);
	edit_menu->EnableMenuItem(EDIT_UNDO,		MenuEnableDisable[bTreeValidLeafSelected && m_current_edit.GetLength()!=0 && m_pActiveScinCtrl && m_pActiveScinCtrl->CanUndo()]);
	edit_menu->EnableMenuItem(EDIT_REDO,		MenuEnableDisable[bTreeValidLeafSelected && m_current_edit.GetLength()!=0 && m_pActiveScinCtrl && m_pActiveScinCtrl->CanRedo()]);
	edit_menu->EnableMenuItem(EDIT_CUT,			MenuEnableDisable[bTreeValidLeafSelected && m_current_edit.GetLength()!=0 && m_pActiveScinCtrl && m_pActiveScinCtrl->GetSelectionStart()-m_pActiveScinCtrl->GetSelectionEnd()!=0]);
	edit_menu->EnableMenuItem(EDIT_COPY,		MenuEnableDisable[bTreeValidLeafSelected && m_current_edit.GetLength()!=0 && m_pActiveScinCtrl && m_pActiveScinCtrl->GetSelectionStart()-m_pActiveScinCtrl->GetSelectionEnd()!=0]);
	edit_menu->EnableMenuItem(EDIT_PASTE,		MenuEnableDisable[bTreeValidLeafSelected && m_current_edit.GetLength()!=0 && m_pActiveScinCtrl && m_pActiveScinCtrl->CanPaste()]);
	edit_menu->EnableMenuItem(EDIT_DELETE_TEXT,	MenuEnableDisable[bTreeValidLeafSelected && m_current_edit.GetLength()!=0 && m_pActiveScinCtrl && m_pActiveScinCtrl->GetSelectionStart()-m_pActiveScinCtrl->GetSelectionEnd()!=0]);
	edit_menu->EnableMenuItem(EDIT_SELECTALL,	MenuEnableDisable[bTreeValidLeafSelected && m_current_edit.GetLength()!=0 && m_pActiveScinCtrl]);
	if (!AllItems)
		return;

	m_FormulaTree.EnableWindow(true); // Spew: Just in Case?

	// Enable the Buttons
	m_ButtonCalc.EnableWindow(bTreeValidLeafSelected && !bNotesOrDllActive && iWhichTypeSelected != 1);
	m_ButtonAuto.EnableWindow(bDebugActive);

	// File Menu
	CMenu *file_menu = this->GetMenu()->GetSubMenu(0);
	file_menu->EnableMenuItem(0, MenuEnableDisable[m_dirty]);
	file_menu->EnableMenuItem(1, MenuEnableDisable[m_dirty]);
	file_menu->EnableMenuItem(2, MenuEnableDisable[true]);

	// File Menu Buttons
	m_FormulaApply.EnableWindow(m_dirty);
	m_FormulaOK.EnableWindow(m_dirty);
	m_FormulaCancel.EnableWindow(true);

	bool ModifiableSelected = (bTreeValidLeafSelected && iWhichTypeSelected>0);

	// Edit Menu Remaining Items
	edit_menu->EnableMenuItem(EDIT_NEW,			MenuEnableDisable[bFormulaVisible&&iWhichTypeSelected>0]);
	edit_menu->EnableMenuItem(EDIT_RENAME,		MenuEnableDisable[bFormulaVisible&&ModifiableSelected]);
	edit_menu->EnableMenuItem(EDIT_DELETE,		MenuEnableDisable[bFormulaVisible&&ModifiableSelected]);

	edit_menu->EnableMenuItem(EDIT_FIND,		MenuEnableDisable[bTreeValidLeafSelected]);
	edit_menu->EnableMenuItem(EDIT_FIND_NEXT,	MenuEnableDisable[bTreeValidLeafSelected && bFindInfo]);
	edit_menu->EnableMenuItem(EDIT_FIND_PREV,	MenuEnableDisable[bTreeValidLeafSelected && bFindInfo]);

	edit_menu->EnableMenuItem(EDIT_UNDO,		MenuEnableDisable[bEditorWindowActive]);
	edit_menu->EnableMenuItem(EDIT_REDO,		MenuEnableDisable[bEditorWindowActive]);
	edit_menu->EnableMenuItem(EDIT_CUT,			MenuEnableDisable[bEditorWindowActive]);
	edit_menu->EnableMenuItem(EDIT_COPY,		MenuEnableDisable[bEditorWindowActive]);
	edit_menu->EnableMenuItem(EDIT_PASTE,		MenuEnableDisable[bEditorWindowActive]);
	edit_menu->EnableMenuItem(EDIT_DELETE_TEXT,	MenuEnableDisable[bEditorWindowActive]);
	edit_menu->EnableMenuItem(EDIT_SELECTALL,	MenuEnableDisable[bEditorWindowActive]);

	edit_menu->EnableMenuItem(EDIT_FONT,		MenuEnableDisable[true]);
	edit_menu->EnableMenuItem(EDIT_SETTINGS,	MenuEnableDisable[true]);
	edit_menu->EnableMenuItem(EDIT_HANDLIST,	MenuEnableDisable[bTreeValidLeafSelected && iWhichTypeSelected==1]);

	// Similar to MenuEnableDisable, this is used to make the code more readable (index by true/false)
	const int MenuCheckUncheck[2] = {MF_BYPOSITION | MF_UNCHECKED, MF_BYPOSITION | MF_CHECKED };

	// View Menu
	CMenu *view_menu = this->GetMenu()->GetSubMenu(2);
	view_menu->CheckMenuItem(VIEW_TOOLBAR,			MenuCheckUncheck[true]);
	view_menu->CheckMenuItem(VIEW_LINENUMBERS,		MenuCheckUncheck[m_are_linenumbers_visible]);
	view_menu->CheckMenuItem(VIEW_SELECTIONMARGIN,	MenuCheckUncheck[m_is_selection_margin_visible]);
	view_menu->CheckMenuItem(VIEW_FOLDINGMARGIN,	MenuCheckUncheck[m_is_folding_margin_visible]);
	view_menu->CheckMenuItem(VIEW_SYNTAXCOLORING,	MenuCheckUncheck[m_is_syntax_colored]);
	view_menu->CheckMenuItem(VIEW_SORTUDF,			MenuCheckUncheck[m_udf_sort]);
	view_menu->CheckMenuItem(VIEW_GROUPUDF,			MenuCheckUncheck[m_udf_group]);

	// Debug Menu
	CMenu *debug_menu = this->GetMenu()->GetSubMenu(3);
	debug_menu->CheckMenuItem(DEBUG_FDEBUG_LOGGING,			MenuCheckUncheck[m_fdebuglog]);
	debug_menu->CheckMenuItem(DEBUG_FDEBUG_MYTURN,			MenuCheckUncheck[m_fdebuglog_myturn]);

	// Debug Toolbar Items
	m_toolBar.GetToolBarCtrl().EnableButton(ID_FORMULA_TOOLBAR_LESS_PRECISION, bDebugActive);
	m_toolBar.GetToolBarCtrl().EnableButton(ID_FORMULA_TOOLBAR_MORE_PRECISION, bDebugActive);
	m_toolBar.GetToolBarCtrl().EnableButton(ID_FORMULA_TOOLBAR_EQUAL_LEFT, bDebugActive);
	m_toolBar.GetToolBarCtrl().EnableButton(ID_FORMULA_TOOLBAR_EQUAL_RIGHT, bDebugActive);

	// Set toolbar properties
	m_toolBar.GetToolBarCtrl().EnableButton(ID_FORMULA_TOOLBAR_NEW,	bFormulaVisible&&iWhichTypeSelected>0);
	m_toolBar.GetToolBarCtrl().EnableButton(ID_FORMULA_TOOLBAR_RENAME,	bFormulaVisible&&ModifiableSelected);
	m_toolBar.GetToolBarCtrl().EnableButton(ID_FORMULA_TOOLBAR_DELETE,	bFormulaVisible&&ModifiableSelected);
	m_toolBar.GetToolBarCtrl().EnableButton(ID_FORMULA_TOOLBAR_FONT,	true);
	m_toolBar.GetToolBarCtrl().EnableButton(ID_FORMULA_TOOLBAR_SETTINGS, true);
	m_toolBar.GetToolBarCtrl().EnableButton(ID_FORMULA_TOOLBAR_HANDLIST, bTreeValidLeafSelected && iWhichTypeSelected==1);
}


HTREEITEM CDlgFormulaScintilla::AddSymbolSubTitle(HTREEITEM parentItem, const char *title, const char *description)
{
	HTREEITEM ret = m_SymbolTree.InsertItem(title, parentItem);

	m_SymbolTree.SetItemData(ret, (DWORD_PTR)description);

	return ret;
}

HTREEITEM CDlgFormulaScintilla::AddSymbolTitle(const char *title, const char *description, HTREEITEM parentItem)
{
	HTREEITEM ret = m_SymbolTree.InsertItem(title, parentItem);

	m_SymbolTree.SetItemData(ret, (DWORD_PTR)description);
	m_SymbolTree.SetItemState(ret, TVIS_BOLD, TVIS_BOLD);

	return ret;
}

HTREEITEM CDlgFormulaScintilla::AddSymbol(HTREEITEM parentItem, const char *symbol, const char *description)
{
	HTREEITEM ret  = m_SymbolTree.InsertItem(symbol, hRawItem);
	m_SymbolTree.SetItemData(ret, (DWORD_PTR)description);

	ret = m_SymbolTree.InsertItem(symbol, parentItem);
	m_SymbolTree.SetItemData(ret, (DWORD_PTR)description);

	return ret;
}

void CDlgFormulaScintilla::PopulateSymbols()
{
	if (m_SymbolTree.GetRootItem() != NULL)
		return;

	hRawItem = AddSymbolTitle("All Symbols", NULL, NULL);
	hCatItem = AddSymbolTitle("Categories", NULL, NULL);

	HTREEITEM mainParent, parent;
	mainParent = parent = AddSymbolTitle("General symbols", NULL, hCatItem);
	AddSymbol(parent, "ismanual", "true if you're in manual mode, false otherwise");
	AddSymbol(parent, "isppro", "true if you're connected to a ppro server");
	AddSymbol(parent, "site", "0=user/ppro 1=scraped");
	AddSymbol(parent, "nchairs", "the integer value for the Table Map symbol _s$nchairs");
	AddSymbol(parent, "isbring", "true if winholdem is attached to a bring client window");
	AddSymbol(parent, "session", "the current logging instance (0-9)");
	AddSymbol(parent, "handnumber", "the site hand number if available");
	AddSymbol(parent, "version", "returns the version number of OpenHoldem that is currently running");

	mainParent = parent = AddSymbolTitle("Table Map symbols", NULL, hCatItem);
	AddSymbol(parent, "_sitename$abc", "true if user defined string abc appears within the Table Map symbol _s$_sitename");
	AddSymbol(parent, "_network$def", "true if user defined string def appears within the Table Map symbol _s$_network");
	AddSymbol(parent, "swagdelay", "autoplayer delay in milliseconds between swag keystrokes and button click");
	AddSymbol(parent, "allidelay", "autoplayer delay in milliseconds between alli slider jam and button click");
	AddSymbol(parent, "_swagtextmethod", "the site interpretation for swag edit text (Table Map symbol) 1=f$srai 2=f$srai+call 3=f$srai+call+currentbet");
	AddSymbol(parent, "_potmethod", "the site interpretation for the contents of c0pot0 (Table Map symbol) 1=common (default) 2=total");
	AddSymbol(parent, "_activemethod", " 1=inactive unless pXactive returns true/active (default) 2=active unless pXactive returns false/inactive/out/away");

	mainParent = parent = AddSymbolTitle("Formula file", NULL, hCatItem);
	AddSymbol(parent, "rake", "percentage amount added/subtracted to/from the pot");
	AddSymbol(parent, "nit", "number of iterations tested by the analyzer(s)");
	AddSymbol(parent, "bankroll", "the user defined real world bankroll");

	mainParent = parent = AddSymbolTitle("Limits", NULL, hCatItem);
	AddSymbol(parent, "bblind", "the big blind amount");
	AddSymbol(parent, "sblind", "the small blind amount");
	AddSymbol(parent, "ante", "the current pre-deal ante requirement");
	AddSymbol(parent, "lim", "the current table limit 0=NL 1=PL 2=FL");
	AddSymbol(parent, "isnl", "(lim==0)");
	AddSymbol(parent, "ispl", "(lim==1)");
	AddSymbol(parent, "isfl", "(lim==2)");
	AddSymbol(parent, "sraiprev", "the difference between the two largest unique wagers");
	AddSymbol(parent, "sraimin", "Scraped - (currentbet+call); PokerPro - the greater of bet and the current raise");
	AddSymbol(parent, "sraimax", "balance-call");
	AddSymbol(parent, "istournament", "true if a tournament table is detected");

	mainParent = parent = AddSymbolTitle("Hand Rank", NULL, hCatItem);
	AddSymbol(parent, "handrank", "one of the following based on your selected option");
	AddSymbol(parent, "handrank169", "your pocket holdem hand rank 1-169 (see table)");
	AddSymbol(parent, "handrank2652", "your pocket holdem hand rank 12-2652 (see table)");
	AddSymbol(parent, "handrank1326", "your pocket holdem hand rank 6-1326 (handrank2652/2)");
	AddSymbol(parent, "handrank1000", "your pocket holdem hand rank 4-1000 (1000*handrank2652/2652)");
	AddSymbol(parent, "handrankp", "2652 / (1+nopponents)");

	mainParent = parent = AddSymbolTitle("Chairs", NULL, hCatItem);
	AddSymbol(parent, "chair", "your chair number 0-9 ... 0 is usually top right");
	AddSymbol(parent, "userchair", "user chair number (0-9)");
	AddSymbol(parent, "dealerchair", "dealer chair number (0-9)");
	AddSymbol(parent, "raischair", "raising chair number (0-9)");
	AddSymbol(parent, "oppchair", "raising chair number (0-9)");
	AddSymbol(parent, "chair$abc", "player abc chair number (0-9); -1 if not found");
	AddSymbol(parent, "chairbit$abc", "player abc chairbit (1 << chai_r$abc); 0 if not found");

	mainParent = parent = AddSymbolTitle("Rounds / Positions", NULL, hCatItem);
	AddSymbol(parent, "betround", "betting round (1-4) 1=preflop, 2=flop, 3=turn, 4=river");
	AddSymbol(parent, "br", "abbreviation for betround");
	AddSymbol(parent, "betposition", "your bet position (1=sblind,2=bblind,...,nplayersdealt=dealer).  Betposition will change as players fold in front of you.");
	AddSymbol(parent, "dealposition", "your deal position (1=sblind,2=bblind ... nplayersdealt=dealer).  Dealposition will not change as players fold.");
	AddSymbol(parent, "callposition", "your numbered offset from the raising player (who is 0)");
	AddSymbol(parent, "seatposition", "your seat position relative to the dealer");
	AddSymbol(parent, "dealpositionrais", "the deal position of the raising player (1-10)");
	AddSymbol(parent, "betpositionrais", "the bet position of the raising player (1-10)");

	mainParent = parent = AddSymbolTitle("Probabilities", NULL, hCatItem);
	AddSymbol(parent, "prwin", "the probability of winning this hand (0.000 - 1.000)");
	AddSymbol(parent, "prlos", "the probability of losing this hand (0.000 - 1.000)");
	AddSymbol(parent, "prtie", "the probability of pushing this hand (0.000 - 1.000)");
	AddSymbol(parent, "prwinnow", "probability that all opponents have a lower hand right now");
	AddSymbol(parent, "prlosnow", "probability that any opponents have a higher hand right now");
	AddSymbol(parent, "random", "random number between (0.000-1.000)");
	AddSymbol(parent, "randomhand", "random number between (0.000-1.000) for the hand");
	AddSymbol(parent, "randomround", "random number between (0.000-1.000) for the current round");
	AddSymbol(parent, "randomround1 - randomround4", "random number between (0.000-1.000) for round 1 - 4");

	mainParent = parent = AddSymbolTitle("Formulas", NULL, hCatItem);
	AddSymbol(parent, "f$name", "reference the specified formula");

	mainParent = parent = AddSymbolTitle("P Formula", NULL, hCatItem);
	AddSymbol(parent, "defcon", "defense level used in P formula.  Determines the number of analyzer opponents (0.000=maxoffense 1.000=maxdefense) (the defense level dialog uses values 0-10)");
	AddSymbol(parent, "isdefmode", "true when defcon is at max");
	AddSymbol(parent, "isaggmode", "true when defcon is at min");

	mainParent = parent = AddSymbolTitle("Chip Amounts", NULL, hCatItem);
	AddSymbol(parent, "balance", "your balance");
	AddSymbol(parent, "balance0 - balance9", "specific player/chair balance");
	AddSymbol(parent, "stack0 - stack9", "sorted playersplaying balances from 0=biggest to 9=smallest");
	AddSymbol(parent, "currentbet", "your current amount of chips in play");
	AddSymbol(parent, "currentbet0 - currentbet9", "specific player/chair currentbet");
	AddSymbol(parent, "call", "the amount you need to call");
	AddSymbol(parent, "bet", "the amount of a single initial bet or raise for current round");
	AddSymbol(parent, "bet1 - bet 4", "the amount of a single initial bet or raise for round 1 - round 4");
	AddSymbol(parent, "pot", "the total amount of chips in play including player bets");
	AddSymbol(parent, "potcommon", "the total amount of chips in the middle");
	AddSymbol(parent, "potplayer", "the total amount of chips in front of all players");
	AddSymbol(parent, "callshort", "total amount that will be added to the pot if all players call");
	AddSymbol(parent, "raisshort", "callshort + bet * nplayersplaying");

	mainParent = parent = AddSymbolTitle("Number of Bets", NULL, hCatItem);
	AddSymbol(parent, "nbetstocall", "total number of additional bets required to call.");
	AddSymbol(parent, "nbetstorais", "total number of additional bets required to raise.");
	AddSymbol(parent, "ncurrentbets", "total number of bets currently in front of you.");
	AddSymbol(parent, "ncallbets", "total number of bets you would have on the table if you call");
	AddSymbol(parent, "nraisbets", "total number of bets you would have on the table if you raise");

	mainParent = parent = AddSymbolTitle("List Tests", NULL, hCatItem);
	AddSymbol(parent, "islist0 - islist99 ", "true if your hand is in the numbered (0-99) list");
	AddSymbol(parent, "islistcall", "true if your hand is in list 0");
	AddSymbol(parent, "islistrais", "true if your hand is in list 1");
	AddSymbol(parent, "islistalli", "true if your hand is in list 7");
	AddSymbol(parent, "isemptylistcall", "true if the call list is empty");
	AddSymbol(parent, "isemptylistrais", "true if the rais list is empty");
	AddSymbol(parent, "isemptylistalli", "true if the alli list is empty");
	AddSymbol(parent, "nlistmax", "highest list number in which your hand is listed");
	AddSymbol(parent, "nlistmin", "lowest list number in which your hand is listed");

	mainParent = parent = AddSymbolTitle("Poker Values", NULL, hCatItem);
	AddSymbol(parent, "pokerval", "absolute poker value for your 5 card hand");
	AddSymbol(parent, "pokervalplayer", "absolute poker value for your 2 card pocket hand only.");
	AddSymbol(parent, "pokervalcommon", "absolute poker value for the common cards");
	AddSymbol(parent, "pcbits", "bit list of where your pocket cards are used in your 5 card hand");
	AddSymbol(parent, "npcbits", "number (0-2) of your pocket cards used in your 5 card hand");

	mainParent = parent = AddSymbolTitle("Poker Value Constants", NULL, hCatItem);
	AddSymbol(parent, "hicard", "1<< 0 (2 ** 0)");
	AddSymbol(parent, "onepair", "1<<24 (2 ** 24)");
	AddSymbol(parent, "twopair", "1<<25 (2 ** 25)");
	AddSymbol(parent, "threeofakind", "1<<26 (2 ** 26)");
	AddSymbol(parent, "straight", "1<<27 (2 ** 27)");
	AddSymbol(parent, "flush", "1<<28 (2 ** 28)");
	AddSymbol(parent, "fullhouse", "1<<29 (2 ** 29)");
	AddSymbol(parent, "fourofakind", "1<<30 (2 ** 30)");
	AddSymbol(parent, "straightflush", "1<<31 (2 ** 31)");
	AddSymbol(parent, "royalflush", "0x800edcba");
	AddSymbol(parent, "fiveofakind", "0xff000000");

	mainParent = parent = AddSymbolTitle("Hand Tests", NULL, hCatItem);
	AddSymbol(parent, "$CCc", "see the hand symbol reference for details");
	AddSymbol(parent, "ishandup", "true if your hand has gone up a level (i.e. from 1 pair to 2 pair)");
	AddSymbol(parent, "ishandupcommon", "true if common hand has gone up a level (i.e. from 1 pair to 2 pair)");
	AddSymbol(parent, "ishicard", "true when you have hicard hand");
	AddSymbol(parent, "isonepair", "true when you have one pair");
	AddSymbol(parent, "istwopair", "true when you have two pair");
	AddSymbol(parent, "isthreeofakind", "true when you have three of a kind");
	AddSymbol(parent, "isstraight", "true when you have a straight");
	AddSymbol(parent, "isflush", "true when you have a flush");
	AddSymbol(parent, "isfullhouse", "true when you have a full house");
	AddSymbol(parent, "isfourofakind", "true when you have four of a kind");
	AddSymbol(parent, "isstraightflush", "true when you have a straight flush");
	AddSymbol(parent, "isroyalflush", "true when you have a royal flush");
	AddSymbol(parent, "isfiveofakind", "true when you have a five of a kind");

	mainParent = parent = AddSymbolTitle("Pocket Tests", NULL, hCatItem);
	AddSymbol(parent, "ispair", "true when your two dealt pocket cards are rank equal (0-1)");
	AddSymbol(parent, "issuited", "true when your two dealt pocket cards are suit equal (0-1)");
	AddSymbol(parent, "isconnector", "true when your two dealt pocket cards are rank adjacent (0-1)");

	mainParent = parent = AddSymbolTitle("Pocket/Common Tests", NULL, hCatItem);
	AddSymbol(parent, "ishipair", "true when you have hi pair (0-1)");
	AddSymbol(parent, "islopair", "true when you have lo pair (0-1)");
	AddSymbol(parent, "ismidpair", "true when you have mid pair (0-1)");
	AddSymbol(parent, "ishistraight", "true when you have the highest straight possible");
	AddSymbol(parent, "ishiflush", "true when you have the highest flush possible");

	mainParent = parent = AddSymbolTitle("Players, Friends, Opponents", "Note that the [friends] symbols are not interpreted the same way as they are in WinHoldem.  OpenHoldem does not include any automated collusion capabilities, and thus [friends] has been redefined in OpenHoldem to mean you only.  Thus if you are seated, nfriendsseated will resolve to 1. If you are not seated, nfriendsseated will resolve to zero.", hCatItem);
	AddSymbol(parent, "nopponents", "P formula value for the userchair iterator");
	AddSymbol(parent, "nopponentsmax", "maximum allowable value for nopponents (1-22 default=9)");
	AddSymbol(parent, "nplayersseated", "number of players seated (including you) (0-10)");
	AddSymbol(parent, "nplayersactive", "number of players active (including you) (0-10)");
	AddSymbol(parent, "nplayersdealt", "number of players dealt (including you) (0-10)");
	AddSymbol(parent, "nplayersplaying", "number of players playing (including you) (0-10)");
	AddSymbol(parent, "nplayersblind", "number of players blind (including you) (0-10)");
	AddSymbol(parent, "nfriendsseated", "1 of you are seated, 0 otherwise (0-1)");
	AddSymbol(parent, "nfriendssactive", "1 of you are active, 0 otherwise (0-1)");
	AddSymbol(parent, "nfriendsdealt", "1 of you are dealt, 0 otherwise (0-1)");
	AddSymbol(parent, "nfriendsplaying", "1 of you are playing, 0 otherwise (0-1)");
	AddSymbol(parent, "nfriendsblind", "1 of you are in a blind, 0 otherwise (0-1)");
	AddSymbol(parent, "nopponentsseated", "number of opponents seated (not including you) (0-9)");
	AddSymbol(parent, "nopponentssactive", "number of opponents active (not including you) (0-9)");
	AddSymbol(parent, "nopponentsdealt", "number of opponents dealt (not including you) (0-9)");
	AddSymbol(parent, "nopponentsplaying", "number of opponents playing (not including you) (0-9)");
	AddSymbol(parent, "nopponentsblind", "number of opponents blind (not including you) (0-9)");
	AddSymbol(parent, "nopponentschecking", "number of opponents playing with a zero current bet equal to the previous bettor (0-9)");
	AddSymbol(parent, "nopponentscalling", "number of opponents playing with a non-zero current bet equal to the previous bettor (0-9)");
	AddSymbol(parent, "nopponentsraising", "number of opponents playing with a current bet greater than the previous bettor (0-9)");
	AddSymbol(parent, "nopponentsbetting", "number of opponents playing with a non zero current bet (0-9)");
	AddSymbol(parent, "nopponentsfolded", "number of opponents that have folded this hand (0-9)");
	AddSymbol(parent, "nplayerscallshort", "number of players that must call to stay in the hand");
	AddSymbol(parent, "nchairsdealtright", "number of chairs dealt before your chair");
	AddSymbol(parent, "nchairsdealtleft", "number of chairs dealt after your chair");
	AddSymbol(parent, "playersseatedbits", "bits 9-0: 1=seated 0=unseated");
	AddSymbol(parent, "playersactivebits", "bits 9-0: 1=active 0=inactive");
	AddSymbol(parent, "playersdealtbits", "bits 9-0: 1=dealt 0=notdealt");
	AddSymbol(parent, "playersplayingbits", "bits 9-0: 1=playing 0=notplaying");
	AddSymbol(parent, "playersblindbits", "bits 9-0: 1=blind 0=notblind");
	AddSymbol(parent, "opponentsseatedbits", "bits 9-0: 1=seated 0=unseated");
	AddSymbol(parent, "opponentsactivebits", "bits 9-0: 1=active 0=inactive");
	AddSymbol(parent, "opponentsdealtbits", "bits 9-0: 1=dealt 0=notdealt");
	AddSymbol(parent, "opponentsplayingbits", "bits 9-0: 1=playing 0=notplaying");
	AddSymbol(parent, "opponentsblindbits", "bits 9-0: 1=blind 0=notblind");
	AddSymbol(parent, "friendsseatedbits", "bits 9-0: 1=seated 0=unseated, you only");
	AddSymbol(parent, "friendsactivebits", "bits 9-0: 1=active 0=inactive, you only");
	AddSymbol(parent, "friendsdealtbits", "bits 9-0: 1=dealt 0=notdealt, you only");
	AddSymbol(parent, "friendsplayingbits", "bits 9-0: 1=playing 0=notplaying, you only");
	AddSymbol(parent, "friendsblindbits", "bits 9-0: 1=blind 0=notblind, you only");

	mainParent = parent = AddSymbolTitle("Flags", NULL, hCatItem);
	AddSymbol(parent, "fmax", "highest numbered flag button pressed");
	AddSymbol(parent, "f0 - f19", "true if flag 0 - flag 19 button is pressed false otherwise");
	AddSymbol(parent, "fbits", "flag button bits 19-0 - 1=pressed 0=notpressed");

	mainParent = parent = AddSymbolTitle("Common Cards", NULL, hCatItem);
	AddSymbol(parent, "ncommoncardspresent", "number of common cards present (normal or highlighted)");
	AddSymbol(parent, "ncommoncardsknown", "number of common cards known (normal not highlighted)");
	AddSymbol(parent, "nflopc", "short for ncommoncardsknown");

	mainParent = parent = AddSymbolTitle("(Un)known Cards", NULL, hCatItem);
	AddSymbol(parent, "nouts", "the total number of unseen single cards that if dealt to the board might put your hand in the lead. to be counted as an out, the card must be able to bump your level and your new level must be higher than the resulting common level");
	AddSymbol(parent, "ncardsknown", "total number of cards you can see (yours and commons)");
	AddSymbol(parent, "ncardsunknown", "total number of cards you cannot see (deck and opponents)");
	AddSymbol(parent, "ncardsbetter", "total number of single unknown cards that can beat you, e.g. if the board is four suited in hearts, and you have two spades, then ncardsbetter will be at least 9, because of the possible flush.");

	mainParent = parent = AddSymbolTitle("nhands", NULL, hCatItem);
	AddSymbol(parent, "nhands", "total possible number of two-card hands using the unseen cards (nhandshi+nhandslo+nhandsti)");
	AddSymbol(parent, "nhandshi", "number of hands that can beat you in a showdown right now");
	AddSymbol(parent, "nhandslo", "number of hands that you can beat in a showdown right now");
	AddSymbol(parent, "nhandsti", "number of hands that can tie you in a showdown right now");

	mainParent = parent = AddSymbolTitle("Flushes / Straights / Sets", NULL, hCatItem);
	AddSymbol(parent, "nsuited", "total number of same suited cards you have (1-7)");
	AddSymbol(parent, "nsuitedcommon", "total number of same suited cards in the middle (1-5)");
	AddSymbol(parent, "tsuit", "specific card suit for nsuited (1-4)");
	AddSymbol(parent, "tsuitcommon", "specific card suit for nsuitedcommon (1-4)");
	AddSymbol(parent, "nranked", "total number of same ranked cards you have (1-4)");
	AddSymbol(parent, "nrankedcommon", "total number of same ranked cards in the middle (1-4)");
	AddSymbol(parent, "trank", "specific card rank for nranked (2-14)");
	AddSymbol(parent, "trankcommon", "specific card rank for nrankedcommon (2-14)");
	AddSymbol(parent, "nstraight", "total number of connected cards you have (1-7)");
	AddSymbol(parent, "nstraightcommon", "total number of connected common cards (1-5)");
	AddSymbol(parent, "nstraightfill", "total number of cards needed to fill a straight (0-5)");
	AddSymbol(parent, "nstraightfillcommon", "total number of cards needed to fill a common straight (0-5)");
	AddSymbol(parent, "nstraightflush", "total number of suited connected cards you have (1-7)");
	AddSymbol(parent, "nstraightflushcommon", "total number of suited connected common cards (1-5)");
	AddSymbol(parent, "nstraightflushfill", "total number of cards needed to fill a straightflush (0-5)");
	AddSymbol(parent, "nstraightflushfillcommon", "total number of cards needed to fill a common straightflush (0-5)");

	mainParent = parent = AddSymbolTitle("Rank Bits (aces are hi and lo)", NULL, hCatItem);
	AddSymbol(parent, "rankbits", "bit list of card ranks (yours and commons)");
	AddSymbol(parent, "rankbitscommon", "bit list of card ranks (commons)");
	AddSymbol(parent, "rankbitsplayer", "bit list of card ranks (yours)");
	AddSymbol(parent, "rankbitspoker", "bit list of card ranks (pokerval)");
	AddSymbol(parent, "srankbits", "bit list of suited card ranks (yours and commons tsuit)");
	AddSymbol(parent, "srankbitscommon", "bit list of suited card ranks (commons tsuitcommon)");
	AddSymbol(parent, "srankbitsplayer", "bit list of suited card ranks (yours tsuit)");
	AddSymbol(parent, "srankbitspoker", "bit list of suited card ranks (pokerval tsuit)");

	mainParent = parent = AddSymbolTitle("Rank Hi (aces are hi)", NULL, hCatItem);
	AddSymbol(parent, "rankhi", "highest card rank (14-2) (yours and commons)");
	AddSymbol(parent, "rankhicommon", "highest card rank (14-2) (commons)");
	AddSymbol(parent, "rankhiplayer", "highest card rank (14-2) (yours)");
	AddSymbol(parent, "rankhipoker", "highest card rank (14-2) (pokerval)");
	AddSymbol(parent, "srankhi", "highest suited card rank (14-2) (yours and commons tsuit)");
	AddSymbol(parent, "srankhicommon", "highest suited card rank (14-2) (commons tsuitcommon)");
	AddSymbol(parent, "srankhiplayer", "highest suited card rank (14-2) (yours tsuit)");
	AddSymbol(parent, "srankhipoker", "highest suited card rank (14-2) (pokerval tsuit)");

	mainParent = parent = AddSymbolTitle("Rank Lo (aces are hi)", NULL, hCatItem);
	AddSymbol(parent, "ranklo", "lowest card rank (14-2) (yours and commons)");
	AddSymbol(parent, "ranklocommon", "lowest card rank (14-2) (commons)");
	AddSymbol(parent, "rankloplayer", "lowest card rank (14-2) (yours)");
	AddSymbol(parent, "ranklopoker", "lowest card rank (14-2) (pokerval)");
	AddSymbol(parent, "sranklo", "lowest suited card rank (14-2) (yours and commons tsuit)");
	AddSymbol(parent, "sranklocommon", "lowest suited card rank (14-2) (commons tsuitcommon)");
	AddSymbol(parent, "srankloplayer", "lowest suited card rank (14-2) (yours tsuit)");
	AddSymbol(parent, "sranklopoker", "lowest suited card rank (14-2) (pokerval tsuit)");

	mainParent = parent = AddSymbolTitle("Time", NULL, hCatItem);
	AddSymbol(parent, "elapsed", "time in seconds since sitting down");
	AddSymbol(parent, "elapsedhand", "time in seconds since end of previous hand");
	AddSymbol(parent, "elapsedauto", "time in seconds since autoplayer took action");
	AddSymbol(parent, "elapsedtoday", "time in seconds since midnight GMT");
	AddSymbol(parent, "elapsed1970", "time in seconds since 1970-01-01 00:00:00 GMT (THURSDAY)");
	AddSymbol(parent, "clocks", "number of cpu clocks since the last scrape");
	AddSymbol(parent, "nclockspersecond", "number of cpu clocks per second");
	AddSymbol(parent, "ncps", "number of cpu clocks per second");

	mainParent = parent = AddSymbolTitle("Autoplayer", NULL, hCatItem);
	AddSymbol(parent, "myturnbits", "bits 3210 correspond to buttons ARCF (alli rais call fold)");
	AddSymbol(parent, "ismyturn", "(myturnbits & 7) (rais or call/chec or fold)");
	AddSymbol(parent, "issittingin", "true when you are not being dealt out");
	AddSymbol(parent, "issittingout", "true when you are being dealt out");
	AddSymbol(parent, "isautopost", "true when you are autoposting");
	AddSymbol(parent, "isfinalanswer", "true when autoplayer preparing to act; false any other time.");

	mainParent = parent = AddSymbolTitle("History", NULL, hCatItem);
	AddSymbol(parent, "nplayersround1 - nplayersround4", "number of players that began betting round 1 - round 4");
	AddSymbol(parent, "nplayersround", "number of players that began the current betting round");
	AddSymbol(parent, "prevaction", "record of previously attempted autoplayer action. (-1=fold 0=chec 1=call 2=rais 3=swag 4=alli)");
	AddSymbol(parent, "didchec", "true if the autoplayer has checked during current round");
	AddSymbol(parent, "didcall", "true if the autoplayer has called during current round");
	AddSymbol(parent, "didrais", "true if the autoplayer has raised during current round");
	AddSymbol(parent, "didswag", "true if the autoplayer has swag'd during current round");
	AddSymbol(parent, "nbetsround1 - nbetsround4", "the largest number of bets in front of any player during round 1- round 4");
	AddSymbol(parent, "nbetsround", "the largest number of bets in front of any player right now");
	AddSymbol(parent, "didchecround1 - didchecround4", "true if userchair checked during round 1 - round 4");
	AddSymbol(parent, "didcallround1 - didcallround4", "true if userchair called during round 1 - round 4");
	AddSymbol(parent, "didraisround1 - didraisround4", "true if userchair raised during round 1 - round 4");
	AddSymbol(parent, "didswaground1 - didswaground4", "true if userchair swag'd during round 1 - round 4");

	mainParent = parent = AddSymbolTitle("RON / RUN", "These symbols report the total number of possible river endings for the opponent (ron$) and the user (run$). A value of zero means that type of poker hand is not possible. Any non-zero value means that type of poker hand will be seen that many times.", hCatItem);
	AddSymbol(parent, "ron$royfl", "river opponent number : possible royal flush");
	AddSymbol(parent, "ron$strfl", "river opponent number : possible straight flush");
	AddSymbol(parent, "ron$4kind", "river opponent number : possible four of a kind");
	AddSymbol(parent, "ron$fullh", "river opponent number : possible full house");
	AddSymbol(parent, "ron$flush", "river opponent number : possible flush");
	AddSymbol(parent, "ron$strai", "river opponent number : possible straight");
	AddSymbol(parent, "ron$3kind", "river opponent number : possible three of a kind");
	AddSymbol(parent, "ron$2pair", "river opponent number : possible two pair");
	AddSymbol(parent, "ron$1pair", "river opponent number : possible one pair");
	AddSymbol(parent, "ron$hcard", "river opponent number : possible high card");
	AddSymbol(parent, "ron$total", "river opponent number : sum of all possible river endings");
	AddSymbol(parent, "ron$pokervalmax", "the maximum possible pokerval for the opponent");
	AddSymbol(parent, "ron$prnuts", "opponent chances of hitting the nuts on or before the river");
	AddSymbol(parent, "ron$prbest", "opponent chances of hitting pokervalmax on or before the river");
	AddSymbol(parent, "ron$clocks", "total number of cpu clocks used to calculate the ron$ symbols");
	AddSymbol(parent, "run$royfl", "river user number : possible royal flush");
	AddSymbol(parent, "run$strfl", "river user number : possible straight flush");
	AddSymbol(parent, "run$4kind", "river user number : possible four of a kind");
	AddSymbol(parent, "run$fullh", "river user number : possible full house");
	AddSymbol(parent, "run$flush", "river user number : possible flush");
	AddSymbol(parent, "run$strai", "river user number : possible straight");
	AddSymbol(parent, "run$3kind", "river user number : possible three of a kind");
	AddSymbol(parent, "run$2pair", "river user number : possible two pair");
	AddSymbol(parent, "run$1pair", "river user number : possible one pair");
	AddSymbol(parent, "run$hcard", "river user number : possible high card");
	AddSymbol(parent, "run$total", "river user number : sum of all possible river endings");
	AddSymbol(parent, "run$pokervalmax", "the maximum possible pokerval for the user");
	AddSymbol(parent, "run$prnuts", "user chances of hitting the nuts on or before the river");
	AddSymbol(parent, "run$prbest", "user chances of hitting pokervalmax on or before the river");
	AddSymbol(parent, "run$clocks", "total number of cpu clocks used to calculate the run$ symbols");

	mainParent = parent = AddSymbolTitle("Versus symbols", NULL, hCatItem);
	AddSymbol(parent, "vs$nhands", "Total possible number of opponent hands");
	AddSymbol(parent, "vs$nhandshi", "Number of opponent hands that have higher river chances ");
	AddSymbol(parent, "vs$nhandsti", "Number of opponent hands that have equal river chances");
	AddSymbol(parent, "vs$nhandslo", "Number of opponent hands that have lower river chances");
	AddSymbol(parent, "vs$prwin", "Probability (0.000 - 1.000) of winning versus all possible opponent hands ");
	AddSymbol(parent, "vs$prtie", "Probability (0.000 - 1.000) of chopping versus all possible opponent hands ");
	AddSymbol(parent, "vs$prlos", "Probability (0.000 - 1.000) of losing versus all possible opponent hands ");
	AddSymbol(parent, "vs$prwinhi", "Probability (0.000 - 1.000) of winning versus higher opponent hands ");
	AddSymbol(parent, "vs$prtiehi", "Probability (0.000 - 1.000) of chopping versus higher opponent hands ");
	AddSymbol(parent, "vs$prloshi", "Probability (0.000 - 1.000) of losing versus higher opponent hands ");
	AddSymbol(parent, "vs$prwinti", "Probability (0.000 - 1.000) of winning versus equal opponent hands ");
	AddSymbol(parent, "vs$prtieti", "Probability (0.000 - 1.000) of chopping versus equal opponent hands ");
	AddSymbol(parent, "vs$prlosti", "Probability (0.000 - 1.000) of losing versus equal opponent hands ");
	AddSymbol(parent, "vs$prwinlo", "Probability (0.000 - 1.000) of winning versus lower opponent hands ");
	AddSymbol(parent, "vs$prtielo", "Probability (0.000 - 1.000) of chopping versus lower opponent hands ");
	AddSymbol(parent, "vs$prloslo", "Probability (0.000 - 1.000) of losing versus lower opponent hands ");
	AddSymbol(parent, "vs$x$prwin", "Probability (0.000 - 1.000) of winnning versus hand list x");
	AddSymbol(parent, "vs$x$prlos", "Probability (0.000 - 1.000) of losing versus hand list x");
	AddSymbol(parent, "vs$x$prtie", "Probability (0.000 - 1.000) of a tie versus hand list x");

	mainParent = parent = AddSymbolTitle("History symbols", NULL, hCatItem);
	AddSymbol(parent, "hi_<sym>x (x=1-4)", "the value of the symbol <sym> as of your last turn in betting round x.  Example: hi_prwin1 would return prwin as of your last turn in br1.");
	/*
	Valid values for <sym> are:
	*PROBABILITIES: prwin, prlos, prtie
	*CHIP AMOUNTS: balance, balance0, balance1, balance2, balance3, balance4, balance5, balance6, balance7, balance8, balance9, stack0, stack1, stack2, stack3, stack4, stack5, stack6, stack7, stack8, stack9
	*POKER VALUES: pokerval, pokervalplayer, pokervalcommon, pcbits, npcbits
	*HAND TESTS: ishandup, ishandupcommon, ishicard, isonepair, istwopair, isthreeofakind, isstraight, isflush, isfullhouse, isfourofakind, isstraightflush, isroyalflush, isfiveofakind
	*POCKET/COMMON TESTS: ishipair, islopair, ismidpair, ishistraight, ishiflush
	*(UN)KNOWN CARDS: nouts, ncardsbetter
	*NHANDS: nhands, nhandshi, nhandslo, nhandsti, prwinnow, prlosnow
	*FLUSHES SETS STRAIGHTS: nsuited, nsuitedcommon, tsuit, tsuitcommon, nranked, nrankedcommon, trank, trankcommon, nstraight, nstraightcommon, nstraightfill, nstraightfillcommon, nstraightflush, nstraightflushcommon, nstraightflushfill, nstraightflushfillcommon
	*RANK BITS: rankbits, rankbitscommon, rankbitsplayer, rankbitspoker, srankbits, 	srankbitscommon, srankbitsplayer, srankbitspoker
	*RANK HI: rankhi, rankhicommon, rankhiplayer, rankhipoker, srankhi, srankhicommon, srankhiplayer, srankhipoker
	*RANK LO: ranklo, ranklocommon, rankloplayer, ranklopoker, sranklo, sranklocommon, srankloplayer, sranklopoker
	*run$ ron$: ron$royfl, ron$strfl, ron$4kind, ron$fullh, ron$strai, ron$3kind, ron$2pair, ron$1pair, ron$hcard, ron$total, ron$pokervalmax, ron$prnuts, ron$prbest, run$royfl, run$strfl, run$4kind, run$fullh, run$strai, run$3kind, run$2pair, run$1pair, run$hcard, run$total, run$pokervalmax, run$prnuts, run$prbest
	*/

	mainParent = parent = AddSymbolTitle("Action symbols", NULL, hCatItem);
	AddSymbol(parent, "lastraisedx (x=1-4)", "which chair was the last to raise in round x");
	AddSymbol(parent, "raisbitsx (x=1-4)", "which chairs raised in round x ");
	AddSymbol(parent, "callbitsx (x=1-4)", "which chairs called in round x");
	AddSymbol(parent, "foldbitsx (x=1-4)", "which chairs folded in round x");
	AddSymbol(parent, "oppdealt", "Trailing indicator for nopponentsdealt");

	mainParent = parent = AddSymbolTitle("MyHand symbols", NULL, hCatItem);
	AddSymbol(parent, "mh_3straightxy", "(x=1 for wheel, 0 not, y=1 for broadway, 0 not) - returns true if the board has a wheel straight draw or broadway straight draw, given the wheel/broadway parameters");
	AddSymbol(parent, "mh_bottomsd", "true if I have a bottom straight draw");
	AddSymbol(parent, "mh_nsuitedbetter", "number of missing suited cards that are higher than my best suited card");
	AddSymbol(parent, "mh_kickerbetter", "number of cards that can beat your kicker");
	AddSymbol(parent, "mh_kickerrank", "rank of your kicker (returns 0 if kicker is shared [board] and thus useless)");
	AddSymbol(parent, "mh_nouts", "number of outs (HTC's formula)");
	AddSymbol(parent, "mh_str_strflush", "0-5 (5 best) of the relative strength of your straight flush");
	AddSymbol(parent, "mh_str_quads", "0-5 (5 best) of the relative strength of your four of a kind");
	AddSymbol(parent, "mh_str_fullhouse", "0-5 (5 best) of the relative strength of your full house");
	AddSymbol(parent, "mh_str_flush", "0-5 (5 best) of the relative strength of your flush");
	AddSymbol(parent, "mh_str_straight", "0-5 (5 best) of the relative strength of your straight");
	AddSymbol(parent, "mh_str_trips", "0-5 (5 best) of the relative strength of your three of a kind");
	AddSymbol(parent, "mh_str_twopair", "0-5 (5 best) of the relative strength of your two pair"); 
	AddSymbol(parent, "mh_str_onepair", "0-5 (5 best) of the relative strength of your one pair");

	mainParent = parent = AddSymbolTitle("Table stats symbols", "Note: the setting for [y minutes] can be found in Edit/Preferences, and defaults to 15 minutes.", hCatItem);
	AddSymbol(parent, "floppct", "percentage of players seeing the flop for the last y minutes");
	AddSymbol(parent, "turnpct", "percentage of players seeing the turn for the last y minutes");
	AddSymbol(parent, "riverpct", "percentage of players seeing the river for the last y minutes");
	AddSymbol(parent, "avgbetspf", "average number of bets preflop for the last y minutes");
	AddSymbol(parent, "tablepfr", "pfr percentage preflop for the last y minutes");
	AddSymbol(parent, "maxbalance", "my highest balance during the session");
	AddSymbol(parent, "handsplayed", "number of hands played this session");
	AddSymbol(parent, "balance_rankx (x=0-9)", "ranked list of player balances (includes players not currently in hand, and includes currentbet for each player as well).  rank0 has highest balance.");

	mainParent = parent = AddSymbolTitle("Poker Tracker symbols", NULL, hCatItem);
	parent = AddSymbolSubTitle(mainParent, "Ring symbols");
	AddSymbol(parent, "pt_iconx (x=0-9)", "Poker Tracker auto-rate icon code for chair x");
	AddSymbol(parent, "pt_iconlastrx (x=1-4)", "Poker Tracker auto-rate icon code for the last raiser in round x");
	AddSymbol(parent, "pt_pfrx (x=0-9)", "Poker Tracker pre-flop raise percentage for chair x");
	AddSymbol(parent, "pt_aggtotx (x=0-9)", "Poker Tracker total aggression for chair x");
	AddSymbol(parent, "pt_aggtotnopfx (x=0-9)", "Poker Tracker total aggression excluding preflop for chair x");
	AddSymbol(parent, "pt_aggpx (x=0-9)", "Poker Tracker preflop aggression for chair x");
	AddSymbol(parent, "pt_aggfx (x=0-9)", "Poker Tracker flop aggression for chair x");
	AddSymbol(parent, "pt_aggtx (x=0-9)", "Poker Tracker turn aggression for chair x");
	AddSymbol(parent, "pt_aggrx (x=0-9)", "Poker Tracker river aggression for chair x");
	AddSymbol(parent, "pt_floppctx (x=0-9)", "Poker Tracker saw flop pct for chair x");
	AddSymbol(parent, "pt_turnpctx (x=0-9)", "Poker Tracker saw turn pct for chair x");
	AddSymbol(parent, "pt_riverpctx (x=0-9)", "Poker Tracker saw river pct for chair x");
	AddSymbol(parent, "pt_vpipx (x=0-9)", "Poker Tracker VP$IP for chair x");
	AddSymbol(parent, "pt_handsx (x=0-9)", "Poker Tracker number of hands that are in the database for chair x");
	AddSymbol(parent, "pt_pf_rfix (x=0-9)", "Poker Tracker [pre-flop raise first in] pct for chair x");
	AddSymbol(parent, "pt_pf_crx (x=0-9)", "Poker Tracker [pre-flop called raise] pct for chair x");
	AddSymbol(parent, "pt_pfatsx (x=0-9)", "Poker Tracker attempt to steal blinds for chair x");
	AddSymbol(parent, "pt_wsdpx (x=0-9)", "Poker Tracker went to showdown percentage for chair x");
	AddSymbol(parent, "pt_wssdx (x=0-9)", "Poker Tracker won $ at showdown for chair x");
	AddSymbol(parent, "pt_fsbtsx (x=0-9)", "Poker Tracker folded small blind to steal for chair x");
	AddSymbol(parent, "pt_fbbtsx (x=0-9)", "Poker Tracker folded big blind to steal for chair x ");

	parent = AddSymbolSubTitle(mainParent, "Ring symbols for the [raischair]");
	AddSymbol(parent, "pt_ricon", "Poker Tracker auto-rate icon code for raise chair");
	AddSymbol(parent, "pt_rpfr", "Poker Tracker pre-flop raise percentage for raise chair");
	AddSymbol(parent, "pt_raggtot", "Poker Tracker total aggression for raise chair");
	AddSymbol(parent, "pt_raggtotnopf", "Poker Tracker total aggression excluding preflop for raise chair");
	AddSymbol(parent, "pt_raggp", "Poker Tracker preflop aggression for raise chair");
	AddSymbol(parent, "pt_raggf", "Poker Tracker flop aggression for raise chair");
	AddSymbol(parent, "pt_raggt", "Poker Tracker turn aggression for raise chair");
	AddSymbol(parent, "pt_raggrv", "Poker Tracker river aggression for raise chair");
	AddSymbol(parent, "pt_rfloppct", "Poker Tracker saw flop pct for raise chair");
	AddSymbol(parent, "pt_rturnpct", "Poker Tracker saw turn pct for raise chair");
	AddSymbol(parent, "pt_rriverpct", "Poker Tracker saw river pct for raise chair");
	AddSymbol(parent, "pt_rvpip", "Poker Tracker VP$IP for raise chair");
	AddSymbol(parent, "pt_rhands", "Poker Tracker number of hands that are in the database for raise chair");
	AddSymbol(parent, "pt_rpf_rfi", "Poker Tracker [pre-flop raise first in] pct for raise chair");
	AddSymbol(parent, "pt_rpf_cr", "Poker Tracker [pre-flop called raise] pct for raise chair");
	AddSymbol(parent, "pt_rpfats", "Poker Tracker attempt to steal blinds for raise chair");
	AddSymbol(parent, "pt_rwsdp", "Poker Tracker went to showdown percentage for raise chair");
	AddSymbol(parent, "pt_rwssd", "Poker Tracker won $ at showdown for raise chair");
	AddSymbol(parent, "pt_rfsbts", "Poker Tracker folded small blind to steal for raise chair");
	AddSymbol(parent, "pt_rfbbts", "Poker Tracker folded big blind to steal for raise chair");

	parent = AddSymbolSubTitle(mainParent, "Tournament symbols");
	AddSymbol(parent, "ptt_iconx (x=0-9)", "Poker Tracker auto-rate icon code for chair x");
	AddSymbol(parent, "ptt_iconlastrx (x=1-4)", "Poker Tracker auto-rate icon code for the last raiser in round x");
	AddSymbol(parent, "ptt_pfrx (x=0-9)", "Poker Tracker pre-flop raise percentage for chair x");
	AddSymbol(parent, "ptt_aggtotx (x=0-9)", "Poker Tracker total aggression for chair x");
	AddSymbol(parent, "ptt_aggtotnopfx (x=0-9)", "Poker Tracker total aggression excluding preflop for chair x");
	AddSymbol(parent, "ptt_aggpx (x=0-9)", "Poker Tracker preflop aggression for chair x");
	AddSymbol(parent, "ptt_aggfx (x=0-9)", "Poker Tracker flop aggression for chair x");
	AddSymbol(parent, "ptt_aggtx (x=0-9)", "Poker Tracker turn aggression for chair x");
	AddSymbol(parent, "ptt_aggrx (x=0-9)", "Poker Tracker river aggression for chair x");
	AddSymbol(parent, "ptt_floppctx (x=0-9)", "Poker Tracker saw flop pct for chair x");
	AddSymbol(parent, "ptt_turnpctx (x=0-9)", "Poker Tracker saw turn pct for chair x");
	AddSymbol(parent, "ptt_riverpctx (x=0-9)", "Poker Tracker saw river pct for chair x");
	AddSymbol(parent, "ptt_vpipx (x=0-9)", "Poker Tracker VP$IP for chair x");
	AddSymbol(parent, "ptt_handsx (x=0-9)", "Poker Tracker number of hands that are in the database for chair x");
	AddSymbol(parent, "ptt_pf_rfix (x=0-9)", "Poker Tracker [pre-flop raise first in] pct for chair x");
	AddSymbol(parent, "ptt_pf_crx (x=0-9)", "Poker Tracker [pre-flop called raise] pct for chair x");
	AddSymbol(parent, "ptt_pfatsx (x=0-9)", "Poker Tracker attempt to steal blinds for chair x");
	AddSymbol(parent, "ptt_wsdpx (x=0-9)", "Poker Tracker went to showdown percentage for chair x");
	AddSymbol(parent, "ptt_wssdx (x=0-9)", "Poker Tracker won $ at showdown for chair x");
	AddSymbol(parent, "ptt_fsbtsx (x=0-9)", "Poker Tracker folded small blind to steal for chair x");
	AddSymbol(parent, "ptt_fbbtsx (x=0-9)", "Poker Tracker folded big blind to steal for chair x ");

	parent = AddSymbolSubTitle(mainParent, "Tournament symbols for the [raischair]");
	AddSymbol(parent, "ptt_ricon", "Poker Tracker auto-rate icon code for raise chair");
	AddSymbol(parent, "ptt_rpfr", "Poker Tracker pre-flop raise percentage for raise chair");
	AddSymbol(parent, "ptt_raggtot", "Poker Tracker total aggression for raise chair");
	AddSymbol(parent, "ptt_raggtotnopf", "Poker Tracker total aggression excluding preflop for raise chair");
	AddSymbol(parent, "ptt_raggp", "Poker Tracker preflop aggression for raise chair");
	AddSymbol(parent, "ptt_raggf", "Poker Tracker flop aggression for raise chair");
	AddSymbol(parent, "ptt_raggt", "Poker Tracker turn aggression for raise chair");
	AddSymbol(parent, "ptt_raggr", "Poker Tracker river aggression for raise chair");
	AddSymbol(parent, "ptt_rfloppct", "Poker Tracker saw flop pct for raise chair");
	AddSymbol(parent, "ptt_rturnpct", "Poker Tracker saw turn pct for raise chair");
	AddSymbol(parent, "ptt_rriverpct", "Poker Tracker saw river pct for raise chair");
	AddSymbol(parent, "ptt_rvpip", "Poker Tracker VP$IP for raise chair");
	AddSymbol(parent, "ptt_rhands", "Poker Tracker number of hands that are in the database for raise chair");
	AddSymbol(parent, "ptt_rpf_rfi", "Poker Tracker [pre-flop raise first in] pct for raise chair");
	AddSymbol(parent, "ptt_rpf_cr", "Poker Tracker [pre-flop called raise] pct for raise chair");
	AddSymbol(parent, "ptt_rpfats", "Poker Tracker attempt to steal blinds for raise chair");
	AddSymbol(parent, "ptt_rwsdp", "Poker Tracker went to showdown percentage for raise chair");
	AddSymbol(parent, "ptt_rwssd", "Poker Tracker won $ at showdown for raise chair");
	AddSymbol(parent, "ptt_rfsbts", "Poker Tracker folded small blind to steal for raise chair");
	AddSymbol(parent, "ptt_rfbbts", "Poker Tracker folded big blind to steal for raise chair");

	mainParent = parent = AddSymbolTitle("ICM calculator symbols", NULL, hCatItem);
	AddSymbol(parent, "icm", "my tournament equity before any action is considered (just balances)");
	AddSymbol(parent, "icm_fold", "my tournament equity if I fold");
	AddSymbol(parent, "icm_callwin", "my tournament equity if I call and win");
	AddSymbol(parent, "icm_calllose", "my tournament equity if I call and lose");
	AddSymbol(parent, "icm_alliwin0 - icm_alliwin9", "my tournament equity if I push all-in and win against 0 - 9 callers");
	AddSymbol(parent, "icm_allilose1 - icm_allilose9", "my tournament equity if I push all-in and lose against 0 - 9 callers ");

	mainParent = parent = AddSymbolTitle("Hand multiplexor symbols", NULL, hCatItem);
	AddSymbol(parent, "f$$X", "Evaluate the UDF that corresponds to my first card");
	AddSymbol(parent, "f$$XX", "Evaluate the UDF that corresponds to my first card and second card");
	AddSymbol(parent, "f$$XXx", "Evaluate the UDF that corresponds to my first card, second card and suited/unsuited state");

	mainParent = parent = AddSymbolTitle("Memory symbols", NULL, hCatItem);
	AddSymbol(parent, "me_st_", "Stores a value. Example: me_st_abc_123_45 - stores the value '123.45' in variable 'abc'.");
	AddSymbol(parent, "me_re_", "Retrieves a previously stored value. Example: me_re_abc - retrieves the value from variable 'abc'.");

	mainParent = parent = AddSymbolTitle("Hand Symbols", NULL, hCatItem);
	AddSymbol(parent, "$RR", "You can reference your dealt hand directly by using the $ symbols. The general form of the $ hand symbols are: $RRs. The $ character is required followed by 1 or 2 standard card rank characters. The hand symbols are not case sensitive.");

	mainParent = parent = AddSymbolTitle("Card Symbols", NULL, hCatItem);
	AddSymbol(parent, "$$ccX", "the card value for common card X (X=0-4)");
	AddSymbol(parent, "$$crX", "the rank value for common card X (X=0-4)");
	AddSymbol(parent, "$$csX", "the suit value for common card X (X=0-4)");
	AddSymbol(parent, "$$pcX", "the card value for player card X (X=0-1)");
	AddSymbol(parent, "$$prX", "the rank value for player card X (X=0-1)");
	AddSymbol(parent, "$$psX", "the suit value for player card X (X=0-1)");

	m_SymbolTree.SortChildren(hRawItem);
}

bool CDlgFormulaScintilla::GetProfileFont(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font, CDC* pDC)
{
	CWinApp *pApp = AfxGetApp();
	ASSERT_VALID(pApp);
	CString s = pApp->GetProfileString(lpszKey, lpszVal);
	if (s.IsEmpty())
		return FALSE;

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfCharSet = DEFAULT_CHARSET;
	int bItalic;
	int iPtSize;

	if (sscanf_s(s.GetString(), "%[a-zA-Z ],%d,%d,%d",
			   lf.lfFaceName, 32, &iPtSize, &lf.lfWeight, &bItalic) != 4)
		return FALSE;

	lf.lfHeight = MulDiv(-iPtSize, 	// convert ptsize to logical units
						 ::GetDeviceCaps(pDC ? pDC->m_hDC : ::GetDC(NULL), LOGPIXELSY), 72);
	lf.lfItalic = bItalic;	// because lf.lfItalic is a BYTE
	font.DeleteObject();		// bye

	return font.CreateFontIndirect(&lf)!=0;
}


bool CDlgFormulaScintilla::WriteProfileFont(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font, CDC* pDC)
{
	CWinApp *pApp = AfxGetApp();
	ASSERT_VALID(pApp);
	LOGFONT lf;

	font.GetLogFont(&lf);

	int iPtSize = MulDiv(-lf.lfHeight, 72,
						 ::GetDeviceCaps(pDC ? pDC->m_hDC : ::GetDC(NULL), LOGPIXELSY));
	CString s;

	s.Format("%s,%d,%d,%d", lf.lfFaceName, iPtSize, lf.lfWeight, lf.lfItalic);

	return pApp->WriteProfileString(lpszKey, lpszVal, s)!=0;
}

