#ifndef INC_CCONFIGURATIONCHECK_H
#define INC_CCONFIGURATIONCHECK_H

extern class CConfigurationCheck
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
	void CheckKeyboardSettings();
	void CheckForClassicalTheme();
	void CheckForFontSmoothing();

private:
	bool CheckOfPerlInstallationNecessary();
	void CheckForMissingMSVCRT();
	void CheckForMissingActivePerl();
	void CheckForPerlPath();
} *p_configurationcheck;

#endif // INC_CCONFIGURATIONCHECK_H
