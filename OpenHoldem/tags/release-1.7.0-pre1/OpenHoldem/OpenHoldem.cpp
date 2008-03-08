// OpenHoldem.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <psapi.h>

#include "OpenHoldem.h"
#include "MainFrm.h"

#include "OpenHoldemDoc.h"
#include "OpenHoldemView.h"
#include "DialogFormulaScintilla.h"
#include "debug.h"
#include "global.h"
#include "threads.h"
#include "PokerPro.h"
#include "pokertracker.h"

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
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// COpenHoldemApp construction
COpenHoldemApp::COpenHoldemApp() {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif
	
#ifdef SEH_ENABLE
	try {
#endif
		// Critical sections
		InitializeCriticalSection(&cs_prwin);
		InitializeCriticalSection(&cs_heartbeat);
		InitializeCriticalSection(&cs_updater);		
		InitializeCriticalSection(&cs_calc_f$symbol);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("COpenHoldemApp::Constructor :\n"); 
		throw;
	}
#endif
}

// COpenHoldemApp destruction
COpenHoldemApp::~COpenHoldemApp() {
#ifdef SEH_ENABLE
	try {
#endif
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("COpenHoldemApp::Destructor :\n"); 
		throw;
	}
#endif
}

// The one and only COpenHoldemApp object
COpenHoldemApp theApp;

// COpenHoldemApp initialization
BOOL COpenHoldemApp::InitInstance() 
{
#ifdef SEH_ENABLE
	try {
#endif
		HMODULE hMod;
		HANDLE hProcess;
		DWORD curprocid, aProcesses[1024], cbNeeded, cProcesses;
		char sCurProcessName[MAX_PATH], sProcessName[MAX_PATH];
		int i;
			
		Scintilla_RegisterClasses(AfxGetInstanceHandle());

		// Initialize richedit2 library
		AfxInitRichEdit2();

		// Change class name of Dialog
		WNDCLASS wc;
		GetClassInfo(AfxGetInstanceHandle(), "#32770", &wc);
		
		wc.lpszClassName = "OpenHoldemFormula";
		wc.hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
		RegisterClass(&wc);

		// Figure out our session number - get name of current process
		curprocid = GetCurrentProcessId();
		hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, curprocid );
		if (hProcess != NULL) 
		{
			if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) ) 
			{
				GetModuleBaseName( hProcess, hMod, sCurProcessName, MAX_PATH );
			}
		}
		CloseHandle(hProcess);

		// Noew look through process list and count number of matching processes
		EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded );
		cProcesses = cbNeeded / sizeof(DWORD);
		global.sessionnum = 0;
		for (i=0; i<(int) cProcesses; i++) 
		{
			hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, aProcesses[i] );
			if (hProcess != NULL) 
			{
				if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) ) 
				{
					GetModuleBaseName( hProcess, hMod, sProcessName, MAX_PATH );
				}
			}
			CloseHandle(hProcess);

			if (strcmp(sCurProcessName, sProcessName)==0) 
			{
				global.sessionnum++;
			}
		}

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

		// Standard initialization
		// If you are not using these features and wish to reduce the size
		// of your final executable, you should remove from the following
		// the specific initialization routines you do not need
		// Change the registry key under which our settings are stored
		SetRegistryKey(_T("OpenHoldem"));

		MyLoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
		if (m_pRecentFileList == NULL)
			AfxMessageBox("Still NULL");
		// Register the application's document templates.  Document templates
		//  serve as the connection between documents, frame windows and views
		CSingleDocTemplate* pDocTemplate;

		pDocTemplate = new CSingleDocTemplate(
			IDR_MAINFRAME,
			RUNTIME_CLASS(COpenHoldemDoc),
			RUNTIME_CLASS(CMainFrame),       // main SDI frame window
			RUNTIME_CLASS(COpenHoldemView));
		if (!pDocTemplate)
			return FALSE;
		AddDocTemplate(pDocTemplate);

		// Enable DDE Execute open
		EnableShellOpen();
		RegisterShellFileTypes(TRUE);

		// Parse command line for standard shell commands, DDE, file open
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		// Open the most recently saved file. (First on the MRU list.) Get the last
		// file from the registry. We need not account for cmdInfo.m_bRunAutomated and
		// cmdInfo.m_bRunEmbedded as they are processed before we get here.
		if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew) {
			CString sLastPath(GetProfileString(_afxFileSection, "File1"));

			if (! sLastPath.IsEmpty()) {
				CFile f;

				// If file is there, set to open!
				if (f.Open(sLastPath, CFile::modeRead)) {
					cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
					cmdInfo.m_strFileName = sLastPath;
				}
			}
		}

		// Dispatch commands specified on the command line.  Will return FALSE if
		// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
		if (!ProcessShellCommand(cmdInfo))
			return FALSE;

		// The one and only window has been initialized, so show and update it
		m_pMainWnd->ShowWindow(SW_SHOW);
		m_pMainWnd->UpdateWindow();
		// call DragAcceptFiles only if there's a suffix
		//  In an SDI app, this should occur after ProcessShellCommand
		// Enable drag/drop open
		m_pMainWnd->DragAcceptFiles();

		global.hMainFrame = m_pMainWnd->GetSafeHwnd();

		// Seed RNG
		LARGE_INTEGER	qpc;
		QueryPerformanceCounter(&qpc);
		srand(qpc.LowPart);

		return TRUE;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("COpenHoldemApp::InitInstance :\n"); 
		throw;
	}
#endif
}

int COpenHoldemApp::ExitInstance()
{
#ifdef SEH_ENABLE
	try {
#endif
		bool	upd = true;
		int		updcount=0;

		// wait for an update cycle to finish, if necessary
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

		DeleteCriticalSection(&cs_prwin);
		DeleteCriticalSection(&cs_heartbeat);
		DeleteCriticalSection(&cs_updater);
		DeleteCriticalSection(&cs_calc_f$symbol);

		stop_log();

		Scintilla_ReleaseResources();

		return CWinApp::ExitInstance();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("COpenHoldemApp::ExitInstance :\n"); 
		throw;
	}
#endif
}


// CDlgAbout dialog used for App About

class CDlgAbout : public CDialog {
public:
	CDlgAbout();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CDlgAbout::CDlgAbout() : CDialog(CDlgAbout::IDD) {
}

void CDlgAbout::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgAbout, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void COpenHoldemApp::OnAppAbout() {
	CDlgAbout aboutDlg;
	aboutDlg.DoModal();
}

// Added due to inability to get standard LoadStdProfileSettings working properly
void COpenHoldemApp::MyLoadStdProfileSettings(UINT nMaxMRU) {
#ifdef SEH_ENABLE
	try {
#endif
		ASSERT_VALID(this);
		ASSERT(m_pRecentFileList == NULL);

		if (nMaxMRU != 0) {
			// create file MRU since nMaxMRU not zero
			m_pRecentFileList = new CRecentFileList(0, _afxFileSection, _afxFileEntry, nMaxMRU);
			m_pRecentFileList->ReadList();
		}
		// 0 by default means not set
		m_nNumPreviewPages = GetProfileInt(_afxPreviewSection, _afxPreviewEntry, 0);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("COpenHoldemApp::MyLoadStdProfileSettings :\n"); 
		throw;
	}
#endif
}
