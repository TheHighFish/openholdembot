#include "stdafx.h"

#include <afxwin.h>

#include "CAutoConnector.h"
#include "CAutoplayer.h"
#include "CFormula.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSharedMem.h"
#include "CSymbols.h"
#include "..\CTablemap\CTablemap.h"
#include "..\CTransform\CTransform.h"
#include "DialogScraperOutput.h"
#include "DialogSelectTable.h"
#include "MainFrm.h"
#include "OpenHoldem.h"


const int k_MaxNumberOfTableMaps = 25;


typedef struct
{
	CString			FilePath;
	CString			SiteName;
	unsigned int	ClientSizeX, ClientSizeY;
	unsigned int	ClientSizeMinX, ClientSizeMinY;
	unsigned int	ClientSizeMaxX, ClientSizeMaxY;
	CString			TitleText;
	CString			TitleText_0_9[10];
	CString			NegativeTitleText;
	CString			NegativeTitleText_0_9[10];
} t_TablemapConnectionData;


t_TablemapConnectionData			TablemapConnectionData[k_MaxNumberOfTableMaps];
CAutoConnector						*p_autoconnector = NULL;
CArray <STableList, STableList>		g_tlist; 


void CTableMapToSWholeMap(CTablemap *cmap, SWholeMap *smap)
{
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] CTableMapToSWholeMap: %s\n", p_tablemap->filepath());
	smap->z$ = p_tablemap->z$();
	smap->s$ = p_tablemap->s$();
	smap->r$ = p_tablemap->r$();
	for (int i=0; i<=3; i++)
	{
		smap->t$[i] = p_tablemap->t$(i);
		smap->p$[i] = p_tablemap->p$(i);
		smap->h$[i] = p_tablemap->h$(i);
	}
	smap->i$ = p_tablemap->i$();
	smap->filepath = p_tablemap->filepath();
	smap->sitename = p_tablemap->sitename();
}


CAutoConnector::CAutoConnector()
{
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] CAutoConnector()\n");

	CString MutexName = prefs.mutex_name() + "AutoConnector";
	_autoconnector_mutex = new CMutex(false, MutexName);

	p_sharedmem->MarkPokerWindowAsUnAttached();
	set_attached_hwnd(NULL);
	TablemapsInScraperFolderAlreadyParsed = false;
	NumberOfTableMapsLoaded = 0;

	// Parse all tablemaps once on startup.
	// We want to avoid heavy workload in the connect()-function.
	ParseAllTableMapsToLoadConnectionData();

	CheckForDuplicatedTablemaps(); 
}


CAutoConnector::~CAutoConnector()
{
	// Releasing the mutex in case we hold it.
	// If we don't hold it, Unlock() will "fail" silently.
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] ~CAutoConnector()\n");
	_autoconnector_mutex->Unlock();
	if (_autoconnector_mutex != NULL)
	{
		delete _autoconnector_mutex;
		_autoconnector_mutex = NULL;
	}
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] ~CAutoConnector()\n");
	p_sharedmem->MarkPokerWindowAsUnAttached();
	set_attached_hwnd(NULL);
	// Load all TMs once here in the constructor to reduce workload 
	// in the Connection-method, which is synchronized by a mutex.
	ParseAllTableMapsToLoadConnectionData();
}


bool CAutoConnector::IsConnected()
{
	// We do no longer check for IsWindow(_attached_hwnd) here,
	// as it might happen that the window disappears (gets closed),
	// OH disconnects and in the meantime the auto-connector-thread
	// tries to connect already again.
	// This leads to a dead-lock.
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=174&t=15158
	return (_attached_hwnd != NULL);
}


