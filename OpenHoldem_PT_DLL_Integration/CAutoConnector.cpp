#include "stdafx.h"
#include "CAutoConnector.h"

#include <afxwin.h>
#include "..\CTablemap\CTablemap.h"
#include "..\CTablemap\CTablemapAccess.h"
#include "..\CTransform\CTransform.h"
#include "CAutoplayer.h"
#include "CEngineContainer.h"
#include "CFilenames.h"
#include "CFlagsToolbar.h"
#include "CFormula.h"
#include "CHeartbeatThread.h"
#include "CIteratorVars.h"
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

void CAutoConnector::WriteLogTableReset()
{
	CString site = "";
	SMapCI site_i = p_tablemap->s$()->find("sitename");
	if (site_i != p_tablemap->s$()->end())
	{
		site = site_i->second.text;
	}
	CString formula = p_formula->formula_name();

	write_log(k_always_log_basic_information, 
		"\n"
		"*************************************************************\n"
		"TABLE RESET %s - %s(%s)\n"
		"*************************************************************\n",
		formula.GetString(), site.GetString(), p_scraper->title());
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

		if (prefs.autoconnector_when_to_connect() == k_AutoConnector_Connect_Once)
		{
			if (cySize != 18 || cyMenuSize != 19)
			{
				OH_MessageBox_Error_Warning(
					"Cannot find table.\n\n"
					"It appears that your settings are not configured according to OpenHoldem specifications.\n"
					"You must ensure that XP themes are not used (Use Windows Classic style) and\n"
					"font size is set to normal.\n\n"
					"For more info, read the manual and visit the user forums.", 
					"Cannot find table");
			}
			else
			{
				OH_MessageBox_Error_Warning(
					"No valid tables found\n\n"
					"There seems to be no unserved table open\n"
					"or your table does not match the size and titlestring\n"
					"defined in your tablemaps.\n"
					"For more info, read the manual and visit the user forums.",
					"Cannot find table");
			}
		}
	}
	else 
	{
		SelectedItem = SelectTableMapAndWindow(N);

		if (SelectedItem == k_undefined)
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

			p_tablemap->LoadTablemap((char *) g_tlist[SelectedItem].path.GetString());

			// Create bitmaps
			p_scraper->CreateBitmaps();

			// Clear scraper fields
			p_scraper->ClearScrapeAreas();

			// Reset symbols
			p_engine_container->ResetOnConnection();

			// Reset "ScraperOutput" dialog, if it is live
			if (m_ScraperOutputDlg) 
			{
				m_ScraperOutputDlg->Reset();
			}

			LoadScraperDLL();
			LoadScraperPreprocessorDLL();
			
			p_flags_toolbar->DisableButtonsOnConnect();

			// Make sure autoplayer is off
			p_autoplayer->set_autoplayer_engaged(false);

			// mark symbol result cache as stale
			p_formula->MarkCacheStale();

			// reset iterator vars !!! not here!
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
			PMainframe()->StartTimer();

			// Reset display
			PMainframe()->ResetDisplay();

			// Start logging, in case the log-level got changed.
			start_log();
			
			// log OH title bar text and table reset
			::GetWindowText(_attached_hwnd, title, MAX_WINDOW_TITLE);

			WriteLogTableReset();

			p_table_positioner->PositionMyWindow();
//!!!			p_pokertracker_thread->ClearAllStats();
			p_pokertracker_thread->StartThread();
		}
	}
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
	p_filenames->SwitchToOpenHoldemDirectory();
	theApp._scraper_dll = LoadLibrary(filename);

	if (theApp._scraper_dll == NULL)
	{
		CString	error_message = "";
		error_message.Format("Unable to load scraper-dll: \"%s\"\n\n"
			"Error-code: %d", filename, GetLastError());
		OH_MessageBox_Error_Warning(error_message, "Error");
		return;
	}

	theApp._dll_scraper_process_message = (scraper_process_message_t) GetProcAddress(theApp._scraper_dll, "ProcessMessage");
	theApp._dll_scraper_override = (scraper_override_t) GetProcAddress(theApp._scraper_dll, "OverrideScraper");

	if (theApp._dll_scraper_process_message==NULL || theApp._dll_scraper_override==NULL)
	{
		OH_MessageBox_Error_Warning("Unable to find all symbols in scraper.dll", "Error");
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
	p_filenames->SwitchToOpenHoldemDirectory();
	theApp._scraperpreprocessor_dll = LoadLibrary(filename);

	if (theApp._scraperpreprocessor_dll==NULL)
	{
		CString error_message = "";
		error_message.Format("Unable to load scraper-preprocessor-dll: \"%s\"\n\n"
			"Error-code: %d", filename, GetLastError());
		OH_MessageBox_Error_Warning(error_message, "OpenHoldem scraperpre.dll WARNING");
		return;
	}
	theApp._dll_scraperpreprocessor_process_message = (scraperpreprocessor_process_message_t) GetProcAddress(theApp._scraperpreprocessor_dll, "ProcessMessage");
	if (theApp._dll_scraperpreprocessor_process_message==NULL)
	{
		CString	error_message = "";
		error_message.Format("Unable to find symbols in scraper_preprocessor.dll");
		OH_MessageBox_Error_Warning(error_message, "OpenHoldem scraperpreprocessor.dll ERROR");
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
	PMainframe()->KillTimer();
	p_flags_toolbar->UnattachOHFromPokerWindow();
	p_flags_toolbar->EnableButtonsOnDisconnect();

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
	p_engine_container->ResetOnConnection();

	// Change window title
	PMainframe()->UpdateWindowTitle();

	// Reset Display 
	PMainframe()->ResetDisplay();

	// Reset "ScraperOutput" dialog, if it is live
	if (m_ScraperOutputDlg)
	{
		m_ScraperOutputDlg->Reset();
	}
	WriteLogTableReset();

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
	// Always connecting automatically
	return SelectTableMapAndWindowAutomatically(Choices);
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

double CAutoConnector::TimeSincelast_failed_attempt_to_connect()
{
	time_t last_failed_attempt_to_connect = p_sharedmem->GetTimeOfLastFailedAttemptToConnect(); //???
	time_t CurrentTime;
	time(&CurrentTime);
	double _TimeSincelast_failed_attempt_to_connect = difftime(CurrentTime, last_failed_attempt_to_connect);
	write_log(prefs.debug_autoconnector(), "[CAutoConnector] TimeSincelast_failed_attempt_to_connect %f\n", _TimeSincelast_failed_attempt_to_connect);
	return _TimeSincelast_failed_attempt_to_connect;
}