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

/*
	   Author	:	spud
	   Date		:	2007-MAR-08

	  Notation: P(i,n)=probability that player i finishes in nth place
	  P(w|z)=probability that w occurs given that z occurs
	  I=i's stack
	  T=SUM[I]

	  P(i,1)=I/T
	  P(i,n)=SUM(j!=i)[P(j,1)*P(i,n|j,1)]

	  ICM assumes that P(i,n|j,1) = (P(i,n-1) in a game where j has been removed)
*/

#include "stdafx.h"
#include "CSymbolEngineICM.h"

#include "CFunctionCollection.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSymbolEngineTableLimits.h"
#include "..\CTablemap\CTablemap.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineBlinds.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineUserchair.h"
#include "CTableState.h"
#include "MagicNumbers.h"
#include "NumericalFunctions.h"
#include "OH_MessageBox.h"

CSymbolEngineICM *p_symbol_engine_icm = NULL;

CSymbolEngineICM::CSymbolEngineICM() {
  // The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	assert(p_symbol_engine_active_dealt_playing != NULL);
  assert(p_symbol_engine_blinds != NULL);
  assert(p_symbol_engine_chip_amounts != NULL);
  assert(p_symbol_engine_dealerchair != NULL);
  assert(p_symbol_engine_userchair != NULL);
}

CSymbolEngineICM::~CSymbolEngineICM() {
}

void CSymbolEngineICM::InitOnStartup() {
}

void CSymbolEngineICM::ResetOnConnection() {
}

void CSymbolEngineICM::ResetOnHandreset() {
}

void CSymbolEngineICM::ResetOnNewRound() {
}

void CSymbolEngineICM::ResetOnMyTurn() {
}

void CSymbolEngineICM::ResetOnHeartbeat() {
}

double P(int i, int n, double *s, int N)
{
	double p = 0.;

	if (n==1)
	{
		double I = s[i];
		double T = 0;

		for (int k=0; k < N; k++) T+=s[k];

		p = (T > 0.) ? I / T : 0.;
	}
	else
	{
		for (int j=0; j < N; j++)
		{
			if (j != i)
			{
				double *ss = (double *) calloc(N-1,sizeof(double));
				int ii = (i > j) ? i-1 : i;
				memcpy(ss, s, j * sizeof(double));
				memcpy(ss + j, s + (j + 1), (N - j -1) * sizeof(double));

				p += P(j, 1, s, N) * P(ii, n-1, ss, N-1);

				free(ss);
			}
		}
	}
	return p;
}

