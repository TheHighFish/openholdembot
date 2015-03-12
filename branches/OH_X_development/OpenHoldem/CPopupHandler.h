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

#ifndef INC_CPOPUPHANDLER_H
#define INC_CPOPUPHANDLER_H

class CPopupHandler
{
public:
	CPopupHandler();
	~CPopupHandler();
public:
	// To be called by the auto-connector-thread
	// after failed tries to connect
	void HandleAllWindows();
	// To be called by callback-function EnumProcPotentialPopups
	void HandlePotentialPopup(HWND potential_popup, bool hard_kill);
private:
	// To be called by instance 0
	void MinimizeAllOnstartup();
private:
	void HandleAllWindows(bool hard_kill);
};

#endif