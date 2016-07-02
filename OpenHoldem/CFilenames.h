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

#ifndef INC_CFILENAMES_H
#define INC_CFILENAMES_H

class CFilenames {
 public:
	CFilenames();
	~CFilenames();
 public:
  // Directories
  CString BotlogicDirectory();
  CString LogsDirectory();
  CString OpenHoldemDirectory();
  CString PathOfExecutable();
  CString ReplaySessionDirectory();
  CString ScraperDirectory();
public:
  // Filenames
	CString LogFilename();
	CString MiniDumpFilename();
	CString ReplayBitmapFilename(int frame_number);
	CString ReplayHTMLFilename(int frame_number);
	CString ExecutableFilename();
	CString PureExecutableFilename();
public:
  // Paths
  CString IniFilePath();
  CString VersusPath();
  CString OpenPPLLibraryPath();
  CString CustomLibraryPath();
public:
  // Others
  CString TableMapWildcard();
 public:
	void SwitchToOpenHoldemDirectory();
  bool Exists(CString filename_or_pattern);
 private:
	CString IniFilename();
  void Log(CString name, CString value);
 private:
	char _startup_path[MAX_PATH];
};

extern CFilenames *p_filenames;

#endif INC_CFILENAMES_H