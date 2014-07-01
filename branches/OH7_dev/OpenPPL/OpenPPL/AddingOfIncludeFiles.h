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


#ifndef INC_ADDINGOFINCLUDEFILES_H
#define INC_ADDINGOFINCLUDEFILES_H

#include <afx.h>

// Mandatory up to 2.1.9.4, then depricated
const CString k_swag_file_name       = "OpenPPL_SwagFunctions_For_OpenHoldem_2.1.x_Only_Please_Delete_This_File_Once_You_Use_OpenHoldem_2.2.0_And_Later.ohf";

// Optionally since OH 4.0.0
const CString k_custom_OHF_file_name = "My_Custom_OH_Script_Code.ohf";

void AppendFileToOHFOutputIfFileExists(CString file_name, bool error_on_failure);


#endif INC_ADDINGOFINCLUDEFILES_H