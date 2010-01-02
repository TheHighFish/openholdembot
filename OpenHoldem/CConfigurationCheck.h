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
	void CheckForMissingLibraries();
	void CheckFontSmoothing();
} *p_cconfigurationcheck;

#endif // INC_CCONFIGURATIONCHECK_H