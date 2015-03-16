//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
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
#include "CAutoConnector.h"
#include "CAutoplayer.h"
#include "CAutoplayerFunctions.h"
#include "CConfigurationCheck.h"
#include "CDebugTab.h"
#include "CDllExtension.h"
#include "CFilenames.h"
#include "CFlagsToolbar.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "COpenHoldemStatusbar.h"
#include "COpenHoldemTitle.h"
#include "CPopupHandler.h"
#include "CPreferences.h"
#include "CProblemSolver.h"
#include "CSymbolEngineReplayFrameController.h"
#include "CScraper.h"
#include "CSymbolEngineUserchair.h"
#include "CTablePositioner.h"
#include "CSymbolEngineTableLimits.h"
#include "..\CTransform\CTransform.h"
#include "CValidator.h"
#include "CVersionInfo.h"
#include "DialogFormulaScintilla.h"
#include "DialogSAPrefs2.h"
#include "DialogSAPrefs3.h"
#include "DialogSAPrefs4.h"
#include "DialogSAPrefs6.h"
#include "DialogSAPrefs8.h"
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
#include "MagicNumbers.h"
#include "OH_MessageBox.h"
#include "OpenHoldem.h"
#include "OpenHoldemDoc.h"
#include "SAPrefsDialog.h"
#include "Singletons.h"
#include "StringFunctions.h"

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
	ON_UPDATE_COMMAND_UI(ID_DLL_LOAD, &CMainFrame::OnUpdateMenuDllLoad)
	ON_UPDATE_COMMAND_UI(ID_DLL_LOADSPECIFICFILE, &CMainFrame::OnUpdateDllLoadspecificfile)

	//  Menu commands
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_EDIT_FORMULA, &CMainFrame::OnEditFormula)
	ON_COMMAND(ID_EDIT_PREFERENCES, &CMainFrame::OnEditPreferences)
	ON_COMMAND(ID_EDIT_VIEWLOG, &CMainFrame::OnEditViewLog)
	ON_COMMAND(ID_EDIT_TAGLOG, &CMainFrame::OnEditTagLog)
	ON_COMMAND(ID_VIEW_SCRAPEROUTPUT, &CMainFrame::OnScraperOutput)
	ON_COMMAND(ID_VIEW_SHOOTREPLAYFRAME, &CMainFrame::OnViewShootreplayframe)
	ON_COMMAND(ID_DLL_LOAD, &CMainFrame::OnDllLoad)
	ON_COMMAND(ID_DLL_LOADSPECIFICFILE, &CMainFrame::OnDllLoadspecificfile)
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

	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_PLCARDS,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_COMCARDS,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_POKERHAND,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_HANDRANK,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_PRWIN,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_NOPP,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_NIT,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_ACTION,OnUpdateStatus)

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

  //!!!!! Here?
  CConfigurationCheck configuration_check;
  configuration_check.CheckEverything();

  // Other "global" objects, not GUI
  _p_popup_handler = new CPopupHandler;
  _p_table_positioner = new CTablePositioner;
  _p_validator = new CValidator;
  _p_version_info = new CVersionInfo;
  // GUI objects
  _p_debug_tab = new CDebugTab;
  _p_formulaScintillaDlg = new CDlgFormulaScintilla;
  _p_ScraperOutputDlg = new CDlgScraperOutput;
  //!!!!!_p_openholdem_statusbar = new COpenHoldemStatusbar;
  _p_openholdem_title = new COpenHoldemTitle;
  //!!!_p_flags_toolbar = new CFlagsToolbar;
  //!!!_p_white_info_box = new CWhiteInfoBox;
}

