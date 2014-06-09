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

#ifndef INC_DIALOGSAPREFS13_H
#define INC_DIALOGSAPREFS13_H

#include "resource.h"
#include "afxwin.h"

#include "SAPrefsDialog.h"

// CDlgSAPrefs13 dialog

class CDlgSAPrefs13 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs13)

public:
	CDlgSAPrefs13(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs13();

// Dialog Data
	enum { IDD = IDD_SAPREFS13 };
	CEdit	_class_name_edit;
	CEdit	_class_mutex_edit;
	CButton _simple_window_title;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();

protected:
};

#endif