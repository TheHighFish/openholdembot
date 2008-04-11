// DialogFormulaScintilla.cpp : implementation file
//

// menu options, menu edit commands

#include "stdafx.h"
#include "OpenHoldem.h"
#include "OpenHoldemDoc.h"
#include "MainFrm.h"
#include "DialogFormulaScintilla.h"
#include "global.h"
#include "registry.h"
#include "DialogRename.h"
#include "DialogNew.h"
#include "DialogSettings.h"
#include "DialogHandList.h"
#include "grammar.h"
#include "winmgr/WinMgr.h"
#include "threads.h"
#include "symbols.h"
#include "debug.h"

#include "../scintilla/include/Scintilla.h"
#include "../scintilla/include/SciLexer.h"

// CDlgFormulaScintilla dialog
CDlgFormulaScintilla	*m_formulaScintillaDlg = NULL;

char *	keywords = "ismanual isppro site nchairs isbring session handnumber "
		"sitename$ network$ swagdelay allidelay swagtextmethod potmethod activemethod rake nit bankroll bblind sblind "
		"ante lim isnl ispl isfl sraiprev sraimin sraimax istournament handrank "
		"handrank169 handrank2652 handrank1326 handrank1000 handrankp chair userchair "
		"dealerchair raischair chair$ chairbit$ betround br betposition dealposition "
		"callposition seatposition dealpositionrais betpositionrais prwin prtie prlos "
		"random randomhand randomround randomround1 randomround2 randomround3 randomround4 "
		"callror raisror srairor alliror callmean raismean sraimean allimean callvariance "
		"raisvariance sraivariance allivariance defcon isdefmode isaggmode balance balance0 "
		"balance1 balance2 balance3 balance4 balance5 balance6 balance7 balance8 balance9 "
		"stack0 stack1 stack2 stack3 stack4 stack5 stack6 stack7 stack8 stack9 currentbet "
		"currentbet0 currentbet1 currentbet2 currentbet3 currentbet4 currentbet5 currentbet6 "
		"currentbet7 currentbet8 currentbet9 call bet bet1 bet2 bet3 bet4 pot potcommon "
		"potplayer callshort raisshort nbetstocall nbetstorais ncurrentbets ncallbets nraisbets "
		"islist0 islist1 islist2 islist3 islist4 islist5 islist6 islist7 islist8 islist9 "
		"islist10 islist11 islist12 islist13 islist14 islist15 islist16 islist17 islist18 islist19 "
		"islist20 islist21 islist22 islist23 islist24 islist25 islist26 islist27 islist28 islist29 "
		"islist30 islist31 islist32 islist33 islist34 islist35 islist36 islist37 islist38 islist39 "
		"islist40 islist41 islist42 islist43 islist44 islist45 islist46 islist47 islist48 islist49 "
		"islist50 islist51 islist52 islist53 islist54 islist55 islist56 islist57 islist58 islist59 "
		"islist60 islist61 islist62 islist63 islist64 islist65 islist66 islist67 islist68 islist69 "
		"islist70 islist71 islist72 islist73 islist74 islist75 islist76 islist77 islist78 islist79 "
		"islist80 islist81 islist82 islist83 islist84 islist85 islist86 islist87 islist88 islist89 "
		"islist90 islist91 islist92 islist93 islist94 islist95 islist96 islist97 islist98 islist99 "
		"islistcall islistrais islistalli isemptylistcall isemptylistrais isemptylistalli "
		"nlistmax nlistmin pokerval pokervalplayer pokervalcommon pcbits npcbits hicard onepair "
		"twopair threeofakind straight flush fullhouse fourofakind straightflush royalflush "
		"fiveofakind ishandup ishandupcommon ishicard isonepair istwopair isthreeofakind isstraight "
		"isflush isfullhouse isfourofakind isstraightflush isroyalflush isfiveofakind ispair "
		"issuited isconnector ishipair islopair ismidpair ishistraight ishiflush nopponents "
		"nopponentsmax nplayersseated nplayersactive nplayersdealt nplayersplaying nplayersblind "
		"nopponentsseated nopponentsactive nopponentsdealt nopponentsplaying nopponentsblind "
		"nfriendsseated nfriendsactive nfriendsdealt nfriendsplaying nfriendsblind "
		"nopponentschecking nopponentscalling nopponentsraising nopponentsbetting nopponentsfolded "
		"nplayerscallshort nchairsdealtright nchairsdealtleft playersseatedbits playersactivebits "
		"playersdealtbits playersplayingbits playersblindbits opponentsseatedbits opponentsactivebits "
		"opponentsdealtbits opponentsplayingbits opponentsblindbits friendsseatedbits friendsactivebits "
		"friendsdealtbits friendsplayingbits friendsblindbits fmax f0 f1 f2 f3 f4 f5 "
		"f6 f7 f8 f9 ncommoncardspresent ncommoncardsknown nflopc nouts ncardsknown ncardsunknown "
		"ncardsbetter nhands nhandshi nhandslo nhandsti prwinnow prlosnow nsuited nsuitedcommon "
		"tsuit tsuitcommon nranked nrankedcommon trank trankcommon nstraight nstraightcommon "
		"nstraightfill nstraightfillcommon nstraightflush nstraightflushcommon nstraightflushfill "
		"nstraightflushfillcommon rankbits rankbitscommon rankbitsplayer rankbitspoker srankbits "
		"srankbitscommon srankbitsplayer srankbitspoker rankhi rankhicommon rankhiplayer rankhipoker "
		"srankhi srankhicommon srankhiplayer srankhipoker ranklo ranklocommon rankloplayer ranklopoker "
		"sranklo sranklocommon srankloplayer sranklopoker elapsed elapsedhand elapsedauto elapsedtoday "
		"elapsed1970 clocks nclockspersecond ncps myturnbits ismyturn issittingin issittingout "
		"isautopost isfinalanswer nplayersround1 nplayersround2 nplayersround3 nplayersround4 "
		"nplayersround prevaction didchec didcall didrais didswag nbetsround1 nbetsround2 nbetsround3 "
		"nbetsround4 nbetsround didchecround1 didchecround2 didchecround3 didchecround4 "
		"didcallround1 didcallround2 didcallround3 didcallround4 didraisround1 didraisround2 "
		"didraisround3 didraisround4 didswaground1 didswaground2 didswaground3 didswaground4 "
		"ron$royfl ron$strfl ron$4kind ron$fullh ron$flush ron$strai ron$3kind ron$2pair ron$1pair "
		"ron$hcard ron$total ron$pokervalmax ron$prnuts ron$prbest ron$clocks run$royfl run$strfl "
		"run$4kind run$fullh run$flush run$strai run$3kind run$2pair run$1pair run$hcard run$total "
		"run$pokervalmax run$prnuts run$prbest run$clocks vs$nhands vs$nhandshi vs$nhandsti vs$nhandslo "
		"vs$prwin vs$prtie vs$prlos vs$prwinhi vs$prtiehi vs$prloshi vs$prwinti vs$prtieti vs$prlosti "
		"vs$prwinlo vs$prtielo vs$prloslo "
		"f$alli f$swag f$srai f$rais f$call f$prefold f$evrais f$evcall f$P f$play f$test "
		"lastraised1 lastraised2 lastraised3 lastraised4 "
		"raisbits1 raisbits2 raisbits3 raisbits4 " 
		"callbits1 callbits2 callbits3 callbits4 "
		"foldbits1 foldbits2 foldbits3 foldbits4 " 
		"oppdealt floppct turnpct riverpct avgbetspf tablepfr maxbalance handsplayed "
		"balance_rank0 balance_rank1 balance_rank2 balance_rank3 balance_rank4 balance_rank5 "
		"balance_rank6 balance_rank7 balance_rank8 balance_rank9 "; 

#define ID_SCIN_SIZERBAR 5555

static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

static UINT m_lpaIDToolBar[] = {
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

static UINT indicators[] = {
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_FORMULA_CURPOS,
};

// for rank to card translation
static char * card_chars = "23456789TJQKA";

// Window map tells CWinMgr how to position dialog controls
BEGIN_WINDOW_MAP(ScintillaFormulaMap)
  BEGINCOLS(WRCT_REST,0,RCMARGINS(4,4))

	BEGINROWS(WRCT_TOFIT,0,RCMARGINS(0,4))
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

	  BEGINCOLS(WRCT_REST,0,0)
        RCREST(IDC_SCINCTRL)				// formula edit
	  ENDGROUP()

	  RCSPACE(8)

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
					CDialog(CDlgFormulaScintilla::IDD, pParent), m_winMgr(ScintillaFormulaMap) {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif
	
#ifdef SEH_ENABLE
	try {
#endif

		in_startup = true;

		// Copy current doc formula into working set
		global.ClearFormula(&m_wrk_formula);
		global.CopyFormula(&global.formula, &m_wrk_formula);

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

		ok_to_update_debug = false;

		m_pActiveScinCtrl = NULL;
		m_pFRDlg = NULL;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::Constructor :\n"); 
		throw;
	}
#endif
}

CDlgFormulaScintilla::~CDlgFormulaScintilla() {
#ifdef SEH_ENABLE
	try {
#endif

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::Destructor :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCINCTRL, m_EmptyScinCtrl);
	DDX_Control(pDX, IDC_SCINTILLA_FORMULA_TREE, m_FormulaTree);
	DDX_Control(pDX, IDC_SCINTILLA_CALC, m_ButtonCalc);
	DDX_Control(pDX, IDC_SCINTILLA_AUTO, m_ButtonAuto);
	DDX_Control(pDX, IDC_SCINTILLA_CALC_RESULT, m_CalcResult);
	DDX_Control(pDX, IDC_FORMULA_TAB, m_TabControl);
	DDX_Control(pDX, IDC_SYMBOL_TREE, m_SymbolTree);
	DDX_Control(pDX, IDCANCEL, m_FormulaCancel);
	DDX_Control(pDX, IDC_SCINTILLA_APPLY, m_FormulaApply);
	DDX_Control(pDX, IDOK, m_FormulaOK);
}


BEGIN_MESSAGE_MAP(CDlgFormulaScintilla, CDialog)
	// Menu items
	ON_COMMAND(ID_FORMULA_FILE_APPLY, &CDlgFormulaScintilla::OnBnClickedApply)
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

	ON_NOTIFY(TCN_SELCHANGE, IDC_FORMULA_TAB, OnTabSelectionChange)
	
	ON_NOTIFY(TVN_GETINFOTIP, IDC_SYMBOL_TREE, OnSymbolTreeTipInfo)
	ON_NOTIFY(NM_RCLICK, IDC_SYMBOL_TREE, OnSymbolContextMenu)

	// Tooltips
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)

	// Timer
	ON_WM_TIMER()

	ON_COMMAND(ID_FORMULA_DEBUG_MYTURN, &CDlgFormulaScintilla::OnFormulaDebugMyturn)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_FORMULA_VIEW_SORTUDF, &CDlgFormulaScintilla::OnFormulaViewSortudf)
