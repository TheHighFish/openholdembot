//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_DIALOGSAPREFS22_H
#define INC_DIALOGSAPREFS22_H

#include "resource.h"
#include "afxwin.h"

#include "SAPrefsDialog.h"

// CDlgSAPrefs22 dialog

class CDlgSAPrefs22 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs22)

public:
	CDlgSAPrefs22(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs22();

private:
// Dialog Data
	enum { IDD = IDD_SAPREFS22 };
	CButton _popup_disabled;
	CButton _popup_minimize;
	CButton _popup_kill;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};

#endif