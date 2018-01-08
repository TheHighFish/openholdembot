//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MainFrm.h"
#include <io.h>
#include <process.h>
#include "..\DLLs\Files_DLL\Files.h"
#include "..\DLLs\StringFunctions_DLL\string_functions.h"
#include "..\DLLs\WindowFunctions_DLL\window_functions.h"
#include "CAutoConnector.h"
#include "CAutoplayer.h"
#include "CAutoplayerFunctions.h"
#include "CEngineContainer.h"
#include "CFlagsToolbar.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "COpenHoldemHopperCommunication.h"
#include "COpenHoldemStatusbar.h"
#include "COpenHoldemTitle.h"
#include "CPreferences.h"
#include "CProblemSolver.h"
#include "CSymbolEngineReplayFrameController.h"
#include "CScraper.h"
#include "CSessionCounter.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbolEngineTableLimits.h"
#include "..\CTransform\CTransform.h"
#include "CValidator.h"
#include "DialogFormulaScintilla.h"
#include "DialogSAPrefs2.h"
#include "DialogSAPrefs3.h"
#include "DialogSAPrefs4.h"
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
#include "DialogSAPrefs19.h"
#include "DialogSAPrefs20.h"
#include "DialogSAPrefs21.h"
#include "DialogSAPrefs22.h"
#include "DialogScraperOutput.h"
#include "inlines/eval.h"
#include "OpenHoldem.h"
#include "OpenHoldemDoc.h"
#include "SAPrefsDialog.h"
#include "Singletons.h"

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()

	// Menu updates
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, &CMainFrame::OnUpdateMenuFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CMainFrame::OnUpdateMenuFileOpen)
  ON_UPDATE_COMMAND_UI(ID_EDIT_FORMULA, &CMainFrame::OnUpdateMenuFileEdit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOOTREPLAYFRAME, &CMainFrame::OnUpdateViewShootreplayframe)
  ON_UPDATE_COMMAND_UI(ID_VIEW_SCRAPEROUTPUT, &CMainFrame::OnUpdateViewScraperOutput)

	//  Menu commands
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_EDIT_FORMULA, &CMainFrame::OnEditFormula)
	ON_COMMAND(ID_EDIT_PREFERENCES, &CMainFrame::OnEditPreferences)
	ON_COMMAND(ID_EDIT_VIEWLOG, &CMainFrame::OnEditViewLog)
	ON_COMMAND(ID_EDIT_TAGLOG, &CMainFrame::OnEditTagLog)
  ON_COMMAND(ID_EDIT_CLEARLOG, &CMainFrame::OnEditClearLog)
	ON_COMMAND(ID_VIEW_SCRAPEROUTPUT, &CMainFrame::OnScraperOutput)
	ON_COMMAND(ID_VIEW_SHOOTREPLAYFRAME, &CMainFrame::OnViewShootreplayframe)
	ON_COMMAND(ID_HELP_HELP, &CMainFrame::OnHelp)
	ON_COMMAND(ID_HELP_OPEN_PPL, &CMainFrame::OnHelpOpenPPL)
	ON_COMMAND(ID_HELP_FORUMS, &CMainFrame::OnHelpForums)
	ON_COMMAND(ID_HELP_PROBLEMSOLVER, &CMainFrame::OnHelpProblemSolver)

	// Main toolbar 
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_AUTOPLAYER, &CMainFrame::OnAutoplayer)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_FORMULA, &CMainFrame::OnEditFormula)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_VALIDATOR, &CMainFrame::OnValidator)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_TAGLOGFILE, &CMainFrame::OnEditTagLog)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, &CMainFrame::OnScraperOutput)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_SHOOTFRAME, &CMainFrame::OnViewShootreplayframe)
  ON_BN_CLICKED(ID_MAIN_TOOLBAR_MANUALMODE, &CMainFrame::OnManualMode)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_HELP, &CMainFrame::OnHelp)

	// Hopper
	ON_MESSAGE(WMA_SETWINDOWTEXT, &COpenHoldemHopperCommunication::OnSetWindowText)
	ON_MESSAGE(WMA_DOCONNECT,     &COpenHoldemHopperCommunication::OnConnectMessage)
	ON_MESSAGE(WMA_DODISCONNECT,  &COpenHoldemHopperCommunication::OnDisconnectMessage)
	ON_MESSAGE(WMA_CONNECTEDHWND, &COpenHoldemHopperCommunication::OnConnectedHwndMessage)
	ON_MESSAGE(WMA_SETFLAG,       &COpenHoldemHopperCommunication::OnSetFlagMessage)
	ON_MESSAGE(WMA_RESETFLAG,     &COpenHoldemHopperCommunication::OnResetFlagMessage)
	ON_MESSAGE(WMA_ISREADY,       &COpenHoldemHopperCommunication::OnIsReadyMessage)

	// Flags
	ON_BN_CLICKED(ID_NUMBER0,  &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER1,  &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER2,  &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER3,  &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER4,  &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER5,  &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER6,  &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER7,  &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER8,  &CMainFrame::OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER9,  &CMainFrame::OnClickedFlags)
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
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_ACTION, OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_HANDRANK,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_PRWIN,OnUpdateStatus)
	
	ON_WM_SETCURSOR()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

