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
#include "CSymbolEngineChipAmounts.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CScrapedActionInterface.h"
#include "CSymbolEngineTableLimits.h"
#include "CSymbolEngineUserchair.h"
#include "CTableState.h"
#include "FloatingPoint_Comparisions.h"
#include "NumericalFunctions.h"

CSymbolEngineChipAmounts *p_symbol_engine_chip_amounts = NULL;

CSymbolEngineChipAmounts::CSymbolEngineChipAmounts()
{
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(SYM->p_symbol_engine_tablelimits() != NULL);
	assert(SYM->p_symbol_engine_userchair() != NULL);
}

CSymbolEngineChipAmounts::~CSymbolEngineChipAmounts()
{}

void CSymbolEngineChipAmounts::InitOnStartup()
{
	ResetOnConnection();
}

void CSymbolEngineChipAmounts::ResetOnConnection()
{
	ResetOnHandreset();

	_maxbalance = k_undefined_zero;
	_balanceatstartofsession = k_undefined_zero;
}

void CSymbolEngineChipAmounts::ResetOnHandreset() {
	for (int i=0; i<k_max_number_of_players; i++)	{
		_stack[i]      = 0;
		_currentbet[i] = 0;
		_stacks_at_hand_start[i] = 0;
		_stacks_at_hand_start[i] = 0;
		_stacks_at_hand_start[i] = p_table_state->_players[i]._balance 
      + p_table_state->_players[i]._bet;
	}	
  _pot = 0;
	_potplayer = 0;
	_potcommon = 0;
  _call = 0;
  _nbetstocall = 0.0;
  _nbetstorais = 0.0;
  _ncallbets = 0.0;
  _nraisbets = 0.0;
}

void CSymbolEngineChipAmounts::ResetOnNewRound() {
}

void CSymbolEngineChipAmounts::ResetOnMyTurn() {
  SetBalanceAtStartOfSessionConditionally();
  SetMaxBalanceConditionally();
}

void CSymbolEngineChipAmounts::ResetOnHeartbeat() {
	CalculateStacks();
	CalculateCurrentbets();
	CalculatePots();
	CalculateBetsToCallToRaise();
	CalculateAmountsToCallToRaise();
}

void CSymbolEngineChipAmounts::SetMaxBalanceConditionally() { 
  double user_balance = p_table_state->User()->_balance;
	if (user_balance > _maxbalance) {
		_maxbalance = user_balance;
	}
}

void CSymbolEngineChipAmounts::SetBalanceAtStartOfSessionConditionally() {
  double user_balance = p_table_state->User()->_balance;
	if ((_balanceatstartofsession <= 0) && (user_balance > 0)) {
		_balanceatstartofsession = user_balance;
	}
}

void CSymbolEngineChipAmounts::CalculateStacks()
{
	// simple bubble sort for 10 stack values
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		if (p_table_state->_players[i].HasAnyCards())
		{
      _stack[i] = p_table_state->_players[i]._balance;
		}
		else
		{
			_stack[i] = 0;
		}
	}
	for (int i=0; i<p_tablemap->nchairs()-1; i++)
	{
		for (int j=i+1; j<p_tablemap->nchairs(); j++)
		{
			if (_stack[i] < _stack[j])
			{
				SwapDoubles(&_stack[i], &_stack[j]);
			}
		}
	}
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		assert(_stack[i] >= 0);									
	}
	for (int i=p_tablemap->nchairs(); i<k_max_number_of_players; i++)
	{
		_stack[i] = 0;
	}
}

void CSymbolEngineChipAmounts::CalculateCurrentbets()
{
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		_currentbet[i] = p_table_state->_players[i]._bet;
    assert(_currentbet[i] >= 0.0);
	}
}

