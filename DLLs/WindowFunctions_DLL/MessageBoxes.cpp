//*******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

// Needs to be defined here, before #include "window_functions.h"
// to generate proper export- and inport-definitions
#define WINDOW_FUNCTIONS_EXPORTS

#include "window_functions.h"
#include <assert.h>

const int k_messagebox_standard_flags = MB_OK | MB_TOPMOST;
const int k_messagebox_error_flags = k_messagebox_standard_flags | MB_ICONWARNING;

void MessageBox_Error_Warning(CString Message, CString Title /* = "Error" */) {
  // Only OpenHoldem supports this setting,
  // but not OpenScrape or other potential applications
  /*!!!!!if (preferences.disable_msgbox()) {
    return;
  }*/
  MessageBox(0, Message, Title, k_messagebox_error_flags);
}

int MessageBox_Interactive(CString Message, CString Title, int Flags) {
  return MessageBox(0, Message, Title, (Flags | k_messagebox_standard_flags));
}

// MessageBox for the msgbox$MESSAGE-command of OH-script
// Returns 0 as a result
void MessageBox_OH_Script_Messages(CString message) {
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
  MessageBox_Error_Warning(message, "OH-Script Message");
}