// DialogSAPrefs10.cpp : implementation file
//

#include "stdafx.h"
#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs11.h"
#include "global.h"
#include "Registry.h"

#define MAX_MAX_LOG 1000000

// DialogSAPrefs11 dialog


IMPLEMENT_DYNAMIC(CDlgSAPrefs11, CDialog)

CDlgSAPrefs11::CDlgSAPrefs11(CWnd* pParent /*=NULL*/)
        : CSAPrefsSubDlg(CDlgSAPrefs11::IDD, pParent)
{

}

CDlgSAPrefs11::~CDlgSAPrefs11()
{
}

void CDlgSAPrefs11::DoDataExchange(CDataExchange* pDX)
{
    CSAPrefsSubDlg::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ENABLE_LOG, m_EnableLog);
    DDX_Control(pDX, IDC_MAXIMUM_LOG, m_MaximumLog);
    DDX_Control(pDX, IDC_MAXIMUM_LOG_SPIN, m_MaximumLog_Spin);
    DDX_Control(pDX, IDC_ENABLE_TRACE, m_EnableTrace);
	DDX_Control(pDX, IDC_TRACE_LIST, m_TraceList);
}


BOOL CDlgSAPrefs11::OnInitDialog()
{
    CSAPrefsSubDlg::OnInitDialog();
    CString		text;

    m_EnableLog.SetCheck(global.preferences.LogSymbol_enabled ? BST_CHECKED : BST_UNCHECKED);

    text.Format("%d", global.preferences.LogSymbol_max_log);
    m_MaximumLog.SetWindowText(text);
    m_MaximumLog_Spin.SetRange(0, MAX_MAX_LOG);
    m_MaximumLog_Spin.SetPos(global.preferences.LogSymbol_max_log);
    m_MaximumLog_Spin.SetBuddy(&m_MaximumLog);

    m_EnableTrace.SetCheck(global.preferences.Trace_enabled ? BST_CHECKED : BST_UNCHECKED);
    m_TraceList.AddString("f$alli");
    m_TraceList.AddString("f$swag");
    m_TraceList.AddString("f$rais");
    m_TraceList.AddString("f$call");
    m_TraceList.AddString("f$play");
	for (int i=0;i<nTraceFunctions;i++)
	{
		m_TraceList.SetCheck(i, global.preferences.Trace_functions[i]);
	}

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSAPrefs11::OnOK()
{
    Registry		reg;
    CString			text;

    global.preferences.LogSymbol_enabled = m_EnableLog.GetCheck()==BST_CHECKED ? true : false;
    global.preferences.Trace_enabled = m_EnableTrace.GetCheck()==BST_CHECKED ? true : false;
	for (int i=0;i<nTraceFunctions;i++)
	{
		global.preferences.Trace_functions[i] = m_TraceList.GetCheck(i);
	}

    m_MaximumLog.GetWindowText(text);
    if (strtoul(text.GetString(), 0, 10)<0 || strtoul(text.GetString(), 0, 10)>MAX_MAX_LOG) {
        MessageBox("Invalid maximum log amount!", "ERROR", MB_OK);
        return;
    }
    global.preferences.LogSymbol_max_log = strtoul(text.GetString(), 0, 10);

    reg.read_reg();
    reg.LogSymbol_enabled = global.preferences.LogSymbol_enabled;
    reg.LogSymbol_max_log = global.preferences.LogSymbol_max_log;
    reg.Trace_enabled = global.preferences.Trace_enabled;
    memcpy(reg.Trace_functions, global.preferences.Trace_functions, sizeof(bool)*nTraceFunctions);
    reg.write_reg();

    CSAPrefsSubDlg::OnOK();
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs11, CSAPrefsSubDlg)
    ON_BN_CLICKED(IDC_ENABLE_LOG, &CDlgSAPrefs11::OnBnClickedEnableLog)
END_MESSAGE_MAP()


// DialogSAPrefs11 message handlers

void CDlgSAPrefs11::OnBnClickedEnableLog()
{
    // TODO: Add your control notification handler code here
}
