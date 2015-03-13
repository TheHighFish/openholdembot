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

#include "StdAfx.h"
#include "CTableMaploader.h"

#include "CAutoConnector.h"
#include "CFilenames.h"
#include "CFileSystemMonitor.h"
#include "CPreferences.h"
#include "CTablemapCompletenessChecker.h"
#include "MagicNumbers.h"
#include "..\CTablemap\CTablemapAccess.h"
#include "..\CTransform\CTransform.h"
#include "OH_MessageBox.h"
#include "OpenHoldem.h"

CTableMapLoader *p_tablemap_loader = NULL;

typedef struct {
	CString	FilePath;
	CString	SiteName;
	int	    ClientSizeX, ClientSizeY;
	int	    ClientSizeMinX, ClientSizeMinY;
	int	    ClientSizeMaxX, ClientSizeMaxY;
	CString	TitleText;
	CString	TitleText_0_9[10];
	CString	NegativeTitleText;
	CString	NegativeTitleText_0_9[10];
	STablemapRegion	TablePoint[10];
	int			TablePointCount;

} t_tablemap_connection_data;


t_tablemap_connection_data			tablemap_connection_data[k_max_nmber_of_tablemaps];


CTableMapLoader::CTableMapLoader() {
	write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] CTableMapLoader()\n");
	tablemaps_in_scraper_folder_already_parsed = false;

	// Parse all tablemaps once on startup.
	// We want to avoid heavy workload in the connect()-function.
	ParseAllTableMapsToLoadConnectionData();
	CheckForDuplicatedTablemaps(); 
}

CTableMapLoader::~CTableMapLoader() {
}

void CTableMapLoader::ParseAllTableMapsToLoadConnectionData(CString TableMapWildcard) {
	CFileFind	hFile;
	
	write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] ParseAllTableMapsToLoadConnectionData: %s\n", TableMapWildcard);
	_number_of_tablemaps_loaded = 0;
	CString	current_path = p_tablemap->filepath();
	BOOL bFound = hFile.FindFile(TableMapWildcard.GetString());
	while (bFound) {
		if (_number_of_tablemaps_loaded >= k_max_nmber_of_tablemaps) {
			write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] CAutoConnector: Error: Too many tablemaps. The autoconnector can only handle 25 TMs.", "Error", 0);
			OH_MessageBox_Error_Warning("To many tablemaps.\n"
				"The auto-connector can handle 25 at most.");
			return;
		}
		bFound = hFile.FindNextFile();
		if (!hFile.IsDots() && !hFile.IsDirectory() && hFile.GetFilePath()!=current_path) {
			int ret = p_tablemap->LoadTablemap(hFile.GetFilePath().GetString());
			if (ret == SUCCESS)	{
				ExtractConnectionDataFromCurrentTablemap(p_tablemap);
        CTablemapCompletenessChecker tablemap_completeness_checker;
        tablemap_completeness_checker.VerifyMap();
				write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] Number of TMs loaded: %d\n", _number_of_tablemaps_loaded);
			}
		}
	}
}


void CTableMapLoader::ParseAllTableMapsToLoadConnectionData() {
	CString TableMapWildcard;
	
	write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] ParseAllTableMapsToLoadConnectionData\n");
	_number_of_tablemaps_loaded = 0;
	ParseAllTableMapsToLoadConnectionData(p_filenames->TableMapWildcard());	
	tablemaps_in_scraper_folder_already_parsed = true;
}

bool CTableMapLoader::tablemap_connection_dataAlreadyStored(CString TablemapFilePath) {
  assert(TablemapFilePath != "");
	for (int i=0; i<_number_of_tablemaps_loaded; i++)	{
    assert(tablemap_connection_data[i].FilePath != "");
		if (tablemap_connection_data[i].FilePath == TablemapFilePath)	{
			write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] tablemap_connection_dataAlreadyStored [%s] [true]\n", TablemapFilePath);
			return true;
		}
	}
	write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] tablemap_connection_dataAlreadyStored [%s] [false]\n", TablemapFilePath);
	return false;
}


