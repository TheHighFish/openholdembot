#include "stdafx.h"
#include "OH_MessageBox.h"
#include "CPreferences.h"


void OH_MessageBox(CString Message, CString Title, int Flags)
{
	MessageBox(0, Message, Title, Flags);
}

int OH_MessageBox_Interactive(CString Message, CString Title, int Flags)
{
	return MessageBox(0, Message, Title, Flags);
}