// CMainFrame construction/destruction
CMainFrame::CMainFrame() {
	_wait_cursor = false;

	_prev_att_rect.bottom = 0;
	_prev_att_rect.left = 0;
	_prev_att_rect.right = 0;
	_prev_att_rect.top = 0;
	_prev_wrect.bottom = 0;
	_prev_wrect.left = 0;
	_prev_wrect.right = 0;
	_prev_wrect.top = 0;
}

CMainFrame::~CMainFrame() {
	if (p_flags_toolbar != NULL) {
		delete(p_flags_toolbar);
	}
  if (p_openholdem_statusbar != NULL) {
    delete p_openholdem_statusbar;
  }
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CString			t = "";
	lpCreateStruct->dwExStyle |= WS_MINIMIZE;
	if (CFrameWnd::OnCreate(lpCreateStruct) == kUndefined)
		return -1;
	// Tool bar
	p_flags_toolbar = new CFlagsToolbar(this);
	// Status bar
	p_openholdem_statusbar = new COpenHoldemStatusbar(this);
	// Start timer that checks if we should enable buttons
	SetTimer(ENABLE_BUTTONS_TIMER, 50, 0);
	// Start timer that updates status bar
	SetTimer(UPDATE_STATUS_BAR_TIMER, 500, 0);
  // Start timer that checks for continued existence of attached HWND 		
  SetTimer(HWND_CHECK_TIMER, 200, 0);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
	if ( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	int			max_x = 0, max_y = 0;

	WNDCLASS wnd;
	HINSTANCE hInst = AfxGetInstanceHandle();

	// Set class name
	if (!(::GetClassInfo(hInst, preferences.window_class_name(), &wnd))) {
		wnd.style			    = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wnd.lpfnWndProc		= ::DefWindowProc;
		wnd.cbClsExtra		= wnd.cbWndExtra = 0;
		wnd.hInstance		  = hInst;
		wnd.hIcon			    = AfxGetApp()->LoadIcon(IDI_ICON1);
		wnd.hCursor			  = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wnd.hbrBackground	= (HBRUSH) (COLOR_3DFACE + 1);
		wnd.lpszMenuName	= NULL;
		wnd.lpszClassName	= preferences.window_class_name();
    // Fixed size window, not resizable 
    // Because bad-sized windows are annoying
    // and because of potential support for a 4th user-card ;-)
    // http://arstechnica.com/civis/viewtopic.php?f=20&t=848676
    // http://msdn.microsoft.com/en-us/library/aa925944.aspx
    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	  cs.style &= (0xFFFFFFFF ^ WS_SIZEBOX);
	  cs.style |= WS_BORDER;
	  cs.style &= (0xFFFFFFFF ^ WS_MAXIMIZEBOX);

		AfxRegisterClass( &wnd );
	}
	cs.lpszClass = preferences.window_class_name();

	// Restore window location and size
  // -32 to avoid placement directly under the taskbar,
  // so that at least a little bit is visible
  // if the values in the ini-file are out of range.
	max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON - 32);
	max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON - 32);
  // Make sure that our coordinates are not out of screen
  // (too large or even negative)
	cs.x = min(preferences.main_x(), max_x);
	cs.y = min(preferences.main_y(), max_y);
  cs.x = max(cs.x, 0);
  cs.y = max(cs.y, 0);
  // GUI size
	cs.cx = kMainSizeX;
	cs.cy = kMainSizeY;

	return true;
}

// CMainFrame message handlers

