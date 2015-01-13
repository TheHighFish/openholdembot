//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************


#include "stdafx.h"
#include "OH_MessageBox.h"

#include <assert.h>
#include "CFormulaParser.h"
#include "CParseErrors.h"
#include "CPreferences.h"


const int k_messagebox_standard_flags = MB_OK | MB_TOPMOST;
const int k_messagebox_error_flags = k_messagebox_standard_flags | MB_ICONWARNING;


void OH_MessageBox_Error_Warning(CString Message, CString Title /* = "Error" */) {
#ifdef OPENHOLDEM_PROGRAM
	// Only OpenHoldem supports this setting,
	// but not OpenScrape or other potential applications
	if (preferences.disable_msgbox())	{
		return;
	}
#endif
	MessageBox(0, Message, Title, k_messagebox_error_flags);
}

int OH_MessageBox_Interactive(CString Message, CString Title, int Flags) {
	return MessageBox(0, Message, Title, (Flags | k_messagebox_standard_flags));
}

// MessageBox for the msgbox$MESSAGE-command of OH-script
// Returns 0 as a result
int OH_MessageBox_OH_Script_Messages(CString message) {
	// Preprocess message
  const char* msgbox_prefix = "msgbox$";
	assert(message.Left(strlen(msgbox_prefix)) == msgbox_prefix);
	int length_of_parameter = message.GetLength() - strlen(msgbox_prefix);
	message = message.Right(length_of_parameter);
	message.Replace("_B", " ");
	message.Replace("_C", ",");
	message.Replace("_D", ".");
	message.Replace("_N", "\n");
	// At the very last: underscores (to avoid incorrect future replacements)
	message.Replace("_U", "_");
	OH_MessageBox_Error_Warning(message, "OH-Script Message");
	return 0;
}

#ifdef OPENHOLDEM_PROGRAM
// For OpenHoldem only, not for OpenScrape
void OH_MessageBox_Formula_Error(CString Message, CString Title) {
  // Make sure, that we have a line-break at the end.
  if (Message.Right(1) != "\n") {
    Message += "\n";
  }
  if (p_formula_parser->IsParsing()) {
    CParseErrors::Error(Message);
  } else {
    OH_MessageBox_Error_Warning(Message, Title);
  }
}
#endif OPENHOLDEM_PROGRAM