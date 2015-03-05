//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************



#include "stdafx.h"

#include "registry.h"
#include "stdio.h"
#include "stdlib.h"
#include "debug.h"

#define MM_SUBKEY  "Software\\OpenHoldem\\ManualMode"

Registry::Registry(void) 
{
}

void Registry::read_reg(void) 
{
	HKEY		hKey;
	DWORD		dwType, cbData;
	LONG		hkResult;
	char		str[256];

	// Defaults
	manual_x = manual_y = 0;
	macro = "RNbBPpPAdKsP";

	// Manual Mode Settings
	hkResult = RegOpenKeyEx(HKEY_CURRENT_USER, MM_SUBKEY, 0, KEY_READ, &hKey);
	if (hkResult==ERROR_SUCCESS) {

		// Window location
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "manual_x", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			manual_x = atoi(str);
		}

		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "manual_y", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			manual_y = atoi(str);
		}

		// Macro
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "macro", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			macro = str;
		}
	}

	RegCloseKey(hKey);
}

void Registry::write_reg(void) 
{
	HKEY		hKey;
	DWORD		dwDisp;
	char		str[256];

	// Settings
	RegCreateKeyEx(HKEY_CURRENT_USER, MM_SUBKEY, 0, NULL, 
	REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisp);

	// Window location and size
	sprintf_s(str, 256, "%d", manual_x);
	RegSetValueEx(hKey, "manual_x", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

	sprintf_s(str, 256, "%d", manual_y);
	RegSetValueEx(hKey, "manual_y", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

	// Macro
	sprintf_s(str, 256, "%s", macro.GetString());
	RegSetValueEx(hKey, "macro", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

	RegCloseKey(hKey);
}
