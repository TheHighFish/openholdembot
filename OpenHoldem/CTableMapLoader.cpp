#include "StdAfx.h"
#include "CTableMaploader.h"

#include "..\CTablemap\CTablemapAccess.h"
#include "..\CTransform\CTransform.h"
#include "CFileSystemMonitor.h"
#include "CPreferences.h"
#include "MagicNumbers.h"
#include "OH_MessageBox.h"
#include "OpenHoldem.h"

CTableMapLoader *p_tablemap_loader = NULL;

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


t_TablemapConnectionData			TablemapConnectionData[k_max_nmber_of_tablemaps];
CArray <STableList, STableList>		g_tlist; 


CTableMapLoader::CTableMapLoader()
{
	_number_of_tablemaps_loaded = 0;
	tablemaps_in_scraper_folder_already_parsed = false;

	// Parse all tablemaps once on startup.
	// We want to avoid heavy workload in the connect()-function.
	ParseAllTableMapsToLoadConnectionData();
	CheckForDuplicatedTablemaps(); 
}

CTableMapLoader::~CTableMapLoader()
{}


void CTableMapLoader::CTableMapToSWholeMap(CTablemap *cmap, SWholeMap *smap)
{
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] CTableMapToSWholeMap: %s\n", p_tablemap->filepath());
	smap->z$ = p_tablemap->z$();
	smap->s$ = p_tablemap->s$();
	smap->r$ = p_tablemap->r$();

	for (int i = 0; i < k_max_number_of_font_groups_in_tablemap; i++)
	{
		smap->t$[i] = p_tablemap->t$(i);
	}
	for (int i = 0; i < k_max_number_of_hash_groups_in_tablemap; i++)
	{
		smap->p$[i] = p_tablemap->p$(i);
	}
	for (int i = 0; i < k_max_number_of_hash_groups_in_tablemap; i++)
	{
		smap->h$[i] = p_tablemap->h$(i);
	}
	smap->i$ = p_tablemap->i$();
	smap->filepath = p_tablemap->filepath();
	smap->sitename = p_tablemap->sitename();
}


void CTableMapLoader::ParseAllTableMapsToLoadConnectionData(CString TableMapWildcard)
{
	CFileFind	hFile;
	SWholeMap	smap;
	int			line = 0;

	write_log(prefs.debug_autoconnector(), "[CAutoConnector] ParseAllTableMapsToLoadConnectionData: %s\n", TableMapWildcard);
	CString	current_path = p_tablemap->filepath();
	BOOL bFound = hFile.FindFile(TableMapWildcard.GetString());
	while (bFound)
	{
		if (_number_of_tablemaps_loaded >= k_max_nmber_of_tablemaps)
		{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] CAutoConnector: Error: Too many tablemaps. The autoconnector can only handle 25 TMs.", "Error", 0);
			OH_MessageBox("To many tablemaps.\n"
				"The auto-connector can handle 25 at most.", "ERROR", 0);
			return;
		}
		bFound = hFile.FindNextFile();
		if (!hFile.IsDots() && !hFile.IsDirectory() && hFile.GetFilePath()!=current_path)
		{
			int ret = p_tablemap->LoadTablemap((char *) hFile.GetFilePath().GetString(), 
				VER_OPENSCRAPE_2, &line);
			if (ret == SUCCESS)
			{
				CTableMapToSWholeMap(p_tablemap, &smap);
				ExtractConnectionDataFromCurrentTablemap(&smap);
				write_log(prefs.debug_autoconnector(), "[CAutoConnector] Number of TMs loaded: %d\n", _number_of_tablemaps_loaded);
			}
		}
	}
}


void CTableMapLoader::ParseAllTableMapsToLoadConnectionData()
{
	CString TableMapWildcard;
	
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] ParseAllTableMapsToLoadConnectionData\n");
	TableMapWildcard.Format("%s\\scraper\\*.tm", _startup_path);
	ParseAllTableMapsToLoadConnectionData(TableMapWildcard);	
	tablemaps_in_scraper_folder_already_parsed = true;
}


