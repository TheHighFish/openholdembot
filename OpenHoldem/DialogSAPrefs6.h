#ifndef INC_DIALOGSAPREFS6_H
#define INC_DIALOGSAPREFS6_H

#include "resource.h"

// CDlgSAPrefs6 dialog

class CDlgSAPrefs6 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs6)

public:
	CDlgSAPrefs6(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs6();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedPtTest();

	enum { IDD = IDD_SAPREFS6 };
	CEdit m_pt_ip, m_pt_port, m_pt_user, m_pt_pass, m_pt_dbname, m_UpdateDelay, m_CacheRefresh;
	CSpinButtonCtrl m_UpdateDelay_Spin, m_CacheRefresh_Spin;
	CButton m_pt_disable;

	DECLARE_MESSAGE_MAP()
};


#endif //INC_DIALOGSAPREFS6_H