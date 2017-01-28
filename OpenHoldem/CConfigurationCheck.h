//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
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
public:
  // For Menu -> ProblemSolver
  void ForceAllConfigurationChercks();
private:
  void CheckEnabledConfigurationChecks(bool force_all);
private:
	HKEY GetHive(CString mhive);
	bool OpenKey(CString mhive, CString registry_path);
	CString GetValue(CString mhive, int type, CString registry_path, CString key_name);
	void CheckColourDepth();
	void CheckInputSettings();
	void CheckForSwapMouseBtns();
	void CheckForFontSmoothing();
};

extern CConfigurationCheck *p_configurationcheck;

#endif // INC_CCONFIGURATIONCHECK_H
