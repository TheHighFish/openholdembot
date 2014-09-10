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

#ifndef INC_CCONFIGURATIONCHECK_H
#define INC_CCONFIGURATIONCHECK_H

class CConfigurationCheck
{

public:
	CConfigurationCheck();
	~CConfigurationCheck();

private:
	void CheckEverything();

private:
	HKEY GetHive(CString mhive);
	bool OpenKey(CString mhive, CString registry_path);
	CString GetValue(CString mhive, int type, CString registry_path, CString key_name);
	void CheckColourDepth();
	void CheckInputSettings();
	void CheckForSwapMouseBtns();
	void CheckForClassicalTheme();
	void CheckForFontSmoothing();

private:
	bool CheckOfPerlInstallationNecessary();
	void CheckForMissingMSVCRT();
	void CheckForMissingActivePerl();
	void CheckForPerlPath();
};

extern CConfigurationCheck *p_configurationcheck;

#endif // INC_CCONFIGURATIONCHECK_H