void CTableMapLoader::CheckForDuplicatedTablemaps() {
	CString error_message = "";
	for (int i=0; i<_number_of_tablemaps_loaded; i++)
	{
		for (int j=i+1; j<_number_of_tablemaps_loaded; j++)
		{
			if	((tablemap_connection_data[i].SiteName == tablemap_connection_data[j].SiteName)
				&& (tablemap_connection_data[i].TitleText == tablemap_connection_data[j].TitleText))
			{
				write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] tablemap_connection_dataDuplicated [%s, %s]  [true]\n", 
					tablemap_connection_data[i].SiteName, tablemap_connection_data[i].TitleText);
				error_message.Format("It seems you have multiple versions of the same map in your scraper folder.\n\n"\
					"SiteName = %s\n"\
					"TitleText = %s\n\n"\
					"This will cause problems as the autoconnector won't be able to decide which one to use.\n"\
					"Please remove the superfluous maps from the scraper folder.\n", 
					tablemap_connection_data[i].SiteName, tablemap_connection_data[i].TitleText);
				OH_MessageBox_Error_Warning((LPCTSTR) error_message, 
					"Warning! Duplicate SiteName and TitleText");
			}
		}
	}
}

void CTableMapLoader::ExtractConnectionDataFromCurrentTablemap(CTablemap *cmap) {
	write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] ExtractConnectionDataFromCurrentTablemap(): %s\n", cmap->filepath());
	write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] number_of_tablemaps_loaded: %d\n", _number_of_tablemaps_loaded);

	// Avoiding to store the data twice, e.g. when we load a known TM manually
	if (tablemap_connection_dataAlreadyStored(cmap->filepath())) 	{
		write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] ExtractConnectionDataFromCurrentTablemap(): already stored; early exit\n");
		return;
	}

	tablemap_connection_data[_number_of_tablemaps_loaded].FilePath = cmap->filepath();
	tablemap_connection_data[_number_of_tablemaps_loaded].SiteName = cmap->sitename();
	tablemap_connection_data[_number_of_tablemaps_loaded].TitleText = cmap->titletext();

	if (cmap->sitename() == "") {
		CString error_message;
		error_message.Format("Tablemap contains no sitename.\n"
			"Sitenames are necessary to recognize duplicate TMs\n"
			"(and for other features like PokerTracker).\n\n",
			"%s", cmap->filepath());
		OH_MessageBox_Error_Warning(error_message, "Warning");
	}
	
	// Get clientsize info through TM-access-class
	p_tablemap_access->SetClientSize("clientsize", tablemap_connection_data[_number_of_tablemaps_loaded].ClientSizeX, tablemap_connection_data[_number_of_tablemaps_loaded].ClientSizeY);
	p_tablemap_access->SetClientSize("clientsizemin", tablemap_connection_data[_number_of_tablemaps_loaded].ClientSizeMinX, tablemap_connection_data[_number_of_tablemaps_loaded].ClientSizeMinY);
	p_tablemap_access->SetClientSize("clientsizemax", tablemap_connection_data[_number_of_tablemaps_loaded].ClientSizeMaxX, tablemap_connection_data[_number_of_tablemaps_loaded].ClientSizeMaxY);

	// Extract title text information
	p_tablemap_access->SetTitleText("titletext", tablemap_connection_data[_number_of_tablemaps_loaded].TitleText);
	// Extract negative title texs
	p_tablemap_access->SetTitleText("!titletext", tablemap_connection_data[_number_of_tablemaps_loaded].NegativeTitleText);
		
	CString s = "";
	for (int i=0; i<k_max_number_of_titletexts; i++) {
		s.Format("titletext%d", i);
		p_tablemap_access->SetTitleText(s, tablemap_connection_data[_number_of_tablemaps_loaded].TitleText_0_9[i]);

		s.Format("!titletext%d", i);
		p_tablemap_access->SetTitleText(s, tablemap_connection_data[_number_of_tablemaps_loaded].NegativeTitleText_0_9[i]);		
	}
	// Extract tablepoints r$tablepoint0..9
	s = "";
	int tpcount = 0;
	for (int i=0; i<10; ++i) {
		s.Format("tablepoint%d", i);
		if (p_tablemap_access->GetTableMapRegion(s, &tablemap_connection_data[_number_of_tablemaps_loaded].TablePoint[i])) {
		  ++tpcount;
		} else {
      break;
    }
	}
	tablemap_connection_data[_number_of_tablemaps_loaded].TablePointCount = tpcount;
	++_number_of_tablemaps_loaded;
}

