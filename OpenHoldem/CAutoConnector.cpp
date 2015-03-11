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
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "COpenHoldemTitle.h"
#include "CPokerTrackerThread.h"
#include "CPopupHandler.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScrapedActionInterface.h"
#include "CSharedMem.h"
#include "CTableMapLoader.h"
#include "CTableState.h"
#include "CTablePositioner.h"
#include "CVersionInfo.h"
#include "DialogScraperOutput.h"
#include "MagicNumbers.h"
#include "MainFrm.h"
#include "OH_MessageBox.h"
#include "OpenHoldem.h"

CAutoConnector						*p_autoconnector = NULL;
CArray <STableList, STableList>		g_tlist; 

CAutoConnector::CAutoConnector()
{
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] CAutoConnector()\n");

	CString MutexName = preferences.mutex_name() + "AutoConnector";
	_autoconnector_mutex = new CMutex(false, MutexName);

	p_sharedmem->MarkPokerWindowAsUnAttached();
	set_attached_hwnd(NULL);
}


CAutoConnector::~CAutoConnector()
{
	// Releasing the mutex in case we hold it.
	// If we don't hold it, Unlock() will "fail" silently.
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] ~CAutoConnector()\n");
	_autoconnector_mutex->Unlock();
	if (_autoconnector_mutex != NULL)
	{

		write_log(preferences.debug_autoconnector(), "[CAutoConnector] ~CAutoConnector() Deleting auto-connector-mutex\n");
		delete _autoconnector_mutex;
		_autoconnector_mutex = NULL;
	}
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] ~CAutoConnector() Marking table as not atached\n");
	p_sharedmem->MarkPokerWindowAsUnAttached();
	set_attached_hwnd(NULL);
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] ~CAutoConnector() Finished\n");
}


bool CAutoConnector::IsConnected()
{
	return ((_attached_hwnd != NULL) && IsWindow(_attached_hwnd));
}


void CAutoConnector::Check_TM_Against_All_Windows_Or_TargetHWND(int tablemap_index, HWND targetHWnd)
{
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Check_TM_Against_All_Windows(..)\n");

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

	write_log(preferences.debug_autoconnector(), "[CAutoConnector] EnumProcTopLevelWindowList(..)\n");
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Tablemap nr. %d\n", tablemap_index);
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
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] EnumProcTopLevelWindowList(..) found a window candidate...\n");
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
			write_log(preferences.debug_autoconnector(), "[CAutoConnector] Window candidate already served: [%d]\n", hwnd);
		}
		else
		{
			write_log(preferences.debug_autoconnector(), "[CAutoConnector] Adding window candidate to the list: [%d]\n", hwnd);
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
	write_log(k_always_log_basic_information,
		"\n"
		"==============================================\n"
		"%s"
		"TABLE RESET\n"
		"==============================================\n",
		p_version_info->GetVersionInfo());
}

void CAutoConnector::FailedToConnectBecauseNoWindowInList()
{
	p_sharedmem->RememberTimeOfLastFailedAttemptToConnect();

	int cySize = GetSystemMetrics(SM_CYSIZE);
	int cyMenuSize = GetSystemMetrics(SM_CYMENU);

	if (preferences.autoconnector_when_to_connect() == k_AutoConnector_Connect_Once)
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
	GoIntoPopupBlockingMode();
}

void CAutoConnector::FailedToConnectProbablyBecauseAllTablesAlreadyServed()
{
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Attempt to connect did fail\n");
	p_sharedmem->RememberTimeOfLastFailedAttemptToConnect();
	GoIntoPopupBlockingMode();
}

void CAutoConnector::GoIntoPopupBlockingMode()
{
	// We have a free instance that has nothing to do.
	// Care about potential popups here, once per auto-connector-heartbeat.
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Not connected. Going into popup-blocking mode.\n");
	if (p_sharedmem->AnyWindowAttached())
	{
		// Only handle popups if at least one bot is connected to a table.
		// Especially stop popup-handling if the last table got closed
		// to allow "normal" human work again.
		assert(p_popup_handler != NULL);
		p_popup_handler->HandleAllWindows();
	}
}

