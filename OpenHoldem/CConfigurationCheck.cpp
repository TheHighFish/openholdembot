#include "stdafx.h"

#include "CConfigurationCheck.h"
#include "CPreferences.h"


CConfigurationCheck *p_configurationcheck = 0;

// A full list of keyboard layout codes can be found here:
// http://msdn.microsoft.com/en-us/goglobal/bb895996.aspx
const TCHAR k_KeyboardLayout_UK_US_English[KL_NAMELENGTH] = "00000409";

const int k_NumberOfRequiredLibraries = 3;
char k_RequiredLibraries[k_NumberOfRequiredLibraries][13] = {"MSVCRT80.dll", 
	"MSVCP80D.DLL", "MSVCR80D.DLL"};


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
	if (prefs.configurationcheck_perl_dependencies())
	{
		CheckForMissingPerlLibraries();
	}

}

void CConfigurationCheck::CheckForMissingPerlLibraries()
{
	for (int i=0; i<k_NumberOfRequiredLibraries; i++)
	{
		HMODULE hMSVCRT = LoadLibrary(k_RequiredLibraries[i]);
		if (hMSVCRT != NULL)
		{
			FreeLibrary(hMSVCRT);
		}
		else
		{
			CString ErrorMessage = CString(k_RequiredLibraries[i]) +  CString(" could not be loaded.\n")
				+ CString("That library may be required by Perl.\n")
				+ CString("If you don't use Perl you may turn that warning off.\n")
				+ CString("If your setup causes problems you should install the missing DLL(s).\n");
			MessageBox(0, ErrorMessage, "Caution: Missing library", MB_OK|MB_ICONWARNING);
		}
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