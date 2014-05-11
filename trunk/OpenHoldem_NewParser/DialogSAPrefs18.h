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

// !! Now unused; might be "recycled"

#ifndef INC_DIALOGSAPREFS18_H
#define INC_DIALOGSAPREFS1_H

#include "resource.h"

#include "SAPrefsDialog.h"

// CDlgSAPrefs18 dialog

class CDlgSAPrefs18 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs18)

public:
	CDlgSAPrefs18(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs18();

// Dialog Data
	enum { IDD = IDD_SAPREFS18 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CButton _scrape_always_nutton;
	CButton _scrape_when_cards_button;
	CButton _scrape_when_my_turn_button;

protected:
	virtual void OnOK();
};


#endif //INC_DIALOGSAPREFS18_H