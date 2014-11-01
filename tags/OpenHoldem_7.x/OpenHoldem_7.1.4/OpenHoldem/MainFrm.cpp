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
#include "CDllExtension.h"
#include "CFilenames.h"
#include "CFlagsToolbar.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "COpenHoldemHopperCommunication.h"
#include "COpenHoldemStatusbar.h"
#include "COpenHoldemTitle.h"
#include "CPerl.hpp"
#include "CPreferences.h"
#include "CProblemSolver.h"
#include "CSymbolEngineReplayFrameController.h"
#include "CScraper.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbolEngineTableLimits.h"
#include "..\CTransform\CTransform.h"
#include "CValidator.h"
#include "DialogFormulaScintilla.h"
#include "DialogSAPrefs2.h"
#include "DialogSAPrefs3.h"
#include "DialogSAPrefs4.h"
#include "DialogSAPrefs6.h"
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
	ON_UPDATE_COMMAND_UI(ID_DLL_LOAD, &CMainFrame::OnUpdateMenuDllLoad)
	ON_UPDATE_COMMAND_UI(ID_DLL_LOADSPECIFICFILE, &CMainFrame::OnUpdateDllLoadspecificfile)
	ON_UPDATE_COMMAND_UI(ID_PERL_LOADFORMULA, &CMainFrame::OnUpdateMenuPerlLoad)
	ON_UPDATE_COMMAND_UI(ID_PERL_LOADSPECIFICFORMULA, &CMainFrame::OnUpdateMenuPerlLoadSpecificFormula)
	ON_UPDATE_COMMAND_UI(ID_PERL_RELOADFORMULA, &CMainFrame::OnUpdateMenuPerlReloadFormula)
	ON_UPDATE_COMMAND_UI(ID_PERL_CHECKSYNTAX, &CMainFrame::OnUpdateMenuPerlCheckSyntax)
	ON_UPDATE_COMMAND_UI(ID_PERL_EDITMAINFORMULA, &CMainFrame::OnUpdateMenuPerlEditMainFormula)

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
	ON_COMMAND(ID_PERL_LOADFORMULA, &CMainFrame::OnPerlLoadFormula)
	ON_COMMAND(ID_PERL_LOADSPECIFICFORMULA, &CMainFrame::OnPerlLoadSpecificFormula)
	ON_COMMAND(ID_PERL_RELOADFORMULA, &CMainFrame::OnPerlReloadFormula)
	ON_COMMAND(ID_PERL_CHECKSYNTAX, &CMainFrame::OnPerlCheckSyntax)
	ON_COMMAND(ID_PERL_EDITMAINFORMULA, &CMainFrame::OnPerlEditMainFormula)
	ON_COMMAND(ID_HELP_HELP, &CMainFrame::OnHelp)
	ON_COMMAND(ID_HELP_OPEN_PPL, &CMainFrame::OnHelpOpenPPL)
	ON_COMMAND(ID_HELP_FORUMS, &CMainFrame::OnHelpForums)
	ON_COMMAND(ID_HELP_PROBLEMSOLVER, &CMainFrame::OnHelpProblemSolver)

	// Main toolbar 
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_AUTOPLAYER, &CMainFrame::OnAutoplayer)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_FORMULA, &CMainFrame::OnEditFormula)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_VALIDATOR, &CMainFrame::OnValidator)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_TAGLOGFILE, &CMainFrame::OnEditTagLog)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_MINMAX, &CMainFrame::OnMinMax)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_ATTACH_TOP, &CMainFrame::OnAttachTop)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, &CMainFrame::OnAttachBottom)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, &CMainFrame::OnScraperOutput)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_SHOOTFRAME, &CMainFrame::OnViewShootreplayframe)
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
CMainFrame::CMainFrame() 
{
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

CMainFrame::~CMainFrame() 
{
	if (p_flags_toolbar != NULL)
	{
		p_flags_toolbar->~CFlagsToolbar();
		delete(p_flags_toolbar);
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CString			t = "";
	lpCreateStruct->dwExStyle |= WS_MINIMIZE;
	if (CFrameWnd::OnCreate(lpCreateStruct) == k_undefined)
		return -1;

	// Tool bar
	p_flags_toolbar = new CFlagsToolbar(this);

	// Status bar
	p_openholdem_statusbar = new COpenHoldemStatusbar(this);

	// Start timer that checks if we should enable buttons
	SetTimer(ENABLE_BUTTONS_TIMER, 50, 0);

	// Start timer that updates status bar
	SetTimer(UPDATE_STATUS_BAR_TIMER, 500, 0);

	// Start timer that attaches the OH window when the poker window moves
	SetTimer(ATTACH_WINDOW_TIMER, 20, 0);

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
	if (!(::GetClassInfo(hInst, preferences.window_class_name(), &wnd)))
	{
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
	max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
	max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
	cs.x = min(preferences.main_x(), max_x);
	cs.y = min(preferences.main_y(), max_y);
	cs.cx = kMainSizeX;
	cs.cy = kMainSizeY;

	return true;
}

// CMainFrame message handlers

void CMainFrame::OnEditFormula() {
	if (m_formulaScintillaDlg) {
		if (m_formulaScintillaDlg->m_dirty)	{
			if (OH_MessageBox_Interactive(
				  "The Formula Editor has un-applied changes.\n"
				  "Really exit?", 
				  "Formula Editor", MB_ICONWARNING|MB_YESNO) == IDNO) {
				p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_FORMULA, true);
				return;
			}
		}
    BOOL	bWasShown = ::IsWindow(m_formulaScintillaDlg->m_hWnd) && m_formulaScintillaDlg->IsWindowVisible();
    m_formulaScintillaDlg->DestroyWindow();
    if (bWasShown)
    {
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

void CMainFrame::OnEditViewLog()
{
	if (p_filenames == NULL)
	{
		return;
	}
	ShellExecute(NULL, "open", p_filenames->LogFilename(), NULL, NULL, SW_SHOW);
}

void CMainFrame::OnEditTagLog()
{
	write_log(k_always_log_basic_information,
		"[*** ATTENTION ***] User tagged this situation for review\n");
  p_flags_toolbar->CheckButton(ID_MAIN_TOOLBAR_TAGLOGFILE, false);
}

// Menu -> Edit -> View Scraper Output
void CMainFrame::OnScraperOutput() 
{
	if (m_ScraperOutputDlg) 
	{
		write_log(preferences.debug_gui(), "[GUI] m_ScraperOutputDlg = %i\n", m_ScraperOutputDlg);
		write_log(preferences.debug_gui(), "[GUI] Going to destroy existing scraper output dialog\n");

		BOOL	bWasShown = ::IsWindow(m_ScraperOutputDlg->m_hWnd) && m_ScraperOutputDlg->IsWindowVisible();
		write_log(preferences.debug_gui(), "[GUI] Scraper output dialog was visible: %s\n", Bool2CString(bWasShown));

		m_ScraperOutputDlg->DestroyWindow();
		delete m_ScraperOutputDlg;
		m_ScraperOutputDlg = NULL;

		if (bWasShown)
		{
			write_log(preferences.debug_gui(), "[GUI] Scraper output dialog destroyed; going to return\n");
			return;
		}
	}
	else
	{
		write_log(preferences.debug_gui(), "[GUI] Scraper output dialog does not yet exist\n");
	}
	
	OH_MessageBox_Interactive("Please note:\n"
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

void CMainFrame::OnViewShootreplayframe()
{
	p_symbol_engine_replayframe_controller->ShootReplayFrameIfNotYetDone();
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
	dlg.AddPage(page10, "Chat");
	dlg.AddPage(page17, "Configuration Check");
	dlg.AddPage(page20, "Debugging");
	dlg.AddPage(page3,  "DLL Extension");
	dlg.AddPage(page15, "GUI");
	dlg.AddPage(page19, "Handhistory Generator");
	dlg.AddPage(page11, "Logging");
	dlg.AddPage(page9,  "Perl");
	dlg.AddPage(page6,  "Poker Tracker v3");
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

BOOL CMainFrame::DestroyWindow() 
{
	WINDOWPLACEMENT wp;

	//unload dll
	p_dll_extension->Unload();

	StopThreads();

	// Save window position
	if (!p_flags_toolbar->IsButtonChecked(ID_MAIN_TOOLBAR_MINMAX)) 
	{
		GetWindowPlacement(&wp);
		preferences.SetValue(k_prefs_main_x, wp.rcNormalPosition.left);
		preferences.SetValue(k_prefs_main_y, wp.rcNormalPosition.top);
	}
	else 
	{
		preferences.SetValue(k_prefs_main_x, _table_view_size.left);
		preferences.SetValue(k_prefs_main_y, _table_view_size.top);
	}

	return CFrameWnd::DestroyWindow();
}

void CMainFrame::OnFileOpen() 
{
    COpenHoldemDoc *pDoc = (COpenHoldemDoc *)this->GetActiveDocument();   
   
    if (!pDoc->SaveModified())
        return;        // leave the original one

	CFileDialog			cfd(true);

	cfd.m_ofn.lpstrInitialDir = preferences.path_ohf();
  // http://msdn.microsoft.com/en-us/library/windows/desktop/ms646839%28v=vs.85%29.aspx
  cfd.m_ofn.lpstrFilter = "OpenHoldem Formula Files (*.ohf, *.oppl)\0*.ohf;*.oppl\0All files (*.*)\0*.*\0\0";
	cfd.m_ofn.lpstrTitle = "Select Formula file to OPEN";
	if (cfd.DoModal() == IDOK)
	{				
		pDoc->OnOpenDocument(cfd.GetPathName());
		pDoc->SetPathName(cfd.GetPathName());
		// Update window title, registry
		p_openholdem_title->UpdateTitle();
		preferences.SetValue(k_prefs_path_ohf, cfd.GetPathName());
		theApp.StoreLastRecentlyUsedFileList();
	}
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	__TRACE
	RECT			att_rect = {0}, wrect = {0};

	if (nIDEvent == HWND_CHECK_TIMER)
	{
		__TRACE
		if (!IsWindow(p_autoconnector->attached_hwnd()))
		{
			// Table disappeared
			p_autoplayer->EngageAutoplayer(false);
			p_autoconnector->Disconnect();
		}
	}

	else if (nIDEvent == ENABLE_BUTTONS_TIMER)
	{
		__TRACE
		// Autoplayer
		// Since OH 4.0.5 we support autoplaying immediatelly after connection
		// without the need to know the userchair to act on secondary formulas.
		if (p_symbol_engine_userchair != NULL
			&& p_autoconnector->IsConnected())
		{
			p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, true);
		}
		else
		{
			p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
		}

		// attach
		if (p_autoconnector->attached_hwnd() != NULL) 
		{
			p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_ATTACH_TOP, true);
			p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, true);
		}
		else 
		{
			p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_ATTACH_TOP, false);
			p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, false);
		}

		// Shoot replay frame
		if (p_autoconnector->attached_hwnd() != NULL)
			p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, true);
		else
			p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_SHOOTFRAME, false);

	}
	else if (nIDEvent == UPDATE_STATUS_BAR_TIMER) 
	{
		__TRACE
		p_openholdem_statusbar->OnUpdateStatusbar();
	}
	else if (nIDEvent == ATTACH_WINDOW_TIMER)
	{
		__TRACE
		::GetWindowRect(p_autoconnector->attached_hwnd(), &att_rect);
		GetWindowRect(&wrect);

		if (memcmp(&_prev_att_rect, &att_rect, sizeof(RECT))!=0 ||
				memcmp(&_prev_wrect, &wrect, sizeof(RECT))!=0)
		{
			::GetWindowRect(p_autoconnector->attached_hwnd(), &_prev_att_rect);
			GetWindowRect(&_prev_wrect);

			if (p_flags_toolbar->IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_TOP))
			{
				if (IsWindow(p_autoconnector->attached_hwnd()))
					MoveWindow(att_rect.left, att_rect.top-(wrect.bottom-wrect.top), att_rect.right-att_rect.left, wrect.bottom-wrect.top);
			}

			if (p_flags_toolbar->IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_BOTTOM))
			{
				if (IsWindow(p_autoconnector->attached_hwnd()))
					MoveWindow(att_rect.left, att_rect.bottom, att_rect.right-att_rect.left, wrect.bottom-wrect.top);
			}
		}
	}
	__TRACE
	CWnd::OnTimer(nIDEvent); 
	__TRACE
}

