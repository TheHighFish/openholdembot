#include "stdafx.h"
#include "OH_MessageBox.h"
#include "CPreferences.h"


const int k_messagebox_standard_flags = MB_OK | MB_TOPMOST;
const int k_messagebox_error_flags = k_messagebox_standard_flags | MB_ICONWARNING;


void OH_MessageBox_Error_Warning(CString Message, CString Title)
{
#ifdef OPENHOLDEM_PROGRAM
	// Only OpenHoldem supports this setting,
	// but not OpenScrape or other potential applications
	if (prefs.disable_msgbox())
	{
		return;
	}
#endif
	MessageBox(0, Message, Title, k_messagebox_error_flags);
}

int OH_MessageBox_Interactive(CString Message, CString Title, int Flags)
{
	return MessageBox(0, Message, Title, (Flags | k_messagebox_standard_flags));
}