// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <io.h>
#include <process.h>

#include "MainFrm.h"

#include "CAutoConnector.h"
#include "CAutoplayer.h"
#include "CDllExtension.h"
#include "CFormula.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "CPerl.hpp"
#include "CPokerPro.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CReplayFrame.h"
#include "CScraper.h"
#include "CSymbols.h"
#include "CTableLimits.h"
#include "..\CTransform\CTransform.h"
#include "CValidator.h"
#include "DialogChairNum.h"
#include "DialogFormulaScintilla.h"
#include "DialogLockBlinds.h"
#include "DialogPPro.h"
#include "DialogSAPrefs1.h"
#include "DialogSAPrefs2.h"
#include "DialogSAPrefs3.h"
#include "DialogSAPrefs4.h"
#include "DialogSAPrefs5.h"
#include "DialogSAPrefs6.h"
#include "DialogSAPrefs7.h"
#include "DialogSAPrefs8.h"
#include "DialogSAPrefs9.h"
#include "DialogSAPrefs10.h"
#include "DialogSAPrefs11.h"
#include "DialogSAPrefs12.h"
#include "DialogSAPrefs13.h"
#include "DialogSAPrefs14.h"
#include "DialogSAPrefs15.h"
#include "DialogSAPrefs16.h"
#include "DialogSAPrefs17.h"
#include "DialogScraperOutput.h"
#include "inlines/eval.h"
#include "MagicNumbers.h"
#include "OpenHoldem.h"
#include "OpenHoldemDoc.h"
#include "SAPrefsDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()

	// Menu updates
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, &CMainFrame::OnUpdateMenuFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CMainFrame::OnUpdateMenuFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOADTABLEMAP, &CMainFrame::OnUpdateMenuFileLoadProfile)
	ON_UPDATE_COMMAND_UI(ID_FILE_CONNECT, &CMainFrame::OnUpdateFileConnect)
	ON_UPDATE_COMMAND_UI(ID_FILE_DISCONNECT, &CMainFrame::OnUpdateFileDisconnect)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FORCEUSERCHAIR, &CMainFrame::OnUpdateEditForceuserchair)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAIN_TOOLBAR, &CMainFrame::OnUpdateViewMainToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FLAGS_TOOLBAR, &CMainFrame::OnUpdateViewFlagsToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUSBAR, &CMainFrame::OnUpdateViewStatusbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOOTREPLAYFRAME, &CMainFrame::OnUpdateViewShootreplayframe)
	ON_UPDATE_COMMAND_UI(ID_DLL_LOAD, &CMainFrame::OnUpdateMenuDllLoad)
	ON_UPDATE_COMMAND_UI(ID_DLL_LOADSPECIFICFILE, &CMainFrame::OnUpdateDllLoadspecificfile)
	ON_UPDATE_COMMAND_UI(ID_POKERPRO_CONNECT, &CMainFrame::OnUpdatePokerproConnect)
	ON_UPDATE_COMMAND_UI(ID_PERL_LOADFORMULA, &CMainFrame::OnUpdateMenuPerlLoad)
	ON_UPDATE_COMMAND_UI(ID_PERL_LOADSPECIFICFORMULA, &CMainFrame::OnUpdateMenuPerlLoadSpecificFormula)
	ON_UPDATE_COMMAND_UI(ID_PERL_RELOADFORMULA, &CMainFrame::OnUpdateMenuPerlReloadFormula)
	ON_UPDATE_COMMAND_UI(ID_PERL_CHECKSYNTAX, &CMainFrame::OnUpdateMenuPerlCheckSyntax)
	ON_UPDATE_COMMAND_UI(ID_PERL_EDITMAINFORMULA, &CMainFrame::OnUpdateMenuPerlEditMainFormula)

	//  Menu commands
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_FILE_LOADTABLEMAP, &CMainFrame::OnFileLoadTableMap)
	ON_COMMAND(ID_FILE_CONNECT, &CMainFrame::OnBnClickedGreenCircle)
	ON_COMMAND(ID_FILE_DISCONNECT, &CMainFrame::OnBnClickedRedCircle)
	ON_COMMAND(ID_EDIT_FORMULA, &CMainFrame::OnEditFormula)
	ON_COMMAND(ID_EDIT_PREFERENCES, &CMainFrame::OnEditPreferences)
	ON_COMMAND(ID_EDIT_FORCEUSERCHAIR, &CMainFrame::OnEditForceuserchair)
	ON_COMMAND(ID_VIEW_MAIN_TOOLBAR, &CMainFrame::OnFormulaViewMainToolbar)
	ON_COMMAND(ID_VIEW_FLAGS_TOOLBAR, &CMainFrame::OnFormulaViewFlagsToolbar)
	ON_COMMAND(ID_VIEW_STATUSBAR, &CMainFrame::OnFormulaViewStatusbar)
	ON_COMMAND(ID_VIEW_SCRAPEROUTPUT, &CMainFrame::OnScraperOutput)
	ON_COMMAND(ID_VIEW_SHOOTREPLAYFRAME, &CMainFrame::OnViewShootreplayframe)
	ON_COMMAND(ID_DLL_LOAD, &CMainFrame::OnDllLoad)
	ON_COMMAND(ID_DLL_LOADSPECIFICFILE, &CMainFrame::OnDllLoadspecificfile)
	ON_COMMAND(ID_PERL_LOADFORMULA, &CMainFrame::OnPerlLoadFormula)
	ON_COMMAND(ID_PERL_LOADSPECIFICFORMULA, &CMainFrame::OnPerlLoadSpecificFormula)
	ON_COMMAND(ID_PERL_RELOADFORMULA, &CMainFrame::OnPerlReloadFormula)
	ON_COMMAND(ID_PERL_CHECKSYNTAX, &CMainFrame::OnPerlCheckSyntax)
	ON_COMMAND(ID_PERL_EDITMAINFORMULA, &CMainFrame::OnPerlEditMainFormula)
	ON_COMMAND(ID_HELP_HELP, &CMainFrame::OnHelp)
	ON_COMMAND(ID_HELP_DOCUMENTATIONWIKI, &CMainFrame::OnHelpWiki)
	ON_COMMAND(ID_HELP_FORUMS, &CMainFrame::OnHelpForums)

	// Main toolbar
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_GREENCIRCLE, &CMainFrame::OnBnClickedGreenCircle)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_REDCIRCLE, &CMainFrame::OnBnClickedRedCircle)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_AUTOPLAYER, &CMainFrame::OnAutoplayer)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_FORMULA, &CMainFrame::OnEditFormula)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_VALIDATOR, &CMainFrame::OnValidator)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_MINMAX, &CMainFrame::OnMinMax)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_ATTACH_TOP, &CMainFrame::OnAttachTop)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, &CMainFrame::OnAttachBottom)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_LOCK_BLINDS, &CMainFrame::OnLockBlinds)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, &CMainFrame::OnScraperOutput)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_SHOOTFRAME, &CMainFrame::OnViewShootreplayframe)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_HELP, &CMainFrame::OnHelp)

	// Flags toolbar
	ON_BN_CLICKED(ID_NUMBER0, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER1, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER2, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER3, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER4, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER5, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER6, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER7, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER8, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER9, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER10, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER11, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER12, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER13, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER14, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER15, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER16, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER17, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER18, &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER19, &CMainFrame::OnClickedFlags)

	ON_WM_TIMER()

	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_PLCARDS,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_COMCARDS,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_PRWIN,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_NOPP,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_NIT,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_ACTION,OnUpdateStatus)

	ON_COMMAND(ID_POKERPRO_CONNECT, &CMainFrame::OnPokerproConnect)
	ON_MESSAGE(WMA_SETWINDOWTEXT, OnSetWindowText)
	ON_MESSAGE(WMA_DOCONNECT, &CMainFrame::OnConnectMessage)
	ON_MESSAGE(WMA_DODISCONNECT, &CMainFrame::OnDisconnectMessage)
    ON_MESSAGE(WMA_CONNECTEDHWND, &CMainFrame::OnConnectedHwndMessage)

	ON_WM_SETCURSOR()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

