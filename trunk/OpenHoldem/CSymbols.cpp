#include "stdafx.h"
#include "CSymbols.h"

#include <assert.h>
#include <process.h>
#include <float.h>

#include "CAutoconnector.h"
#include "CAutoplayer.h"
#include "CAutoplayerFunctions.h"
#include "CBetroundCalculator.h"
#include "CFlagsToolbar.h"
#include "inlines/eval.h"
#include "CGameState.h"
#include "CGrammar.h"
#include "Chair$Symbols.h"
#include "CHandresetDetector.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "CIteratorVars.h"
#include "MainFrm.h"
#include "MagicNumbers.h"
#include "OpenHoldem.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSessionCounter.h"
#include "CStringMatch.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineBlinds.h"
#include "CSymbolEngineCards.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineHandrank.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEngineIsTournament.h"
#include "CSymbolEngineLists.h"
#include "CSymbolEnginePokerTracker.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEnginePositions.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbolEngineRaisersCallers.h"
#include "CSymbolEngineRandom.h"
#include "CSymbolEngineTime.h"
#include "CSymbolEngineUserchair.h"
#include "CTableLimits.h"
#include "..\CTablemap\CTablemap.h"
#include "..\CTransform\CTransform.h"
#include "CVersionInfo.h"
#include "CVersus.h"
#include "OH_MessageBox.h"

CSymbols			*p_symbols = NULL;


CSymbols::CSymbols()
{}

CSymbols::~CSymbols()
{}

void CSymbols::InitOnStartup()
{
	logsymbols_collection_removeall();
	symboltrace_collection_removeall();
}

void CSymbols::ResetOnConnection()
{}

void CSymbols::ResetOnHandreset()
{
	LogHandReset();
}


void CSymbols::ResetOnNewRound()
{}


void CSymbols::ResetOnMyTurn()
{
	// !! Better make it a symbol-engine
	p_versus->GetCounts();
}


void CSymbols::ResetOnHeartbeat()
{
	// log$ symbols
	logsymbols_collection_removeall();
	symboltrace_collection_removeall();
}


// !! CSymbols: not really the right place for that
void CSymbols::LogHandReset()
{
	// Update game_state so it knows that a new hand has happened
	p_game_state->set_new_hand(true);

	// Reset display
	InvalidateRect(theApp.m_pMainWnd->GetSafeHwnd(), NULL, true);

	int player_card_cur[2] = {0};
	player_card_cur[0] = p_scraper->card_player(p_symbol_engine_userchair->userchair(), 0);
	player_card_cur[1] = p_scraper->card_player(p_symbol_engine_userchair->userchair(), 1);
	char card0[10];
	char card1[10];
	// log new hand
	if (player_card_cur[0]==CARD_NOCARD || player_card_cur[1]==CARD_NOCARD)
	{
		strcpy_s(card0, 10, "NONE");
		strcpy_s(card1, 10, "");
	}
	else if (player_card_cur[0]==CARD_BACK || player_card_cur[1]==CARD_BACK)
	{
		strcpy_s(card0, 10, "BACKS");
		strcpy_s(card1, 10, "");
	}
	else
	{
		StdDeck_cardToString(player_card_cur[0], card0);
		StdDeck_cardToString(player_card_cur[1], card1);
	}
	char title[512] = "--- No title specified ---";
	GetWindowText(p_autoconnector->attached_hwnd(), title, 512);
	write_log(k_always_log_basic_information, 
		"\n"
		"*************************************************************\n"
		"%s\n" 
		"HAND RESET (num: %s dealer: %d cards: %s%s): %s\n"
		"*************************************************************\n",
		p_version_info->GetVersionInfo(),
		p_handreset_detector->GetHandNumber(),
		p_symbol_engine_dealerchair->dealerchair(), 
		card0, card1, title);
}