void CAutoConnector::ParseAllOpenScrapeOrWinScrapeTableMapsToLoadConnectionData(CString TableMapWildcard)
{
	CFileFind	hFile;
	SWholeMap	smap;
	int			line = 0;

	write_log(prefs.debug_autoconnector(), "[CAutoConnector] ParseAllOpenScrapeOrWinScrapeTableMapsToLoadConnectionData: %s\n", TableMapWildcard);
	CString	current_path = p_tablemap->filepath();
	BOOL bFound = hFile.FindFile(TableMapWildcard.GetString());
	while (bFound)
	{
		if (NumberOfTableMapsLoaded >= k_MaxNumberOfTableMaps)
		{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] CAutoConnector: Error: Too many tablemaps. The autoconnector can only handle 25 TMs.", "Error", 0);
			MessageBox(0, "To many tablemaps. The auto-connector can handle 25 at most.", "ERROR", 0);
			return;
		}
		bFound = hFile.FindNextFile();
		if (!hFile.IsDots() && !hFile.IsDirectory() && hFile.GetFilePath()!=current_path)
		{
			int ret = p_tablemap->LoadTablemap((char *) hFile.GetFilePath().GetString(), VER_OPENSCRAPE_2, false, &line, prefs.disable_msgbox());
			if (ret == SUCCESS)
			{
				CTableMapToSWholeMap(p_tablemap, &smap);
				ExtractConnectionDataFromCurrentTablemap(&smap);
				write_log(prefs.debug_autoconnector(), "[CAutoConnector] Number of TMs loaded: %d\n", NumberOfTableMapsLoaded);
			}
		}
	}
}


void CAutoConnector::ParseAllTableMapsToLoadConnectionData()
{
	CString TableMapWildcard;
	
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] ParseAllTableMapsToLoadConnectionData\n");
	TableMapWildcard.Format("%s\\scraper\\*.tm", _startup_path);
	ParseAllOpenScrapeOrWinScrapeTableMapsToLoadConnectionData(TableMapWildcard);	
	TableMapWildcard.Format("%s\\scraper\\*.ws", _startup_path);
	ParseAllOpenScrapeOrWinScrapeTableMapsToLoadConnectionData(TableMapWildcard);
	TablemapsInScraperFolderAlreadyParsed = true;
}


bool CAutoConnector::TablemapConnectionDataAlreadyStored(CString TablemapFilePath)
{
	for (int i=0; i<=NumberOfTableMapsLoaded; i++)
	{
		if (TablemapConnectionData[i].FilePath == TablemapFilePath)
		{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] TablemapConnectionDataAlreadyStored [%s] [true]\n", TablemapFilePath);
			return true;
		}
	}
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] TablemapConnectionDataAlreadyStored [%s] [false]\n", TablemapFilePath);
	return false;
}

void CAutoConnector::CheckForDuplicatedTablemaps()
{
	CString error_message = "";
	for (int i=0; i<NumberOfTableMapsLoaded; i++)
	{
		for (int j=i+1; j<NumberOfTableMapsLoaded; j++)
		{
			if (TablemapConnectionData[i].SiteName == TablemapConnectionData[j].SiteName)
			{
				write_log(prefs.debug_autoconnector(), "[CAutoConnector] TablemapConnectionDataDuplicated [%s] [true]\n", 
					TablemapConnectionData[i].SiteName);
				error_message.Format("It seems you have multiple versions of the same map in your scraper folder.\n\n"\
					"SITENAME = %s\n\n"\
					"This will cause problems as the autoconnector won't be able to decide which one to use.\n"\
					"Please remove the superfluous maps from the scraper folder.\n", 
					TablemapConnectionData[i].SiteName);
				MessageBox(0, (LPCTSTR) error_message, 
					"Warning! Duplicate SiteName", MB_OK|MB_ICONWARNING);
			}
		}
	}
}

