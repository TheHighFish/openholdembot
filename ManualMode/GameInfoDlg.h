#pragma once
#include "afxwin.h"
#include "resource.h"

// CGameInfoDlg dialog

class CGameInfoDlg : public CDialog {

public:
	CGameInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_GAMEINFO };

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();


	CEdit m_SmallBlind;
	CEdit m_BigBlind;
	CEdit m_BigBet;
	CEdit m_Ante;
	CComboBox m_GameType;
	CButton m_Tournament;

	CString	m_sblind, m_bblind, m_bigbet, m_ante;
	int		m_gametype;
	bool	m_tournament;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