bool CAutoConnector::Connect(HWND targetHWnd) {
	int					N = 0, line = 0, ret = 0;
	char				title[MAX_WINDOW_TITLE] = {0};
	int					SelectedItem = -1;
	CString			current_path = "";
	BOOL				bFound = false;
	CFileFind   hFile;

	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Connect(..)\n");

	ASSERT(_autoconnector_mutex->m_hObject != NULL); 
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Locking autoconnector-mutex\n");
	if (!_autoconnector_mutex->Lock(500))	{
		write_log(preferences.debug_autoconnector(), "[CAutoConnector] Could not grab mutex; early exit\n");
		return false; 
	}
  // Clear global list for holding table candidates
	g_tlist.RemoveAll();
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Number of tablemaps loaded: %i\n",
    p_tablemap_loader->NumberOfTableMapsLoaded());
	for (int tablemap_index=0; tablemap_index<p_tablemap_loader->NumberOfTableMapsLoaded(); tablemap_index++) {
		write_log(preferences.debug_autoconnector(), "[CAutoConnector] Going to check TM nr. %d out of %d\n", 
			tablemap_index, p_tablemap_loader->NumberOfTableMapsLoaded());
		Check_TM_Against_All_Windows_Or_TargetHWND(tablemap_index, targetHWnd);
	}
	
	// Put global candidate table list in table select dialog variables
	N = (int) g_tlist.GetSize();
  write_log(preferences.debug_autoconnector(), "[CAutoConnector] Number of table candidates> %i\n", N);
	if (N == 0) {
		FailedToConnectBecauseNoWindowInList();
	}	else 	{
		SelectedItem = SelectTableMapAndWindow(N);
		if (SelectedItem == k_undefined) {
			FailedToConnectProbablyBecauseAllTablesAlreadyServed();
		}	else {
			write_log(preferences.debug_autoconnector(), "[CAutoConnector] Window [%d] selected\n", g_tlist[SelectedItem].hwnd);
			p_sharedmem->MarkPokerWindowAsAttached(g_tlist[SelectedItem].hwnd);
			write_log(preferences.debug_autoconnector(), "[CAutoConnector] Window marked at shared memory\n");

			// Load correct tablemap, and save hwnd/rect/numchairs of table that we are "attached" to
			set_attached_hwnd(g_tlist[SelectedItem].hwnd);
			assert(p_tablemap != NULL);
			CString tablemap_to_load = g_tlist[SelectedItem].path.GetString();
			write_log(preferences.debug_autoconnector(), "[CAutoConnector] Selected tablemap: %s\n", tablemap_to_load);
			p_tablemap->LoadTablemap(tablemap_to_load);
			write_log(preferences.debug_autoconnector(), "[CAutoConnector] Tablemap loaded\n");

			// Create bitmaps
			p_scraper->CreateBitmaps();
			write_log(preferences.debug_autoconnector(), "[CAutoConnector] Scraper-bitmaps created\n");

			// Clear scraper fields
			p_table_state->Reset();
			write_log(preferences.debug_autoconnector(), "[CAutoConnector] Table state cleared\n");

			// Reset symbols
			p_engine_container->ResetOnConnection();

			write_log(preferences.debug_autoconnector(), "[CAutoConnector] ResetOnConnection executed (during connection)\n");
			write_log(preferences.debug_autoconnector(), "[CAutoConnector] Going to continue with scraper output and scraper DLL\n");

			// Reset "ScraperOutput" dialog, if it is live
			if (GUI->p_ScraperOutputDlg()) {
				GUI->p_ScraperOutputDlg()->Reset();
			}

			LoadScraperDLL();
			GUI->p_flags_toolbar()->ResetButtonsOnConnect();

			// Send "connect" and HWND to scraper DLL, if loaded
			if (theApp._dll_scraper_process_message) {
				(theApp._dll_scraper_process_message) ("connect", &_attached_hwnd);
      }

			p_table_state->_SCI.InitOnConnect();
			// Start timer that checks for continued existence of attached HWND
			PMainframe()->StartTimer();

			// Reset display
			PMainframe()->ResetDisplay();

			// log OH title bar text and table reset
			::GetWindowText(_attached_hwnd, title, MAX_WINDOW_TITLE);
      WriteLogTableReset();

			p_table_positioner->PositionMyWindow();
			p_autoplayer->EngageAutoPlayerUponConnectionIfNeeded();
		}
	}
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Unlocking autoconnector-mutex\n");
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
		OH_MessageBox_Error_Warning(error_message);
		return;
	}

	theApp._dll_scraper_process_message = (scraper_process_message_t) GetProcAddress(theApp._scraper_dll, "ProcessMessage");
	theApp._dll_scraper_override = (scraper_override_t) GetProcAddress(theApp._scraper_dll, "OverrideScraper");

	if (theApp._dll_scraper_process_message==NULL || theApp._dll_scraper_override==NULL)
	{
		OH_MessageBox_Error_Warning("Unable to find all symbols in scraper.dll");
		theApp.UnloadScraperDLL();
	}
	else
	{
			write_log(preferences.debug_autoconnector(), "[CAutoConnector] scraper.dll (%s) loaded, ProcessMessage and OverrideScraper found.\n", filename);
	}
}

