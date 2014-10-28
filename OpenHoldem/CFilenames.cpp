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

#include "stdafx.h"
#include "CFilenames.h"

#include "CPreferences.h"
#include "CSessionCounter.h"
#include "OH_MessageBox.h"

CFilenames *p_filenames = NULL;

const char* k_default_ini_filename = "OpenHoldem_Preferences__feel_free_to_rename_this_file_to_whatever_you_like.INI";

CFilenames::CFilenames() {
  // Save directory that contains openHoldem for future use.
  // This is NOT GetCurrentDirectory(), which returns the working-directorz,
  // i.e. the working directorz of the process that started OpenHoldem.
  //  
  // If we don't get this path right and the user stars OpenHoldem
  // from a different directorz with a script we won't find our files.
  // http://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
  // http://stackoverflow.com/questions/4517425/how-to-get-program-path
  ::GetModuleFileName(NULL, _startup_path, _MAX_PATH);
  // Remove executable filename from path
  // http://pic.dhe.ibm.com/infocenter/zvm/v6r2/index.jsp?topic=%2Fcom.ibm.zvm.v620.edclv%2Fstrrchr.htm
  char *last_backslash_in_path = strrchr(_startup_path, '\\');
  if (last_backslash_in_path != NULL) {
    ++last_backslash_in_path;
    *last_backslash_in_path = '\0';
  }
  // Create logs-directoy if necessary
  CreateDirectory(LogsDirectory(), NULL);
}

CFilenames::~CFilenames() {
}

void CFilenames::Log(CString name, CString value) {
  CString message;
  message.Format("[CFilenames] %s = %s\n", name, value);
  write_log(preferences.debug_filenames(), (char*)message.GetString());
}

CString CFilenames::OpenHoldemDirectory() {
	assert(_startup_path != "");
  Log("OpenHoldemDirectory", _startup_path);
	return _startup_path;
}

CString CFilenames::IniFilePath() {
	// We need the complete path here,
	// otherwise the file would be expected in the default location,
	// i.e. the Windows-directory.
	return OpenHoldemDirectory() + CString("\\") + IniFilename();
}

CString CFilenames::IniFilename() {
	WIN32_FIND_DATA find_file_data;
	HANDLE h_find;
	CString wildcard = OpenHoldemDirectory() + "\\*.INI";
	CString ini_filename;

	// Try to find "first" ini-file.
	// No matter how it is named -- it is the right one.
	// http://msdn.microsoft.com/en-us/library/windows/desktop/aa364418(v=vs.85).aspx
	h_find = FindFirstFile(wildcard, &find_file_data);
	if (h_find == INVALID_HANDLE_VALUE) {
		// No ini-file found.
		// Use default one (for saving).
		FindClose(h_find);
    Log("IniFilename", k_default_ini_filename);
		return k_default_ini_filename;
	}
	ini_filename = find_file_data.cFileName;
	// Check that no more ini-files exist (potential problems).
	// http://msdn.microsoft.com/en-us/library/windows/desktop/aa364428(v=vs.85).aspx
	if (FindNextFile(h_find, &find_file_data)) {
		FindClose(h_find);
		OH_MessageBox_Error_Warning(
			"More than one ini-file in OpenHoldem-directory.\n"
			"Don't know which one to use.\n"
			"\n"
			"Going to terminate...");
		PostQuitMessage(-1);
		// Previously we returned "a_result_to_make_the_compiler_happy.ini"
		// believing the result was meaning-less and would never be used.
		// However PostQuitMessage(-1) uses a message-queue,
		// so it may take some time, and OH will continue a little bit.
		// Therefore we take the last filename now in order to 
		// not create yet another one with a funny name. ;-)
		// http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=16229
    Log("IniFilename", ini_filename.GetString());
		return ini_filename;
	}
	FindClose(h_find);
	// Exactly one ini-file found
  Log("IniFilename", ini_filename.GetString());
	return ini_filename;
}

