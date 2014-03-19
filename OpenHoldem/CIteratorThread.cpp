//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose: PrWin-simulation
//
//***************************************************************************** 

#include "stdafx.h"
#include "CIteratorThread.h"

#include <process.h>
#include "CBetroundCalculator.h"
#include "CGrammar.h"
#include "CiteratorVars.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineBlinds.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEnginePrwin.h"
#include "CValidator.h"
#include "inlines/eval.h"
#include "MagicNumbers.h"
#include "Numericalfunctions.h"
#include "PrWinHandranges.h"

CIteratorThread		*p_iterator_thread = NULL;

// weighted prwin lookup tables for non-suited and suited cards
int pair2rank_offsuited[170] = {0}, pair2rank_suited[170] = {0};
// used to resolve ascii cards to numbers for init of above
char ctonum[k_number_of_ranks_per_deck + 1]="23456789TJQKA";
// int willplay = 0, wontplay = 0, topclip = 0, mustplay = 0
sprw1326	_prw1326;	//prwin 1326 data structure Matrix 2008-04-29


// handrank table used to prime weighted prwin lookup table.
// reflects incidence of cards people actually play to flop.
// left in this form for ease of developer modification.
// converted at startup to the tables actually used by prwin calculation
char *prwin_handrank_table_169[k_number_of_starting_hands] =
{
	"AA ","KK ","QQ ","AKs","JJ ","AQs","KQs","TT ","AJs","KJs",
	"JTs","QJs","QTs","99 ","ATs","KTs","88 ","T9s","AK ","J9s",
	"77 ","98s","Q9s","66 ","A9s","K9s","T8s","55 ","A5s","54s",
	"44 ","A8s","87s","33 ","65s","22 ","AQ ","A4s","J8s","A3s",
	"76s","97s","A7s","KQ ","A2s","Q8s","86s","K8s","A6s","75s",
	"T7s","53s","64s","K7s","AJ ","KJ ","43s","QJ ","96s","JT ",
	"J7s","K6s","K5s","QT ","85s","Q7s","KT ","AT ","K4s","K3s",
	"K2s","74s","T6s","52s","Q6s","63s","Q5s","Q4s","42s","Q3s",
	"95s","J6s","J5s","32s","Q2s","T9 ","J4s","84s","T5s","J3s",
	"T4s","73s","J2s","J9 ","T3s","T2s","62s","94s","93s","92s",
	"83s","Q9 ","A9 ","98 ","82s","T8 ","K9 ","72s","54 ","87 ",
	"A8 ","A5 ","65 ","A4 ","76 ","J8 ","97 ","A3 ","A7 ","A2 ",
	"86 ","A6 ","Q8 ","75 ","53 ","K8 ","T7 ","64 ","K7 ","43 ",
	"96 ","K6 ","J7 ","85 ","K5 ","Q7 ","K4 ","K3 ","74 ","52 ",
	"T6 ","Q6 ","K2 ","Q5 ","42 ","63 ","Q4 ","95 ","Q3 ","J6 ",
	"J5 ","32 ","Q2 ","J4 ","T5 ","84 ","J3 ","J2 ","T4 ","73 ",
	"T3 ","T2 ","94 ","62 ","93 ","92 ","83 ","82 ","72 "
};

// Make some variables global
// because they need to be accessed and shared 
// by both class functions and static iterator-functions
// and because the former "solution" of passing class pointers 
// to static iterator-functions was no real option either. ;-(
int				deck[k_number_of_cards_per_deck];
CardMask		usedCards,temp_usedCards;
unsigned int	ocard[MAX_OPPONENTS*k_number_of_cards_per_player];
CardMask		addlcomCards;
CardMask		evalCards = {0}, opp_evalCards = {0};
int				_willplay, _wontplay, _mustplay, _topclip;
int				_nplCards, _ncomCards;
double			_win, _tie, _los;

