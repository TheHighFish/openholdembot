//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Monitorinf the scraper directory for
//   * changed files
//   * new files
//   * renamed files
//   and providing a method to check for changes.
//   The autoconnector will then automatically 
//   reload all tablemaps on the next attempt to connect
//
//******************************************************************************

#ifndef INC_CFILESYSTEM_MONITOR_H
#define INC_CFILESYSTEM_MONITOR_H

#include "CSpaceOptimizedGlobalObject.h"

class CFileSystemMonitor : public CSpaceOptimizedGlobalObject {
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

extern CFileSystemMonitor * p_filesystem_monitor;

#endif // INC_CFILESYSTEM_MONITOR_H