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

#ifndef INC_DIALOGSAPREFS8_H
#define INC_DIALOGSAPREFS8_H

#include "resource.h"

#include "SAPrefsDialog.h"

// CDlgSAPrefs8 dialog

class CDlgSAPrefs8 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs8)

public:
	CDlgSAPrefs8(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs8();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	enum { IDD = IDD_SAPREFS8 };

	DECLARE_MESSAGE_MAP()

public:
	CButton m_RecordFrames;
	CButton m_RecordMyTurn;
	CButton m_RecordEveryChange;
	CEdit m_MaxFrames;
	CButton m_RecordEveryChangePlaying;
	CSpinButtonCtrl m_MaxFramesSpin;
	afx_msg void OnBnClickedRecordFrames();
};


#endif //INC_DIALOGSAPREFS8_H
