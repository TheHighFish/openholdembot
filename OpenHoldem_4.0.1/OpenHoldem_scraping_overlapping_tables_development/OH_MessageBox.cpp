#include "stdafx.h"
#include "OH_MessageBox.h"
#include "CPreferences.h"

// !!! To be removed
void OH_MessageBox_Error_Warning(CString message, CString title)
{
	OH_MessageBox(message, title, 0);
}

void OH_MessageBox(CString message, CString title, int flags)
{
	MessageBox(0, message, title, flags);
}

int OH_MessageBox_Interactive(CString message, CString title, int flags)
{
	return MessageBox(0, message, title, flags);
}