void CMainFrame::OnAutoplayer() 
{
	bool is_autoplaying = p_autoplayer->autoplayer_engaged();
	// Toggle the autoplayer-state
	p_autoplayer->EngageAutoplayer(!is_autoplaying);
}

void CMainFrame::OnValidator() 
{
	if (p_flags_toolbar->IsButtonChecked(ID_MAIN_TOOLBAR_VALIDATOR)) 
	{
		p_flags_toolbar->CheckButton(ID_MAIN_TOOLBAR_VALIDATOR, true);
		p_validator->SetEnabledManually(true);
	}
	else
	{
		p_flags_toolbar->CheckButton(ID_MAIN_TOOLBAR_VALIDATOR, false);
		p_validator->SetEnabledManually(false);
	}
}

void CMainFrame::OnUpdateStatus(CCmdUI *pCmdUI) 
{
	p_openholdem_statusbar->OnUpdateStatusbar();
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

	cfd.m_ofn.lpstrInitialDir = preferences.path_dll();
	cfd.m_ofn.lpstrFilter = "DLL Files (.dll)\0*.dll\0\0";
	cfd.m_ofn.lpstrTitle = "Select OpenHoldem DLL file to OPEN";

	if (cfd.DoModal() == IDOK) {
		p_dll_extension->Load(cfd.m_ofn.lpstrFile);
		preferences.SetValue(k_prefs_path_dll, cfd.GetPathName());
	}
}