END_MESSAGE_MAP()

CScintillaWnd *CDlgFormulaScintilla::SetupScintilla(CScintillaWnd *pWnd) {
#ifdef SEH_ENABLE
	try {
#endif
	if (!pWnd) {
		pWnd = new CScintillaWnd();
		m_ScinArray.Add(pWnd);

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
	pWnd->SendMessage(SCI_SETKEYWORDS, 0, (LPARAM) keywords);
	pWnd->EnableWindow(false);
	set_style_colors(pWnd, m_is_syntax_colored);

	LOGFONT logfont;
	editfont.GetLogFont(&logfont);
	for (int i=0; i<=MAX_STYLE_NUM; i++) {
		pWnd->SetFontname(i, logfont.lfFaceName);
		pWnd->SetFontheight(i, -logfont.lfHeight);
		pWnd->SetBold(i, (logfont.lfWeight==FW_BOLD ? true : false));
		pWnd->SetItalic(i, logfont.lfItalic);
	}

	return pWnd;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::SetupScintilla :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::DeleteScintilla(CScintillaWnd *pWnd) {
#ifdef SEH_ENABLE
	try {
#endif

	for (int i=0; i<m_ScinArray.GetSize(); i++)
	{
		if (m_ScinArray[i] == pWnd) {
			m_ScinArray.RemoveAt(i);
		}
	}
	pWnd->DestroyWindow();
	delete pWnd;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::DeleteScintilla :\n"); 
		throw;
	}
#endif
}

// CDlgFormulaScintilla message handlers
BOOL CDlgFormulaScintilla::OnInitDialog() {
#ifdef SEH_ENABLE
	try {
#endif
		int			i, N;
		Registry	reg;
		int			max_x, max_y;

		in_startup = true;

		CDialog::OnInitDialog();

		// Restore font of Scintilla control
		if (!reg.GetProfileFont("Font", "Formula", editfont)) {
			// Use 8pt Courier (monospace) default
			editfont.CreatePointFont(100,"Courier");
		}

		// Get saved registry settings
		reg.read_reg();

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
		resize_dialog_for_control_bars();

		// Accelerators
		HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_FORMULA_EDIT_ACC), RT_ACCELERATOR);
		m_hEditAccelTable = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_FORMULA_EDIT_ACC));
		hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_FORMULA_DIALOG_ACC), RT_ACCELERATOR);
		m_hDialogAccelTable = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_FORMULA_DIALOG_ACC));

		m_TabControl.InsertItem(0, "Formulas");
		m_TabControl.InsertItem(1, "Symbols");

		// Setup the tree
		HTREEITEM	parent;

		parent = m_FormulaTree.InsertItem("Standard Functions");
		m_FormulaTree.SetItemState(parent, TVIS_BOLD | (reg.expand_std ? TVIS_EXPANDED : 0), TVIS_BOLD | (reg.expand_std ? TVIS_EXPANDED : 0) );
		m_FormulaTree.InsertItem("notes", parent);
		m_FormulaTree.InsertItem("dll", parent);
		m_FormulaTree.InsertItem("f$alli", parent);
		m_FormulaTree.InsertItem("f$swag", parent);
		m_FormulaTree.InsertItem("f$srai", parent);
		m_FormulaTree.InsertItem("f$rais", parent);
		m_FormulaTree.InsertItem("f$call", parent);
		m_FormulaTree.InsertItem("f$prefold", parent);
		m_FormulaTree.InsertItem("f$evrais", parent);
		m_FormulaTree.InsertItem("f$evcall", parent);
		m_FormulaTree.InsertItem("f$P", parent);
		m_FormulaTree.InsertItem("f$play", parent);
		m_FormulaTree.InsertItem("f$test", parent);
		m_FormulaTree.InsertItem("f$debug", parent);

		parent = m_FormulaTree.InsertItem("Hand Lists");
		m_FormulaTree.SetItemState(parent, TVIS_BOLD | (reg.expand_list ? TVIS_EXPANDED : 0), TVIS_BOLD | (reg.expand_list ? TVIS_EXPANDED : 0) );
		N = (int) m_wrk_formula.mHandList.GetSize();
		for (i=0; i<N; i++) {
			m_FormulaTree.InsertItem(m_wrk_formula.mHandList[i].list, parent);
		}

		parent = m_FormulaTree.InsertItem("User Defined Functions");
		m_FormulaTree.SetItemState(parent, TVIS_BOLD | (reg.expand_udf ? TVIS_EXPANDED : 0), TVIS_BOLD | (reg.expand_udf ? TVIS_EXPANDED : 0) );
		N = (int) m_wrk_formula.mFunction.GetSize();
		for (i=0; i<N; i++) {
			if (m_wrk_formula.mFunction[i].func != "notes" &&
				m_wrk_formula.mFunction[i].func != "dll" &&
				m_wrk_formula.mFunction[i].func != "f$alli" &&
				m_wrk_formula.mFunction[i].func != "f$swag" &&
				m_wrk_formula.mFunction[i].func != "f$srai" &&
				m_wrk_formula.mFunction[i].func != "f$rais" &&
				m_wrk_formula.mFunction[i].func != "f$call" &&
				m_wrk_formula.mFunction[i].func != "f$prefold" &&
				m_wrk_formula.mFunction[i].func != "f$evrais" &&
				m_wrk_formula.mFunction[i].func != "f$evcall" &&
				m_wrk_formula.mFunction[i].func != "f$P" &&
				m_wrk_formula.mFunction[i].func != "f$play" &&
				m_wrk_formula.mFunction[i].func != "f$test" &&
				m_wrk_formula.mFunction[i].func != "f$debug" ) {
				m_FormulaTree.InsertItem(m_wrk_formula.mFunction[i].func, parent);
			}
		}

		SetupScintilla(&m_EmptyScinCtrl);
		m_ScinArray.Add(&m_EmptyScinCtrl);
		m_pActiveScinCtrl = &m_EmptyScinCtrl;
	
		// Set dialog title
		::SetWindowText(m_hWnd, "Formula Editor");

		// Set dialog icon
		HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
		this->SetIcon(hIcon, FALSE);

		// Restore window location and size, precision preference
		max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
		max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
		::SetWindowPos(m_hWnd, HWND_TOP, min(reg.formula_x, max_x), min(reg.formula_y, max_y),
			reg.formula_dx, reg.formula_dy, SWP_NOCOPYBITS);
		m_precision = reg.precision;
		m_equal = reg.equal;

		// Debug logging preferences
		m_fdebuglog = reg.fdebuglog;
		m_fdebuglog_myturn = reg.fdebuglog_myturn;
		m_wrote_fdebug_header = false;

		// Sort UDF?
		m_udf_sort = reg.udf_sort;
		if (m_udf_sort)
			sort_udf_tree();

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

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnInitDialog :\n"); 
		throw;
	}
#endif
}