CMainFrame::~CMainFrame() {
  // GUI objects
  delete _p_debug_tab;
  delete _p_formulaScintillaDlg;
  delete _p_ScraperOutputDlg;
  delete _p_openholdem_statusbar;
  delete _p_openholdem_title;
  delete _p_flags_toolbar;
  delete _p_white_info_box;
  // Other "global" objects, not GUI
  delete _p_popup_handler;
  delete _p_table_positioner;
  delete _p_validator;
  delete _p_version_info;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CString			t = "";
	lpCreateStruct->dwExStyle |= WS_MINIMIZE;
	if (CFrameWnd::OnCreate(lpCreateStruct) == k_undefined)
		return -1;
	// Tool bar
	_p_flags_toolbar = new CFlagsToolbar(this);
	// Status bar
	_p_openholdem_statusbar = new COpenHoldemStatusbar(this);
	// Start timer that checks if we should enable buttons
	SetTimer(ENABLE_BUTTONS_TIMER, 50, 0);
	// Start timer that updates status bar
	SetTimer(UPDATE_STATUS_BAR_TIMER, 500, 0);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
	if ( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	int			max_x = 0, max_y = 0;

	WNDCLASS wnd;
	HINSTANCE hInst = AfxGetInstanceHandle();

	// Set class name
	if (!(::GetClassInfo(hInst, theApp.p_preferences()->window_class_name(), &wnd)))
	{
		wnd.style			    = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wnd.lpfnWndProc		= ::DefWindowProc;
		wnd.cbClsExtra		= wnd.cbWndExtra = 0;
		wnd.hInstance		  = hInst;
		wnd.hIcon			    = AfxGetApp()->LoadIcon(IDI_ICON1);
		wnd.hCursor			  = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wnd.hbrBackground	= (HBRUSH) (COLOR_3DFACE + 1);
		wnd.lpszMenuName	= NULL;
		wnd.lpszClassName	= theApp.p_preferences()->window_class_name();
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
	cs.lpszClass = theApp.p_preferences()->window_class_name();

	// Restore window location and size
  // -32 to avoid placement directlz under the taskbar,
  // so that at least a little bit is visible
  // if the values in the ini-file are out of range.
	max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON - 32);
	max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON - 32);
  // Make sure that our coordinates are not out of screen
  // (too large or even negative)
	cs.x = min(theApp.p_preferences()->main_x(), max_x);
	cs.y = min(theApp.p_preferences()->main_y(), max_y);
  cs.x = max(cs.x, 0);
  cs.y = max(cs.y, 0);
  // GUI size
	cs.cx = kMainSizeX;
	cs.cy = kMainSizeY;

	return true;
}

// CMainFrame message handlers

void CMainFrame::OnEditFormula() {
	if (_p_formulaScintillaDlg) {
		if (_p_formulaScintillaDlg->m_dirty)	{
			if (OH_MessageBox_Interactive(
				  "The Formula Editor has un-applied changes.\n"
				  "Really exit?", 
				  "Formula Editor", MB_ICONWARNING|MB_YESNO) == IDNO) {
				_p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_FORMULA, true);
				return;
			}
		}
    BOOL	bWasShown = ::IsWindow(_p_formulaScintillaDlg->m_hWnd) && _p_formulaScintillaDlg->IsWindowVisible();
    _p_formulaScintillaDlg->DestroyWindow();
    if (bWasShown) {
      return;
    }
	}
  if (p_autoplayer->autoplayer_engaged()) {
    // The menu item Edit->Formula is disabled,
    // this is just an extra failsafe.
    return;
  }
  _p_formulaScintillaDlg = new CDlgFormulaScintilla(this);
	_p_formulaScintillaDlg->Create(CDlgFormulaScintilla::IDD,this);
	_p_formulaScintillaDlg->ShowWindow(SW_SHOW);
	_p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_FORMULA, true);
}

void CMainFrame::OnEditViewLog() {
	ShellExecute(NULL, "open", CFilenames::LogFilename(), NULL, NULL, SW_SHOW);
}

void CMainFrame::OnEditTagLog() {
	write_log(k_always_log_basic_information,
		"[*** ATTENTION ***] User tagged this situation for review\n");
}

// Menu -> Edit -> View Scraper Output
void CMainFrame::OnScraperOutput() {
	if (_p_ScraperOutputDlg) {
		write_log(theApp.p_preferences()->debug_gui(), "[GUI] m_ScraperOutputDlg = %i\n", _p_ScraperOutputDlg);
		write_log(theApp.p_preferences()->debug_gui(), "[GUI] Going to destroy existing scraper output dialog\n");

		BOOL	bWasShown = ::IsWindow(_p_ScraperOutputDlg->m_hWnd) && _p_ScraperOutputDlg->IsWindowVisible();
		write_log(theApp.p_preferences()->debug_gui(), "[GUI] Scraper output dialog was visible: %s\n", Bool2CString(bWasShown));

    CDlgScraperOutput::DestroyWindowSafely();
		if (bWasShown) {
			write_log(theApp.p_preferences()->debug_gui(), "[GUI] Scraper output dialog destroyed; going to return\n");
			return;
		}
	}	else {
		write_log(theApp.p_preferences()->debug_gui(), "[GUI] Scraper output dialog does not yet exist\n");
	}
	
	OH_MessageBox_Interactive("Please note:\n"
	  "OpenScrape scrapes everything, but OpenHoldem is optimized\n"		  
	  "to scrape only necessary info.\n"
	  "\n"
	  "For example:\n"
	  "If a players first card is \"cardback\" we don't even have to scrape the second one.\n"
	  "This is a feature, not a bug.\n",
	  "Info", 0);

	write_log(theApp.p_preferences()->debug_gui(), "[GUI] Going to create scraper output dialog\n");
	_p_ScraperOutputDlg = new CDlgScraperOutput(this);
	write_log(theApp.p_preferences()->debug_gui(), "[GUI] Scraper output dialog: step 1 finished\n");
	_p_ScraperOutputDlg->Create(CDlgScraperOutput::IDD,this);
	write_log(theApp.p_preferences()->debug_gui(), "[GUI] Scraper output dialog: step 2 finished\n");
	_p_ScraperOutputDlg->ShowWindow(SW_SHOW);
	write_log(theApp.p_preferences()->debug_gui(), "[GUI] Scraper output dialog: step 3 finished\n");
	_p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, true);
	write_log(theApp.p_preferences()->debug_gui(), "[GUI] Scraper output dialog: step 4 (final) finished\n"); 
}