int CSymbolEngineICM::GetChairFromDealPos(const char* name)
{
	int	sym_playersseatedbits =	p_symbol_engine_active_dealt_playing->playersseatedbits();
	int	sym_nplayersseated =	p_symbol_engine_active_dealt_playing->nplayersseated();
	int	sym_dealerchair =		p_symbol_engine_dealerchair->dealerchair();
	int	sym_nplayersblind =		p_symbol_engine_blinds->nplayersblind();
	int	chair = -1, sb_offset = 1, hu_offset = 0, eb_offset = 1;

	if (sym_playersseatedbits&k_exponents[sym_dealerchair])
		eb_offset = 0;
		
	// if 2 players posted blinds, no sb_offset
	if (sym_nplayersblind == 2)
		sb_offset = 0;

	else if (sym_nplayersblind < 2)
	{
		for (int i=sym_dealerchair+1; i<=sym_dealerchair+p_tablemap->nchairs(); i++)
		{
			int next_chair = i%p_tablemap->nchairs();
			double p_bet = p_scraper->player_bet(next_chair);

			if (p_bet > 0 && p_bet <= p_symbol_engine_tablelimits->sblind())
				sb_offset = 0;
		}
	}

	// If only 2 players active, we are HU.
	if (sym_nplayersseated == 2 && eb_offset == 0)
	{
		sb_offset = 0;
		hu_offset = 1;
	}
	// If empty button reset other possible offsets
	if (eb_offset == 1)
	{
		sb_offset = 0;
		hu_offset = 0;
	}

	if (sym_nplayersseated > 0)
	{
		int dealPos = -1;

		if (strcmp(name,"SB")==0)
		{
			if (sb_offset == 0) dealPos = 1 - eb_offset - hu_offset;
		}
		else if (strcmp(name,"BB")==0)
			dealPos = 2 - eb_offset - sb_offset - hu_offset;
		else if (strcmp(name,"UTG")==0)
			dealPos = 3 - eb_offset - sb_offset - hu_offset;
		else if (strcmp(name,"UTG1")==0)
			dealPos = 4 - eb_offset - sb_offset - hu_offset;
		else if (strcmp(name,"UTG2")==0)
			dealPos = 5 - eb_offset - sb_offset - hu_offset;
		else if (strcmp(name,"UTG3")==0)
			dealPos = 6 - eb_offset - sb_offset - hu_offset;
		else if (strcmp(name,"UTG4")==0)
			dealPos = 7 - eb_offset - sb_offset - hu_offset;
		else if (strcmp(name,"UTG5")==0)
			dealPos = 8 - eb_offset - sb_offset - hu_offset;
		else if (strcmp(name,"UTG6")==0)
			dealPos = 9 - eb_offset - sb_offset - hu_offset;
		else if (strcmp(name,"D")==0)
		{
			if (eb_offset == 0) dealPos = 0;
		}
		else if (strcmp(name,"CO")==0)
			dealPos = sym_nplayersseated - 1 - eb_offset;

		if (dealPos >= 0)
		{
			chair = sym_dealerchair;
			while (dealPos >= ( eb_offset == 0 ? 1 : 0 ))
			{
				chair = (chair + 1) % k_max_number_of_players;
				if (IsBitSet(sym_playersseatedbits, chair))
					dealPos--;
			}
		}
	}

	return chair;
}

double CSymbolEngineICM::EquityICM(double *stacks, double *prizes, int playerNB, int player)
{
	double ICM = 0.0;

	int			sym_opponentsseatedbits = p_symbol_engine_active_dealt_playing->opponentsseatedbits();

	for (int i=0; i<playerNB; ++i) {
    write_log(preferences.debug_icm(),
      "[CSymbolEngineICM] player %d  stack = %1.2f \n", i, stacks[i]);
	}

	// Degenerate case when the player's stack is zero.  Place
	// him after all opponents who had stacks at the start of this
	// hand.  This will not tie-break if two players bust on the
	// same hand.

	if (stacks[player] == 0.)
	{
		int place = 0;

		for (int i = 0; i < k_max_number_of_players; i++)
		{
			if (IsBitSet(sym_opponentsseatedbits, i))
			{
				place++;
			}
		}

		return prizes[place];

	}

	int i = 0;
	while (i < playerNB && prizes[i] > 0.) {
		double p = P(player, i + 1, stacks, playerNB);
		write_log(preferences.debug_icm(),
      "[CSymbolEngineICM] prob place %d = %1.4f \n", i + 1, p);
		ICM += prizes[i] * p;
		i++;
	}

	return ICM;
}

