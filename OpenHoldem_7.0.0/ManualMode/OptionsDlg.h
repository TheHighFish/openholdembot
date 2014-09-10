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


#pragma once


// OptionsDlg dialog

class OptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionsDlg)

public:
	OptionsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~OptionsDlg();

// Dialog Data
	enum { IDD = IDD_OPTIONSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// move popup menu outside right edge
	BOOL m_unobstructivePopup;
};
