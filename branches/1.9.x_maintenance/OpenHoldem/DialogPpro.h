#ifndef INC_DIALOGPPRO_H
#define INC_DIALOGPPRO_H

#include "resource.h"

#define		STATE_TIMER		1
#define		AUTOSEAT_TIMER	2

// CDlgPpro dialog

class CDlgPpro : public CDialog {
	DECLARE_DYNAMIC(CDlgPpro)

public:
	CDlgPpro(CWnd* pParent = NULL);   // standard constructor
	enum { IDD = IDD_POKERPRO };
	virtual ~CDlgPpro();

	virtual BOOL DestroyWindow();
	CString		messages;
	bool		messages_changed;

protected:
		DECLARE_MESSAGE_MAP()

public:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void OnTimer(UINT nIDEvent);

	bool	need_to_do_autoseat;
	bool	need_to_do_autochips;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedConnectButton();
	afx_msg void OnBnClickedJointableButton();
	afx_msg void OnBnClickedSitdownButton();
	afx_msg void OnBnClickedBuychipsButton();
	afx_msg void OnBnClickedDepositButton();
	afx_msg void OnLbnDblclkTableList();
	afx_msg void OnBnClickedSitinButton();
	void DoTableSelect(void);
	void SaveSettingsToRegistry(void);
	virtual BOOL OnInitDialog();

	CEdit m_HostName;
	CEdit m_Port;
	CEdit m_UserName;
	CEdit m_Password;
	CEdit m_Table;
	CEdit m_Chips;
	CButton m_AutoSeat;
	CStatic m_ConnectStatus;
	CStatic m_LoginStatus;
	CButton m_ConnectButton;
	CStatic m_StatusTable;
	CStatic m_StatusChair;
	CListBox m_TableList;
	CButton m_JoinTable_Button;
	CButton m_SitDownButton;
	CButton m_BuyChipsButton;
	CButton m_DepositButton;
	CEdit m_ChipsToBuy;
	CEdit m_DepositAmount;
	CButton m_SitInButton;
	CRichEditCtrl m_PproMessages;
	CButton m_HandHistory;
};

extern CDlgPpro			*m_pproDlg;


#endif //INC_DIALOGPPRO_H