bool CSymbolEngineICM::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  // Fast exit on other symbols
  int	sym_userchair = p_symbol_engine_userchair->userchair();
  if (memcmp(name, "icm", 3) != 0) return false;
	if (sym_userchair == k_undefined) {
		*result = 0.0;
    return true;
	}

  double		prizes[k_max_number_of_players] = {0};
	double		stacks[k_max_number_of_players] = {0};

  int number_of_icm_prizes = k_icm_prize5 - k_icm_prize1 + 1;
  double sum_of_prizes = 0.0;
  for (int i=0; i<number_of_icm_prizes; ++i) {
    int function_name_index = k_icm_prize1 + i;
    prizes[i] = p_function_collection->Evaluate(
      k_standard_function_names[function_name_index]);
    sum_of_prizes += prizes[i];
  }
  // Sanity check: sum of prizes should be ~1.00
  if ((sum_of_prizes <= 0) || (sum_of_prizes > 1)) {
    CString error_message;
    error_message.Format("Incorrect f$icm_prizeX-functions.\n"
      "The sum of all values should be in the ranhe (0..1].\n"
      "Current value: %.3f", sum_of_prizes);
    OH_MessageBox_Error_Warning(error_message);
    *result = 0.0;
    return true;
  }

	int			sym_opponentsplayingbits = p_symbol_engine_active_dealt_playing->opponentsplayingbits();
	int			sym_nopponentsplaying = p_symbol_engine_active_dealt_playing->nopponentsplaying();
	int			sym_nplayersseated = p_symbol_engine_active_dealt_playing->nplayersseated();
	int			sym_playersseatedbits = p_symbol_engine_active_dealt_playing->playersseatedbits();
	double	sym_pot = p_symbol_engine_chip_amounts->pot();
	double	sym_call = p_symbol_engine_chip_amounts->call();
	double	sym_currentbet[k_max_number_of_players]={0};

	for (int i = 0; i < k_max_number_of_players; i++)
	{
		if (IsBitSet(sym_playersseatedbits, i))
		{
			stacks[i] = p_table_state->_players[i]._balance;
			sym_currentbet[i] = p_symbol_engine_chip_amounts->currentbet(i);
		}
	}

	if (strncmp(name,"icm_fold",8)==0)
	{
		double to_split = p_symbol_engine_chip_amounts->potcommon();

		for (int i = 0; i < k_max_number_of_players; i++)
		{
			if (IsBitSet(sym_opponentsplayingbits, i))
				stacks[i] += sym_currentbet[i];
			else
				to_split += sym_currentbet[i];
		}

		double win = to_split / sym_nopponentsplaying;

		for (int i = 0; i < k_max_number_of_players; i++)
		{
			if (IsBitSet(sym_opponentsplayingbits, i))
				stacks[i] += win;
		}
	}

	else if (strncmp(name,"icm_callwin",11)==0)
	{
		double call = sym_call;

		if (stacks[sym_userchair] < call)
		{
			double myTotalBet = sym_currentbet[sym_userchair] + stacks[sym_userchair];

			for (int i = 0; i < k_max_number_of_players; i++)
			{
				if (IsBitSet(sym_opponentsplayingbits, i) && 
					myTotalBet < sym_currentbet[i])
				{
					double extra = sym_currentbet[i] - myTotalBet;

					stacks[i] += extra;
					sym_pot -= extra;
				}
			}
		}
		stacks[sym_userchair] += sym_pot;
	}

	else if (strncmp(name,"icm_calllose",12)==0)
	{
		double mycall = min(sym_call,stacks[sym_userchair]);
		double win = (sym_pot + mycall) / sym_nopponentsplaying;

		stacks[sym_userchair] -= mycall;

		for (int i = 0; i < k_max_number_of_players; i++)
		{
			if (IsBitSet(sym_opponentsplayingbits, i))
			{
				stacks[i] += win;
			}
		}
	}

	else if (strncmp(name,"icm_calltie",11)==0)
	{
		double win = 0.;

		if (stacks[sym_userchair] < sym_call)
		{
			double myTotalBet = sym_currentbet[sym_userchair] + stacks[sym_userchair];

			for (int i = 0; i < k_max_number_of_players; i++)
			{
				if (IsBitSet(sym_opponentsplayingbits, i) 
					&& myTotalBet < sym_currentbet[i])
				{
					double extra = sym_currentbet[i] - myTotalBet;

					stacks[i] += extra;
					sym_pot -= extra;
				}
			}
		}
		sym_pot += min(sym_call, stacks[sym_userchair]);
		win = sym_pot / (sym_nopponentsplaying +1);
		stacks[sym_userchair] += win;
		for (int i = 0; i < k_max_number_of_players; i++)
		{
			if (IsBitSet(sym_opponentsplayingbits, i))
			{
				stacks[i] += win;
			}
		}
	}

	else if (strncmp(name,"icm_alliwin",11)==0)
	{
		if (isdigit(name[11]))
		{
			//assume callers are n smallest stacks
			bool callers[k_max_number_of_players] = {0};
			int ncallers = min(name[11]-'0', sym_nopponentsplaying);


			for (int i = 0; i < ncallers; i++)
			{
				int jsmallest = -1;
				double smalleststack = DBL_MAX;

				for (int j = 0; j < k_max_number_of_players; j++)
				{
					if (IsBitSet(sym_opponentsplayingbits, j))
					{
						if (!callers[j] && (sym_currentbet[j] + stacks[j]) < smalleststack)
						{
							smalleststack = sym_currentbet[j] + stacks[j];
							jsmallest = j;
						}
					}
				}
				if (jsmallest > -1)
				{
					double oppCurrentBet = sym_currentbet[jsmallest];
					double myTotalBet = sym_currentbet[sym_userchair] + stacks[sym_userchair];
					double extra = ((oppCurrentBet + stacks[jsmallest]) > myTotalBet) ? myTotalBet - oppCurrentBet :stacks[jsmallest];
					stacks[jsmallest] -= extra;
					callers[jsmallest] = true;
					sym_pot+=extra;
				}
			}
			stacks[sym_userchair]+=sym_pot;
		}
		else
		{
			//assume only one particular caller
			int oppChair = GetChairFromDealPos(name + 8);
			if (oppChair == k_undefined || oppChair == sym_userchair) {
        *result = k_undefined;
        return true;
      }
			if (oppChair >= 0)
			{
				double oppCurrentBet = sym_currentbet[oppChair];
				double myTotalBet = sym_currentbet[sym_userchair] + stacks[sym_userchair];
				double extra = ((oppCurrentBet + stacks[oppChair]) > myTotalBet) ? myTotalBet - oppCurrentBet :stacks[oppChair];

				stacks[oppChair] -= extra;
				stacks[sym_userchair]+= (sym_pot + extra);
			}
		}
	}

	else if (strncmp(name,"icm_allilose",12)==0)
	{
		if (isdigit(name[12]))
		{
			//assume callers are n biggest stacks
			int ncallers = min(name[12]-'0', sym_nopponentsplaying);

			if (ncallers > 0)
			{
				bool callers[k_max_number_of_players] = {0};
				int *biggest =(int *) calloc(ncallers, sizeof(int));
				double *sidepots = (double *)calloc(ncallers, sizeof(double));
				double mybet = 0.;

				memset(sidepots,0, ncallers * sizeof(double));

				for (int i = 0; i < ncallers; i++)
				{
					int jbiggest = -1;
					double biggeststack = 0.;

					for (int j = 0; j < k_max_number_of_players; j++)
					{
						if (IsBitSet(sym_opponentsplayingbits, j))
						{
							if (!callers[j] && (sym_currentbet[j] + stacks[j]) > biggeststack)
							{
								biggeststack = sym_currentbet[j] + stacks[j];
								jbiggest = j;
							}
						}
					}
					biggest[i] = jbiggest;
					if (jbiggest > -1)
					{
						callers[jbiggest] = true;
						stacks[jbiggest] += sym_currentbet[jbiggest];
						sym_pot -= sym_currentbet[jbiggest];
					}
				}
				stacks[sym_userchair] += sym_currentbet[sym_userchair];
				sym_pot -= sym_currentbet[sym_userchair];
				for (int i = (ncallers -1); i>=0; i--)
				{
					ASSERT(biggest[i] >= 0);
					sidepots[i] = min(stacks[biggest[i]], stacks[sym_userchair]) - mybet;
					mybet += sidepots[i];
				}
				stacks[sym_userchair] -= mybet;
				sidepots[ncallers - 1] += sym_pot;
				for (int i = 0; i<ncallers; i++)
				{
					double win = sidepots[i] / (i + 1);

					for (int j = 0; j<=i; j++)
					{
						ASSERT(biggest[j] >= 0);
						stacks[biggest[j]] += win;
					}
				}
				free(biggest);
				free(sidepots);
			}
		}
		else
		{
			//assume only one particular caller
			int oppChair = GetChairFromDealPos(name + 9);
			if (oppChair == k_undefined || oppChair == sym_userchair) {
        *result = k_undefined;
        return true;
      }
			if (oppChair >= 0)
			{
				double oppTotalBet = sym_currentbet[oppChair] + stacks[oppChair];
				double myTotalBet = sym_currentbet[sym_userchair] + stacks[sym_userchair];
				double extra = (oppTotalBet > myTotalBet) ? stacks[sym_userchair] : oppTotalBet - sym_currentbet[sym_userchair];

				stacks[oppChair]+= (sym_pot + extra);
				stacks[sym_userchair]-= extra;
			}
		}
	}

	else if (strncmp(name,"icm_allitie",11)==0)
	{
		//assume only one particular caller
		int oppChair = GetChairFromDealPos(name + 8);
		if (oppChair == k_undefined || oppChair == sym_userchair) {
      *result = k_undefined;
      return true;
    }
		if (oppChair >= 0)
		{
			stacks[oppChair]+= sym_pot / 2;
			stacks[sym_userchair]+= sym_pot / 2;
		}
	}

	else if(strcmp(name,"icm")==0)
	{
		for (int i = 0; i < k_max_number_of_players; i++)
		{
			if (IsBitSet(sym_playersseatedbits, i))
			{
				stacks[i] = p_symbol_engine_chip_amounts->stacks_at_hand_start(i);
			}
	   }
	}
  else
  {
    // Not a valid symbol
    return false;
  } 

	*result = EquityICM(stacks, prizes, k_max_number_of_players, sym_userchair);
  return true;
}

