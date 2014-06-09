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

class COpenHoldemTitle
{
public:
	COpenHoldemTitle();
	~COpenHoldemTitle();
public:
	CString GetTitle();
	void SetUserDefinedOpenHoldemTitle(CString new_title);
	void UpdateTitle();
private:
	CString FullTitle();			// normal title with table-name, casino and limits
private:
	CString user_defined_title;		// defined via hopper-messages
	CString simple_title;			// executale-name
};

extern COpenHoldemTitle *p_openholdem_title;