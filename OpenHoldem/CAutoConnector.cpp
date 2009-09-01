#include "stdafx.h"

#include <afxwin.h>
//#include <process.h>
//#include <windows.h>
//#include <WinUser.h>

#include "CAutoConnector.h"
#include "CAutoplayer.h"
#include "CFormula.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSymbols.h"
#include "..\CTablemap\CTablemap.h"
#include "..\CTransform\CTransform.h"
#include "DialogScraperOutput.h"
#include "DialogSelectTable.h"
#include "MainFrm.h"
#include "OpenHoldem.h"




#include "stdafx.h"
#include <process.h>

#include "MainFrm.h"

#include "CAutoConnector.h"
#include "CAutoplayer.h"
#include "CDllExtension.h"
#include "CFormula.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "CPerl.hpp"
#include "CPokerPro.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CReplayFrame.h"
#include "CScraper.h"
#include "CSymbols.h"
#include "..\CTransform\CTransform.h"
#include "CValidator.h"
#include "DialogChairNum.h"
#include "DialogFormulaScintilla.h"
#include "DialogLockBlinds.h"
#include "DialogPPro.h"
#include "DialogSAPrefs1.h"
#include "DialogSAPrefs2.h"
#include "DialogSAPrefs3.h"
#include "DialogSAPrefs4.h"
#include "DialogSAPrefs5.h"
#include "DialogSAPrefs6.h"
#include "DialogSAPrefs7.h"
#include "DialogSAPrefs8.h"
#include "DialogSAPrefs9.h"
#include "DialogSAPrefs10.h"
#include "DialogSAPrefs11.h"
#include "DialogSAPrefs12.h"
#include "DialogSAPrefs13.h"
#include "DialogSAPrefs14.h"
#include "DialogSAPrefs15.h"
#include "DialogScraperOutput.h"
#include "inlines/eval.h"
#include "OpenHoldem.h"
#include "OpenHoldemDoc.h"
#include "SAPrefsDialog.h"


CAutoConnector						*p_autoconnector = NULL;
CArray <STableList, STableList>		g_tlist; 


CAutoConnector::CAutoConnector()
{
}


CAutoConnector::~CAutoConnector()
{
}


// TODO? Move to CTablemap?
void CAutoConnector::Check_TM_Against_All_Windows(SWholeMap smap, HWND targetHWnd)
{
	smap.z$ = p_tablemap->z$();
	smap.s$ = p_tablemap->s$();
	smap.r$ = p_tablemap->r$();
	for (int i=0; i<=3; i++)
	{
		smap.t$[i] = p_tablemap->t$(i);
		smap.p$[i] = p_tablemap->p$(i);
		smap.h$[i] = p_tablemap->h$(i);
	}
	smap.i$ = p_tablemap->i$();
	smap.filepath = p_tablemap->filepath();

	if (targetHWnd == NULL)
		EnumWindows(EnumProcTopLevelWindowList, (LPARAM) &smap);
	else
		EnumProcTopLevelWindowList(targetHWnd, (LPARAM) &smap);
}

