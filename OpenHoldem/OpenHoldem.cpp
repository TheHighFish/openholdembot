// OpenHoldem.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <psapi.h>
#include <windows.h>

#include "OpenHoldem.h"
#include "OpenHoldemDoc.h"
#include "OpenHoldemView.h"
#include "MainFrm.h"

#include "CScraper.h"
#include "CSymbols.h"
#include "CAutoplayer.h"
#include "CIteratorThread.h"
#include "CHeartbeatThread.h"
#include "CPokerTrackerThread.h"
#include "CGlobal.h"

#include "DialogFormulaScintilla.h"
#include "grammar.h"
#include "PokerPro.h"
#include "Perl.hpp"
#include "CSessionCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char	_startup_path[MAX_PATH];

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
COpenHoldemApp::COpenHoldemApp()
{
    __SEH_SET_EXCEPTION_HANDLER

    __SEH_HEADER

    // Save startup directory
    ::GetCurrentDirectory(MAX_PATH - 1, _startup_path);

    __SEH_LOGFATAL("COpenHoldemApp::Constructor :\n");
}

// COpenHoldemApp destruction
COpenHoldemApp::~COpenHoldemApp()
{
    __SEH_HEADER

    __SEH_LOGFATAL("COpenHoldemApp::Destructor :\n");
}

// The one and only COpenHoldemApp object
COpenHoldemApp theApp;

// COpenHoldemApp initialization
BOOL COpenHoldemApp::InitInstance()
{
    __SEH_HEADER
    
	// Critical sections
	InitializeCriticalSectionAndSpinCount(&cs_iterator, 4000);
	InitializeCriticalSectionAndSpinCount(&cs_calc_f$symbol, 4000);
    InitializeCriticalSectionAndSpinCount(&cs_parse, 4000);

	// Classes
	if (!p_global)  p_global = new CGlobal;
	if (!p_scraper)  p_scraper = new CScraper;
	if (!p_symbols)  p_symbols = new CSymbols;
	if (!p_autoplayer)  p_autoplayer = new CAutoplayer(false, "OHAntiColl");
	if (!the_Perl_Interpreter)  the_Perl_Interpreter = new Perl;

	Scintilla_RegisterClasses(AfxGetInstanceHandle());

    // Initialize richedit2 library
    AfxInitRichEdit2();

    // Change class name of Dialog
    WNDCLASS wc;
    GetClassInfo(AfxGetInstanceHandle(), "#32770", &wc);

    wc.lpszClassName = "OpenHoldemFormula";
    wc.hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
    RegisterClass(&wc);

	p_global->set_session_id(SessionCounter.session_id());

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
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
    {
        CString sLastPath(GetProfileString(_afxFileSection, "File1"));

        if (! sLastPath.IsEmpty())
        {
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

    p_global->set_h_main_frame(m_pMainWnd->GetSafeHwnd());

    // Bring main window to front
    m_pMainWnd->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    m_pMainWnd->SetActiveWindow();
    m_pMainWnd->SetFocus();
    m_pMainWnd->SetForegroundWindow();

    return TRUE;

    __SEH_LOGFATAL("COpenHoldemApp::InitInstance :\n");
}

int COpenHoldemApp::ExitInstance()
{
    __SEH_HEADER

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

	// critical sections
	DeleteCriticalSection(&cs_iterator);
	DeleteCriticalSection(&cs_calc_f$symbol);
    DeleteCriticalSection(&cs_parse);

	// classes
	if (p_global)  delete p_global;
	if (p_scraper)  delete p_scraper;
	if (p_symbols)  delete p_symbols;
	if (p_autoplayer)  delete p_autoplayer;
	if (the_Perl_Interpreter)  delete the_Perl_Interpreter;

    stop_log();

    Scintilla_ReleaseResources();

    return CWinApp::ExitInstance();

    __SEH_LOGFATAL("COpenHoldemApp::ExitInstance :\n");
}


// CDlgAbout dialog used for App About

class CDlgAbout : public CDialog 
{
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

// Added due to inability to get standard LoadStdProfileSettings working properly
void COpenHoldemApp::MyLoadStdProfileSettings(UINT nMaxMRU) 
{
    __SEH_HEADER

    ASSERT_VALID(this);
    ASSERT(m_pRecentFileList == NULL);

    if (nMaxMRU != 0) 
	{
        // create file MRU since nMaxMRU not zero
        m_pRecentFileList = new CRecentFileList(0, _afxFileSection, _afxFileEntry, nMaxMRU);
        m_pRecentFileList->ReadList();
    }
    // 0 by default means not set
    m_nNumPreviewPages = GetProfileInt(_afxPreviewSection, _afxPreviewEntry, 0);

    __SEH_LOGFATAL("COpenHoldemApp::MyLoadStdProfileSettings :\n");
}
