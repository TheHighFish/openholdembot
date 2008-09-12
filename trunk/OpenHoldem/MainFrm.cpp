// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <process.h>

#include "MainFrm.h"
#include "OpenHoldem.h"
#include "OpenHoldemDoc.h"

#include "CScraper.h"
#include "CSymbols.h"
#include "CIteratorThread.h"
#include "CHeartbeatThread.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CDllExtension.h"
#include "CPokerPro.h"
#include "CAutoplayer.h"
#include "CReplayFrame.h"

#include "..\CTransform\CTransform.h"
#include "CFormula.h"

#include "DialogFormulaScintilla.h"
#include "SAPrefsDialog.h"
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

#include "DialogSelectTable.h"
#include "inlines/eval.h"
#include "DialogPPro.h"
#include "DialogScraperOutput.h"
#include "DialogLockBlinds.h"
#include "CPerl.hpp"

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
    ON_UPDATE_COMMAND_UI(ID_VIEW_MAIN_TOOLBAR, &CMainFrame::OnUpdateViewMainToolbar)
    ON_UPDATE_COMMAND_UI(ID_VIEW_FLAGS_TOOLBAR, &CMainFrame::OnUpdateViewFlagsToolbar)
    ON_UPDATE_COMMAND_UI(ID_VIEW_STATUSBAR, &CMainFrame::OnUpdateViewStatusbar)
    ON_UPDATE_COMMAND_UI(ID_DLL_LOAD, &CMainFrame::OnUpdateMenuDllLoad)
    ON_UPDATE_COMMAND_UI(ID_DLL_LOADSPECIFICFILE, &CMainFrame::OnUpdateDllLoadspecificfile)
    ON_UPDATE_COMMAND_UI(ID_POKERPRO_CONNECT, &CMainFrame::OnUpdatePokerproConnect)
    //  2008.03.04 by THF
    ON_UPDATE_COMMAND_UI(ID_PERL_LOADFORMULA, &CMainFrame::OnUpdateMenuPerlLoad)
    //  2008.03.07 by THF
    ON_UPDATE_COMMAND_UI(ID_PERL_LOADSPECIFICFORMULA, &CMainFrame::OnUpdateMenuPerlLoadSpecificFormula)
    //  2008.03.20 by THF
    ON_UPDATE_COMMAND_UI(ID_PERL_RELOADFORMULA, &CMainFrame::OnUpdateMenuPerlReloadFormula)
	//  2008.04.29 by THF
	ON_UPDATE_COMMAND_UI(ID_PERL_CHECKSYNTAX, &CMainFrame::OnUpdateMenuPerlCheckSyntax)
	ON_UPDATE_COMMAND_UI(ID_PERL_EDITMAINFORMULA, &CMainFrame::OnUpdateMenuPerlEditMainFormula)

    //  Menu commands
    ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
    ON_COMMAND(ID_FILE_LOADTABLEMAP, &CMainFrame::OnFileLoadTableMap)
    ON_COMMAND(ID_FILE_CONNECT, &CMainFrame::OnBnClickedGreenCircle)
    ON_COMMAND(ID_EDIT_FORMULA, &CMainFrame::OnEditFormula)
    ON_COMMAND(ID_EDIT_PREFERENCES, &CMainFrame::OnEditPreferences)
    ON_COMMAND(ID_VIEW_MAIN_TOOLBAR, &CMainFrame::OnFormulaViewMainToolbar)
    ON_COMMAND(ID_VIEW_FLAGS_TOOLBAR, &CMainFrame::OnFormulaViewFlagsToolbar)
    ON_COMMAND(ID_VIEW_STATUSBAR, &CMainFrame::OnFormulaViewStatusbar)
    ON_COMMAND(ID_VIEW_SCRAPEROUTPUT, &CMainFrame::OnScraperOutput)
    ON_COMMAND(ID_VIEW_SHOOTREPLAYFRAME, &CMainFrame::OnViewShootreplayframe)
    ON_COMMAND(ID_DLL_LOAD, &CMainFrame::OnDllLoad)
    ON_COMMAND(ID_DLL_LOADSPECIFICFILE, &CMainFrame::OnDllLoadspecificfile)
    //  2008.03.03 by THF
    ON_COMMAND(ID_PERL_LOADFORMULA, &CMainFrame::OnPerlLoadFormula)
    ON_COMMAND(ID_PERL_LOADSPECIFICFORMULA, &CMainFrame::OnPerlLoadSpecificFormula)
    ON_COMMAND(ID_PERL_RELOADFORMULA, &CMainFrame::OnPerlReloadFormula)
    ON_COMMAND(ID_PERL_CHECKSYNTAX, &CMainFrame::OnPerlCheckSyntax)
    ON_COMMAND(ID_PERL_EDITMAINFORMULA, &CMainFrame::OnPerlEditMainFormula)

    // Main toolbar
    ON_BN_CLICKED(ID_MAIN_TOOLBAR_GREENCIRCLE, &CMainFrame::OnBnClickedGreenCircle)
    ON_BN_CLICKED(ID_MAIN_TOOLBAR_REDCIRCLE, &CMainFrame::OnBnClickedRedCircle)
    ON_BN_CLICKED(ID_MAIN_TOOLBAR_AUTOPLAYER, &CMainFrame::OnAutoplayer)
    ON_BN_CLICKED(ID_MAIN_TOOLBAR_FORMULA, &CMainFrame::OnEditFormula)
    ON_BN_CLICKED(ID_MAIN_TOOLBAR_MINMAX, &CMainFrame::OnMinMax)
    ON_BN_CLICKED(ID_MAIN_TOOLBAR_ATTACH_TOP, &CMainFrame::OnAttachTop)
    ON_BN_CLICKED(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, &CMainFrame::OnAttachBottom)
    ON_BN_CLICKED(ID_MAIN_TOOLBAR_LOCK_BLINDS, &CMainFrame::OnLockBlinds)
    ON_BN_CLICKED(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, &CMainFrame::OnScraperOutput)
    ON_BN_CLICKED(ID_MAIN_TOOLBAR_SHOOTFRAME, &CMainFrame::OnViewShootreplayframe)

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

    ON_WM_TIMER()

    ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_PLCARDS,OnUpdateStatus)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_COMCARDS,OnUpdateStatus)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_PRWIN,OnUpdateStatus)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_NOPP,OnUpdateStatus)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_NIT,OnUpdateStatus)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_ACTION,OnUpdateStatus)

    ON_COMMAND(ID_POKERPRO_CONNECT, &CMainFrame::OnPokerproConnect)
	ON_MESSAGE(WMA_SETWINDOWTEXT, OnSetWindowText)

    ON_WM_SETCURSOR()
    ON_WM_SYSCOMMAND()
    ON_UPDATE_COMMAND_UI(ID_VIEW_SHOOTREPLAYFRAME, &CMainFrame::OnUpdateViewShootreplayframe)
END_MESSAGE_MAP()

static UINT indicators[] = 
{
    ID_SEPARATOR,           // status line indicator
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
    __SEH_SET_EXCEPTION_HANDLER

    __SEH_HEADER

	_autoplay_pressed = false;
	_attached_hwnd = NULL;

	for (int i=0; i<=9; i++)
		_flags[i] = false;

    __SEH_LOGFATAL("CMainFrame::Constructor :\n");
}

