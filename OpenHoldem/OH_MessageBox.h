//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_OH_MESSAGEBOX_H
#define INC_OH_MESSAGEBOX_H

#include "stdafx.h"

#ifdef OPENHOLDEM_PROGRAM
// In parsing mode: very detailed error-message
// Otherwise; normal error-message
// It might happen that we detect errors only at run-time,
// e.g. if a DLL requests an invalid symbol.
//
// For OpenHoldem only, not for OpenScrape
void OH_MessageBox_Formula_Error(CString Message, CString Title);
#endif

#endif