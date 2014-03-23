//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

// OpenHoldem.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "OpenHoldem.h"

#include <psapi.h>
#include <windows.h>

#include "..\CTablemap\CTablemap.h"
#include "..\CTablemap\CTableMapAccess.h"
#include "CAutoConnector.h"
#include "CAutoConnectorThread.h"
#include "CFilenames.h"
#include "CGrammar.h"
#include "COpenHoldemHopperCommunication.h"
#include "CopenHoldemTitle.h"
#include "CPreferences.h"
#include "CSessionCounter.h"
#include "DialogFormulaScintilla.h"
#include "MainFrm.h"
#include "OH_MessageBox.h"
#include "OpenHoldemDoc.h"
#include "OpenHoldemView.h"
#include "Singletons.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Supports MRU
AFX_STATIC_DATA const TCHAR _afxFileSection[] = _T("Recent File List");
AFX_STATIC_DATA const TCHAR _afxFileEntry[] = _T("File%d");
AFX_STATIC_DATA const TCHAR _afxPreviewSection[] = _T("Settings");
AFX_STATIC_DATA const TCHAR _afxPreviewEntry[] = _T("PreviewPages");

// COpenHoldemApp
extern bool Scintilla_RegisterClasses(void *hInstance);
extern bool Scintilla_ReleaseResources();

BEGIN_MESSAGE_MAP(COpenHoldemApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &COpenHoldemApp::OnAppAbout)
	ON_COMMAND(ID_HELP_FORCECRASH, &COpenHoldemApp::OnForceCrash)	
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_FINISH_INITIALIZATION, &COpenHoldemApp::FinishInitialization)
END_MESSAGE_MAP()

// COpenHoldemApp construction
COpenHoldemApp::COpenHoldemApp()
{
	_dll_scraper_process_message = NULL;
	_dll_scraper_override = NULL;
}

// COpenHoldemApp destruction
COpenHoldemApp::~COpenHoldemApp()
{
}

// The one and only COpenHoldemApp object
COpenHoldemApp theApp;


