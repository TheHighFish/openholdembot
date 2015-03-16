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
#include "Singletons.h"

#include "CCasinoInterface.h"
#include "CAutoConnector.h"
#include "CAutoplayer.h"
#include "CAutoplayerFunctions.h"
#include "CAutoplayerTrace.h"
#include "CConfigurationCheck.h"
#include "CDebugTab.h"
#include "CDllExtension.h"
#include "CEngineContainer.h"
#include "CFilenames.h"
#include "CFileSystemMonitor.h"
#include "CFormulaParser.h"
#include "CGameState.h"
#include "CHandresetDetector.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "CLazyScraper.h"
#include "COcclusioncheck.h"
#include "COpenHoldemTitle.h"
#include "CParserSymbolTable.h"
#include "CPokerTrackerThread.h"
#include "CPopupHandler.h"
#include "CPreferences.h"
#include "CRebuyManagement.h"
#include "CScraper.h"
#include "CScrapedActionInterface.h"
#include "CSessionCounter.h"
#include "CSharedMem.h"
#include "CStableFramesCounter.h"
#include "CStringMatch.h"
#include "CSymbolEngineTableLimits.h"
#include "CTableMaploader.h"
#include "CTablePositioner.h"
#include "CTableState.h"
#include "CValidator.h"
#include "CVersionInfo.h"
#include "CWhiteInfoBox.h"

void InstantiateAllSingletons() {
  // Instantiation of all singletons, except session-counter.
  // session-counter has to be done do earlier, as it is needed 
  // to create the log-file, which might be needed before this point.
  // This function gets executed exactly once at startup.
  // So the global class-pointers have to be NULL.
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CStringMatch\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CAutoplayerTrace\n");
  assert(!p_autoplayer_trace);
  p_autoplayer_trace = new CAutoplayerTrace;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CTableState\n");
  assert(!p_table_state);
  p_table_state = new CTableState;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CHandresetDetector\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CAutoplayerFunctions\n");
  assert(!p_autoplayer_functions);
  p_autoplayer_functions = new CAutoplayerFunctions;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CSharedMem\n");
  assert(!p_sharedmem);
  p_sharedmem = new CSharedMem;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CStableFramesCounter\n");
  assert(!p_stableframescounter);
  p_stableframescounter = new CStableFramesCounter;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CScraper\n");
  assert(!p_scraper); 
  p_scraper = new CScraper;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CTablemap\n");
  assert(!p_tablemap);
  p_tablemap = new CTablemap;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CTablemapAccess\n");
  assert(!p_tablemap_access);
  p_tablemap_access = new CTablemapAccess;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CDebugTab\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CFormulaParser\n");
  assert(!p_formula_parser);
  p_formula_parser = new CFormulaParser;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CAutoplayer\n");
  assert(!p_autoplayer);
  p_autoplayer = new CAutoplayer();
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CPokerTrackerThread\n");
  assert(!p_pokertracker_thread);
  p_pokertracker_thread = new CPokerTrackerThread;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CDllExtension\n");
  assert(!p_dll_extension);
  p_dll_extension = new CDllExtension;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CGameState\n");
  assert(!p_game_state);
  p_game_state = new CGameState;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CAutoConnector\n");
  assert(!p_autoconnector);
  p_autoconnector = new CAutoConnector;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CEngineContainer\n");
  assert(!p_engine_container);
  p_engine_container = new CEngineContainer;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CRebuyManagement\n");
  assert(!p_rebuymanagement); 
  p_rebuymanagement = new CRebuyManagement;
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Going to create CCasinoInterface\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] All singletons created.\n");
}

bool all_threads_stopped = false;

// To be executed first,
// as these threads might access some variables.
void StopThreads()
{
	write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] StopThreads()\n");
	if (p_heartbeat_thread)
	{
		write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting heartbeat-thread\n");
		delete p_heartbeat_thread;
		p_heartbeat_thread = NULL;
	}
	if (p_iterator_thread) 
	{
		write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting iterator-thread\n");
		delete p_iterator_thread;
		p_iterator_thread = NULL;
	}
	if (p_pokertracker_thread)	
	{ 
		write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting PokerTracker-thread\n");
		delete p_pokertracker_thread; 
		p_pokertracker_thread = NULL; 
	}
	all_threads_stopped = true;
}

#define DELETE_AND_CLEAR(object_pointer) if (object_pointer)	{ delete object_pointer; object_pointer = NULL; }

void DeleteAllSingletons() {
  // First all threads have to be stopped, then all singletons can (have to) be deleted
  // StopThreads gets called by CMainFrame::DestroyWindow()
  // DeleteAllSingletons() by COpenHoldemApp::ExitInstance()
  // Correct order should be guaranteed, because of
  //   "At the time ExitInstance is called, the main window no longer exists"
  //   (http://computer-programming-forum.com/82-mfc/7ad0828fdb127d7b.htm)
  // AnyWay: we make sure with an assertion
  assert(all_threads_stopped);
  // Global instances.
  // Releasing in reverse order should be good,
  // but we have to be careful, as sometimes we do some work in the destructors,
  // that depends on other classes, e.g. the destructor of the autoconnector
  // needs its session_id (CSessionCounter).
  //
  DELETE_AND_CLEAR(p_rebuymanagement)
  DELETE_AND_CLEAR(p_engine_container)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting autoconnector\n");
  DELETE_AND_CLEAR(p_autoconnector)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting version_info\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting tablemap loader\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 01\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 02\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 03\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 05\n");
  DELETE_AND_CLEAR(p_game_state)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 06\n");
  DELETE_AND_CLEAR(p_dll_extension)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 07\n");
  DELETE_AND_CLEAR(p_autoplayer)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 08\n");
  DELETE_AND_CLEAR(p_formula_parser)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 10\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 11\n");
  DELETE_AND_CLEAR(p_tablemap_access)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 12\n");
  DELETE_AND_CLEAR(p_tablemap)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 13\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 15\n");
  DELETE_AND_CLEAR(p_scraper)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 16\n");
  DELETE_AND_CLEAR(p_stableframescounter)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 17\n");
  DELETE_AND_CLEAR(p_sharedmem)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 18\n");
  DELETE_AND_CLEAR(p_sessioncounter)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 19\n");
  DELETE_AND_CLEAR(p_autoplayer_functions)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 20\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 21\n");
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 22\n");
  DELETE_AND_CLEAR(p_autoplayer_trace)
  write_log(theApp.p_preferences()->debug_alltherest(), "[Singletons] Deleting 23\n");
  DELETE_AND_CLEAR(p_table_state)
}
  
  