BOOL CDlgFormulaScintilla::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
    {
		if(m_hDialogAccelTable != NULL &&  ::TranslateAccelerator(m_hWnd, m_hDialogAccelTable, pMsg))
		   return TRUE;
		if (m_pActiveScinCtrl && GetFocus() == m_pActiveScinCtrl) {
			if(m_hEditAccelTable != NULL &&  ::TranslateAccelerator(m_hWnd, m_hEditAccelTable, pMsg))
			   return TRUE;
		}
    }
    return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFormulaScintilla::resize_dialog_for_control_bars(void) {
#ifdef SEH_ENABLE
	try {
#endif
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
		while (pwndChild) {                              
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
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::resize_dialog_for_control_bars :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnTvnSelchangingFormulaTree(NMHDR *pNMHDR, LRESULT *pResult) {
#ifdef SEH_ENABLE
	try {
#endif
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		
		LastChangeToFormula(&m_wrk_formula);

		*pResult = 0;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnTvnSelchangingFormulaTree :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnTabSelectionChange(NMHDR *pNMHDR, LRESULT *pResult) 
{
#ifdef SEH_ENABLE
	try {
#endif
	bool bFormulaShowing = (m_TabControl.GetCurSel() == 0);
	if (!bFormulaShowing)
		PopulateSymbols();

	m_FormulaTree.ShowWindow(bFormulaShowing ? SW_SHOW : SW_HIDE);
	m_SymbolTree.ShowWindow(!bFormulaShowing ? SW_SHOW : SW_HIDE);
	m_FormulaTree.EnableWindow(bFormulaShowing);
	m_SymbolTree.EnableWindow(!bFormulaShowing);

	HandleEnables(true);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnTabSelectionChange() :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnSymbolTreeTipInfo(NMHDR *pNMHDR, LRESULT *pResult) 
{
#ifdef SEH_ENABLE
	try {
#endif
	LPNMTVGETINFOTIP lpGetInfoTip = (LPNMTVGETINFOTIP)pNMHDR;

	if (lpGetInfoTip && lpGetInfoTip->hItem) 
	{
		const char *description = (const char *)m_SymbolTree.GetItemData(lpGetInfoTip->hItem);
		if (description)
			strcpy(lpGetInfoTip->pszText, description);
	}

	*pResult = 0;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnSymbolTreeTipInfo() :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnSymbolContextMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
#ifdef SEH_ENABLE
	try {
#endif
	CPoint cursorPos, clientPos;
	cursorPos.x= GetCurrentMessage()->pt.x;
	cursorPos.y= GetCurrentMessage()->pt.y;
	clientPos = cursorPos;
	m_SymbolTree.ScreenToClient(&clientPos);
	HTREEITEM hItem = m_SymbolTree.HitTest(clientPos);

	if (hItem != NULL && m_SymbolTree.GetChildItem(hItem) == NULL) {
		CMenu contextMenu;
		contextMenu.LoadMenu(IDR_FORMULA_SYMBOL_MENU);
		int ret = contextMenu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, cursorPos.x, cursorPos.y, this);
		if (ret == ID_FORMULA_SYMBOL_COPY) {
			if (OpenClipboard() && EmptyClipboard()) {
				CString symbol;
				symbol = m_SymbolTree.GetItemText(hItem);

				HANDLE hMem = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE , symbol.GetLength()+1);
				if (hMem) {
					LPSTR lpStr = (LPSTR)::GlobalLock(hMem);
					strcpy(lpStr, symbol);
					::GlobalUnlock(hMem);
					if ( ::SetClipboardData( CF_TEXT, hMem ) != NULL )
						CloseClipboard();
				} else
					CloseClipboard();
			}
		}
	}


	*pResult = 0;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnSymbolTreeTipInfo() :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnTvnSelchangedFormulaTree(NMHDR *pNMHDR, LRESULT *pResult) {
#ifdef SEH_ENABLE
	try {
#endif
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		CString			s;
		int				N, i;
		CMenu			*edit_menu = this->GetMenu()->GetSubMenu(1);

		ok_to_update_debug = false;

		s = m_FormulaTree.GetItemText(m_FormulaTree.GetSelectedItem());

		StopAutoButton();

		// A root item was selected
		if (m_FormulaTree.GetParentItem(m_FormulaTree.GetSelectedItem()) == NULL) 
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
		else {
			if (s == "notes" || s == "dll") {
				SetWindowText("Formula Editor - " + s);
				N = (int) m_wrk_formula.mFunction.GetSize();
				for (i=0; i<N; i++) {
					if (m_wrk_formula.mFunction[i].func == s) {
						CScintillaWnd *pCurScin = reinterpret_cast<CScintillaWnd *>(m_FormulaTree.GetItemData(m_FormulaTree.GetSelectedItem()));
						if (!pCurScin) {
							pCurScin = SetupScintilla(NULL);
							m_FormulaTree.SetItemData(m_FormulaTree.GetSelectedItem(), (DWORD_PTR)pCurScin);

							pCurScin->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
							pCurScin->SetText(m_wrk_formula.mFunction[i].func_text.GetString());
							pCurScin->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
							pCurScin->SendMessage(SCI_EMPTYUNDOBUFFER, 0, 0);
						}

						ASSERT(pCurScin != NULL);
						if (m_pActiveScinCtrl) {
							m_pActiveScinCtrl->ShowWindow(SW_HIDE);
							m_pActiveScinCtrl->EnableWindow(false);
						}
						m_pActiveScinCtrl = pCurScin;
						m_pActiveScinCtrl->ShowWindow(SW_SHOW);
						m_pActiveScinCtrl->EnableWindow(true);

						m_current_edit = m_wrk_formula.mFunction[i].func;
						i=N+1;
					}
				}
			}
			else if (memcmp(s.GetString(), "f$", 2) == 0) {
				SetWindowText("Formula Editor - " + s);
				N = (int) m_wrk_formula.mFunction.GetSize();
				for (i=0; i<N; i++) {
					if (m_wrk_formula.mFunction[i].func == s) {
						CScintillaWnd *pCurScin = reinterpret_cast<CScintillaWnd *>(m_FormulaTree.GetItemData(m_FormulaTree.GetSelectedItem()));
						if (!pCurScin) {
							pCurScin = SetupScintilla(NULL);
							m_FormulaTree.SetItemData(m_FormulaTree.GetSelectedItem(), (DWORD_PTR)pCurScin);

							pCurScin->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
							pCurScin->SetText(m_wrk_formula.mFunction[i].func_text.GetString());
							pCurScin->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
							pCurScin->SendMessage(SCI_EMPTYUNDOBUFFER, 0, 0);
						}

						ASSERT(pCurScin != NULL);
						if (m_pActiveScinCtrl) {
							m_pActiveScinCtrl->ShowWindow(SW_HIDE);
							m_pActiveScinCtrl->EnableWindow(false);
						}
						m_pActiveScinCtrl = pCurScin;
						m_pActiveScinCtrl->ShowWindow(SW_SHOW);
						m_pActiveScinCtrl->EnableWindow(true);

						m_current_edit = m_wrk_formula.mFunction[i].func;
						i=N+1;
					}
				}
			}
			else if (memcmp(s.GetString(), "list", 4) == 0) {
				// Find proper list and display it
				N = (int) m_wrk_formula.mHandList.GetSize();
				for (i=0; i<N; i++) {
					if (m_wrk_formula.mHandList[i].list == s) {
						SetWindowText("Formula Editor - " + s);

						CScintillaWnd *pCurScin = reinterpret_cast<CScintillaWnd *>(m_FormulaTree.GetItemData(m_FormulaTree.GetSelectedItem()));
						if (!pCurScin) {
							pCurScin = SetupScintilla(NULL);
							m_FormulaTree.SetItemData(m_FormulaTree.GetSelectedItem(), (DWORD_PTR)pCurScin);

							pCurScin->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
							pCurScin->SetText(m_wrk_formula.mHandList[i].list_text.GetString());
							pCurScin->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
							pCurScin->SendMessage(SCI_EMPTYUNDOBUFFER, 0, 0);
						}

						ASSERT(pCurScin != NULL);
						if (m_pActiveScinCtrl) {
							m_pActiveScinCtrl->ShowWindow(SW_HIDE);
							m_pActiveScinCtrl->EnableWindow(false);
						}
						m_pActiveScinCtrl = pCurScin;
						m_pActiveScinCtrl->ShowWindow(SW_SHOW);
						m_pActiveScinCtrl->EnableWindow(true);

						m_current_edit = m_wrk_formula.mHandList[i].list;
						i=N+1;
					}
				}
			}
		}

		if (s == "f$debug") {
			init_debug_array();
		}

		HandleEnables(true);

		*pResult = 0;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnTvnSelchangedFormulaTree :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnNew() {
#ifdef SEH_ENABLE
	try {
#endif
		CDlgNew newdlg;
		SFunction Func; 
		SHandList List;
		CString	s;
		HTREEITEM	p, newhtitem;

		StopAutoButton();

		s = m_FormulaTree.GetItemText(m_FormulaTree.GetSelectedItem());
		
		if (s == "Hand Lists" || (s.Find("list") != -1 && s.Find("f$") == -1))
			newdlg.type = 0;
		else
			newdlg.type = 1;

		if (newdlg.DoModal() == IDOK) {
			if (newdlg.type == 0) {
				List.list = newdlg.CSnewname; 
				List.list_text = "";

				// Add it to working set CArray
				m_wrk_formula.mHandList.Add(List); 

				// Add to tree
				p = m_FormulaTree.GetParentItem(m_FormulaTree.GetSelectedItem());
				if (p == NULL) {  // Selected item is a parent item
					newhtitem = m_FormulaTree.InsertItem(newdlg.CSnewname, m_FormulaTree.GetSelectedItem());
				}
				else {
					newhtitem = m_FormulaTree.InsertItem(newdlg.CSnewname, p);
				}
			}
			else {
				Func.func = newdlg.CSnewname;
				Func.func_text = "";
				Func.fresh = false;

				// Add it to working set CArray
				m_wrk_formula.mFunction.Add(Func); 

				// Add to tree
				p = m_FormulaTree.GetParentItem(m_FormulaTree.GetSelectedItem());
				if (p == NULL) {  // Selected item is a parent item
					newhtitem = m_FormulaTree.InsertItem(newdlg.CSnewname, m_FormulaTree.GetSelectedItem());
				}
				else {
					newhtitem = m_FormulaTree.InsertItem(newdlg.CSnewname, p);
				}

				if (m_udf_sort)
					sort_udf_tree();
			}

			// Select newly inserted item
			m_FormulaTree.SelectItem(newhtitem);
			m_FormulaTree.SetFocus();
			m_dirty = true;
			HandleEnables(true);
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnNew :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnRename() {
#ifdef SEH_ENABLE
	try {
#endif
		CDlgRename rendlg;
		CString	s;
		char str[512];
		int N, i;
		bool didRename = false;

		StopAutoButton();

		s = m_FormulaTree.GetItemText(m_FormulaTree.GetSelectedItem());
		strcpy(str, s.GetString());

		rendlg.CSoldname = s;

		if (rendlg.DoModal() == IDOK) {
			// Renaming a list
			if (memcmp(str, "list", 4) == 0) {
				// Find proper list
				N = (int) m_wrk_formula.mHandList.GetSize();
				for (i=0; i<N; i++) {
					if (m_wrk_formula.mHandList[i].list == rendlg.CSoldname) {
						// Update it in the CArray working set
						m_wrk_formula.mHandList[i].list = rendlg.CSnewname;
						didRename = true;
						i=N+1; // Spew: I am not sure why the loop continues and skips over the next item on the list but it does.  I would hope that you do not have duplicate names
					}
				}
			}

			// Renaming a UDF
			else if (memcmp(str, "f$", 2) == 0) {
				// Find proper UDF and display it
				N = (int) m_wrk_formula.mFunction.GetSize();
				for (i=0; i<N; i++) {
					if (m_wrk_formula.mFunction[i].func == s) {
						// Update it in the CArray working set
						m_wrk_formula.mFunction[i].func = rendlg.CSnewname;
						m_wrk_formula.mFunction[i].fresh = false;
						didRename = true;
						i=N+1; // Spew: I am not sure why the loop continues and skips over the next item on the list but it does.  I would hope that you do not have duplicate names
					}
				}
			}
			if (didRename) {
				m_FormulaTree.SetItemText(m_FormulaTree.GetSelectedItem(), rendlg.CSnewname);
				if (m_udf_sort && memcmp(str, "f$", 2) == 0)
					sort_udf_tree();
				SetWindowText("Formula - " + rendlg.CSnewname);
				m_FormulaTree.SetFocus();
				m_dirty = true;
				HandleEnables(true);
			}
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnRename :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnDelete() {
#ifdef SEH_ENABLE
	try {
#endif
		int ret, N, i;
		HTREEITEM h = m_FormulaTree.GetSelectedItem();
		CString s = m_FormulaTree.GetItemText(m_FormulaTree.GetSelectedItem());
		CMenu *file_menu = this->GetMenu()->GetSubMenu(0);

		StopAutoButton();

		ret = MessageBox("REALLY delete \"" + m_FormulaTree.GetItemText(m_FormulaTree.GetSelectedItem()) + "\" ?", 
			"Confirm Delete", MB_YESNO | MB_ICONWARNING);

		if (ret == IDYES) {

			// Delete a UDF
			if (s.Find("f$") != -1) {
				N = (int) m_wrk_formula.mFunction.GetSize();
				for (i=0; i<N; i++) {
					if (m_wrk_formula.mFunction[i].func == s) {
						// Update the dialog
						//m_FormulaTree.SelectItem(NULL);
						m_FormulaTree.SetFocus();
						SetWindowText("Formula - ");
						if (m_pActiveScinCtrl) {
							m_pActiveScinCtrl->ShowWindow(SW_HIDE);
							m_pActiveScinCtrl->EnableWindow(false);
						}
						m_pActiveScinCtrl = &m_EmptyScinCtrl;
						m_pActiveScinCtrl->ShowWindow(SW_SHOW);
						m_pActiveScinCtrl->EnableWindow(false);

						// Remove it from the CArray working set
						m_wrk_formula.mFunction.RemoveAt(i, 1);
						// Update the tree
						m_FormulaTree.DeleteItem(h);
						m_dirty = true;
						i=N+1;
					}
				}
			}

			// Delete a list
			else if (s.Find("list") != -1) {
				N = (int) m_wrk_formula.mHandList.GetSize();
				for (i=0; i<N; i++) {
					if (m_wrk_formula.mHandList[i].list == s) {
						// Update the dialog
						//m_FormulaTree.SelectItem(NULL);
						m_FormulaTree.SetFocus();
						SetWindowText("Formula - ");
						if (m_pActiveScinCtrl) {
							m_pActiveScinCtrl->ShowWindow(SW_HIDE);
							m_pActiveScinCtrl->EnableWindow(false);
						}
						m_pActiveScinCtrl = &m_EmptyScinCtrl;
						m_pActiveScinCtrl->ShowWindow(SW_SHOW);
						m_pActiveScinCtrl->EnableWindow(false);

						// Remove it from the CArray working set
						m_wrk_formula.mHandList.RemoveAt(i, 1);
						// Update the tree
						m_FormulaTree.DeleteItem(h);
						m_dirty = true;
						i=N+1;
					}
				}
			}
		}
		HandleEnables(true);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnDelete :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnToggleBookmark()
{
	if (!m_pActiveScinCtrl)
		return;
	int iLine = m_pActiveScinCtrl->GetCurrentLine();
	if (m_pActiveScinCtrl->HasBookmark(iLine))
		m_pActiveScinCtrl->DeleteBookmark(iLine);
	else {
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
	if (lpFindReplace && m_pActiveScinCtrl) {
		if (lpFindReplace->Flags & FR_DIALOGTERM) {
			CloseFindReplaceDialog();
		} else if (lpFindReplace->Flags & FR_FINDNEXT) {
			m_FindLastSearch = lpFindReplace->lpstrFindWhat;
			m_FindMatchCase = (lpFindReplace->Flags & FR_MATCHCASE ? true : false);
			m_FindWholeWord = (lpFindReplace->Flags & FR_WHOLEWORD ? true : false);
			DoFind((lpFindReplace->Flags & FR_DOWN ? true : false));
			HandleEnables(true);
		} else if (lpFindReplace->Flags & FR_REPLACE) {
		} else if (lpFindReplace->Flags & FR_REPLACEALL) {
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
   if( NULL == m_pFRDlg )
   {
      m_pFRDlg = new CFindReplaceDialog();  // Must be created on the heap
      m_pFRDlg->Create(TRUE, "", "", FR_DOWN, this); 
      m_pFRDlg->m_fr.lStructSize = sizeof(FINDREPLACE);
      m_pFRDlg->m_fr.hwndOwner = this->m_hWnd;
   }
}

void CDlgFormulaScintilla::OnFont() {
#ifdef SEH_ENABLE
	try {
#endif
		CFontDialog fontdlg;
		LOGFONT curlf, newlf;
		Registry reg;
		COLORREF g_rgbText = RGB(0, 0, 0);
		int	i;

		editfont.GetLogFont(&curlf);
		fontdlg.m_cf.lStructSize = sizeof(CHOOSEFONT);
		fontdlg.m_cf.Flags |= CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
		fontdlg.m_cf.hwndOwner = NULL;
		fontdlg.m_cf.lpLogFont = &curlf;
		fontdlg.m_cf.rgbColors = g_rgbText;
		fontdlg.m_cf.Flags ^= CF_EFFECTS;
		fontdlg.m_cf.Flags |= CF_NOSCRIPTSEL;

		if(fontdlg.DoModal() == IDOK) {
			// Create the font using the selected font from CFontDialog.
			memcpy(&newlf, fontdlg.m_cf.lpLogFont, sizeof(LOGFONT));
			editfont.DeleteObject();
			editfont.CreateFontIndirect(&newlf);

			// Save the new font
			reg.WriteProfileFont("Font", "Formula", editfont);

			// Set the new font to all Scintilla windows
			for (int iScint=0; iScint<m_ScinArray.GetSize(); iScint++)
			{
				CScintillaWnd *pCurScint = m_ScinArray[iScint];
				for (i=0; i<=MAX_STYLE_NUM; i++) {
					pCurScint->SetFontname(i, fontdlg.m_cf.lpLogFont->lfFaceName);
					pCurScint->SetFontheight(i, -fontdlg.m_cf.lpLogFont->lfHeight);
					pCurScint->SetBold(i, (fontdlg.m_cf.lpLogFont->lfWeight==FW_BOLD ? true : false));
					pCurScint->SetItalic(i, fontdlg.m_cf.lpLogFont->lfItalic);
				}
			}
		}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnFont :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnSettings() {
#ifdef SEH_ENABLE
	try {
#endif
		CDlgSettings		myDialog;
		CMenu				*file_menu = this->GetMenu()->GetSubMenu(0);
		COpenHoldemDoc		*pDoc = COpenHoldemDoc::GetDocument();

		myDialog.bankroll = m_wrk_formula.dBankroll;
		myDialog.defcon = m_wrk_formula.dDefcon;
		myDialog.rake = m_wrk_formula.dRake;
		myDialog.nit = m_wrk_formula.dNit;

		if(myDialog.DoModal() == IDOK) {
			m_wrk_formula.dBankroll = myDialog.bankroll;
			m_wrk_formula.dDefcon = myDialog.defcon;
			m_wrk_formula.dRake = myDialog.rake;
			m_wrk_formula.dNit = myDialog.nit;

			m_dirty = true;
			pDoc->SetModifiedFlag(true);
			HandleEnables(true);
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnSettings :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnHandList() {
#ifdef SEH_ENABLE
	try {
#endif
		CDlgHandList		myDialog;
		CString				s = m_FormulaTree.GetItemText(m_FormulaTree.GetSelectedItem());
		int					list_index, i, j;
		CMenu				*file_menu = this->GetMenu()->GetSubMenu(0);
		CString				token, hand, newstring;
		bool				do_crlf;

		// Find appropriate list in the internal structure
		list_index = -1;
		for (i=0; i<m_wrk_formula.mHandList.GetSize() && list_index == -1; i++) {
			if (m_wrk_formula.mHandList[i].list == s)
				list_index = i;
		}
		if (list_index == -1)  return;

		CGlobal::ParseHandList(m_wrk_formula.mHandList[list_index].list_text, myDialog.checked);

		// Window title and static text content
		myDialog.hand_list_num = atoi(s.Mid(4).GetString());

		// Start dialog
		if(myDialog.DoModal() == IDOK) {

			// Save handlist as string and update display
			newstring = "";
			// pairs
			do_crlf = false;
			for (i=12; i>=0; i--)
			{
				if (myDialog.checked[i][i])
				{
					newstring += card_chars[i];
					newstring += card_chars[i];
					newstring += "  ";
					do_crlf = true;
				}
			}
			
			if (do_crlf)
				newstring += "\r\n";
			
			// suiteds
			do_crlf = false;
			for (i=12; i>=1; i--)
			{
				for (j=i-1; j>=0; j--)
				{
					if (myDialog.checked[i][j])
					{
						newstring += card_chars[i];
						newstring += card_chars[j];
						newstring += "s ";
						do_crlf = true;
					}
				}
			}
			
			if (do_crlf)
				newstring += "\r\n";

			// unsuiteds
			do_crlf = false;
			for (i=11; i>=0; i--)
			{
				for (j=12; j>=i+1; j--)
				{
					if (myDialog.checked[i][j])
					{
						newstring += card_chars[i];
						newstring += card_chars[j];
						newstring += "  ";
						do_crlf = true;
					}
				}
			}
			
			if (do_crlf)
				newstring += "\r\n";

			// save it internally
			m_wrk_formula.mHandList[list_index].list_text = newstring;

			// update scintilla window
			CScintillaWnd *pCurScin = reinterpret_cast<CScintillaWnd *>(m_FormulaTree.GetItemData(m_FormulaTree.GetSelectedItem()));
			ASSERT(pCurScin != NULL);
			if (pCurScin) {
				pCurScin->SetText(m_wrk_formula.mHandList[list_index].list_text.GetString());
			}

			m_dirty = true;
		}
		HandleEnables(true);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnHandList :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::LastChangeToFormula(SFormula *f) {
#ifdef SEH_ENABLE
	try {
#endif
		CString	s;
		int N, i;

		HTREEITEM hSelected = m_FormulaTree.GetSelectedItem();
		if (hSelected == NULL)
			return;

		s = m_FormulaTree.GetItemText(hSelected);
		CScintillaWnd *pCurScin = reinterpret_cast<CScintillaWnd *>(m_FormulaTree.GetItemData(hSelected));

		// A root item was selected, do nothing
		if (m_FormulaTree.GetParentItem(m_FormulaTree.GetSelectedItem()) == NULL) 
		{
		}
		else if (!pCurScin) {
			ASSERT(false);
		}
		// A child item was selected, copy text from rich edit control to working set CArray
		else {
			// check for function
			N = (int) f->mFunction.GetSize();
			for (i=0; i<N; i++) {
				if (f->mFunction[i].func == s) {
					pCurScin->GetText(f->mFunction[i].func_text); 
					f->mFunction[i].dirty = true;
					f->mFunction[i].fresh = false;
					return;
				}
			}

			// check for list
			if (memcmp(s.GetString(), "list", 4) == 0) {
				// Find proper list and load it
				N = (int) f->mHandList.GetSize();
				for (i=0; i<N; i++) {
					if (f->mHandList[i].list == s) {
						pCurScin->GetText(f->mHandList[i].list_text);
						return;
					}
				}
			}
		}	
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::LastChangeToFormula :\n"); 
		throw;
	}
#endif
}


BOOL CDlgFormulaScintilla::DestroyWindow()
{
#ifdef SEH_ENABLE
	try {
#endif
		COpenHoldemDoc		*pDoc = COpenHoldemDoc::GetDocument();
		CMainFrame			*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

		StopAutoButton();

		save_settings_to_registry();

		if (m_dirty) {
			if (!PromptToSave())
				return FALSE;
		}

		CloseFindReplaceDialog();

		// Uncheck formula button on main toolbar
		pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, false);
		global.ClearFormula(&m_wrk_formula);
		return CDialog::DestroyWindow();

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::DestroyWindow :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::PostNcDestroy() 
{
#ifdef SEH_ENABLE
	try {
#endif
		editfont.DeleteObject();

		delete m_formulaScintillaDlg;
		m_formulaScintillaDlg	=	NULL;

		CDialog::PostNcDestroy();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::PostNcDestroy :\n"); 
		throw;
	}
#endif
}

BOOL CDlgFormulaScintilla::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
#ifdef SEH_ENABLE
	try {
#endif
		
		NMHDR *phDR;
		phDR = (NMHDR*)lParam;
		CMenu *file_menu = this->GetMenu()->GetSubMenu(0);
		CString s;

		if (in_startup)  { return true; }

		// does notification come from my scintilla control?
		if (phDR != NULL && phDR->hwndFrom == m_pActiveScinCtrl->m_hWnd) {
			SCNotification *pMsg = (SCNotification*)lParam;
			switch (phDR->code) {
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
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnNotify :\n"); 
		throw;
	}
#endif
}

BOOL CDlgFormulaScintilla::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult) {
#ifdef SEH_ENABLE
	try {
#endif
		// allow top level routing frame to handle the message
		if (GetRoutingFrame() != NULL)
			return false;

		// need to handle both ANSI and UNICODE versions of the message
		TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
		TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
		TCHAR szFullText[256];
		CString strTipText;
		UINT nID = (UINT) pNMHDR->idFrom;
		if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
			pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND)) {
			// idFrom is actually the HWND of the tool
			nID = ::GetDlgCtrlID((HWND)nID);
		}

		if (nID != 0) { // will be zero on a separator
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
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnToolTipText :\n"); 
		throw;
	}
#endif
} 

void CDlgFormulaScintilla::ToggleToolbar() {
#ifdef SEH_ENABLE
	try {
#endif
		CMenu *view_menu = this->GetMenu()->GetSubMenu(2);

		if (m_toolBar.IsWindowVisible()) {
			m_toolBar.ShowWindow(SW_HIDE);
			view_menu->CheckMenuItem(VIEW_TOOLBAR, MF_BYPOSITION | MF_UNCHECKED);
			m_is_toolbar_visible = false;
		}
		else {
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

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::ToggleToolbar :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::ToggleLineNumbers() {
#ifdef SEH_ENABLE
	try {
#endif
		m_are_linenumbers_visible = !m_are_linenumbers_visible;
		m_pActiveScinCtrl->SetDisplayLinenumbers(m_are_linenumbers_visible);

		HandleEnables(true);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::ToggleLineNumbers :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::ToggleSelectionMargin() {
#ifdef SEH_ENABLE
	try {
#endif
		m_is_selection_margin_visible = !m_is_selection_margin_visible;
		m_pActiveScinCtrl->SetDisplaySelection(m_is_selection_margin_visible);

		HandleEnables(true);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::ToggleSelectionMargin :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::ToggleFoldingMargin() {
#ifdef SEH_ENABLE
	try {
#endif
		m_is_folding_margin_visible = !m_is_folding_margin_visible;
		m_pActiveScinCtrl->SetDisplayFolding(m_is_folding_margin_visible);

		HandleEnables(true);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::ToggleFoldingMargin :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::ToggleSyntaxColoring() {
#ifdef SEH_ENABLE
	try {
#endif
		m_is_syntax_colored = !m_is_syntax_colored;
		set_style_colors(m_pActiveScinCtrl, m_is_syntax_colored);

		HandleEnables(true);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::ToggleSyntaxColoring :\n"); 
		throw;
	}
#endif
}

LRESULT CDlgFormulaScintilla::OnWinMgr(WPARAM wp, LPARAM lp) {
#ifdef SEH_ENABLE
	try {
#endif
		ASSERT(lp);
		NMWINMGR& nmw = *(NMWINMGR*)lp;

		if (nmw.code==NMWINMGR::GET_SIZEINFO) {
			if (wp==(WORD)GetDlgCtrlID()) {
				// Parent frame is requesting my size info. Report min size.
				m_winMgr.GetMinMaxInfo(this, nmw.sizeinfo);
				return true; // handled--important!
			}
		} 
		
		else if (nmw.code==NMWINMGR::SIZEBAR_MOVED) {
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
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnWinMgr :\n"); 
		throw;
	}
#endif
}


void CDlgFormulaScintilla::OnBnClickedLessPrecision() {
#ifdef SEH_ENABLE
	try {
#endif
		CString s;

		if (m_precision > 0) {
			m_precision--;
			create_debug_tab(&s);
			m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
			m_pActiveScinCtrl->SetText(s.GetString());
			m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnBnClickedLessPrecision :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnBnClickedMorePrecision() {
#ifdef SEH_ENABLE
	try {
#endif
		CString s;

		m_precision++;
		create_debug_tab(&s);
		m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
		m_pActiveScinCtrl->SetText(s.GetString());
		m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnBnClickedMorePrecision :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnBnClickedEqualLeft() {
#ifdef SEH_ENABLE
	try {
#endif
		CString s;

		if (m_equal > 0) {
			m_equal--;
			create_debug_tab(&s);
			m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
			m_pActiveScinCtrl->SetText(s.GetString());
			m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnBnClickedEqualLeft :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnBnClickedEqualRight() {
#ifdef SEH_ENABLE
	try {
#endif
		CString s;

		m_equal++;
		create_debug_tab(&s);
		m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
		m_pActiveScinCtrl->SetText(s.GetString());
		m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnBnClickedEqualRight :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::ResizeScintillaWindows()
{
#ifdef SEH_ENABLE
	try {
#endif
		if (::IsWindow(m_EmptyScinCtrl.GetSafeHwnd())) {
			CRect rc;
			m_EmptyScinCtrl.GetWindowRect(rc);
			ScreenToClient(rc);

			HDWP hdwp = ::BeginDeferWindowPos(m_ScinArray.GetSize()-1);
			for (int i=1; i<m_ScinArray.GetSize(); i++)
			{
				::DeferWindowPos(hdwp, m_ScinArray[i]->GetSafeHwnd(), NULL, rc.left,rc.top,rc.Width(),rc.Height(), SWP_NOZORDER);
			}
			::EndDeferWindowPos(hdwp);
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::ResizeScintillaWindows :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnSize(UINT nType, int cx, int cy) {
#ifdef SEH_ENABLE
	try {
#endif

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

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnSize :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnBnClickedCalc() {
#ifdef SEH_ENABLE
	try {
#endif
		CString					Cstr, title, s;
		double					ret;
		int						error, i;
		std::string				str;
		struct SDebugTabInfo	debug_struct;
		LARGE_INTEGER			bcount, ecount, lFrequency;
		char					format[50];
		
		StopAutoButton();

		// Clear result box
		m_CalcResult.SetWindowText("");

		// Caclulate symbols
		//symbols.CalcSymbols();

		// mark symbol result cache as stale
		for (i=0; i < (int) m_wrk_formula.mFunction.GetSize(); i++) 
			m_wrk_formula.mFunction[i].fresh = false;

		// Make sure we "calc" the latest
		LastChangeToFormula(&m_wrk_formula);

		// calc hand lists
		global.create_hand_list_matrices(&m_wrk_formula);

		// Validate parse trees
		if (!global.ParseAllFormula(this->GetSafeHwnd(), &m_wrk_formula)) {
			s.Format("There are syntax errors in one or more formulas that are\n");
			s.Append("preventing calculation of this formula.\n");
			s.Append("These errors need to be corrected before the 'Calc'\n");
			s.Append("button can be used.");
			MessageBox(s, "PARSE ERROR(s)", MB_OK);
			return;
		}

		//
		// f$debug is a special case, handle it first
		//
		if (m_current_edit == "f$debug") {
			LastChangeToFormula(&m_wrk_formula);
			init_debug_array();
			update_debug_auto();
		}

		// Processing for any other formula
		else {
			// time how long it takes
			QueryPerformanceCounter(&bcount);

			// Execute the currently selected formula
			error = SUCCESS;
			ret = calc_f$symbol(&m_wrk_formula, (char *) m_current_edit.GetString(), &error);

			// time how long it takes
			QueryPerformanceCounter(&ecount);
			QueryPerformanceFrequency(&lFrequency);
			if (error == SUCCESS) {
				// display result
				sprintf(format, "%%.%df", m_precision);
				Cstr.Format(format, ret);
				m_CalcResult.SetWindowText(Cstr);
	
				// display total time for calculation
				title.Format("Formula Editor - %s - [%.8f]", m_current_edit.GetString(), 
					((double) (ecount.LowPart - bcount.LowPart))/((double) lFrequency.LowPart));
				SetWindowText(title);
			}
			else {
				switch (error) {
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
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnBnClickedButtonCalc :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnBnClickedAuto()
{
#ifdef SEH_ENABLE
	try {
#endif
		if (m_ButtonAuto.GetCheck() == 1)
		{
			boost::spirit::tree_parse_info<>	tpi;
			CString  s;

			ok_to_update_debug = false;
			m_CalcResult.SetWindowText("");

			// cal hand lists
			global.create_hand_list_matrices(&m_wrk_formula);

			// Validate parse trees
			if (!global.ParseAllFormula(this->GetSafeHwnd(), &m_wrk_formula)) {
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
			init_debug_array();
			
			ok_to_update_debug = true;
		}
		else {
			ok_to_update_debug = false;
			m_ButtonAuto.SetWindowText("Auto");
		}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnBnClickedAuto :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::StopAutoButton()
{
#ifdef SEH_ENABLE
	try {
#endif
		// If auto button is on, turn it off
		if (m_ButtonAuto.GetCheck() == 1) {
			m_ButtonAuto.SetCheck(0);
			OnBnClickedAuto();
		}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::StopAutoButton :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::update_debug_auto(void) {
#ifdef SEH_ENABLE
	try {
#endif
		int				N, i;
		CString			Cstr;

		global.m_WaitCursor = true;
		BeginWaitCursor();

		// mark symbol result cache as stale
		for (i=0; i < (int) m_wrk_formula.mFunction.GetSize(); i++) 
			m_wrk_formula.mFunction[i].fresh = false;


		// Loop through each line in the debug tab and evaluate it
		N = (int) debug_ar.GetSize();
		for (i=0; i<N; i++) {
			if (debug_ar[i].valid) {
				debug_ar[i].error = SUCCESS;
				debug_ar[i].ret = evaluate(&m_wrk_formula, debug_ar[i].tree, &debug_ar[i].error);
			}
			else {
				debug_ar[i].ret = 0;
			}
		}
		// Format the text
		create_debug_tab(&Cstr);

		// Write the tab's contents to a log file, if selected
		if (m_fdebuglog &&
			((m_fdebuglog_myturn && symbols.sym.ismyturn) || !m_fdebuglog_myturn)) {
			
			if (!m_wrote_fdebug_header) {
				write_fdebug_log(true);
				m_wrote_fdebug_header = true;

			}
			else {
				write_fdebug_log(false);
			}
		}

		// Display the text in the debug tab
		m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, 0, 0);
		m_pActiveScinCtrl->SendMessage(SCI_SETTEXT,0,(LPARAM)Cstr.GetString());
		m_pActiveScinCtrl->GotoPosition(0);
		m_pActiveScinCtrl->SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);

		EndWaitCursor();
		global.m_WaitCursor = false;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::update_debug_auto :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::create_debug_tab(CString *cs) {
#ifdef SEH_ENABLE
	try {
#endif
		int N, i, j;
		CString newline;
		char format[50];

		sprintf(format, "%%%d.%df = %%s", m_precision+m_equal, m_precision);
		
		*cs = "";

		N = (int) debug_ar.GetSize();
		for (i=0; i<N; i++) {
			if (debug_ar[i].valid) {
				if (debug_ar[i].error == SUCCESS) {
					newline.Format(format, debug_ar[i].ret, debug_ar[i].exp.GetString());
					newline.Append("\r\n");
					cs->Append(newline);
				}
				else {
					newline="";
					for (j=0; j<m_precision+m_equal-9; j++) {
						newline.Append(" ");
					}

					switch (debug_ar[i].error) {
						case ERR_INVALID_SYM:
							newline.Append("ERR: sym  = ");
							break;
						case ERR_INVALID_FUNC_SYM:
							newline.Append("ERR: f$   = ");
							break;
						case ERR_INVALID_DLL_SYM:
							newline.Append("ERR: dll$ = ");
							break;
						case ERR_INVALID_EXPR:
							newline.Append("ERR: expr = ");
							break;
						case ERR_DIV_ZERO:
							newline.Append("ERR: div0 = ");
							break;
						default:
							newline.Append("ERR: unkn = ");
							break;
					}

					//newline.Append("ERROR = ");
					newline.Append(debug_ar[i].exp);
					newline.Append("\r\n");
					cs->Append(newline);
				}
			}
			else {
				cs->Append(debug_ar[i].exp);
				cs->Append("\r\n");
			}
		}

		// Remove last trailing \n so we don't extend field length
		if (cs->GetLength()>=1) {
			*cs = cs->Mid(0, cs->GetLength()-1);
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::create_debug_tab :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::write_fdebug_log(bool write_header) {
#ifdef SEH_ENABLE
	try {
#endif
	
		int			N, i;
		CString		temp, line, header;
		char		format[50], nowtime[26];

		sprintf(format, "%%0.%df", m_precision);
		line.Format("%s,", get_time(nowtime));
		header.Format("date/time,");

		N = (int) debug_ar.GetSize();
		for (i=0; i<N; i++) {
			if (debug_ar[i].valid) {
				if (debug_ar[i].error == SUCCESS) {
					temp.Format(format, debug_ar[i].ret);
				}
				else {
					temp="";
				}
				line.Append(temp);
				header.Append(debug_ar[i].exp);
				if (i != N-1) {
					line.Append(",");
					header.Append(",");
				}
			}
		}

		// write the line to the log
		CString fn;
		fn.Format("%s\\f$debug%d.log", global.startup_path, global.sessionnum);
		FILE *fp = fopen(fn.GetString(), "a");
		if (write_header) {
			fprintf(fp, "%s\n", header.GetString());
		}
		fprintf(fp, "%s\n", line.GetString());
		fclose(fp);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::write_fdebug_log :\n"); 
		throw;
	}
#endif
}


void CDlgFormulaScintilla::init_debug_array(void) {
#ifdef SEH_ENABLE
	try {
#endif

		char				buf[1000], *eq;
		SDebugTabInfo		debug_struct;
		CString				Cstr, allText, strLine;
		int					stopchar, pos;
		CString				s;

		global.m_WaitCursor = true;
		BeginWaitCursor();
		//
		// Loop through each line in the debug tab and parse it
		//
		debug_ar.RemoveAll();

		m_pActiveScinCtrl->GetText(allText);
		pos=0;
		while (pos<allText.GetLength()) {
			// Get next line from entire buffer
			strLine = "";
			while (allText.Mid(pos, 2)!="\r\n" && allText.Mid(pos, 1)!="\r" && allText.Mid(pos, 1)!="\n" && pos<allText.GetLength()) {
				strLine += allText.Mid(pos, 1);
				pos++;
			}
			if (allText.Mid(pos, 2)=="\r\n") { pos += 2; }
			else if (allText.Mid(pos, 1)=="\r" || allText.Mid(pos, 1)=="\n") { pos += 1; }
		
			// Find the equal sign, if it exists
			strcpy(buf, strLine.GetString());
			if ((eq = strstr(buf, "=")) && buf[0]!='/' && buf[1]!='/') {
				eq++;  
				while (*eq==' ' && *eq!='\0') {
					eq++;
				}

				// parse
				debug_struct.exp.Format("%s", eq);
				parse(&debug_struct.exp, &debug_struct.tree, &stopchar);
				debug_struct.ret = 0;
				debug_struct.valid = true;
				debug_struct.error = SUCCESS;
			}
			else {
				debug_struct.exp = buf;
				Cstr = "";
				parse(&Cstr, &debug_struct.tree, &stopchar);
				debug_struct.ret = 0;
				debug_struct.valid = false;
				debug_struct.error = ERR_DEBUG_NOEQ;
			}
			debug_ar.Add(debug_struct);
		}

		EndWaitCursor();
		global.m_WaitCursor = false;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::init_debug_array :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnBnClickedApply() {
#ifdef SEH_ENABLE
	try {
#endif
		CMenu				*file_menu = this->GetMenu()->GetSubMenu(0);
		COpenHoldemDoc		*pDoc = COpenHoldemDoc::GetDocument();
		CMainFrame			*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

		// If autoplayer is engaged, dis-engage it
		if (global.autoplay) 
		{
			if (MessageBox("Autoplayer is currently enabled.\nWould you like to disable the autoplayer?",
						   "Disable Autoplayer?", MB_YESNO) == IDYES)
			{
				pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
				EnterCriticalSection(&cs_heartbeat);
				global.autoplay = false;
				LeaveCriticalSection(&cs_heartbeat);
			}
		}
	
		// Save settings to registry
		save_settings_to_registry();

		// Copy working set to global set
		LastChangeToFormula(&m_wrk_formula);
		global.CopyFormula(&m_wrk_formula, &global.formula);
		pDoc->SetModifiedFlag(true);

		// Re-calc hand lists
		global.create_hand_list_matrices(&m_wrk_formula);
		global.create_hand_list_matrices(&global.formula);

		// Re-parse
		global.ParseAllFormula(this->GetSafeHwnd(), &m_wrk_formula);
		global.ParseAllFormula(this->GetSafeHwnd(), &global.formula);

		// Re-calc symbols
		symbols.CalcSymbols();

		// Rewrite f$debug log header, if required
		m_wrote_fdebug_header = false;

		m_dirty = false;

		HandleEnables(true);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnBnClickedApply :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnBnClickedOk() {
#ifdef SEH_ENABLE
	try {
#endif
		COpenHoldemDoc		*pDoc = COpenHoldemDoc::GetDocument();
		CMainFrame			*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

		// If autoplayer is engaged, dis-engage it
		if (global.autoplay) 
		{
			if (MessageBox("Autoplayer is currently enabled.\nWould you like to disable the autoplayer?",
						   "Disable Autoplayer?", MB_YESNO) == IDYES)
			{
				pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
				EnterCriticalSection(&cs_heartbeat);
				global.autoplay = false;
				LeaveCriticalSection(&cs_heartbeat);
			}
		}
	
		StopAutoButton();

		// Copy working set to global set
		LastChangeToFormula(&m_wrk_formula);
		global.CopyFormula(&m_wrk_formula, &global.formula);
		global.ClearFormula(&m_wrk_formula);
		pDoc->SetModifiedFlag(true);
		m_dirty = false;

		// Re-calc hand lists
		global.create_hand_list_matrices(&global.formula);

		// Re-parse
		global.ParseAllFormula(this->GetSafeHwnd(), &global.formula);

		// Re-calc symbols
		symbols.CalcSymbols();

		// Save settings to registry
		save_settings_to_registry();

		// Uncheck formula button on main toolbar
		pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, false);

		OnOK();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnBnClickedOk :\n"); 
		throw;
	}
#endif
}

bool CDlgFormulaScintilla::PromptToSave()
{
	COpenHoldemDoc		*pDoc = COpenHoldemDoc::GetDocument();
	CMainFrame			*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	int ret = MessageBox("You have made changes to this formula.\n\nDo you want to apply changes?", "Save changes?", MB_YESNOCANCEL);
	if (ret == IDYES) {
		LastChangeToFormula(&m_wrk_formula);
		global.CopyFormula(&m_wrk_formula, &global.formula);
		global.ClearFormula(&m_wrk_formula);
		pDoc->SetModifiedFlag(true);

		// Re-calc hand lists
		global.create_hand_list_matrices(&global.formula);

		// Re-parse
		global.ParseAllFormula(this->GetSafeHwnd(), &global.formula);

		// Uncheck formula button on main toolbar
		pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, false);

		m_dirty = false;

		return true;
	}
	else if (ret == IDNO) {
		global.ClearFormula(&m_wrk_formula);

		// Uncheck formula button on main toolbar
		pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, false);

		m_dirty = false;

		return true;
	}
	return false;
}

void CDlgFormulaScintilla::OnBnClickedCancel() {
#ifdef SEH_ENABLE
	try {
#endif
		COpenHoldemDoc		*pDoc = COpenHoldemDoc::GetDocument();
		CMainFrame			*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

		StopAutoButton();

		save_settings_to_registry();

		// Prompt for changes if needed
		if (m_dirty) {
			if (PromptToSave()) {
				CloseFindReplaceDialog();
				OnCancel();
		}
		}
		else {
			global.ClearFormula(&m_wrk_formula);

			// Uncheck formula button on main toolbar
			pMyMainWnd->m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, false);
			CloseFindReplaceDialog();

			OnCancel();
		}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnBnClickedCancel :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnEditCut() {
#ifdef SEH_ENABLE
	try {
#endif
		m_pActiveScinCtrl->Cut();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnEditCut\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnEditCopy() {
#ifdef SEH_ENABLE
	try {
#endif
		m_pActiveScinCtrl->Copy();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnEditCopy\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnEditPaste() {
#ifdef SEH_ENABLE
	try {
#endif
		m_pActiveScinCtrl->Paste();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnEditPaste\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnEditDeleteText() {
#ifdef SEH_ENABLE
	try {
#endif
		m_pActiveScinCtrl->SendMessage(SCI_DELETEBACK, 0, 0);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnEditDeleteText\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnEditUndo() {
#ifdef SEH_ENABLE
	try {
#endif
		m_pActiveScinCtrl->Undo();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnEditUndo\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnEditRedo() {
#ifdef SEH_ENABLE
	try {
#endif
		m_pActiveScinCtrl->Redo();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnEditRedo\n"); 
		throw;
	}
#endif
}
void CDlgFormulaScintilla::OnEditSelectAll() {
#ifdef SEH_ENABLE
	try {
#endif
		m_pActiveScinCtrl->SelectAll();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnEditSelectAll\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnTimer(UINT nIDEvent) {
#ifdef SEH_ENABLE
	try {
#endif
		CMenu *edit_menu = this->GetMenu()->GetSubMenu(1);

		if (nIDEvent == MENU_UPDATE_TIMER) {
			HandleEnables(false);
		}
		// Update debug tab (if auto button is pressed)
		else if (nIDEvent == DEBUG_UPDATE_TIMER) 
		{
			EnterCriticalSection(&cs_updater);

			if (m_ButtonAuto.GetCheck() == 1 && global.update_in_process == false && m_current_edit == "f$debug" && ok_to_update_debug) {
				update_debug_auto();
			}

			LeaveCriticalSection(&cs_updater);
		}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnTimer :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::set_style_colors(CScintillaWnd *pWnd, bool enabled) {
#ifdef SEH_ENABLE
	try {
#endif
		int i;

		if (enabled) {
			pWnd->SetForeground(0, RGB(0x00, 0x00, 0x00));		// SCE_C_DEFAULT 0
			pWnd->SetForeground(1, RGB(0x00, 0x99, 0x00));		// SCE_C_COMMENT 1      (//)
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
			//pWnd->SetForeground(19, RGB(0x00, 0x00, 0x00));  // SCE_C_GLOBALCLASS 19
		}
		else {
			for (i=0; i<=MAX_STYLE_NUM; i++) {
				pWnd->SetForeground(i, RGB(0x00, 0x00, 0x00));
			}
		}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::set_style_colors :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnFormulaDebugLogfdebug() {
#ifdef SEH_ENABLE
	try {
#endif
	m_fdebuglog = !m_fdebuglog;

	HandleEnables(true);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnFormulaDebugLogfdebug :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnHelpWiki() 
{
#ifdef SEH_ENABLE
	try {
#endif

		ShellExecute(NULL, "open", "http://www.maxinmontreal.com/wiki/index.php5?title=Main_Page", "", "", SW_SHOWDEFAULT);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnHelpWiki :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnHelpForums()
{
#ifdef SEH_ENABLE
	try {
#endif

		ShellExecute(NULL, "open", "http://www.maxinmontreal.com/forums", "", "", SW_SHOWDEFAULT);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnHelpForums :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::OnFormulaDebugMyturn() {
#ifdef SEH_ENABLE
	try {
#endif
	m_fdebuglog_myturn = !m_fdebuglog_myturn;

	HandleEnables(true);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::OnFormulaDebugMyturn :\n"); 
		throw;
	}
#endif
}

BOOL CDlgFormulaScintilla::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (global.m_WaitCursor)
	{
		RestoreWaitCursor();
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CDlgFormulaScintilla::OnFormulaViewSortudf()
{
	m_udf_sort = !m_udf_sort;
	if (m_udf_sort)
		sort_udf_tree();

	HandleEnables(true);
}

void CDlgFormulaScintilla::sort_udf_tree()
{
	HTREEITEM	hItem = m_FormulaTree.GetChildItem(NULL);
	CString		text = "";

	if (hItem != NULL)
		text = m_FormulaTree.GetItemText(hItem);

	while (hItem != NULL && text != "User Defined Functions")
	{
		hItem = m_FormulaTree.GetNextItem(hItem, TVGN_NEXT);
		if (hItem != NULL)
			text = m_FormulaTree.GetItemText(hItem);
	}

	if (hItem != NULL && text == "User Defined Functions")
		m_FormulaTree.SortChildren(hItem);
}

void CDlgFormulaScintilla::save_settings_to_registry()
{
	Registry		reg;
	WINDOWPLACEMENT wp;
	HTREEITEM		hItem;
	CString			text = "";
	UINT			state;

	reg.read_reg();

	GetWindowPlacement(&wp);
	reg.formula_x = wp.rcNormalPosition.left;
	reg.formula_y = wp.rcNormalPosition.top;
	reg.formula_dx = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
	reg.formula_dy = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
	reg.precision = m_precision;
	reg.equal = m_equal;
	reg.fdebuglog = m_fdebuglog;
	reg.fdebuglog_myturn = m_fdebuglog_myturn;
	reg.udf_sort = m_udf_sort;

	// Tree expansion settings
	hItem = m_FormulaTree.GetChildItem(NULL);
	while (hItem != NULL)
	{
		text = m_FormulaTree.GetItemText(hItem);
		state = m_FormulaTree.GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED;

		if (text == "Standard Functions")
			reg.expand_std = state;
		else if (text == "Hand Lists") 
			reg.expand_list = state;
		else if (text == "User Defined Functions")
			reg.expand_udf = state;

		hItem = m_FormulaTree.GetNextItem(hItem, TVGN_NEXT);
		if (hItem != NULL)
			text = m_FormulaTree.GetItemText(hItem);
	}

	if (hItem != NULL && text == "User Defined Functions")
		m_FormulaTree.SortChildren(hItem);

	reg.write_reg();
}

void CDlgFormulaScintilla::HandleEnables(bool AllItems)
{
#ifdef SEH_ENABLE
	try {
#endif
	// These are what drive the enable/disable logic
	bool bFormulaVisible = true;
	bool bTreeHeadingSelected = false;
	bool bTreeValidLeafSelected = false;
	int  iWhichTypeSelected = -1; // 0=Standard, 1=Hand List, 2=User Defined Function, -1=Invalid
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
	bTreeValidLeafSelected = (!bTreeHeadingSelected && curSelected);
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
	if (headingText == "Standard Functions")			iWhichTypeSelected = 0;
	else if (headingText == "Hand Lists")				iWhichTypeSelected = 1;
	else if (headingText == "User Defined Functions")	iWhichTypeSelected = 2;

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
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::HandleEnables() :\n"); 
		throw;
	}
#endif
}


HTREEITEM CDlgFormulaScintilla::AddSymbolSubTitle(HTREEITEM parentItem, const char *title, const char *description)
{
#ifdef SEH_ENABLE
	try {
#endif
	HTREEITEM ret = m_SymbolTree.InsertItem(title, parentItem);

	m_SymbolTree.SetItemData(ret, (DWORD_PTR)description);

	return ret;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::AddSymbolSubTitle() :\n"); 
		throw;
	}
#endif
}

HTREEITEM CDlgFormulaScintilla::AddSymbolTitle(const char *title, const char *description, HTREEITEM parentItem)
{
#ifdef SEH_ENABLE
	try {
#endif
	HTREEITEM ret = m_SymbolTree.InsertItem(title, parentItem);

	m_SymbolTree.SetItemData(ret, (DWORD_PTR)description);
	m_SymbolTree.SetItemState(ret, TVIS_BOLD, TVIS_BOLD);

	return ret;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::AddSymbolTitle() :\n"); 
		throw;
	}
#endif
}

HTREEITEM CDlgFormulaScintilla::AddSymbol(HTREEITEM parentItem, const char *symbol, const char *description)
{
#ifdef SEH_ENABLE
	try {
#endif
	HTREEITEM ret  = m_SymbolTree.InsertItem(symbol, hRawItem);
	m_SymbolTree.SetItemData(ret, (DWORD_PTR)description);

	ret = m_SymbolTree.InsertItem(symbol, parentItem);
	m_SymbolTree.SetItemData(ret, (DWORD_PTR)description);

	return ret;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::AddSymbol() :\n"); 
		throw;
	}
#endif
}

void CDlgFormulaScintilla::PopulateSymbols()
{
#ifdef SEH_ENABLE
	try {
#endif
	if (m_SymbolTree.GetRootItem() != NULL)
		return;

	hRawItem = AddSymbolTitle("All Symbols", NULL, NULL);
	hCatItem = AddSymbolTitle("Categories", NULL, NULL);

	HTREEITEM mainParent, parent;
	mainParent = parent = AddSymbolTitle("General symbols", NULL, hCatItem);
	AddSymbol(parent, "ismanual", "true if you're in manual mode, false otherwise");
	AddSymbol(parent, "isppro", "true if you're connected to a ppro server");
	AddSymbol(parent, "site", "0=user/ppro 1=scraped");
	AddSymbol(parent, "nchairs", "the integer value for the Table Map symbol s$nchairs");
	AddSymbol(parent, "isbring", "true if winholdem is attached to a bring client window");
	AddSymbol(parent, "session", "the current logging instance (0-9)");
	AddSymbol(parent, "handnumber", "the site hand number if available");
	AddSymbol(parent, "version", "returns the version number of OpenHoldem that is currently running");

	mainParent = parent = AddSymbolTitle("Table Map symbols", NULL, hCatItem);
	AddSymbol(parent, "sitename$abc", "true if user defined string abc appears within the Table Map symbol s$sitename");
	AddSymbol(parent, "network$def", "true if user defined string def appears within the Table Map symbol s$network");
	AddSymbol(parent, "swagdelay", "autoplayer delay in milliseconds between swag keystrokes and button click");
	AddSymbol(parent, "allidelay", "autoplayer delay in milliseconds between alli slider jam and button click");
	AddSymbol(parent, "swagtextmethod", "the site interpretation for swag edit text (Table Map symbol) 1=f$srai 2=f$srai+call 3=f$srai+call+currentbet");
	AddSymbol(parent, "potmethod", "the site interpretation for the contents of c0pot0 (Table Map symbol) 1=common (default) 2=total");
	AddSymbol(parent, "activemethod", " 1=inactive unless pXactive returns true/active (default) 2=active unless pXactive returns false/inactive/out/away");

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
	AddSymbol(parent, "chairbit$abc", "player abc chairbit (1 << chair$abc); 0 if not found");

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

	mainParent = parent = AddSymbolTitle("Statistics", "Note that these symbols have not been created for use in OpenHoldem yet.   Please see the to-do thread in the OpenHoldem stickies section of the forum for more information.", hCatItem);
	AddSymbol(parent, "callror", "the statistical risk of ruin for call decision");
	AddSymbol(parent, "raisror", "the statistical risk of ruin for rais decision");
	AddSymbol(parent, "srairor", "the statistical risk of ruin for srai decision");
	AddSymbol(parent, "alliror", "the statistical risk of ruin for alli decision");
	AddSymbol(parent, "callmean", "the statistical mean for call decision");
	AddSymbol(parent, "raismean", "the statistical mean for rais decision");
	AddSymbol(parent, "sraimean", "the statistical mean for srai decision");
	AddSymbol(parent, "allimean", "the statistical mean for alli decision");
	AddSymbol(parent, "callvariance", "the statistical variance for call decision");
	AddSymbol(parent, "raisvariance", "the statistical variance for rais decision");
	AddSymbol(parent, "sraivariance", "the statistical variance for srai decision");
	AddSymbol(parent, "allivariance", "the statistical variance for alli decision");

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
	AddSymbol(parent, "f0 - f9", "true if flag 0 - flag 9 button is pressed false otherwise");
	AddSymbol(parent, "fbits", "flag button bits 9-0 - 1=pressed 0=notpressed");

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

	m_SymbolTree.SortChildren(hRawItem);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDlgFormulaScintilla::PopulateSymbols() :\n"); 
		throw;
	}
#endif
}
