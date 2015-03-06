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

#ifndef INC_CENGINECONTAINER_H
#define INC_CENGINECONTAINER_H

#include "CVirtualSymbolEngine.h"

const int k_max_number_of_symbol_engines = 100;

// Forward declaration to avoid including lots of header files
class CSymbolEngineTableLimits;
class CSymbolEngineActiveDealtPlaying;
class CSymbolEngineAutoplayer;
class CSymbolEngineBlinds;
class CSymbolEngineCards;
class CSymbolEngineCasino;
class CSymbolEngineChairs;
class CSymbolEngineChipAmounts;
class CSymbolEngineColourCodes;
class CSymbolEngineDealerchair;
class CSymbolEngineEventLogging;
class CSymbolEngineGameType;
class CSymbolEngineHandrank;
class CSymbolEngineICM;
class CSymbolEngineHistory;
class CSymbolEngineIniFunctions;
class CSymbolEngineIsOmaha;
class CSymbolEngineIsTournament;
class CSymbolEngineMemorySymbols;
class CSymbolEngineMTTInfo;
class CSymbolEngineOpenPPL;
class CSymbolEngineOpenPPLHandAndBoardExpression;
class CSymbolEngineOpenPPLUserVariables;
class CSymbolEnginePokerAction;
class CSymbolEnginePokerTracker;
class CSymbolEnginePokerval;
class CSymbolEnginePositions;
class CSymbolEnginePrwin;
class CSymbolEngineRaisersCallers;
class CSymbolEngineRandom;
class CSymbolEngineReplayFrameController;
class CSymbolEngineTableStats;
class CSymbolEngineTime;
class CSymbolEngineUserchair;
class CSymbolEngineVariousDataLookup;
class CSymbolEngineVersus;
// Special symbol engines
class CFunctionCollection;
class CHandresetDetector;
// Forward declaration for hand-history modules
class CHandHistoryAction;   
class CHandHistoryDealPhase;    
class CHandHistoryShowdown;
class CHandHistoryUncontested;
class CHandHistoryWriter;       
class CHandresetDetector;   