void CSymbolEngineChipAmounts::CalculatePots() {
	_pot = 0;
	_potplayer = 0;
	_potcommon = 0;
	for (int i=0; i<p_tablemap->nchairs(); i++) {
    assert(_currentbet[i] >= 0.0);
		_potplayer += _currentbet[i];	
	}
  assert(_potplayer >= 0.0);
	// pot, potcommon, based on value of potmethod
	if (p_tablemap->potmethod() == 2)	{
		_pot = p_table_state->_pot[0];
		_potcommon = _pot - _potplayer;
	}
	else if(p_tablemap->potmethod() == 3) {
		_pot = p_table_state->_pot[0];
		for (int i=1; i<k_max_number_of_pots; i++) {
			_pot = max(_pot, p_table_state->_pot[i]);
		}
		_potcommon = _pot - _potplayer;
	} else { // potmethod() == 1
		_potcommon = 0;
		for (int i=0; i<k_max_number_of_pots; i++) {
			_potcommon += p_table_state->_pot[i];
		}
		_pot = _potcommon + _potplayer;
	}
  // Avoiding problems with floatingpoint-representations here
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=110&t=17984#p125241
  if (IsSmaller(_potcommon, 0)) {
	  // This can happen for potmethod = 2 and incorrectly scraped (occluded) main-pot
	  write_log(k_always_log_errors, "[CSymbolEngineChipAmounts] ERROR: negative potcommon. Probably miss-scraped main-pot. Adapting to 0.0\n");
	  _potcommon = 0;
  }
}

void CSymbolEngineChipAmounts::CalculateAmountsToCallToRaise() 
{
	int	next_largest_bet = 0;
	double largest_bet = Largestbet();

	if (SYM->p_symbol_engine_userchair()->userchair_confirmed()) {
		_call = largest_bet - _currentbet[USER_CHAIR];
	} else {
		_call = 0;
	}
  write_log(theApp.p_preferences()->debug_symbolengine(),
    "[CSymbolEngineChipAmounts] call = %.2f\n", _call);
  // In case we are covered consider only the effective amount to call,
  // but onlz if our balance is reasonable.
  double balance = p_table_state->User()->_balance;
  if ((_call > balance) && (balance > 0)) {
    _call = balance;
  }
	next_largest_bet = 0;
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		if (_currentbet[i] != largest_bet 
			&& _currentbet[i] > next_largest_bet)
		{
			next_largest_bet = _currentbet[i];
		}
	}
	_sraiprev = largest_bet - next_largest_bet;			
}

void CSymbolEngineChipAmounts::CalculateBetsToCallToRaise() {
	double bet = SYM->p_symbol_engine_tablelimits()->bet();
	if (bet <= 0) {
    // Fail-safe for the case of not being connected 
    // and completely bogus input
    bet = 1.00;
	}
  double users_currentbet = 0;
	if (SYM->p_symbol_engine_userchair()->userchair_confirmed())	{
		_nbetstocall = _call / bet;	
    users_currentbet = _currentbet[USER_CHAIR];
	} else {
    _nbetstocall = 0;
  }
  _nbetstorais = _nbetstocall + 1;
  assert(bet > 0.0);
  assert(users_currentbet >= 0);
  assert(_call >= 0);
  // Computation below works even without playing user,
  // which might be important for table-stats.
	_ncallbets = Largestbet() / bet;				
	_nraisbets = _ncallbets + 1;	// fixed limit
  assert(_ncallbets >= 0.0);
}

double CSymbolEngineChipAmounts::Largestbet()
{
	double largest_bet = 0.0;
	for (int i=0; i<p_tablemap->nchairs(); i++)
	{
		if (_currentbet[i] > largest_bet) 
		{
			largest_bet = _currentbet[i];
		}
	}
	return largest_bet;
}

double CSymbolEngineChipAmounts::SortedBalance(const int rank) {
  assert(rank >= 0);
  assert(rank < k_max_number_of_players);
	double	stacks[k_max_number_of_players];
  for (int i=0; i<k_max_number_of_players; ++i) {
    stacks[i] = _currentbet[i] + p_table_state->_players[i]._balance;
  }
	// bubble sort stacks // !! duplicate code?
	for (int i=0; i<(k_max_number_of_players-1); ++i)	{
		for (int n=i+1; n<k_max_number_of_players; ++n)	{
			if (stacks[i] < stacks[n]) {
				SwapDoubles(&stacks[i], &stacks[n]);
			}
		}
	}
  return stacks[rank];
}