static UINT indicators[] = 
{
	ID_SEPARATOR,		   // status line indicator
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
	ID_INDICATOR_STATUS_PLCARDS,
	ID_INDICATOR_STATUS_COMCARDS,
	ID_INDICATOR_STATUS_POKERHAND,
	ID_INDICATOR_STATUS_HANDRANK,
	ID_INDICATOR_STATUS_PRWIN,
	ID_INDICATOR_STATUS_NOPP,
	ID_INDICATOR_STATUS_NIT,
	ID_INDICATOR_STATUS_ACTION,
};

// CMainFrame construction/destruction
CMainFrame::CMainFrame() 
{
	_autoplay_pressed = false;
	_wait_cursor = false;

	for (int i=0; i<=19; i++)
		_flags[i] = false;

	_prev_att_rect.bottom = 0;
	_prev_att_rect.left = 0;
	_prev_att_rect.right = 0;
	_prev_att_rect.top = 0;
	_prev_wrect.bottom = 0;
	_prev_wrect.left = 0;
	_prev_wrect.right = 0;
	_prev_wrect.top = 0;
}

CMainFrame::~CMainFrame() 
{
}

void CMainFrame::DisableButtonsOnConnect()
{
	// Disable buttons, menu items
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, false);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, false);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_GREENCIRCLE, false);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_REDCIRCLE, true);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, true);
}

void CMainFrame::EnableButtonsOnDisconnect()
{
	// Reset toolbar/menu button state
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_GREENCIRCLE, true);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_REDCIRCLE, false);
	m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, true);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, true);
}

void CMainFrame::UnattachOHFromPokerWindow()
{
	// Unattach OH from any poker window
	if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_TOP))
		m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_ATTACH_TOP, false);
	
	if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_BOTTOM))
		m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, false);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CString			t = "";
	lpCreateStruct->dwExStyle |= WS_MINIMIZE;
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create toolbars
	EnableDocking(CBRS_ALIGN_ANY);
	CreateMainToolbar();
	CreateFlagsToolbar();
	AlignToolbars();

	// Status bar
	CreateStatusBar();

	// Set toolbar button status
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_GREENCIRCLE, true);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_REDCIRCLE, false);

	// Start timer that checks if we should enable buttons
	SetTimer(ENABLE_BUTTONS_TIMER, 50, 0);

	// Start timer that updates status bar
	SetTimer(UPDATE_STATUS_BAR_TIMER, 50, 0);

	// Start timer that attaches the OH window when the poker window moves
	SetTimer(ATTACH_WINDOW_TIMER, 20, 0);

	if (prefs.simple_window_title())
		SetMainWindowTitle("");

	return 0;
}

int CMainFrame::CreateMainToolbar(void) 
{
	TBBUTTONINFO	tbi;

	tbi.cbSize = sizeof(TBBUTTONINFO);
	tbi.dwMask = TBIF_STYLE;
	tbi.fsStyle = TBSTYLE_CHECK;

	// Main toolbar
	m_MainToolBar.CreateEx(this, NULL, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
						   CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_MainToolBar.LoadToolBar(IDR_MAINFRAME);
	m_MainToolBar.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_MainToolBar);

	// Make formula button sticky
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_FORMULA, &tbi);

	// Make validator button sticky
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_VALIDATOR, &tbi);

	// Make scraper output button sticky
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, &tbi);

	// Make autoplayer button sticky, and start it out disabled
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_AUTOPLAYER, &tbi);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);

	// Make minmax and attach buttons sticky and start disabled
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_MINMAX, &tbi);
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_ATTACH_TOP, &tbi);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_ATTACH_TOP, false);
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, &tbi);
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, false);

	// Make lock blinds button sticky
	m_MainToolBar.GetToolBarCtrl().SetButtonInfo(ID_MAIN_TOOLBAR_LOCK_BLINDS, &tbi);

	// Start shoot replay frame button disabled
	m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, false);

	// Title of floating main toolbar
	m_MainToolBar.SetWindowText("Main");

	return 0;
}

