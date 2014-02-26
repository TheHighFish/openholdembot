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
#include "CPokerAction.h"

#include "CBetroundCalculator.h"
#include "CGameState.h"
#include "CPokerTrackerLookUp.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEnginePositions.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbolEngineTableLimits.h"

CPokerAction::CPokerAction()
{
}

CPokerAction::~CPokerAction()
{
}

const double CPokerAction::ProcessQuery(const char * pquery, int *e)
{
	if (memcmp(pquery,"ac_agchair_after", 16) == 0)			return AgchairAfter();
	if (memcmp(pquery,"ac_preflop_pos", 14) == 0)			return PreflopPos();
	if (memcmp(pquery,"ac_prefloprais_pos", 18) == 0)		return PreflopRaisPos();
	if (memcmp(pquery,"ac_postflop_pos", 15) == 0)			return PostflopPos();
	if (memcmp(pquery,"ac_first_into_pot", 17) == 0)		return FirstIntoPot();
	if (memcmp(pquery,"ac_betpos", 9) == 0)					return BetPosition(pquery[9]-'0');
	if (memcmp(pquery,"ac_dealpos", 10) == 0)				return DealPosition(pquery[10]-'0');

	*e = ERR_INVALID_SYM;
	return 0.0;
}

const int CPokerAction::PreflopPos (void)
{
	int		sym_nplayersdealt = p_symbol_engine_active_dealt_playing->nplayersdealt();
	int		sym_dealposition  = p_symbol_engine_positions->dealposition();
	

	//SB=1 BB=2 Early=3 Middle=4 Late=5 Dealer=6
	return
		sym_nplayersdealt==10 ? (sym_dealposition==1 ? 1 :
								 sym_dealposition==2 ? 2 :
								 sym_dealposition==3 ? 3 :
								 sym_dealposition==4 ? 3 :
								 sym_dealposition==5 ? 3 :
								 sym_dealposition==6 ? 4 :
								 sym_dealposition==7 ? 4 :
								 sym_dealposition==8 ? 5 :
								 sym_dealposition==9 ? 5 :
								 sym_dealposition==10 ? 6 : 0):
		sym_nplayersdealt==9  ? (sym_dealposition==1 ? 1 :
								 sym_dealposition==2 ? 2 :
								 sym_dealposition==3 ? 3 :
								 sym_dealposition==4 ? 3 :
								 sym_dealposition==5 ? 4 :
								 sym_dealposition==6 ? 4 :
								 sym_dealposition==7 ? 5 :
								 sym_dealposition==8 ? 5 :
								 sym_dealposition==9 ? 6 : 0):
		sym_nplayersdealt==8  ? (sym_dealposition==1 ? 1 :
								 sym_dealposition==2 ? 2 :
								 sym_dealposition==3 ? 3 :
								 sym_dealposition==4 ? 4 :
								 sym_dealposition==5 ? 4 :
								 sym_dealposition==6 ? 5 :
								 sym_dealposition==7 ? 5 :
								 sym_dealposition==8 ? 6 : 0):
		sym_nplayersdealt==7  ? (sym_dealposition==1 ? 1 :
								 sym_dealposition==2 ? 2 :
								 sym_dealposition==3 ? 4 :
								 sym_dealposition==4 ? 4 :
								 sym_dealposition==5 ? 5 :
								 sym_dealposition==6 ? 5 :
								 sym_dealposition==7 ? 6 : 0):
		sym_nplayersdealt==6  ? (sym_dealposition==1 ? 1 :
								 sym_dealposition==2 ? 2 :
								 sym_dealposition==3 ? 4 :
								 sym_dealposition==4 ? 5 :
								 sym_dealposition==5 ? 5 :
								 sym_dealposition==6 ? 6 : 0):
		sym_nplayersdealt==5  ? (sym_dealposition==1 ? 1 :
								 sym_dealposition==2 ? 2 :
								 sym_dealposition==3 ? 5 :
								 sym_dealposition==4 ? 5 :
								 sym_dealposition==5 ? 6 : 0):
		sym_nplayersdealt==4  ? (sym_dealposition==1 ? 1 :
								 sym_dealposition==2 ? 2 :
								 sym_dealposition==3 ? 5 :
								 sym_dealposition==4 ? 6 : 0):
		sym_nplayersdealt==3  ? (sym_dealposition==1 ? 1 :
								 sym_dealposition==2 ? 2 :
								 sym_dealposition==3 ? 6 : 0):
		sym_nplayersdealt==2  ? (
								// reversed blinds - dealer is BB
								// Even worse code. Looks like a code-clone
								(pt_lookup.GetSiteId()==11) ?
								(sym_dealposition==1 ? 1 :
								sym_dealposition==2 ? 6 : 0 ) :
								// Normal blinds - dealposition==1 is BB
								(sym_dealposition==1 ? 2 :
								 sym_dealposition==2 ? 6 : 0)) :0;
}