CMainFrame::~CMainFrame() 
{
    __SEH_HEADER

    __SEH_LOGFATAL("CMainFrame::Destructor :\n");
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    __SEH_HEADER

    CString			t;

    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create toolbars
    EnableDocking(CBRS_ALIGN_ANY);
    create_main_toolbar();
    create_flags_toolbar();
    align_toolbars();

    // Status bar
    create_status_bar();

    // Set toolbar button status
    m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_GREENCIRCLE, true);
    m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_REDCIRCLE, false);

    // Start timer that checks if we should enable buttons
    SetTimer(ENABLE_BUTTONS_TIMER, 50, 0);

    // Start timer that updates status bar
    SetTimer(UPDATE_STATUS_BAR_TIMER, 50, 0);

    // Start timer that attaches the OH window when the poker window moves
    SetTimer(ATTACH_WINDOW_TIMER, 20, 0);

    return 0;

    __SEH_LOGFATAL("CMainFrame::OnCreate :\n");
}

int CMainFrame::create_main_toolbar(void) 
{
    __SEH_HEADER

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


    __SEH_LOGFATAL("CMainFrame::create_main_toolbar :\n");
}

int CMainFrame::create_flags_toolbar(void) 
{
    __SEH_HEADER

    TBBUTTONINFO	tbi;
    tbi.cbSize = sizeof(TBBUTTONINFO);
    tbi.dwMask = TBIF_STYLE;
    tbi.fsStyle = TBSTYLE_CHECK;

    // Flags toolbar
    m_FlagsToolBar.CreateEx(this, NULL, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
                            CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
    m_FlagsToolBar.LoadToolBar(IDR_FLAGS);

    // Make flags toolbar buttons sticky
    m_FlagsToolBar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER0, &tbi);
    m_FlagsToolBar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER1, &tbi);
    m_FlagsToolBar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER2, &tbi);
    m_FlagsToolBar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER3, &tbi);
    m_FlagsToolBar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER4, &tbi);
    m_FlagsToolBar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER5, &tbi);
    m_FlagsToolBar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER6, &tbi);
    m_FlagsToolBar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER7, &tbi);
    m_FlagsToolBar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER8, &tbi);
    m_FlagsToolBar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER9, &tbi);
    m_FlagsToolBar.EnableDocking(CBRS_ALIGN_ANY);
    m_FlagsToolBar.EnableDocking(CBRS_ALIGN_TOP);
    DockControlBar(&m_FlagsToolBar);

    // Title of floating flags toolbar
    m_FlagsToolBar.SetWindowText("Flags");

    return 0;

    __SEH_LOGFATAL("CMainFrame::create_flags_toolbar :\n");
}


void CMainFrame::align_toolbars(void) 
{
    __SEH_HEADER

    // Put the main toolbar and flags toolbar on the same line
    CRect rectBar1, rectBar2;
    RecalcLayout();
    m_MainToolBar.GetWindowRect(rectBar1);
    m_FlagsToolBar.GetWindowRect(rectBar2);
    DockControlBar(&m_FlagsToolBar, AFX_IDW_DOCKBAR_TOP, rectBar1); //will be first
    UINT uiBarWidth = rectBar2.Width();
    rectBar2.left = rectBar1.right;
    rectBar2.top = rectBar1.top;
    rectBar2.bottom = rectBar1.bottom;
    rectBar2.right = rectBar1.right + uiBarWidth;
    DockControlBar(&m_FlagsToolBar, AFX_IDW_DOCKBAR_TOP, rectBar2); //will be second
    RecalcLayout();

    __SEH_LOGFATAL("CMainFrame::align_toolbars :\n");
}

int CMainFrame::create_status_bar(void) 
{
    __SEH_HEADER

    if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))	
	{
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }
    m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_STRETCH | SBPS_NOBORDERS, 0);
    m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_STATUS_PLCARDS, NULL, 30);
    m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_STATUS_COMCARDS, NULL, 75);
    m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_STATUS_POKERHAND, NULL, 65);
    m_wndStatusBar.SetPaneInfo(4, ID_INDICATOR_STATUS_HANDRANK, NULL, 55);
    m_wndStatusBar.SetPaneInfo(5, ID_INDICATOR_STATUS_PRWIN, NULL, 62);
    m_wndStatusBar.SetPaneInfo(6, ID_INDICATOR_STATUS_NOPP, NULL, 15);
    m_wndStatusBar.SetPaneInfo(7, ID_INDICATOR_STATUS_NIT, NULL, 90);
    m_wndStatusBar.SetPaneInfo(8, ID_INDICATOR_STATUS_ACTION, NULL, 70);

	return 0;

	__SEH_LOGFATAL("CMainFrame::create_status_bar :\n");
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
    __SEH_HEADER

    if ( !CFrameWnd::PreCreateWindow(cs) )
        return FALSE;

    int			max_x, max_y;

    WNDCLASS wnd;
    HINSTANCE hInst = AfxGetInstanceHandle();
   // Set class name
    if (!(::GetClassInfo(hInst, "OpenHoldem", &wnd)))
    {
        wnd.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wnd.lpfnWndProc      = ::DefWindowProc;
        wnd.cbClsExtra       = wnd.cbWndExtra = 0;
        wnd.hInstance        = hInst;
        wnd.hIcon            = AfxGetApp()->LoadIcon(IDI_ICON1);
        wnd.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wnd.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wnd.lpszMenuName     = NULL;
        wnd.lpszClassName    = "OpenHoldem";

        AfxRegisterClass( &wnd );
    }
    cs.lpszClass = "OpenHoldem";

    // Restore window location and size
    max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
    max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
    cs.x = min(prefs.main_x(), max_x);
    cs.y = min(prefs.main_y(), max_y);
    cs.cx = prefs.main_dx();
    cs.cy = prefs.main_dy();

    return true;

    __SEH_LOGFATAL("CMainFrame::PreCreateWindow :\n");
}

// CMainFrame message handlers

void CMainFrame::OnEditFormula() 
{
    __SEH_HEADER

    if (m_formulaScintillaDlg) 
	{
        BOOL	bWasShown = ::IsWindow(m_formulaScintillaDlg->m_hWnd) && m_formulaScintillaDlg->IsWindowVisible();

        if (!m_formulaScintillaDlg->DestroyWindow())
        {
            m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, true);
            return; // The Formula user requested to not have the dialog close
        }
        delete m_formulaScintillaDlg;
        m_formulaScintillaDlg = NULL;

        if (bWasShown)
            return;
    }

    m_formulaScintillaDlg = new CDlgFormulaScintilla(this);
    m_formulaScintillaDlg->Create(CDlgFormulaScintilla::IDD,this);
    m_formulaScintillaDlg->ShowWindow(SW_SHOW);
    m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, true);

    __SEH_LOGFATAL("CMainFrame::OnEditFormula :\n");
}

void CMainFrame::OnScraperOutput() 
{
    __SEH_HEADER

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


    __SEH_LOGFATAL("CMainFrame::OnScraperOutput :\n");
}

void CMainFrame::OnViewShootreplayframe()
{
	__SEH_HEADER

	CReplayFrame	crf;
    crf.CreateReplayFrame();

	__SEH_LOGFATAL("CMainFrame::OnViewShootreplayframe\n")
}

