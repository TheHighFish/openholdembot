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
#include "CFlagsToolbar.h"
#include "CFormula.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "COpenHoldemHopperCommunication.h"
#include "COpenHoldemStatusbar.h"
#include "CPerl.hpp"
#include "CPreferences.h"
#include "CProblemSolver.h"
#include "CReplayFrame.h"
#include "CScraper.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbols.h"
#include "CTableLimits.h"
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
#include "DialogSAPrefs18.h"
#include "DialogSAPrefs19.h"
#include "DialogSAPrefs20.h"
#include "DialogSAPrefs21.h"
#include "DialogScraperOutput.h"
#include "inlines/eval.h"
#include "MagicNumbers.h"
#include "OH_MessageBox.h"
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
	ON_COMMAND(ID_HELP_PROBLEMSOLVER, &CMainFrame::OnHelpProblemSolver)

	// Main toolbar 
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_AUTOPLAYER, &CMainFrame::OnAutoplayer)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_FORMULA, &CMainFrame::OnEditFormula)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_VALIDATOR, &CMainFrame::OnValidator)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_MINMAX, &CMainFrame::OnMinMax)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_ATTACH_TOP, &CMainFrame::OnAttachTop)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_ATTACH_BOTTOM, &CMainFrame::OnAttachBottom)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, &CMainFrame::OnScraperOutput)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_SHOOTFRAME, &CMainFrame::OnViewShootreplayframe)
	ON_BN_CLICKED(ID_MAIN_TOOLBAR_HELP, &CMainFrame::OnHelp)

	ON_WM_TIMER()

	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_READY,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_PLCARDS,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_COMCARDS,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_POKERHAND,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_HANDRANK,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_PRWIN,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_NOPP,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_NIT,OnUpdateStatus)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS_ACTION,OnUpdateStatus)

	ON_MESSAGE(WMA_SETWINDOWTEXT, &COpenHoldemHopperCommunication::OnSetWindowText)
	ON_MESSAGE(WMA_DOCONNECT,     &COpenHoldemHopperCommunication::OnConnectMessage)
	ON_MESSAGE(WMA_DODISCONNECT,  &COpenHoldemHopperCommunication::OnDisconnectMessage)
	ON_MESSAGE(WMA_CONNECTEDHWND, &COpenHoldemHopperCommunication::OnConnectedHwndMessage)
	ON_MESSAGE(WMA_SETFLAG,       &COpenHoldemHopperCommunication::OnSetFlagMessage)
	ON_MESSAGE(WMA_RESETFLAG,     &COpenHoldemHopperCommunication::OnResetFlagMessage)

	ON_WM_SETCURSOR()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

// CMainFrame construction/destruction
CMainFrame::CMainFrame() 
{
	_autoplay_pressed = false;
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

	if (prefs.simple_window_title())
		SetMainWindowTitle("");

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
		wnd.lpfnWndProc		= ::DefWindowProc;
		wnd.cbClsExtra		= wnd.cbWndExtra = 0;
		wnd.hInstance		= hInst;
		wnd.hIcon			= AfxGetApp()->LoadIcon(IDI_ICON1);
		wnd.hCursor			= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wnd.hbrBackground	= (HBRUSH) (COLOR_3DFACE + 1);
		wnd.lpszMenuName	= NULL;
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
		if (m_formulaScintillaDlg->m_dirty)
		{
			if (OH_MessageBox_Interactive(
				"The Formula Editor has un-applied changes.\n"
				"Really exit?", 
				"Formula Editor", MB_ICONWARNING|MB_YESNO) == IDNO)
			{
				p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_FORMULA, true);
				return;
			}
		}

		BOOL	bWasShown = ::IsWindow(m_formulaScintillaDlg->m_hWnd) && m_formulaScintillaDlg->IsWindowVisible();

		m_formulaScintillaDlg->DestroyWindow();

		if (bWasShown)
			return;
	}

	m_formulaScintillaDlg = new CDlgFormulaScintilla(this);
	m_formulaScintillaDlg->Create(CDlgFormulaScintilla::IDD,this);
	m_formulaScintillaDlg->ShowWindow(SW_SHOW);
	p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_FORMULA, true);
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
	p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_SCRAPER_OUTPUT, true);
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
	CDlgSAPrefs18 page18;
	CDlgSAPrefs19 page19;
	CDlgSAPrefs20 page20;
	CDlgSAPrefs21 page21;

	// add pages
	dlg.AddPage(page14, "Auto-Connector");
	dlg.AddPage(page2,  "Autoplayer");
	dlg.AddPage(page10, "Chat");
	dlg.AddPage(page17, "Configuration Check");
	dlg.AddPage(page20, "Debugging");
	dlg.AddPage(page3,  "DLL Extension");
	dlg.AddPage(page15, "GUI");
	dlg.AddPage(page19, "Handhistory Generator");
	dlg.AddPage(page7,  "ICM");
	dlg.AddPage(page18, "Lazy Scraping");
	dlg.AddPage(page11, "Logging");
	dlg.AddPage(page9,  "Perl");
	dlg.AddPage(page6,  "Poker Tracker");
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
	p_dll_extension->UnloadDll();

	StopThreads();

	// Save window position
	if (!p_flags_toolbar->IsButtonChecked(ID_MAIN_TOOLBAR_MINMAX)) 
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
    COpenHoldemDoc *pDoc = (COpenHoldemDoc *)this->GetActiveDocument();   
   
    if (!pDoc->SaveModified())
        return;        // leave the original one

	CFileDialog			cfd(true);

	cfd.m_ofn.lpstrInitialDir = prefs.path_ohf();
	cfd.m_ofn.lpstrFilter = "OpenHoldem Files (.ohf)\0*.ohf\0All files (*.*)\0*.*\0\0";
	cfd.m_ofn.lpstrTitle = "Select Formula file to OPEN";
	if (cfd.DoModal() == IDOK)
	{				
		pDoc->OnOpenDocument(cfd.GetPathName());
		pDoc->SetPathName(cfd.GetPathName());
		// Update window title, registry
		SetMainWindowTitle(cfd.GetFileTitle() + " - " + CString(MAKEINTRESOURCE(AFX_IDS_APP_TITLE)));
		prefs.set_path_ohf(cfd.GetPathName());
	}
}


