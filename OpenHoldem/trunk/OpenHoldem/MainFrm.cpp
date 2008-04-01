// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <process.h>

#include "OpenHoldem.h"
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
//  2008.03.02 by THF
#include "DialogSAPrefs9.h"
#include "DialogSAPrefs10.h"
#include <windows.h>

#include "registry.h"
#include "debug.h"
#include "DialogSelectTable.h"
#include "scraper.h"
#include "tablemap.h"
#include "threads.h"
#include "global.h"
#include "symbols.h"
#include "inlines/eval.h"
#include "dll_extension.h"
#include "PokerPro.h"
#include "DialogPpro.h"
#include "DialogScraperOutput.h"
#include "DialogLockBlinds.h"
#include "pokertracker.h"
#include "Perl.hpp"

#include "MainFrm.h"

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

	//  Menu commands	
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
	ON_WM_SETCURSOR()
	ON_WM_SYSCOMMAND()
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOOTREPLAYFRAME, &CMainFrame::OnUpdateViewShootreplayframe)
END_MESSAGE_MAP()

static UINT indicators[] = {
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
CMainFrame::CMainFrame() {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif
	
#ifdef SEH_ENABLE
	try {
#endif
		// Save startup directory
		::GetCurrentDirectory(sizeof(global.startup_path) - 1, global.startup_path);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::Constructor :\n"); 
		throw;
	}
#endif
}

CMainFrame::~CMainFrame() {
#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::Destructor :\n"); 
		throw;
	}
#endif
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
#ifdef SEH_ENABLE
	try {
#endif
		CString			t;
		
		if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
			return -1;
		}

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
		SetTimer(ENABLE_BUTTONS_TIMER, 200, 0);

		// Start timer that updates status bar
		SetTimer(UPDATE_STATUS_BAR_TIMER, 200, 0);

		// Start timer that attaches the OH window when the poker window moves
		SetTimer(ATTACH_WINDOW_TIMER, 20, 0);

		return 0;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnCreate :\n"); 
		throw;
	}
#endif
}

int CMainFrame::create_main_toolbar(void) {
#ifdef SEH_ENABLE
	try {
#endif
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

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::create_main_toolbar :\n"); 
		throw;
	}
#endif
}

int CMainFrame::create_flags_toolbar(void) {
#ifdef SEH_ENABLE
	try {
#endif
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
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::create_flags_toolbar :\n"); 
		throw;
	}
#endif
}


void CMainFrame::align_toolbars(void) {
#ifdef SEH_ENABLE
	try {
#endif
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
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::align_toolbars :\n"); 
		throw;
	}
#endif
}

int CMainFrame::create_status_bar(void) {
#ifdef SEH_ENABLE
	try {
#endif
		if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))	{
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

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::create_status_bar :\n"); 
		throw;
	}
#endif
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
#ifdef SEH_ENABLE
	try {
#endif
		if( !CFrameWnd::PreCreateWindow(cs) )
			return FALSE;

		Registry	reg;
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
		reg.read_reg();
		max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
		max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
		cs.x = min(reg.main_x, max_x);
		cs.y = min(reg.main_y, max_y);
		cs.cx = reg.main_dx;
		cs.cy = reg.main_dy;

		return true;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::PreCreateWindow :\n"); 
		throw;
	}
#endif
}

// CMainFrame message handlers

