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
		CheckForMissingActivePerl();
		CheckForPerlPath();
	}
}

bool CConfigurationCheck::CheckOfPerlInstallationNecessary()
{
	// We better make that check dependend of the perl-preferences,
	// instead of a separate option that might be turned off
	// or might confuse other users.
	return (prefs.perl_load_default_formula() || prefs.perl_load_interpreter());
}

bool CConfigurationCheck::OpenKey(CString registry_path)
{
	HKEY hKey;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, registry_path, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return true;
	}

	RegCloseKey(hKey);
	return false;
}

CString CConfigurationCheck::GetValue(int type, CString registry_path, CString key_name)
{
	DWORD dwData, nBytes, dwType;
	CString value;
	HKEY pKey;

	// Open the Key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,registry_path,0,KEY_QUERY_VALUE,&pKey) == ERROR_SUCCESS)
	{
		switch(type)
    		{
      			//REG_DWORD
      			case 0 :

        			if (RegQueryValueEx(pKey,key_name,NULL,&dwType,(LPBYTE)&dwData,&nBytes) == ERROR_SUCCESS)
        			{
          				value.Format("%d", dwData);
          				RegCloseKey(pKey);
          				return value;
        			}

      			//REG_EXPAND_SZ
      			case 1 :

        			if (RegQueryValueEx(pKey,key_name,NULL,&dwType,0,&nBytes) == ERROR_SUCCESS)
        			{
          				// Allocate string buffer
          				LPTSTR buffer = value.GetBuffer(nBytes/sizeof(TCHAR));
          				// Read string
          				VERIFY(RegQueryValueEx(pKey,key_name,NULL,0,(LPBYTE)buffer,&nBytes) == ERROR_SUCCESS);
          				// Release string buffer
          				value.ReleaseBuffer();
          				RegCloseKey(pKey);
          				return value;
        			}

      			default :
        			return NULL;
    		}
  	}

  	else
  	{
    		return NULL;
  	}
}

void CConfigurationCheck::CheckForMissingMSVCRT()
{
	CString p_szKeyCRT = "SOFTWARE\\Microsoft\\DevDiv\\VC\\Servicing\\8.0\\RED\\1033\\";
	CString p_szNameCRT = "Install";

	bool installed = false;

	if (OpenKey(p_szKeyCRT) && atoi(GetValue(0, p_szKeyCRT, p_szNameCRT)) == 1)
	{
		installed = true;
	}

	if (installed == false)
	{
		MessageBox(0, "Unable to detect\n"
			"Microsoft Visual C++ 2005 redistributable runtime library.\n"
			"\n"
			"This library is necessary for Perl users.\n"
			"if you don't use Perl you may turn this warning off\n"
			"by not loading the perl interpreter by default.",
			"Caution: MSVCRT 8.0 missing", MB_OK|MB_ICONWARNING);
	}
}

void CConfigurationCheck::CheckForMissingActivePerl()
{
	CString p_szKeyAP = "SOFTWARE\\Activestate\\ActivePerl\\";

	if (!OpenKey(p_szKeyAP))
	{
		MessageBox(0, "Unable to detect\n"
			"ActiveState ActivePerl.\n"
			"\n"
			"This version is required for Perl users.\n"
			"if you don't use Perl you may turn this warning off\n"
			"by not loading the perl interpreter by default.",
			"Caution: ActivePerl missing", MB_OK|MB_ICONWARNING);
	}
}

void CConfigurationCheck::CheckForPerlPath()
{
  CString p_szKeyP = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\";
  CString p_szNameP = "Path";

  CString path = GetValue(1, p_szKeyP, p_szNameP);

  if (path.Find("Perl") == -1)
  {
    	MessageBox(0, "Path\n"
			"Perl was not correctly detected in your Path.\n"
			"\n"
			"If you don't use Perl you may turn this warning off\n"
			"by not loading the perl interpreter by default.",
			"Caution: Perl not found in %PATH%", MB_OK|MB_ICONWARNING);
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