void CMainFrame::OnEditPreferences() 
{
    __SEH_HEADER

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
    //  2008.03.02 by THF
    CDlgSAPrefs9 page9;
    CDlgSAPrefs10 page10;
    CDlgSAPrefs11 page11;

    // add pages
    // 2008.03.02 by THF: Perl, Chat, alphabetical order
    dlg.AddPage(page1, "Analyzer");
    dlg.AddPage(page2, "Autoplayer");
    dlg.AddPage(page10, "Chat");
    dlg.AddPage(page3, "DLL Extension");
    dlg.AddPage(page7, "ICM");
    dlg.AddPage(page11, "Logging");
    dlg.AddPage(page9, "Perl");
    dlg.AddPage(page6, "Poker Tracker");
    dlg.AddPage(page8, "Replay Frames");
    dlg.AddPage(page4, "Scraper");
    dlg.AddPage(page5, "Symbols");

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

    __SEH_LOGFATAL("CMainFrame::OnEditPreferences :\n");
}

BOOL CMainFrame::DestroyWindow() 
{
    __SEH_HEADER

    WINDOWPLACEMENT wp;

	//unload dll   Matrix (suggested by Spud) 2008-05-17
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
		p_pokertracker_thread->StopThread();

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
        prefs.set_main_x(table_view_size.left);
        prefs.set_main_y(table_view_size.top);
        prefs.set_main_dx(table_view_size.right - table_view_size.left);
        prefs.set_main_dy(table_view_size.bottom - table_view_size.top);
    }

    return CFrameWnd::DestroyWindow();

    __SEH_LOGFATAL("CMainFrame::DestroyWindow :\n");
}

void CMainFrame::OnFileOpen() 
{
    __SEH_HEADER

    CFileDialog			cfd(true);
    CString				theKey = "DefWHFOpenLocation";
    char				path[MAX_PATH];

    ReadRegString(theKey, path);
    cfd.m_ofn.lpstrInitialDir = path;
    cfd.m_ofn.lpstrFilter = "OpenHoldem Files (.ohf)\0*.ohf\0WinHoldem Files (.whf)\0*.whf\0All files (*.*)\0*.*\0\0";
    cfd.m_ofn.lpstrTitle = "Select Formula file to OPEN";
    if (cfd.DoModal() == IDOK)
    {			
        COpenHoldemDoc *pDoc = (COpenHoldemDoc *)this->GetActiveDocument();		
		pDoc->OnOpenDocument(cfd.GetPathName());
		pDoc->SetPathName(cfd.GetPathName());
        // Update window title, registry
        SetWindowText(cfd.GetFileTitle() + " - " + CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)));
        WriteRegString(theKey, cfd.GetPathName());
    }

    __SEH_LOGFATAL("CMainFrame::OnFileOpen :\n");
}


void CMainFrame::OnFileLoadTableMap() 
{
    __SEH_HEADER

    CFileDialog			cfd(true);
    int					line, ret;
    CString				e;
    CString				theKey = "DefTMOpenLocation";
    char				path[MAX_PATH];

    ReadRegString(theKey, path);
    cfd.m_ofn.lpstrInitialDir = path;
    cfd.m_ofn.lpstrFilter = "OpenScrape Table Maps (.tm)\0*.tm\0All files (*.*)\0*.*\0\0";
    cfd.m_ofn.lpstrTitle = "Select OpenScrape table map to OPEN";

    if (cfd.DoModal() == IDOK)
    {
        ret = p_tablemap->LoadTablemap(cfd.m_ofn.lpstrFile, VER_OPENSCRAPE_1, false, &line);
        if (ret == ERR_VERSION)
        {
            e.Format("This is an OpenHoldem v1 profile (.ohdb1).\n"\
                     "OpenHoldem versions 1.2.0 and higher require v2 Profiles (.ohdb2),\n"\
                     "or OpenScrape Table Maps.\n");
            MessageBox(e, "Table map load error", MB_OK);
        }
        else if (ret != SUCCESS)
        {
            e.Format("Error code: %d  line: %d", ret, line);
            MessageBox(e, "Table map load error", MB_OK);
        }
        else
        {
            p_tablemap->SaveR$Indices();
            p_tablemap->SaveS$Indices();
            p_tablemap->SaveS$Strings();

            // Reset "ScraperOutput" dialog, if it is live
            if (m_ScraperOutputDlg) 
			{
                m_ScraperOutputDlg->add_listbox_items();
                m_ScraperOutputDlg->do_update_display();
            }

            WriteRegString(theKey, cfd.GetPathName());
        }
    }

    __SEH_LOGFATAL("CMainFrame::OnFileLoadTableMap :\n");
}

