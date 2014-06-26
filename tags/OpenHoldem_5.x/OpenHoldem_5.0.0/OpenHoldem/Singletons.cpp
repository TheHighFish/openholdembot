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
#include "CHandHistory.h"
#include "CHandresetDetector.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "CLazyScraper.h"
#include "COcclusioncheck.h"
#include "COpenHoldemTitle.h"
#include "CParserSymbolTable.h"
#include "CPerl.hpp"
#include "CPokerTrackerThread.h"
#include "CPopupHandler.h"
#include "CPreferences.h"
#include "CRebuyManagement.h"
#include "CReplayFramesCounter.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSessionCounter.h"
#include "CSharedMem.h"
#include "CStableFramesCounter.h"
#include "CStringMatch.h"
#include "CSymbolEngineTableLimits.h"
#include "CTableMaploader.h"
#include "CTablePositioner.h"
#include "CValidator.h"
#include "CVersionInfo.h"

void InstantiateAllSingletons() {
  // Instantiation of all singletons, except session-counter.
  // session-counter has to be done do earlier, as it is needed 
  // to create the log-file, which might be needed before this point.
    // This function gets executed exactly once at startup.
  // So the global class-pointers have to be NULL.
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CStringMatch\n");
  assert(!p_string_match);
  p_string_match = new CStringMatch;
    write_log(preferences.debug_alltherest(), "[Singletons] Going to create CAutoplayerTrace\n");
    assert(!p_autoplayer_trace);
    p_autoplayer_trace = new CAutoplayerTrace;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CHandresetDetector\n");
  assert(!p_handreset_detector);
  p_handreset_detector = new CHandresetDetector;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CConfigurationCheck\n");
  assert(!p_configurationcheck);
  p_configurationcheck = new CConfigurationCheck;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CAutoplayerFunctions\n");
  assert(!p_autoplayer_functions);
  p_autoplayer_functions = new CAutoplayerFunctions;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CSharedMem\n");
  assert(!p_sharedmem);
  p_sharedmem = new CSharedMem;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CStableFramesCounter\n");
  assert(!p_stableframescounter);
  p_stableframescounter = new CStableFramesCounter;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CScraper\n");
  assert(!p_scraper); 
  p_scraper = new CScraper;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CScraperAccess\n");
  assert(!p_scraper_access);
  p_scraper_access = new CScraperAccess;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CLazyScraper\n");
  assert(!p_lazyscraper);
  p_lazyscraper = new CLazyScraper;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CTablemap\n");
  assert(!p_tablemap);
  p_tablemap = new CTablemap;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CTablemapAccess\n");
  assert(!p_tablemap_access);
  p_tablemap_access = new CTablemapAccess;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CReplayFramesCounter\n");
  assert(!p_replayframes_counter);
  p_replayframes_counter = new CReplayFramesCounter;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CDebugTab\n");
  assert(p_debug_tab == NULL);
  p_debug_tab = new CDebugTab;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CParserSymbolTable\n");
  assert(!p_parser_symbol_table);
  p_parser_symbol_table = new CParserSymbolTable;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CFormulaParser\n");
  assert(!p_formula_parser);
  p_formula_parser = new CFormulaParser;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CAutoplayer\n");
  assert(!p_autoplayer);
  p_autoplayer = new CAutoplayer();
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CPokerTrackerThread\n");
  assert(!p_pokertracker_thread);
  p_pokertracker_thread = new CPokerTrackerThread;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CDllExtension\n");
  assert(!p_dll_extension);
  p_dll_extension = new CDllExtension;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CGameState\n");
  assert(!p_game_state);
  p_game_state = new CGameState;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CPerl\n");
  assert(!p_perl);
  p_perl = new CPerl;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CValidator\n");
  assert(!p_validator);
  p_validator = new CValidator;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CTablePositioner\n");
  assert(!p_table_positioner);
  p_table_positioner = new CTablePositioner;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CFileSystemMonitor\n");
  assert(!p_filesystem_monitor);
  p_filesystem_monitor = new CFileSystemMonitor;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CTableMapLoader\n");
  assert(!p_tablemap_loader);
  p_tablemap_loader = new CTableMapLoader;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CVersionInfo\n");
  assert(!p_version_info);
  p_version_info = new CVersionInfo;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CPopupHandler\n");
  assert(!p_popup_handler);
  p_popup_handler = new CPopupHandler;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CAutoConnector\n");
  assert(!p_autoconnector);
  p_autoconnector = new CAutoConnector;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create COpenHoldemTitle\n");
  assert(!p_openholdem_title);
  p_openholdem_title = new COpenHoldemTitle;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CEngineContainer\n");
  assert(!p_engine_container);
  p_engine_container = new CEngineContainer;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CRebuyManagement\n");
  assert(!p_rebuymanagement); 
  p_rebuymanagement = new CRebuyManagement;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create COcclusionCheck\n");
  assert(!p_occlusioncheck);
  p_occlusioncheck = new COcclusionCheck;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create HandHistory\n");
  assert(!p_handhistory);
  p_handhistory = new CHandHistory;
  write_log(preferences.debug_alltherest(), "[Singletons] Going to create CCasinoInterface\n");
  assert(!p_casino_interface);
  p_casino_interface = new CCasinoInterface;
  write_log(preferences.debug_alltherest(), "[Singletons] All singletons created.\n");
}