// COpenHoldemApp initialization
BOOL COpenHoldemApp::InitInstance()
{
	// Since OH 4.0.0 we always use an ini-file,
	// the one and only in our OH-directory,
	// no matter how it is named.
	// For the technical details please see:
	// http://msdn.microsoft.com/de-de/library/xykfyy20(v=vs.80).aspx
	InstantiateSomeSingletonsForVeryEarlyUseInInitInstance();

	Scintilla_RegisterClasses(AfxGetInstanceHandle());

	// Initialize richedit2 library
	AfxInitRichEdit2();

	// Change class name of Dialog
	WNDCLASS wc;
	GetClassInfo(AfxGetInstanceHandle(), "#32770", &wc);

	wc.lpszClassName = "OpenHoldemFormula";
	wc.hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	RegisterClass(&wc);

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	free((void*)m_pszProfileName);
	m_pszProfileName = _strdup(p_filenames->IniFilePath().GetString());
	preferences.LoadPreferences();
	
	// Classes
	if (!p_sessioncounter) p_sessioncounter = new CSessionCounter;
	// Start logging immediatelly after the loading the preferences
	// and initializing the sessioncounter, which is necessary for 
	// the filename of the log (oh_0.log, etc).
	start_log();
	InstantiateAllSingletons();

	write_log(preferences.debug_openholdem(), "[OpenHoldem] Going to load mouse.DLL\n");
	// mouse.dll - failure in load is fatal
	_mouse_dll = LoadLibrary("mouse.dll");
	if (_mouse_dll==NULL)
	{
		CString		t = "";
		t.Format("Unable to load mouse.dll, error: %d\n\nExiting.", GetLastError());
		OH_MessageBox_Error_Warning(t, "OpenHoldem mouse.dll ERROR");
		return false;
	}
	else
	{
		_dll_mouse_process_message = (mouse_process_message_t) GetProcAddress(_mouse_dll, "ProcessMessage");
		_dll_mouse_click = (mouse_click_t) GetProcAddress(_mouse_dll, "MouseClick");
		_dll_mouse_click_drag = (mouse_clickdrag_t) GetProcAddress(_mouse_dll, "MouseClickDrag");

		if (_dll_mouse_process_message==NULL || _dll_mouse_click==NULL || _dll_mouse_click_drag==NULL)
		{
			CString		t = "";
			t.Format("Unable to find all symbols in mouse.dll");
			OH_MessageBox_Error_Warning(t, "OpenHoldem mouse.dll ERROR");

			FreeLibrary(_mouse_dll);
			_mouse_dll = NULL;
			return false;
		}
	
	write_log(preferences.debug_openholdem(), "[OpenHoldem] Going to load keyboard.DLL\n");}

	// keyboard.dll - failure in load is fatal
	_keyboard_dll = LoadLibrary("keyboard.dll");
	if (_keyboard_dll==NULL)
	{
		CString		t = "";
		t.Format("Unable to load keyboard.dll, error: %d\n\nExiting.", GetLastError());
		OH_MessageBox_Error_Warning(t, "OpenHoldem keyboard.dll ERROR");
		return false;
	}
	else
	{
		_dll_keyboard_process_message = (keyboard_process_message_t) GetProcAddress(_keyboard_dll, "ProcessMessage");
		_dll_keyboard_sendstring = (keyboard_sendstring_t) GetProcAddress(_keyboard_dll, "SendString");
		_dll_keyboard_sendkey = (keyboard_sendkey_t) GetProcAddress(_keyboard_dll, "SendKey");

		if (_dll_keyboard_process_message==NULL || _dll_keyboard_sendstring==NULL || _dll_keyboard_sendkey==NULL)
		{
			CString		t = "";
			t.Format("Unable to find all symbols in keyboard.dll");
			OH_MessageBox_Error_Warning(t, "OpenHoldem keyboard.dll ERROR");

			FreeLibrary(_keyboard_dll);
			_keyboard_dll = NULL;
			return false;
		}
	}

	LoadLastRecentlyUsedFileList();
	// Register the application's document templates.  Document templates
	// serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;

	write_log(preferences.debug_openholdem(), "[OpenHoldem] Going to create CSingleDocTemplate()\n");
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(COpenHoldemDoc),
		RUNTIME_CLASS(CMainFrame),	   // main SDI frame window
		RUNTIME_CLASS(COpenHoldemView));

	if (!pDocTemplate)
	{
		write_log(preferences.debug_openholdem(), "[OpenHoldem] Creating CSingleDocTemplate() failed\n");
		return FALSE;
	}

	write_log(preferences.debug_openholdem(), "[OpenHoldem] Going to AddDocTemplate()\n");
	AddDocTemplate(pDocTemplate);

	write_log(preferences.debug_openholdem(), "[OpenHoldem] Going to EnableShellOpen()\n");
	EnableShellOpen();
	write_log(preferences.debug_openholdem(), "[OpenHoldem] Going to RegisterShellFileTypes(false)\n");
	RegisterShellFileTypes(false);
	OpenLastRecentlyUsedFile();

	write_log(preferences.debug_openholdem(), "[OpenHoldem] m_pMainWnd = %i\n",
		m_pMainWnd);

	write_log(preferences.debug_openholdem(), "[OpenHoldem] Posting message that finishes initialization later\n");
	FinishInitialization();

	write_log(preferences.debug_openholdem(), "[OpenHoldem] InitInstance done\n");
	return TRUE;
}

void COpenHoldemApp::FinishInitialization()
{
	write_log(preferences.debug_openholdem(), "[OpenHoldem] FinishInitialization()\n");
	write_log(preferences.debug_openholdem(), "[OpenHoldem] m_pMainWnd = %i\n",
		m_pMainWnd);

	assert(p_openholdem_title != NULL);
	p_openholdem_title->UpdateTitle();

	// The one and only window has been initialized, so show and update it
	if (preferences.gui_start_minimized())
	{
		m_pMainWnd->ShowWindow(SW_MINIMIZE);
	}
	else
	{
		m_pMainWnd->ShowWindow(SW_SHOW);
	}
	
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	// In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Bring main window to front
	m_pMainWnd->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	m_pMainWnd->SetActiveWindow();
	m_pMainWnd->SetFocus();
	m_pMainWnd->SetForegroundWindow();

	write_log(preferences.debug_openholdem(), "[OpenHoldem] Going to connect\n");
	// autoconnect on start, if preferred
	if (preferences.autoconnector_when_to_connect() == k_AutoConnector_Connect_Once)
	{
		p_autoconnector->Connect(NULL);
	}
	// Start thread anyway; permanent connection might be enabled later via preferences.
	p_autoconnectorthread->StartThread();	
}

