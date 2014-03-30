// DialogSAPrefs10.cpp : implementation file
//

#include "stdafx.h"
#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs10.h"
#include "global.h"
#include "Registry.h"


#define MAX_DELAY 1000000


// DialogSAPrefs10 dialog


IMPLEMENT_DYNAMIC(CDlgSAPrefs10, CDialog)

CDlgSAPrefs10::CDlgSAPrefs10(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgSAPrefs10::IDD, pParent)
{

}

CDlgSAPrefs10::~CDlgSAPrefs10()
{
}

void CDlgSAPrefs10::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENABLE_CHAT, m_EnableChat);
	DDX_Control(pDX, IDC_MINIMUM_DELAY, m_MinimumDelay);
	DDX_Control(pDX, IDC_MINIMUM_DELAY_SPIN, m_MinimumDelay_Spin);
	DDX_Control(pDX, IDC_RANDOM_DELAY, m_RandomDelay);
	DDX_Control(pDX, IDC_RANDOM_DELAY_SPIN, m_RandomDelay_Spin);
}


BOOL CDlgSAPrefs10::OnInitDialog()
{
	CSAPrefsSubDlg::OnInitDialog();
	CString		text;

	m_EnableChat.SetCheck(global.preferences.Chat_enabled ? BST_CHECKED : BST_UNCHECKED);

	text.Format("%d", global.preferences.Chat_min_Delay);
	m_MinimumDelay.SetWindowText(text);
	m_MinimumDelay_Spin.SetRange(0, MAX_DELAY);
	m_MinimumDelay_Spin.SetPos(global.preferences.Chat_min_Delay);
	m_MinimumDelay_Spin.SetBuddy(&m_MinimumDelay);

	text.Format("%d", global.preferences.Chat_random_Delay);
	m_RandomDelay.SetWindowText(text);
	m_RandomDelay_Spin.SetRange(0, MAX_DELAY);
	m_RandomDelay_Spin.SetPos(global.preferences.Chat_random_Delay);
	m_RandomDelay_Spin.SetBuddy(&m_RandomDelay);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSAPrefs10::OnOK()
{
	Registry		reg;
	CString			text;

	global.preferences.Chat_enabled = m_EnableChat.GetCheck()==BST_CHECKED ? true : false;

	m_MinimumDelay.GetWindowText(text);
	if (strtoul(text.GetString(), 0, 10)<0 || strtoul(text.GetString(), 0, 10)>MAX_DELAY) {
		MessageBox("Invalid minimum Chat Delay", "ERROR", MB_OK);
		return;
	}
	global.preferences.Chat_min_Delay = strtoul(text.GetString(), 0, 10);

	m_RandomDelay.GetWindowText(text);
	if (strtoul(text.GetString(), 0, 10)<0 || strtoul(text.GetString(), 0, 10)>MAX_DELAY) {
		MessageBox("Invalid random Chat Delay", "ERROR", MB_OK);
		return;
	}
	global.preferences.Chat_random_Delay = strtoul(text.GetString(), 0, 10);

	reg.read_reg();
	reg.Chat_enabled = global.preferences.Chat_enabled;
	reg.Chat_min_Delay = global.preferences.Chat_min_Delay;
	reg.Chat_random_Delay = global.preferences.Chat_random_Delay;
	reg.write_reg();

	CSAPrefsSubDlg::OnOK();
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs10, CSAPrefsSubDlg)
	ON_BN_CLICKED(IDC_ENABLE_CHAT, &CDlgSAPrefs10::OnBnClickedEnableChat)
END_MESSAGE_MAP()


// DialogSAPrefs10 message handlers

void CDlgSAPrefs10::OnBnClickedEnableChat()
{
	// TODO: Add your control notification handler code here
}
