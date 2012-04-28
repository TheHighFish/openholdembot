#include "StdAfx.h"
#include "CFileSystemMonitor.h"

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

CFileSystemMonitor *p_file_system_monitor = NULL;


CFileSystemMonitor::CFileSystemMonitor()
{
	absolute_path_to_scraper_directory =
		CString(_startup_path) + "\\scraper\\";
	InitMonitor();
}

CFileSystemMonitor::~CFileSystemMonitor()
{
	// !!! Close monitor?
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
		// !!! printf("\n ERROR: FindFirstChangeNotification function failed.\n");
		ExitProcess(GetLastError()); 
	}
}

bool CFileSystemMonitor::AnyChanges()
{
	dwWaitStatus = WaitForMultipleObjects(
		1,					// number of handles
		&dwChangeHandle,	// handle
		FALSE,				// ???
		0);					// time to wait
	if (dwWaitStatus == WAIT_OBJECT_0)
	{
		MessageBox(0, "directory changed", "FileMon", 0);
		return true;
	}
	MessageBox(0, "No changes", "FileMon", 0);
	return false;
}

