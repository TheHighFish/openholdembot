#ifndef INC_DIALOGSAPREFS9_H
#define INC_DIALOGSAPREFS9_H

#include "resource.h"

#include "SAPrefsDialog.h"

// DialogSAPrefs9 dialog

class CDlgSAPrefs9 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs9)

public:
	CDlgSAPrefs9(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs9();

// Dialog Data
	enum { IDD = IDD_SAPREFS9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	CButton m_LoadPerlInterpreter;
	CButton m_LoadDefaultPerlFormula;
	CEdit m_DefaultPerlFormula;
	CEdit m_PerlEditor;
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};


#endif //INC_DIALOGSAPREFS9_H