void CMainFrame::OnEditFormula() {
	if (m_formulaScintillaDlg) {
		if (m_formulaScintillaDlg->m_dirty)	{
			if (MessageBox_Interactive(
				  "The Formula Editor has un-applied changes.\n"
				  "Really exit?", 
				  "Formula Editor", MB_ICONWARNING|MB_YESNO) == IDNO) {
				p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_FORMULA, true);
				return;
			}
		}
    BOOL	bWasShown = ::IsWindow(m_formulaScintillaDlg->m_hWnd) && m_formulaScintillaDlg->IsWindowVisible();
    m_formulaScintillaDlg->DestroyWindow();
    if (bWasShown) {
			return;
    }
	}
  if (p_autoplayer->autoplayer_engaged()) {
    // The menu item Edit->Formula is disabled,
    // this is just an extra failsafe.
    return;
  }
  m_formulaScintillaDlg = new CDlgFormulaScintilla(this);
	m_formulaScintillaDlg->Create(CDlgFormulaScintilla::IDD,this);
	m_formulaScintillaDlg->ShowWindow(SW_SHOW);
	p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_FORMULA, true);
}

void CMainFrame::OnEditViewLog() {
  assert(p_sessioncounter != nullptr);
  OpenFileInExternalSoftware(LogFilePath(p_sessioncounter->session_id()));
}

void CMainFrame::OnEditTagLog() {
	write_log(k_always_log_basic_information,
		"[*** ATTENTION ***] User tagged this situation for review\n");
}

void CMainFrame::OnEditClearLog() {
  clear_log();
}

// Menu -> Edit -> View Scraper Output
void CMainFrame::OnScraperOutput() {
	if (m_ScraperOutputDlg) {
		write_log(preferences.debug_gui(), "[GUI] m_ScraperOutputDlg = %i\n", m_ScraperOutputDlg);
		write_log(preferences.debug_gui(), "[GUI] Going to destroy existing scraper output dialog\n");

		BOOL	bWasShown = ::IsWindow(m_ScraperOutputDlg->m_hWnd) && m_ScraperOutputDlg->IsWindowVisible();
		write_log(preferences.debug_gui(), "[GUI] Scraper output dialog was visible: %s\n", Bool2CString(bWasShown));

    CDlgScraperOutput::DestroyWindowSafely();
		if (bWasShown) {
			write_log(preferences.debug_gui(), "[GUI] Scraper output dialog destroyed; going to return\n");
			return;
		}
	}	else {
		write_log(preferences.debug_gui(), "[GUI] Scraper output dialog does not yet exist\n");
	}
	
	MessageBox_Interactive("Please note:\n"
	  "OpenScrape scrapes everything, but OpenHoldem is optimized\n"		  
	  "to scrape only necessary info.\n"
	  "\n"
	  "For example:\n"
	  "If a players first card is \"cardback\" we don't even have to scrape the second one.\n"
	  "This is a feature, not a bug.\n",
	  "Info", 0);

	write_log(preferences.debug_gui(), "[GUI] Going to create scraper output dialog\n");
	m_ScraperOutputDlg = new CDlgScraperOutput(this);
	write_log(preferences.debug_gui(), "[GUI] Scraper output dialog: step 1 finished\n");
	m_ScraperOutputDlg->Create(CDlgScraperOutput::IDD,this);
	write_log(preferences.debug_gui(), "[GUI] Scraper output dialog: step 2 finished\n");
	m_ScraperOutputDlg->ShowWindow(SW_SHOW);
	write_log(preferences.debug_gui(), "[GUI] Scraper output dialog: step 3 finished\n");
	p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, true);
	write_log(preferences.debug_gui(), "[GUI] Scraper output dialog: step 4 (final) finished\n"); 
}

void CMainFrame::OnViewShootreplayframe() {
	p_engine_container->symbol_engine_replayframe_controller()->ShootReplayFrameIfNotYetDone();
}

void CMainFrame::OnManualMode() {
  // Manualmode usually is in the same directory.
  // No error-checking here~> if it does not work, then we silently fail.
  // http://msdn.microsoft.com/en-us/library/windows/desktop/bb762153%28v=vs.85%29.aspx
  ShellExecute(
    NULL,               // Pointer to parent window; not needed
    "open",             // "open" == "execute" for an executable
    ManualModePath(),
		NULL, 		          // Parameters
		ToolsDirectory(), // Working directory, location of ManualMode
		SW_SHOWNORMAL);		  // Active window, default size
}