// TODO? Move to CTablemap?
// This function has to be global and can't be part of the class,
// as it has to be called by the callback-function 
// BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam) 
bool Check_TM_Against_Single_Window(SWholeMap *map, HWND h, RECT r, CString title) 
{
	int				i = 0;
	bool			good_size = false, good_pos_title = false, good_neg_title = false, good_table_points = false;
	int				width = 0, height = 0, x = 0, y = 0;
	HDC				hdcScreen = NULL, hdcCompatible = NULL;
	HBITMAP			hbmScreen = NULL, hOldScreenBitmap = NULL;
	BYTE			*pBits = NULL, alpha = 0, red = 0, green = 0, blue = 0;
	int				exact_width = 0, exact_height = 0, min_width = 0, min_height = 0, max_width = 0, max_height = 0;
	CTransform		trans;
	CString			s;

	ZMapCI z_iter = map->z$->end();
	z_iter = map->z$->find("clientsize");
	if (z_iter != map->z$->end())
	{
		exact_width = z_iter->second.width;
		exact_height = z_iter->second.height;
	}
	z_iter = map->z$->find("clientsizemin");
	if (z_iter != map->z$->end())
	{
		min_width = z_iter->second.width;
		min_height = z_iter->second.height;
	}
	z_iter = map->z$->find("clientsizemax");
	if (z_iter != map->z$->end())
	{
		max_width = z_iter->second.width;
		max_height = z_iter->second.height;
	}

	// Check for exact match on client size
	good_size = false;
	if (r.right == exact_width && r.bottom == exact_height)
	{
		good_size = true;
	}

	// Check for client size that falls within min/max
	if (min_width != 0 && min_height != 0 && max_width != 0 &&  max_height != 0 &&
			r.right >= (int) min_width && r.right <= (int) max_width &&
			r.bottom >= (int) min_height && r.bottom <= (int) max_height)
	{
		good_size = true;
	}

	if (!good_size)
		return false;

	// Check title text for match
	SMapCI s_iter = map->s$->end();

	good_pos_title = false;

	s_iter = map->s$->find("titletext");
	if (s_iter!=map->s$->end())
		if (title.Find(s_iter->second.text)!=-1)
			good_pos_title = true;
	
	for (i=0; i<=9; i++)
	{
		s.Format("titletext%d", i);
		s_iter = map->s$->find(s.GetString());
		if (s_iter!=map->s$->end())
			if (title.Find(s_iter->second.text)!=-1)
				good_pos_title = true;
	}

	if (!good_pos_title)
		return false;

	// Check for no negative title text matches
	good_neg_title = true;

	s_iter = map->s$->find("!titletext");
	if (s_iter!=map->s$->end())
		if (title.Find(s_iter->second.text)!=-1)
			good_neg_title = true;

	for (i=0; i<=9; i++)
	{
		s.Format("!titletext%d", i);
		s_iter = map->s$->find(s.GetString());
		if (s_iter!=map->s$->end())
			if (title.Find(s_iter->second.text)!=-1)
				good_neg_title = true;
	}

	if (!good_neg_title)
		return false;

	// Allocate heap space for BITMAPINFO
	BITMAPINFO	*bmi;
	int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
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
	bool found = false;
	for (RMapCI r_iter=map->r$->begin(); r_iter!=map->r$->end() && !found; r_iter++)
	{
		if (r_iter->second.name.Find("tablepoint") != -1 &&
			r_iter->second.right - r_iter->second.left == 1 &&
			r_iter->second.bottom - r_iter->second.top == 1 &&
			r_iter->second.transform == "C")
		{

			x = r_iter->second.left;
			y = r_iter->second.top;

			int pbits_loc = y*width*4 + x*4;
			alpha = pBits[pbits_loc + 3];
			red = pBits[pbits_loc + 2];
			green = pBits[pbits_loc + 1];
			blue = pBits[pbits_loc + 0];

			// positive radius
			if (r_iter->second.radius >= 0)
			{
				if (!trans.IsInARGBColorCube((r_iter->second.color>>24)&0xff,
											 GetRValue(r_iter->second.color),
											 GetGValue(r_iter->second.color),
											 GetBValue(r_iter->second.color),
											 r_iter->second.radius,
											 alpha, red, green, blue))
				{
					good_table_points = false;
					found = true;
				}
			}
			// negative radius (logical not)
			else
			{
				if (trans.IsInARGBColorCube((r_iter->second.color>>24)&0xff,
											r_iter->second.color,
											r_iter->second.color,
											r_iter->second.color,
											-r_iter->second.radius,
											alpha, red, green, blue))
				{
					good_table_points = false;
					found = true;
				}
			}

		}
	}

	// Clean up
	HeapFree(GetProcessHeap(), NULL, bmi);
	delete []pBits;
	SelectObject(hdcCompatible, hOldScreenBitmap);
	DeleteObject(hbmScreen);
	DeleteDC(hdcCompatible);
	ReleaseDC(h, hdcScreen);

	if (!good_table_points)
		return false;

	return true;
}


BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam) 
{
	CString				title = "", winclass = "";
	char				text[512] = {0};
	RECT				crect = {0};
	STableList			tablelisthold;
	SWholeMap			*map = (SWholeMap *) (lparam);

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
	GetClientRect(hwnd, &crect);

	// See if it matches the currently loaded table map
	if (Check_TM_Against_Single_Window(map, hwnd, crect, title))
	{
		tablelisthold.hwnd = hwnd;
		tablelisthold.title = title;
		tablelisthold.path = map->filepath;
		tablelisthold.crect.left = crect.left;
		tablelisthold.crect.top = crect.top;
		tablelisthold.crect.right = crect.right;
		tablelisthold.crect.bottom = crect.bottom;
		g_tlist.Add(tablelisthold);
	}

	return true;  // keep processing through entire list of windows
}