void CMainFrame::OnBnClickedGreenCircle() 
{
    __SEH_HEADER

    int					i, N, line, ret;
    CDlgSelectTable		cstd;
    STableList			tablelisthold;
    int					result;
    char				title[512];
	SWholeMap			smap;
	CString				path, current_path;
	BOOL				bFound;
	CFileFind			hFile;

    // Clear global list for holding table candidates
    g_tlist.RemoveAll();

    // First check explicitly loaded/last used tablemap
	current_path = "";
	if (p_tablemap->valid())
	{
		smap.z$ = p_tablemap->z$();
		smap.s$ = p_tablemap->s$();
		smap.r$ = p_tablemap->r$();
		smap.t$ = p_tablemap->t$();
		smap.p$ = p_tablemap->p$();
		smap.h$ = p_tablemap->h$();
		smap.i$ = p_tablemap->i$();
		smap.filepath = p_tablemap->filepath();

	    EnumWindows(EnumProcTopLevelWindowList, (LPARAM) &smap);
		current_path = p_tablemap->filepath();
	}

    // OpenScrape table maps
    path.Format("%s\\scraper\\*.tm", _startup_path);
    bFound = hFile.FindFile(path.GetString());
    while (bFound)
    {
        bFound = hFile.FindNextFile();
        if (!hFile.IsDots() && !hFile.IsDirectory() && hFile.GetFilePath()!=current_path)
		{
			ret = p_tablemap->LoadTablemap((char *) hFile.GetFilePath().GetString(), VER_OPENSCRAPE_1, false, &line);
            if (ret == SUCCESS)
			{
				smap.z$ = p_tablemap->z$();
				smap.s$ = p_tablemap->s$();
				smap.r$ = p_tablemap->r$();
				smap.t$ = p_tablemap->t$();
				smap.p$ = p_tablemap->p$();
				smap.h$ = p_tablemap->h$();
				smap.i$ = p_tablemap->i$();
				smap.filepath = p_tablemap->filepath();
		
				EnumWindows(EnumProcTopLevelWindowList, (LPARAM) &smap);
			}
		}
    }

	// WinScrape converted profiles
    path.Format("%s\\scraper\\*.ws", _startup_path);
    bFound = hFile.FindFile(path.GetString());
    while (bFound)
    {
        bFound = hFile.FindNextFile();
        if (!hFile.IsDots() && !hFile.IsDirectory() && hFile.GetFilePath()!=current_path)
		{
			ret = p_tablemap->LoadTablemap((char *) hFile.GetFilePath().GetString(), VER_OPENSCRAPE_1, false, &line);
            if (ret == SUCCESS)
			{
				smap.z$ = p_tablemap->z$();
				smap.s$ = p_tablemap->s$();
				smap.r$ = p_tablemap->r$();
				smap.t$ = p_tablemap->t$();
				smap.p$ = p_tablemap->p$();
				smap.h$ = p_tablemap->h$();
				smap.i$ = p_tablemap->i$();
				smap.filepath = p_tablemap->filepath();

				EnumWindows(EnumProcTopLevelWindowList, (LPARAM) &smap);
			}
		}
    }

    // Put global candidate table list in table select dialog variables
	N = (int) g_tlist.GetSize();
    if (N == 0) 
	{
        int cySize = GetSystemMetrics(SM_CYSIZE);
        int cyMenuSize = GetSystemMetrics(SM_CYMENU);
        if (cySize != 18 && cyMenuSize != 19)
            MessageBox("Cannot find table\n\nIt appears that your settings are not configured according to OpenHoldem specifications.\nYou must ensure that XP themes are not used (Use Windows Classic style) and\nfont size is set to normal.\n\nFor more info, look at the wiki documentation and the user forums", "Cannot find table", MB_OK);
        else
            MessageBox("No valid tables found", "Cannot find table", MB_OK);
    }
    else 
	{
        if (N==1) 
		{
            result = IDOK;
            cstd.selected_item = 0;
        }
        else if (N>=2) 
		{
            for (i=0; i<N; i++) 
			{
                tablelisthold.hwnd = g_tlist[i].hwnd;
                tablelisthold.title = g_tlist[i].title;
                tablelisthold.path = g_tlist[i].path;
                tablelisthold.crect.left = g_tlist[i].crect.left;
                tablelisthold.crect.top = g_tlist[i].crect.top;
                tablelisthold.crect.right = g_tlist[i].crect.right;
                tablelisthold.crect.bottom = g_tlist[i].crect.bottom;
                cstd.tlist.Add(tablelisthold);
            }

            // Display table select dialog
            result = cstd.DoModal();
        }

        if (result == IDOK) 
		{
            // Load correct tablemap, and save hwnd/rect/numchairs of table that we are "attached" to
            set_attached_hwnd(g_tlist[cstd.selected_item].hwnd);
			p_tablemap->LoadTablemap((char *) g_tlist[cstd.selected_item].path.GetString(), VER_OPENSCRAPE_1, false, &line);
            p_tablemap->SaveR$Indices();
            p_tablemap->SaveS$Indices();
            p_tablemap->SaveS$Strings();

            // Create bitmaps
			p_scraper->CreateBitmaps();

            // Clear scraper fields
			p_scraper->ClearScrapeAreas();

            // Reset symbols
            p_symbols->ResetSymbolsFirstTime();

            // Reset "ScraperOutput" dialog, if it is live
            if (m_ScraperOutputDlg) 
			{
                m_ScraperOutputDlg->add_listbox_items();
                m_ScraperOutputDlg->do_update_display();
            }

            // Disable buttons, menu items
            m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, false);
            m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, false);
            m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_GREENCIRCLE, false);
            m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_REDCIRCLE, true);
            m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, true);

            // Make sure autoplayer is off
            p_autoplayer->set_autoplayer_engaged(false);

            // mark symbol result cache as stale
			p_formula->MarkCacheStale();

			// reset iterator vars
			EnterCriticalSection(&cs_iterator);
				_iter_vars.iterator_thread_running = false;
				_iter_vars.iterator_thread_complete = false;
				_iter_vars.iterator_thread_progress = 0;
				_iter_params.nit = 0;
				_iter_params.f$p = 0;
				_iter_params.br = 0;
				for (i=0; i<=1; i++)
					_iter_params.pcard[i] = CARD_NOCARD;
				for (i=0; i<=4; i++)
					_iter_params.ccard[i] = CARD_NOCARD;

				_iter_vars.prwin = 0.;
				_iter_vars.prtie = 0.;
				_iter_vars.prlos = 0.;
			LeaveCriticalSection(&cs_iterator);

            // start heartbeat thread
            if (p_heartbeat_thread)
				delete p_heartbeat_thread;

			p_heartbeat_thread = new CHeartbeatThread;

            // Start timer that checks for continued existence of attached HWND
            SetTimer(HWND_CHECK_TIMER, 200, 0);

            // Reset display
            InvalidateRect(NULL, true);

            // Start logging
            start_log();

            CWindowDC dc(NULL);
            int nBitsPerPixel = dc.GetDeviceCaps(PLANES) * dc.GetDeviceCaps(BITSPIXEL);
            if (nBitsPerPixel < 24)
                MessageBox("It appears that your Display settings are not configured according to OpenHoldem specifications.\n24 bit color or higher is needed to reliably extract information from the poker client\n\nFor more info, look at the wiki documentation and the user forums", "Caution: Color Depth Too Low", MB_OK|MB_ICONWARNING);

            BOOL fontSmoothingEnabled = FALSE;
            SystemParametersInfo(SPI_GETFONTSMOOTHING, 0, (LPVOID)&fontSmoothingEnabled, 0);
            if (fontSmoothingEnabled)
                MessageBox("It appears that font smoothing is enabled. In order for OpenHoldem to reliably\nextract information from the poker client you should disable Font Smoothing", "Caution: Font smoothing is enabled", MB_OK|MB_ICONWARNING);

            // log OH title bar text and table reset
            ::GetWindowText(_attached_hwnd, title, 512);
            write_log("\n*************************************************************\nTABLE RESET %s - %s(%s)\n*************************************************************\n",

				p_formula->formula_name().GetString(), p_tablemap->s$items()->sitename.GetString(), title);

        }
    }

    __SEH_LOGFATAL("CMainFrame::OnBnClickedGreenCircle :\n");
}