int CMainFrame::CreateFlagsToolbar(void) 
{
	TBBUTTONINFO	tbi;

	tbi.cbSize = sizeof(TBBUTTONINFO);
	tbi.dwMask = TBIF_STYLE;
	tbi.fsStyle = TBSTYLE_CHECK;

	// Flags toolbar
	_tool_bar.CreateEx(this, NULL, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
							CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	_tool_bar.LoadToolBar(IDR_FLAGS);

	// Make flags toolbar buttons sticky
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER0, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER1, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER2, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER3, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER4, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER5, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER6, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER7, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER8, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER9, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER10, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER11, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER12, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER13, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER14, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER15, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER16, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER17, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER18, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER19, &tbi);
	_tool_bar.EnableDocking(CBRS_ALIGN_ANY);
	_tool_bar.EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&_tool_bar);

	// Title of floating flags toolbar
	_tool_bar.SetWindowText("Flags");

	return 0;
}


void CMainFrame::AlignToolbars(void) 
{
	// Put the main toolbar and flags toolbar on the same line
	CRect rectBar1, rectBar2;

	RecalcLayout();

	m_MainToolBar.GetWindowRect(rectBar1);
	_tool_bar.GetWindowRect(rectBar2);

	DockControlBar(&_tool_bar, AFX_IDW_DOCKBAR_TOP, rectBar1); //will be first
	UINT uiBarWidth = rectBar2.Width();
	rectBar2.left = rectBar1.right;
	rectBar2.top = rectBar1.top;
	rectBar2.bottom = rectBar1.bottom;
	rectBar2.right = rectBar1.right + uiBarWidth;

	DockControlBar(&_tool_bar, AFX_IDW_DOCKBAR_TOP, rectBar2); //will be second

	RecalcLayout();
}

int CMainFrame::CreateStatusBar(void) 
{
	if (!_status_bar.Create(this) || !_status_bar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))	
	{
		TRACE0("Failed to create status bar\n");
		return -1;	  // fail to create
	}
	_status_bar.SetPaneInfo(0, ID_SEPARATOR, SBPS_STRETCH | SBPS_NOBORDERS, 0);
	_status_bar.SetPaneInfo(1, ID_INDICATOR_STATUS_PLCARDS, NULL, 30);
	_status_bar.SetPaneInfo(2, ID_INDICATOR_STATUS_COMCARDS, NULL, 75);
	_status_bar.SetPaneInfo(3, ID_INDICATOR_STATUS_POKERHAND, NULL, 65);
	_status_bar.SetPaneInfo(4, ID_INDICATOR_STATUS_HANDRANK, NULL, 55);
	_status_bar.SetPaneInfo(5, ID_INDICATOR_STATUS_PRWIN, NULL, 62);
	_status_bar.SetPaneInfo(6, ID_INDICATOR_STATUS_NOPP, NULL, 15);
	_status_bar.SetPaneInfo(7, ID_INDICATOR_STATUS_NIT, NULL, 90);
	_status_bar.SetPaneInfo(8, ID_INDICATOR_STATUS_ACTION, NULL, 70);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	if ( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	int			max_x = 0, max_y = 0;

	WNDCLASS wnd;
	HINSTANCE hInst = AfxGetInstanceHandle();

	// Set class name
	if (!(::GetClassInfo(hInst, prefs.window_class_name(), &wnd)))
	{
		wnd.style			= CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wnd.lpfnWndProc	  = ::DefWindowProc;
		wnd.cbClsExtra	   = wnd.cbWndExtra = 0;
		wnd.hInstance		= hInst;
		wnd.hIcon			= AfxGetApp()->LoadIcon(IDI_ICON1);
		wnd.hCursor		  = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wnd.hbrBackground	= (HBRUSH) (COLOR_3DFACE + 1);
		wnd.lpszMenuName	 = NULL;
		wnd.lpszClassName	= prefs.window_class_name();

		AfxRegisterClass( &wnd );
	}
	cs.lpszClass = prefs.window_class_name();

	// Restore window location and size
	max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
	max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
	cs.x = min(prefs.main_x(), max_x);
	cs.y = min(prefs.main_y(), max_y);
	cs.cx = prefs.main_dx();
	cs.cy = prefs.main_dy();

	return true;
}

// CMainFrame message handlers

void CMainFrame::OnEditFormula() 
{
	if (m_formulaScintillaDlg) 
	{
		BOOL	bWasShown = ::IsWindow(m_formulaScintillaDlg->m_hWnd) && m_formulaScintillaDlg->IsWindowVisible();

		m_formulaScintillaDlg->EndDialog(IDCANCEL);
		delete m_formulaScintillaDlg;
		m_formulaScintillaDlg = NULL;

		if (bWasShown)
			return;
	}

	m_formulaScintillaDlg = new CDlgFormulaScintilla(this);
	m_formulaScintillaDlg->Create(CDlgFormulaScintilla::IDD,this);
	m_formulaScintillaDlg->ShowWindow(SW_SHOW);
	m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, true);
}

void CMainFrame::OnScraperOutput() 
{
	if (m_ScraperOutputDlg) 
	{
		BOOL	bWasShown = ::IsWindow(m_ScraperOutputDlg->m_hWnd) && m_ScraperOutputDlg->IsWindowVisible();

		m_ScraperOutputDlg->DestroyWindow();
		delete m_ScraperOutputDlg;
		m_ScraperOutputDlg = NULL;

		if (bWasShown)
			return;
	}

	m_ScraperOutputDlg = new CDlgScraperOutput(this);
	m_ScraperOutputDlg->Create(CDlgScraperOutput::IDD,this);
	m_ScraperOutputDlg->ShowWindow(SW_SHOW);
	m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, true);
}

void CMainFrame::OnViewShootreplayframe()
{
	CReplayFrame	crf;
	crf.CreateReplayFrame();
}

