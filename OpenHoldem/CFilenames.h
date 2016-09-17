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

// OpenPPL-files in the botlogic-directory
const int kNumberOfOpenPPLLibraries = 9;
const CString kOpenPPLLibraries[kNumberOfOpenPPLLibraries] = {
  "OpenPPL_Library_01_Constants.ohf",
  "OpenPPL_Library_02_Verbosity.ohf",
  "OpenPPL_Library_03_Technical_Card_Symbols.ohf",
  "OpenPPL_Library_04_Board_Symbols.ohf",
  "OpenPPL_Library_06_HandStrength_Symbols.ohf",
  "OpenPPL_Library_07_NutFullHouseOrFourOfAKind.ohf",
  "OpenPPL_Library_08_Ante_Others_IsFinalTable.ohf",
  "OpenPPL_Library_09_Technical_Stacksizes.ohf",
  "OpenPPL_Library_10_Stacksizes.ohf"
};

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
  CString ToolsDirectory();
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
  CString CustomLibraryPath();
  CString ManualModePath();
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