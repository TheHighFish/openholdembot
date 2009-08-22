#ifndef INC_DIALOGSAPREFS14_H
#define INC_DIALOGSAPREFS14_H

#include "resource.h"
#include "afxwin.h"


// CDlgSAPrefs14 dialog

class CDlgSAPrefs14 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs14)

public:
	CDlgSAPrefs14(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs14();

// Dialog Data
	enum { IDD = IDD_SAPREFS14 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};

#endif