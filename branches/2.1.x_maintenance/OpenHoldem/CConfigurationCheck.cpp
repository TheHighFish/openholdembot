#include "stdafx.h"

#include "CConfigurationCheck.h"
#include "CPreferences.h"


CConfigurationCheck *p_configurationcheck = 0;

// A full list of keyboard layout codes can be found here:
// http://msdn.microsoft.com/en-us/goglobal/bb895996.aspx
const TCHAR k_KeyboardLayout_UK_US_English[KL_NAMELENGTH] = "00000409";


CConfigurationCheck::CConfigurationCheck()
{
	CheckEverything();
}

CConfigurationCheck::~CConfigurationCheck()
{}

void CConfigurationCheck::CheckEverything()
{
	// Check really critical settings in any case.
	// OpenHoldem will never work, if these are not right.
	CheckColourDepth();
	CheckFontSmoothing();

	// Check non-critical things, if checks are not disabled.
	// OpenHoldem may or may not work, if these are not right.
	if (prefs.configurationcheck_keyboard_settings())
	{
		CheckKeyboardSettings();
	}
	if (CheckOfPerlInstallationNecessary())
	{
		CheckForMissingMSVCRT();
	}

}

bool CConfigurationCheck::CheckOfPerlInstallationNecessary()
{
	// We better make that check dependend of the perl-preferences,
	// instead of a separate option that might be turned off
	// or might confuse other users.
	return (prefs.perl_load_default_formula() || prefs.perl_load_interpreter());
}

bool CConfigurationCheck::DoesRegistryKeyExist(CString registry_path, CString key_name)
{
	DWORD dwData;					
	HKEY hKey;
	DWORD dwType = REG_DWORD; 
	DWORD dwSize = sizeof(DWORD)*2;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     registry_path,
                     0,
                     KEY_QUERY_VALUE,
                     &hKey) == ERROR_SUCCESS) 
	{
		if(RegQueryValueEx(hKey, 
						   key_name, 
						   NULL, 
						   &dwType, 
						   (LPBYTE)&dwData, 
						   &dwSize) == ERROR_SUCCESS && dwData == 1)
		{
			RegCloseKey(hKey);
			return true;  
		}
	}
	RegCloseKey(hKey);
	return false;
}

void CConfigurationCheck::CheckForMissingMSVCRT()
{
	char p_szKey[64] = "SOFTWARE\\Microsoft\\DevDiv\\VC\\Servicing\\8.0\\RED\\1033\\";
	char p_szName[8] = "Install";

	if (!DoesRegistryKeyExist(p_szKey, p_szName))
	{
		MessageBox(0, "Unable to detect\n"
			"Microsoft Visual C++ 2005 redistributable runtime library.\n" 
			"\n"
			"This library is necessary for Perl users.\n"
			"If you don't use Perl you may turn this warning off.",
			"Caution: MSVCRT 8.0 missing", MB_OK|MB_ICONWARNING);
	}
}

void CConfigurationCheck::CheckKeyboardSettings()
{
	TCHAR KeyboardLayout[KL_NAMELENGTH];
	bool Success = GetKeyboardLayoutName((LPSTR)&KeyboardLayout);

	if (Success && (_tcscmp(KeyboardLayout, k_KeyboardLayout_UK_US_English) != 0))
	{
		MessageBox(0, "You seem to have non-english keyboard settings.\n"
				"Keyboard settings affect especially the decimal point in numbers\n"
				"and therefore the scraper-engine and the auto-player.\n"
				"If you continue, OpenHoldem may or may not work as expected.\n"
				"If you are an experienced user with a working setup\n"
				"you may turn this warning off.\n"
				"If you are new to OpenHoldem or encounter problems\n"
				"you should fix your keyboard settings\n",
				"Caution: Improper keyboard settings", MB_OK|MB_ICONWARNING);
	}
}

void CConfigurationCheck::CheckColourDepth()
{
	CWindowDC dc(NULL);
	int nBitsPerPixel = dc.GetDeviceCaps(PLANES) * dc.GetDeviceCaps(BITSPIXEL);
	if (nBitsPerPixel < 24 && !prefs.disable_msgbox())
		MessageBox(0, "It appears that your Display settings are not configured according to OpenHoldem specifications.\n"
				   "24 bit color or higher is needed to reliably extract information from the poker client.\n\n"
				   "For more info, look at the manual and the user forums", 
				   "Caution: Color Depth Too Low", MB_OK|MB_ICONWARNING);
}

void CConfigurationCheck::CheckFontSmoothing()
{
	BOOL fontSmoothingEnabled = FALSE;
	SystemParametersInfo(SPI_GETFONTSMOOTHING, 0, (LPVOID)&fontSmoothingEnabled, 0);
	if (fontSmoothingEnabled && !prefs.disable_msgbox())
		MessageBox(0, "It appears that font smoothing is enabled.\n" 
					"In order for OpenHoldem to reliably\n"
					"extract information from the poker client\n" 
					"you should disable Font Smoothing.", 
					"Caution: Font smoothing is enabled", MB_OK|MB_ICONWARNING);
}