bool CAutoConnector::Connect(HWND targetHWnd)
{
	int					i = 0, N = 0, line = 0, ret = 0;
	char				title[512] = {0};
	int					SelectedItem = -1;
	SWholeMap			smap;
	CString				path = "", current_path = "";
	BOOL				bFound = false;
	CFileFind			hFile;

	// Clear global list for holding table candidates
	g_tlist.RemoveAll();

	// First check explicitly loaded/last used tablemap
	current_path = "";
	if (p_tablemap->valid())
	{
		Check_TM_Against_All_Windows(smap, targetHWnd);
		current_path = p_tablemap->filepath();
	}

	// OpenScrape table maps
	path.Format("%s\\scraper\\*.tm", _startup_path);
	bFound = hFile.FindFile(path.GetString());
	while (bFound)
	{
		bFound = hFile.FindNextFile();
		if (!hFile.IsDots() && !hFile.IsDirectory() && hFile.GetFilePath()!=current_path)
		{
			ret = p_tablemap->LoadTablemap((char *) hFile.GetFilePath().GetString(), VER_OPENSCRAPE_2, false, &line, prefs.disable_msgbox());
			if (ret == SUCCESS)
			{
				Check_TM_Against_All_Windows(smap, targetHWnd);
			}
		}
	}

	// WinScrape converted profiles
	path.Format("%s\\scraper\\*.ws", _startup_path);
	bFound = hFile.FindFile(path.GetString());
	while (bFound)
	{
		bFound = hFile.FindNextFile();
		if (!hFile.IsDots() && !hFile.IsDirectory() && hFile.GetFilePath()!=current_path)
		{
			ret = p_tablemap->LoadTablemap((char *) hFile.GetFilePath().GetString(), VER_OPENSCRAPE_2, false, &line, prefs.disable_msgbox());
			if (ret == SUCCESS)
			{
				Check_TM_Against_All_Windows(smap, targetHWnd);
			}
		}
	}

	// Put global candidate table list in table select dialog variables
	N = (int) g_tlist.GetSize();
	if (N == 0) 
	{
		if (targetHWnd != NULL) {
			int cySize = GetSystemMetrics(SM_CYSIZE);
			int cyMenuSize = GetSystemMetrics(SM_CYMENU);

			if (!prefs.disable_msgbox())
			{
				if (cySize != 18 && cyMenuSize != 19)
					MessageBox(0, "Cannot find table\n\n"
							   "It appears that your settings are not configured according to OpenHoldem specifications.\n"
							   "You must ensure that XP themes are not used (Use Windows Classic style) and\n"
							   "font size is set to normal.\n\n"
							   "For more info, look at the wiki documentation and the user forums", "Cannot find table", MB_OK);
				else
					MessageBox(0, "No valid tables found", "Cannot find table", MB_OK);
			}
		}
	}
	else 
	{
		SelectedItem = SelectTableMapAndWindow(N);
		if (SelectedItem != -1)
		{
			// Load correct tablemap, and save hwnd/rect/numchairs of table that we are "attached" to
			set_attached_hwnd(g_tlist[SelectedItem].hwnd);
			CString loaded_version;
			p_tablemap->LoadTablemap((char *) g_tlist[SelectedItem].path.GetString(), VER_OPENSCRAPE_2, false, &line, 
									 prefs.disable_msgbox(), &loaded_version);

//!!!!!!!!!!!!
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
				int index = p_tablemap->filepath().ReverseFind('\\');
				if (index > 0)
					path.Format("%s\\%s", p_tablemap->filepath().Left(index), filename);
				else
					path = filename;
				theApp._scraper_dll = LoadLibrary(path);
			}
			if (theApp._scraper_dll==NULL)
			{
				if (!filename.IsEmpty() && !prefs.disable_msgbox())		
				{
					CString		t = "";
					t.Format("Unable to load %s\n\nError: %d", path, GetLastError());
					MessageBox(0, t, "OpenHoldem scraper.dll WARNING", MB_OK | MB_TOPMOST);
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
						CString		t = "";
						t.Format("Unable to find all symbols in scraper.dll");
						MessageBox(0, t, "OpenHoldem scraper.dll ERROR", MB_OK | MB_TOPMOST);
					}

					theApp.UnloadScraperDLL();
				}
				else
				{
					if (!prefs.disable_msgbox())		
					{
					}
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

			// start heartbeat thread
			if (p_heartbeat_thread)
				delete p_heartbeat_thread;

			p_heartbeat_thread = new CHeartbeatThread;

			// Start timer that checks for continued existence of attached HWND
			pMyMainWnd->StartTimer(); //!!!

			// Reset display //!!!
			pMyMainWnd->ResetDisplay();

			// Start logging
			start_log();

			if (targetHWnd == NULL) 
			{
				CWindowDC dc(NULL);
				int nBitsPerPixel = dc.GetDeviceCaps(PLANES) * dc.GetDeviceCaps(BITSPIXEL);

				if (nBitsPerPixel < 24 && !prefs.disable_msgbox())
					MessageBox(0, "It appears that your Display settings are not configured according to OpenHoldem specifications.\n"
							   "24 bit color or higher is needed to reliably extract information from the poker client\n\n"
							   "For more info, look at the wiki documentation and the user forums", 
							   "Caution: Color Depth Too Low", MB_OK|MB_ICONWARNING);

				BOOL fontSmoothingEnabled = FALSE;
				SystemParametersInfo(SPI_GETFONTSMOOTHING, 0, (LPVOID)&fontSmoothingEnabled, 0);

				if (fontSmoothingEnabled && !prefs.disable_msgbox())
					MessageBox(0, "It appears that font smoothing is enabled. In order for OpenHoldem to reliably\n"
							   "extract information from the poker client you should disable Font Smoothing", 
							   "Caution: Font smoothing is enabled", MB_OK|MB_ICONWARNING);
			}

			// log OH title bar text and table reset
			::GetWindowText(_attached_hwnd, title, 512);

			CString site="";
			SMapCI site_i = p_tablemap->s$()->find("sitename");
			if (site_i != p_tablemap->s$()->end())
				site = site_i->second.text;

			CString formula = p_formula->formula_name();

			write_log(1, "\n*************************************************************\n"
					  "TABLE RESET %s - %s(%s)\n"
					  "*************************************************************\n",
					  formula.GetString(), site.GetString(), title);

		}
	}
	return (SelectedItem != -1);
}