CString CSymbolEngineICM::SymbolsProvided() {
  return "icm "
    "icm_fold icm_callwin icm_calllose icm_calltie "
    // Winning against 0..9 opponents
    "icm_alliwin0 icm_alliwin1 icm_alliwin2 icm_alliwin3 icm_alliwin4 "
    "icm_alliwin5 icm_alliwin6 icm_alliwin7 icm_alliwin8 icm_alliwin9 "
    // Losing against 1..9 opponents
    "icm_allilose1 cm_allilose2 cm_allilose3 cm_allilose4 cm_allilose5 "
    "cm_allilose6 cm_allilose7 cm_allilose8 cm_allilose9 "
    // Wining allin against a particular opponent
    "icm_alliwinSB icm_alliwinBB icm_alliwinUTG icm_alliwinUTG1 "
    "icm_alliwinUTG2 icm_alliwinUTG3 icm_alliwin4 icm_alliwin5 "
    "icm_alliwinUTG6 icm_alliwinCO icm_alliwinD "
    // Tieing allin against a particular opponent
    "icm_allitieSB icm_allitieBB icm_allitieUTG icm_allitieUTG1 "
    "icm_allitieUTG2 icm_allitieUTG3 icm_allitie4 icm_allitie5 "
    "icm_allitieUTG6 icm_allitieCO icm_allitieD "
    // Losing allin against a particular opponent
    "icm_allilosSB icm_allilosBB icm_allilosUTG icm_allilosUTG1 "
    "icm_allilosUTG2 icm_allilosUTG3 icm_allilos4 icm_allilos5 "
    "icm_allilosUTG6 icm_allilosCO icm_allilosD ";
}