void CMainFrame::OnViewShootreplayframe() {
	SYM->p_symbol_engine_replayframe_controller()->ShootReplayFrameIfNotYetDone();
}

void CMainFrame::OnManualMode() {
  // Manualmode usually is in the same directory.
  // No error-checking here~> if it does not work, then we silently fail.
  // http://msdn.microsoft.com/en-us/library/windows/desktop/bb762153%28v=vs.85%29.aspx
  ShellExecute(
		NULL,               // Pointer to parent window; not needed
		"open",             // "open" == "execute" for an executable
		"ManualMode.exe",		// ManualMode to be executed
		NULL, 		          // Parameters
		CFilenames::OpenHoldemDirectory(), // Working directory
		SW_SHOWNORMAL);		  // Active window, default size
}

void CMainFrame::OnEditPreferences() 
{
	//CDlgPreferences  myDialog;
	CSAPrefsDialog dlg;

	// the "pages" (all derived from CSAPrefsSubDlg)
	CDlgSAPrefs2 page2;
	CDlgSAPrefs3 page3;
	CDlgSAPrefs4 page4;
	CDlgSAPrefs6 page6;
	CDlgSAPrefs8 page8;
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
	if (dlg.DoModal()==IDOK)
	{
		//m_csStupidCString = dlg1.m_csText;
	}
}

BOOL CMainFrame::DestroyWindow() {
	p_dll_extension->Unload();
	StopThreads();
	// Save window position
  WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp); 		
	theApp.p_preferences()->SetValue(k_prefs_main_x, wp.rcNormalPosition.left); 		
 	theApp.p_preferences()->SetValue(k_prefs_main_y, wp.rcNormalPosition.top);
  return CFrameWnd::DestroyWindow();
}

void CMainFrame::OnFileOpen() 
{
    COpenHoldemDoc *pDoc = (COpenHoldemDoc *)this->GetActiveDocument();   
   
    if (!pDoc->SaveModified())
        return;        // leave the original one

	CFileDialog			cfd(true);

	cfd.m_ofn.lpstrInitialDir = theApp.p_preferences()->path_ohf();
  // http://msdn.microsoft.com/en-us/library/windows/desktop/ms646839%28v=vs.85%29.aspx
  cfd.m_ofn.lpstrFilter = "OpenHoldem Formula Files (*.ohf, *.oppl)\0*.ohf;*.oppl\0All files (*.*)\0*.*\0\0";
	cfd.m_ofn.lpstrTitle = "Select Formula file to OPEN";
	if (cfd.DoModal() == IDOK)
	{				
		pDoc->OnOpenDocument(cfd.GetPathName());
		pDoc->SetPathName(cfd.GetPathName());
		// Update window title, registry
		_p_openholdem_title->UpdateTitle();
		theApp.p_preferences()->SetValue(k_prefs_path_ohf, cfd.GetPathName());
		theApp.StoreLastRecentlyUsedFileList();
	}
}

