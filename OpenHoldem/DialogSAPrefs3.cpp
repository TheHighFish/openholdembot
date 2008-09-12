// DialogSAPrefs3.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs3.h"
#include "CPreferences.h"

// CDlgSAPrefs3 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs3, CSAPrefsSubDlg)

CDlgSAPrefs3::CDlgSAPrefs3(CWnd* pParent /*=NULL*/)
		: CSAPrefsSubDlg(CDlgSAPrefs3::IDD, pParent)
{
	__SEH_SET_EXCEPTION_HANDLER
}

CDlgSAPrefs3::~CDlgSAPrefs3()
{
}

void CDlgSAPrefs3::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOAD_DLL_ON_STARTUP, m_LoadDllOnStartup);
	DDX_Control(pDX, IDC_DLLNAME, m_DllName);
	DDX_Control(pDX, IDC_ALWAYS_SEND_STATE, m_AlwaysSendState);
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs3, CSAPrefsSubDlg)
END_MESSAGE_MAP()

// CDlgSAPrefs3 message handlers
BOOL CDlgSAPrefs3::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();
	CString		text = "";

	m_AlwaysSendState.SetCheck(prefs.dll_always_send_state() ? BST_CHECKED : BST_UNCHECKED);
	m_LoadDllOnStartup.SetCheck(prefs.dll_load_on_startup() ? BST_CHECKED : BST_UNCHECKED);
	m_DllName.SetWindowText(prefs.dll_name());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs3::OnOK()
{
	CString			text = "";

	prefs.set_dll_always_send_state(m_AlwaysSendState.GetCheck()==BST_CHECKED ? true : false);
	prefs.set_dll_load_on_startup(m_LoadDllOnStartup.GetCheck()==BST_CHECKED ? true : false);

	m_DllName.GetWindowText(text);
	prefs.set_dll_name(text);

	CSAPrefsSubDlg::OnOK();
}