void CMainFrame::OnMinMax(void) 
{
	RECT		crect = {0}, wrect = {0}, rectBar1 = {0}, rectBar2 = {0}, statusBar = {0};
	POINT		pt = {0};

	GetClientRect(&crect);
	GetWindowRect(&wrect);

	if (p_flags_toolbar->IsButtonChecked(ID_MAIN_TOOLBAR_MINMAX)) 
	{
		GetWindowRect(&_table_view_size);
		p_openholdem_statusbar->GetWindowRect(&statusBar);

		pt.x = (wrect.right - wrect.left) - crect.right;
		pt.y = (wrect.bottom - wrect.top) - crect.bottom;

		// After refactoring the toolbar(s) it is no longer that easy to get its size,
		// but as it is no longer moveable and dockable we can take a constant here.
		const int k_height_of_toolbar = 48;
		MoveWindow(wrect.left, wrect.top, wrect.right - wrect.left,
				   pt.y + k_height_of_toolbar + (statusBar.bottom-statusBar.top) - 2,
				   true);
	}
	else 
	{
		MoveWindow(_table_view_size.left, _table_view_size.top,
		  _table_view_size.right - _table_view_size.left,
			_table_view_size.bottom - _table_view_size.top, true);
	}
}
/*!!!!!
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) {
  // set the minimum tracking width
  // and the minimum tracking height of the window
  lpMMI->ptMinTrackSize.x = 300;
  lpMMI->ptMinTrackSize.y = 350;
}*/