void CMainFrame::OnBnClickedRedCircle() 
{
    __SEH_HEADER

	// stop threads
	if (p_heartbeat_thread)
	{
		delete p_heartbeat_thread;
		p_heartbeat_thread = NULL;
	}

	if (p_pokertracker_thread)
		p_pokertracker_thread->StopThread();

    // Make sure autoplayer is off
    p_autoplayer->set_autoplayer_engaged(false);
    _autoplay_pressed = false;

    // Clear "attached" info
    set_attached_hwnd(NULL);

    // Stop timer that checks for valid hwnd
    KillTimer(HWND_CHECK_TIMER);

    // Unattach OH from any poker window
    if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_TOP))
        m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_ATTACH_TOP, false);
    
	if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_BOTTOM))
        m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, false);

    // Reset toolbar/menu button state
    m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_GREENCIRCLE, true);
    m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_REDCIRCLE, false);
    m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
    m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
    m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, true);
    m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, true);

    // Delete bitmaps
	p_scraper->DeleteBitmaps();

    // Clear profile indices
    p_tablemap->ClearR$Indices();

    // Clear scraper fields
	p_scraper->ClearScrapeAreas();

    // Reset symbols
    p_symbols->ResetSymbolsFirstTime();

    // Change window title
    SetWindowText(p_formula->formula_name() + " - " + m_strTitle);

    // Reset Display
    InvalidateRect(NULL, false);

    // Reset "ScraperOutput" dialog, if it is live
    if (m_ScraperOutputDlg)
    {
        m_ScraperOutputDlg->add_listbox_items();
        m_ScraperOutputDlg->do_update_display();
    }

    // log OH title bar text and table reset

    write_log("%s - %s(NOT ATTACHED)\n", p_formula->formula_name().GetString(), p_tablemap->s$items()->sitename.GetString());

    write_log("TABLE RESET\n*************************************************************\n");

    // Stop logging
    stop_log();

    __SEH_LOGFATAL("CMainFrame::OnBnClickedRedCircle :\n");
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
    __SEH_HEADER

    CardMask		Cards;
    int				nCards;
    HandVal			hv;
    char			hvstring[100];
    int				i;
    char			*card;
    CString			temp;
    bool			playing;
    RECT			att_rect, wrect;
    static RECT		prev_att_rect={0}, prev_wrect={0};

	if (nIDEvent == HWND_CHECK_TIMER)
    {
        if (!IsWindow(_attached_hwnd))
        {
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
        if (_attached_hwnd != NULL) 
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
        if (_attached_hwnd != NULL)
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
		status_plcards = "";
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
					status_plcards.Append(temp);
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
					status_plcards.Append(temp);
					CardMask_SET(Cards, p_scraper->card_player_for_display(i));
					nCards++;
				}
			}
		}

		// Common cards
		status_comcards = "";
		for (i=0; i<=4; i++) 
		{
			if (p_scraper->card_common(i) != CARD_BACK && 
				p_scraper->card_common(i) != CARD_NOCARD) 
			{
				card = StdDeck_cardString(p_scraper->card_common(i));
				temp.Format("%s ", card);
				status_comcards.Append(temp);
				CardMask_SET(Cards, p_scraper->card_common(i));
				nCards++;
			}
		}

		// poker hand
		hv = Hand_EVAL_N(Cards, nCards);
		HandVal_toString(hv, hvstring);
		status_pokerhand = hvstring;
		status_pokerhand = status_pokerhand.Mid(0, status_pokerhand.Find(" "));

		// handrank
		if (prefs.sym_handrank_value() == "169")
			status_handrank.Format("%.0f/169", p_symbols->sym()->handrank169);

		else if (prefs.sym_handrank_value() == "1000")
			status_handrank.Format("%.0f/1000", p_symbols->sym()->handrank1000);

		else if (prefs.sym_handrank_value() == "1326")
			status_handrank.Format("%.0f/1326", p_symbols->sym()->handrank1326);

		else if (prefs.sym_handrank_value() == "2652")
			status_handrank.Format("%.0f/2652", p_symbols->sym()->handrank2652);

		else if (prefs.sym_handrank_value() == "p")
			status_handrank.Format("%.2f/2652", p_symbols->sym()->handrankp);

		// nopponents
		if (playing)
			status_nopp.Format("%d", (int) p_symbols->sym()->nopponents);

		else
			status_nopp = "";

		// Always update prwin/nit
		if (p_symbols->user_chair_confirmed() && playing)
		{
			status_prwin.Format("%d/%d/%d", 
				(int) (_iter_vars.prwin*1000), 
				(int) (_iter_vars.prtie*1000),
				(int) (_iter_vars.prlos*1000));
			status_nit.Format("%d/%d", 
				_iter_vars.iterator_thread_progress, 
				(int) p_formula->formula()->dNit);
		}
		else
		{
			status_prwin = "0/0/0";
			status_nit.Format("0/%d", (int) p_formula->formula()->dNit);
		}

		// action
		if (!p_symbols->user_chair_confirmed() || !playing)
			status_action = "Notplaying";

		else if (p_symbols->f$prefold())
		{
			status_action = "Pre-fold";
		}

		else if (p_symbols->user_chair_confirmed() && _iter_vars.iterator_thread_complete)
		{
			if (p_symbols->f$alli())  status_action = "Allin";
			else if (p_symbols->f$swag())  status_action.Format("SWAG: %.2f", p_symbols->f$swag());
			else if (p_symbols->f$rais())  status_action = "Bet/Raise";
			else if (p_symbols->f$call())  status_action = "Call/Check";
			else  status_action = "Fold/Check";
		}

		else if (p_symbols->sym()->nopponents==0)
			status_action = "Idle (f$P==0)";

		else
			status_action = "Thinking";
    }

    else if (nIDEvent == ATTACH_WINDOW_TIMER)
    {
        ::GetWindowRect(_attached_hwnd, &att_rect);
        GetWindowRect(&wrect);

        if (memcmp(&prev_att_rect, &att_rect, sizeof(RECT))!=0 ||
                memcmp(&prev_wrect, &wrect, sizeof(RECT))!=0)
        {
            ::GetWindowRect(_attached_hwnd, &prev_att_rect);
            GetWindowRect(&prev_wrect);

            if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_TOP))
            {
                if (IsWindow(_attached_hwnd))
                    MoveWindow(att_rect.left, att_rect.top-(wrect.bottom-wrect.top), att_rect.right-att_rect.left, wrect.bottom-wrect.top);
            }

            if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_BOTTOM))
            {
                if (IsWindow(_attached_hwnd))
                    MoveWindow(att_rect.left, att_rect.bottom, att_rect.right-att_rect.left, wrect.bottom-wrect.top);
            }
        }
    }

    __SEH_LOGFATAL("CMainFrame::OnTimer :\n");
}

void CMainFrame::OnClickedFlags() 
{
    __SEH_HEADER

    set_flags(0, m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER0));
	set_flags(1, m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER1));
	set_flags(2, m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER2));
	set_flags(3, m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER3));
	set_flags(4, m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER4));
	set_flags(5, m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER5));
	set_flags(6, m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER6));
	set_flags(7, m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER7));
	set_flags(8, m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER8));
	set_flags(9, m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER9));

	p_symbols->CalcSymbols();

    __SEH_LOGFATAL("CMainFrame::OnClickedFlags :\n");
}


void CMainFrame::OnAutoplayer() 
{
    __SEH_HEADER

    CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);

    if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_AUTOPLAYER)) 
	{
        // calc hand lists
        p_formula->CreateHandListMatrices();

        // one last parse - do not engage if parse fails
        if (p_formula->ParseAllFormula(pMyMainWnd->GetSafeHwnd())) 
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

    __SEH_LOGFATAL("CMainFrame::OnAutoplayer :\n");
}

void CMainFrame::OnUpdateStatus(CCmdUI *pCmdUI) 
{
    __SEH_HEADER

    m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_PLCARDS), status_plcards);
    m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_COMCARDS), status_comcards);
    m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_POKERHAND), status_pokerhand);
    m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_HANDRANK), status_handrank);
    m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_PRWIN), status_prwin);
    m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_NOPP), status_nopp);
    m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_NIT), status_nit);
    m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_ACTION), status_action);
    // if action==bet ... m_wndStatusBar.GetStatusBarCtrl().SetBkColor(RGB(180,180,180));

    __SEH_LOGFATAL("CMainFrame::OnUpdateStatus :\n");
}

void CMainFrame::OnDllLoad() 
{
    __SEH_HEADER

		if (p_dll_extension->IsDllLoaded())
			p_dll_extension->UnloadDll();
		else
			p_dll_extension->LoadDll("");

    __SEH_LOGFATAL("CMainFrame::OnDllLoad :\n");
}


void CMainFrame::OnDllLoadspecificfile()
{
    __SEH_HEADER

    CFileDialog			cfd(true);
    CString				theKey = "DefDLLOpenLocation";
    char				path[MAX_PATH];

    ReadRegString(theKey, path);
    cfd.m_ofn.lpstrInitialDir = path;
    cfd.m_ofn.lpstrFilter = "DLL Files (.dll)\0*.dll\0\0";
    cfd.m_ofn.lpstrTitle = "Select OpenHoldem DLL file to OPEN";

    if (cfd.DoModal() == IDOK) 
	{
        p_dll_extension->UnloadDll();

		p_dll_extension->LoadDll(cfd.m_ofn.lpstrFile);

        WriteRegString(theKey, cfd.GetPathName());
    }

    __SEH_LOGFATAL("CMainFrame::OnDllLoadspecificfile :\n");
}