void CMainFrame::OnEditFormula() {
#ifdef SEH_ENABLE
	try {
#endif

		if(m_formulaScintillaDlg) {
			BOOL	bWasShown = ::IsWindow(m_formulaScintillaDlg->m_hWnd) && m_formulaScintillaDlg->IsWindowVisible();

			if (!m_formulaScintillaDlg->DestroyWindow()) 
			{
				m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, true);
				return; // The Formula user requested to not have the dialog close
			}
			delete m_formulaScintillaDlg;
			m_formulaScintillaDlg = NULL;

			if(bWasShown)
			return;
		}

		m_formulaScintillaDlg = new CDlgFormulaScintilla(this);
		m_formulaScintillaDlg->Create(CDlgFormulaScintilla::IDD,this);
		m_formulaScintillaDlg->ShowWindow(SW_SHOW);
		m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_FORMULA, true);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnEditFormula :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnScraperOutput() {
#ifdef SEH_ENABLE
	try {
#endif

		if(m_ScraperOutputDlg) {
			BOOL	bWasShown = ::IsWindow(m_ScraperOutputDlg->m_hWnd) && m_ScraperOutputDlg->IsWindowVisible();

			m_ScraperOutputDlg->DestroyWindow();
			delete m_ScraperOutputDlg;
			m_ScraperOutputDlg = NULL;

			if(bWasShown)
			return;
		}

		m_ScraperOutputDlg = new CDlgScraperOutput(this);
		m_ScraperOutputDlg->Create(CDlgScraperOutput::IDD,this);
		m_ScraperOutputDlg->ShowWindow(SW_SHOW);
		m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, true);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnScraperOutput :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnViewShootreplayframe()
{
	global.create_replay_frame();
}

void CMainFrame::OnEditPreferences() {
#ifdef SEH_ENABLE
	try {
#endif
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

		// add pages
		// 2008.03.02 by THF: Perl, Chat, alphabetical order 
		dlg.AddPage(page1, "Analyzer");
		dlg.AddPage(page2, "Autoplayer");
		dlg.AddPage(page10, "Chat");
		dlg.AddPage(page3, "DLL Extension");		
		dlg.AddPage(page7, "ICM");
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

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnEditPreferences :\n"); 
		throw;
	}
#endif
}

BOOL CMainFrame::DestroyWindow() {
#ifdef SEH_ENABLE
	try {
#endif
		bool			upd = true;
		int				updcount=0;
		Registry		reg;
		WINDOWPLACEMENT wp;

		// wait for an update cycle to finish, if necessary
		while (upd && updcount<20) 
		{
			EnterCriticalSection(&cs_updater);
			upd = global.update_in_process;
			LeaveCriticalSection(&cs_updater);
			Sleep(100);
			updcount++;
		}

		// Stop heartbeat thread
		if (heartbeat_thread_alive) {
			heartbeat_thread_alive = false;
			WaitForSingleObject(h_heartbeat_thread, THREAD_WAIT);
		}

		// Stop prwin and pokertracker threads
		if (prwin_thread_alive) 
		{
			EnterCriticalSection(&cs_prwin);
			prwin_thread_alive = false;
			LeaveCriticalSection(&cs_prwin);
		}
		if (pokertracker_thread_alive)  pokertracker_thread_alive = false;

		// Wait for threads to finish
		HANDLE handles[2];
		handles[0] = h_prwin_thread;
		handles[1] = h_pokertracker_thread;
		WaitForMultipleObjects(2, handles, true, THREAD_WAIT);

		// Save window position
		reg.read_reg();
		if (!m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_MINMAX)) {
			GetWindowPlacement(&wp);

			reg.main_x = wp.rcNormalPosition.left;
			reg.main_y = wp.rcNormalPosition.top;
			reg.main_dx = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
			reg.main_dy = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
		}
		else {
			reg.main_x = table_view_size.left;
			reg.main_y = table_view_size.top;
			reg.main_dx = table_view_size.right - table_view_size.left;
			reg.main_dy = table_view_size.bottom - table_view_size.top;
		}
		reg.write_reg();

		return CFrameWnd::DestroyWindow();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::DestroyWindow :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnFileLoadTableMap() {
#ifdef SEH_ENABLE
	try {
#endif
		CFileDialog			cfd(true);
		int					line, ret;
		CString				e;

		cfd.m_ofn.lpstrFilter = "OpenScrape Table Maps (.tm)\0*.tm\0All files (*.*)\0*.*\0\0";
		cfd.m_ofn.lpstrTitle = "Select OpenScrape table map to OPEN";

		if (cfd.DoModal() == IDOK) 
		{
			ret = load_tablemap(cfd.m_ofn.lpstrFile, &global.tablemap, VER_OPENSCRAPE_1, false, &line);
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
				global.save_r$indexes();
				global.save_s$indexes();
				global.save_s$strings();

				// Reset "ScraperOutput" dialog, if it is live
				if(m_ScraperOutputDlg) {
					m_ScraperOutputDlg->add_listbox_items();
					m_ScraperOutputDlg->do_update_display();
				}

			}
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnFileLoadTableMap :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnBnClickedGreenCircle() {
#ifdef SEH_ENABLE
	try {
#endif
		int								i, N, line;
		CDlgSelectTable					cstd;
		STableList						tablelisthold;
		int								result;
		char							title[512];
		CFileFind						hFile;
		CString							path, filename;
		BOOL							bFound;
		s_tablemaps						maps;
		int								last_frame_num, frame_num;
		CTime							time, latest_time;

		// Clear global list for holding table candidates
		global.g_tlist.RemoveAll();

		////////////////////////////////////////////////////
		// Load all profiles in scraper directory into array
		maps.n_elem = 0;
		
		if (global.tablemap.valid && maps.n_elem<MAX_MAPS_IN_STABLEMAPS)
			if (load_tablemap((char *) global.tablemap.filepath.GetString(), &maps.tablemap[maps.n_elem], VER_OPENSCRAPE_1, false, &line) == SUCCESS)
				maps.n_elem++;

		// WinScrape converted profiles
		path.Format("%s\\scraper\\*.ws", global.startup_path);
		bFound = hFile.FindFile(path.GetString());
		while (bFound)
		{
			bFound = hFile.FindNextFile();
			if(!hFile.IsDots() && !hFile.IsDirectory() && hFile.GetFilePath()!=global.tablemap.filepath && maps.n_elem<MAX_MAPS_IN_STABLEMAPS) 
				if (load_tablemap((char *) hFile.GetFilePath().GetString(), &maps.tablemap[maps.n_elem], VER_OPENSCRAPE_1, false, &line) == SUCCESS)
					maps.n_elem++;
		}

		// OpenScrape table maps
		path.Format("%s\\scraper\\*.tm", global.startup_path);
		bFound = hFile.FindFile(path.GetString());
		while (bFound) 
		{
			bFound = hFile.FindNextFile();
			if(!hFile.IsDots() && !hFile.IsDirectory() && hFile.GetFilePath()!=global.tablemap.filepath && maps.n_elem<MAX_MAPS_IN_STABLEMAPS)
				if (load_tablemap((char *) hFile.GetFilePath().GetString(), &maps.tablemap[maps.n_elem], VER_OPENSCRAPE_1, false, &line) == SUCCESS)
					maps.n_elem++;
		}

		////////////////////////////////////////////////
		// Populate global candidate table list
		EnumWindows(EnumProcTopLevelWindowList, (LPARAM) &maps);

		// Put global candidate table list in table select dialog variables
		N = (int) global.g_tlist.GetSize();
		if (N==0) {
			int cySize = GetSystemMetrics(SM_CYSIZE);
			int cyMenuSize = GetSystemMetrics(SM_CYMENU);
			if (cySize != 18 && cyMenuSize != 19)
				MessageBox("Cannot find table\n\nIt appears that your settings are not configured according to OpenHoldem specifications.\nYou must ensure that XP themes are not used (Use Windows Classic style) and\nfont size is set to normal.\n\nFor more info, look at the wiki documentation and the user forums", "Cannot find table", MB_OK);
			else
				MessageBox("No valid tables found", "Cannot find table", MB_OK);
		}
		else {
			if (N==1) {
				result = IDOK;
				cstd.selected_item = 0;
			}
			else if (N>=2) {
				for (i=0; i<N; i++) {
					tablelisthold.hwnd = global.g_tlist[i].hwnd;
					tablelisthold.title = global.g_tlist[i].title;
					tablelisthold.path = global.g_tlist[i].path;
					tablelisthold.crect.left = global.g_tlist[i].crect.left;
					tablelisthold.crect.top = global.g_tlist[i].crect.top;
					tablelisthold.crect.right = global.g_tlist[i].crect.right;
					tablelisthold.crect.bottom = global.g_tlist[i].crect.bottom;
					cstd.tlist.Add(tablelisthold);
				}

				// Display table select dialog
				result = cstd.DoModal();
			}

			if (result == IDOK) {
				// Load correct profile, and save hwnd/rect/numchairs of table that we are "attached" to
				global.attached_hwnd = global.g_tlist[cstd.selected_item].hwnd;
				global.attached_rect.left = global.g_tlist[cstd.selected_item].crect.left;
				global.attached_rect.top = global.g_tlist[cstd.selected_item].crect.top;
				global.attached_rect.right = global.g_tlist[cstd.selected_item].crect.right;
				global.attached_rect.bottom = global.g_tlist[cstd.selected_item].crect.bottom;
				load_tablemap((char *) global.g_tlist[cstd.selected_item].path.GetString(), &global.tablemap, VER_OPENSCRAPE_1, false, &line);
				global.save_r$indexes();
				global.save_s$indexes();
				global.save_s$strings();

				// Create bitmaps
				scraper.create_bitmaps();

				// Clear scraper fields
				scraper.clear_scrape_areas();

				// Reset symbols
				symbols.ResetSymbolsFirstTime();

				// Reset "ScraperOutput" dialog, if it is live
				if(m_ScraperOutputDlg) {
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
				EnterCriticalSection(&cs_heartbeat);
				global.autoplay = false;
				LeaveCriticalSection(&cs_heartbeat);

				// mark symbol result cache as stale
				N = (int) global.formula.mFunction.GetSize();
				for (i=0; i<N; i++) 
				{
					global.formula.mFunction[i].fresh = false;
				}
				
				// Find next replay frame number
				latest_time = 0;
				global.next_replay_frame = last_frame_num = -1;
				path.Format("%s\\replay\\session%d\\*.bmp", global.startup_path, global.sessionnum);
				bFound = hFile.FindFile(path.GetString());
				while (bFound)
				{
					bFound = hFile.FindNextFile();
					if(!hFile.IsDots() && !hFile.IsDirectory()) 
					{
						filename = hFile.GetFileName();
						hFile.GetLastWriteTime(time);
						sscanf(filename.GetString(), "frame%d.bmp", &frame_num);

						if (time>latest_time)
						{
							last_frame_num = frame_num;
							latest_time = time;
						}
					}
				}

				global.next_replay_frame = last_frame_num + 1;
				if (global.next_replay_frame >= global.preferences.replay_max_frames)
					global.next_replay_frame = 0;

				// start heartbeat
				heartbeat_thread_alive = true;
				h_heartbeat_thread = AfxBeginThread(heartbeat_thread, 0);

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
				::GetWindowText(global.attached_hwnd, title, 512);
				write_log("\n*************************************************************\nTABLE RESET %s - %s(%s)\n*************************************************************\n", 
					global.formula_name.GetString(), global.tablemap.sitename.GetString(), title);
				
			}
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnBnClickedGreenCircle :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnBnClickedRedCircle() {
#ifdef SEH_ENABLE
	try {
#endif
		int			updcount;
		bool		upd = true;

		// wait for an update cycle to finish, if necessary
		updcount = 0;
		while (upd && updcount<20) {
			EnterCriticalSection(&cs_updater);
			upd = global.update_in_process;
			LeaveCriticalSection(&cs_updater);
			Sleep(100);
			updcount++;
		}

		// Stop heartbeat thread
		if (heartbeat_thread_alive) {
			heartbeat_thread_alive = false;
			WaitForSingleObject(h_heartbeat_thread, THREAD_WAIT);
		}

		// Stop prwin and pokertracker threads
		if (prwin_thread_alive) 
		{
			EnterCriticalSection(&cs_prwin);
			prwin_thread_alive = false;
			LeaveCriticalSection(&cs_prwin);
		}
		if (pokertracker_thread_alive)  pokertracker_thread_alive = false;

		// Wait for threads to finish
		HANDLE handles[2];
		handles[0] = h_prwin_thread;
		handles[1] = h_pokertracker_thread;
		WaitForMultipleObjects(2, handles, true, THREAD_WAIT);

		// Make sure autoplayer is off
		global.autoplay = false;
		global.auto_autoplay_pressed = false;

		// Clear "attached" info
		global.attached_hwnd = NULL;
		global.attached_rect.left = 0;
		global.attached_rect.top = 0;
		global.attached_rect.right = 0;
		global.attached_rect.bottom = 0;

		// Stop timer that checks for valid hwnd
		KillTimer(HWND_CHECK_TIMER);

		// Unattach OH from any poker window
		if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_TOP)) {
			m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_ATTACH_TOP, false);
		}
		if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_BOTTOM)) {
			m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, false);
		}

		// Reset toolbar/menu button state
		m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_GREENCIRCLE, true);
		m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_REDCIRCLE, false);
		m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
		m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
		m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_NEW, true);
		m_MainToolBar.GetToolBarCtrl().EnableButton(ID_FILE_OPEN, true);

		// Delete bitmaps
		scraper.delete_bitmaps();

		// Clear profile indices
		global.clear_r$indexes();

		// Clear scraper fields
		scraper.clear_scrape_areas();

		// Reset symbols
		symbols.ResetSymbolsFirstTime();

		// Change window title
		SetWindowText(global.formula_name + " - " + m_strTitle);

		// Reset Display
		InvalidateRect(NULL, false);

		// Reset "ScraperOutput" dialog, if it is live
		if(m_ScraperOutputDlg) 
		{
			m_ScraperOutputDlg->add_listbox_items();
			m_ScraperOutputDlg->do_update_display();
		}

		// log OH title bar text and table reset
		write_log("%s - %s(NOT ATTACHED)\n", global.formula_name.GetString(), global.tablemap.sitename.GetString());
		write_log("TABLE RESET\n*************************************************************\n");

		// Stop logging
		stop_log();

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnBnClickedRedCircle :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnTimer(UINT nIDEvent) {
#ifdef SEH_ENABLE
	try {
#endif
		CardMask	Cards;
		int			nCards;
		HandVal		hv;
		char		hvstring[100];
		int			i;
		bool		prwin_running, scrape_running;
		char		*card;
		CString		temp;
		int			cur_prwin_iterator_progress;
		bool		cur_prwin_thread_complete;
		bool		playing;
		RECT		att_rect, wrect;
		static RECT	prev_att_rect={0}, prev_wrect={0};

		if (nIDEvent == HWND_CHECK_TIMER) 
		{
			if (!IsWindow(global.attached_hwnd)) 
			{
				OnBnClickedRedCircle();
			}
		}

		else if (nIDEvent == ENABLE_BUTTONS_TIMER) 
		{
			// Autoplayer
			if ((symbols.user_chair_confirmed && m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE)) ||
				 ppro.data.m_userchair!=-1)
			{
				m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, true);
			}
			else 
			{
				m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
			}

			// Automatically start autoplayer, if set in preferences
			if (global.preferences.ap_auto && !m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_AUTOPLAYER) &&
				((symbols.user_chair_confirmed && m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE)) ||
				 ppro.data.m_userchair!=-1))
			{
				if (!global.auto_autoplay_pressed)
				{
					m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, true);
					OnAutoplayer();
					global.auto_autoplay_pressed = true;
				}
			}

			// attach
			if (global.attached_hwnd != NULL) {
				m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_ATTACH_TOP, true);
				m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, true);
			}
			else {
				m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_ATTACH_TOP, false);
				m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, false);
			}

			// Shoot replay frame
			if (global.attached_hwnd != NULL)
				m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, true);
			else
				m_MainToolBar.GetToolBarCtrl().EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, false);

		}

		else if (nIDEvent == UPDATE_STATUS_BAR_TIMER) {
			// Don't update certain items if prwin iterator or scrape/symbol calc is in progress
			EnterCriticalSection(&cs_prwin);
			prwin_running = prwin_thread_alive;
			LeaveCriticalSection(&cs_prwin);
			EnterCriticalSection(&cs_updater);
			scrape_running = global.update_in_process;
			LeaveCriticalSection(&cs_updater);

			// Figure out if I am "notplaying"
			if (scraper.card_player[(int) symbols.sym.userchair][0] == CARD_BACK ||
				scraper.card_player[(int) symbols.sym.userchair][1] == CARD_BACK ||
				scraper.card_player[(int) symbols.sym.userchair][0] == CARD_NOCARD ||
				scraper.card_player[(int) symbols.sym.userchair][1] == CARD_NOCARD) 
			{
				symbols.sym.playing = playing = false; //2008-03-25 Matrix
			}
			else 
			{
				symbols.sym.playing = playing = true;
			}

			// Only update these things if we are not in the middle of a heartbeat update, so as to
			// avoid flakey information in the display
			if (!scrape_running) 
			{
				CardMask_RESET(Cards);
				nCards=0;

				// Player cards
				status_plcards = "";
				if (symbols.user_chair_confirmed && playing) {
					for (i=0; i<=1; i++) {
						// player cards
						if (scraper.card_player[(int) symbols.sym.userchair][i] != CARD_BACK &&
							scraper.card_player[(int) symbols.sym.userchair][i] != CARD_NOCARD) {
							card = StdDeck_cardString(scraper.card_player[(int) symbols.sym.userchair][i]);
							temp.Format("%s ", card);
							status_plcards.Append(temp);
							CardMask_SET(Cards, scraper.card_player[(int) symbols.sym.userchair][i]);
							nCards++;
						}
					}
				}
				else {
					if (scraper.card_player_for_display[0] != CARD_BACK && scraper.card_player_for_display[0] != CARD_NOCARD) {
						card = StdDeck_cardString(scraper.card_player_for_display[0]);
						temp.Format("%s ", card);
						status_plcards.Append(temp);
						CardMask_SET(Cards, scraper.card_player_for_display[0]);
						nCards++;
					}
					if (scraper.card_player_for_display[1] != CARD_BACK && scraper.card_player_for_display[1] != CARD_NOCARD) {
						card = StdDeck_cardString(scraper.card_player_for_display[1]);
						temp.Format("%s ", card);
						status_plcards.Append(temp);
						CardMask_SET(Cards, scraper.card_player_for_display[1]);
						nCards++;
					}
				}

				// Common cards
				status_comcards = "";
				for (i=0; i<=4; i++) {
					if (scraper.card_common[i] != CARD_BACK &&
						scraper.card_common[i] != CARD_NOCARD) {
						card = StdDeck_cardString(scraper.card_common[i]);
						temp.Format("%s ", card);
						status_comcards.Append(temp);
						CardMask_SET(Cards, scraper.card_common[i]);
						nCards++;
					}
				}

				// poker hand
				hv = Hand_EVAL_N(Cards, nCards);
				HandVal_toString(hv, hvstring);
				status_pokerhand = hvstring;
				status_pokerhand = status_pokerhand.Mid(0, status_pokerhand.Find(" "));

				// handrank
				if (global.preferences.handrank_value == "169") {
					status_handrank.Format("%.0f/169", symbols.sym.handrank169);
				}
				else if (global.preferences.handrank_value == "1000") {
					status_handrank.Format("%.0f/1000", symbols.sym.handrank1000);
				}
				else if (global.preferences.handrank_value == "1326") {
					status_handrank.Format("%.0f/1326", symbols.sym.handrank1326);
				}
				else if (global.preferences.handrank_value == "2652") {
					status_handrank.Format("%.0f/2652", symbols.sym.handrank2652);
				}
				else if (global.preferences.handrank_value == "p") {
					status_handrank.Format("%.2f/2652", symbols.sym.handrankp);
				}

				// nopponents
				if (playing) {
					status_nopp.Format("%d", (int) symbols.sym.nopponents);
				}
				else {
					status_nopp = "";
				}
			}

			// Always update prwin/nit
			// prwin/nit
			EnterCriticalSection(&cs_prwin);
			cur_prwin_iterator_progress = prwin_iterator_progress;
			cur_prwin_thread_complete = prwin_thread_complete;
			LeaveCriticalSection(&cs_prwin);
			if (symbols.user_chair_confirmed && playing) 
			{
				EnterCriticalSection(&cs_prwin);
				status_prwin.Format("%d/%d/%d", (int) (symbols.sym.prwin*1000), (int) (symbols.sym.prtie*1000), 
					(int) (symbols.sym.prlos*1000));
				status_nit.Format("%d/%d", cur_prwin_iterator_progress, (int) global.formula.dNit);
				LeaveCriticalSection(&cs_prwin);
			}
			else 
			{
				status_prwin = "0/0/0";
				status_nit.Format("0/%d", (int) global.formula.dNit);
			}

			// action
			if (!symbols.user_chair_confirmed || !playing)
				status_action = "Notplaying";

			else if (symbols.user_chair_confirmed && cur_prwin_thread_complete)
			{
				if (symbols.f$alli)  status_action = "Allin";
				else if (symbols.f$swag)  status_action.Format("SWAG: %.2f", symbols.f$swag);
				else if (symbols.f$rais)  status_action = "Bet/Raise";
				else if (symbols.f$call)  status_action = "Call/Check";
				else if (symbols.f$prefold)  status_action = "Pre-fold";
				else  status_action = "Fold/Check";
			}

			else if (symbols.sym.nopponents==0)
				status_action = "Idle (f$P==0)";

			else
				status_action = "thinking";

		}

		else if (nIDEvent == ATTACH_WINDOW_TIMER) 
		{
			::GetWindowRect(global.attached_hwnd, &att_rect);
			GetWindowRect(&wrect);

			if (memcmp(&prev_att_rect, &att_rect, sizeof(RECT))!=0 ||
				memcmp(&prev_wrect, &wrect, sizeof(RECT))!=0) 
			{
				::GetWindowRect(global.attached_hwnd, &prev_att_rect);
				GetWindowRect(&prev_wrect);

				if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_TOP)) 
				{
					if (IsWindow(global.attached_hwnd))
						MoveWindow(att_rect.left, att_rect.top-(wrect.bottom-wrect.top), att_rect.right-att_rect.left, wrect.bottom-wrect.top);
				}

				if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_BOTTOM)) 
				{
					if (IsWindow(global.attached_hwnd))
						MoveWindow(att_rect.left, att_rect.bottom, att_rect.right-att_rect.left, wrect.bottom-wrect.top);
				}
			}
		}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnTimer :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnClickedFlags() {
#ifdef SEH_ENABLE
	try {
#endif
		if (m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER0)) { global.flags[0] = true; }
		else { global.flags[0] = false; }
		if (m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER1)) { global.flags[1] = true; }
		else { global.flags[1] = false; }
		if (m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER2)) { global.flags[2] = true; }
		else { global.flags[2] = false; }
		if (m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER3)) { global.flags[3] = true; }
		else { global.flags[3] = false; }
		if (m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER4)) { global.flags[4] = true; }
		else { global.flags[4] = false; }
		if (m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER5)) { global.flags[5] = true; }
		else { global.flags[5] = false; }
		if (m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER6)) { global.flags[6] = true; }
		else { global.flags[6] = false; }
		if (m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER7)) { global.flags[7] = true; }
		else { global.flags[7] = false; }
		if (m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER8)) { global.flags[8] = true; }
		else { global.flags[8] = false; }
		if (m_FlagsToolBar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER9)) { global.flags[9] = true; }
		else { global.flags[9] = false; }
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnClickedFlags :\n"); 
		throw;
	}