void CAutoConnector::ExtractConnectionDataFromCurrentTablemap(SWholeMap *map)
{
	CString s;
	
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] ExtractConnectionDataFromCurrentTablemap(): %s\n", map->filepath);
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] NumberOfTableMapsLoaded: %d\n", NumberOfTableMapsLoaded);

	// Avoiding to store the data twice, e.g. when we load a known TM manually
	if (TablemapConnectionDataAlreadyStored(map->filepath))
	{
		write_log(prefs.debug_autoconnector(), "[CAutoConnector] ExtractConnectionDataFromCurrentTablemap(): already stored; early exit\n");
		return;
	}

	TablemapConnectionData[NumberOfTableMapsLoaded].FilePath = map->filepath;
	TablemapConnectionData[NumberOfTableMapsLoaded].SiteName = map->sitename;

	if (map->sitename == "")
	{
		CString error_message;
		error_message.Format("Tablemap contains no sitename.\n"
			"Sitenames are necessary to recognize duplicate TMs\n"
			"(and for other features like PokerTracker).\n\n",
			"%s", map->filepath);
		MessageBox(0, error_message, "Warning", 0);
	}
	
	// Extract client size information
	ZMapCI z_iter = map->z$->end();
	z_iter = map->z$->find("clientsize");
	if (z_iter != map->z$->end())
	{
		TablemapConnectionData[NumberOfTableMapsLoaded].ClientSizeX = z_iter->second.width;
		TablemapConnectionData[NumberOfTableMapsLoaded].ClientSizeY = z_iter->second.height;
	}
	else
	{
		TablemapConnectionData[NumberOfTableMapsLoaded].ClientSizeX = 0;
		TablemapConnectionData[NumberOfTableMapsLoaded].ClientSizeY = 0;
	}
	z_iter = map->z$->find("clientsizemin");
	if (z_iter != map->z$->end())
	{
		TablemapConnectionData[NumberOfTableMapsLoaded].ClientSizeMinX = z_iter->second.width;
		TablemapConnectionData[NumberOfTableMapsLoaded].ClientSizeMinY = z_iter->second.height;
	}
	else
	{
		TablemapConnectionData[NumberOfTableMapsLoaded].ClientSizeMinX = 0;
		TablemapConnectionData[NumberOfTableMapsLoaded].ClientSizeMinY = 0;
	}
	z_iter = map->z$->find("clientsizemax");
	if (z_iter != map->z$->end())
	{
		TablemapConnectionData[NumberOfTableMapsLoaded].ClientSizeMaxX = z_iter->second.width;
		TablemapConnectionData[NumberOfTableMapsLoaded].ClientSizeMaxY = z_iter->second.height;
	}
	else
	{
		TablemapConnectionData[NumberOfTableMapsLoaded].ClientSizeMaxX = 0;
		TablemapConnectionData[NumberOfTableMapsLoaded].ClientSizeMaxY = 0;
	}

	// Extract title text information
	SMapCI s_iter = map->s$->end();
	s_iter = map->s$->find("titletext");
	if (s_iter!=map->s$->end())
		TablemapConnectionData[NumberOfTableMapsLoaded].TitleText =	s_iter->second.text;
	else
		TablemapConnectionData[NumberOfTableMapsLoaded].TitleText = "";
			
	for (int i=0; i<=9; i++)
	{
		s.Format("titletext%d", i);
		s_iter = map->s$->find(s.GetString());
		if (s_iter!=map->s$->end())
			TablemapConnectionData[NumberOfTableMapsLoaded].TitleText_0_9[i] =	s_iter->second.text;
		else
			TablemapConnectionData[NumberOfTableMapsLoaded].TitleText_0_9[i] = "";
	}

	// Extract negative title texs
	s_iter = map->s$->find("!titletext");
	if (s_iter!=map->s$->end())
		TablemapConnectionData[NumberOfTableMapsLoaded].NegativeTitleText = s_iter->second.text;
	else
		TablemapConnectionData[NumberOfTableMapsLoaded].NegativeTitleText = "";

	for (int i=0; i<=9; i++)
	{
		s.Format("!titletext%d", i);
		s_iter = map->s$->find(s.GetString());
		if (s_iter!=map->s$->end())
			TablemapConnectionData[NumberOfTableMapsLoaded].NegativeTitleText_0_9[i] = s_iter->second.text;
		else
			TablemapConnectionData[NumberOfTableMapsLoaded].NegativeTitleText_0_9[i] = "";
	}
	NumberOfTableMapsLoaded++;
}


void CAutoConnector::Check_TM_Against_All_Windows_Or_TargetHWND(int TablemapIndex, HWND targetHWnd)
{
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Check_TM_Against_All_Windows(..)\n");

	if (targetHWnd == NULL)
		EnumWindows(EnumProcTopLevelWindowList, (LPARAM) TablemapIndex);
	else
		EnumProcTopLevelWindowList(targetHWnd, (LPARAM) TablemapIndex);
}