void CMainFrame::OnEditPreferences() 
{
	//CDlgPreferences  myDialog;
	CSAPrefsDialog dlg;

	// the "pages" (all derived from CSAPrefsSubDlg)
	CDlgSAPrefs1 page1;
	CDlgSAPrefs2 page2;
	CDlgSAPrefs3 page3;
	CDlgSAPrefs4 page4;
	CDlgSAPrefs5 page5;
	CDlgSAPrefs6 page6;
	CDlgSAPrefs7 page7;
	CDlgSAPrefs8 page8;
	CDlgSAPrefs9 page9;
	CDlgSAPrefs10 page10;
	CDlgSAPrefs11 page11;
	CDlgSAPrefs12 page12;
	CDlgSAPrefs13 page13;
	CDlgSAPrefs14 page14;
	CDlgSAPrefs15 page15;
	CDlgSAPrefs16 page16;
	CDlgSAPrefs17 page17;

	// add pages
	dlg.AddPage(page1,  "Analyzer");
	dlg.AddPage(page14, "Auto-Connector");
	dlg.AddPage(page2,  "Autoplayer");
	dlg.AddPage(page10, "Chat");
	dlg.AddPage(page17, "Configuration Check");
	dlg.AddPage(page3,  "DLL Extension");
	dlg.AddPage(page15, "GUI");
	dlg.AddPage(page7,  "ICM");
	dlg.AddPage(page11, "Logging");
	dlg.AddPage(page13, "Obscure");
	dlg.AddPage(page9,  "Perl");
	dlg.AddPage(page6,  "Poker Tracker");
	dlg.AddPage(page16, "Rebuy");
	dlg.AddPage(page8,  "Replay Frames");
	dlg.AddPage(page4,  "Scraper");
	dlg.AddPage(page5,  "Symbols");
	dlg.AddPage(page12, "Validator");	

	// this one will be a child node on the tree
	// (&page3 specifies the parent)
	//dlg.AddPage(dlg4, "Page 4", &page3);

	// the prefs dialog title
	dlg.SetTitle("OpenHoldem Preferences");

	// text drawn on the right side of the shaded
	// page label. this does not change when the
	// pages change, hence "constant".
	dlg.SetConstantText("Preferences");

	// launch it like any other dialog...
	//dlg1.m_csText = m_csStupidCString;
	if (dlg.DoModal()==IDOK)
	{
		//m_csStupidCString = dlg1.m_csText;
	}
}

BOOL CMainFrame::DestroyWindow() 
{
	WINDOWPLACEMENT wp;

	//unload dll
	p_dll_extension->UnloadDll();

	// stop threads
	if (p_iterator_thread) 
	{
		delete p_iterator_thread;
		p_iterator_thread = NULL;
	}
	if (p_heartbeat_thread)
	{
		delete p_heartbeat_thread;
		p_heartbeat_thread = NULL;
	}

	if (p_pokertracker_thread)
	{
		delete p_pokertracker_thread;
		p_pokertracker_thread = NULL;
	}

	// Save window position
	if (!m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_MINMAX)) 
	{
		GetWindowPlacement(&wp);

		prefs.set_main_x(wp.rcNormalPosition.left);
		prefs.set_main_y(wp.rcNormalPosition.top);
		prefs.set_main_dx(wp.rcNormalPosition.right - wp.rcNormalPosition.left);
		prefs.set_main_dy(wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);
	}
	else 
	{
		prefs.set_main_x(_table_view_size.left);
		prefs.set_main_y(_table_view_size.top);
		prefs.set_main_dx(_table_view_size.right - _table_view_size.left);
		prefs.set_main_dy(_table_view_size.bottom - _table_view_size.top);
	}

	return CFrameWnd::DestroyWindow();
}

void CMainFrame::SetMainWindowTitle(LPCSTR title)
{
	if (prefs.simple_window_title()) {
		if (_exec_filename.IsEmpty()) {
			char exec[_MAX_PATH];
			GetModuleFileName(AfxGetInstanceHandle(), exec, _MAX_PATH);
			_exec_filename = exec;
			int index = _exec_filename.ReverseFind('\\');
			if (index >= 0)
				_exec_filename = _exec_filename.Mid(index+1);
			index = _exec_filename.ReverseFind('.');
			if (index > 0)
				_exec_filename = _exec_filename.Left(index);
		}
		SetWindowText(_exec_filename);
	} else {
		SetWindowText(title);
	}
}

void CMainFrame::OnFileOpen() 
{
	CFileDialog			cfd(true);

	cfd.m_ofn.lpstrInitialDir = prefs.path_ohf();
	cfd.m_ofn.lpstrFilter = "OpenHoldem Files (.ohf)\0*.ohf\0WinHoldem Files (.whf)\0*.whf\0All files (*.*)\0*.*\0\0";
	cfd.m_ofn.lpstrTitle = "Select Formula file to OPEN";
	if (cfd.DoModal() == IDOK)
	{			
		COpenHoldemDoc *pDoc = (COpenHoldemDoc *)this->GetActiveDocument();		
		pDoc->OnOpenDocument(cfd.GetPathName());
		pDoc->SetPathName(cfd.GetPathName());
		// Update window title, registry
		SetMainWindowTitle(cfd.GetFileTitle() + " - " + CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)));
		prefs.set_path_ohf(cfd.GetPathName());
	}
}

void CMainFrame::OnFileLoadTableMap() 
{
	CFileDialog			cfd(true);
	int					line = 0, ret = 0;
	CString				e = "";

	cfd.m_ofn.lpstrInitialDir = prefs.path_tm();
	cfd.m_ofn.lpstrFilter = "OpenScrape Table Maps (.tm)\0*.tm\0All files (*.*)\0*.*\0\0";
	cfd.m_ofn.lpstrTitle = "Select OpenScrape table map to OPEN";

	if (cfd.DoModal() == IDOK)
	{
		CString loaded_version;
		ret = p_tablemap->LoadTablemap(cfd.m_ofn.lpstrFile, VER_OPENSCRAPE_2, false, &line, prefs.disable_msgbox(), &loaded_version);
		
		if (loaded_version == VER_OPENHOLDEM_1 && ret == ERR_VERSION && !prefs.disable_msgbox())
		{
			e.Format("This is an OpenHoldem v1 profile (.ohdb1).\n"
					 "OpenHoldem versions 1.2.0 and higher require v2 Profiles (.ohdb2),\n"
					 "or OpenScrape Table Maps.\n");
			MessageBox(e, "Table map load error", MB_OK);
		}

		else if ( (loaded_version == VER_OPENSCRAPE_1 || loaded_version == VER_OPENHOLDEM_2) 
				  && !prefs.disable_msgbox())
		{
			MessageBox("This is a version 1 table map.\n\n"\
					   "Version 2.0.0 and higher of OpenHoldem use a new format (version 2).  This\n"\
					   "table map has been loaded, but it is highly unlikely to work correctly until\n"\
					   "it has been opened in OpenScrape version 2.0.0 or higher, and adjustments\n"\
					   "have been made to autoplayer settings and region sizes.\n\n"\
					   "Please do not use this table map prior to updating it to version 2 in\n"\
					   "OpenScrape or you run the very serious risk of costly mis-scrapes.",
					   "Table map load warning", MB_OK | MB_ICONEXCLAMATION);		
		}

		else if (ret != SUCCESS && !prefs.disable_msgbox())
		{
			e.Format("Error code: %d  line: %d", ret, line);
			MessageBox(e, "Table map load error", MB_OK);
		}
		
		if (ret == SUCCESS)
		{
			// Reset "ScraperOutput" dialog, if it is live
			if (m_ScraperOutputDlg) 
			{
				m_ScraperOutputDlg->AddListboxItems();
				m_ScraperOutputDlg->UpdateDisplay();
			}

			prefs.set_path_tm(cfd.GetPathName());
		}
	}
}

