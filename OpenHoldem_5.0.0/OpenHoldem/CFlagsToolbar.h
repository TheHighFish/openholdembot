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

#ifndef INC_CFLAGSTOOLBAR_H
#define INC_CFLAGSTOOLBAR_H

#include "MyCtoolbar.h"

class CFlagsToolbar: public CWnd
{
public:
	CFlagsToolbar(CFrameWnd *parent_window);
	~CFlagsToolbar();
public:
	bool GetFlag(const int i); 
	int  GetFlagMax();
	long int  GetFlagBits();
public:
	void SetFlag(int flag_number, bool new_value);
	void DisableButtonsOnConnect();
	void EnableButtonsOnDisconnect();
	void UnattachOHFromPokerWindow();
public:
	void EnableButton(int button_ID, bool new_status);
	void CheckButton(int button_ID, bool new_status);
	bool IsButtonChecked(int button_ID);
	bool IsButtonEnabled(int button_ID);
public:
	CMyToolBar _tool_bar;
public:
	afx_msg void OnClickedFlags();
	DECLARE_MESSAGE_MAP()
private:
	void CreateMainToolbar();
	void CreateFlagsToolbar();
	void AlignToolbars();
private:
	CMyToolBar	m_MainToolBar;
	bool		_flags[k_number_of_flags];
	CFrameWnd	*_parent_window;
	CCritSec	m_critsec;
};

extern CFlagsToolbar *p_flags_toolbar;

#endif INC_CFLAGSTOOLBAR_H