const int CPokerAction::PreflopRaisPos (void)
{
	int		e = SUCCESS;
	int		sym_nplayersdealt    = p_symbol_engine_active_dealt_playing->nplayersdealt();
	int		sym_dealpositionrais = p_symbol_engine_positions->dealpositionrais();
	
	//SB=1 BB=2 Early=3 Middle=4 Late=5 Dealer=6
	return
		sym_nplayersdealt==10 ? (sym_dealpositionrais==1 ? 1 :
								sym_dealpositionrais==2 ? 2 :
								sym_dealpositionrais==3 ? 3 :
								sym_dealpositionrais==4 ? 3 :
								sym_dealpositionrais==5 ? 3 :
								sym_dealpositionrais==6 ? 4 :
								sym_dealpositionrais==7 ? 4 :
								sym_dealpositionrais==8 ? 5 :
								sym_dealpositionrais==9 ? 5 :
								sym_dealpositionrais==10 ? 6 : 0):
		sym_nplayersdealt==9  ? (sym_dealpositionrais==1 ? 1 :
								sym_dealpositionrais==2 ? 2 :
								sym_dealpositionrais==3 ? 3 :
								sym_dealpositionrais==4 ? 3 :
								sym_dealpositionrais==5 ? 4 :
								sym_dealpositionrais==6 ? 4 :
								sym_dealpositionrais==7 ? 5 :
								sym_dealpositionrais==8 ? 5 :
								sym_dealpositionrais==9 ? 6 : 0):
		sym_nplayersdealt==8  ? (sym_dealpositionrais==1 ? 1 :
								sym_dealpositionrais==2 ? 2 :
								sym_dealpositionrais==3 ? 3 :
								sym_dealpositionrais==4 ? 4 :
								sym_dealpositionrais==5 ? 4 :
								sym_dealpositionrais==6 ? 5 :
								sym_dealpositionrais==7 ? 5 :
								sym_dealpositionrais==8 ? 6 : 0):
		sym_nplayersdealt==7  ? (sym_dealpositionrais==1 ? 1 :
								sym_dealpositionrais==2 ? 2 :
								sym_dealpositionrais==3 ? 4 :
								sym_dealpositionrais==4 ? 4 :
								sym_dealpositionrais==5 ? 5 :
								sym_dealpositionrais==6 ? 5 :
								sym_dealpositionrais==7 ? 6 : 0):
		sym_nplayersdealt==6  ? (sym_dealpositionrais==1 ? 1 :
								sym_dealpositionrais==2 ? 2 :
								sym_dealpositionrais==3 ? 4 :
								sym_dealpositionrais==4 ? 5 :
								sym_dealpositionrais==5 ? 5 :
								sym_dealpositionrais==6 ? 6 : 0):
		sym_nplayersdealt==5  ? (sym_dealpositionrais==1 ? 1 :
								sym_dealpositionrais==2 ? 2 :
								sym_dealpositionrais==3 ? 5 :
								sym_dealpositionrais==4 ? 5 :
								sym_dealpositionrais==5 ? 6 : 0):
		sym_nplayersdealt==4  ? (sym_dealpositionrais==1 ? 1 :
								sym_dealpositionrais==2 ? 2 :
								sym_dealpositionrais==3 ? 5 :
								sym_dealpositionrais==4 ? 6 : 0):
		sym_nplayersdealt==3  ? (sym_dealpositionrais==1 ? 1 :
								sym_dealpositionrais==2 ? 2 :
								sym_dealpositionrais==3 ? 6 : 0):
		sym_nplayersdealt==2  ? (
								// reversed blinds - dealer is BB
								// !! Bad code. We should get rid of hard-coded constants
								// !! does also not work with new PT3 constants
								(pt_lookup.GetSiteId()==11) ?
								(sym_dealpositionrais==1 ? 1 :
								sym_dealpositionrais==2 ? 6 : 0 ) :
								// Normal blinds - dealposition==1 is BB
								(sym_dealpositionrais==1 ? 2 :
								sym_dealpositionrais==2 ? 6 : 0)) :0;
}