void CMainFrame::OnBnClickedGreenCircle() 
{
	p_autoconnector->Connect(NULL);
}

void CMainFrame::OnBnClickedRedCircle() 
{
	p_autoconnector->Disconnect();
	_autoplay_pressed = false;
}

void CMainFrame::OnEditForceuserchair()
{
	CDlgChairNum	dlg;

	if (dlg.DoModal()==IDOK)
	{
		p_symbols->set_sym_chair(dlg.chair);
		p_symbols->set_sym_userchair(dlg.chair);
		p_symbols->set_user_chair_confirmed(true); 
		time_t tm;
		time(&tm);
		write_log(3, "Force set userchair to %d\n", dlg.chair);
	}
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	CardMask		Cards;
	int				nCards = 0;
	HandVal			hv = 0;
	char			hvstring[100] = {0};
	int				i = 0;
	char			*card = NULL;
	CString			temp = "";
	bool			playing = false;
	RECT			att_rect = {0}, wrect = {0};

	if (nIDEvent == HWND_CHECK_TIMER)
	{
		if (!IsWindow(p_autoconnector->attached_hwnd()))
		{
			// Table disappeared
			OnBnClickedRedCircle();
		}
	}

	else if (nIDEvent == ENABLE_BUTTONS_TIMER)
	{

		// Autoplayer
		if ((p_symbols->user_chair_confirmed() && m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE)) ||
				p_pokerpro->ppdata()->m_userchair!=-1)
		{
			m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, true);
		}
		else
		{
			m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
		}

		// Automatically start autoplayer, if set in preferences
		if (prefs.ap_auto() && !m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_AUTOPLAYER) &&
				((p_symbols->user_chair_confirmed() && m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE)) ||
				 p_pokerpro->ppdata()->m_userchair!=-1))
		{
			if (!_autoplay_pressed)
			{
				m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, true);
				OnAutoplayer();
				_autoplay_pressed = true;
			}
		}

		// attach
		if (p_autoconnector->attached_hwnd() != NULL) 
		{
			m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_ATTACH_TOP, true);
			m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, true);
		}
		else 
		{
			m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_ATTACH_TOP, false);
			m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, false);
		}

		// Shoot replay frame
		if (p_autoconnector->attached_hwnd() != NULL)
			m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, true);
		else
			m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, false);

	}

	else if (nIDEvent == UPDATE_STATUS_BAR_TIMER) 
	{
		// Figure out if I am "notplaying"
		int sym_userchair = (int) p_symbols->sym()->userchair;

		if (p_scraper->card_player(sym_userchair, 0) == CARD_BACK || 
			p_scraper->card_player(sym_userchair, 0) == CARD_NOCARD || 
			p_scraper->card_player(sym_userchair, 1) == CARD_BACK || 
			p_scraper->card_player(sym_userchair, 1) == CARD_NOCARD)
		{
			p_symbols->set_sym_playing(false); 
			playing = false; 
		}
		else
		{
			p_symbols->set_sym_playing(true); 
			playing = true;
		}

		// Player cards
		CardMask_RESET(Cards);
		nCards=0;
		_status_plcards = "";
		if (p_symbols->user_chair_confirmed() && playing) 
		{
			for (i=0; i<=1; i++) 
			{
				// player cards
				if (p_scraper->card_player(sym_userchair, i) != CARD_BACK && 
					p_scraper->card_player(sym_userchair, i) != CARD_NOCARD) 
				{
					card = StdDeck_cardString(p_scraper->card_player(sym_userchair, i));
					temp.Format("%s ", card);
					_status_plcards.Append(temp);
					CardMask_SET(Cards, p_scraper->card_player(sym_userchair, i));
					nCards++;
				}
			}
		}
		else 
		{
			for (i=0; i<=1; i++) 
			{
				if (p_scraper->card_player_for_display(i) != CARD_BACK && 
					p_scraper->card_player_for_display(i) != CARD_NOCARD) 
				{
					card = StdDeck_cardString(p_scraper->card_player_for_display(i));
					temp.Format("%s ", card);
					_status_plcards.Append(temp);
					CardMask_SET(Cards, p_scraper->card_player_for_display(i));
					nCards++;
				}
			}
		}

		// Common cards
		_status_comcards = "";
		for (i=0; i<=4; i++) 
		{
			if (p_scraper->card_common(i) != CARD_BACK && 
				p_scraper->card_common(i) != CARD_NOCARD) 
			{
				card = StdDeck_cardString(p_scraper->card_common(i));
				temp.Format("%s ", card);
				_status_comcards.Append(temp);
				CardMask_SET(Cards, p_scraper->card_common(i));
				nCards++;
			}
		}

		// poker hand
		hv = Hand_EVAL_N(Cards, nCards);
		HandVal_toString(hv, hvstring);
		_status_pokerhand = hvstring;
		_status_pokerhand = _status_pokerhand.Mid(0, _status_pokerhand.Find(" "));

		// handrank
		if (prefs.sym_handrank_value() == "169")
			_status_handrank.Format("%.0f/169", p_symbols->sym()->handrank169);

		else if (prefs.sym_handrank_value() == "1000")
			_status_handrank.Format("%.0f/1000", p_symbols->sym()->handrank1000);

		else if (prefs.sym_handrank_value() == "1326")
			_status_handrank.Format("%.0f/1326", p_symbols->sym()->handrank1326);

		else if (prefs.sym_handrank_value() == "2652")
			_status_handrank.Format("%.0f/2652", p_symbols->sym()->handrank2652);

		else if (prefs.sym_handrank_value() == "p")
			_status_handrank.Format("%.2f/2652", p_symbols->sym()->handrankp);

		// nopponents
		if (playing)
			_status_nopp.Format("%d", (int) p_symbols->sym()->nopponents);

		else
			_status_nopp = "";

		// Always update prwin/nit
		if (p_symbols->user_chair_confirmed() && playing)
		{
			_status_prwin.Format("%d/%d/%d", 
				(int) (iter_vars.prwin()*1000), 
				(int) (iter_vars.prtie()*1000),
				(int) (iter_vars.prlos()*1000));
			_status_nit.Format("%d/%d", 
				iter_vars.iterator_thread_progress(),
				(int) p_formula->formula()->dNit);
		}
		else
		{
			_status_prwin = "0/0/0";
			_status_nit.Format("0/%d", (int) p_formula->formula()->dNit);
		}

		// action
		if (!p_symbols->user_chair_confirmed() || !playing)
			_status_action = "Notplaying";

		else if (p_symbols->f$prefold())
		{
			_status_action = "Pre-fold";
		}

		else if (p_symbols->user_chair_confirmed() && iter_vars.iterator_thread_complete())
		{
			if (p_symbols->f$alli())  _status_action = "Allin";
			else if (p_symbols->f$swag())  _status_action.Format("SWAG: %.2f", p_symbols->f$swag());
			else if (p_symbols->f$rais())  _status_action = "Bet/Raise";
			else if (p_symbols->f$call())  _status_action = "Call/Check";
			else  _status_action = "Fold/Check";
		}

		else if (p_symbols->sym()->nopponents==0)
			_status_action = "Idle (f$P==0)";

		else
			_status_action = "Thinking";
	}

	else if (nIDEvent == ATTACH_WINDOW_TIMER)
	{
		::GetWindowRect(p_autoconnector->attached_hwnd(), &att_rect);
		GetWindowRect(&wrect);

		if (memcmp(&_prev_att_rect, &att_rect, sizeof(RECT))!=0 ||
				memcmp(&_prev_wrect, &wrect, sizeof(RECT))!=0)
		{
			::GetWindowRect(p_autoconnector->attached_hwnd(), &_prev_att_rect);
			GetWindowRect(&_prev_wrect);

			if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_TOP))
			{
				if (IsWindow(p_autoconnector->attached_hwnd()))
					MoveWindow(att_rect.left, att_rect.top-(wrect.bottom-wrect.top), att_rect.right-att_rect.left, wrect.bottom-wrect.top);
			}

			if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_BOTTOM))
			{
				if (IsWindow(p_autoconnector->attached_hwnd()))
					MoveWindow(att_rect.left, att_rect.bottom, att_rect.right-att_rect.left, wrect.bottom-wrect.top);
			}
		}
	}
}

