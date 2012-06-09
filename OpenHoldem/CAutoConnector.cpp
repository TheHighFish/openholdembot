#include "stdafx.h"
#include "CAutoConnector.h"

#include <afxwin.h>
#include "..\CTablemap\CTablemap.h"
#include "..\CTablemap\CTablemapAccess.h"
#include "..\CTransform\CTransform.h"
#include "CAutoplayer.h"
#include "CFormula.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSharedMem.h"
#include "CSymbols.h"
#include "CTableMapLoader.h"
#include "CTablePositioner.h"
#include "DialogScraperOutput.h"
#include "DialogSelectTable.h"
#include "MagicNumbers.h"
#include "MainFrm.h"
#include "OH_MessageBox.h"
#include "OpenHoldem.h"


CAutoConnector						*p_autoconnector = NULL;






CAutoConnector::CAutoConnector()
{
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] CAutoConnector()\n");

	CString MutexName = prefs.mutex_name() + "AutoConnector";
	_autoconnector_mutex = new CMutex(false, MutexName);

	p_sharedmem->MarkPokerWindowAsUnAttached();
	set_attached_hwnd(NULL);
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
}


bool CAutoConnector::IsConnected()
{
	return ((_attached_hwnd != NULL) && IsWindow(_attached_hwnd));
}


void CAutoConnector::Check_TM_Against_All_Windows_Or_TargetHWND(int tablemap_index, HWND targetHWnd)
{
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Check_TM_Against_All_Windows(..)\n");

	if (targetHWnd == NULL)
		EnumWindows(EnumProcTopLevelWindowList, (LPARAM) tablemap_index);
	else
		EnumProcTopLevelWindowList(targetHWnd, (LPARAM) tablemap_index);
}


BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam) 
{
	CString				title = "", winclass = "";
	char				text[MAX_WINDOW_TITLE] = {0};
	RECT				crect = {0};
	STableList			tablelisthold;
	int					tablemap_index = (int)(lparam);

	write_log(prefs.debug_autoconnector(), "[CAutoConnector] EnumProcTopLevelWindowList(..)\n");
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Tablemap nr. %d\n", tablemap_index);
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
	if (Check_TM_Against_Single_Window(tablemap_index, hwnd, crect, title))
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
			tablelisthold.path = p_tablemap_loader->GetTablemapPathToLoad(tablemap_index);
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
	char				title[MAX_WINDOW_TITLE] = {0};
	int					SelectedItem = -1;
	SWholeMap			smap;
	CString				current_path = "";
	BOOL				bFound = false;
	CFileFind			hFile;

	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Connect(..)\n");
	p_tablemap_loader->ReloadAllTablemapsIfChanged();

	ASSERT(_autoconnector_mutex->m_hObject != NULL); 
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Locking autoconnector-mutex\n");
	if (!_autoconnector_mutex->Lock(500))
	{
		write_log(prefs.debug_autoconnector(), "[CAutoConnector] Could not grab mutex; early exit\n");
		return false; 
	}

	// Clear global list for holding table candidates
	g_tlist.RemoveAll();
	
	for (int tablemap_index=0; tablemap_index<p_tablemap_loader->NumberOfTableMapsLoaded(); tablemap_index++)
	{
		write_log(prefs.debug_autoconnector(), "[CAutoConnector] Going to check TM nr. %d out of %d\n", 
			tablemap_index, p_tablemap_loader->NumberOfTableMapsLoaded());
		Check_TM_Against_All_Windows_Or_TargetHWND(tablemap_index, targetHWnd);
	}
	
	// Put global candidate table list in table select dialog variables
	N = (int) g_tlist.GetSize();
	if (N == 0) 
	{
		p_sharedmem->RememberTimeOfLastFailedAttemptToConnect();

		int cySize = GetSystemMetrics(SM_CYSIZE);
		int cyMenuSize = GetSystemMetrics(SM_CYMENU);

		if (prefs.autoconnector_when_to_connect() != k_AutoConnector_Connect_Permanent)
		{
			if (cySize != 18 && cyMenuSize != 19)
			{
				OH_MessageBox(
					"Cannot find table.\n\n"
					"It appears that your settings are not configured according to OpenHoldem specifications.\n"
					"You must ensure that XP themes are not used (Use Windows Classic style) and\n"
					"font size is set to normal.\n\n"
					"For more info, read the manual and visit the user forums.", 
					"Cannot find table", MB_OK);
			}
			else
			{
				OH_MessageBox(
					"No valid tables found\n\n"
					"There seems to be no unserved table open\n"
					"or your table does not match the size and titlestring\n"
					"defined in your tablemaps.\n"
					"For more info, read the manual and visit the user forums.",
					"Cannot find table", MB_OK);
			}
		}
	}
	else 
	{
		SelectedItem = SelectTableMapAndWindow(N);

		if (SelectedItem == -1)
		{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] Attempt to connect did fail\n");
			p_sharedmem->RememberTimeOfLastFailedAttemptToConnect();
		}
		else		
		{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] Window [%d] selected\n", g_tlist[SelectedItem].hwnd);
			p_sharedmem->MarkPokerWindowAsAttached(g_tlist[SelectedItem].hwnd);
			// Load correct tablemap, and save hwnd/rect/numchairs of table that we are "attached" to
			set_attached_hwnd(g_tlist[SelectedItem].hwnd);
			CString loaded_version;
			p_tablemap->LoadTablemap((char *) g_tlist[SelectedItem].path.GetString(), 
				VER_OPENSCRAPE_2, &line, &loaded_version);

			if ( (loaded_version == VER_OPENSCRAPE_1 || loaded_version == VER_OPENHOLDEM_2))
			{
				OH_MessageBox("You have loaded a version 1 table map for this poker table.\n\n"\
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

			LoadScraperDLL();
			LoadScraperPreprocessorDLL();
			
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
			p_scraper_access->InitOnConnect();

			// Start timer that checks for continued existence of attached HWND
			pMyMainWnd->StartTimer();

			// Reset display
			pMyMainWnd->ResetDisplay();

			// Start logging, in case the log-level got changed.
			start_log();
			
			// log OH title bar text and table reset
			::GetWindowText(_attached_hwnd, title, MAX_WINDOW_TITLE);

			CString site = "";
			SMapCI site_i = p_tablemap->s$()->find("sitename");
			if (site_i != p_tablemap->s$()->end())
				site = site_i->second.text;

			CString formula = p_formula->formula_name();
			write_log(k_always_log_basic_information, 
				"\n*************************************************************\n"
				"TABLE RESET %s - %s(%s)\n"
				"*************************************************************\n",
				formula.GetString(), site.GetString(), title);
		}
	}
	p_table_positioner->PositionMyWindow();

	write_log(prefs.debug_autoconnector(), "[CAutoConnector] Unlocking autoconnector-mutex\n");
	_autoconnector_mutex->Unlock();
	return (SelectedItem != -1);
}


