#ifndef INC_DIALOGLOCKBLINDS_H
#define INC_DIALOGLOCKBLINDS_H

// CDlgLockBlinds dialog

class CDlgLockBlinds : public CDialog {

public:
	CDlgLockBlinds(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_LOCK_BLINDS };

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();


	CEdit m_SmallBlind;
	CEdit m_BigBlind;
	CEdit m_BigBet;
	CEdit m_Ante;
	CComboBox m_GameType;

	double	sblind, bblind, bbet, ante;
	int		gametype;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


#endif //INC_DIALOGLOCKBLINDS_H