LRESULT CMainFrame::OnSetWindowText(WPARAM, LPARAM title)
{
	CString *sTitle = (CString *)title;

	SetWindowText(sTitle->GetString());

	delete sTitle;
	return 0l;
}

void CMainFrame::OnPokerproConnect() 
{
    __SEH_HEADER
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

    __SEH_LOGFATAL("CMainFrame::OnPokerproConnect :\n");
}

void CMainFrame::OnMinMax(void) 
{
    __SEH_HEADER

    RECT		crect, wrect, rectBar1, rectBar2, statusBar;
    POINT		pt;
    int			tb_top, tb_bottom;
    GetClientRect(&crect);
    GetWindowRect(&wrect);


    if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_MINMAX)) 
	{
        GetWindowRect(&table_view_size);
        m_MainToolBar.GetWindowRect(&rectBar1);
        m_FlagsToolBar.GetWindowRect(&rectBar2);
        m_wndStatusBar.GetWindowRect(&statusBar);

        // figure out size of toolbars
        if (m_MainToolBar.IsFloating() && m_FlagsToolBar.IsFloating()) {

            tb_top = 0;
            tb_bottom = 0;
        }
        else if (!m_MainToolBar.IsFloating() && m_FlagsToolBar.IsFloating()) 
		{
            tb_top = rectBar1.top;
            tb_bottom = rectBar1.bottom;
        }
        else if (m_MainToolBar.IsFloating() && !m_FlagsToolBar.IsFloating()) 
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
        MoveWindow(table_view_size.left, table_view_size.top,
                   table_view_size.right - table_view_size.left,
                   table_view_size.bottom - table_view_size.top, true);
    }

    __SEH_LOGFATAL("CMainFrame::OnMinMax :\n");
}

void CMainFrame::OnAttachTop(void) 
{
    __SEH_HEADER

    RECT	att_rect, wrect;
    ::GetWindowRect(_attached_hwnd, &att_rect);
    GetWindowRect(&wrect);

    if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_TOP)) 
	{
        // uncheck attach_bottom, if necessary
        m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, false);

        MoveWindow(att_rect.left, att_rect.top-(wrect.bottom-wrect.top), att_rect.right-att_rect.left, wrect.bottom-wrect.top);
    }

    __SEH_LOGFATAL("CMainFrame::OnAttachTop :\n");
}

void CMainFrame::OnAttachBottom(void)
{
    __SEH_HEADER

    RECT	att_rect, wrect;
    ::GetWindowRect(_attached_hwnd, &att_rect);
    GetWindowRect(&wrect);

    if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_BOTTOM)) 
	{
        // uncheck attach_top, if necessary
        m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_ATTACH_TOP, false);

        MoveWindow(att_rect.left, att_rect.bottom, att_rect.right-att_rect.left, wrect.bottom-wrect.top);
    }

    __SEH_LOGFATAL("CMainFrame::OnAttachBottom :\n");
}

void CMainFrame::OnLockBlinds(void)
{
    __SEH_HEADER

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
			// Tell CScraper about our locked blinds
			SLockBlinds LB;
			LB.blinds_are_locked = true;
			LB.sblind = lockblinds_dlg.sblind;
			LB.bblind = lockblinds_dlg.bblind;
			LB.bbet = lockblinds_dlg.bbet;
			LB.ante = lockblinds_dlg.ante;
			LB.gametype = lockblinds_dlg.gametype;

            p_scraper->SetLockedBlinds(LB);

			// Save locked blinds info for future use
            prefs.set_sblind(lockblinds_dlg.sblind);
            prefs.set_bblind(lockblinds_dlg.bblind);
            prefs.set_bbet(lockblinds_dlg.bbet);
            prefs.set_ante(lockblinds_dlg.ante);
            prefs.set_gametype(lockblinds_dlg.gametype);
            
        }
        else 
		{
            m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_LOCK_BLINDS, false);

			// Tell CScraper about our *not* locked blinds
			SLockBlinds LB;
			LB.blinds_are_locked = false;
			LB.sblind = 0.;
			LB.bblind = 0.;
			LB.bbet = 0.;
			LB.ante = 0.;
			LB.gametype = 0;

            p_scraper->SetLockedBlinds(LB);
        }

    }

    __SEH_LOGFATAL("CMainFrame::OnLockBlinds :\n");
}

void CMainFrame::OnFormulaViewMainToolbar() 
{
    __SEH_HEADER

    if (!m_MainToolBar.IsVisible())
        ShowControlBar(&m_MainToolBar, TRUE, FALSE);

	else
        ShowControlBar(&m_MainToolBar, FALSE, FALSE);

	RecalcLayout();

    __SEH_LOGFATAL("CMainFrame::OnFormulaViewMainToolbar :\n");
}

void CMainFrame::OnFormulaViewFlagsToolbar() 
{
    __SEH_HEADER

    if (!m_FlagsToolBar.IsVisible())
        ShowControlBar(&m_FlagsToolBar, TRUE, FALSE);

	else
        ShowControlBar(&m_FlagsToolBar, FALSE, FALSE);

	RecalcLayout();

    __SEH_LOGFATAL("CMainFrame::OnFormulaViewFlagsToolbar :\n");
}

void CMainFrame::OnFormulaViewStatusbar() 
{
    __SEH_HEADER

    if (!m_wndStatusBar.IsVisible())
        ShowControlBar(&m_wndStatusBar, TRUE, FALSE);

	else
        ShowControlBar(&m_wndStatusBar, FALSE, FALSE);

	RecalcLayout();

    __SEH_LOGFATAL("CMainFrame::OnFormulaViewStatusbar :\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Menu updaters

void CMainFrame::OnUpdateMenuFileNew(CCmdUI* pCmdUI)
{
	__SEH_HEADER

    pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
                    !m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);

	__SEH_LOGFATAL("CMainFrame::")
}

void CMainFrame::OnUpdateMenuFileOpen(CCmdUI* pCmdUI)
{
	__SEH_HEADER

    pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
                    !m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);

	__SEH_LOGFATAL("CMainFrame::OnUpdateMenuFileOpen\n")
}

void CMainFrame::OnUpdateMenuFileLoadProfile(CCmdUI* pCmdUI)
{
	__SEH_HEADER

    pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
                    !m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);

	__SEH_LOGFATAL("CMainFrame::OnUpdateMenuFileLoadProfile\n")
}

void CMainFrame::OnUpdateFileConnect(CCmdUI *pCmdUI)
{
	__SEH_HEADER

    pCmdUI->Enable(!_attached_hwnd);

	__SEH_LOGFATAL("CMainFrame::OnUpdateFileConnect\n")
}

void CMainFrame::OnUpdateMenuDllLoad(CCmdUI* pCmdUI)
{
	__SEH_HEADER

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

	__SEH_LOGFATAL("CMainFrame::OnUpdateMenuDllLoad\n")
}

void CMainFrame::OnUpdateDllLoadspecificfile(CCmdUI *pCmdUI)
{
	__SEH_HEADER

	pCmdUI->Enable(p_dll_extension->IsDllLoaded() ? false : true);

	__SEH_LOGFATAL("CMainFrame::OnUpdateDllLoadspecificfile\n")
}