CIteratorThread::CIteratorThread()
{
	write_log(preferences.debug_prwin(), "[PrWinThread] Iterator Thread starting.\n");

	// Create events
	_m_stop_thread = CreateEvent(0, TRUE, FALSE, 0);
	_m_wait_thread = CreateEvent(0, TRUE, FALSE, 0);

	// Initialize variables
	InitIteratorLoop();
	InitHandranktTableForPrwin();

	// FIRST mark thread as running,
	// THEN start thread.
	// The other way can lead to an ugly race-condition,
	// if the thread has already stopped, when we mark it as running.
	iter_vars.set_iterator_thread_running(true);
	AfxBeginThread(IteratorThreadFunction, this);

	write_log(preferences.debug_prwin(), "[PrWinThread] Iterator Thread started.\n");
}

CIteratorThread::~CIteratorThread()
{
	write_log(preferences.debug_prwin(), "[PrWinThread] Iterator Thread ending...\n");

	if (_m_stop_thread)
	{
		// Trigger thread to die
		::SetEvent(_m_stop_thread);

		// Wait until thread finished
		::WaitForSingleObject(_m_wait_thread, k_max_time_to_wait_for_thread_to_shutdown);

		// Close handles
		::CloseHandle(_m_stop_thread);
		::CloseHandle(_m_wait_thread);
	}
	p_iterator_thread = NULL;

	write_log(preferences.debug_prwin(), "[PrWinThread] Iterator Thread ended.\n");
}

void CIteratorThread::StopIteratorThread()
{
	if (p_iterator_thread)
	{
		write_log(preferences.debug_prwin(), "[PrWinThread] Stopping iterator thread.\n");
		// Delete does implicitly call the destructor.
		// All other stuff gets handled there.
		delete p_iterator_thread;
	}
}

void CIteratorThread::RestartIteratorThread()
{
	StopIteratorThread();
	write_log(preferences.debug_prwin(), "[PrWinThread] Restarting iterator thread.\n");
	p_iterator_thread = new CIteratorThread;
}

void CIteratorThread::StartIteratorThreadIfNeeded()
{					
	p_validator->ValidateIt();
	if (p_iterator_thread)
	{
		write_log(preferences.debug_prwin(), "[PrWinThread] IteratorThread running. No need to restart.\n");
		return;
	}
	write_log(preferences.debug_prwin(), "[PrWinThread] IteratorThread not running. Going to restart.\n");
	if (p_symbol_engine_autoplayer->IsFirstHeartbeatOfMyTurn())
	{
		RestartIteratorThread();
		return;
	}
	// Otherwise: nothing to do, e.g. not my turn
}

void CIteratorThread::AdjustPrwinVariablesIfNecessary(CIteratorThread *pParent /* needed ???*/)
{
	// Cut off from IteratorThreadFunction
	// Also moved outside of the loop.

	int	sym_nopponents = p_symbol_engine_prwin->nopponents_for_prwin();
	//Correct the protection aganst low f$willplay/f$wontplay - Matrix 2008-12-22
	if (_willplay && (_willplay < 2 * sym_nopponents + 1))
	{
		write_log(preferences.debug_prwin(), "[PrWinThread] Adjusting willplay (too low)\n");
		_willplay = 2 * sym_nopponents + 1; //too low a value can give lockup
	}
	if (_wontplay < _willplay)
	{
		write_log(preferences.debug_prwin(), "[PrWinThread] Adjusting wontplay (too low)\n");
		_wontplay = _willplay; //wontplay cannot safely be less than willplay
	}
}

