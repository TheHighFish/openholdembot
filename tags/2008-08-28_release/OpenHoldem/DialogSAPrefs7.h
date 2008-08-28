#ifndef INC_DIALOGSAPREFS7_H
#define INC_DIALOGSAPREFS7_H

#include "resource.h"

// CDlgSAPrefs7 dialog

class CDlgSAPrefs7 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs7)

public:
	CDlgSAPrefs7(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs7();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	enum { IDD = IDD_SAPREFS7 };
	CEdit m_ICM1, m_ICM2, m_ICM3, m_ICM4;

	DECLARE_MESSAGE_MAP()
};


#endif //INC_DIALOGSAPREFS7_H