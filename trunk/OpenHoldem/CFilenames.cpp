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

#include "stdafx.h"
#include "CFilenames.h"

#include "CSessionCounter.h"
#include "OH_MessageBox.h"

CFilenames *p_filenames = NULL;

const CString k_default_ini_filename = "OpenHoldem_Preferences__feel_free_to_rename_this_file_to_whatever_you_like.INI";

CFilenames::CFilenames()
{
	// Save startup directory
	::GetCurrentDirectory(MAX_PATH - 1, _startup_path);
	// Create logs-directoy if necessary
	CreateDirectory(LogsDirectory(), NULL);
}

CFilenames::~CFilenames()
{}

CString CFilenames::OpenHoldemDirectory()
{
	assert(_startup_path != "");
	return _startup_path;
}

CString CFilenames::IniFilePath()
{
	// We need the complete path here,
	// otherwise the file would be expected in the default location,
	// i.e. the Windows-directory.
	return OpenHoldemDirectory() + CString("\\") + IniFilename();
}

CString CFilenames::IniFilename()
{
	WIN32_FIND_DATA find_file_data;
	HANDLE h_find;
	CString wildcard = OpenHoldemDirectory() + "\\*.INI";
	CString ini_filename;

	// Try to find "first" ini-file.
	// No matter how it is named -- it is the right one.
	// http://msdn.microsoft.com/en-us/library/windows/desktop/aa364418(v=vs.85).aspx
	h_find = FindFirstFile(wildcard, &find_file_data);
	if (h_find == INVALID_HANDLE_VALUE) 
	{
		// No ini-file found.
		// Use default one (for saving).
		FindClose(h_find);
		return k_default_ini_filename;
	}
	ini_filename = find_file_data.cFileName;
	// Check that no more ini-files exist (potential problems).
	// http://msdn.microsoft.com/en-us/library/windows/desktop/aa364428(v=vs.85).aspx
	if (FindNextFile(h_find, &find_file_data))
	{
		FindClose(h_find);
		OH_MessageBox_Error_Warning(
			"More than one ini-file in OpenHoldem-directory.\n"
			"Don't know which one to use.\n"
			"\n"
			"Going to terminate...",
			"ERROR");
		PostQuitMessage(-1);
		// Previously we returned "a_result_to_make_the_compiler_happy.ini"
		// believing the result was meaning-less and would never be used.
		// However PostQuitMessage(-1) uses a message-queue,
		// so it may take some time, and OH will continue a little bit.
		// Therefore we take the last filename now in order to 
		// not create yet another one with a funny name. ;-)
		// http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=16229
		return ini_filename;
	}
	FindClose(h_find);

	// Exactly one ini-file found
	return ini_filename;
}

void CFilenames::SwitchToOpenHoldemDirectory()
{
	assert(_startup_path != "");
	SetCurrentDirectory(_startup_path);
}

CString CFilenames::TableMapWildcard()
{
	assert(_startup_path != "");
	CString wildcard;
	wildcard.Format("%s\\scraper\\*.tm", _startup_path);
	return wildcard;
}

CString CFilenames::DebugTabLogFilename()
{
	assert(_startup_path != "");
	CString filename;
	filename.Format("%s\\f$debug_%lu.log", LogsDirectory(), p_sessioncounter->session_id());
	return filename;
}

CString CFilenames::ScraperDirectory()
{
	assert(_startup_path != "");
	return CString(_startup_path) + "\\scraper\\";
}

CString CFilenames::ReplaySessionDirectory()
{
	assert(_startup_path != "");
	CString path;
	path.Format("%s\\replay\\session_%lu\\", _startup_path, p_sessioncounter->session_id());
	return path;
}

CString CFilenames::ReplayBitmapFilename(int frame_number)
{
	CString path;
	if (frame_number == k_undefined)
		path.Format("%s\\*.bmp", ReplaySessionDirectory());
	else
		path.Format("%s\\frame%06d.bmp", ReplaySessionDirectory(), frame_number);
	return path;
}

CString CFilenames::ReplayHTMLFilename(int frame_number)
{
	CString path;
	path.Format("%s\\frame%06d.htm", ReplaySessionDirectory(), frame_number);
	return path;
}

CString CFilenames::LogsDirectory()
{
	assert(_startup_path != "");
	CString path;
	path.Format("%s\\logs\\", _startup_path);
	return path;
};

CString CFilenames::LogFilename()
{
	CString path;
	path.Format("%s\\oh_%lu.log", LogsDirectory(), p_sessioncounter->session_id());
	return path;
}

CString CFilenames::MiniDumpFilename()
{
	SYSTEMTIME	stLocalTime;
	char		szFileName[MAX_PATH]; 

	GetLocalTime(&stLocalTime);
	assert(_startup_path != "");
	sprintf_s(szFileName, MAX_PATH, "%s\\%s-%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp", 
		_startup_path, "OpenHoldem", VERSION_TEXT, stLocalTime.wYear, stLocalTime.wMonth, 
		stLocalTime.wDay, stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, 
		GetCurrentProcessId(), GetCurrentThreadId());
	return szFileName;
}

CString CFilenames::PathOfExecutable()
{
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms683197(v=vs.85).aspx
	// http://stackoverflow.com/questions/124886/how-to-get-the-application-executable-name-in-windows-c-win32-or-c-cli
	TCHAR exe_path[MAX_PATH+1];
	GetModuleFileName(0, exe_path, MAX_PATH+1);
	return exe_path;
}