double CSymbols::GetSymbolVal(const char *a, int *e)
{
	// Look up a symbol value.
	// As a long series of string comparisions is not efficient
	// (especially for large formulas or with the validator turned on),
	// we do now a two-step-comparision (preselection for symbol groups).
	// This should result in still quite maintenable and more efficient code.

	// PLAYERS OPPONENTS 1(7)
	if (memcmp(a, "nopponents", 10)==0)
	{
		if (memcmp(a, "nopponents", 10)==0 && strlen(a)==10)				return p_symbol_engine_prwin->nopponents_for_prwin();
		if (memcmp(a, "nopponentsseated", 16)==0 && strlen(a)==16)			return p_symbol_engine_active_dealt_playing->nopponentsseated();
		if (memcmp(a, "nopponentsactive", 16)==0 && strlen(a)==16)			return p_symbol_engine_active_dealt_playing->nopponentsactive();
		if (memcmp(a, "nopponentsdealt", 15)==0 && strlen(a)==15)			return p_symbol_engine_active_dealt_playing->nopponentsdealt();
		if (memcmp(a, "nopponentsplaying", 17)==0 && strlen(a)==17)			return p_symbol_engine_active_dealt_playing->nopponentsplaying();
		if (memcmp(a, "nopponentsblind", 15)==0 && strlen(a)==15)			return p_symbol_engine_blinds->nopponentsblind();
		if (memcmp(a, "nopponentschecking", 18)==0 && strlen(a)==18)		return p_symbol_engine_raisers_callers->nopponentschecking();
		if (memcmp(a, "nopponentscalling", 17)==0 && strlen(a)==17)			return p_symbol_engine_raisers_callers->nopponentscalling();
		if (memcmp(a, "nopponentsraising", 17)==0 && strlen(a)==17)			return p_symbol_engine_raisers_callers->nopponentsraising();
		if (memcmp(a, "nopponentsbetting", 17)==0 && strlen(a)==17)			return p_symbol_engine_raisers_callers->nopponentsbetting();
		if (memcmp(a, "nopponentsfolded", 16)==0 && strlen(a)==16)			return p_symbol_engine_raisers_callers->nopponentsfolded();
	}

	// PLAYERS OPPONENTS 2(7)
	// HISTORY 2(3)
	if (memcmp(a, "nplayers", 8)==0)
	{
		// PLAYERS OPPONENTS 2(7
		if (memcmp(a, "nplayersseated", 14)==0 && strlen(a)==14)			return p_symbol_engine_active_dealt_playing->nplayersseated();
		if (memcmp(a, "nplayersactive", 14)==0 && strlen(a)==14)			return p_symbol_engine_active_dealt_playing->nplayersactive();
		if (memcmp(a, "nplayersdealt", 13)==0 && strlen(a)==13)				return p_symbol_engine_active_dealt_playing->nplayersdealt();
		if (memcmp(a, "nplayersplaying", 15)==0 && strlen(a)==15)			return p_symbol_engine_active_dealt_playing->nplayersplaying();
		if (memcmp(a, "nplayersblind", 13)==0 && strlen(a)==13)				return p_symbol_engine_blinds->nplayersblind();
		if (memcmp(a, "nplayerscallshort", 17)==0 && strlen(a)==17)			return p_symbol_engine_raisers_callers->nplayerscallshort();

		// HISTORY 2(3)
		if (memcmp(a, "nplayersround", 13)==0 && strlen(a)==13)				return p_symbol_engine_history->nplayersround(p_betround_calculator->betround());
		if (memcmp(a, "nplayersround", 13)==0 && strlen(a)==14)				return p_symbol_engine_history->nplayersround(a[13]-'0');
	}

	// PLAYERS OPPONENTS 4(7)	
	if (memcmp(a, "nchairs", 7)==0)
	{
		if (memcmp(a, "nchairsdealtright", 17)==0 && strlen(a)==17)			return p_symbol_engine_positions->nchairsdealtright();
		if (memcmp(a, "nchairsdealtleft", 16)==0 && strlen(a)==16)			return p_symbol_engine_positions->nchairsdealtleft();
	}

	// PLAYERS OPPONENTS 5(7)
	if (memcmp(a, "players", 7)==0)
	{
		if (memcmp(a, "playersseatedbits", 17)==0 && strlen(a)==17)			return p_symbol_engine_active_dealt_playing->playersseatedbits();
		if (memcmp(a, "playersactivebits", 17)==0 && strlen(a)==17)			return p_symbol_engine_active_dealt_playing->playersactivebits();
		if (memcmp(a, "playersdealtbits", 16)==0 && strlen(a)==16)			return p_symbol_engine_active_dealt_playing->playersdealtbits();
		if (memcmp(a, "playersplayingbits", 18)==0 && strlen(a)==18)		return p_symbol_engine_active_dealt_playing->playersplayingbits();
		if (memcmp(a, "playersblindbits", 16)==0 && strlen(a)==16)			return p_symbol_engine_blinds->playersblindbits();
	}

	// PLAYERS OPPONENTS 6(7)
	if (memcmp(a, "opponents", 9)==0)
	{
		if (memcmp(a, "opponentsseatedbits", 19)==0 && strlen(a)==19)		return p_symbol_engine_active_dealt_playing->opponentsseatedbits();
		if (memcmp(a, "opponentsactivebits", 19)==0 && strlen(a)==19)		return p_symbol_engine_active_dealt_playing->opponentsactivebits();
		if (memcmp(a, "opponentsdealtbits", 18)==0 && strlen(a)==18)		return p_symbol_engine_active_dealt_playing->opponentsdealtbits();
		if (memcmp(a, "opponentsplayingbits", 20)==0 && strlen(a)==20)		return p_symbol_engine_active_dealt_playing->opponentsplayingbits();
		if (memcmp(a, "opponentsblindbits", 18)==0 && strlen(a)==18)		return p_symbol_engine_blinds->opponentsblindbits();
	}

	// LIST TESTS 1(2)
	if (memcmp(a, "islist", 6) == 0)	return p_symbol_engine_lists->IsList(atoi(a+6));

	// PokerTracker symbols
	if (memcmp(a,"pt",2 )==0)
	{
		return p_symbol_engine_pokertracker->ProcessQuery(a);
	}

	// HAND RANK
	if (memcmp(a, "handrank", 8) == 0)
	{
		if (memcmp(a, "handrank169", 11)==0 && strlen(a)==11)	return p_symbol_engine_handrank->handrank169();
		if (memcmp(a, "handrank2652", 12)==0 && strlen(a)==12)	return p_symbol_engine_handrank->handrank2652();
		if (memcmp(a, "handrank1326", 12)==0 && strlen(a)==12)	return p_symbol_engine_handrank->handrank1326();
		if (memcmp(a, "handrank1000", 12)==0 && strlen(a)==12)	return p_symbol_engine_handrank->handrank1000();
		if (memcmp(a, "handrankp", 9)==0 && strlen(a)==9)		return p_symbol_engine_handrank->handrankp();
	}

	// NHANDS 1(2)
	if (memcmp(a, "nhands", 6)==0)
	{
		if (memcmp(a, "nhands", 6)==0 && strlen(a)==6)			return p_symbol_engine_prwin->nhands();
		if (memcmp(a, "nhandshi", 8)==0 && strlen(a)==8)		return p_symbol_engine_prwin->nhandshi();
		if (memcmp(a, "nhandslo", 8)==0 && strlen(a)==8)		return p_symbol_engine_prwin->nhandslo();
		if (memcmp(a, "nhandsti", 8)==0 && strlen(a)==8)		return p_symbol_engine_prwin->nhandsti();
	}

	// RANK HI
	// Part 1(2): rankhi...symbols
	if (memcmp(a, "rankhi", 6) == 0)
	{
		if (memcmp(a, "rankhi", 6)==0 && strlen(a)==6)						return p_symbol_engine_pokerval->rankhi();
		if (memcmp(a, "rankhicommon", 12)==0 && strlen(a)==12)				return p_symbol_engine_pokerval->rankhicommon();
		if (memcmp(a, "rankhiplayer", 12)==0 && strlen(a)==12)				return p_symbol_engine_pokerval->rankhiplayer();
		if (memcmp(a, "rankhipoker", 11)==0 && strlen(a)==11)				return p_symbol_engine_pokerval->rankhipoker();
	}

	// SRANK HI
	// Part 2(2): srankhi...symbols
	if (memcmp(a, "srankhi", 7) == 0)
	{
		if (memcmp(a, "srankhi", 7)==0 && strlen(a)==7)						return p_symbol_engine_pokerval->srankhi();
		if (memcmp(a, "srankhicommon", 13)==0 && strlen(a)==13)				return p_symbol_engine_pokerval->srankhicommon();
		if (memcmp(a, "srankhiplayer", 13)==0 && strlen(a)==13)				return p_symbol_engine_pokerval->srankhiplayer();
		if (memcmp(a, "srankhipoker", 12)==0 && strlen(a)==12)				return p_symbol_engine_pokerval->srankhipoker();
	}

	// RANK LO
	// Part 1(2): ranklo...symbols
	if (memcmp(a, "ranklo", 6) == 0)
	{
		if (memcmp(a, "ranklo", 6)==0 && strlen(a)==6)						return p_symbol_engine_pokerval->ranklo();
		if (memcmp(a, "ranklocommon", 12)==0 && strlen(a)==12)				return p_symbol_engine_pokerval->ranklocommon();
		if (memcmp(a, "rankloplayer", 12)==0 && strlen(a)==12)				return p_symbol_engine_pokerval->rankloplayer();
		if (memcmp(a, "ranklopoker", 11)==0 && strlen(a)==11)				return p_symbol_engine_pokerval->ranklopoker();
	}

	// SRANK LO
	// Part 2(2): sranklo...symbols
	if (memcmp(a, "sranklo", 7) == 0)
	{
		if (memcmp(a, "sranklo", 7)==0 && strlen(a)==7)						return p_symbol_engine_pokerval->sranklo();
		if (memcmp(a, "sranklocommon", 13)==0 && strlen(a)==13)				return p_symbol_engine_pokerval->sranklocommon();
		if (memcmp(a, "srankloplayer", 13)==0 && strlen(a)==13)				return p_symbol_engine_pokerval->srankloplayer();
		if (memcmp(a, "sranklopoker", 12)==0 && strlen(a)==12)				return p_symbol_engine_pokerval->sranklopoker();
	}

	// Versus
	if (memcmp(a, "vs$", 3) == 0)
	{
		return p_versus->GetSymbol(a, e);
	}

	// Probabilities
	// Part 1(2): random...-symbols
	if (memcmp(a, "random", 6) == 0)
	{
		if (memcmp(a, "randomheartbeat", 15)==0 && strlen(a)==15)			return p_symbol_engine_random->randomheartbeat();
		if (memcmp(a, "randomhand", 10)==0 && strlen(a)==10)				return p_symbol_engine_random->randomhand();
		if (memcmp(a, "randomround", 11)==0 && strlen(a)==11)				return p_symbol_engine_random->randomround();
		if (memcmp(a, "random", 6) == 0 && strlen(a)==6)					return p_symbol_engine_random->random();
	}

	// History
	// Part 1(3): did...-symbols
	if (memcmp(a, "did", 3) == 0)
	{
		if (memcmp(a, "didchec", 7)==0 && strlen(a)==7)						return p_symbol_engine_history->didchec(p_betround_calculator->betround());
		if (memcmp(a, "didcall", 7)==0 && strlen(a)==7)						return p_symbol_engine_history->didcall(p_betround_calculator->betround());
		if (memcmp(a, "didrais", 7)==0 && strlen(a)==7)						return p_symbol_engine_history->didrais(p_betround_calculator->betround());
		if (memcmp(a, "didswag", 7)==0 && strlen(a)==7)						return p_symbol_engine_history->didswag(p_betround_calculator->betround());
		if (memcmp(a, "didchecround", 12)==0 && strlen(a)==13)				return p_symbol_engine_history->didchec(a[12]-'0');
		if (memcmp(a, "didcallround", 12)==0 && strlen(a)==13)				return p_symbol_engine_history->didcall(a[12]-'0');
		if (memcmp(a, "didraisround", 12)==0 && strlen(a)==13)				return p_symbol_engine_history->didrais(a[12]-'0');
		if (memcmp(a, "didswaground", 12)==0 && strlen(a)==13)				return p_symbol_engine_history->didswag(a[12]-'0');
	}	

	// History symbols
	if (memcmp(a, "hi_", 3)==0)
	{
		char	sym[50] = {0};
		int		round = 0;
		strcpy_s(sym, 50, &a[3]);
		round = sym[strlen(sym)-1]-'0';
		sym[strlen(sym)-1] = '\0';
		return p_game_state->OHSymHist(sym, round);
	}

	// RANK BITS 1(2)
	if (memcmp(a, "rankbits", 8)==0)
	{
		if (memcmp(a, "rankbits", 8)==0 && strlen(a)==8)					return p_symbol_engine_pokerval->rankbits();
		if (memcmp(a, "rankbitscommon", 14)==0 && strlen(a)==14)			return p_symbol_engine_pokerval->rankbitscommon();
		if (memcmp(a, "rankbitsplayer", 14)==0 && strlen(a)==14)			return p_symbol_engine_pokerval->rankbitsplayer();
		if (memcmp(a, "rankbitspoker", 13)==0 && strlen(a)==13)				return p_symbol_engine_pokerval->rankbitspoker();
	}

	// SRANK BITS 2(2)
	if (memcmp(a, "srankbits", 9)==0)
	{
		if (memcmp(a, "srankbits", 9)==0 && strlen(a)==9)					return p_symbol_engine_pokerval->srankbits();
		if (memcmp(a, "srankbitscommon", 15)==0 && strlen(a)==15)			return p_symbol_engine_pokerval->srankbitscommon();
		if (memcmp(a, "srankbitsplayer", 15)==0 && strlen(a)==15)			return p_symbol_engine_pokerval->srankbitsplayer();
		if (memcmp(a, "srankbitspoker", 14)==0 && strlen(a)==14)			return p_symbol_engine_pokerval->srankbitspoker();
	}
	
	// TIME 1(2)
	if (memcmp(a, "elapsed", 7)==0)
	{
		if (memcmp(a, "elapsed", 7)==0 && strlen(a)==7)			return p_symbol_engine_time->elapsed();
		if (memcmp(a, "elapsedhand", 11)==0 && strlen(a)==11)	return p_symbol_engine_time->elapsedhand();
		if (memcmp(a, "elapsedauto", 11)==0 && strlen(a)==11)	return p_symbol_engine_time->elapsedauto();
		if (memcmp(a, "elapsedtoday", 12)==0 && strlen(a)==12)	return p_symbol_engine_time->elapsedtoday();
	}

	// Various is...symbols
	if (memcmp(a, "is", 2)==0)
	{
		// GENERAL
		if (memcmp(a, "isfinaltable", 12)==0 && strlen(a)==12)				return p_symbol_engine_autoplayer->isfinaltable();

		// LIMITS 1(3)
		if (memcmp(a, "isnl", 4)==0 && strlen(a)==4)						return p_tablelimits->isnl();
		if (memcmp(a, "ispl", 4)==0 && strlen(a)==4)						return p_tablelimits->ispl();
		if (memcmp(a, "isfl", 4)==0 && strlen(a)==4)						return p_tablelimits->isfl();
		if (memcmp(a, "istournament", 12)==0 && strlen(a)==12)				return p_symbol_engine_istournament->istournament();

		// HAND TESTS 2(2)
		if (memcmp(a, "ishandup", 8)==0 && strlen(a)==8)					return p_symbol_engine_pokerval->ishandup(); 
		if (memcmp(a, "ishandupcommon", 14)==0 && strlen(a)==14)			return p_symbol_engine_pokerval->ishandupcommon();
		if (memcmp(a, "ishicard", 8)==0 && strlen(a)==8)					return p_symbol_engine_pokerval->ishicard();
		if (memcmp(a, "isonepair", 9)==0 && strlen(a)==9)					return p_symbol_engine_pokerval->isonepair();
		if (memcmp(a, "istwopair", 9)==0 && strlen(a)==9)					return p_symbol_engine_pokerval->istwopair();
		if (memcmp(a, "isthreeofakind", 14)==0 && strlen(a)==14)			return p_symbol_engine_pokerval->isthreeofakind();
		if (memcmp(a, "isstraight", 10)==0 && strlen(a)==10)				return p_symbol_engine_pokerval->isstraight();
		if (memcmp(a, "isflush", 7)==0 && strlen(a)==7)						return p_symbol_engine_pokerval->isflush();
		if (memcmp(a, "isfullhouse", 11)==0 && strlen(a)==11)				return p_symbol_engine_pokerval->isfullhouse();
		if (memcmp(a, "isfourofakind", 13)==0 && strlen(a)==13)				return p_symbol_engine_pokerval->isfourofakind();
		if (memcmp(a, "isstraightflush", 15)==0 && strlen(a)==15)			return p_symbol_engine_pokerval->isstraightflush();
		if (memcmp(a, "isroyalflush", 12)==0 && strlen(a)==12)				return p_symbol_engine_pokerval->isroyalflush();

		// POCKET TESTS
		if (memcmp(a, "ispair", 6)==0 && strlen(a)==6)						return p_symbol_engine_cards->ispair();
		if (memcmp(a, "issuited", 8)==0 && strlen(a)==8)					return p_symbol_engine_cards->issuited();
		if (memcmp(a, "isconnector", 11)==0 && strlen(a)==11)				return p_symbol_engine_cards->isconnector();

		// POCKET/COMMON TESTS
		if (memcmp(a, "ishipair", 8)==0 && strlen(a)==8)					return p_symbol_engine_pokerval->ishipair();
		if (memcmp(a, "islopair", 8)==0 && strlen(a)==8)					return p_symbol_engine_pokerval->islopair();
		if (memcmp(a, "ismidpair", 9)==0 && strlen(a)==9)					return p_symbol_engine_pokerval->ismidpair();
		if (memcmp(a, "ishistraight", 12)==0 && strlen(a)==12)				return p_symbol_engine_pokerval->ishistraight();
		if (memcmp(a, "ishiflush", 9)==0 && strlen(a)==9)					return p_symbol_engine_pokerval->ishiflush();

		// AUTOPLAYER 2(2)
		if (memcmp(a, "ismyturn", 8)==0 && strlen(a)==8)					return p_symbol_engine_autoplayer->ismyturn();
		if (memcmp(a, "issittingin", 11)==0 && strlen(a)==11)				return p_symbol_engine_autoplayer->issittingin();
		if (memcmp(a, "issittingout", 12)==0 && strlen(a)==12)				return p_symbol_engine_autoplayer->issittingout();
		if (memcmp(a, "isautopost", 10)==0 && strlen(a)==10)				return p_symbol_engine_autoplayer->isautopost();
		if (memcmp(a, "isfinalanswer", 13)==0 && strlen(a)==13)				return p_symbol_engine_autoplayer->isfinalanswer();
	}

	// HAND TESTS 1(2)
	if (memcmp(a, "$", 1)==0)
	{
		if (memcmp(a, "$$pc", 4)==0)										return p_symbol_engine_cards->$$pc(a[4]-'0');
		if (memcmp(a, "$$pr", 4)==0)										return p_symbol_engine_cards->$$pr(a[4]-'0');
		if (memcmp(a, "$$ps", 4)==0)  										return p_symbol_engine_cards->$$ps(a[4]-'0');
		if (memcmp(a, "$$cc", 4)==0)  										return p_symbol_engine_cards->$$cc(a[4]-'0');
		if (memcmp(a, "$$cr", 4)==0)  										return p_symbol_engine_cards->$$cr(a[4]-'0');
		if (memcmp(a, "$$cs", 4)==0)  										return p_symbol_engine_cards->$$cs(a[4]-'0');
		if (memcmp(a, "$", 1)==0)  											return p_symbol_engine_cards->IsHand(a, e);
	}

	// POKER VALUES 1(2)
	if (memcmp(a, "pokerval", 8)==0)
	{
		if (memcmp(a, "pokerval", 8)==0 && strlen(a)==8)					return p_symbol_engine_pokerval->pokerval();
		if (memcmp(a, "pokervalplayer", 14)==0 && strlen(a)==14)			return p_symbol_engine_pokerval->pokervalplayer();
		if (memcmp(a, "pokervalcommon", 14)==0 && strlen(a)==14)			return p_symbol_engine_pokerval->pokervalcommon();
	}

	// (UN) KNOWN CARDS 1(3)
	if (memcmp(a, "ncards", 6)==0)
	{
		if (memcmp(a, "ncardsknown", 11)==0 && strlen(a)==11)				return p_symbol_engine_cards->ncardsknown();
		if (memcmp(a, "ncardsunknown", 13)==0 && strlen(a)==13)				return p_symbol_engine_cards->ncardsunknown();
		if (memcmp(a, "ncardsbetter", 12)==0 && strlen(a)==12)				return p_symbol_engine_cards->ncardsbetter();
	}

	// Varios probabilities
	if (memcmp(a, "pr", 2)==0)
	{
		// PROBABILITIES
		// Part 2(2)
		if (memcmp(a, "prwin", 5)==0 && strlen(a)==5)						return iter_vars.prwin();
		if (memcmp(a, "prlos", 5)==0 && strlen(a)==5)						return iter_vars.prlos();
		if (memcmp(a, "prtie", 5)==0 && strlen(a)==5)						return iter_vars.prtie();

		//NHANDS 2(2)
		if (memcmp(a, "prwinnow", 8)==0 && strlen(a)==8)					return p_symbol_engine_prwin->prwinnow();
		if (memcmp(a, "prlosnow", 8)==0 && strlen(a)==8)					return p_symbol_engine_prwin->prlosnow();
	}

	// FLUSHES SETS STRAIGHTS 1(5)
	if (memcmp(a, "nsuited", 7)==0)
	{
		if (memcmp(a, "nsuited", 7)==0 && strlen(a)==7)						return p_symbol_engine_cards->nsuited();
		if (memcmp(a, "nsuitedcommon", 13)==0 && strlen(a)==13)				return p_symbol_engine_cards->nsuitedcommon();
	}

	// FLUSHES SETS STRAIGHTS 2(5)
	if (memcmp(a, "tsuit", 5)==0)
	{
		if (memcmp(a, "tsuit", 5)==0 && strlen(a)==5)						return p_symbol_engine_cards->tsuit();
		if (memcmp(a, "tsuitcommon", 11)==0 && strlen(a)==11)				return p_symbol_engine_cards->tsuitcommon();
	}

	// FLUSHES SETS STRAIGHTS 3(5)
	if (memcmp(a, "nranked", 7)==0)
	{
		if (memcmp(a, "nranked", 7)==0 && strlen(a)==7)						return p_symbol_engine_cards->nranked();
		if (memcmp(a, "nrankedcommon", 13)==0 && strlen(a)==13)				return p_symbol_engine_cards->nrankedcommon();
	}

	// FLUSHES SETS STRAIGHTS 4(5)
	if (memcmp(a, "trank", 5)==0)
	{
		if (memcmp(a, "trank", 5)==0 && strlen(a)==5)						return p_symbol_engine_cards->trank();
		if (memcmp(a, "trankcommon", 11)==0 && strlen(a)==11)				return p_symbol_engine_cards->trankcommon();
	}

	// FLUSHES SETS STRAIGHTS 5(5)
	if (memcmp(a, "nstraight", 9)==0)
	{
		if (memcmp(a, "nstraight", 9)==0 && strlen(a)==9)					return p_symbol_engine_cards->nstraight();
		if (memcmp(a, "nstraightcommon", 15)==0 && strlen(a)==15)			return p_symbol_engine_cards->nstraightcommon();
		if (memcmp(a, "nstraightfill", 13)==0 && strlen(a)==13)				return p_symbol_engine_cards->nstraightfill();
		if (memcmp(a, "nstraightfillcommon", 19)==0 && strlen(a)==19)		return p_symbol_engine_cards->nstraightfillcommon();
		if (memcmp(a, "nstraightflush", 14)==0 && strlen(a)==14)			return p_symbol_engine_cards->nstraightflush();
		if (memcmp(a, "nstraightflushcommon", 20)==0 && strlen(a)==20)		return p_symbol_engine_cards->nstraightflushcommon();
		if (memcmp(a, "nstraightflushfill", 18)==0 && strlen(a)==18)		return p_symbol_engine_cards->nstraightflushfill();
		if (memcmp(a, "nstraightflushfillcommon", 24)==0 && strlen(a)==24)  return p_symbol_engine_cards->nstraightflushfillcommon();
	}

	// LIMITS 2(3)
	if (memcmp(a, "srai", 4)==0)
	{
		if (memcmp(a, "sraiprev", 8)==0 && strlen(a)==8)					return p_symbol_engine_chip_amounts->sraiprev();
		if (memcmp(a, "sraimin", 7)==0 && strlen(a)==7)						return p_symbol_engine_chip_amounts->sraimin();
		if (memcmp(a, "sraimax", 7)==0 && strlen(a)==7)						return p_symbol_engine_chip_amounts->sraimax();
	}

	// (UN)KNOWN CARDS 2(3)
	if (memcmp(a, "ncards", 6)==0)
	{
		if (memcmp(a, "ncardsknown", 11)==0 && strlen(a)==11)				return p_symbol_engine_cards->ncardsknown();
		if (memcmp(a, "ncardsunknown", 13)==0 && strlen(a)==13)				return p_symbol_engine_cards->ncardsunknown();
		if (memcmp(a, "ncardsbetter", 12)==0 && strlen(a)==12)				return p_symbol_engine_cards->ncardsbetter();
	}

	// CHAIRS 1(2)
	if (memcmp(a, "chair", 5)==0)
	{
		if (memcmp(a, "chair$", 6)==0)									return Chair$(&a[6]);
		if (memcmp(a, "chairbit$", 9)==0)								return Chairbit$(&a[9]);
	}

	// Various pot..symbols
	if (memcmp(a, "pot", 3)==0)
	{
		// CHIP AMOUNTS 1(2)
		if (memcmp(a, "pot", 3)==0 && strlen(a)==3)							return p_symbol_engine_chip_amounts->pot();
		if (memcmp(a, "potcommon", 9)==0 && strlen(a)==9)					return p_symbol_engine_chip_amounts->potcommon();
		if (memcmp(a, "potplayer", 9)==0 && strlen(a)==9)					return p_symbol_engine_chip_amounts->potplayer();
	}

	// Various symbols below
	// without any optimized lookup.
	//
	// CHAIRS 2(2)	
	if (memcmp(a, "userchair", 9)==0 && strlen(a)==9)					return p_symbol_engine_userchair->userchair();
	if (memcmp(a, "dealerchair", 11)==0 && strlen(a)==11)				return p_symbol_engine_dealerchair->dealerchair();
	if (memcmp(a, "raischair", 9)==0 && strlen(a)==9)					return p_symbol_engine_raisers_callers->raischair();

	//ROUND&POSITIONS
	if (memcmp(a, "betround", 8)==0 && strlen(a)==8)					return p_betround_calculator->betround();
	if (memcmp(a, "betposition", 11)==0 && strlen(a)==11)				return p_symbol_engine_positions->betposition();
	if (memcmp(a, "dealposition", 12)==0 && strlen(a)==12)				return p_symbol_engine_positions->dealposition();
	if (memcmp(a, "callposition", 12)==0 && strlen(a)==12)				return p_symbol_engine_positions->callposition();
	if (memcmp(a, "dealpositionrais", 16)==0 && strlen(a)==16)			return p_symbol_engine_positions->dealpositionrais();
	if (memcmp(a, "betpositionrais", 15)==0 && strlen(a)==15)			return p_symbol_engine_positions->betpositionrais();

	//CHIP AMOUNTS 2(2)
	if (memcmp(a, "balance", 7)==0 && strlen(a)==7)						return p_symbol_engine_chip_amounts->balance(p_symbol_engine_userchair->userchair()); 
	if (memcmp(a, "balance", 7)==0 && strlen(a)==8)						return p_symbol_engine_chip_amounts->balance(a[7]-'0');
	if (memcmp(a, "maxbalance", 10)==0 && strlen(a)==10)  				return p_symbol_engine_chip_amounts->maxbalance();
	if (memcmp(a, "balanceatstartofsession", 23)==0 && strlen(a)==24)	return p_symbol_engine_chip_amounts->balanceatstartofsession();
	if (memcmp(a, "stack", 5)==0 && strlen(a)==6)						return p_symbol_engine_chip_amounts->stack(a[5]-'0');
	if (memcmp(a, "currentbet", 10)==0 && strlen(a)==10)				return p_symbol_engine_chip_amounts->currentbet(p_symbol_engine_userchair->userchair());
	if (memcmp(a, "currentbet", 10)==0 && strlen(a)==11)				return p_symbol_engine_chip_amounts->currentbet(a[10]-'0');
	if (memcmp(a, "call", 4)==0 && strlen(a)==4)						return p_symbol_engine_chip_amounts->call();
	if (memcmp(a, "bet", 3)==0 && strlen(a)==3)							return p_tablelimits->bet();
	if (memcmp(a, "bet", 3)==0 && strlen(a)==4)							return p_tablelimits->bet(a[3]-'0');
	
	//NUMBER OF BETS
	if (memcmp(a, "nbetstocall", 11)==0 && strlen(a)==11)				return p_symbol_engine_chip_amounts->nbetstocall();
	if (memcmp(a, "nbetstorais", 11)==0 && strlen(a)==11)				return p_symbol_engine_chip_amounts->nbetstorais();
	if (memcmp(a, "ncurrentbets", 12)==0 && strlen(a)==12)				return p_symbol_engine_chip_amounts->ncurrentbets();
	if (memcmp(a, "ncallbets", 9)==0 && strlen(a)==9)					return p_symbol_engine_chip_amounts->ncallbets();
	if (memcmp(a, "nraisbets", 9)==0 && strlen(a)==9)					return p_symbol_engine_chip_amounts->nraisbets();

	//POKER VALUES 2(2)	
	if (memcmp(a, "pcbits", 6)==0 && strlen(a)==6)						return p_symbol_engine_pokerval->pcbits();
	if (memcmp(a, "npcbits", 7)==0 && strlen(a)==7)						return p_symbol_engine_pokerval->npcbits();

	//POKER VALUE CONSTANTS
	if (memcmp(a, "hicard", 6)==0 && strlen(a)==6)						return k_pokerval_hicard;
	if (memcmp(a, "onepair", 7)==0 && strlen(a)==7)						return k_pokerval_onepair;
	if (memcmp(a, "twopair", 7)==0 && strlen(a)==7)						return k_pokerval_twopair;
	if (memcmp(a, "threeofakind", 12)==0 && strlen(a)==12)				return k_pokerval_threeofakind;
	if (memcmp(a, "straight", 8)==0 && strlen(a)==8)					return k_pokerval_straight;
	if (memcmp(a, "flush", 5)==0 && strlen(a)==5)						return k_pokerval_flush;
	if (memcmp(a, "fullhouse", 9)==0 && strlen(a)==9)					return k_pokerval_fullhouse;
	if (memcmp(a, "fourofakind", 11)==0 && strlen(a)==11)				return k_pokerval_fourofakind;
	if (memcmp(a, "straightflush", 13)==0 && strlen(a)==13)				return k_pokerval_straightflush;
	if (memcmp(a, "royalflush", 10)==0 && strlen(a)==10)				return k_pokerval_royalflush;

	// callbits, raisbits, etc. 
	if (memcmp(a, "bblindbits", 10)==0 && strlen(a)==10)  				return p_symbol_engine_blinds->bblindbits();
	if (memcmp(a, "raisbits", 8)==0 && strlen(a)==9)  					return p_symbol_engine_raisers_callers->raisbits(a[8]-'0');
	if (memcmp(a, "callbits", 8)==0 && strlen(a)==9)  					return p_symbol_engine_raisers_callers->callbits(a[8]-'0');
	if (memcmp(a, "foldbits", 8)==0 && strlen(a)==9)  					return p_symbol_engine_raisers_callers->foldbits(a[8]-'0');

	//FLAGS
	if (memcmp(a, "fmax", 4)==0 && strlen(a)==4)						return p_flags_toolbar->GetFlagMax();
	if (memcmp(a, "f", 1)==0 && strlen(a)==2)							return p_flags_toolbar->GetFlag(a[1]-'0');
	if (memcmp(a, "f", 1)==0 && strlen(a)==3)							return p_flags_toolbar->GetFlag(10 * (a[1]-'0') + a[2] - '0');
	if (memcmp(a, "fbits", 5)==0 && strlen(a)==5)						return p_flags_toolbar->GetFlagBits();

	//COMMON CARDS
	if (memcmp(a, "ncommoncardsknown", 17)==0 && strlen(a)==17)			return p_symbol_engine_cards->ncommoncardsknown();

	//(UN)KNOWN CARDS 3(3)
	if (memcmp(a, "nouts", 5)==0 && strlen(a)==5)						return p_symbol_engine_cards->nouts();

	// HISTORY S
	// Part 3(3)
	if (memcmp(a, "prevaction", 10)==0 && strlen(a)==10)				return p_symbol_engine_history->prevaction();
	if (memcmp(a, "nbetsround", 10)==0 && strlen(a)==10)				return p_symbol_engine_history->nbetsround(p_betround_calculator->betround());
	if (memcmp(a, "nbetsround", 10)==0 && strlen(a)==11)				return p_symbol_engine_history->nbetsround(a[10]-'0');

	// GENERAL
	if (memcmp(a, "nchairs", 7)==0 && strlen(a)==7)						return p_tablemap->nchairs();
	if (memcmp(a, "session", 7)==0 && strlen(a)==7)						return p_sessioncounter->session_id();
	if (memcmp(a, "version", 7)==0 && strlen(a)==7)						return VERSION_NUMBER;

	// LIMITS 3(3)
	if (memcmp(a, "bblind", 6)==0 && strlen(a)==6)						return p_tablelimits->bblind();
	if (memcmp(a, "sblind", 6)==0 && strlen(a)==6)						return p_tablelimits->sblind();
	if (memcmp(a, "ante", 4)==0 && strlen(a)==4)						return p_tablelimits->ante();
	if (memcmp(a, "lim", 3)==0 && strlen(a)==3)							return p_tablelimits->gametype();

	//PROFILE
	if (memcmp(a, "sitename$", 9)==0)									return p_tablemap->sitename().Find(&a[9])!=-1;
	if (memcmp(a, "network$", 8)==0)									return p_tablemap->network().Find(&a[8])!=-1;

	//FORMULA FILE
	if (memcmp(a, "f$prwin_number_of_iterations", 28)==0 && strlen(a)==28)	iter_vars.nit(); 

	// AUTOPLAYER 1(2)
	if (memcmp(a, "myturnbits", 10)==0 && strlen(a)==10)				return p_symbol_engine_autoplayer->myturnbits();

	// GameState symbols
	if (memcmp(a, "lastraised", 10)==0 && strlen(a)==11)  				return p_game_state->LastRaised(a[10]-'0');
	if (memcmp(a, "floppct", 7)==0 && strlen(a)==7)  					return p_game_state->FlopPct();
	if (memcmp(a, "turnpct", 7)==0 && strlen(a)==7)  					return p_game_state->TurnPct();
	if (memcmp(a, "riverpct", 8)==0 && strlen(a)==8)  					return p_game_state->RiverPct();
	if (memcmp(a, "avgbetspf", 9)==0 && strlen(a)==9)  					return p_game_state->AvgBetsPf();
	if (memcmp(a, "tablepfr", 8)==0 && strlen(a)==8)  					return p_game_state->TablePfr();	
	if (memcmp(a, "handsplayed", 11)==0 && strlen(a)==11)  				return p_game_state->hands_played();
	if (memcmp(a, "balance_rank", 12)==0 && strlen(a)==13)  			return p_game_state->SortedBalance(a[12]-'0');

	// OH-script-messagebox
	if (memcmp(a, "msgbox$", 7)==0 && strlen(a)>7)  					return OH_MessageBox_OH_Script_Messages(a);

	*e = ERR_INVALID_SYM;
	return 0.0;
}

