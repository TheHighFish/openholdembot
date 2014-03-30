#ifndef INC_DIALOGSAPREFS4_H
#define INC_DIALOGSAPREFS4_H

#include "resource.h"

// CDlgSAPrefs4 dialog

class CDlgSAPrefs4 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs4)

public:
	CDlgSAPrefs4(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs4();

// Dialog Data
	enum { IDD = IDD_SAPREFS4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ScrapeDelay;
	CSpinButtonCtrl m_ScrapeDelay_Spin;
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();

public:
	afx_msg void OnDeltaposScrapedelaySpin(NMHDR *pNMHDR, LRESULT *pResult);
};


#endif //INC_DIALOGSAPREFS4_H