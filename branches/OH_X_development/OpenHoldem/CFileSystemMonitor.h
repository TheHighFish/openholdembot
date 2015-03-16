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

#ifndef INC_CFILESYSTEM_MONITOR_H
#define INC_CFILESYSTEM_MONITOR_H

class CFileSystemMonitor {
 public:
	CFileSystemMonitor();
	~CFileSystemMonitor();
 public:
	bool AnyChanges();
 private:
	void InitMonitor();
 private:
	CString absolute_path_to_scraper_directory;
	DWORD  dwWaitStatus; 
	HANDLE dwChangeHandle;
  bool _scraper_directory_on_a_network_drive;
};

#endif // INC_CFILESYSTEM_MONITOR_H