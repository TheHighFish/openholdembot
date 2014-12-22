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

#include "stdafx.h"
#include "CSymbolEngineTableStats.h"

#include "CGameState.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CStringMatch.h"
#include "MagicNumbers.h"

CSymbolEngineTableStats *p_symbol_engine_table_stats = NULL;

CSymbolEngineTableStats::CSymbolEngineTableStats()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
  //
  // This symbol-engine does not depend on other symbol-engines
  // but on CGameState.
  // Be careful if you turn CGameState into a symbol-engine!!
}

CSymbolEngineTableStats::~CSymbolEngineTableStats()
{}

void CSymbolEngineTableStats::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineTableStats::ResetOnConnection()
{}

void CSymbolEngineTableStats::ResetOnHandreset()
{}

void CSymbolEngineTableStats::ResetOnNewRound()
{}

void CSymbolEngineTableStats::ResetOnMyTurn()
{}

void CSymbolEngineTableStats::ResetOnHeartbeat()
{}

double CSymbolEngineTableStats::FlopTurnRiverPct(int betround) {
  int	elapsed_start = 0, num_dealt = 0, hands = 0, num_saw_cards = 0;

  assert(betround >= k_betround_flop);
  assert(betround >= k_betround_river);
	if (p_game_state->_m_ftr_ndx>1) {
		for (int i=p_game_state->_m_ftr_ndx-1; i>=1; i--)	{
			if (p_game_state->_m_ftr[i&0xff].elapsed_time != 0 && p_game_state->_m_ftr[i&0xff].n_pl_dealt > 1) {
				if (elapsed_start == 0) {
					elapsed_start = p_game_state->_m_ftr[i&0xff].elapsed_time;
        }
				if (elapsed_start-p_game_state->_m_ftr[i&0xff].elapsed_time<k_time_frame_for_table_stats_in_seconds) 	{
					num_dealt+=p_game_state->_m_ftr[i&0xff].n_pl_dealt;
					++hands;
          switch (betround) {
            case k_betround_flop:
              num_saw_cards += p_game_state->_m_ftr[i&0xff].n_pl_saw_flop;
              break;
            case k_betround_turn:
              num_saw_cards += p_game_state->_m_ftr[i&0xff].n_pl_saw_turn;
              break;
            case k_betround_river:
              num_saw_cards += p_game_state->_m_ftr[i&0xff].n_pl_saw_river;
              break;
            default:
              assert(k_this_must_not_happen);
          }
				}	else {
					break;
				}
			}
		}
	}
	if (num_dealt != 0) {
		return (double) num_saw_cards/(double) num_dealt;
  }	
	return k_undefined_zero;
}

double CSymbolEngineTableStats::AvgBetsPf() {
	int		 elapsed_start = 0, num_dealt = 0, hands = 0, num_saw_cards = 0;
	double bets_preflop = 0.;

	if (p_game_state->_m_ftr_ndx>1)	{
		for (int i=p_game_state->_m_ftr_ndx-1; i>=1; i--)	{
			if (p_game_state->_m_ftr[i&0xff].elapsed_time!=0 && p_game_state->_m_ftr[i&0xff].n_pl_dealt>1) 	{
				if (elapsed_start==0) {
					elapsed_start=p_game_state->_m_ftr[i&0xff].elapsed_time;
        }
        if (elapsed_start-p_game_state->_m_ftr[i&0xff].elapsed_time<k_time_frame_for_table_stats_in_seconds) {
					num_dealt+=p_game_state->_m_ftr[i&0xff].n_pl_dealt;
					hands++;
					bets_preflop+=p_game_state->_m_ftr[i&0xff].n_bets_preflop;
				}	else {
					break;
				}
			}
		}
	}

	if (num_dealt!=0) {
		return (double) bets_preflop/(double) hands;
  }
	return k_undefined_zero;
}

double CSymbolEngineTableStats::TablePfr() {
	int	elapsed_start = 0, num_dealt = 0, hands = 0, num_saw_cards = 0, bets_preflop = 0;

	if (p_game_state->_m_ftr_ndx>1) 	{
		for (int i=p_game_state->_m_ftr_ndx-1; i>=1; i--)		{
			if (p_game_state->_m_ftr[i&0xff].elapsed_time!=0 && p_game_state->_m_ftr[i&0xff].n_pl_dealt>1) 	{
				if (elapsed_start==0) {
					elapsed_start=p_game_state->_m_ftr[i&0xff].elapsed_time;
        }
				if (elapsed_start-p_game_state->_m_ftr[i&0xff].elapsed_time<k_time_frame_for_table_stats_in_seconds) {
					num_dealt+=p_game_state->_m_ftr[i&0xff].n_pl_dealt;
					hands++;
          // Either there was more than one raise or no one saw a flop (hence a raise)
					if (p_game_state->_m_ftr[i&0xff].n_bets_preflop > 1 || p_game_state->_m_ftr[i&0xff].n_pl_saw_flop == 0) {
						bets_preflop++;
          }
				}	else {
					break;
				}
			}
		}
	}
	if (num_dealt!=0) {
		return (double) bets_preflop/(double) hands;
  }
	return k_undefined_zero;
}

bool CSymbolEngineTableStats::EvaluateSymbol(const char *name, double *result) {
  if (memcmp(name, "floppct", 7)==0 && strlen(name)==7)  				*result = FlopTurnRiverPct(k_betround_flop);
  else if (memcmp(name, "turnpct", 7)==0 && strlen(name)==7)  	*result = FlopTurnRiverPct(k_betround_turn);
  else if (memcmp(name, "riverpct", 8)==0 && strlen(name)==8)  	*result = FlopTurnRiverPct(k_betround_river);
  else if (memcmp(name, "avgbetspf", 9)==0 && strlen(name)==9)  *result = AvgBetsPf();
  else if (memcmp(name, "tablepfr", 8)==0 && strlen(name)==8)  	*result = TablePfr();	
  else {
	  // Symbol of a different symbol-engine
	  return false;
  }
  return true;
}

CString CSymbolEngineTableStats::SymbolsProvided() {
  return "floppct turnpct riverpct avgbetspf tablepfr ";
}