void CAutoConnector::Disconnect()
{
	// stop threads
	if (p_heartbeat_thread)
	{
		delete p_heartbeat_thread;
		p_heartbeat_thread = NULL;
	}

	if (p_pokertracker_thread)
		p_pokertracker_thread->StopThread();

	// Make sure autoplayer is off
	p_autoplayer->set_autoplayer_engaged(false);
	_autoplay_pressed = false;

	// Send "disconnect" to scraper DLL, if loaded
	if (theApp._dll_scraper_process_message)
			(theApp._dll_scraper_process_message) ("disconnect", NULL);

	theApp.UnloadScraperDLL();

	// Clear "attached" info
	set_attached_hwnd(NULL);

	// Stop timer that checks for valid hwnd, then unattach OH. ///!!!
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	pMyMainWnd->KillTimer();
	pMyMainWnd->UnattachOHFromPokerWindow();
	pMyMainWnd->EnableButtonsOnDisconnect();

	// Delete bitmaps
	p_scraper->DeleteBitmaps();

	// Clear scraper fields
	p_scraper->ClearScrapeAreas();

	// Reset symbols
	p_symbols->ResetSymbolsFirstTime();

	// Change window title
	pMyMainWnd->UpdateWindowTitle();

	// Reset Display //!!!
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
}


int CAutoConnector::SelectTableMapAndWindow(int Choices)
{
	MessageBox(0, "Connect()", "Connector", 0);
	if (prefs.autoconnector_connection_method() == 0)
	{
		return SelectTableMapAndWindowManually(Choices);
	}
	else
	{
		return SelectTableMapAndWindowAutomatically(Choices);
	}
}


int CAutoConnector::SelectTableMapAndWindowManually(int Choices)
{
	STableList			tablelisthold;
	CDlgSelectTable		cstd;
	int					result = 0;

	if (Choices == 1) 
	{
		//result = IDOK; !!!
		// First (and only) item selected
		//cstd.selected_item = 0;
		return 0;
	}
	else if (Choices >= 2) 
	{
		for (int i=0; i<Choices; i++) 
		{
			tablelisthold.hwnd = g_tlist[i].hwnd;
			tablelisthold.title = g_tlist[i].title;
			tablelisthold.path = g_tlist[i].path;
			tablelisthold.crect.left = g_tlist[i].crect.left;
			tablelisthold.crect.top = g_tlist[i].crect.top;
			tablelisthold.crect.right = g_tlist[i].crect.right;
			tablelisthold.crect.bottom = g_tlist[i].crect.bottom;
			cstd.tlist.Add(tablelisthold);
		}

		// Display table select dialog
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
	else
	{
		// 0 (or even less) windows and TMs.
		return -1;
	}
}


int CAutoConnector::SelectTableMapAndWindowAutomatically(int Choices)
{
	return -1;
}


void CAutoConnector::ShutdownOH()
{
}

