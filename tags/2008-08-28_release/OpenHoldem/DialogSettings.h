#ifndef INC_DIALOGSETTINGS_H
#define INC_DIALOGSETTINGS_H

// CDlgSettings dialog

class CDlgSettings : public CDialog
{
	DECLARE_DYNAMIC(CDlgSettings)

public:
	CDlgSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettings();
	double bankroll, nit, defcon, rake;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	enum { IDD = IDD_SETTINGS };
	CEdit m_Bankroll;
	CEdit m_Nit;
	CEdit m_Defcon;
	CEdit m_Rake;


	DECLARE_MESSAGE_MAP()
};


#endif //INC_DIALOGSETTINGS_H