UINT CIteratorThread::IteratorThreadFunction(LPVOID pParam)
{
	CIteratorThread *pParent = static_cast<CIteratorThread*>(pParam);

	// Loop-variables j, k get used inside and outside loops.
	// It is a bit messy, nearly impossible to fix it.
	// At least the outer loops ("f$prwin_number_of_iterations" and "i") could be improved.
	unsigned int	pl_pokval = 0, opp_pokval = 0, opp_pokvalmax = 0;
	HandVal			pl_hv = 0, opp_hv = 0;
	int				dummy = 0;

	int				sym_nopponents = p_symbol_engine_prwin->nopponents_for_prwin();
	bool			hand_lost;

	ResetGlobalVariables();
	// Seed the RNG
	srand((unsigned)GetTickCount());

	//
	// Main iterator loop
	//
	write_log(preferences.debug_prwin(), "[PrWinThread] Start of main loop.\n");

	// "f$prwin_number_of_iterations" has to be declared outside of the loop,
	// as we check afterwards, if the loop terminated successfully.
	AdjustPrwinVariablesIfNecessary(pParent);
	unsigned int nit;
	for (nit=0; nit < iter_vars.nit(); nit++)
	{
		// Check event for thread stop signal
		if(::WaitForSingleObject(pParent->_m_stop_thread, 0) == WAIT_OBJECT_0)
		{
			// Set event
			::SetEvent(pParent->_m_wait_thread);
			AfxEndThread(0);
		}

		CardMask_OR(usedCards, pParent->_plCards, pParent->_comCards);
		
		if (UseEnhancedPrWin())
		{
			EnhancedDealingAlgorithm();
		}
		else
		{ 
			StandardDealingAlgorithm(sym_nopponents);
		}

		// Get my handval/pokerval
		CardMask_OR(evalCards, pParent->_plCards, pParent->_comCards);
		CardMask_OR(evalCards, evalCards, addlcomCards);
		pl_hv = Hand_EVAL_N(evalCards, 7);
		pl_pokval = p_symbol_engine_pokerval->CalculatePokerval(pl_hv, 7, &dummy, CARD_NOCARD, CARD_NOCARD);

		// Scan through opponents' handvals/pokervals
		// - if we find one better than ours, then we are done, increment los
		// - for win/tie, we need to wait until we scan them all
		opp_pokvalmax = 0;
		hand_lost = false;
		for (int i=0; i<sym_nopponents; i++)
		{
			CardMask_RESET(opp_evalCards);
			CardMask_OR(opp_evalCards, pParent->_comCards, addlcomCards);
			CardMask_SET(opp_evalCards, ocard[i*2]);
			CardMask_SET(opp_evalCards, ocard[(i*2)+1]);
			opp_hv = Hand_EVAL_N(opp_evalCards, 7);
			opp_pokval = p_symbol_engine_pokerval->CalculatePokerval(opp_hv, 7, &dummy, CARD_NOCARD, CARD_NOCARD);

			if (opp_pokval>pl_pokval)
			{
				_los++;
				hand_lost = true;
				break;
			}
			else
			{
				if (opp_pokval > opp_pokvalmax)
				{
					opp_pokvalmax = opp_pokval;
				}
			}
		}
		if (!hand_lost)
		{
			if (pl_pokval > opp_pokvalmax)
			{
				_win++;
			}
			else
			{
				_tie++;
			}
		}

		UpdateIteratorVarsForDisplay(nit);
	}

	write_log(preferences.debug_prwin(), "[PrWinThread] End of main loop.\n");

	if (SimulationFinished(nit))
	{
		iter_vars.set_iterator_thread_running(false);
		iter_vars.set_iterator_thread_complete(true);
		UpdateIteratorVarsForDisplay(nit);
	}
	else
	{
		iter_vars.set_iterator_thread_running(false);
		iter_vars.set_iterator_thread_complete(false);
		iter_vars.set_iterator_thread_progress(0); //???
		iter_vars.set_nit(0);

		for (int i=0; i<k_number_of_cards_per_player; i++)
			iter_vars.set_pcard(i, CARD_NOCARD);

		for (int i=0; i<k_number_of_community_cards; i++)
		{
			iter_vars.set_ccard(i, CARD_NOCARD);
		}
		ResetIteratorVars();
	}

	::SetEvent(pParent->_m_wait_thread);
	StopIteratorThread();

	return 0;
}