// This function has to be global and can't be part of the class,
// as it has to be called by the callback-function 
// BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam) 
bool Check_TM_Against_Single_Window(int MapIndex, HWND h, RECT r, CString title) 
{
	bool			good_pos_title = false, good_neg_title = false, good_table_points = false;
	int				width = 0, height = 0, x = 0, y = 0;
	HDC				hdcScreen = NULL, hdcCompatible = NULL;
	HBITMAP			hbmScreen = NULL, hOldScreenBitmap = NULL;
	BYTE			*pBits = NULL, alpha = 0, red = 0, green = 0, blue = 0;
	CTransform		trans;
	CString			s;

	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Check_TM_Against_Single_Window(..)\n");
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Checking map nr. %d\n", MapIndex);
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Window title: %s\n", title);
	
	// Check for exact match on client size
	if (!((r.right == TablemapConnectionData[MapIndex].ClientSizeX)
		&& (r.bottom == TablemapConnectionData[MapIndex].ClientSizeY)))
	{
		// Exact size didn't match.
		// So check for client size that falls within min/max
		if (!((TablemapConnectionData[MapIndex].ClientSizeMinX != 0) 
			&& (TablemapConnectionData[MapIndex].ClientSizeMinY != 0) 
			&& (TablemapConnectionData[MapIndex].ClientSizeMaxX != 0) 
			&& (TablemapConnectionData[MapIndex].ClientSizeMaxY != 0) 
			&& (r.right  >= (int) TablemapConnectionData[MapIndex].ClientSizeMinX)
			&& (r.right  <= (int) TablemapConnectionData[MapIndex].ClientSizeMaxX)
			&& (r.bottom >= (int) TablemapConnectionData[MapIndex].ClientSizeMinY)
			&& (r.bottom <= (int) TablemapConnectionData[MapIndex].ClientSizeMaxY)))
		{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] No good size.\n");
			return false;
		}
	}

	// Check for match positive title text matches
	good_pos_title = false;
	if ((TablemapConnectionData[MapIndex].TitleText != "")
		&& title.Find(TablemapConnectionData[MapIndex].TitleText)!=-1)
	{
		good_pos_title = true;
	}
	else
	{
		// titletext din't match
		// Check for titletext0..titletext9
		for (int i=0; i<=9; i++)
		{
			if ((TablemapConnectionData[MapIndex].TitleText_0_9[i] != "")
				&& (title.Find(TablemapConnectionData[MapIndex].TitleText_0_9[i])!=-1))
			{
				good_pos_title = true;
				break;
			}
		}
	}
	if (!good_pos_title)
	{
		write_log(prefs.debug_autoconnector(), "[CAutoConnector] No good title.\n");
		return false;
	}

	// Check for no negative title text matches
	good_neg_title = false;
	if ((TablemapConnectionData[MapIndex].NegativeTitleText != "")
		&& (title.Find(TablemapConnectionData[MapIndex].NegativeTitleText)!=-1))
	{
		good_neg_title = true;
	}
	else
	{
		for (int i=0; i<=9; i++)
		{
			if ((TablemapConnectionData[MapIndex].NegativeTitleText_0_9[i] != "")
				&&title.Find(TablemapConnectionData[MapIndex].NegativeTitleText_0_9[i])!=-1)
			{
				good_neg_title = true;
				break;
			}
		}
	}
	if (good_neg_title)
	{
		write_log(prefs.debug_autoconnector(), "[CAutoConnector] Negative title.\n"); 
		return false;
	}

	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Window ia a match\n");
	return true;
}


CString Wanted = "OpenHoldem Poker - Fixed Limit - blinds 5/10 - ante 0";

BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam) 
{
	CString				title = "", winclass = "";
	char				text[512] = {0};
	RECT				crect = {0};
	STableList			tablelisthold;
	int					TablemapIndex = (int)(lparam);

	write_log(prefs.debug_autoconnector(), "[CAutoConnector] EnumProcTopLevelWindowList(..)\n");
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Tablemap nr. %d\n", TablemapIndex);
	// If this is not a top level window, then return
	if (GetParent(hwnd) != NULL)
		return true;

	// If this window is not visible, then return
	if (!IsWindowVisible(hwnd))
		return true;

	// If there is no caption on this window, then return
	GetWindowText(hwnd, text, sizeof(text));
	if (strlen(text)==0)
		return true;
 
	title = text;

	// Found a candidate window, get client area rect
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] EnumProcTopLevelWindowList(..) found a window candidate...\n");
	GetClientRect(hwnd, &crect);

	// See if it matches the currently loaded table map
	if (Check_TM_Against_Single_Window(TablemapIndex, hwnd, crect, title))
	{
		// Filter out served tables already here,
		// otherwise the other list used in the dialog
		// to select windows manually will cause us lots of headaches,
		// as the lists will be of different size 
		// and the indexes will not match.
		if (p_sharedmem->PokerWindowAttached(hwnd))
		{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] Window candidate already served: [%d]\n", hwnd);
		}
		else
		{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] Adding window candidate to the list: [%d]\n", hwnd);
			tablelisthold.hwnd = hwnd;
			tablelisthold.title = title;
			tablelisthold.path = TablemapConnectionData[TablemapIndex].FilePath;
			tablelisthold.crect.left = crect.left;
			tablelisthold.crect.top = crect.top;
			tablelisthold.crect.right = crect.right;
			tablelisthold.crect.bottom = crect.bottom;
			g_tlist.Add(tablelisthold);
		}
	}

	return true;  // keep processing through entire list of windows
}


