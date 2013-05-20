#ifndef INC_OH_MESSAGEBOX_H
#define INC_OH_MESSAGEBOX_H

#include "stdafx.h"

// Errors and warnings.
// Get displayed (or not) depending on settings
void OH_MessageBox_Error_Warning(CString Message, CString Title);

// Interactive messages.
// Get always displayed, even if they are not critical.
// They are either a result of the users action 
// or we need request some direct input.
int OH_MessageBox_Interactive(CString Message, CString Title, int Flags);

#endif