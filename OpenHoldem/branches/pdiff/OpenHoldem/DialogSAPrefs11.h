#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "global.h"


// DialogSAPrefs11 dialog

class CDlgSAPrefs11 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs11)

public:
	CDlgSAPrefs11(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs11();

// Dialog Data
	enum { IDD = IDD_SAPREFS11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	CButton m_EnableLog;
    CEdit m_MaximumLog;
	CSpinButtonCtrl m_MaximumLog_Spin;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEnableLog();
};
