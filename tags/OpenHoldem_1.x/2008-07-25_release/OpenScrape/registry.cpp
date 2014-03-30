
#include "stdafx.h"

#include "registry.h"
#include "stdio.h"
#include "stdlib.h"
#include "structs_defines.h"

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
	main_x = main_y = 0;
	main_dx = 800;
	main_dy = 400;

	tablemap_x = tablemap_y = 0;
	tablemap_dx = 600;
	tablemap_dy = 750;

	grhash_x = grhash_y = 0;
	grhash_dx = 380;
	grhash_dy = 330;

	hkResult = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\OpenHoldem\\OpenScrape", 0, KEY_READ, &hKey);
	if (hkResult==ERROR_SUCCESS) {

		// Main window location and size
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "main_x", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			main_x = atoi(str);
		}
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "main_y", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			main_y = atoi(str);
		}
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "main_dx", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			main_dx = atoi(str);
		}
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "main_dy", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			main_dy = atoi(str);
		}

		// tablemap window location and size
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "tablemap_x", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			tablemap_x = atoi(str);
		}
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "tablemap_y", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			tablemap_y = atoi(str);
		}
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "tablemap_dx", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			tablemap_dx = atoi(str);
		}
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "tablemap_dy", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			tablemap_dy = atoi(str);
		}

		// graphical hash point window location and size
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "grhash_x", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			grhash_x = atoi(str);
		}
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "grhash_y", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			grhash_y = atoi(str);
		}
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "grhash_dx", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			grhash_dx = atoi(str);
		}
		cbData = sizeof(str);
		if ( (hkResult = RegQueryValueEx(hKey, "grhash_dy", NULL, &dwType, (LPBYTE) str, &cbData)) == ERROR_SUCCESS) {
			grhash_dy = atoi(str);
		}
	}

	RegCloseKey(hKey);
}

void Registry::write_reg(void) 
{
	HKEY		hKey;
	DWORD		dwDisp;
	char		str[256];

	//PokerTracker Settings
	RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\OpenHoldem\\OpenScrape", 0, NULL, 
	REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisp);

	// Main window location and size
	sprintf(str, "%d", main_x);
	RegSetValueEx(hKey, "main_x", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
	sprintf(str, "%d", main_y);
	RegSetValueEx(hKey, "main_y", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
	sprintf(str, "%d", main_dx);
	RegSetValueEx(hKey, "main_dx", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
	sprintf(str, "%d", main_dy);
	RegSetValueEx(hKey, "main_dy", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

	// tablemap window location and size
	sprintf(str, "%d", tablemap_x);
	RegSetValueEx(hKey, "tablemap_x", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
	sprintf(str, "%d", tablemap_y);
	RegSetValueEx(hKey, "tablemap_y", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
	sprintf(str, "%d", tablemap_dx);
	RegSetValueEx(hKey, "tablemap_dx", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
	sprintf(str, "%d", tablemap_dy);
	RegSetValueEx(hKey, "tablemap_dy", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

	// graphical hash point window location and size
	sprintf(str, "%d", grhash_x);
	RegSetValueEx(hKey, "grhash_x", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
	sprintf(str, "%d", grhash_y);
	RegSetValueEx(hKey, "grhash_y", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
	sprintf(str, "%d", grhash_dx);
	RegSetValueEx(hKey, "grhash_dx", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);
	sprintf(str, "%d", grhash_dy);
	RegSetValueEx(hKey, "grhash_dy", 0, REG_SZ, (LPBYTE) str, (DWORD) strlen(str)+1);

	RegCloseKey(hKey);
}