int COpenHoldemApp::ExitInstance()
{
	DeleteAllSingletons();
	stop_log();
	Scintilla_ReleaseResources();
	return CWinApp::ExitInstance();
}

void COpenHoldemApp::UnloadScraperDLL()
{
	if (_scraper_dll)
		FreeLibrary(_scraper_dll);
	_scraper_dll = NULL;
	_dll_scraper_process_message = NULL;
	_dll_scraper_override = NULL;
}

// CDlgAbout dialog used for App About
class CDlgAbout : public CDialog 
{
public:
	CDlgAbout();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};


CDlgAbout::CDlgAbout() : CDialog(CDlgAbout::IDD) 
{
}


void CDlgAbout::DoDataExchange(CDataExchange* pDX) 
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgAbout, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void COpenHoldemApp::OnAppAbout() 
{
	CDlgAbout aboutDlg;
	aboutDlg.DoModal();
}

void COpenHoldemApp::OnForceCrash() 
{
	int choice = OH_MessageBox_Interactive("Do you REALLY want to CRASH?", 
		"CONFIRMATION", MB_YESNO | MB_DEFBUTTON2 | MB_ICONEXCLAMATION | MB_TOPMOST);
	if (choice == IDYES) 
	{
		// FORCE A CRASH
		int *invalid_memory_access = NULL;
		*invalid_memory_access = 0;
	}
}

void COpenHoldemApp::LoadLastRecentlyUsedFileList()
{
	//!!!
	// Added due to inability to get standard LoadStdProfileSettings working properly
	ASSERT_VALID(this);
	ASSERT(m_pRecentFileList == NULL);

	write_log(preferences.debug_openholdem(), "[OpenHoldem] Going to load file history\n");
	if (k_number_of_last_recently_used_files_in_file_menu > 0) 
	{
		// create file MRU since nMaxMRU not zero
		m_pRecentFileList = new CRecentFileList(0, _afxFileSection, _afxFileEntry, 
			k_number_of_last_recently_used_files_in_file_menu);
		m_pRecentFileList->ReadList();
	}
	// 0 by default means not set
	m_nNumPreviewPages = GetProfileInt(_afxPreviewSection, _afxPreviewEntry, 0);
}

void COpenHoldemApp::StoreLastRecentlyUsedFileList()
{
	m_pRecentFileList->WriteList();
}

void COpenHoldemApp::OpenLastRecentlyUsedFile()
{
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	write_log(preferences.debug_openholdem(), "[OpenHoldem] Going to open last recently used file\n");
	// Open the most recently used file. (First on the MRU list.) Get the last
	// file from the registry. We need not account for cmdInfo.m_bRunAutomated and
	// cmdInfo.m_bRunEmbedded as they are processed before we get here.
	
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
		CString sLastPath(GetProfileString(_afxFileSection, "File1"));

		if (!sLastPath.IsEmpty())
		{
			write_log(preferences.debug_openholdem(), "[OpenHoldem] Last path: %s\n", 
				sLastPath);

			CFile f;
			// If file is there, set to open!
			if (f.Open(sLastPath, CFile::modeRead | CFile::shareDenyWrite))
			{
				cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
				cmdInfo.m_strFileName = sLastPath;
				f.Close();
			}
		}
	}
	write_log(preferences.debug_openholdem(), "[OpenHoldem] Going to dispatch command-line\n");
	// Dispatch commands specified on the command line.  Will fail if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.	
	if (!ProcessShellCommand(cmdInfo))
	{
		write_log(preferences.debug_openholdem(), "[OpenHoldem] Dispatching command-line failed\n");
	}
}