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

#pragma once

#include "resource.h"

#include "SAPrefsDialog.h"

// CDlgSAPrefs16 dialog

class CDlgSAPrefs16 : public CSAPrefsSubDlg
{
	DECLARE_DYNAMIC(CDlgSAPrefs16)

public:
	CDlgSAPrefs16(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSAPrefs16();

// Dialog Data
	enum { IDD = IDD_SAPREFS16 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();
public:
	CButton			_rebuy_condition_no_cards_Button;
	CButton			_rebuy_condition_change_in_handnumber_Button;
	CButton			_rebuy_condition_heuristic_check_for_occlusion_Button;
	CSpinButtonCtrl	_rebuy_minimum_time_to_next_try_Spin;
	CEdit			_rebuy_minimum_time_to_next_try_Edit;
	CEdit			_rebuy_script_Edit;
};
