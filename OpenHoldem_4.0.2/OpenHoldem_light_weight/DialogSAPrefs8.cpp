// DialogSAPrefs8.cpp : implementation file
//

#include "stdafx.h"

#include "SAPrefsSubDlg.h"
#include "DialogSAPrefs8.h"

#include "CPreferences.h"

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
	DDX_Control(pDX, IDC_RECORD_MY_TURN, m_RecordMyTurn);
	DDX_Control(pDX, IDC_RECORD_EVERY_CHANGE, m_RecordEveryChange);
	DDX_Control(pDX, IDC_MAXFRAMES, m_MaxFrames);
	DDX_Control(pDX, IDC_RECORD_EVERY_CHANGE_PLAYING, m_RecordEveryChangePlaying);
	DDX_Control(pDX, IDC_MAXFRAMES_SPIN, m_MaxFramesSpin);
}

BEGIN_MESSAGE_MAP(CDlgSAPrefs8, CDialog)
	ON_BN_CLICKED(IDC_RECORD_FRAMES, &CDlgSAPrefs8::OnBnClickedRecordFrames)
END_MESSAGE_MAP()

// CDlgSAPrefs8 message handlers
BOOL CDlgSAPrefs8::OnInitDialog()
{
	CString text = "";

	CSAPrefsSubDlg::OnInitDialog();

	if (prefs.replay_record()){
		m_RecordMyTurn.SetCheck(BST_CHECKED);
		m_RecordFrames.SetCheck(BST_CHECKED);
	}
	else if (prefs.replay_record_every_change()){
		m_RecordEveryChange.SetCheck(BST_CHECKED);
		m_RecordFrames.SetCheck(BST_CHECKED);
	}
	else if (prefs.replay_record_every_change_playing()){
		m_RecordEveryChangePlaying.SetCheck(BST_CHECKED);
		m_RecordFrames.SetCheck(BST_CHECKED);
	}
	else{
		m_RecordFrames.SetCheck(BST_UNCHECKED);
		m_RecordMyTurn.EnableWindow(0);
		m_RecordEveryChange.EnableWindow(0);
		m_RecordEveryChangePlaying.EnableWindow(0);
	}

	text.Format("%d", prefs.replay_max_frames());
	m_MaxFrames.SetWindowText(text.GetString());
	m_MaxFramesSpin.SetRange(1, 1000);
	m_MaxFramesSpin.SetPos(prefs.replay_max_frames());
	m_MaxFramesSpin.SetBuddy(&m_MaxFrames);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSAPrefs8::OnOK()
{
	CString			text = "";

	if (m_RecordFrames.GetCheck() == BST_CHECKED){
		prefs.set_replay_record(m_RecordMyTurn.GetCheck() == BST_CHECKED);
		prefs.set_replay_record_every_change(m_RecordEveryChange.GetCheck() == BST_CHECKED);
		prefs.set_replay_record_every_change_playing(m_RecordEveryChangePlaying.GetCheck() == BST_CHECKED);
	}
	else{
		prefs.set_replay_record(0);
		prefs.set_replay_record_every_change(0);
		prefs.set_replay_record_every_change_playing(0);
	}

	m_MaxFrames.GetWindowText(text);
	prefs.set_replay_max_frames(atoi(text.GetString()));

	CSAPrefsSubDlg::OnOK();
}

void CDlgSAPrefs8::OnBnClickedRecordFrames()
{
	if (m_RecordFrames.GetCheck() == BST_CHECKED){
		m_RecordMyTurn.EnableWindow(1);
		m_RecordEveryChange.EnableWindow(1);
		m_RecordEveryChangePlaying.EnableWindow(1);
		if(!(m_RecordMyTurn.GetCheck() || m_RecordEveryChange.GetCheck() || m_RecordEveryChangePlaying.GetCheck()))
			m_RecordMyTurn.SetCheck(BST_CHECKED);
	}
	else{
		m_RecordMyTurn.EnableWindow(0);
		m_RecordEveryChange.EnableWindow(0);
		m_RecordEveryChangePlaying.EnableWindow(0);
	}
}