void CMainFrame::OnUpdatePokerproConnect(CCmdUI *pCmdUI) 
{
	__SEH_HEADER

    pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
                    !m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) &&
                   !p_pokerpro->IsConnected() ? false : true);

	__SEH_LOGFATAL("CMainFrame::OnUpdatePokerproConnect\n")
}


void CMainFrame::OnUpdateViewShootreplayframe(CCmdUI *pCmdUI)
{
	__SEH_HEADER

    pCmdUI->Enable(_attached_hwnd != NULL);

	__SEH_LOGFATAL("CMainFrame::OnUpdateViewShootreplayframe\n")
}

void CMainFrame::OnUpdateViewMainToolbar(CCmdUI *pCmdUI) 
{
	__SEH_HEADER

    pCmdUI->SetCheck(m_MainToolBar.IsVisible() ? 1 : 0);

	__SEH_LOGFATAL("CMainFrame::OnUpdateViewMainToolbar\n")
}

void CMainFrame::OnUpdateViewFlagsToolbar(CCmdUI *pCmdUI) 
{
	__SEH_HEADER

    pCmdUI->SetCheck(m_FlagsToolBar.IsVisible() ? 1 : 0);

	__SEH_LOGFATAL("CMainFrame::OnUpdateViewFlagsToolbar\n")
}

void CMainFrame::OnUpdateViewStatusbar(CCmdUI *pCmdUI) 
{
	__SEH_HEADER

    pCmdUI->SetCheck(m_wndStatusBar.IsVisible() ? 1 : 0);

	__SEH_LOGFATAL("CMainFrame::OnUpdateViewStatusbar\n")
}


//  2008.03.04 by THF
void CMainFrame::OnUpdateMenuPerlLoad(CCmdUI* pCmdUI)
{
	__SEH_HEADER

    if (p_perl->IsAFormulaLoaded()) 
        pCmdUI->SetText("&Unload Formula\tF7");

	else 
        pCmdUI->SetText("&Load Formula\tF7");

    pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
                    !m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);

	__SEH_LOGFATAL("CMainFrame::OnUpdateMenuPerlLoad\n")
}

//  2008.02.07  by THF
void CMainFrame::OnUpdateMenuPerlLoadSpecificFormula(CCmdUI* pCmdUI)
{
	__SEH_HEADER

    pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
                    !m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);

	__SEH_LOGFATAL("CMainFrame::OnUpdateMenuPerlLoadSpecificFormula\n")
}


//  2008.03.20 by THF
void CMainFrame::OnUpdateMenuPerlReloadFormula(CCmdUI* pCmdUI)
{
	__SEH_HEADER

    pCmdUI->Enable((p_perl->IsAFormulaLoaded() &&
					!m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) &&
                    m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)));

	__SEH_LOGFATAL("CMainFrame::OnUpdateMenuPerlReloadFormula\n")
}




BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	__SEH_HEADER

	if (_wait_cursor)
    {
        RestoreWaitCursor();
        return TRUE;
    }

    return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);

	__SEH_LOGFATAL("CMainFrame::OnSetCursor\n")
}


void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	__SEH_HEADER

    if (nID == SC_CLOSE)
    {
        if (m_formulaScintillaDlg)
        {
            if (m_formulaScintillaDlg->m_dirty &&
                    MessageBox("The Formula Editor has un-applied (and unsaved) changes.\n"\
                               "Really exit?", "Unsaved formula warning", MB_YESNO) == IDNO)
            {
                return;
            }
        }
    }
    CFrameWnd::OnSysCommand(nID, lParam);

	__SEH_LOGFATAL("CMainFrame::OnSysCommand\n")
}


//  2008.03.03 by THF
void CMainFrame::OnPerlLoadFormula() 
{
    __SEH_HEADER

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

    __SEH_LOGFATAL("CMainFrame::OnPerlLoadFormula :\n");
}


//  2008.03.03 by THF
void CMainFrame::OnPerlLoadSpecificFormula() 
{
    __SEH_HEADER

    CFileDialog			cfd(true);
    CString				theKey = "DefPLOpenLocation";
    char				path[MAX_PATH];

    ReadRegString(theKey, path);
    cfd.m_ofn.lpstrInitialDir = path;
    cfd.m_ofn.lpstrFilter = "Perl Scripts (*.pl)\0*.pl\0Perl Modules (*.pm)\0*.pm\0All Files (*.*)\0*.*\0\0";
    cfd.m_ofn.lpstrTitle = "Select Perl formula file to OPEN";
    if (cfd.DoModal() == IDOK)
    {
        p_perl->LoadFormulaFile(cfd.m_ofn.lpstrFile);
        WriteRegString(theKey, cfd.GetPathName());
    }

    __SEH_LOGFATAL("CMainFrame::OnPerlLoadSpecificFormula :\n");
}


//  2008.03.03 by THF
void CMainFrame::OnPerlEditMainFormula() 
{
    __SEH_HEADER

    p_perl->EditMainFormulaFile();

    __SEH_LOGFATAL("CMainFrame::OnPerlEditMainFormula :\n");
}


//  2008.03.20 by THF
void CMainFrame::OnPerlReloadFormula() 
{
    __SEH_HEADER

    //  Reload the most recent formula
    //    (This is a shortcut for unload + load.)
    p_perl->ReloadFormulaFile();
	//  Make some noise, as there's no visible reaction
	Beep(880, 125);

    __SEH_LOGFATAL("CMainFrame::OnPerlReloadFormula :\n");
}


void CMainFrame::OnPerlCheckSyntax() 
{
    __SEH_HEADER

    p_perl->CheckSyntax();

    __SEH_LOGFATAL("CMainFrame::OnPerlEditCheckSyntax :\n");
}

//  2008.04.29 by THF
void CMainFrame::OnUpdateMenuPerlCheckSyntax(CCmdUI* pCmdUI) 
{
	__SEH_HEADER	

	pCmdUI->Enable(p_perl->IsAFormulaLoaded());

	__SEH_LOGFATAL("CMainFrame::OnUpdateMenuPerlCheckSyntax")
}


void CMainFrame::OnUpdateMenuPerlEditMainFormula(CCmdUI* pCmdUI) 
{
	__SEH_HEADER

	pCmdUI->Enable(p_perl->IsAFormulaLoaded());

	__SEH_LOGFATAL("CMainFrame::OnUpdateMenuPerlEditMainFormula")
}


void CMainFrame::ReadRegString(CString RegistryKey, char* RegistryValue)
{
    __SEH_HEADER

    HKEY				hKey;
    LONG				result;
    DWORD				strSize = MAX_PATH;

    result = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\OpenHoldem\\OpenHoldem", 0, KEY_READ, &hKey);

    if (result==ERROR_SUCCESS)
	{
        RegQueryValueEx(hKey,RegistryKey,NULL,NULL,(LPBYTE)RegistryValue,&strSize);
    }

    RegCloseKey(hKey);

    __SEH_LOGFATAL("CMainFrame::ReadRegString :\n");
}