void CFilenames::SwitchToOpenHoldemDirectory() {
	assert(_startup_path != "");
	SetCurrentDirectory(_startup_path);
}

CString CFilenames::TableMapWildcard() {
	assert(_startup_path != "");
	CString wildcard;
	wildcard.Format("%sscraper\\*.tm", _startup_path);
  Log("TableMapWildcard", wildcard.GetString());
	return wildcard;
}

CString CFilenames::ScraperDirectory() {
	assert(_startup_path != "");
  CString scraper_dir = CString(_startup_path) + "scraper\\";
  Log("ScraperDirectory", (char*)scraper_dir.GetString());
	return scraper_dir;
}

CString CFilenames::ReplaySessionDirectory() {
	assert(_startup_path != "");
	CString path;
	path.Format("%sreplay\\session_%lu\\", _startup_path, p_sessioncounter->session_id());
	return path;
}

CString CFilenames::ReplayBitmapFilename(int frame_number) {
	CString path;
	if (frame_number == k_undefined)
		path.Format("%s*.bmp", ReplaySessionDirectory());
	else
		path.Format("%sframe%06d.bmp", ReplaySessionDirectory(), frame_number);
  Log("ReplayBitmapFilename", path.GetString());
	return path;
}

CString CFilenames::ReplayHTMLFilename(int frame_number) {
	CString path;
	path.Format("%sframe%06d.htm", ReplaySessionDirectory(), frame_number);
  Log("ReplayHTMLFilename", path.GetString());
	return path;
}

CString CFilenames::LogsDirectory() {
	assert(_startup_path != "");
	CString path;
	path.Format("%slogs\\", _startup_path);
  Log("LogsDirectory", path.GetString());
	return path;
};

CString CFilenames::LogFilename() {
	CString path;
	path.Format("%soh_%lu.log", LogsDirectory(), p_sessioncounter->session_id());
  Log("LogFilename", path.GetString());
	return path;
}

CString CFilenames::MiniDumpFilename() {
	SYSTEMTIME	stLocalTime;
	char		szFileName[MAX_PATH]; 

	GetLocalTime(&stLocalTime);
	assert(_startup_path != "");
	sprintf_s(szFileName, MAX_PATH, "%s\\%s-%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp", 
		_startup_path, "OpenHoldem", VERSION_TEXT, stLocalTime.wYear, stLocalTime.wMonth, 
		stLocalTime.wDay, stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, 
		GetCurrentProcessId(), GetCurrentThreadId());
  Log("MiniDumpFilename", szFileName);
  return szFileName;
}

CString CFilenames::PathOfExecutable() {
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms683197(v=vs.85).aspx
	// http://stackoverflow.com/questions/124886/how-to-get-the-application-executable-name-in-windows-c-win32-or-c-cli
	TCHAR exe_path[MAX_PATH+1];
	GetModuleFileName(0, exe_path, MAX_PATH+1);
  Log("PathOfExecutable", exe_path);
	return exe_path;
}

CString CFilenames::ExecutableFilename() {
	CString complete_path = PathOfExecutable();
	int pos = complete_path.ReverseFind('\\');
	if (pos > 0) {
		assert(pos < complete_path.GetLength() - 1);
		return complete_path.Mid(pos + 1);
	}
  Log("ExecutableFilename", complete_path.GetString());
	return complete_path;
}

CString CFilenames::PureExecutableFilename() {
	CString filename = ExecutableFilename();
	int pos = filename.ReverseFind('.');
  CString result;
	if (pos > 0) {
		result = filename.Left(pos);
	} else {
    result = filename;
  }
  Log("PureExecutableFilename", result.GetString());
	return result;
}

CString CFilenames::VersusPath() {
  CString result = OpenHoldemDirectory() + "versus.bin";
  Log("VersusPath", result.GetString());
	return result;
}

CString CFilenames::OpenPPLLibraryPath() {
  CString result = OpenHoldemDirectory() + "\\OpenPPL_Library.ohf";
  Log("OpenPPLLibraryPath", result.GetString());
	return result;
}