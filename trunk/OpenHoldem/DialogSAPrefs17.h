#ifndef INC_DIALOGSAPREFS17_H
#define INC_DIALOGSAPREFS1_H

#include "resource.h"

#include "SAPrefsDialog.h"

// CDlgSAPrefs17 dialog

class CDlgSAPrefs17 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs17)

public:
	CDlgSAPrefs17(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs17();

// Dialog Data
	enum { IDD = IDD_SAPREFS17 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_ConfigurationDisableLessCriticalChecks_Button;
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();

public:
	afx_msg void OnEnChangeMaxopponents();
};


#endif //INC_DIALOGSAPREFS17_H