void InstantiateSomeSingletonsForVeryEarlyUseInInitInstance()
{
	// Filenames have to be available very early,
	// even before we read the ini-file.
	assert(!p_filenames);
	p_filenames = new CFilenames;
}

bool all_threads_stopped = false;

// To be executed first,
// as these threads might access some variables.
void StopThreads()
{
	write_log(preferences.debug_alltherest(), "[Singletons] StopThreads()\n");
	if (p_heartbeat_thread)
	{
		write_log(preferences.debug_alltherest(), "[Singletons] Deleting heartbeat-thread\n");
		delete p_heartbeat_thread;
		p_heartbeat_thread = NULL;
	}
	if (p_iterator_thread) 
	{
		write_log(preferences.debug_alltherest(), "[Singletons] Deleting iterator-thread\n");
		delete p_iterator_thread;
		p_iterator_thread = NULL;
	}
	if (p_pokertracker_thread)	
	{ 
		write_log(preferences.debug_alltherest(), "[Singletons] Deleting PokerTracker-thread\n");
		delete p_pokertracker_thread; 
		p_pokertracker_thread = NULL; 
	}
	all_threads_stopped = true;
}

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
  if (p_casino_interface)
	{ delete p_casino_interface; p_casino_interface = NULL; }
  if (p_handhistory) 
	{ delete p_handhistory; p_handhistory = NULL; }
  if (p_occlusioncheck) 
	{ delete p_occlusioncheck; p_occlusioncheck = NULL; }
  if (p_rebuymanagement) 
	{ delete p_rebuymanagement; p_rebuymanagement = NULL; }
  if (p_engine_container)
	{ delete p_engine_container; p_engine_container = NULL; }
  if (p_openholdem_title)
	{ delete p_openholdem_title; p_openholdem_title = NULL; }
  if (p_popup_handler)
	{ delete p_popup_handler; p_popup_handler = NULL; }
  if (p_autoconnector) 
  { 
    write_log(preferences.debug_alltherest(), "[Singletons] Deleting autoconnector\n");
	  delete p_autoconnector; 
	  p_autoconnector = NULL; 
  }
  if (p_version_info)
	{ delete p_version_info; p_version_info = NULL; }
  if (p_tablemap_loader)
	{ delete p_tablemap_loader; p_tablemap_loader = NULL; }
  if (p_filesystem_monitor)
	{ delete p_filesystem_monitor; p_filesystem_monitor = NULL; }
  if (p_table_positioner)
	{ delete p_table_positioner; p_table_positioner = NULL; }
  if (p_validator) 
	{ delete p_validator; p_validator = NULL; }
  if (p_perl)  
	{ delete p_perl; p_perl = NULL; }
  if (p_game_state)  
	{ delete p_game_state; p_game_state = NULL; }
  if (p_dll_extension)  
	{ delete p_dll_extension; p_dll_extension = NULL; }
  if (p_autoplayer)  
	{ delete p_autoplayer; p_autoplayer = NULL; }
  if (p_formula_parser)
	{ delete p_formula_parser; p_formula_parser = NULL; }
  if (p_parser_symbol_table)
    { delete p_parser_symbol_table; p_parser_symbol_table = NULL; }
  if (p_debug_tab) 
    { delete p_debug_tab; p_debug_tab = NULL; }
  if (p_replayframes_counter)
	{ delete p_replayframes_counter; p_replayframes_counter = NULL; }
  if (p_tablemap_access)  
	{ delete p_tablemap_access; p_tablemap_access = NULL; }
  if (p_tablemap)  
	{ delete p_tablemap; p_tablemap = NULL; }
  if (p_lazyscraper)	        
	{ delete p_lazyscraper; p_lazyscraper = NULL; }
  if (p_scraper_access)       
	{ delete p_scraper_access; p_scraper_access = NULL; }
  if (p_scraper)              
	{ delete p_scraper; p_scraper = NULL; }
  if (p_stableframescounter)  
	{ delete p_stableframescounter; p_stableframescounter = NULL; }
  if (p_sharedmem)            
	{ delete p_sharedmem; p_sharedmem = NULL; }
  if (p_sessioncounter)       
	{ delete p_sessioncounter; p_sessioncounter = NULL; }
  if (p_autoplayer_functions) 
	{ delete p_autoplayer_functions; p_autoplayer_functions = NULL; }
  if (p_configurationcheck)   
	{ delete p_configurationcheck; p_configurationcheck = NULL; }
  if (p_handreset_detector)   
	{ delete p_handreset_detector; p_handreset_detector = NULL; }
  if (p_autoplayer_trace)
    { delete p_autoplayer_trace; p_autoplayer_trace = NULL; }
  if (p_string_match)         
	{delete p_string_match; p_string_match = NULL; }
  if (p_filenames)
	{ delete p_filenames; p_filenames = NULL; }
}

