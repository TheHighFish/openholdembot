#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "global.h"

// CDlgSAPrefs1 dialog

class CDlgSAPrefs1 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs1)

public:
	CDlgSAPrefs1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs1();

// Dialog Data
	enum { IDD = IDD_SAPREFS1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_MaxOpponents;
	CSpinButtonCtrl m_MaxOpponents_Spin;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnEnChangeMaxopponents();
};
