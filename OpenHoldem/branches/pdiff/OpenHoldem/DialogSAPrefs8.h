#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"


// CDlgSAPrefs8 dialog

class CDlgSAPrefs8 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs8)

public:
	CDlgSAPrefs8(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs8();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	enum { IDD = IDD_SAPREFS8 };

	DECLARE_MESSAGE_MAP()
public:
	CButton m_RecordFrames;
	CButton m_RecordEveryChange;
	CEdit m_MaxFrames;
	CSpinButtonCtrl m_MaxFramesSpin;
	afx_msg void OnBnClickedRecordFrames();
};