void CMainFrame::OnTimer(UINT nIDEvent) {
	RECT			att_rect = {0}, wrect = {0};

	if (nIDEvent == HWND_CHECK_TIMER)	{
		if (!IsWindow(p_autoconnector->attached_hwnd()))		{
			// Table disappeared
			p_autoplayer->EngageAutoplayer(false);
			p_autoconnector->Disconnect();
		}
	}	else if (nIDEvent == ENABLE_BUTTONS_TIMER) {
		// Autoplayer
		// Since OH 4.0.5 we support autoplaying immediatelly after connection
		// without the need to know the userchair to act on secondary formulas.
		if (SYM->p_symbol_engine_userchair() != NULL
			  && p_autoconnector->IsConnected()) 	{
			_p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, true);
		}	else {
			_p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
		}

		// Shoot replay frame
		if (p_autoconnector->attached_hwnd() != NULL) {
			_p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, true);
    }	else {
      _p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, false);
    }
	}	else if (nIDEvent == UPDATE_STATUS_BAR_TIMER) {
		GUI->p_openholdem_statusbar()->OnUpdateStatusbar();
	}
	CWnd::OnTimer(nIDEvent); 
}

void CMainFrame::OnAutoplayer() 
{
	bool is_autoplaying = p_autoplayer->autoplayer_engaged();
	// Toggle the autoplayer-state
	p_autoplayer->EngageAutoplayer(!is_autoplaying);
}

void CMainFrame::OnValidator()  {
	if (_p_flags_toolbar->IsButtonChecked(ID_MAIN_TOOLBAR_VALIDATOR)) {
		_p_flags_toolbar->CheckButton(ID_MAIN_TOOLBAR_VALIDATOR, true);
		MAIN->p_validator()->SetEnabledManually(true);
	}	else {
		_p_flags_toolbar->CheckButton(ID_MAIN_TOOLBAR_VALIDATOR, false);
		MAIN->p_validator()->SetEnabledManually(false);
	}
}

void CMainFrame::OnUpdateStatus(CCmdUI *pCmdUI) 
{
	GUI->p_openholdem_statusbar()->OnUpdateStatusbar();
}

void CMainFrame::OnDllLoad() 
{
	if (p_dll_extension->IsLoaded())
		p_dll_extension->Unload();
	else
		p_dll_extension->Load("");
}

void CMainFrame::OnDllLoadspecificfile() {
	CFileDialog			cfd(true);

	cfd.m_ofn.lpstrInitialDir = theApp.p_preferences()->path_dll();
	cfd.m_ofn.lpstrFilter = "DLL Files (.dll)\0*.dll\0\0";
	cfd.m_ofn.lpstrTitle = "Select OpenHoldem DLL file to OPEN";

	if (cfd.DoModal() == IDOK) {
		p_dll_extension->Load(cfd.m_ofn.lpstrFile);
		theApp.p_preferences()->SetValue(k_prefs_path_dll, cfd.GetPathName());
	}
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

void CMainFrame::OnClickedFlags() {
	_p_flags_toolbar->OnClickedFlags();
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

void CMainFrame::OnUpdateMenuDllLoad(CCmdUI* pCmdUI) {
	if (p_dll_extension->IsLoaded()) {
		pCmdUI->SetText("&Unload\tF4");
  } else {
		pCmdUI->SetText("&Load\tF4");
  }
	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
}

void CMainFrame::OnUpdateDllLoadspecificfile(CCmdUI *pCmdUI) {
	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
}

void CMainFrame::OnUpdateViewShootreplayframe(CCmdUI *pCmdUI) {
	pCmdUI->Enable(p_autoconnector->attached_hwnd() != NULL);
}

void CMainFrame::OnUpdateViewScraperOutput(CCmdUI *pCmdUI) {
	pCmdUI->Enable(p_autoconnector->attached_hwnd() != NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Other functions

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

void CMainFrame::OpenHelpFile(CString windows_help_file_chm)
{
	long long int RetValue = long long int(ShellExecute(NULL, "open", windows_help_file_chm, NULL, NULL, SW_SHOW));
	if (RetValue <= 32)
	{
		CString error_message;
		error_message.Format("Could not open help-file %s\n"
			"Please put it into your OpenHoldem folder\n",
			windows_help_file_chm);
		OH_MessageBox_Interactive(error_message, "Error", 0);
	}
}

void CMainFrame::OnHelp() {
	OpenHelpFile("OpenHoldem_Manual.chm");
}

void CMainFrame::OnHelpOpenPPL()
{
	OpenHelpFile("OpenPPL_Manual.chm");
}

void CMainFrame::OnHelpForums()
{
	ShellExecute(NULL, "open", "http://www.maxinmontreal.com/forums/index.php", "", "", SW_SHOWDEFAULT);
}

void CMainFrame::OnHelpProblemSolver() 
{
	CProblemSolver my_problem_solver;
	my_problem_solver.TryToDetectBeginnersProblems();
}


CMainFrame* PMainframe()
{
	CMainFrame *p_mainframe = (CMainFrame *) (theApp.m_pMainWnd);
	return p_mainframe;
}



