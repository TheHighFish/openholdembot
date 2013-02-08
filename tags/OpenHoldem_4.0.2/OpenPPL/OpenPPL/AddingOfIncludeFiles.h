#ifndef INC_ADDINGOFINCLUDEFILES_H
#define INC_ADDINGOFINCLUDEFILES_H

#include <afx.h>

// Mandatory up to 2.1.9.4, then depricated
const CString k_swag_file_name       = "OpenPPL_SwagFunctions_For_OpenHoldem_2.1.x_Only_Please_Delete_This_File_Once_You_Use_OpenHoldem_2.2.0_And_Later.ohf";

// Optionally since OH 4.0.0
const CString k_custom_OHF_file_name = "My_Custom_OH_Script_Code.ohf";

// Mandatory since OH 2.2.0
const CString k_hand_strength_library_file_name = "mh_str_Handstrength_Library.ohf";


void AppendFileToOHFOutputIfFileExists(CString file_name, bool error_on_failure);


#endif INC_ADDINGOFINCLUDEFILES_H