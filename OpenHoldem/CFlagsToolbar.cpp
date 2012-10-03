#include "stdafx.h"
#include "CFlagsToolbar.h"

#include <afxwin.h>
#include "CSymbols.h"

CFlagsToolbar *p_flags_toolbar = NULL;

#define ENT CSLock lock(m_critsec);

BEGIN_MESSAGE_MAP(CFlagsToolbar, CWnd)
	ON_BN_CLICKED(ID_NUMBER0,  &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER1,  &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER2,  &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER3,  &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER4,  &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER5,  &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER6,  &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER7,  &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER8,  &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER9,  &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER10, &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER11, &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER12, &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER13, &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER14, &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER15, &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER16, &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER17, &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER18, &OnClickedFlags)
	ON_BN_CLICKED(ID_NUMBER19, &OnClickedFlags)
END_MESSAGE_MAP()

CFlagsToolbar::CFlagsToolbar()
{
	InitCFlagsToolbar();
	for (int i=0; i<k_number_of_flags; i++)
	{
		SetFlag(i, false);
	}
}

CFlagsToolbar::~CFlagsToolbar()
{}

void CFlagsToolbar::InitCFlagsToolbar()
{
	TBBUTTONINFO	tbi;

	tbi.cbSize = sizeof(TBBUTTONINFO);
	tbi.dwMask = TBIF_STYLE;
	tbi.fsStyle = TBSTYLE_CHECK;

	// Flags toolbar
	_tool_bar.CreateEx(this, NULL, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
							CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	_tool_bar.LoadToolBar(IDR_FLAGS);

	// Make flags toolbar buttons sticky
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER0,  &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER1,  &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER2,  &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER3,  &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER4,  &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER5,  &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER6,  &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER7,  &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER8,  &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER9,  &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER10, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER11, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER12, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER13, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER14, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER15, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER16, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER17, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER18, &tbi);
	_tool_bar.GetToolBarCtrl().SetButtonInfo(ID_NUMBER19, &tbi);
	_tool_bar.EnableDocking(CBRS_ALIGN_ANY);
	_tool_bar.EnableDocking(CBRS_ALIGN_TOP);

	// Title of floating flags toolbar
	_tool_bar.SetWindowText("Flags");
}

bool CFlagsToolbar::GetFlag(int flag_number)
{
	ENT
	if (flag_number>=0 && flag_number<k_number_of_flags) 
	{
		return _flags[flag_number]; 
	}
	else 
	{
		return false; 
	}
}

int CFlagsToolbar::GetFlagMax()
{
	int flag_max = -1;
	for (int i=0; i<k_number_of_flags; i++)
	{
		if (_flags[i])
		{
			flag_max = i;
		}
	}
	return flag_max;
}

long int CFlagsToolbar::GetFlagBits()
{
	long int flag_bits = 0;
	for (int i=0; i<k_number_of_flags; i++)
	{
		if (_flags[i])
		{
			flag_bits |= (1 << i);
		}
	}
	return flag_bits;
}


void CFlagsToolbar::SetFlag(int flag_number, bool new_value)
{
	ENT
	// No assertion and no error-message here, only a range-check,
	// as this function might get called by external programs
	// via window-messages.
	if (flag_number>=0 && flag_number<k_number_of_flags) 
	{
		_flags[flag_number] = new_value; 
		switch (flag_number)
		{
			case 0: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER0, new_value); break;
			case 1: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER1, new_value); break;
			case 2: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER2, new_value); break;
			case 3: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER3, new_value); break;
			case 4: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER4, new_value); break;
			case 5: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER5, new_value); break;
			case 6: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER6, new_value); break;
			case 7: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER7, new_value); break;
			case 8: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER8, new_value); break;
			case 9: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER9, new_value); break;
			case 10: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER10, new_value); break;
			case 11: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER11, new_value); break;
			case 12: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER12, new_value); break;
			case 13: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER13, new_value); break;
			case 14: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER14, new_value); break;
			case 15: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER15, new_value); break;
			case 16: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER16, new_value); break;
			case 17: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER17, new_value); break;
			case 18: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER18, new_value); break;
			case 19: _tool_bar.GetToolBarCtrl().CheckButton(ID_NUMBER19, new_value); break;
		}
	}
}

void CFlagsToolbar::OnClickedFlags() 
{
	SetFlag(0,  _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER0));
	SetFlag(1,  _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER1));
	SetFlag(2,  _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER2));
	SetFlag(3,  _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER3));
	SetFlag(4,  _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER4));
	SetFlag(5,  _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER5));
	SetFlag(6,  _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER6));
	SetFlag(7,  _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER7));
	SetFlag(8,  _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER8));
	SetFlag(9,  _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER9));
	SetFlag(10, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER10));
	SetFlag(11, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER11));
	SetFlag(12, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER12));
	SetFlag(13, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER13));
	SetFlag(14, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER14));
	SetFlag(15, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER15));
	SetFlag(16, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER16));
	SetFlag(17, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER17));
	SetFlag(18, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER18));
	SetFlag(19, _tool_bar.GetToolBarCtrl().IsButtonChecked(ID_NUMBER19));

	p_symbols->CalcSymbols();
}