void CIteratorThread::UpdateIteratorVarsForDisplay(unsigned int nit)
{
	// Update display once every 1000 iterations
	if (SimulationFinished(nit)
		|| ((nit % 1000 == 0) && (nit >= 1000)))
	{
		iter_vars.set_iterator_thread_progress(nit);
		iter_vars.set_prwin(_win / (double) nit);
		iter_vars.set_prtie(_tie / (double) nit);
		iter_vars.set_prlos(_los / (double) nit);
		write_log(preferences.debug_prwin(), "[PrWinThread] Progress: %d %.3f %.3f %.3f\n", 
			nit, 
			iter_vars.prwin(),
			iter_vars.prtie(), 
			iter_vars.prlos());
	}
}

bool CIteratorThread::SimulationFinished(unsigned int nit)
{
	return (nit >= iter_vars.nit());
}

void CIteratorThread::ResetIteratorVars()
{
	iter_vars.set_prwin(0);
	iter_vars.set_prtie(0);
	iter_vars.set_prlos(0);
}

void CIteratorThread::ResetGlobalVariables()
{
	CardMask_RESET(usedCards);
	CardMask_RESET(temp_usedCards);
	CardMask_RESET(addlcomCards);
	CardMask_RESET(evalCards);
	CardMask_RESET(opp_evalCards);
	for (int i=0; i<k_number_of_cards_per_deck; i++)
	{
		deck[i] = 0;
	}
	for (int i=0; i<MAX_OPPONENTS; i++)
	{
		ocard[2*i] = 0;
		ocard[2*i + 1] = 0;
	}
}

void CIteratorThread::InitNumberOfIterations()
{
	int			e = SUCCESS;
	CGrammar	gram;

	int number_of_iterations = gram.CalcF$symbol(p_formula, 
		"f$prwin_number_of_iterations", true, &e);
	iter_vars.set_nit(2500); //!! number_of_iterations); 
}

void CIteratorThread::InitIteratorLoop()
{
	int			e = SUCCESS;
	CGrammar	gram;

	write_log(preferences.debug_prwin(), "[PrWinThread] Initializing iterator loop\n");
	// Set starting status and parameters
	iter_vars.set_iterator_thread_running(true);
	iter_vars.set_iterator_thread_complete(false);
	iter_vars.set_iterator_thread_progress(0);
	InitNumberOfIterations();

	// Users cards
	for (int i=0; i<k_number_of_cards_per_player; i++)
	{
		iter_vars.set_pcard(i, p_scraper->card_player(p_symbol_engine_userchair->userchair(), i));
	}
	// Community cards
	for (int i=0; i<k_number_of_community_cards; i++)
	{
		iter_vars.set_ccard(i, p_scraper->card_common(i));
	}

	iter_vars.set_prwin(0);
	iter_vars.set_prtie(0);
	iter_vars.set_prlos(0);

	// player cards
	CardMask_RESET(_plCards);
	CardMask_RESET(_comCards);
	_nplCards = _ncomCards = 0;

	// Counters
	_win = _tie = _los = 0;

	// setup masks
	for (int i=0; i<k_number_of_cards_per_player; i++)
	{
		if (p_scraper_access->IsKnownCard(iter_vars.pcard(i)))
		{
			CardMask_SET(_plCards, iter_vars.pcard(i));
			_nplCards++;
		}
	}
	for (int i=0; i<k_number_of_community_cards; i++)
	{
		if (p_scraper_access->IsKnownCard(iter_vars.ccard(i)))
		{
			CardMask_SET(_comCards, iter_vars.ccard(i));
			_ncomCards++;
		}
	}

	//Weighted prwin only for nopponents <=13
	e = SUCCESS;
	_willplay = (int) gram.CalcF$symbol(p_formula, "f$prwin_willplay", &e);
	e = SUCCESS;
	_wontplay = (int) gram.CalcF$symbol(p_formula, "f$prwin_wontplay", &e);
	e = SUCCESS;
	_mustplay = (int) gram.CalcF$symbol(p_formula, "f$prwin_mustplay", &e);
	e = SUCCESS;
	_topclip = (int) gram.CalcF$symbol(p_formula, "f$prwin_topclip", &e);

	// Call prw1326 callback if needed
	if (_prw1326.useme==1326 
		&& _prw1326.usecallback==1326 
		&& (p_betround_calculator->betround()!= k_betround_preflop
			|| _prw1326.preflop==1326) )
	{
		_prw1326.prw_callback(); //Matrix 2008-05-09
	}
}



