#include "stdafx.h"

#include "CConfigurationCheck.h"
#include "CPreferences.h"
#include "OH_MessageBox.h"


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
	CheckForSwapMouseBtns();

	// OpenHoldem may or may not work, if these are not right.
	if (preferences.configurationcheck_input_settings())
	{
		CheckInputSettings();
	}

	if (preferences.configurationcheck_theme_settings())
	{
		CheckForClassicalTheme();
	}

	if (preferences.configurationcheck_font_settings())
	{
		CheckForFontSmoothing();
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
	return (preferences.perl_default_formula() != "");
}

HKEY CConfigurationCheck::GetHive(CString mhive)
{
	HKEY hive;

	if (mhive == "HKCU")
		hive = HKEY_CURRENT_USER;

	if (mhive == "HKLM")
		hive = HKEY_LOCAL_MACHINE;

	return hive;
}

bool CConfigurationCheck::OpenKey(CString mhive, CString registry_path)
{
	HKEY hive, hKey;
	hive = GetHive(mhive);

	if (RegOpenKeyEx(hive, registry_path, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return true;
	}

	RegCloseKey(hKey);
	return false;
}


CString CConfigurationCheck::GetValue(CString mhive, int type, CString registry_path, CString key_name)
{
	DWORD dwData, nBytes, dwType;
	CString value;
	HKEY hive, pKey;
	hive = GetHive(mhive);

	// Open the Key
	if(RegOpenKeyEx(hive,registry_path,0,KEY_QUERY_VALUE,&pKey)==ERROR_SUCCESS)
	{
		switch(type)
		{
			//REG_DWORD
			case 0 :

				if(RegQueryValueEx(pKey,key_name,NULL,&dwType,(LPBYTE)&dwData,&nBytes)==ERROR_SUCCESS)
				{
					value.Format("%d", dwData);
					RegCloseKey(pKey);
					return value;
				}

			//REG_SZ, REG_EXPAND_SZ
			case 1 :

				if(RegQueryValueEx(pKey,key_name,NULL,&dwType,0,&nBytes)==ERROR_SUCCESS)
				{
					// Allocate string buffer
					LPTSTR buffer = value.GetBuffer(nBytes/sizeof(TCHAR));
					// Read string
					VERIFY(RegQueryValueEx(pKey,key_name,NULL,0,(LPBYTE)buffer,&nBytes)==ERROR_SUCCESS);
					// Release string buffer
					value.ReleaseBuffer();
					//printf("Get SZ Value OK\n");
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

void CConfigurationCheck::CheckColourDepth()
{
	CWindowDC dc(NULL);
	int nBitsPerPixel = dc.GetDeviceCaps(PLANES) * dc.GetDeviceCaps(BITSPIXEL);
	if (nBitsPerPixel < 24)
		OH_MessageBox_Error_Warning("It appears that your Display settings are not configured according to OpenHoldem specifications.\n"
				"24 bit color or higher is needed to reliably extract information from the poker client.\n\n"
				"For more info, look at the manual and the user forums",
				"Caution: Color Depth Too Low");
}

void CConfigurationCheck::CheckInputSettings()
{
	TCHAR KeyboardLayout[KL_NAMELENGTH];
	bool Success = GetKeyboardLayoutName((LPSTR)&KeyboardLayout);

	if (Success && (_tcscmp(KeyboardLayout, k_KeyboardLayout_UK_US_English) != 0))
	{
		OH_MessageBox_Error_Warning("You seem to have non-english keyboard settings.\n"
				"Keyboard settings affect especially the decimal point in numbers\n"
				"and therefore the scraper-engine and the auto-player.\n"
				"If you continue, OpenHoldem may or may not work as expected.\n"
				"If you are an experienced user with a working setup\n"
				"you may turn this warning off.\n"
				"If you are new to OpenHoldem or encounter problems\n"
				"you should fix your keyboard settings\n",
				"Caution: Improper keyboard settings");
	}
}

void CConfigurationCheck::CheckForSwapMouseBtns()
{
	CString p_szKeySwapBtns = "Control Panel\\Mouse\\";
	CString p_szNameSwapBtns = "SwapMouseButtons";

	bool SwapBtns = false;

	if (atoi(GetValue("HKCU", 1, p_szKeySwapBtns, p_szNameSwapBtns)) == 1)
	{
		SwapBtns = true;
	}

	if (SwapBtns == true)
	{
		OH_MessageBox_Error_Warning("It appears that your Mouse settings are not configured according to OpenHoldem specifications.\n\n"
			
			"The autoplayer has to generate mouse-events via a very low-level system-interface,\n"
			"and windows might/will swap these hardware-events for their inverse logical responses.\n\n"
			"This means Casinos clients will no longer interpret mouse-events correctly !\n"
			"Please Disable the - Swap Mouse Buttons - option in :\n\n"
			"Control Panel-> Mouse.\n\n"
			"and restart your user session in order to proceed\n",
			"Caution: Swap Mouse Buttons Activated");
	}
}

void CConfigurationCheck::CheckForClassicalTheme()
{
	CString p_szKeyThemeActive = "Software\\Microsoft\\Windows\\CurrentVersion\\ThemeManager\\";
	CString p_szKeyWinVersion = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\";

	CString p_szNameThemeActive = "ThemeActive";
	CString p_szNameWinVersion = "CurrentVersion";

	bool classic_theme = false;

	CString win_Vs = GetValue("HKLM", 1, p_szKeyWinVersion, p_szNameWinVersion);

	CString WIN_2000 = "5.0";
	CString SRV_2003 = "5.2";

	if(win_Vs == WIN_2000 || win_Vs == SRV_2003)
	{
		classic_theme = true;
	}

	else if (atoi(GetValue("HKCU", 1, p_szKeyThemeActive, p_szNameThemeActive)) == 0)
	{
		classic_theme = true;
	}

	if (classic_theme == false)
	{
		OH_MessageBox_Error_Warning("Classical Theme Not Found\n"
			"Settings deviate from recommended defaults.\n"
			"\n"
			"Please enable windows classical theme.\n"
			"for optimum compatibility \n",
			"Caution: Classical Theme Disabled");
	}
}


void CConfigurationCheck::CheckForFontSmoothing()
{
	CString p_szKeyFontSmoothing = "Control Panel\\Desktop\\";
	CString p_szNameFontSmooth = "FontSmoothing";
	CString p_szNameFontSmoothType = "FontSmoothingType";

	bool font_smoothing = true;

	if (atoi(GetValue("HKCU", 1, p_szKeyFontSmoothing, p_szNameFontSmooth)) == 0)
	{
		if (atoi(GetValue("HKCU", 0, p_szKeyFontSmoothing, p_szNameFontSmoothType)) == 0)
		{
			font_smoothing = false;
		}
	}

	if (font_smoothing == true)
	{
		if (atoi(GetValue("HKCU", 1, p_szKeyFontSmoothing, p_szNameFontSmooth)) == 2)
		{
			int FontSmoothType = atoi(GetValue("HKCU", 0, p_szKeyFontSmoothing, p_szNameFontSmoothType));

			if (FontSmoothType == 2)
			{
				OH_MessageBox_Error_Warning("It appears that Cleartype font smoothing is enabled.\n"
							"\n"
							"In order for OpenHoldem to reliably\n"
							"extract information from the poker client\n"
							"you should disable Font Smoothing.",
							"Caution: Font smoothing is enabled");
			}

			else if (FontSmoothType == 1)
			{
				OH_MessageBox_Error_Warning("It appears that Standard font smoothing is enabled.\n"
							"\n"
							"In order for OpenHoldem to reliably\n"
							"extract information from the poker client\n"
							"you should disable Font Smoothing.",
							"Caution: Font smoothing is enabled");
			}
		}
	}
}

void CConfigurationCheck::CheckForMissingMSVCRT()
{
	CString p_szKeyCRT = "SOFTWARE\\Microsoft\\DevDiv\\VC\\Servicing\\8.0\\RED\\1033\\";
	CString p_szNameCRT = "Install";

	bool installed = false;

	if (atoi(GetValue("HKLM", 0, p_szKeyCRT, p_szNameCRT)) == 1)
	{
		installed = true;
	}

	if (installed == false)
	{
		OH_MessageBox_Error_Warning("Unable to detect\n"
			"Microsoft Visual C++ 2005 redistributable runtime library.\n"
			"\n"
			"This library is necessary for Perl users.\n"
			"if you don't use Perl you may turn this warning off\n"
			"by not loading the perl interpreter by default.",
			"Caution: MSVCRT 8.0 missing");
	}
}

void CConfigurationCheck::CheckForMissingActivePerl()
{
	CString p_szKeyAP = "SOFTWARE\\Activestate\\ActivePerl\\";

	if (!OpenKey("HKLM", p_szKeyAP))
	{
		OH_MessageBox_Error_Warning("Unable to detect\n"
			"ActiveState ActivePerl.\n"
			"\n"
			"This version is required for Perl users.\n"
			"if you don't use Perl you may turn this warning off\n"
			"by not loading the perl interpreter by default.",
			"Caution: ActivePerl missing");
	}
}

void CConfigurationCheck::CheckForPerlPath()
{
	CString p_szKeyP = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\";
	CString p_szNameP = "Path";

	CString path = GetValue("HKLM", 1, p_szKeyP, p_szNameP);

	if (path.Find("Perl") == k_undefined)
	{
		OH_MessageBox_Error_Warning("Path\n"
			"Perl was not correctly detected in your Path.\n"
			"\n"
			"If you don't use Perl you may turn this warning off\n"
			"by not loading the perl interpreter by default.",
			"Caution: Perl not found in %PATH%");
	}
}