bool CTableMapLoader::TablemapConnectionDataAlreadyStored(CString TablemapFilePath)
{
	for (int i=0; i<=_number_of_tablemaps_loaded; i++)
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


void CTableMapLoader::CheckForDuplicatedTablemaps()
{
	CString error_message = "";
	for (int i=0; i<_number_of_tablemaps_loaded; i++)
	{
		for (int j=i+1; j<_number_of_tablemaps_loaded; j++)
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
				OH_MessageBox((LPCTSTR) error_message, 
					"Warning! Duplicate SiteName", MB_OK|MB_ICONWARNING);
			}
		}
	}
}


void CTableMapLoader::ExtractConnectionDataFromCurrentTablemap(SWholeMap *map)
{
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] ExtractConnectionDataFromCurrentTablemap(): %s\n", map->filepath);
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] number_of_tablemaps_loaded: %d\n", _number_of_tablemaps_loaded);

	// Avoiding to store the data twice, e.g. when we load a known TM manually
	if (TablemapConnectionDataAlreadyStored(map->filepath))
	{
		write_log(prefs.debug_autoconnector(), "[CAutoConnector] ExtractConnectionDataFromCurrentTablemap(): already stored; early exit\n");
		return;
	}

	TablemapConnectionData[_number_of_tablemaps_loaded].FilePath = map->filepath;
	TablemapConnectionData[_number_of_tablemaps_loaded].SiteName = map->sitename;

	if (map->sitename == "")
	{
		CString error_message;
		error_message.Format("Tablemap contains no sitename.\n"
			"Sitenames are necessary to recognize duplicate TMs\n"
			"(and for other features like PokerTracker).\n\n",
			"%s", map->filepath);
		OH_MessageBox(error_message, "Warning", 0);
	}
	
	// Get clientsize info through TM-access-class
	p_tablemap_access->SetClientSize("clientsize", TablemapConnectionData[_number_of_tablemaps_loaded].ClientSizeX, TablemapConnectionData[_number_of_tablemaps_loaded].ClientSizeY);
	p_tablemap_access->SetClientSize("clientsizemin", TablemapConnectionData[_number_of_tablemaps_loaded].ClientSizeMinX, TablemapConnectionData[_number_of_tablemaps_loaded].ClientSizeMinY);
	p_tablemap_access->SetClientSize("clientsizemax", TablemapConnectionData[_number_of_tablemaps_loaded].ClientSizeMaxX, TablemapConnectionData[_number_of_tablemaps_loaded].ClientSizeMaxY);

	// Extract title text information
	p_tablemap_access->SetTitleText("titletext", TablemapConnectionData[_number_of_tablemaps_loaded].TitleText);
	// Extract negative title texs
	p_tablemap_access->SetTitleText("!titletext", TablemapConnectionData[_number_of_tablemaps_loaded].NegativeTitleText);
		
	CString s = "";
	for (int i=0; i<k_max_number_of_titletexts; i++)
	{
		s.Format("titletext%d", i);
		p_tablemap_access->SetTitleText(s, TablemapConnectionData[_number_of_tablemaps_loaded].TitleText_0_9[i]);

		s.Format("!titletext%d", i);
		p_tablemap_access->SetTitleText(s, TablemapConnectionData[_number_of_tablemaps_loaded].NegativeTitleText_0_9[i]);		
	}

	_number_of_tablemaps_loaded++;
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
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] No good size: Expected (%dpx, %dpx), Got (%dpx, %dpx)\n",
				TablemapConnectionData[MapIndex].ClientSizeX,
				TablemapConnectionData[MapIndex].ClientSizeY,
				r.right,
				r.bottom);
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
		for (int i=0; i<k_max_number_of_titletexts; i++)
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

void CTableMapLoader::ReloadAllTablemapsIfChanged()
{
	/* !!! 
	if (!TimeToReloadTableMaps())
	{
		return;
	}*/
	if (p_filesystem_monitor->AnyChanges())
	{
		ParseAllTableMapsToLoadConnectionData();
	}
}