void CIteratorThread::InitHandranktTableForPrwin()
{
	int		vndx = 0;
	char	*ptr = NULL;

	//Initialise the handrank tables used by prwin
	vndx=0; //used to provide an offset into the vanilla table
	for (int i=0; i<169; i++)
	{
		//normal weighted prwin table
		ptr = prwin_handrank_table_169[i];
		int j=(strchr(ctonum,*ptr)-ctonum)*13 + (strchr(ctonum,*(ptr+1))-ctonum);
		if (*(ptr+2)=='s')
		{
			pair2rank_suited[j]= i+1;
		}
		else 
		{
			pair2rank_offsuited[j] = i+1;
		}
		//prw1326 vanilla table
		j=strchr(ctonum,*ptr)-ctonum;
		int k=strchr(ctonum,*(ptr+1))-ctonum;
		for(;;)
		{
			//I originally had an algorithm to do this, but it was obscure and impenetrable
			//so now I have switched to the clumsy but simple approach.
			if(j==k)//pair
			{
				_prw1326.vanilla_chair.rankhi[vndx]=j;	//h
				_prw1326.vanilla_chair.rankhi[vndx+1]=j;	//h
				_prw1326.vanilla_chair.rankhi[vndx+2]=j; //h
				_prw1326.vanilla_chair.rankhi[vndx+3]=j+13; //d
				_prw1326.vanilla_chair.rankhi[vndx+4]=j+13; //d
				_prw1326.vanilla_chair.rankhi[vndx+5]=j+26; //c
				_prw1326.vanilla_chair.ranklo[vndx]=k+13;	//d
				_prw1326.vanilla_chair.ranklo[vndx+1]=k+26;	//c
				_prw1326.vanilla_chair.ranklo[vndx+2]=k+39;	//s
				_prw1326.vanilla_chair.ranklo[vndx+3]=k+26;	//c	
				_prw1326.vanilla_chair.ranklo[vndx+4]=k+39;	//s
				_prw1326.vanilla_chair.ranklo[vndx+5]=k+39;	//s
				vndx+=6;
				break;
			}
		
			if (*(ptr+2)=='s') //suited
			{
				_prw1326.vanilla_chair.rankhi[vndx]=j;		//h
				_prw1326.vanilla_chair.rankhi[vndx+1]=j+13;	//d
				_prw1326.vanilla_chair.rankhi[vndx+2]=j+26;	//c
				_prw1326.vanilla_chair.rankhi[vndx+3]=j+39;	//s
				_prw1326.vanilla_chair.ranklo[vndx]=k;		//h
				_prw1326.vanilla_chair.ranklo[vndx+1]=k+13;	//d
				_prw1326.vanilla_chair.ranklo[vndx+2]=k+26;	//c
				_prw1326.vanilla_chair.ranklo[vndx+3]=k+39;	//s
				vndx+=4;
				break;
			}
		
			//only unsuited non-pairs left
			_prw1326.vanilla_chair.rankhi[vndx]=j;		//h
			_prw1326.vanilla_chair.rankhi[vndx+1]=j;		//h
			_prw1326.vanilla_chair.rankhi[vndx+2]=j;		//h
			_prw1326.vanilla_chair.rankhi[vndx+3]=j+13;	//d
			_prw1326.vanilla_chair.rankhi[vndx+4]=j+13;	//d
			_prw1326.vanilla_chair.rankhi[vndx+5]=j+13;	//d
			_prw1326.vanilla_chair.rankhi[vndx+6]=j+26;	//c
			_prw1326.vanilla_chair.rankhi[vndx+7]=j+26;	//c
			_prw1326.vanilla_chair.rankhi[vndx+8]=j+26;	//c
			_prw1326.vanilla_chair.rankhi[vndx+9]=j+39;	//s
			_prw1326.vanilla_chair.rankhi[vndx+10]=j+39;	//s
			_prw1326.vanilla_chair.rankhi[vndx+11]=j+39; //s 
			_prw1326.vanilla_chair.ranklo[vndx]=k+13;	//d
			_prw1326.vanilla_chair.ranklo[vndx+1]=k+26;	//c
			_prw1326.vanilla_chair.ranklo[vndx+2]=k+39;	//s
			_prw1326.vanilla_chair.ranklo[vndx+3]=k;		//h
			_prw1326.vanilla_chair.ranklo[vndx+4]=k+26;	//c
			_prw1326.vanilla_chair.ranklo[vndx+5]=k+39;	//s
			_prw1326.vanilla_chair.ranklo[vndx+6]=k;		//h
			_prw1326.vanilla_chair.ranklo[vndx+7]=k+13;	//d
			_prw1326.vanilla_chair.ranklo[vndx+8]=k+39;	//s
			_prw1326.vanilla_chair.ranklo[vndx+9]=k;		//h
			_prw1326.vanilla_chair.ranklo[vndx+10]=k+13;	//d
			_prw1326.vanilla_chair.ranklo[vndx+11]=k+26;	//c
			vndx+=12;
			break;
		}
	}

	_prw1326.vanilla_chair.level=1024;
	_prw1326.vanilla_chair.limit=820; //cut off a little early, since 820-884 very improbable

	const int k_first_third_1326_range  =  442;
	const int k_second_third_1326_range =  884;
	const int k_third_third_1326_range  = 1326;

	// now assign a weight table. Assume upper third fully probable, next third reducing
	// probability, lowest third not played.
	for(int i=0; i<k_first_third_1326_range; i++)
	{
		_prw1326.vanilla_chair.weight[i]=_prw1326.vanilla_chair.level;
	}
	for(int i=k_first_third_1326_range; i<k_second_third_1326_range; i++)
	{
		_prw1326.vanilla_chair.weight[i]=_prw1326.vanilla_chair.level*(884-i)/442;
	}
	for(int i=k_second_third_1326_range; i<k_third_third_1326_range; i++)
	{
		_prw1326.vanilla_chair.weight[i]=0;
	}

	CloneVanillaChairToAllOtherChairs();
}