void CMainFrame::WriteRegString(CString RegistryKey, CString RegistryValue)
{
    __SEH_HEADER

    HKEY				hKey;
    LONG				result;
    char				str[MAX_PATH];

    result = RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\OpenHoldem\\OpenHoldem", 0, NULL,
                            REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

    if (result==ERROR_SUCCESS)
	{
        sprintf_s(str, 256, "%s", RegistryValue);
        RegSetValueEx(hKey, RegistryKey, 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
    }

    RegCloseKey(hKey);

    __SEH_LOGFATAL("CMainFrame::WriteRegString :\n");
}

CArray <STableList, STableList>		g_tlist; 

BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam) 
{
    CString				title, winclass;
    char				text[512];
    RECT				crect;
    STableList			tablelisthold;
    SWholeMap			*map = (SWholeMap *) (lparam);

    // If this is not a top level window, then return
    if (GetParent(hwnd) != NULL)
        return true;

    // If this window is not visible, then return
    if (!IsWindowVisible(hwnd))
        return true;

    // If there is no caption on this window, then return
    GetWindowText(hwnd, text, sizeof(text));
    if (strlen(text)==0)
        return true;
 
    title = text;

    // Found a candidate window, get client area rect
    GetClientRect(hwnd, &crect);

    // See if it matches the currently loaded table map
    if (check_window_match(map, hwnd, crect, title))
    {
        tablelisthold.hwnd = hwnd;
        tablelisthold.title = title;
        tablelisthold.path = map->filepath;
        tablelisthold.crect.left = crect.left;
        tablelisthold.crect.top = crect.top;
        tablelisthold.crect.right = crect.right;
        tablelisthold.crect.bottom = crect.bottom;
        g_tlist.Add(tablelisthold);
    }

    return true;  // keep processing through entire list of windows
}

bool check_window_match(SWholeMap *map, HWND h, RECT r, CString title) 
{
    __SEH_SET_EXCEPTION_HANDLER


    __SEH_HEADER
    int				i;
    bool			good_size, good_pos_title, good_neg_title, good_table_points;
    int				width, height, x, y;
    HDC				hdcScreen, hdcCompatible;
    HBITMAP			hbmScreen, hOldScreenBitmap;
    BYTE			*pBits, alpha, red, green, blue;
    int				exact_width, exact_height, min_width, min_height, max_width, max_height;
	CTransform		trans;

    exact_width = exact_height = min_width = min_height = max_width = max_height = 0;
    for (i=0; i<(int) map->z$->GetSize(); i++)
    {
        if (map->z$->GetAt(i).name == "clientsize")
        {
            exact_width = map->z$->GetAt(i).width;
            exact_height = map->z$->GetAt(i).height;
        }
        else if (map->z$->GetAt(i).name == "clientsizemin")
        {
            min_width = map->z$->GetAt(i).width;
            min_height = map->z$->GetAt(i).height;
        }
        else if (map->z$->GetAt(i).name == "clientsizemax")
        {
            max_width = map->z$->GetAt(i).width;
            max_height = map->z$->GetAt(i).height;
        }
    }

    // Check for exact match on client size
    good_size = false;
    if (r.right == exact_width && r.bottom == exact_height)
    {
        good_size = true;
    }

    // Check for client size that falls within min/max
    if (min_width != 0 && min_height != 0 && max_width != 0 &&  max_height != 0 &&
            r.right >= (int) min_width && r.right <= (int) max_width &&
            r.bottom >= (int) min_height && r.bottom <= (int) max_height)
    {
        good_size = true;
    }

    if (!good_size)
        return false;

    // Check title text for match
    good_pos_title = false;
    for (i=0; i<(int) map->s$->GetSize(); i++)
    {
        if (map->s$->GetAt(i).name.Left(9) == "titletext"  && title.Find(map->s$->GetAt(i).text)!=-1)
        {
            good_pos_title = true;
            i=(int) map->s$->GetSize()+1;
        }
    }

    if (!good_pos_title)
        return false;

    // Check for no negative title text matches
    good_neg_title = true;
    for (i=0; i<(int) map->s$->GetSize(); i++)
    {
        if (map->s$->GetAt(i).name.Left(10) == "!titletext" && title.Find(map->s$->GetAt(i).text)!=-1)
        {
            good_neg_title = false;
            i=(int) map->s$->GetSize()+1;
        }
    }

    if (!good_neg_title)
        return false;

    // Allocate heap space for BITMAPINFO
    BITMAPINFO	*bmi;
    int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
    bmi = (BITMAPINFO *) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

    // Check table points for match
    width = r.right - r.left;
    height = r.bottom - r.top;
    hdcScreen = GetDC(h);
    hdcCompatible = CreateCompatibleDC(hdcScreen);
    hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);
    hOldScreenBitmap = (HBITMAP) SelectObject(hdcCompatible, hbmScreen);
    BitBlt(hdcCompatible, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    // Populate BITMAPINFOHEADER
    bmi->bmiHeader.biSize = sizeof(bmi->bmiHeader);
    bmi->bmiHeader.biBitCount = 0;
    GetDIBits(hdcCompatible, hbmScreen, 0, 0, NULL, bmi, DIB_RGB_COLORS);

    // Get the actual argb bit information
    bmi->bmiHeader.biHeight = -bmi->bmiHeader.biHeight;
    pBits = new BYTE[bmi->bmiHeader.biSizeImage];
    GetDIBits(hdcCompatible, hbmScreen, 0, height, pBits, bmi, DIB_RGB_COLORS);

    good_table_points = true;
    for (i=0; i<(int) map->r$->GetSize(); i++)
    {
        if (map->r$->GetAt(i).name.Find("tablepoint") != -1 &&
                map->r$->GetAt(i).right - map->r$->GetAt(i).left == 1 &&
                map->r$->GetAt(i).bottom - map->r$->GetAt(i).top == 1 &&
                map->r$->GetAt(i).transform == "C")
        {

            x = map->r$->GetAt(i).left;
            y = map->r$->GetAt(i).top;

            int pbits_loc = y*width*4 + x*4;
            alpha = pBits[pbits_loc + 3];
            red = pBits[pbits_loc + 2];
            green = pBits[pbits_loc + 1];
            blue = pBits[pbits_loc + 0];

            // positive radius
            if (map->r$->GetAt(i).radius >= 0)
            {
                if (!trans.IsInARGBColorCube((map->r$->GetAt(i).color>>24)&0xff,
                                             GetRValue(map->r$->GetAt(i).color),
                                             GetGValue(map->r$->GetAt(i).color),
                                             GetBValue(map->r$->GetAt(i).color),
                                             map->r$->GetAt(i).radius,
                                             alpha, red, green, blue))
                {
                    good_table_points = false;
                    i=(int) map->r$->GetSize()+1;
                }
            }
            // negative radius (logical not)
            else
            {
                if (trans.IsInARGBColorCube((map->r$->GetAt(i).color>>24)&0xff,
                                            GetRValue(map->r$->GetAt(i).color),
                                            GetGValue(map->r$->GetAt(i).color),
                                            GetBValue(map->r$->GetAt(i).color),
                                            -map->r$->GetAt(i).radius,
                                            alpha, red, green, blue))
                {
                    good_table_points = false;
                    i=(int) map->r$->GetSize()+1;
                }
            }

        }
    }

    // Clean up
    HeapFree(GetProcessHeap(), NULL, bmi);
    delete []pBits;
    SelectObject(hdcCompatible, hOldScreenBitmap);
    DeleteObject(hbmScreen);
    DeleteDC(hdcCompatible);
    ReleaseDC(h, hdcScreen);

    if (!good_table_points)
        return false;

    return true;

    __SEH_LOGFATAL("::check_window_match :\n");
}
