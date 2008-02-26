#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ScrapeDelay;
	CSpinButtonCtrl m_ScrapeDelay_Spin;
	CEdit m_NameScrapes;
	CSpinButtonCtrl m_NameScrapes_Spin;
	CEdit m_BalanceScrapes;
	CSpinButtonCtrl m_BalanceScrapes_Spin;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnDeltaposScrapedelaySpin(NMHDR *pNMHDR, LRESULT *pResult);
};