void CMainFrame::OnAttachTop(void) 
{
	RECT	att_rect = {0}, wrect = {0};

	::GetWindowRect(p_autoconnector->attached_hwnd(), &att_rect);
	GetWindowRect(&wrect);

	if (p_flags_toolbar->IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_TOP)) 
	{
		// uncheck attach_bottom, if necessary
		p_flags_toolbar->CheckButton(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, false);

		MoveWindow(att_rect.left, att_rect.top-(wrect.bottom-wrect.top), att_rect.right-att_rect.left, wrect.bottom-wrect.top);
	}
}

void CMainFrame::OnAttachBottom(void)
{
	RECT	att_rect = {0}, wrect = {0};

	::GetWindowRect(p_autoconnector->attached_hwnd(), &att_rect);
	GetWindowRect(&wrect);

	if (p_flags_toolbar->IsButtonChecked(ID_MAIN_TOOLBAR_ATTACH_BOTTOM)) 
	{
		// uncheck attach_top, if necessary
		p_flags_toolbar->CheckButton(ID_MAIN_TOOLBAR_ATTACH_TOP, false);

		MoveWindow(att_rect.left, att_rect.bottom, att_rect.right-att_rect.left, wrect.bottom-wrect.top);
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

void CMainFrame::OnClickedFlags()
{
	p_flags_toolbar->OnClickedFlags();
}

void CMainFrame::OnPerlLoadSpecificFormula() 
{
	CFileDialog			cfd(true);

	cfd.m_ofn.lpstrInitialDir = preferences.path_perl();
	cfd.m_ofn.lpstrFilter = "Perl Scripts (*.pl)\0*.pl\0Perl Modules (*.pm)\0*.pm\0All Files (*.*)\0*.*\0\0";
	cfd.m_ofn.lpstrTitle = "Select Perl formula file to OPEN";
	if (cfd.DoModal() == IDOK)
	{
		p_perl->LoadFormulaFile(cfd.m_ofn.lpstrFile);
		preferences.SetValue(k_prefs_path_perl, cfd.GetPathName());
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


void CMainFrame::OnUpdateViewShootreplayframe(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(p_autoconnector->attached_hwnd() != NULL);
}

void CMainFrame::OnUpdateMenuPerlLoad(CCmdUI* pCmdUI)
{
	if (p_perl->IsAFormulaLoaded()) 
	{
		pCmdUI->SetText("&Unload Formula\tF7");
	}
	else 
	{
		pCmdUI->SetText("&Load Formula\tF7");
	}

	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
}

void CMainFrame::OnUpdateMenuPerlLoadSpecificFormula(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
}

void CMainFrame::OnUpdateMenuPerlReloadFormula(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(p_perl->IsAFormulaLoaded() 
		&& !p_autoplayer->autoplayer_engaged());
}

void CMainFrame::OnUpdateMenuPerlCheckSyntax(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(p_perl->IsAFormulaLoaded());
}

void CMainFrame::OnUpdateMenuPerlEditMainFormula(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(p_perl->IsAFormulaLoaded()
		&& !p_autoplayer->autoplayer_engaged());
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

void CMainFrame::OnHelp()
{
  p_flags_toolbar->CheckButton(ID_MAIN_TOOLBAR_HELP, false);
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



