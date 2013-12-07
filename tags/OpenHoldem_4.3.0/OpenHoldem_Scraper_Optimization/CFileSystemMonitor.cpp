#include "StdAfx.h"
#include "CFileSystemMonitor.h"

#include "CFilenames.h"
#include "CPreferences.h"
#include "OpenHoldem.h"


// Class monitors the scraper directory for
// * changed files
// * new files
// * renamed files
// and sprovides a method to check for changes.
// The autoconnector will then automatically 
// reload all tablemaps on the next attempt to connect.
//
// http://msdn.microsoft.com/en-us/library/system.io.filesystemwatcher.aspx
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa364417(v=vs.85).aspx
// http://msdn.microsoft.com/en-us/library/aa364417%28VS.85%29.aspx
// http://msdn.microsoft.com/en-us/library/aa365261(v=vs.85).aspx

CFileSystemMonitor *p_filesystem_monitor = NULL;


CFileSystemMonitor::CFileSystemMonitor()
{
	write_log(prefs.debug_filesystem_monitor(), "[CFileSystemMonitor] executing constructor.)\n");
	absolute_path_to_scraper_directory = p_filenames->ScraperDirectory();
	write_log(prefs.debug_filesystem_monitor(), "[CFileSystemMonitor] Scraper folder: %s\n", 
		absolute_path_to_scraper_directory);
	// Create directory in case it does not exist (mainly debug sessions).
	CreateDirectory(absolute_path_to_scraper_directory, NULL);
	InitMonitor();
}

CFileSystemMonitor::~CFileSystemMonitor()
{
	CloseHandle(dwChangeHandle);
}

void CFileSystemMonitor::InitMonitor()
{
	int changes_to_monitor = FILE_NOTIFY_CHANGE_FILE_NAME
		| FILE_NOTIFY_CHANGE_SIZE
		| FILE_NOTIFY_CHANGE_LAST_WRITE;
	dwChangeHandle = FindFirstChangeNotification(
		absolute_path_to_scraper_directory,
		FALSE,	// do not watch subtree 
		changes_to_monitor);
	if ((dwChangeHandle == INVALID_HANDLE_VALUE) || (dwChangeHandle == NULL))
	{
		write_log(prefs.debug_filesystem_monitor(), "[CFileSystemMonitor] InitMonitor() failed.\n");
		write_log(prefs.debug_filesystem_monitor(), "[CFileSystemMonitor] Going to terminate...\n");
		ExitProcess(GetLastError()); 
	}
}

bool CFileSystemMonitor::AnyChanges()
{
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms687025(v=vs.85).aspx
	dwWaitStatus = WaitForMultipleObjects(
		1,					// number of handles
		&dwChangeHandle,	// handle
		FALSE,				// bWaitAll - does not make a difference here
		0);					// time to wait
	if (dwWaitStatus == WAIT_OBJECT_0)
	{
		write_log(prefs.debug_filesystem_monitor(), "[CFileSystemMonitor] Scraper directoy changed.\n");
		return true;
	}
	write_log(prefs.debug_filesystem_monitor(), "[CFileSystemMonitor] No changes in scraper directoy.\n");
	// Resetting change handle for next query
	dwChangeHandle = 0;
	return false;
}

