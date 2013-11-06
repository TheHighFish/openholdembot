//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#ifndef INC_CTABLEMAPLOADER_H
#define INC_CTABLEMAPLOADER_H

#include "..\CTablemap\CTablemap.h"

// This function has to be global and can't be part of the class,
// as it has to be called by the callback-function 
// BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam) 
bool Check_TM_Against_Single_Window(int MapIndex, HWND h, RECT r, CString title);

extern class CTableMapLoader
{
public:
	CTableMapLoader();
	~CTableMapLoader();
public:
	int NumberOfTableMapsLoaded() { return _number_of_tablemaps_loaded; }
	void ReloadAllTablemapsIfChanged();
	CString GetTablemapPathToLoad(int tablemap_index);
private:
	void CTableMapToSWholeMap(CTablemap *cmap, SWholeMap *smap);
	void ParseAllTableMapsToLoadConnectionData(CString TableMapWildcard);
	void ParseAllTableMapsToLoadConnectionData();
	bool tablemap_connection_dataAlreadyStored(CString TablemapFilePath);
	void CheckForDuplicatedTablemaps();
	void ExtractConnectionDataFromCurrentTablemap(SWholeMap *map);
private:
	bool	tablemaps_in_scraper_folder_already_parsed;
	int		_number_of_tablemaps_loaded;
} *p_tablemap_loader;

#endif // INC_CTABLEMAPLOADER_H