void CIteratorThread::CloneVanillaChairToAllOtherChairs()
{
	// finally copy the vanilla to all user chairs so that someone who just turns on prw1326
	// experimentally does not cause a crash
	for(int i=0; i<k_max_number_of_players; i++)
	{
		_prw1326.chair[i]=_prw1326.vanilla_chair;
	}
}

void CIteratorThread::StandardDealingAlgorithm(int nopponents)
{
	// Normal prwin opponent card selection
	// Random ranges for everybody
	// Number of opponents might be adapted to get more "reasonable" results
	write_log(preferences.debug_prwin(), "[PrWinThread] Using standard prwin.\n");

	// if f$prwin_number_of_opponents<=13 then deal with random replacement algorithm, otherwise deal with swap algorithm
	if (nopponents <= 13)
	{
		StandardDealingAlgorithmForUpTo13Opponents(nopponents);
	}
	else
	{
		SwapDealingAlgorithmForMoreThan13Opponents(nopponents);
	}
}

void CIteratorThread::SwapDealingAlgorithmForMoreThan13Opponents(int nopponents)
{
	write_log(preferences.debug_prwin(), "[PrWinThread] Useing swap-algorithm, as f$prwin_number_of_opponents > 13\n");
	// swap alogorithm
	// weighted prwin not implemented for this case
	int numberOfCards = k_number_of_cards_per_deck;
	for (int i=0; i<numberOfCards; i++)
	{
		deck[i] = i;
	}

	while (numberOfCards >= 1)
	{
		int selected_card = rand() % numberOfCards;
		numberOfCards--;
		if (selected_card != numberOfCards)
		{
			SwapInts(&deck[selected_card], &deck[numberOfCards]);
		}
	}

	// opponent cards
	int x = 0;
	for (int i=0; 
		i<nopponents*k_number_of_cards_per_player; 
		i++)
	{
		while (CardMask_CARD_IS_SET(usedCards, deck[x]) 
			&& (x < k_number_of_cards_per_deck)) 
		{
			x++;
		}
		ocard[i] = deck[x++];
	}

	// additional common cards
	CardMask_RESET(addlcomCards);
	for (int i=0; i<(k_number_of_community_cards - _ncomCards); i++)
	{
		while (CardMask_CARD_IS_SET(usedCards, deck[x]) 
			&& (x < k_number_of_cards_per_deck)) 
		{
			x++;
		}
		CardMask_SET(addlcomCards, deck[x++]);
	}
}

