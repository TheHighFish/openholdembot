#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: File-handling for OpenHoldem
//
//******************************************************************************

#ifdef FILES_DLL_EXPORTS
#define FILES_DLL_API extern "C" __declspec(dllexport)
#else
#define FILES_DLL_API extern "C" __declspec(dllimport)
#endif

#include <afxwin.h>

// OpenPPL-files in the botlogic-directory
const int kNumberOfOpenPPLLibraries = 24;
const CString kOpenPPLLibraries[kNumberOfOpenPPLLibraries] = {
  "OpenPPL_Library_Ante_Others_IsFinalTable.ohf",
  "OpenPPL_Library_Bets_Pots_And_Stacks.ohf",
  "OpenPPL_Library_Betting_Action_Symbols.ohf",
  "OpenPPL_Library_Board_Symbols.ohf",
  "OpenPPL_Library_ChipAmounts.ohf",
  "OpenPPL_Library_Constants.ohf",
  "OpenPPL_Library_Deprecated.ohf",
  "OpenPPL_Library_HandStrength_Symbols.ohf",
  "OpenPPL_Library_History_Symbols.ohf",
  "OpenPPL_Library_HoldEm_HandStrength_Symbols.ohf",
  "OpenPPL_Library_HoldEm_NutFullHouseOrFourOfAKind.ohf",
  "OpenPPL_Library_Omaha_Board_Symbols.ohf",
  "OpenPPL_Library_Omaha_HandStrength_Symbols.ohf",
  "OpenPPL_Library_Omaha_NutFullHouseOrFourOfAKind.ohf",
  "OpenPPL_Library_Players_and_Opponents.ohf",
  "OpenPPL_Library_Position_Symbols.ohf",
  "OpenPPL_Library_Technical_Betting_Action_Symbols.ohf",
  "OpenPPL_Library_Technical_Card_Symbols.ohf",
  "OpenPPL_Library_Technical_HandStrength_Symbols.ohf",
  "OpenPPL_Library_Technical_Position_Symbols.ohf",
  "OpenPPL_Library_Technical_Stacksizes.ohf",
  "OpenPPL_Library_Tournament_Symbols.ohf",
  "OpenPPL_Library_Update_History.ohf",
  "OpenPPL_Library_Verbosity.ohf",
};

// Directories
FILES_DLL_API CString BotlogicDirectory();
FILES_DLL_API CString DefaultLogicDirectory();
FILES_DLL_API CString LogsDirectory();
FILES_DLL_API CString OpenHoldemDirectory();
FILES_DLL_API CString OpenPPLLibraryDirectory();
FILES_DLL_API CString PathOfExecutable();
FILES_DLL_API CString ReplaySessionDirectory(int session_ID);
FILES_DLL_API CString ScraperDirectory();
FILES_DLL_API CString ToolsDirectory();
// Filenames
FILES_DLL_API CString ExecutableFilename();
FILES_DLL_API CString IniFilename();
FILES_DLL_API CString PureExecutableFilename();
FILES_DLL_API CString ReplayBitmapFilename(int session_ID, int frame_number);
FILES_DLL_API CString ReplayHTMLFilename(int session_ID, int frame_number);
// Paths
FILES_DLL_API CString CustomLibraryPath();
FILES_DLL_API CString IniFilePath();
FILES_DLL_API CString LogFilePath(int session_ID);
FILES_DLL_API CString ManualModePath();
FILES_DLL_API CString OpenHoldemManualpath();
FILES_DLL_API CString OpenPPLManualpath();
FILES_DLL_API CString VersusPath();
// Others
FILES_DLL_API void OpenFileInExternalSoftware(CString file_name);
FILES_DLL_API CString TableMapWildcard();
FILES_DLL_API void SwitchToOpenHoldemDirectory();
//!!bool FileExists(CString filename_or_pattern);
FILES_DLL_API CString FilenameWithoutPath(CString path);
FILES_DLL_API CString FilenameWithoutPathAndExtension(CString path);