// This function has to be global and can't be part of the class,
// as it has to be called by the callback-function 
// BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam) 
bool Check_TM_Against_Single_Window(int MapIndex, HWND h, RECT r, CString title) {
	bool			good_pos_title = false, good_neg_title = false; 
  bool      good_table_points = false;
	int				width = 0, height = 0, x = 0, y = 0;
	HDC				hdcScreen = NULL, hdcCompatible = NULL;
	HBITMAP			hbmScreen = NULL, hOldScreenBitmap = NULL;
	BYTE			*pBits = NULL, alpha = 0, red = 0, green = 0, blue = 0;
	CTransform		trans;
	CString			s;

	write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] Check_TM_Against_Single_Window(..)\n");
	write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] Checking map nr. %d\n", MapIndex);
	write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] Window title: %s\n", title);
	
	// Check for exact match on client size
	if (!((r.right == tablemap_connection_data[MapIndex].ClientSizeX)
	  	&& (r.bottom == tablemap_connection_data[MapIndex].ClientSizeY)))	{
		// Exact size didn't match.
		// So check for client size that falls within min/max
		if (!((tablemap_connection_data[MapIndex].ClientSizeMinX != 0) 
			  && (tablemap_connection_data[MapIndex].ClientSizeMinY != 0) 
			  && (tablemap_connection_data[MapIndex].ClientSizeMaxX != 0) 
			  && (tablemap_connection_data[MapIndex].ClientSizeMaxY != 0) 
			  && (r.right  >= tablemap_connection_data[MapIndex].ClientSizeMinX)
			  && (r.right  <= tablemap_connection_data[MapIndex].ClientSizeMaxX)
			  && (r.bottom >= tablemap_connection_data[MapIndex].ClientSizeMinY)
			  && (r.bottom <= tablemap_connection_data[MapIndex].ClientSizeMaxY))) {
			write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] No good size: Expected (%dpx, %dpx), Got (%dpx, %dpx)\n",
				tablemap_connection_data[MapIndex].ClientSizeX,
				tablemap_connection_data[MapIndex].ClientSizeY,
				r.right,
				r.bottom);
			return false;
		}
	}
	write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] Size matches; checking the rest...\n");
  
  // Check for match positive title text matches
	good_pos_title = false;
	if ((tablemap_connection_data[MapIndex].TitleText != "")
		&& title.Find(tablemap_connection_data[MapIndex].TitleText)!=-1)
	{
		good_pos_title = true;
	}
	else
	{
		// titletext din't match
		// Check for titletext0..titletext9
		for (int i=0; i<k_max_number_of_titletexts; i++)
		{
			if ((tablemap_connection_data[MapIndex].TitleText_0_9[i] != "")
				&& (title.Find(tablemap_connection_data[MapIndex].TitleText_0_9[i])!=-1))
			{
				good_pos_title = true;
				break;
			}
		}
	}
	if (!good_pos_title)
	{
		write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] No good title.\n");
		return false;
	}

	// Check for no negative title text matches
	good_neg_title = false;
	if ((tablemap_connection_data[MapIndex].NegativeTitleText != "")
		&& (title.Find(tablemap_connection_data[MapIndex].NegativeTitleText)!=-1))
	{
		good_neg_title = true;
	}
	else
	{
		for (int i=0; i<k_max_number_of_titletexts; i++)
		{
			if ((tablemap_connection_data[MapIndex].NegativeTitleText_0_9[i] != "")
				&&title.Find(tablemap_connection_data[MapIndex].NegativeTitleText_0_9[i])!=-1)
			{
				good_neg_title = true;
				break;
			}
		}
	}
	if (good_neg_title)
	{
		write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] Negative title found -> window is no match.\n"); 
		return false;
	}

  // tablepoint routine 
  if (tablemap_connection_data[MapIndex].TablePointCount > 0) {
    for (int i=0; i<tablemap_connection_data[MapIndex].TablePointCount; i++) {
      // Allocate heap space for BITMAPINFO
      BITMAPINFO  *bmi;
      int         info_len = sizeof(BITMAPINFOHEADER) + 1024;
      bmi = (BITMAPINFO *) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

      // Check table points for match
      width = r.right - r.left;
      height = r.bottom - r.top;
      hdcScreen = GetDC(h);
      hdcCompatible = CreateCompatibleDC(hdcScreen);
      hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);
      hOldScreenBitmap = (HBITMAP) SelectObject(hdcCompatible, hbmScreen);
      BitBlt(hdcCompatible, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

      // Populate BITMAPINFOHEADER
      bmi->bmiHeader.biSize = sizeof(bmi->bmiHeader);
      bmi->bmiHeader.biBitCount = 0;
      GetDIBits(hdcCompatible, hbmScreen, 0, 0, NULL, bmi, DIB_RGB_COLORS);

      // Get the actual argb bit information
      bmi->bmiHeader.biHeight = -bmi->bmiHeader.biHeight;
      pBits = new BYTE[bmi->bmiHeader.biSizeImage];
      GetDIBits(hdcCompatible, hbmScreen, 0, height, pBits, bmi, DIB_RGB_COLORS);

      good_table_points = true;
      x = tablemap_connection_data[MapIndex].TablePoint[i].left;
      y = tablemap_connection_data[MapIndex].TablePoint[i].top;

      int pbits_loc = y*width*4 + x*4;
      alpha = pBits[pbits_loc + 3];
      red = pBits[pbits_loc + 2];
      green = pBits[pbits_loc + 1];
      blue = pBits[pbits_loc + 0];

      COLORREF Color = tablemap_connection_data[MapIndex].TablePoint[i].color;
      // positive radius
      if (tablemap_connection_data[MapIndex].TablePoint[i].radius >= 0)
      {
         if (!trans.IsInARGBColorCube((Color>>24)&0xff, // function GetAValue() does not exist
                               GetRValue(Color),
                               GetGValue(Color),
                               GetBValue(Color),
                               tablemap_connection_data[MapIndex].TablePoint[i].radius,
                               alpha, red, green, blue)) {
            good_table_points = false;
         }
      }
      // negative radius (logical not)
      else {
         if (trans.IsInARGBColorCube((Color>>24)&0xff, // function GetAValue() does not exist
                              GetRValue(Color),
                              GetGValue(Color),
                              GetBValue(Color),
                              -tablemap_connection_data[MapIndex].TablePoint[i].radius,
                              alpha, red, green, blue))
         {
            good_table_points = false;
         }
      }

      // Clean up
      HeapFree(GetProcessHeap(), NULL, bmi);
      delete []pBits;
      SelectObject(hdcCompatible, hOldScreenBitmap);
      DeleteObject(hbmScreen);
      DeleteDC(hdcCompatible);
      ReleaseDC(h, hdcScreen);

      if (!good_table_points) {
         write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] Not all tablepoints match.\n");
         return false;
      }
    }
	}

	write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] Window ia a match\n");
	return true;
}

void CTableMapLoader::ReloadAllTablemapsIfChanged() {
	if (p_filesystem_monitor->AnyChanges())
	{		
		// Since OpenHoldem 4.0.0 there is no longer any possibility 
		// for manual connection; i.e. no possibility for re-connection.
		// Therefore the file-system-monitor does now dis-connect
		// if it finds a new/changed tablemap and the auto-connector-thread 
		// will thereafter reload the TMs and connect again.
		//
		// Note: This solution might lose some game-history (reset),
		// but that is perfectly acceptable for development
		// and hot-plugging of TMs won't happen in production.
		write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] Tablemaps changed; going to reload.\n");
		p_autoconnector->Disconnect();
		ParseAllTableMapsToLoadConnectionData();
	}
	else
	{
		write_log(MAIN->p_preferences()->debug_tablemap_loader(), "[CTablemapLoader] All tablemaps unchanged; nothing to do.\n");
	}
}

CString CTableMapLoader::GetTablemapPathToLoad(int tablemap_index) {
	assert(tablemap_index >= 0);
	assert(tablemap_index < _number_of_tablemaps_loaded);
	return tablemap_connection_data[tablemap_index].FilePath;
}

