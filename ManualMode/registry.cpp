
#include "stdafx.h"

#include "registry.h"
#include "stdio.h"
#include "stdlib.h"
#include "debug.h"

Registry::Registry(void) {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif
}

void Registry::read_reg(void) {
#ifdef SEH_ENABLE
	try {
#endif
		HKEY		hKey;
		DWORD		dwType, cbData;
		LONG		hkResult;
		char		str[256];

		// Defaults
		manual_x = manual_y = 0;
		macro = "RPPPPPPPPPPbB";

		// Manual Mode Settings
		hkResult = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\OpenHoldem\\OpenHoldem", 0, KEY_READ, &hKey);
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
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Registry::read_reg : \n"); 
		throw;
	}
#endif
}

void Registry::write_reg(void) {
#ifdef SEH_ENABLE
	try {
#endif
		HKEY		hKey;
		DWORD		dwDisp;
		char		str[256];

		// Settings
		RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\OpenHoldem\\OpenHoldem", 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisp);

		// Window location and size
		sprintf(str, "%d", manual_x);
		RegSetValueEx(hKey, "manual_x", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		sprintf(str, "%d", manual_y);
		RegSetValueEx(hKey, "manual_y", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		// Macro
		sprintf(str, "%s", macro.GetString());
		RegSetValueEx(hKey, "macro", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

		RegCloseKey(hKey);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Registry::write_reg : \n"); 
		throw;
	}
#endif
}