class CEngineContainer {
 public:
  CEngineContainer();
  ~CEngineContainer();
 public:
  // both to be called by the auto-connector
  // future: to be create / destroyed on connection
  void ResetOnConnection();
  void ResetOnDisconnection();
 public:
  void EvaluateAll();
  bool EvaluateSymbol(const char *name, double *result, bool log = false);
  CString SymbolsProvided()       { BuildListOfSymbolsProvided(); return _list_of_symbols; }
 public:
  // Accessors for symbol engines
  CSymbolEngineActiveDealtPlaying* p_symbol_engine_active_dealt_playing();
  CSymbolEngineAutoplayer*         p_symbol_engine_autoplayer();
  CSymbolEngineBlinds*             p_symbol_engine_blinds();
  CSymbolEngineCards*              p_symbol_engine_cards();
  CSymbolEngineCasino*             p_symbol_engine_casino();
  CSymbolEngineChairs*             p_symbol_engine_chairs();
  CSymbolEngineChipAmounts*        p_symbol_engine_chip_amounts();
  CSymbolEngineColourCodes*        p_symbol_engine_colourcodes();
  CSymbolEngineDealerchair*        p_symbol_engine_dealerchair();
  CSymbolEngineEventLogging*       p_symbol_engine_event_logging();
  CSymbolEngineGameType*           p_symbol_engine_gametype();
  CSymbolEngineHandrank*           p_symbol_engine_handrank();
  CSymbolEngineICM*                p_symbol_engine_icm();
  CSymbolEngineHistory*            p_symbol_engine_history();
  CSymbolEngineIniFunctions*       p_symbol_engine_ini_functions();
  CSymbolEngineIsOmaha*            p_symbol_engine_isomaha();
  CSymbolEngineIsTournament*       p_symbol_engine_istournament();
  CSymbolEngineMemorySymbols*      p_symbol_engine_memory_symbols();
  CSymbolEngineMTTInfo*            p_symbol_engine_mtt_info();
  CSymbolEngineOpenPPL*            p_symbol_engine_open_ppl();
  CSymbolEngineOpenPPLHandAndBoardExpression* p_symbol_engine_open_ppl_hand_and_board_expression();
  CSymbolEngineOpenPPLUserVariables*          p_symbol_engine_openppl_user_variables();
  CSymbolEnginePokerAction*        p_symbol_engine_poker_action();
  CSymbolEnginePokerTracker*       p_symbol_engine_pokertracker();
  CSymbolEnginePokerval*           p_symbol_engine_pokerval();
  CSymbolEnginePositions*          p_symbol_engine_positions();
  CSymbolEnginePrwin*              p_symbol_engine_prwin();
  CSymbolEngineRaisersCallers*     p_symbol_engine_raisers_callers();
  CSymbolEngineRandom*             p_symbol_engine_random();
  CSymbolEngineReplayFrameController*         p_symbol_engine_replayframe_controller();
  CSymbolEngineTableLimits*        p_symbol_engine_tablelimits();
  CSymbolEngineTableStats*         p_symbol_engine_table_stats();
  CSymbolEngineTime*               p_symbol_engine_time();
  CSymbolEngineUserchair*          p_symbol_engine_userchair();
  CSymbolEngineVariousDataLookup*  p_symbol_engine_various_data_lookup();
  CSymbolEngineVersus*             p_symbol_engine_versus;
 private:
  void BuildListOfSymbolsProvided();
 private:
  void CreateSpecialSymbolEngines();
  void AddSymbolEngine(CVirtualSymbolEngine *new_symbol_engine);
  void CreateSymbolEngines();
  void DestroyAllSymbolEngines();
  void DestroyAllSpecialSymbolEngines();
 private:
  void ResetOnHandreset();
  void ResetOnNewRound();
  void ResetOnMyTurn();
  void ResetOnHeartbeat();
 private:
  CVirtualSymbolEngine *_symbol_engines[k_max_number_of_symbol_engines]; 
  int _number_of_symbol_engines_loaded;
  bool _reset_on_connection_executed;
  CString _list_of_symbols;
 private:
  // Symbol engines
  // Read-only access possible with public accessors
  CSymbolEngineActiveDealtPlaying *_p_symbol_engine_active_dealt_playing;
  CSymbolEngineAutoplayer *_p_symbol_engine_autoplayer;
  CSymbolEngineBlinds *_p_symbol_engine_blinds;
  CSymbolEngineCards *_p_symbol_engine_cards;
  CSymbolEngineCasino *_p_symbol_engine_casino;
  CSymbolEngineChairs *_p_symbol_engine_chairs;
  CSymbolEngineChipAmounts *_p_symbol_engine_chip_amounts;
  CSymbolEngineColourCodes *_p_symbol_engine_colourcodes;
  CSymbolEngineDealerchair *_p_symbol_engine_dealerchair;
  CSymbolEngineEventLogging *_p_symbol_engine_event_logging;
  CSymbolEngineGameType *_p_symbol_engine_gametype;
  CSymbolEngineHandrank *_p_symbol_engine_handrank;
  CSymbolEngineICM *_p_symbol_engine_icm;
  CSymbolEngineHistory *_p_symbol_engine_history;
  CSymbolEngineIniFunctions *_p_symbol_engine_ini_functions;
  CSymbolEngineIsOmaha *_p_symbol_engine_isomaha;
  CSymbolEngineIsTournament *_p_symbol_engine_istournament;
  CSymbolEngineMemorySymbols *_p_symbol_engine_memory_symbols;
  CSymbolEngineMTTInfo *_p_symbol_engine_mtt_info;
  CSymbolEngineOpenPPL *_p_symbol_engine_open_ppl;
  CSymbolEngineOpenPPLHandAndBoardExpression  *_p_symbol_engine_open_ppl_hand_and_board_expression;
  CSymbolEngineOpenPPLUserVariables           *_p_symbol_engine_openppl_user_variables;
  CSymbolEnginePokerAction                    *_p_symbol_engine_poker_action;
  CSymbolEnginePokerTracker                   *_p_symbol_engine_pokertracker;
  CSymbolEnginePokerval                       *_p_symbol_engine_pokerval;
  CSymbolEnginePositions                      *_p_symbol_engine_positions;
  CSymbolEnginePrwin                          *_p_symbol_engine_prwin;
  CSymbolEngineRaisersCallers                 *_p_symbol_engine_raisers_callers;
  CSymbolEngineRandom                         *_p_symbol_engine_random;
  CSymbolEngineReplayFrameController          *_p_symbol_engine_replayframe_controller;
  CSymbolEngineTableLimits                    *_p_symbol_engine_tablelimits;
  CSymbolEngineTableStats                     *_p_symbol_engine_table_stats;
  CSymbolEngineTime                           *_p_symbol_engine_time;
  CSymbolEngineUserchair                      *_p_symbol_engine_userchair;
  CSymbolEngineVariousDataLookup              *_p_symbol_engine_various_data_lookup;
  CSymbolEngineVersus                         *_p_symbol_engine_versus;
 private:
  // Special szmbol engines
  CFunctionCollection                         *_p_function_collection;
  CHandresetDetector                          *_p_handreset_detector;
 private:
  // Private modules for hand-history-hgeneration
  CHandHistoryAction       *_p_handhistory_action;
  CHandHistoryDealPhase    *_p_handhistory_deal_phase;
  CHandHistoryShowdown     *_p_handhistory_showdown;
  CHandHistoryUncontested  *_p_handhistory_uncontested;
  CHandHistoryWriter       *_p_handhistory_writer;
};

extern CEngineContainer *_p_engine_container;

// Abbreviation, otherwise the code becomes extremely long:
//   _p_engine_container->p_symbol_engine_dealerchai()->dealerchair()
// and even longer once the engine-containere is part of a table-object.
#define SYM _p_engine_container

#endif INC_CENGINECONTAINER_H