void CAutoConnector::LoadScraperDLL()
{
	// scraper.dll - failure in load is NOT fatal
	theApp.UnloadScraperDLL();
	CString filename = p_tablemap->scraperdll();
	if (filename.IsEmpty()) 
	{
		return;
	}
	// Otherwise: try to load DLL
	SetCurrentDirectory(_startup_path);
	theApp._scraper_dll = LoadLibrary(filename);

	if (theApp._scraper_dll == NULL)
	{
		CString	error_message = "";
		error_message.Format("Unable to load scraper-dll: \"%s\"\n\n"
			"Error-code: %d", filename, GetLastError());
		OH_MessageBox(error_message, "Error", 0);
		return;
	}

	theApp._dll_scraper_process_message = (scraper_process_message_t) GetProcAddress(theApp._scraper_dll, "ProcessMessage");
	theApp._dll_scraper_override = (scraper_override_t) GetProcAddress(theApp._scraper_dll, "OverrideScraper");

	if (theApp._dll_scraper_process_message==NULL || theApp._dll_scraper_override==NULL)
	{
		OH_MessageBox("Unable to find all symbols in scraper.dll", "Error", 0);
		theApp.UnloadScraperDLL();
	}
	else
	{
			write_log(prefs.debug_autoconnector(), "[CAutoConnector] scraper.dll (%s) loaded, ProcessMessage and OverrideScraper found.\n", filename);
	}
}


void CAutoConnector::LoadScraperPreprocessorDLL()
{
	// scraperpreprocessor.dll - failure in load is NOT fatal
	theApp.Unload_ScraperPreprocessor_DLL();
	CString filename = p_tablemap->scraperpreprocessor_dll();
	if (filename.IsEmpty()) 
	{
		return;
	}
	// Otherwise: try to load DLL
	SetCurrentDirectory(_startup_path);
	theApp._scraperpreprocessor_dll = LoadLibrary(filename);

	if (theApp._scraperpreprocessor_dll==NULL)
	{
		CString error_message = "";
		error_message.Format("Unable to load scraper-preprocessor-dll: \"%s\"\n\n"
			"Error-code: %d", filename, GetLastError());
		OH_MessageBox(error_message, "OpenHoldem scraperpre.dll WARNING", MB_OK | MB_TOPMOST);
		return;
	}
	theApp._dll_scraperpreprocessor_process_message = (scraperpreprocessor_process_message_t) GetProcAddress(theApp._scraperpreprocessor_dll, "ProcessMessage");
	if (theApp._dll_scraperpreprocessor_process_message==NULL)
	{
		CString	error_message = "";
		error_message.Format("Unable to find symbols in scraper_preprocessor.dll");
		OH_MessageBox(error_message, "OpenHoldem scraperpreprocessor.dll ERROR", MB_OK | MB_TOPMOST);
		theApp.Unload_ScraperPreprocessor_DLL();
	}
	else
	{
		write_log(prefs.debug_autoconnector(), "[CAutoConnector] scraperpreprocessor.dll loaded, ProcessMessage found.\n");
	}
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

	// Clear "attached" info
	set_attached_hwnd(NULL);

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
	write_log(k_always_log_basic_information, 
		"%s - %s(NOT ATTACHED)\n", p_formula->formula_name().GetString(), sitename);
	write_log(k_always_log_basic_information, 
		"TABLE RESET\n*************************************************************\n");

	// Stop logging
	stop_log();
	
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