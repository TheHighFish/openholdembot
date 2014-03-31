#ifndef INC_CFILESYSTEM_MONITOR_H
#define INC_CFILESYSTEM_MONITOR_H

extern class CFileSystemMonitor
{
public:
	CFileSystemMonitor();
	~CFileSystemMonitor();
public:
	bool AnyChanges();
private:
	void InitMonitor();
private:
	CString absolute_path_to_scraper_directory;
	DWORD dwWaitStatus; 
	HANDLE dwChangeHandle;
} * p_filesystem_monitor;

#endif // INC_CFILESYSTEM_MONITOR_H