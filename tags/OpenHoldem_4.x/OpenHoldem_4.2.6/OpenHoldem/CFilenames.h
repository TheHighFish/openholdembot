//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

extern class CFilenames
{
public:
	CFilenames();
	~CFilenames();
public:
	CString OpenHoldemDirectory();
	CString IniFilePath();
	CString TableMapWildcard();
	CString LogsDirectory();
	CString LogFilename();
	CString DebugTabLogFilename();
	CString MiniDumpFilename();
	CString ScraperDirectory();
	CString ReplaySessionDirectory();
	CString ReplayBitmapFilename(int frame_number);
	CString ReplayHTMLFilename(int frame_number);
	CString PathOfExecutable();
public:
	void SwitchToOpenHoldemDirectory();
private:
	CString IniFilename();
private:
	char _startup_path[MAX_PATH];
} *p_filenames;