void CAutoConnector::Disconnect() {
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Disconnect()\n");

  // First close scraper-output-dialog,
  // as an updating dialog without a connected table can crash.
  CDlgScraperOutput::DestroyWindowSafely();

	// Wait for mutex - "forever" if necessary, as we have to clean up.
	ASSERT(_autoconnector_mutex->m_hObject != NULL); 
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Locking autoconnector-mutex\n");
	_autoconnector_mutex->Lock(INFINITE);

	// Make sure autoplayer is off
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Stopping autoplayer\n");
	p_autoplayer->EngageAutoplayer(false);

	p_engine_container->ResetOnDisconnection();

	// Send "disconnect" to scraper DLL, if loaded
	if (theApp._dll_scraper_process_message)
		(theApp._dll_scraper_process_message) ("disconnect", NULL);

	theApp.UnloadScraperDLL();

	// Clear "attached" info
	set_attached_hwnd(NULL);

	// Stop timer that checks for valid hwnd, then unattach OH.
	PMainframe()->KillTimer();
	GUI->p_flags_toolbar()->UnattachOHFromPokerWindow();
	GUI->p_flags_toolbar()->ResetButtonsOnDisconnect();

	// Mark table as not attached
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Marking table as not attached\n");
	p_sharedmem->MarkPokerWindowAsUnAttached();

	// Release mutex as soon as possible, after critical work is done
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Unlocking autoconnector-mutex\n");
	_autoconnector_mutex->Unlock();	

	// Delete bitmaps
	p_scraper->DeleteBitmaps();

	// Clear scraper fields
	p_table_state->Reset();

	// Reset symbols
	p_engine_container->ResetOnConnection();

	write_log(preferences.debug_autoconnector(), "[CAutoConnector] ResetOnConnection executed (disconnection)\n");
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Going to continue with window title\n");

	// Change window title
	GUI->p_openholdem_title()->UpdateTitle();

	// Reset Display 
	PMainframe()->ResetDisplay();

	// Reset "ScraperOutput" dialog, if it is live
	if (GUI->p_ScraperOutputDlg())	{
		GUI->p_ScraperOutputDlg()->Reset();
	}
	WriteLogTableReset();

	// Close OH, when table disappears and leaving enabled in preferences.
	if (preferences.autoconnector_close_when_table_disappears()) {
		PostQuitMessage(0);
	}
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] Disconnect done\n");
}


int CAutoConnector::SelectTableMapAndWindow(int Choices)
{
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] SelectTableMapAndWindow(..)\n");
	// Always connecting automatically
	return SelectTableMapAndWindowAutomatically(Choices);
}


int CAutoConnector::SelectTableMapAndWindowAutomatically(int Choices)
{
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] SelectTableMapAndWindowAutomatically(..)\n");
	for (int i=0; i<Choices; i++) 
	{
		if (!p_sharedmem->PokerWindowAttached(g_tlist[i].hwnd))
		{
			write_log(preferences.debug_autoconnector(), "[CAutoConnector] Chosen (table, TM)-pair in list: %d\n", i);
			return i;
		}
	}
	// No appropriate table found
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] No appropriate table found.\n");
	return -1;
}

double CAutoConnector::TimeSincelast_failed_attempt_to_connect()
{
	time_t last_failed_attempt_to_connect = p_sharedmem->GetTimeOfLastFailedAttemptToConnect(); 
	time_t CurrentTime;
	time(&CurrentTime);
	double _TimeSincelast_failed_attempt_to_connect = difftime(CurrentTime, last_failed_attempt_to_connect);
	write_log(preferences.debug_autoconnector(), "[CAutoConnector] TimeSincelast_failed_attempt_to_connect %f\n", _TimeSincelast_failed_attempt_to_connect);
	return _TimeSincelast_failed_attempt_to_connect;
}
