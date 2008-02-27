// DialogSAPrefs8.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "global.h"
#include "Registry.h"

#include "DialogSAPrefs8.h"

// CDlgSAPrefs8 dialog

IMPLEMENT_DYNAMIC(CDlgSAPrefs8, CDialog)

CDlgSAPrefs8::CDlgSAPrefs8(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgSAPrefs8::IDD, pParent)
{

}

CDlgSAPrefs8::~CDlgSAPrefs8()
{
}

void CDlgSAPrefs8::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RECORD_FRAMES, m_RecordFrames);
	DDX_Control(pDX, IDC_RECORD_EVERY_CHANGE, m_RecordEveryChange);
	DDX_Control(pDX, IDC_MAXFRAMES, m_MaxFrames);
	DDX_Control(pDX, IDC_MAXFRAMES_SPIN, m_MaxFramesSpin);
}


BEGIN_MESSAGE_MAP(CDlgSAPrefs8, CDialog)
	ON_BN_CLICKED(IDC_RECORD_FRAMES, &CDlgSAPrefs8::OnBnClickedRecordFrames)
END_MESSAGE_MAP()


// CDlgSAPrefs8 message handlers

BOOL CDlgSAPrefs8::OnInitDialog()
{
	CString text;

	CSAPrefsSubDlg::OnInitDialog();

	m_RecordFrames.SetCheck(global.preferences.replay_record ? BST_CHECKED : BST_UNCHECKED);
	m_RecordEveryChange.SetCheck(global.preferences.replay_record_every_change ? BST_CHECKED : BST_UNCHECKED);
	m_RecordEveryChange.EnableWindow(global.preferences.replay_record);

	text.Format("%d", global.preferences.replay_max_frames);
	m_MaxFrames.SetWindowText(text.GetString());
	m_MaxFramesSpin.SetRange(1, 1000);
	m_MaxFramesSpin.SetPos(global.preferences.replay_max_frames);
	m_MaxFramesSpin.SetBuddy(&m_MaxFrames);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs8::OnOK()
{
	Registry		reg;
	CString			text;

	global.preferences.replay_record = m_RecordFrames.GetCheck() == BST_CHECKED;
	global.preferences.replay_record_every_change = m_RecordEveryChange.GetCheck() == BST_CHECKED;

	m_MaxFrames.GetWindowText(text);
	global.preferences.replay_max_frames = atoi(text.GetString());

	reg.read_reg();
	reg.replay_record = global.preferences.replay_record;
	reg.replay_record_every_change = global.preferences.replay_record_every_change;
	reg.replay_max_frames = global.preferences.replay_max_frames;
	reg.write_reg();

	CSAPrefsSubDlg::OnOK();
}

void CDlgSAPrefs8::OnBnClickedRecordFrames()
{
	m_RecordEveryChange.EnableWindow(m_RecordFrames.GetCheck() == BST_CHECKED);
}
