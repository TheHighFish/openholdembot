#ifndef INC_OH_MESSAGEBOX_H
#define INC_OH_MESSAGEBOX_H

#include "stdafx.h"

// !!!To be removed
void OH_MessageBox_Error_Warning(CString message, CString title);

// Errors and warnings.
// Get displayed (or not) depending on settings
void OH_MessageBox(CString message, CString title, int flags);

// Interactive messages.
// Get always displayed, even if they are not critical.
// They are either a result of the users action 
// or we need to request some direct input.
int OH_MessageBox_Interactive(CString message, CString title, int flags);

#endif