#ifndef INC_DIALOGSAPREFS11_H
#define INC_DIALOGSAPREFS11_H

#include "resource.h"
#include "afxwin.h"

// DialogSAPrefs11 dialog

class CDlgSAPrefs11 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs11)

public:
	CDlgSAPrefs11(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs11();
	afx_msg void OnBnClickedDisableMsgbox();

// Dialog Data
	enum { IDD = IDD_SAPREFS11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	CButton m_EnableLog, m_EnableTrace, m_disable_msgbox;
	CEdit m_MaximumLog;
	CCheckListBox m_TraceList;
	CSpinButtonCtrl m_MaximumLog_Spin;
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};


#endif //INC_DIALOGSAPREFS11_H