bool CAutoConnector::Connect(HWND targetHWnd)
{
	int					N = 0, line = 0, ret = 0;
	char				title[512] = {0};
	int					SelectedItem = -1;
	SWholeMap			smap;
	CString				current_path = "";
	BOOL				bFound = false;
	CFileFind			hFile;

	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Connect(..)\n");

	ASSERT(_autoconnector_mutex->m_hObject != NULL); 
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Locking autoconnector-mutex\n");
	if (!_autoconnector_mutex->Lock(500))
	{
		write_log(prefs.debug_autoconnector(), "[CAutoConnector] Could not grab mutex; early exit\n");
		return false; 
	}

	// Adding connection data for current TM, in case we loaded a TM manually.
	if (p_tablemap->valid())
	{
		if (!TablemapConnectionDataAlreadyStored(p_tablemap->filepath()))
		{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] Adding connection data for current map (manually loaded)\n");
			CTableMapToSWholeMap(p_tablemap, &smap);
			ExtractConnectionDataFromCurrentTablemap(&smap);
		}
	}

	if (!TablemapsInScraperFolderAlreadyParsed)
	{
		ParseAllTableMapsToLoadConnectionData();
	}

	// Clear global list for holding table candidates
	g_tlist.RemoveAll();
	
	for (int TablemapIndex=0; TablemapIndex<NumberOfTableMapsLoaded; TablemapIndex++)
	{
		write_log(prefs.debug_autoconnector(), "[CAutoConnector] Going to check TM nr. %d out of %d\n", TablemapIndex, NumberOfTableMapsLoaded);
		Check_TM_Against_All_Windows_Or_TargetHWND(TablemapIndex, targetHWnd);
	}
	
	// Put global candidate table list in table select dialog variables
	N = (int) g_tlist.GetSize();
	if (N == 0) 
	{
		p_sharedmem->RememberTimeOfLastFailedAttemptToConnect();

		int cySize = GetSystemMetrics(SM_CYSIZE);
		int cyMenuSize = GetSystemMetrics(SM_CYMENU);

		if (!prefs.disable_msgbox() && prefs.autoconnector_when_to_connect() != k_AutoConnector_Connect_Permanent)
		{
			if (cySize != 18 && cyMenuSize != 19)
				MessageBox(0, "Cannot find table\n\n"
						   "It appears that your settings are not configured according to OpenHoldem specifications.\n"
						   "You must ensure that XP themes are not used (Use Windows Classic style) and\n"
						   "font size is set to normal.\n\n"
						   "For more info, read the manual and visit the user forums", "Cannot find table", MB_OK);
			else
				MessageBox(0, "No valid tables found", "Cannot find table", MB_OK);
		}
	}
	else 
	{
		SelectedItem = SelectTableMapAndWindow(N);
		if (SelectedItem != -1)
		{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] Window [%d] selected\n", g_tlist[SelectedItem].hwnd);
			p_sharedmem->MarkPokerWindowAsAttached(g_tlist[SelectedItem].hwnd);
		}
		else
		{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] Attempt to connect did fail\n");
			p_sharedmem->RememberTimeOfLastFailedAttemptToConnect();
		}
		if (SelectedItem != -1)
		{
			// Load correct tablemap, and save hwnd/rect/numchairs of table that we are "attached" to
			set_attached_hwnd(g_tlist[SelectedItem].hwnd);
			CString loaded_version;
			p_tablemap->LoadTablemap((char *) g_tlist[SelectedItem].path.GetString(), VER_OPENSCRAPE_2, false, &line, 
									 prefs.disable_msgbox(), &loaded_version);

			if ( (loaded_version == VER_OPENSCRAPE_1 || loaded_version == VER_OPENHOLDEM_2) && !prefs.disable_msgbox())
			{
				MessageBox(0, "You have loaded a version 1 table map for this poker table.\n\n"\
						   "Version 2.0.0 and higher of OpenHoldem use a new format (version 2).  This\n"\
						   "table map has been loaded, but it is highly unlikely to work correctly until\n"\
						   "it has been opened in OpenScrape version 2.0.0 or higher, and adjustments\n"\
						   "have been made to autoplayer settings and region sizes.\n\n"\
						   "Please do not use this table map prior to updating it to version 2 in\n"\
						   "OpenScrape or you run the very serious risk of costly mis-scrapes.",
						   "Table map load warning", MB_OK | MB_ICONEXCLAMATION);
			}

			// Create bitmaps
			p_scraper->CreateBitmaps();

			// Clear scraper fields
			p_scraper->ClearScrapeAreas();

			// Reset symbols
			p_symbols->ResetSymbolsFirstTime();

			// Reset "ScraperOutput" dialog, if it is live
			if (m_ScraperOutputDlg) 
			{
				m_ScraperOutputDlg->AddListboxItems();
				m_ScraperOutputDlg->UpdateDisplay();
			}

			// scraper.dll - failure in load is NOT fatal
			theApp.UnloadScraperDLL();
			CString filename = p_tablemap->scraperdll();
			if (!filename.IsEmpty()) {
				SetCurrentDirectory(_startup_path);
				theApp._scraper_dll = LoadLibrary(filename);
			}

			if (theApp._scraper_dll==NULL)
			{
				if (!filename.IsEmpty() && !prefs.disable_msgbox())		
				{
					CString		t = "";
					t.Format("Unable to load %s\n\nError: %d", filename, GetLastError());
					MessageBox(0, t, "Error", 0);
				}
			}
			else
			{
				theApp._dll_scraper_process_message = (scraper_process_message_t) GetProcAddress(theApp._scraper_dll, "ProcessMessage");
				theApp._dll_scraper_override = (scraper_override_t) GetProcAddress(theApp._scraper_dll, "OverrideScraper");

				if (theApp._dll_scraper_process_message==NULL || theApp._dll_scraper_override==NULL)
				{
					if (!prefs.disable_msgbox())		
					{
						MessageBox(0, "Unable to find all symbols in scraper.dll", "Error", 0);
					}

					theApp.UnloadScraperDLL();
				}
				else
				{
					write_log(prefs.debug_autoconnector(), "[CAutoConnector] scraper.dll (%s) loaded, ProcessMessage and OverrideScraper found.\n", filename);
				}
			}

			// scraperpreprocessor.dll - failure in load is NOT fatal
			theApp.Unload_ScraperPreprocessor_DLL();
			filename = p_tablemap->scraperpreprocessor_dll();
			if (!filename.IsEmpty()) {
				SetCurrentDirectory(_startup_path);
				theApp._scraperpreprocessor_dll = LoadLibrary(filename);
			}

			if (theApp._scraperpreprocessor_dll==NULL)
			{
				if (!filename.IsEmpty() && !prefs.disable_msgbox())		
				{
					CString		t = "";
					t.Format("Unable to load %s\n\nError: %d", filename, GetLastError());
					MessageBox(0, t, "OpenHoldem scraperpre.dll WARNING", MB_OK | MB_TOPMOST);
				}
			}
			else
			{
				theApp._dll_scraperpreprocessor_process_message = (scraperpreprocessor_process_message_t) GetProcAddress(theApp._scraperpreprocessor_dll, "ProcessMessage");

				if (theApp._dll_scraperpreprocessor_process_message==NULL)
				{
					if (!prefs.disable_msgbox())		
					{
						CString		t = "";
						t.Format("Unable to find symbols in scraperpre.dll");
						MessageBox(0, t, "OpenHoldem scraperpre.dll ERROR", MB_OK | MB_TOPMOST);
					}

					theApp.Unload_ScraperPreprocessor_DLL();
				}
				else
				{
					write_log(prefs.debug_autoconnector(), "[CAutoConnector] scraperpreprocessor.dll loaded, ProcessMessage found.\n");
				}
			}


			CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
			pMyMainWnd->DisableButtonsOnConnect();

			// Make sure autoplayer is off
			p_autoplayer->set_autoplayer_engaged(false);

			// mark symbol result cache as stale
			p_formula->MarkCacheStale();

			// reset iterator vars
			iter_vars.ResetVars();

			// Send "connect" and HWND to scraper DLL, if loaded
			if (theApp._dll_scraper_process_message)
				(theApp._dll_scraper_process_message) ("connect", &_attached_hwnd);

			// Send "connect" and HWND to scraper pre DLL, if loaded
			if (theApp._dll_scraperpreprocessor_process_message)
				(theApp._dll_scraperpreprocessor_process_message) ("connect", &_attached_hwnd);

			// start heartbeat thread
			if (p_heartbeat_thread)
				delete p_heartbeat_thread;

			p_heartbeat_thread = new CHeartbeatThread;

			// Start timer that checks for continued existence of attached HWND
			pMyMainWnd->StartTimer();

			// Reset display
			pMyMainWnd->ResetDisplay();

			// Start logging, in case the log-level got changed.
			start_log();
			
			// log OH title bar text and table reset
			::GetWindowText(_attached_hwnd, title, 512);

			CString site = "";
			SMapCI site_i = p_tablemap->s$()->find("sitename");
			if (site_i != p_tablemap->s$()->end())
				site = site_i->second.text;

			CString formula = p_formula->formula_name();
			write_log(1, "\n*************************************************************\n"
					  "TABLE RESET %s - %s(%s)\n"
					  "*************************************************************\n",
					  formula.GetString(), site.GetString(), title);

			CString sForceChair = p_tablemap->forcechair();
			if (!sForceChair.IsEmpty()) {
				int iForceChair = atoi(sForceChair);
				p_symbols->set_sym_chair(iForceChair);
				p_symbols->set_sym_userchair(iForceChair);
				p_symbols->set_user_chair_confirmed(true); 
			}
		}
	}
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Unlocking autoconnector-mutex\n");
	_autoconnector_mutex->Unlock();
	return (SelectedItem != -1);
}


