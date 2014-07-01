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

#ifndef INC_DIALOGSAPREFS11_H
#define INC_DIALOGSAPREFS11_H

#include "resource.h"
#include "afxwin.h"

#include "SAPrefsDialog.h"

// DialogSAPrefs11 dialog

class CDlgSAPrefs11 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs11)

public:
	CDlgSAPrefs11(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs11();

// Dialog Data
	enum { IDD = IDD_SAPREFS11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	CButton m_EnableLog, m_EnableTrace;
	CButton m_EnableBasicInfo, m_EnableErrorLogging, m_EnableDLLLogging;
	CEdit m_MaximumLog, m_MaximumLogSize;
	CSpinButtonCtrl m_MaximumLog_Spin, m_MaximumLogSize_Spin;
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();
public:
	afx_msg void OnCbnSelchangeDebuglevelPt();
};


#endif //INC_DIALOGSAPREFS11_H