double CSymbolEngineChipAmounts::ncurrentbets() {
	if (SYM->p_symbol_engine_tablelimits()->bet() == 0)		{
		return 0;
	}
	return (currentbet(SYM->p_symbol_engine_userchair()->userchair()) / SYM->p_symbol_engine_tablelimits()->bet());
}

bool CSymbolEngineChipAmounts::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name, "pot", 3)==0) {
		// CHIP AMOUNTS 1(2)
		if (memcmp(name, "pot", 3)==0 && strlen(name)==3)	{
			*result = pot();
		}	else if (memcmp(name, "potcommon", 9)==0 && strlen(name)==9) {
			*result = potcommon();
		}	else if (memcmp(name, "potplayer", 9)==0 && strlen(name)==9) {
			*result = potplayer();
		}	else {
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}	else if (memcmp(name, "balance", 7)==0)	{
		if (memcmp(name, "balance", 7)==0 && strlen(name)==7)	{
			*result = p_table_state->User()->_balance; 
		}	else if (memcmp(name, "balance", 7)==0 && strlen(name)==8) {
			*result = p_table_state->_players[name[7]-'0']._balance;
		}	else if (memcmp(name, "balanceatstartofsession", 23)==0 && strlen(name)==23) {
			*result = balanceatstartofsession();
		} else if (memcmp(name, "balance_rank", 12)==0 && strlen(name)==13) {
      *result = SortedBalance(name[12]-'0');
    }	else {
			// Invalid symbol
			return false;
		}
		// Valid symbol
		return true;
	}
	if (memcmp(name, "maxbalance", 10)==0 && strlen(name)==10) {
		*result = maxbalance();
	}	else if (memcmp(name, "stack", 5)==0 && strlen(name)==6) {
		*result = stack(name[5]-'0');
	}	else if (memcmp(name, "currentbet", 10)==0 && strlen(name)==10)	{
		*result = currentbet(SYM->p_symbol_engine_userchair()->userchair());
	}	else if (memcmp(name, "currentbet", 10)==0 && strlen(name)==11)	{
		*result = currentbet(name[10]-'0');
	}	else if (memcmp(name, "call", 4)==0 && strlen(name)==4)	{
		*result = call();
	}	else if (memcmp(name, "nbetstocall", 11)==0 && strlen(name)==11) {
		*result = nbetstocall();
	}	else if (memcmp(name, "nbetstorais", 11)==0 && strlen(name)==11) {
		*result = nbetstorais();
	}	else if (memcmp(name, "ncurrentbets", 12)==0 && strlen(name)==12)	{
		*result = ncurrentbets();
	}	else if (memcmp(name, "ncallbets", 9)==0 && strlen(name)==9) {
		*result = ncallbets();
	}	else if (memcmp(name, "nraisbets", 9)==0 && strlen(name)==9) {
		*result = nraisbets();
	}	else {
		// Symbol of a different symbol-engine
		return false;
	}
	// Valid symbol
	return true;
}

CString CSymbolEngineChipAmounts::SymbolsProvided() {
  CString list = "pot potcommon potplayer "
    "balance balanceatstartofsession maxbalance "
    "currentbet call nbetstocall nbetstorais "
    "ncurrentbets ncallbets nraisbets ";
  list += RangeOfSymbols("balance%i", k_first_chair, k_last_chair);
  list += RangeOfSymbols("balance_rank%i", k_betround_preflop, k_betround_river);
  list += RangeOfSymbols("currentbet%i", k_first_chair, k_last_chair);
  list += RangeOfSymbols("stack%i", k_first_chair, k_last_chair);
  return list;
}