void CSymbols::RecordPrevAction(const ActionConstant action)
{/*
	write_log(prefs.debug_symbolengine(), "CSymbols::AdaptSymbolsForUsersAction(%d)\n", action);
	set_prevaction(action);

	// !! Most things temporary disabled, as this causes only problems
	// Only needed for Gecko

	int user_chair = p_symbol_engine_userchair->userchair();
	int betround = int(p_betround_calculator->betround());

	double bet = p_tablelimits->bet(_sym.betround);
	assert(bet > 0);	

	// Initializing "new" values with known old ones
	double new_pot = _sym.pot;
	double new_bet = _sym.currentbet[10];
	double new_number_of_bets = new_bet / bet;

	// Adapting symbols depending on our action
	switch (action)
	{
		case k_action_fold:	
			write_log(prefs.debug_symbolengine(), "Adjusting symbols for users action: fold - nothing to do\n");
			// Did-symbols - no "didfold"
			// Bets and pot - nothing to adapt
			break;
		case k_action_check:
			write_log(prefs.debug_symbolengine(), "Adjusting symbols for users action: check\n");
			// Did-symbols
			set_didchec(4, p_symbols->sym()->didchec[4] + 1);
			set_didchec(betround-1, p_symbols->sym()->didchec[betround-1] + 1);
			// Bets and pot
			new_number_of_bets = _sym.ncallbets;
			new_bet = new_number_of_bets * bet;
			new_pot = _sym.pot + new_bet - _sym.currentbet[10];
			break;
		case k_action_call:
			assert(p_autoplayer_functions->f$call() > 0);
			write_log(prefs.debug_symbolengine(), "Adjusting symbols for users action: call\n");
			// Did-symbols
			set_didcall(4, p_symbols->sym()->didcall[4] + 1);
			set_didcall(betround-1, p_symbols->sym()->didcall[betround-1] + 1);
			// Bets and pot
			new_number_of_bets = _sym.ncallbets;
			new_bet = new_number_of_bets * bet;
			new_pot = _sym.pot + new_bet - _sym.currentbet[10];
			break;
		case k_action_raise:
			assert(p_autoplayer_functions->f$rais() > 0);
			write_log(prefs.debug_symbolengine(), "Adjusting symbols for users action: raise\n");
			// Did-symbols
			set_didrais(4, p_symbols->sym()->didrais[4] + 1);
			set_didrais(betround-1, p_symbols->sym()->didrais[betround-1] + 1);
			// Bets and pot
			new_number_of_bets = _sym.nraisbets;
			new_bet = new_number_of_bets * bet;
			new_pot = _sym.pot + new_bet - _sym.currentbet[10];
			break;
		case k_action_betsize:
			assert(p_autoplayer_functions->f$betsize() > 0);
			write_log(prefs.debug_symbolengine(), "Adjusting symbols for users action: swag\n");
			// Did-symbols
			set_didswag(4, p_symbols->sym()->didswag[4] + 1);
			set_didswag(betround-1, p_symbols->sym()->didswag[betround-1] + 1);
			// Bets and pot
			// Disabled till OH 2.2 as it causes bad side-effects for the call symbol.
			new_bet = _sym.currentbet[k_max_number_of_players]; // f$swag(); // !! That's not correct, but will be for OH 4.0.0 because of swagadjustment
			new_number_of_bets = new_bet / bet; 
			new_pot = _sym.pot + p_autoplayer_functions->f$betsize() - _sym.currentbet[10];
			break;
		case k_action_allin:
			// No "didallin"
			break;
		default: 
			assert(k_this_must_not_happen);
			break;
	}
	*/
}