int CIteratorThread::GetRandomCard()
{
	// We use bit-operations for truncation
	// That is better as modulo (52) which introdues a bias
	const int k_low_6_bits = 0x3F;
	// The search-loop is guaranteed to terminate,
	// as there are 52-cards in the deck, 
	// but at most 25 cards in use (10*2 player-cards + 5 community-cards).
	while (true)
	{
		int card = rand() & k_low_6_bits;
		if ((card < k_number_of_cards_per_deck)
			&& !CardMask_CARD_IS_SET(usedCards, card))
		{
			// We found a good card
			return card;
		}
	}
}

void CIteratorThread::StandardDealingAlgorithmForUpTo13Opponents(int nopponents)
{
	unsigned int	card = 0;

	write_log(preferences.debug_prwin(), "[PrWinThread] Using random algorithm, as f$prwin_number_of_opponents <= 13\n");
	// random replacement algorithm
	// opponent cards
	if (nopponents < 1)
	{
		write_log(preferences.debug_prwin(), "[PrWinThread] No opponents.\n");
	}
	for (int i=0; 
		i<nopponents*k_number_of_cards_per_player; 
		i+=k_number_of_cards_per_player)
	{
		temp_usedCards=usedCards;
		do
		{
			usedCards = temp_usedCards; //reset the card mask to clear settings from failed card assignments

			for (int j=0; j<k_number_of_cards_per_player; j++)
			{
				card = GetRandomCard();
				CardMask_SET(usedCards, card);
				ocard[i+j] = card;
			}

			if (!_willplay)
			{
				write_log(preferences.debug_prwin(), "[PrWinThread] Weighting disabled. Willplay is 0.\n");
				break; //0 disables weighting
			}

			//put break for i=0 and opponent unraised BB case (cannot assume anything about his cards)
			//In round 1 we should really do an analysis of chairs to find out how many have still to
			//place a bet. Not implemented since accuracy of prwin pre-flop is less critical.
			if (!i)
			{
				//if we called then we are not BB, BB limped to flop,
				//BB still playing, so do not weight his cards
				int betround = p_betround_calculator->betround();
				if (p_symbol_engine_history->nbetsround(betround) < 1.1 
					&& p_symbol_engine_history->didcall(betround) 
					&& (p_symbol_engine_active_dealt_playing->playersplayingbits() 
						& p_symbol_engine_blinds->bblindbits()))
				{
					break;
				}
			}
		} while (!IsHandInWeightedRange(ocard[i], ocard[i+1],
			_willplay, _wontplay, 
			_topclip, _mustplay));
	}
	// additional common cards
	CardMask_RESET(addlcomCards);
	for (int i=0; i<(k_number_of_community_cards - _ncomCards); i++)
	{
		card = GetRandomCard();
		CardMask_SET(usedCards, card);
		CardMask_SET(addlcomCards, card);
	}
}