void CMainFrame::OnTimer(UINT nIDEvent) 
{
	RECT			att_rect = {0}, wrect = {0};

	if (nIDEvent == HWND_CHECK_TIMER)
	{
		if (!IsWindow(p_autoconnector->attached_hwnd()))
		{
			// Table disappeared
			p_autoconnector->Disconnect();
			_autoplay_pressed = false;
		}
	}

	else if (nIDEvent == ENABLE_BUTTONS_TIMER)
	{

		// Autoplayer
		if (p_symbol_engine_userchair != NULL
			&& p_symbol_engine_userchair->userchair_confirmed())
		{
			p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, true);
		}
		else
		{
			p_flags_toolbar->EnableButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
		}

		// Automatically start autoplayer, if set in preferences
		if (prefs.engage_autoplayer() && !p_flags_toolbar->IsButtonChecked(ID_MAIN_TOOLBAR_AUTOPLAYER) &&
				((p_symbol_engine_userchair->userchair_confirmed() 
				&& !p_autoplayer->autoplayer_engaged())))
		{
			if (!_autoplay_pressed)
			{
				p_flags_toolbar->CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, true);
				OnAutoplayer();
				_autoplay_pressed = true;
			}
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
		p_openholdem_statusbar->OnUpdateStatusbar();
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
}

void CMainFrame::OnAutoplayer() 
{
	if (p_flags_toolbar->IsButtonChecked(ID_MAIN_TOOLBAR_AUTOPLAYER)) 
	{
		// calc hand lists
		p_formula->CreateHandListMatrices();

		// one last parse - do not engage if parse fails
		if (p_formula->ParseAllFormula(PMainframe()->GetSafeHwnd()))
		{
			p_autoplayer->set_autoplayer_engaged(true);
		}
		else 
		{
			p_autoplayer->set_autoplayer_engaged(false);
			p_flags_toolbar->CheckButton(ID_MAIN_TOOLBAR_AUTOPLAYER, false);
		}
	}
	else 
	{
		p_autoplayer->set_autoplayer_engaged(false);
	}
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

/*
// ??? conflict and removed in maintenance. Needed?
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		if (m_formulaScintillaDlg)
		{
			if (m_formulaScintillaDlg->m_dirty)
			{
				if (OH_MessageBox_Interactive("The Formula Editor has un-applied (and unsaved) changes.\n"
							   "Really exit?", "Unsaved formula warning", MB_YESNO) == IDNO)
				{
					return;
				}
			}
		}
	}
	CFrameWnd::OnSysCommand(nID, lParam);
}
*/
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
	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
}

void CMainFrame::OnUpdateMenuFileOpen(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
}

void CMainFrame::OnUpdateMenuDllLoad(CCmdUI* pCmdUI)
{
	if (p_dll_extension->IsDllLoaded())
		pCmdUI->SetText("&Unload\tF4");

	else
		pCmdUI->SetText("&Load\tF4");

	pCmdUI->Enable(!p_autoplayer->autoplayer_engaged());
}

void CMainFrame::OnUpdateDllLoadspecificfile(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(p_dll_extension->IsDllLoaded()
		&& !p_autoplayer->autoplayer_engaged());
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
		OH_MessageBox_Interactive("\"OpenHoldem_Manual.chm\" not found.\nPlease put it into your OpenHoldem folder.", "Error", 0);
	}
	else 
	{
		long long int RetValue = long long int(ShellExecute(NULL, "open", "OpenHoldem_Manual.chm", NULL, NULL, SW_SHOW));
		if (RetValue <= 32)
		{
			OH_MessageBox_Interactive("Error opening help-file", "Error", 0);
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