void CMainFrame::OnClickedFlags() 
{
	set_flags(0, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER0));
	set_flags(1, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER1));
	set_flags(2, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER2));
	set_flags(3, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER3));
	set_flags(4, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER4));
	set_flags(5, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER5));
	set_flags(6, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER6));
	set_flags(7, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER7));
	set_flags(8, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER8));
	set_flags(9, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER9));
	set_flags(10, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER10));
	set_flags(11, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER11));
	set_flags(12, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER12));
	set_flags(13, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER13));
	set_flags(14, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER14));
	set_flags(15, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER15));
	set_flags(16, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER16));
	set_flags(17, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER17));
	set_flags(18, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER18));
	set_flags(19, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER19));

	p_symbols->CalcSymbols();
}


void CMainFrame::OnAutoplayer() 
{
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

	if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_AUTOPLAYER)) 
	{
		// calc hand lists
		p_formula->CreateHandListMatrices();

		// one last parse - do not engage if parse fails
		if (p_formula->ParseAllFormula(pMyMainWnd->GetSafeHwnd(), prefs.disable_msgbox()))
		{
			p_autoplayer->set_autoplayer_engaged(true);
		}
		else 
		{
			p_autoplayer->set_autoplayer_engaged(false);
			m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
		}
	}
	else 
	{
		p_autoplayer->set_autoplayer_engaged(false);
	}
}

void CMainFrame::OnValidator() 
{
	if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_VALIDATOR)) 
	{
		m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_VALIDATOR, true);
		p_validator->SetEnabledManually(true);
	}
	else
	{
		m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_VALIDATOR, false);
		p_validator->SetEnabledManually(false);
	}
}

void CMainFrame::OnUpdateStatus(CCmdUI *pCmdUI) 
{
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_PLCARDS), _status_plcards);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_COMCARDS), _status_comcards);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_POKERHAND), _status_pokerhand);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_HANDRANK), _status_handrank);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_PRWIN), _status_prwin);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_NOPP), _status_nopp);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_NIT), _status_nit);
	_status_bar.SetPaneText(_status_bar.CommandToIndex(ID_INDICATOR_STATUS_ACTION), _status_action);
	// if action==bet ... _status_bar.GetStatusBarCtrl().SetBkColor(RGB(180,180,180));
}

void CMainFrame::OnDllLoad() 
{
	if (p_dll_extension->IsDllLoaded())
		p_dll_extension->UnloadDll();
	else
		p_dll_extension->LoadDll("");
}

void CMainFrame::OnDllLoadspecificfile()
{
	CFileDialog			cfd(true);

	cfd.m_ofn.lpstrInitialDir = prefs.path_dll();
	cfd.m_ofn.lpstrFilter = "DLL Files (.dll)\0*.dll\0\0";
	cfd.m_ofn.lpstrTitle = "Select OpenHoldem DLL file to OPEN";

	if (cfd.DoModal() == IDOK) 
	{
		p_dll_extension->UnloadDll();

		p_dll_extension->LoadDll(cfd.m_ofn.lpstrFile);

		prefs.set_path_dll(cfd.GetPathName());
	}
}

