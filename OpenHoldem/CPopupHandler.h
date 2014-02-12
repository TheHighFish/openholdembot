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

#ifndef INC_CPOPUPHANDLER_H
#define INC_CPOPUPHANDLER_H

class CPopupHandler
{
public:
	CPopupHandler();
	~CPopupHandler();
public:
	void MinimizeAllOnstartup();
	void HandleAllWindows();
private:
	void HandleAllWindows(bool hard_kill);
	void HandlePotentialPopup(HWND potential_popup, bool hard_kill);
};

#endif