void CAutoConnector::Disconnect()
{
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Disconnect()\n");

	// Wait for mutex - "forever" if necessary, as we have to clean up.
	ASSERT(_autoconnector_mutex->m_hObject != NULL); 
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Locking autoconnector-mutex\n");
	_autoconnector_mutex->Lock(INFINITE);

	// stop threads
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Stopping heartbeat-thread\n");
	if (p_heartbeat_thread)
	{
		delete p_heartbeat_thread;
		p_heartbeat_thread = NULL;
	}

	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Stopping PokerTracker thread\n");
	if (p_pokertracker_thread)
		p_pokertracker_thread->StopThread();

	// Make sure autoplayer is off
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Stopping autoplayer\n");
	p_autoplayer->set_autoplayer_engaged(false);

	// Send "disconnect" to scraper DLL, if loaded
	if (theApp._dll_scraper_process_message)
			(theApp._dll_scraper_process_message) ("disconnect", NULL);

	theApp.UnloadScraperDLL();

	// Send "disconnect" to scraperpre DLL, if loaded
	if (theApp._dll_scraperpreprocessor_process_message)
			(theApp._dll_scraperpreprocessor_process_message) ("disconnect", NULL);

	theApp.Unload_ScraperPreprocessor_DLL();

	// Stop timer that checks for valid hwnd, then unattach OH.
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	pMyMainWnd->KillTimer();
	pMyMainWnd->UnattachOHFromPokerWindow();
	pMyMainWnd->EnableButtonsOnDisconnect();

	// Mark table as not attached
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Marking table as not attached\n");
	p_sharedmem->MarkPokerWindowAsUnAttached();

	// Release mutex as soon as possible, after critical work is done
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Unlocking autoconnector-mutex\n");
	_autoconnector_mutex->Unlock();

	// Clear "attached" info
	// Do this AFTER unlocking the mutex, 
	// otherwise the auto-connector-thread might try to connect again,
	// leading to a dead-lock
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=174&t=15158
	set_attached_hwnd(NULL);

	// Delete bitmaps
	p_scraper->DeleteBitmaps();

	// Clear scraper fields
	p_scraper->ClearScrapeAreas();

	// Reset symbols
	p_symbols->ResetSymbolsFirstTime();
	p_symbols->set_sym_betround(k_betround_preflop);

	// Change window title
	pMyMainWnd->UpdateWindowTitle();

	// Reset Display 
	pMyMainWnd->ResetDisplay();

	// Reset "ScraperOutput" dialog, if it is live
	if (m_ScraperOutputDlg)
	{
		m_ScraperOutputDlg->AddListboxItems();
		m_ScraperOutputDlg->UpdateDisplay();
	}

	// log OH title bar text and table reset
	CString sitename = (p_tablemap->s$()->find("sitename") != p_tablemap->s$()->end() ? p_tablemap->s$()->find("sitename")->second.text.GetString() : "");
	write_log(1, "%s - %s(NOT ATTACHED)\n", p_formula->formula_name().GetString(), sitename);

	write_log(1, "TABLE RESET\n*************************************************************\n");

	// Stop logging
	stop_log();

	// Finally: reset 
	_attached_hwnd = NULL;
	
	// Close OH, when table disappears and leaving enabled in preferences.
	if (prefs.autoconnector_close_when_table_disappears())
	{
		PostQuitMessage(0);
	}
}