LRESULT CMainFrame::OnConnectMessage(WPARAM, LPARAM hwnd)
{
	return p_autoconnector->Connect((HWND)hwnd);
}

LRESULT CMainFrame::OnDisconnectMessage(WPARAM, LPARAM)
{
	p_autoconnector->Disconnect();
	return true;
}

LRESULT CMainFrame::OnConnectedHwndMessage(WPARAM, LPARAM)
{
	return (LRESULT) p_autoconnector->attached_hwnd();
}

LRESULT CMainFrame::OnSetWindowText(WPARAM, LPARAM title)
{
	if (title) {
		CString *sTitle = (CString *)title;

		SetMainWindowTitle(sTitle->GetString());
		delete sTitle;
	} else {
		SetMainWindowTitle("");
	}
	return 0l;
}

void CMainFrame::OnPokerproConnect() 
{
	if (m_pproDlg) 
	{
		BOOL	bWasShown	=	::IsWindow(m_pproDlg->m_hWnd) && m_pproDlg->IsWindowVisible();

		m_pproDlg->DestroyWindow();
		delete m_pproDlg;
		m_pproDlg = NULL;

		if (bWasShown)
			return;
	}
	m_pproDlg = new CDlgPpro(this);
	m_pproDlg->Create(CDlgPpro::IDD,this);
	m_pproDlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnMinMax(void) 
{
	RECT		crect = {0}, wrect = {0}, rectBar1 = {0}, rectBar2 = {0}, statusBar = {0};
	POINT		pt = {0};
	int			tb_top = 0, tb_bottom = 0;

	GetClientRect(&crect);
	GetWindowRect(&wrect);


	if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_MINMAX)) 
	{
		GetWindowRect(&_table_view_size);
		m_MainToolBar.GetWindowRect(&rectBar1);
		_tool_bar.GetWindowRect(&rectBar2);
		_status_bar.GetWindowRect(&statusBar);

		// figure out size of toolbars
		if (m_MainToolBar.IsFloating() && _tool_bar.IsFloating()) {

			tb_top = 0;
			tb_bottom = 0;
		}
		else if (!m_MainToolBar.IsFloating() && _tool_bar.IsFloating()) 
		{
			tb_top = rectBar1.top;
			tb_bottom = rectBar1.bottom;
		}
		else if (m_MainToolBar.IsFloating() && !_tool_bar.IsFloating()) 
		{
			tb_top = rectBar2.top;
			tb_bottom = rectBar2.bottom;
		}
		else 
		{
			if (rectBar1.top <= rectBar2.top) 
			{
				tb_top = rectBar1.top;
			}
			else 
			{
				tb_top = rectBar2.top;
			}
			if (rectBar1.bottom >= rectBar2.bottom) 
			{
				tb_bottom = rectBar1.bottom;
			}
			else 
			{
				tb_bottom = rectBar2.bottom;
			}
		}

		pt.x = (wrect.right - wrect.left) - crect.right;
		pt.y = (wrect.bottom - wrect.top) - crect.bottom;

		MoveWindow(wrect.left, wrect.top, wrect.right - wrect.left,
				   pt.y + (tb_bottom-tb_top) + (statusBar.bottom-statusBar.top) - 2,
				   true);
	}
	else 
	{
		MoveWindow(_table_view_size.left, _table_view_size.top,
				   _table_view_size.right - _table_view_size.left,
				   _table_view_size.bottom - _table_view_size.top, true);
	}
}

void CMainFrame::OnAttachTop(void) 
{
	RECT	att_rect = {0}, wrect = {0};

	::GetWindowRect(p_autoconnector->attached_hwnd(), &att_rect);
	GetWindowRect(&wrect);

	if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_TOP)) 
	{
		// uncheck attach_bottom, if necessary
		m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, false);

		MoveWindow(att_rect.left, att_rect.top-(wrect.bottom-wrect.top), att_rect.right-att_rect.left, wrect.bottom-wrect.top);
	}
}

void CMainFrame::OnAttachBottom(void)
{
	RECT	att_rect = {0}, wrect = {0};

	::GetWindowRect(p_autoconnector->attached_hwnd(), &att_rect);
	GetWindowRect(&wrect);

	if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_BOTTOM)) 
	{
		// uncheck attach_top, if necessary
		m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_ATTACH_TOP, false);

		MoveWindow(att_rect.left, att_rect.bottom, att_rect.right-att_rect.left, wrect.bottom-wrect.top);
	}
}

void CMainFrame::OnLockBlinds(void)
{
	CDlgLockBlinds	lockblinds_dlg;

	if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_LOCK_BLINDS)) 
	{
		
		lockblinds_dlg.sblind = prefs.sblind();
		lockblinds_dlg.bblind = prefs.bblind();
		lockblinds_dlg.bbet = prefs.bbet();
		lockblinds_dlg.ante = prefs.ante();
		lockblinds_dlg.gametype = prefs.gametype();

		if (lockblinds_dlg.DoModal() == IDOK) 
		{
			p_tablelimits->LockBlindsManually(lockblinds_dlg.sblind, lockblinds_dlg.bblind,
				lockblinds_dlg.bbet, lockblinds_dlg.ante, lockblinds_dlg.gametype);
		}
		else 
		{
			p_tablelimits->UnLockBlindsManually();
			m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_LOCK_BLINDS, false);
		}
	}
}

void CMainFrame::OnFormulaViewMainToolbar() 
{
	if (!m_MainToolBar.IsVisible())
		ShowControlBar(&m_MainToolBar, TRUE, FALSE);

	else
		ShowControlBar(&m_MainToolBar, FALSE, FALSE);

	RecalcLayout();
}

void CMainFrame::OnFormulaViewFlagsToolbar() 
{
	if (!_tool_bar.IsVisible())
		ShowControlBar(&_tool_bar, TRUE, FALSE);

	else
		ShowControlBar(&_tool_bar, FALSE, FALSE);

	RecalcLayout();
}

void CMainFrame::OnFormulaViewStatusbar() 
{
	if (!_status_bar.IsVisible())
		ShowControlBar(&_status_bar, TRUE, FALSE);

	else
		ShowControlBar(&_status_bar, FALSE, FALSE);

	RecalcLayout();
}

BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (_wait_cursor)
	{
		RestoreWaitCursor();
		return TRUE;
	}

	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		if (m_formulaScintillaDlg)
		{
			if (m_formulaScintillaDlg->m_dirty && !prefs.disable_msgbox())
			{
				if (MessageBox("The Formula Editor has un-applied (and unsaved) changes.\n"
							   "Really exit?", "Unsaved formula warning", MB_YESNO) == IDNO)
			{
				return;
			}
		}
	}
	}
	CFrameWnd::OnSysCommand(nID, lParam);
}

void CMainFrame::OnPerlLoadFormula() 
{
	if (p_perl->IsAFormulaLoaded())
	{
		p_perl->UnloadFormulaFile();
	}
	else
	{
		//  Reload the most recent formula
		p_perl->ReloadFormulaFile();
	}
	//  Make some noise, as there's no visible reaction
	Beep(880, 125);	
}

void CMainFrame::OnPerlLoadSpecificFormula() 
{
	CFileDialog			cfd(true);

	cfd.m_ofn.lpstrInitialDir = prefs.path_perl();
	cfd.m_ofn.lpstrFilter = "Perl Scripts (*.pl)\0*.pl\0Perl Modules (*.pm)\0*.pm\0All Files (*.*)\0*.*\0\0";
	cfd.m_ofn.lpstrTitle = "Select Perl formula file to OPEN";
	if (cfd.DoModal() == IDOK)
	{
		p_perl->LoadFormulaFile(cfd.m_ofn.lpstrFile);
		prefs.set_path_perl(cfd.GetPathName());
	}
}

void CMainFrame::OnPerlEditMainFormula() 
{
	p_perl->EditMainFormulaFile();
}

void CMainFrame::OnPerlReloadFormula() 
{
	//  Reload the most recent formula
	//	(This is a shortcut for unload + load.)
	p_perl->ReloadFormulaFile();
	//  Make some noise, as there's no visible reaction
	Beep(880, 125);
}

void CMainFrame::OnPerlCheckSyntax() 
{
	p_perl->CheckSyntax();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Menu updaters

void CMainFrame::OnUpdateMenuFileNew(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
					!m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);
}

void CMainFrame::OnUpdateMenuFileOpen(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
					!m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);
}

void CMainFrame::OnUpdateMenuFileLoadProfile(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
					!m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);
}

void CMainFrame::OnUpdateFileConnect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!p_autoconnector->attached_hwnd());
}

void CMainFrame::OnUpdateFileDisconnect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(p_autoconnector->attached_hwnd()!=NULL);
}

void CMainFrame::OnUpdateMenuDllLoad(CCmdUI* pCmdUI)
{
	if (p_dll_extension->IsDllLoaded())
		pCmdUI->SetText("&Unload\tF4");

	else
		pCmdUI->SetText("&Load\tF4");

	// Not connected to ppro server
	if (!p_pokerpro->IsConnected()) 
	{
		pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
						!m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);
	}
	// connected to ppro server
	else 
	{
		pCmdUI->Enable(p_pokerpro->ppdata()->m_pinf[p_pokerpro->ppdata()->m_userchair].m_isActive&0x1 ? false : true);
	}
}

void CMainFrame::OnUpdateDllLoadspecificfile(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(p_dll_extension->IsDllLoaded() ? false : true);
}

void CMainFrame::OnUpdatePokerproConnect(CCmdUI *pCmdUI) 
{
	pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
					!m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) &&
				   !p_pokerpro->IsConnected() ? false : true);
}

void CMainFrame::OnUpdateViewShootreplayframe(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(p_autoconnector->attached_hwnd() != NULL);
}

void CMainFrame::OnUpdateViewMainToolbar(CCmdUI *pCmdUI) 
{
	pCmdUI->SetCheck(m_MainToolBar.IsVisible() ? 1 : 0);
}

void CMainFrame::OnUpdateViewFlagsToolbar(CCmdUI *pCmdUI) 
{
	pCmdUI->SetCheck(_tool_bar.IsVisible() ? 1 : 0);
}

void CMainFrame::OnUpdateViewStatusbar(CCmdUI *pCmdUI) 
{
	pCmdUI->SetCheck(_status_bar.IsVisible() ? 1 : 0);
}

void CMainFrame::OnUpdateMenuPerlLoad(CCmdUI* pCmdUI)
{
	if (p_perl->IsAFormulaLoaded()) 
		pCmdUI->SetText("&Unload Formula\tF7");

	else 
		pCmdUI->SetText("&Load Formula\tF7");

	pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
					!m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);
}

void CMainFrame::OnUpdateMenuPerlLoadSpecificFormula(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
					!m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);
}

void CMainFrame::OnUpdateMenuPerlReloadFormula(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((p_perl->IsAFormulaLoaded() &&
					!m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) &&
					m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)));
}

void CMainFrame::OnUpdateMenuPerlCheckSyntax(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(p_perl->IsAFormulaLoaded());
}

void CMainFrame::OnUpdateMenuPerlEditMainFormula(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(p_perl->IsAFormulaLoaded());
}

void CMainFrame::OnUpdateEditForceuserchair(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!p_symbols->user_chair_confirmed() && p_autoconnector->attached_hwnd()!=NULL ? true : false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Other functions

void CMainFrame::UpdateWindowTitle()
{
	SetMainWindowTitle(p_formula->formula_name() + " - " + m_strTitle);
}

void CMainFrame::StartTimer()
{
	// Start timer that checks for continued existence of attached HWND
	SetTimer(HWND_CHECK_TIMER, 200, 0);
}

void CMainFrame::ResetDisplay()
{
	InvalidateRect(NULL, true); 
}

void CMainFrame::KillTimer()
{
	CFrameWnd::KillTimer(HWND_CHECK_TIMER);
}

void CMainFrame::OnHelp()
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


void CMainFrame::OnHelpWiki()
{
	ShellExecute(NULL, "open", "http://www.maxinmontreal.com/wiki/index.php5?title=Main_Page", "", "", SW_SHOWDEFAULT);
}

void CMainFrame::OnHelpForums()
{
	ShellExecute(NULL, "open", "http://www.maxinmontreal.com/forums/index.php", "", "", SW_SHOWDEFAULT);
}
