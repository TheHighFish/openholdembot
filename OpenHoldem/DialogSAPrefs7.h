//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#ifndef INC_DIALOGSAPREFS7_H
#define INC_DIALOGSAPREFS7_H

#include "resource.h"
#include "afxwin.h"

#include "SAPrefsDialog.h"

// CDlgSAPrefs7 dialog

class CDlgSAPrefs7 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs7)

public:
	CDlgSAPrefs7(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs7();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	enum { IDD = IDD_SAPREFS7 };
	CEdit m_ICM1, m_ICM2, m_ICM3, m_ICM4, m_ICM5;

	DECLARE_MESSAGE_MAP()
};


#endif //INC_DIALOGSAPREFS7_H