int CAutoConnector::SelectTableMapAndWindow(int Choices)
{
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] SelectTableMapAndWindow(..)\n");
	if (prefs.autoconnector_connection_method() == k_AutoConnector_Connect_Manually)
	{
		return SelectTableMapAndWindowManually(Choices);
	}
	else // k_AutoConnector_Connect_Automatically
	{
		return SelectTableMapAndWindowAutomatically(Choices);
	}
}


int CAutoConnector::SelectTableMapAndWindowManually(int Choices)
{
	STableList			tablelisthold;
	CDlgSelectTable		cstd;
	int					result = 0;

	write_log(prefs.debug_autoconnector(), "[CAutoConnector] SelectTableMapAndWindowManually()\n");
	for (int i=0; i<Choices; i++) 
	{
		// Build list of tables, that do not yet get served.
		assert(!p_sharedmem->PokerWindowAttached(g_tlist[i].hwnd));
		tablelisthold.hwnd = g_tlist[i].hwnd;
		tablelisthold.title = g_tlist[i].title;
		tablelisthold.path = g_tlist[i].path;
		tablelisthold.crect.left = g_tlist[i].crect.left;
		tablelisthold.crect.top = g_tlist[i].crect.top;
		tablelisthold.crect.right = g_tlist[i].crect.right;
		tablelisthold.crect.bottom = g_tlist[i].crect.bottom;
		cstd.tlist.Add(tablelisthold);
		write_log(prefs.debug_autoconnector(), "[CAutoConnector] Adding window to the list [%s], [%i]\n",
		g_tlist[i].title, g_tlist[i].hwnd);
	}
	int NumberOfNonAttachedTables = cstd.tlist.GetSize(); 
	if (NumberOfNonAttachedTables == 0)
	{
		write_log(prefs.debug_autoconnector(), "[CAutoConnector] SelectTableMapAndWindowManually(): all windows served; exiting\n");
		return -1;
	}
	else if (NumberOfNonAttachedTables == 1)
	{
		// Exactly one window, not yet served.
		// First (and only) item selected
		write_log(prefs.debug_autoconnector(), "[CAutoConnector] SelectTableMapAndWindowManually(): exactly one free table; choosing that one\n");
		// We have to return the index in the original list,
		// not the index (0) in the temporary list of the dialog.
		const int k_original_index_for_list_with_single_non_served_window = 0;
		return k_original_index_for_list_with_single_non_served_window;
	}
	// Display table select dialog
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] SelectTableMapAndWindowManually(): multiple free tables; going to display a dialog\n");
	result = cstd.DoModal();
	if (result == IDOK) 
	{
		return cstd.selected_item;
	}
	else
	{
		return -1;
	}
}


int CAutoConnector::SelectTableMapAndWindowAutomatically(int Choices)
{
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] SelectTableMapAndWindowAutomatically(..)\n");
	for (int i=0; i<Choices; i++) 
	{
		if (!p_sharedmem->PokerWindowAttached(g_tlist[i].hwnd))
		{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] Chosen (table, TM)-pair in list: %d\n", i);
			return i;
		}
	}
	// No appropriate table found
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] No appropriate table found.\n");
	return -1;
}

double CAutoConnector::TimeSinceLastFailedAttemptToConnect()
{
	time_t LastFailedAttemptToConnect = p_sharedmem->GetTimeOfLastFailedAttemptToConnect(); //???
	time_t CurrentTime;
	time(&CurrentTime);
	double _TimeSinceLastFailedAttemptToConnect = difftime(CurrentTime, LastFailedAttemptToConnect);
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] TimeSinceLastFailedAttemptToConnect %f\n", _TimeSinceLastFailedAttemptToConnect);
	return _TimeSinceLastFailedAttemptToConnect;
}