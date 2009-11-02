#ifndef INC_CAUTOCONNECTOR_H
#define INC_CAUTOCONNECTOR_H

#include "..\CCritSec\CCritSec.h"
#include "..\CTablemap\CTablemap.h"


typedef struct
{
	CString			FilePath;
	unsigned int	ClientSizeX, ClientSizeY;
	unsigned int	ClientSizeMinX, ClientSizeMinY;
	unsigned int	ClientSizeMaxX, ClientSizeMaxY;
	CString			TitleText;
	CString			TitleText_0_9[10];
	CString			NegativeTitleText;
	CString			NegativeTitleText_0_9[10];
	STablemapRegion	TablePoint;
} t_TablemapConnectionData;


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
	#define ENT CSLock lock(m_critsec);
	void set_attached_hwnd(const HWND h) { ENT _attached_hwnd = h; }
	#undef ENT
private:
	void ParseAllOpenScrapeOrWinScrapeTableMapsToLoadConnectionData(CString TableMapWildcard);
	void ParseAllTableMapsToLoadConnectionData();
	void ExtractConnectionDataFromCurrentTablemap(SWholeMap *map);
private:
	int SelectTableMapAndWindow(int Choices);
	int SelectTableMapAndWindowManually(int Choices);
	int SelectTableMapAndWindowAutomatically(int Choices);
private:
	void Check_TM_Against_All_Windows(SWholeMap smap, HWND targetHWnd);
private:
	// private variables - use public accessors and public mutators to address these
	HWND		_attached_hwnd;	 // Table that we are attached to
	bool		TablemapsInScraperFolderAlreadyParsed;
	int			NumberOfTableMapsLoaded;
	CCritSec	m_critsec;
} *p_autoconnector;

#endif INC_CAUTOCONNECTOR_H