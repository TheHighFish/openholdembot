#ifndef INC_CAUTOCONNECTOR_H
#define INC_CAUTOCONNECTOR_H

#include "..\CCritSec\CCritSec.h"
#include "..\CTablemap\CTablemap.h"


extern class CAutoConnector
{
public:
	CAutoConnector();
	~CAutoConnector();
	bool Connect(HWND targetHWnd);
	void Disconnect();
	bool IsConnected();
	double TimeSinceLastFailedAttemptToConnect(); // seconds
public:
	// public mutators
	const HWND attached_hwnd() { return _attached_hwnd; }
private:
	void ParseAllOpenScrapeOrWinScrapeTableMapsToLoadConnectionData(CString TableMapWildcard);
	void ParseAllTableMapsToLoadConnectionData();
	void ExtractConnectionDataFromCurrentTablemap(SWholeMap *map);
	bool TablemapConnectionDataAlreadyStored(CString TablemapFilePath);
private:
	int SelectTableMapAndWindow(int Choices);
	int SelectTableMapAndWindowManually(int Choices);
	int SelectTableMapAndWindowAutomatically(int Choices);
private:
	void Check_TM_Against_All_Windows_Or_TargetHWND(int TablemapIndex, HWND targetHWnd);
	#define ENT CSLock lock(m_critsec);
	void set_attached_hwnd(const HWND h) { ENT _attached_hwnd = h; }
	#undef ENT
private:
	// private variables - use public accessors and public mutators to address these
	HWND		_attached_hwnd;	 // Table that we are attached to
	bool		TablemapsInScraperFolderAlreadyParsed;
	int			NumberOfTableMapsLoaded;
	CCritSec	m_critsec;
private:
	// Mutex used for cross-instance autoconnector coordination
	CMutex		*_autoconnector_mutex;
} *p_autoconnector;

#endif INC_CAUTOCONNECTOR_H