void CIteratorThread::EnhancedDealingAlgorithm()
{
	// Dealing algorithm for enhanced prwin
	// (user-defined weighting at DLL-level).
	write_log(preferences.debug_prwin(), "[PrWinThread] Using Matrix's enhanced prwin.\n");

	//prw1326 active  Matrix 2008-05-08
	int k = 0; //k is used as an index into ocard[] 
	unsigned int	card = 0;

	int userchair = p_symbol_engine_userchair->userchair();
	int playersplayingbits = p_symbol_engine_active_dealt_playing->playersplayingbits();
	//we have to use actual opponents for prw1326 calculations
	int nopponents = bitcount(playersplayingbits & ~(1 << userchair));
	int betround   = p_betround_calculator->betround();
	int bblindbits = p_symbol_engine_blinds->bblindbits();

	// loop through active opponents
	for(int i=0; i<k_max_number_of_players; i++) 
	{
		if (i == userchair)
			continue; //skip our own chair!

		if (!(playersplayingbits & (1<<i)))
			continue; //skip inactive chairs 

		// first deal with the special non-weighted cases
		// player who is marked 'ignore' or one who is BB and has not VPIP'd
		if (_prw1326.chair[i].ignore || 
			(_prw1326.bblimp && p_symbol_engine_history->nbetsround(betround)<1.1 
				&& (bblindbits&(1<<i))) )
		{
			card = GetRandomCard();
			CardMask_SET(usedCards, card);
			ocard[k++] = card;

			card = GetRandomCard();
			CardMask_SET(usedCards, card);
			ocard[k++] = card;

			continue;
		} // end of special non-weighted cases

		int randfix=(RAND_MAX/_prw1326.chair[i].limit) * _prw1326.chair[i].limit;

		int j;
		while (true)
		{ //find a possible hand for this chair NOTE: may want to put in loop limits to prevent hanging
			do 
			{
				j=rand();
			} while (j>=randfix);

			j = j % _prw1326.chair[i].limit; //j is now any one of the allowed hands

			if(CardMask_CARD_IS_SET(usedCards, _prw1326.chair[i].rankhi[j] ))
				continue; //hand contains dead card

			if(CardMask_CARD_IS_SET(usedCards, _prw1326.chair[i].ranklo[j] ))
				continue; //hand contains dead card

//					if(symbols.prw1326.chair[i].ignore)break; //chair marked as not to be weighted

			if(_prw1326.chair[i].level <= _prw1326.chair[i].weight[j])
				break; //hand marked as always uae

			//check if we want a player who is BB and has not VPIP'd to be analysed further
//					if(symbols.prw1326.bblimp)
//					{
//					if ((symbols.sym.nbetsround[0]<1.1) && ((int)symbols.sym.bblindbits&(1<<i)))break;
//					}

			//we should really do a 'randfix' here for the case where RAND_MAX is not an integral
			//multiple of .level, but the bias introduced is trivial compared to other uncertainties.
			if(rand() % _prw1326.chair[i].level < _prw1326.chair[i].weight[j])
				break; //allowable

			//if we reach here we will loop again to find a suitable hand
		} //end of possible hand find

		ocard[k++] = _prw1326.chair[i].rankhi[j];
		ocard[k++] = _prw1326.chair[i].ranklo[j];

		CardMask_SET(usedCards, ocard[k-2]);
		CardMask_SET(usedCards, ocard[k-1]);

	} //end of active opponent loop

	// additional common cards
	CardMask_RESET(addlcomCards);
	for (int i=0; i<(k_number_of_community_cards - _ncomCards); i++)
	{
		card = GetRandomCard();
		CardMask_SET(usedCards, card);
		CardMask_SET(addlcomCards, card);
	}
} //end of prw1326 code

bool CIteratorThread::UseEnhancedPrWin()
{
	return (_prw1326.useme==1326 
		&& (p_betround_calculator->betround() >= k_betround_flop 
			|| _prw1326.preflop==1326));
}