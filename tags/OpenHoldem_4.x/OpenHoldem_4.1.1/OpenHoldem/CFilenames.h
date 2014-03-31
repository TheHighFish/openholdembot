extern class CFilenames
{
public:
	CFilenames();
	~CFilenames();
public:
	CString OpenHoldemDirectory();
	CString IniFilePath();
	CString TableMapWildcard();
	CString LogsDirectory();
	CString LogFilename();
	CString DebugTabLogFilename();
	CString PokerTrackerLogFilename();
	CString MiniDumpFilename();
	CString ScraperDirectory();
	CString ReplaySessionDirectory();
	CString ReplayBitmapFilename(int frame_number);
	CString ReplayHTMLFilename(int frame_number);
	CString PathOfExecutable();
public:
	void SwitchToOpenHoldemDirectory();
private:
	CString IniFilename();
private:
	char _startup_path[MAX_PATH];
} *p_filenames;