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
		OH_MessageBox(
			"More than one ini-file in OpenHoldem-directory.\n"
			"Don't know which one to use.\n"
			"\n"
			"Going to terminate...",
			"ERROR", 0);
		PostQuitMessage(-1);
		return "a_result_to_make_the_compiler_happy.ini";
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
	filename.Format("%s\\f$debug_%lu.log", _startup_path, p_sessioncounter->session_id());
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

CString CFilenames::LogFilename()
{
	assert(_startup_path != "");
	CString path;
	path.Format("%s\\oh_%lu.log", _startup_path, p_sessioncounter->session_id());
	return path;
}

CString CFilenames::PokerTrackerLogFilename()
{
	assert(_startup_path != "");
	CString path;
	path.Format("%s\\oh_pt_%lu.log", _startup_path, p_sessioncounter->session_id());
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