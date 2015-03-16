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
	static CString OpenHoldemDirectory();
	static CString IniFilePath();
	static CString TableMapWildcard();
	static CString LogsDirectory();
	static CString LogFilename();
	static CString MiniDumpFilename();
	static CString ScraperDirectory();
	static CString ReplaySessionDirectory();
	static CString ReplayBitmapFilename(int frame_number);
	static CString ReplayHTMLFilename(int frame_number);
	static CString PathOfExecutable();
	static CString ExecutableFilename();
	static CString PureExecutableFilename();
  static CString VersusPath();
  static CString OpenPPLLibraryPath();
 public:
	static void SwitchToOpenHoldemDirectory();
 private:
	static CString IniFilename();
  static void Log(CString name, CString value);
  static CString StartupPath();
};

#endif INC_CFILENAMES_H