void CMainFrame::OnEditPreferences() {
	//CDlgPreferences  myDialog;
	CSAPrefsDialog dlg;

	// the "pages" (all derived from CSAPrefsSubDlg)
	CDlgSAPrefs2 page2;
	CDlgSAPrefs3 page3;
	CDlgSAPrefs4 page4;
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
	CDlgSAPrefs19 page19;
	CDlgSAPrefs20 page20;
	CDlgSAPrefs21 page21;
	CDlgSAPrefs22 page22;

	// add pages
	dlg.AddPage(page14, "Auto-Connector");
	dlg.AddPage(page2,  "Autoplayer");
  dlg.AddPage(page9, "Auto-starter");
	dlg.AddPage(page10, "Chat");
	dlg.AddPage(page17, "Configuration Check");
	dlg.AddPage(page20, "Debugging");
	dlg.AddPage(page3,  "DLL Extension");
	dlg.AddPage(page15, "GUI");
	dlg.AddPage(page19, "Handhistory Generator");
	dlg.AddPage(page11, "Logging");
	dlg.AddPage(page6,  "Poker Tracker v4");
	dlg.AddPage(page22, "Popup Blocker");
	dlg.AddPage(page16, "Rebuy");
	dlg.AddPage(page8,  "Replay Frames");
	dlg.AddPage(page4,  "Scraper");
	dlg.AddPage(page13, "Stealth");
	dlg.AddPage(page21, "Table Positioner");
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
	if (dlg.DoModal()==IDOK) {
		//m_csStupidCString = dlg1.m_csText;
	}
}

BOOL CMainFrame::DestroyWindow() {
	StopThreads();
  PMainframe()->KillTimers();
	// Save window position
  WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp); 		
	preferences.SetValue(k_prefs_main_x, wp.rcNormalPosition.left); 		
 	preferences.SetValue(k_prefs_main_y, wp.rcNormalPosition.top);
  write_log(preferences.debug_gui(), "[GUI] Going to delete the GUI\n");
  write_log(preferences.debug_gui(), "[GUI] this = [%i]\n", this);
  // All OK here
  assert(AfxCheckMemory());
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=20459
  // probably caused by incorrect order of deletion,
  // caused by incorrect position of StopThreads and KillTimers.
  bool success = CFrameWnd::DestroyWindow(); 
  write_log(preferences.debug_gui(), "[GUI] Window deleted\n");
  return success;
}

void CMainFrame::OnFileOpen() {
  COpenHoldemDoc *pDoc = (COpenHoldemDoc *)this->GetActiveDocument();   
  if (!pDoc->SaveModified()) {
    return;        // leave the original one
  }
	CFileDialog			cfd(true);
  cfd.m_ofn.lpstrInitialDir = "";
  // http://msdn.microsoft.com/en-us/library/windows/desktop/ms646839%28v=vs.85%29.aspx
  cfd.m_ofn.lpstrFilter = "OpenHoldem Formula Files (*.ohf, *.oppl, *.txt)\0*.ohf;*.oppl;*.txt\0All files (*.*)\0*.*\0\0";
	cfd.m_ofn.lpstrTitle = "Select Formula file to OPEN";
	if (cfd.DoModal() == IDOK) {				
		pDoc->OnOpenDocument(cfd.GetPathName());
		pDoc->SetPathName(cfd.GetPathName());
		// Update window title, registry
		p_openholdem_title->UpdateTitle();
		theApp.StoreLastRecentlyUsedFileList();
	}
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent) {
  write_log(preferences.debug_timers(), "[GUI] CMainFrame::OnTimer()\n");
  // There was a race-condition in this function during termination 
  // if OnTimer was in progress and p_autoconnector became dangling.
  // This is probably fixed, as we now kill the timers
  // before we delete singleton, but we keep these safety-meassures.
  // It is OK to skip CWnd::OnTimer(nIDEvent); if we terminate.
  if (p_flags_toolbar == NULL) {
    return;
  }
  if (p_openholdem_statusbar == NULL) {
    return;
  }
  if (p_autoconnector == NULL) {
    return;
  }
  if (nIDEvent == HWND_CHECK_TIMER) {
    write_log(preferences.debug_timers(), "[GUI] OnTimer checking table connection\n");
    // Important: check is_conected first.
    // Checking only garbage HWND, then disconnecting
    // can lead to freezing if it colludes with Connect()
 	  if (p_autoconnector->IsConnectedToGoneWindow()) {
 	    // Table disappeared 		
      write_log(preferences.debug_timers(), "[GUI] OnTimer found disappeared window()\n");
 	    p_autoconnector->Disconnect("table disappeared"); 		 		
    }
 	} else if (nIDEvent == ENABLE_BUTTONS_TIMER) {
		// Autoplayer
		// Since OH 4.0.5 we support autoplaying immediatelly after connection
		// without the need to know the userchair to act on secondary formulas.
    write_log(preferences.debug_alltherest(), "[GUI] location Johnny_E\n");
		if (p_autoconnector->IsConnectedToAnything()) 	{
      write_log(preferences.debug_timers(), "[GUI] OnTimer enabling buttons\n");
      write_log(preferences.debug_alltherest(), "[GUI] location Johnny_F\n");
			p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, true);
      write_log(preferences.debug_alltherest(), "[GUI] location Johnny_G\n");
      p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, true);
      write_log(preferences.debug_alltherest(), "[GUI] location Johnny_L\n");
		}	else {
      write_log(preferences.debug_timers(), "[GUI] OnTimer disabling buttons\n");
      write_log(preferences.debug_alltherest(), "[GUI] location Johnny_H\n");
			p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
      write_log(preferences.debug_alltherest(), "[GUI] location Johnny_I\n");
      p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, false);
      write_log(preferences.debug_alltherest(), "[GUI] location Johnny_N\n");
		}
    write_log(preferences.debug_alltherest(), "[GUI] location Johnny_O\n");
	}	else if (nIDEvent == UPDATE_STATUS_BAR_TIMER) {
    write_log(preferences.debug_timers(), "[GUI] OnTimer updating statusbar\n");
    write_log(preferences.debug_alltherest(), "[GUI] location Johnny_P\n");
		p_openholdem_statusbar->OnUpdateStatusbar();
    write_log(preferences.debug_alltherest(), "[GUI] location Johnny_Q\n");
	}
  write_log(preferences.debug_alltherest(), "[GUI] location Johnny_R\n");
	CWnd::OnTimer(nIDEvent); 
}