const int CPokerAction::PostflopPos (void)
{
	int		e = SUCCESS;
	int		sym_nplayersplaying = p_symbol_engine_active_dealt_playing->nplayersplaying();
	int		sym_betposition     = p_symbol_engine_positions->betposition();

	//first=1 early=2 middle=3 late=4 last=5
	return
		sym_nplayersplaying==10 ? (sym_betposition==1 ? 1 :
									sym_betposition==2 ? 2 :
									sym_betposition==3 ? 2 :
									sym_betposition==4 ? 2 :
									sym_betposition==5 ? 3 :
									sym_betposition==6 ? 3 :
									sym_betposition==7 ? 3 :
									sym_betposition==8 ? 4 :
									sym_betposition==9 ? 4 :
									sym_betposition==10 ? 5 : 0):
		sym_nplayersplaying==9  ? (sym_betposition==1 ? 1 :
									sym_betposition==2 ? 2 :
									sym_betposition==3 ? 2 :
									sym_betposition==4 ? 3 :
									sym_betposition==5 ? 3 :
									sym_betposition==6 ? 3 :
									sym_betposition==7 ? 4 :
									sym_betposition==8 ? 4 :
									sym_betposition==9 ? 5 : 0):
		sym_nplayersplaying==8  ? (sym_betposition==1 ? 1 :
									sym_betposition==2 ? 2 :
									sym_betposition==3 ? 2 :
									sym_betposition==4 ? 3 :
									sym_betposition==5 ? 3 :
									sym_betposition==6 ? 4 :
									sym_betposition==7 ? 4 :
									sym_betposition==8 ? 5 : 0):
		sym_nplayersplaying==7  ? (sym_betposition==1 ? 1 :
									sym_betposition==2 ? 2 :
									sym_betposition==3 ? 3 :
									sym_betposition==4 ? 3 :
									sym_betposition==5 ? 4 :
									sym_betposition==6 ? 4 :
									sym_betposition==7 ? 5 : 0):
		sym_nplayersplaying==6  ? (sym_betposition==1 ? 1 :
									sym_betposition==2 ? 2 :
									sym_betposition==3 ? 3 :
									sym_betposition==4 ? 4 :
									sym_betposition==5 ? 4 :
									sym_betposition==6 ? 5 : 0):
		sym_nplayersplaying==5  ? (sym_betposition==1 ? 1 :
									sym_betposition==2 ? 2 :
									sym_betposition==3 ? 3 :
									sym_betposition==4 ? 4 :
									sym_betposition==5 ? 5 : 0):
		sym_nplayersplaying==4  ? (sym_betposition==1 ? 1 :
									sym_betposition==2 ? 3 :
									sym_betposition==3 ? 4 :
									sym_betposition==4 ? 5 : 0):
		sym_nplayersplaying==3  ? (sym_betposition==1 ? 1 :
									sym_betposition==2 ? 4 :
									sym_betposition==3 ? 5 : 0):
		sym_nplayersplaying==2  ? (sym_betposition==1 ? 1 :
									sym_betposition==2 ? 5 : 0): 0;
}

const bool CPokerAction::FirstIntoPot (void)
{

	if (p_betround_calculator->betround() == k_betround_preflop)
	{
		return (p_symbol_engine_chip_amounts->potplayer() <= p_symbol_engine_tablelimits->sblind() + p_symbol_engine_tablelimits->bblind()); 
	}
	else
	{
		return (p_symbol_engine_chip_amounts->potplayer() == 0);
	}
}

const int CPokerAction::BetPosition (const int chairnum)
{

	int		i = 0;
	int		betpos = 0;
	int		e = SUCCESS;
	int		sym_dealerchair        = p_symbol_engine_dealerchair->dealerchair();
	int		sym_playersplayingbits = p_symbol_engine_active_dealt_playing->playersplayingbits();

	if (chairnum<0 || chairnum>9)
		return betpos;

	if (!(IsBitSet(sym_playersplayingbits, chairnum)))
		return betpos;

	for (int i=sym_dealerchair+1; i<=sym_dealerchair+10; i++)
	{
		if (IsBitSet(sym_playersplayingbits, (i%10)))
			betpos++;

		if (i%10==chairnum)
			i=99;
	}

	return betpos;
}

const int CPokerAction::DealPosition (const int chairnum)
{
	int		dealposchair = 0 ;
	int		e = SUCCESS;
	int		sym_dealerchair      = p_symbol_engine_dealerchair->dealerchair();
	int		sym_playersdealtbits = p_symbol_engine_active_dealt_playing->playersdealtbits();

	if (chairnum<0 || chairnum>9)
		return dealposchair;

	for (int i=sym_dealerchair+1; i<=sym_dealerchair+10; i++)
	{
		if (IsBitSet(sym_playersdealtbits, (i%10)))
			dealposchair++;

		if (i%10==chairnum)
			break;
	}
	return (IsBitSet(sym_playersdealtbits, chairnum)) ? dealposchair : 0 ;
}

const bool CPokerAction::AgchairAfter (void)
{
	int		e = SUCCESS;

	if (!p_symbol_engine_userchair->userchair_confirmed())
	{
		return false;
	}
	if (p_symbol_engine_raisers_callers->raischair() >=0 )
	{
		return (BetPosition(p_symbol_engine_raisers_callers->raischair()) > p_symbol_engine_positions->betposition());
	}
	else
	{
		return false;
	}
}