#endif
}


void CMainFrame::OnAutoplayer() {
#ifdef SEH_ENABLE
	try {
#endif
		CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	
		if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_AUTOPLAYER)) {
			// calc hand lists
			global.create_hand_list_matrices(&global.formula);

			// one last parse - do not engage if parse fails
			if (global.ParseAllFormula(pMyMainWnd->GetSafeHwnd(), &global.formula)) {
				EnterCriticalSection(&cs_heartbeat);
				global.autoplay = true;
				LeaveCriticalSection(&cs_heartbeat);
			}
			else {
				EnterCriticalSection(&cs_heartbeat);
				global.autoplay = false;
				LeaveCriticalSection(&cs_heartbeat);
				m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
			}
		}
		else {
			EnterCriticalSection(&cs_heartbeat);
			global.autoplay = false;
			LeaveCriticalSection(&cs_heartbeat);
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnAutoplayer :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnUpdateStatus(CCmdUI *pCmdUI) {
#ifdef SEH_ENABLE
	try {
#endif
		m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_PLCARDS), status_plcards);
		m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_COMCARDS), status_comcards);
		m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_POKERHAND), status_pokerhand);
		m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_HANDRANK), status_handrank);
		m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_PRWIN), status_prwin);
		m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_NOPP), status_nopp);
		m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_NIT), status_nit);
		m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS_ACTION), status_action);
		// if action==bet ... m_wndStatusBar.GetStatusBarCtrl().SetBkColor(RGB(180,180,180));
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnUpdateStatus :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnDllLoad() {
#ifdef SEH_ENABLE
	try {
#endif
		if (cdll.hMod_dll!=NULL) {
			cdll.unload_dll();
		}
		else 
		{
			cdll.load_dll("");
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnDllLoad :\n"); 
		throw;
	}
#endif
}


void CMainFrame::OnDllLoadspecificfile()
{
#ifdef SEH_ENABLE
	try {
#endif
		CFileDialog			cfd(true);

		cfd.m_ofn.lpstrFilter = "DLL Files (.dll)\0*.dll\0\0";
		cfd.m_ofn.lpstrTitle = "Select OpenHoldem DLL file to OPEN";

		if (cfd.DoModal() == IDOK) {
			if (cdll.hMod_dll!=NULL) {
				cdll.unload_dll();
			}
			else 
			{
				cdll.load_dll(cfd.m_ofn.lpstrFile);
			}
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnDllLoadspecificfile :\n"); 
		throw;
	}
#endif
}


void CMainFrame::OnPokerproConnect() {
#ifdef SEH_ENABLE
	try {
#endif
		if(m_pproDlg) {
			BOOL	bWasShown	=	::IsWindow(m_pproDlg->m_hWnd) && m_pproDlg->IsWindowVisible();

			m_pproDlg->DestroyWindow();
			delete m_pproDlg;
			m_pproDlg = NULL;

			if(bWasShown) { return; }
		}
		m_pproDlg = new CDlgPpro(this);
		m_pproDlg->Create(CDlgPpro::IDD,this);
		m_pproDlg->ShowWindow(SW_SHOW);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnPokerproConnect :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnMinMax(void) {
#ifdef SEH_ENABLE
	try {
#endif
		RECT		crect, wrect, rectBar1, rectBar2, statusBar;
		POINT		pt;
		int			tb_top, tb_bottom;
		GetClientRect(&crect);
		GetWindowRect(&wrect);


		if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_MINMAX)) {
			GetWindowRect(&table_view_size);
			m_MainToolBar.GetWindowRect(&rectBar1);
			m_FlagsToolBar.GetWindowRect(&rectBar2);
			m_wndStatusBar.GetWindowRect(&statusBar);

			// figure out size of toolbars
			if (m_MainToolBar.IsFloating() && m_FlagsToolBar.IsFloating()) {
				tb_top = 0;
				tb_bottom = 0;
			}
			else if (!m_MainToolBar.IsFloating() && m_FlagsToolBar.IsFloating()) {
				tb_top = rectBar1.top;
				tb_bottom = rectBar1.bottom;
			}
			else if (m_MainToolBar.IsFloating() && !m_FlagsToolBar.IsFloating()) {
				tb_top = rectBar2.top;
				tb_bottom = rectBar2.bottom;
			}
			else {
				if (rectBar1.top <= rectBar2.top) { tb_top = rectBar1.top; }
				else { tb_top = rectBar2.top; }
				if (rectBar1.bottom >= rectBar2.bottom) { tb_bottom = rectBar1.bottom; }
				else { tb_bottom = rectBar2.bottom; }
			}
		
			pt.x = (wrect.right - wrect.left) - crect.right;
			pt.y = (wrect.bottom - wrect.top) - crect.bottom;

			MoveWindow(wrect.left, wrect.top, wrect.right - wrect.left, 
					   pt.y + (tb_bottom-tb_top) + (statusBar.bottom-statusBar.top) - 2, 
					   true);
		}
		else {
			MoveWindow(table_view_size.left, table_view_size.top, 
					   table_view_size.right - table_view_size.left, 
					   table_view_size.bottom - table_view_size.top, true);
		}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnMinMax :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnAttachTop(void) {
#ifdef SEH_ENABLE
	try {
#endif
		RECT	att_rect, wrect;
		::GetWindowRect(global.attached_hwnd, &att_rect);
		GetWindowRect(&wrect);

		if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_TOP)) {
			// uncheck attach_bottom, if necessary
			m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, false);
			
			MoveWindow(att_rect.left, att_rect.top-(wrect.bottom-wrect.top), att_rect.right-att_rect.left, wrect.bottom-wrect.top);
		}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnAttachTop :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnAttachBottom(void){
#ifdef SEH_ENABLE
	try {
#endif
		RECT	att_rect, wrect;
		::GetWindowRect(global.attached_hwnd, &att_rect);
		GetWindowRect(&wrect);

		if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_BOTTOM)) {
			// uncheck attach_top, if necessary
			m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_ATTACH_TOP, false);

			MoveWindow(att_rect.left, att_rect.bottom, att_rect.right-att_rect.left, wrect.bottom-wrect.top);
		}
	
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnAttachBottom :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnLockBlinds(void){
#ifdef SEH_ENABLE
	try {
#endif
		CDlgLockBlinds	lockblinds_dlg;
		Registry reg;

		if (m_MainToolBar.GetToolBarCtrl().IsButtonChecked(ID_MAIN_TOOLBAR_LOCK_BLINDS)) {
			reg.read_reg();

			lockblinds_dlg.sblind = reg.sblind;
			lockblinds_dlg.bblind = reg.bblind;
			lockblinds_dlg.bbet = reg.bbet;
			lockblinds_dlg.ante = reg.ante;
			lockblinds_dlg.gametype = reg.gametype;

			if (lockblinds_dlg.DoModal() == IDOK) {
				scraper.locked_sblind = lockblinds_dlg.sblind;
				scraper.locked_bblind = lockblinds_dlg.bblind;
				scraper.locked_bbet = lockblinds_dlg.bbet;
				scraper.locked_ante = lockblinds_dlg.ante;
				scraper.locked_gametype = lockblinds_dlg.gametype;
				scraper.blinds_are_locked = true;

				reg.read_reg();
				reg.sblind = lockblinds_dlg.sblind;
				reg.bblind = lockblinds_dlg.bblind;
				reg.bbet = lockblinds_dlg.bbet;
				reg.ante = lockblinds_dlg.ante;
				reg.gametype = lockblinds_dlg.gametype;
				reg.write_reg();

			}
			else {
				m_MainToolBar.GetToolBarCtrl().CheckButton(ID_MAIN_TOOLBAR_LOCK_BLINDS, false);
				scraper.blinds_are_locked = false;
			}

		}
	
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnLockBlinds :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnFormulaViewMainToolbar() {
#ifdef SEH_ENABLE
	try {
#endif
		if (!m_MainToolBar.IsVisible()) {
			ShowControlBar(&m_MainToolBar, TRUE, FALSE);
		}
		else {
			ShowControlBar(&m_MainToolBar, FALSE, FALSE);
		}
		RecalcLayout();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnFormulaViewMainToolbar :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnFormulaViewFlagsToolbar() {
#ifdef SEH_ENABLE
	try {
#endif
		if (!m_FlagsToolBar.IsVisible()) {
			ShowControlBar(&m_FlagsToolBar, TRUE, FALSE);
		}
		else {
			ShowControlBar(&m_FlagsToolBar, FALSE, FALSE);
		}
		RecalcLayout();

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnFormulaViewFlagsToolbar :\n"); 
		throw;
	}
#endif
}

void CMainFrame::OnFormulaViewStatusbar() {
#ifdef SEH_ENABLE
	try {
#endif
		if (!m_wndStatusBar.IsVisible()) {
			ShowControlBar(&m_wndStatusBar, TRUE, FALSE);
		}
		else {
			ShowControlBar(&m_wndStatusBar, FALSE, FALSE);
		}
		RecalcLayout();

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnFormulaViewStatusbar :\n"); 
		throw;
	}
#endif
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
	pCmdUI->Enable(!global.attached_hwnd);
}

void CMainFrame::OnUpdateMenuDllLoad(CCmdUI* pCmdUI) 
{
	if (cdll.hMod_dll!=NULL) {
		pCmdUI->SetText("&Unload\tF4");
	}
	else {
		pCmdUI->SetText("&Load\tF4");
	}
	// Not connected to ppro server
	if (ppro.m_socket==INVALID_SOCKET) {
		pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
						!m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);
	}
	// connected to ppro server
	else {
		pCmdUI->Enable(ppro.data.m_pinf[ppro.data.m_userchair].m_isActive&0x1 ? false : true);
	}
}

void CMainFrame::OnUpdateDllLoadspecificfile(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(cdll.hMod_dll ? false : true);
}


void CMainFrame::OnUpdatePokerproConnect(CCmdUI *pCmdUI) {
	pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
		            !m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) &&
					!(ppro.m_socket!=INVALID_SOCKET) ? false : true);
}


void CMainFrame::OnUpdateViewShootreplayframe(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(global.attached_hwnd!=NULL);
}

void CMainFrame::OnUpdateViewMainToolbar(CCmdUI *pCmdUI) {
	pCmdUI->SetCheck(m_MainToolBar.IsVisible() ? 1 : 0);
}

void CMainFrame::OnUpdateViewFlagsToolbar(CCmdUI *pCmdUI) {
	pCmdUI->SetCheck(m_FlagsToolBar.IsVisible() ? 1 : 0);
}

void CMainFrame::OnUpdateViewStatusbar(CCmdUI *pCmdUI) {
	pCmdUI->SetCheck(m_wndStatusBar.IsVisible() ? 1 : 0);
}


//  2008.03.04 by THF
void CMainFrame::OnUpdateMenuPerlLoad(CCmdUI* pCmdUI) 
{
	if (the_Perl_Interpreter.is_a_Formula_loaded()) {
		pCmdUI->SetText("&Unload Formula\tF7");
	}
	else {
		pCmdUI->SetText("&Load Formula\tF7");
	}
	pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
		            !m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);	
}

//  2008.02.07  by THF    
void CMainFrame::OnUpdateMenuPerlLoadSpecificFormula(CCmdUI* pCmdUI)
{
	pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
		            !m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);	
}


//  2008.03.20 by THF
void CMainFrame::OnUpdateMenuPerlReloadFormula(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_REDCIRCLE) ||
		            !m_MainToolBar.GetToolBarCtrl().IsButtonEnabled(ID_MAIN_TOOLBAR_GREENCIRCLE)) ? false : true);	
}




BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (global.m_WaitCursor)
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
			if (m_formulaScintillaDlg->m_dirty &&
				MessageBox("The Formula Editor has un-applied (and unsaved) changes.\n"\
							"Really exit?", "Unsaved formula warning", MB_YESNO) == IDNO)
			{
				return;
			}
		}
	}

	CFrameWnd::OnSysCommand(nID, lParam);
}


//  2008.03.03 by THF
void CMainFrame::OnPerlLoadFormula()
{
#ifdef SEH_ENABLE
	try {
#endif
	if (the_Perl_Interpreter.is_a_Formula_loaded())
	{
		the_Perl_Interpreter.unload_FormulaFile();
	}
	else 
	{
		//  Reload the most recent formula
		the_Perl_Interpreter.reload_FormulaFile();
	}		
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnPerlLoadFormula :\n"); 
		throw;
	}
#endif
}
	

//  2008.03.03 by THF
void CMainFrame::OnPerlLoadSpecificFormula()
{	
#ifdef SEH_ENABLE
	try {
#endif
	CFileDialog			cfd(true);
	cfd.m_ofn.lpstrFilter = "Perl Scripts (*.pl)\0*.pl\0Perl Modules (*.pm)\0*.pm\0All Files (*.*)\0*.*\0\0";		                               
	cfd.m_ofn.lpstrTitle = "Select Perl formula file to OPEN";
	if (cfd.DoModal() == IDOK) 
	{		
		the_Perl_Interpreter.load_FormulaFile(cfd.m_ofn.lpstrFile);
	} 
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnPerlLoadSpecificFormula :\n"); 
		throw;
	}
#endif
}


//  2008.03.03 by THF
void CMainFrame::OnPerlEditMainFormula()
{
#ifdef SEH_ENABLE
	try {
#endif
	the_Perl_Interpreter.edit_main_FormulaFile();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnPerlEditMainFormula :\n"); 
		throw;
	}
#endif
}


//  2008.03.20 by THF
void CMainFrame::OnPerlReloadFormula()
{
#ifdef SEH_ENABLE
	try {
#endif
		//  Reload the most recent formula
		//    (This is a shortcut for unload + load.)
		the_Perl_Interpreter.reload_FormulaFile();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnPerlReloadFormula :\n"); 
		throw;
	}
#endif
}


void CMainFrame::OnPerlCheckSyntax()
{
#ifdef SEH_ENABLE
	try {
#endif
		the_Perl_Interpreter.check_Syntax();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CMainFrame::OnPerlEditCheckSyntax :\n"); 
		throw;
	}
#endif
}