void CMainFrame::OnAutoplayer() {
	bool is_autoplaying = p_autoplayer->autoplayer_engaged();
	// Toggle the autoplayer-state
	p_autoplayer->EngageAutoplayer(!is_autoplaying);
}

void CMainFrame::OnValidator() {
	if (p_flags_toolbar->IsButtonChecked(ID_MAIN_TOOLBAR_VALIDATOR)) {
		p_flags_toolbar->CheckButton(ID_MAIN_TOOLBAR_VALIDATOR, true);
		p_validator->SetEnabledManually(true);
	}	else {
		p_flags_toolbar->CheckButton(ID_MAIN_TOOLBAR_VALIDATOR, false);
		p_validator->SetEnabledManually(false);
	}
}

void CMainFrame::OnUpdateStatus(CCmdUI *pCmdUI) {
	p_openholdem_statusbar->OnUpdateStatusbar();
}

BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	if (_wait_cursor)	{
		RestoreWaitCursor();
		return TRUE;
	}
	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMainFrame::OnClickedFlags() {
	p_flags_toolbar->OnClickedFlags();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Menu updaters

void CMainFrame::OnUpdateMenuFileNew(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
}

void CMainFrame::OnUpdateMenuFileOpen(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
}

void CMainFrame::OnUpdateMenuFileEdit(CCmdUI* pCmdUI) {
	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
}

void CMainFrame::OnUpdateDllLoadspecificfile(CCmdUI *pCmdUI) {
	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
}

void CMainFrame::OnUpdateViewShootreplayframe(CCmdUI *pCmdUI) {
	pCmdUI->Enable(p_autoconnector->IsConnectedToAnything());
}

void CMainFrame::OnUpdateViewScraperOutput(CCmdUI *pCmdUI) {
	pCmdUI->Enable(p_autoconnector->IsConnectedToAnything());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Other functions

void CMainFrame::KillTimers() { 		
  // It is very important that we kill all timers as early as possible
  // on termination. otherwise the timer-functions might access
  // objects loke the auto_connector that already are destructed,
  // thus causing a memory-access-error.
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=20459
 	CFrameWnd::KillTimer(HWND_CHECK_TIMER);
  CFrameWnd::KillTimer(ENABLE_BUTTONS_TIMER);
  CFrameWnd::KillTimer(UPDATE_STATUS_BAR_TIMER);
}

void CMainFrame::ResetDisplay() {
	InvalidateRect(NULL, true); 
}

void CMainFrame::OnHelp() {
  OpenFileInExternalSoftware(OpenHoldemManualpath());
}

void CMainFrame::OnHelpOpenPPL() {
  OpenFileInExternalSoftware(OpenPPLManualpath());
}

void CMainFrame::OnHelpForums() {
	ShellExecute(NULL, "open", "http://www.maxinmontreal.com/forums/index.php", "", "", SW_SHOWDEFAULT);
}

void CMainFrame::OnHelpProblemSolver() {
	CProblemSolver my_problem_solver;
	my_problem_solver.TryToDetectBeginnersProblems();
}


CMainFrame* PMainframe() {
	CMainFrame *p_mainframe = (CMainFrame *) (theApp.m_pMainWnd);
	return p_mainframe;
}



