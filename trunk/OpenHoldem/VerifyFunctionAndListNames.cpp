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

#include "stdafx.h"
#include "VerifyFunctionAndListNames.h"

#include "COHScriptObject.h"
#include "OH_MessageBox.h"

bool VerifyFunctionName(CString name) {
  if (!COHScriptObject::IsFunction(name)) {
	OH_MessageBox_Interactive(
      "UDF's must begin with the name 'f$'\r\n"
      "e.g. 'f$myfunc'", 
      "Invalid Name", MB_ICONERROR);
	  return false;
  } else if (strlen(name)==2) {
   OH_MessageBox_Interactive(
     "UDF's must begin with the name 'f$'\r\n"
     "and be followed be the name of the function.\r\n"
     "e.g. 'f$myfunc'", 
     "Invalid Name", MB_ICONERROR);
	return false;
  }
  return true;
}

bool VerifyListName(CString name) {
  if (memcmp(name, "list", 4)!=0) {
	OH_MessageBox_Interactive(
      "Lists must begin with the name 'list'.\r\n"
      "e.g. 'list_limp_UTG'", 
      "Invalid Name", MB_ICONERROR);
	return false;
  } else if (strlen(name)==4) {
	OH_MessageBox_Interactive(
      "Lists must begin with the name 'list'\r\n"
      "and be followed by a number or identifier.\r\n"
      "e.g. 'list45'", 
      "Invalid Name", MB_ICONERROR);
	return false;
  }
  return true;
}