#ifndef INC_DIALOGSAPREFS3_H
#define INC_DIALOGSAPREFS3_H

#include "resource.h"

// CDlgSAPrefs3 dialog

class CDlgSAPrefs3 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs3)

public:
	CDlgSAPrefs3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs3();

// Dialog Data
	enum { IDD = IDD_SAPREFS3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_LoadDllOnStartup;
	CEdit m_DllName;
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();

public:
	CButton m_AlwaysSendState;
};


#endif //INC_DIALOGSAPREFS3_H