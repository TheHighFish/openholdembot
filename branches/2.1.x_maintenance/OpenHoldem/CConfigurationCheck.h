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
	void CheckKeyboardSettings();
	void CheckColourDepth();
	void CheckForMissingMSVCRT();
	void CheckFontSmoothing();
private:
	bool CheckOfPerlInstallationNecessary();
	bool DoesRegistryKeyExist(CString registry_path, CString key_name);
} *p_configurationcheck;

#endif // INC_CCONFIGURATIONCHECK_H