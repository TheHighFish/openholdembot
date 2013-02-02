#include "stdafx.h"
#include "CIteratorThread.h"

#include <process.h>
#include "CBetroundCalculator.h"
#include "CGrammar.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineBlinds.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineHistory.h"
#include "CSymbolEnginePokerval.h"
#include "CSymbolEnginePrwin.h"
#include "CSymbols.h"
#include "inlines/eval.h"
#include "MagicNumbers.h"
#include "Numericalfunctions.h"


CIteratorThread		*p_iterator_thread = NULL;

CIteratorVars		iter_vars;

//weighted prwin lookup tables for non-suited and suited cards
int pair2ranko[170] = {0}, pair2ranks[170] = {0};
//used to resolve ascii cards to numbers for init of above
char ctonum[14]="23456789TJQKA";
//int willplay = 0, wontplay = 0, topclip = 0, mustplay = 0
sprw1326	_prw1326;								//prwin 1326 data structure Matrix 2008-04-29


//handrank table used to prime weighted prwin lookup table.
//reflects incidence of cards people actually play to flop.
//left in this form for ease of developer modification.
//converted at startup to the tables actually used by prwin calculation
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

CIteratorVars::CIteratorVars()
{
	ResetVars();
}

CIteratorVars::~CIteratorVars()
{
}

void CIteratorVars::ResetVars()
{
	write_log(prefs.debug_prwin(), "[PrWinThread] Reset variables\n");
	_nit = 0;
	_f$p = 0;
	_br = 0;

	for (int i=0; i<k_number_of_cards_per_player; i++)
		_pcard[i] = CARD_NOCARD;

	for (int i=0; i<k_number_of_community_cards; i++)
		_ccard[i] = CARD_NOCARD;

	_prwin = 0;
	_prtie = 0;
	_prlos = 0;
	_iterator_thread_running = false;
	_iterator_thread_complete = true;
	_iterator_thread_progress = 0;
}



CIteratorThread::CIteratorThread()
{
	write_log(prefs.debug_prwin(), "[PrWinThread] Iterator Thread starting.\n");

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

	write_log(prefs.debug_prwin(), "[PrWinThread] Iterator Thread started.\n");
}

CIteratorThread::~CIteratorThread()
{
	write_log(prefs.debug_prwin(), "[PrWinThread] Iterator Thread ending...\n");

	// Trigger thread to die
	::SetEvent(_m_stop_thread);

	// Wait until thread finished
	::WaitForSingleObject(_m_wait_thread, INFINITE);

	// Close handles
	::CloseHandle(_m_stop_thread);
	::CloseHandle(_m_wait_thread);

	StopIteratorThread();

	write_log(prefs.debug_prwin(), "[PrWinThread] Iterator Thread ended.\n");
}

void CIteratorThread::StopIteratorThread()
{
	if (p_iterator_thread)
	{
		write_log(prefs.debug_prwin(), "[PrWinThread] Stopping iterator thread.\n");
		delete p_iterator_thread;
		p_iterator_thread = NULL;
	}
}

void CIteratorThread::RestartIteratorThread()
{
	StopIteratorThread();
	write_log(prefs.debug_prwin(), "[PrWinThread] Restarting iterator thread.\n");
	p_iterator_thread = new CIteratorThread;
}

void CIteratorThread::RestartIteratorThreadIfNotRunning()
{
	if (p_iterator_thread)
	{
		write_log(prefs.debug_prwin(), "[PrWinThread] IteratorThread running. No need to restart.\n");
	}
	else
	{
		write_log(prefs.debug_prwin(), "[PrWinThread] IteratorThread not running. Going to restart.\n");
		RestartIteratorThread();
	}
}

void CIteratorThread::AdjustPrwinVariablesIfNecessary(CIteratorThread *pParent)
{
	// Cut off from IteratorThreadFunction
	// Also moved outside of the loop.

	int	sym_nopponents = p_symbol_engine_prwin->nopponents_for_prwin();
	//Correct the protection aganst low f$willplay/f$wontplay - Matrix 2008-12-22
	if (pParent->_willplay && (pParent->_willplay < 2 * sym_nopponents + 1))
	{
		write_log(prefs.debug_prwin(), "[PrWinThread] Adjusting willplay (too low)\n");
		pParent->_willplay = 2 * sym_nopponents + 1; //too low a value can give lockup
	}
	if (pParent->_wontplay < pParent->_willplay)
	{
		write_log(prefs.debug_prwin(), "[PrWinThread] Adjusting wontplay (too low)\n");
		pParent->_wontplay = pParent->_willplay; //wontplay cannot safely be less than willplay
	}
}

UINT CIteratorThread::IteratorThreadFunction(LPVOID pParam)
{
	CIteratorThread *pParent = static_cast<CIteratorThread*>(pParam);

	// Loop-variables j, k get used inside and outside loops.
	// It is a bit messy, nearly impossible to fix it.
	// At least the outer loops ("f$prwin_number_of_iterations" and "i") could be improved.
	int				j = 0, k = 0;
	int				randfix = 0;
	CardMask		addlcomCards = {0}, evalCards = {0}, opp_evalCards = {0}, usedCards = {0}, temp_usedCards = {0};
	unsigned int	ocard[MAX_OPPONENTS*2] = {0}, card = 0, pl_pokval = 0, opp_pokval = 0, opp_pokvalmax = 0;
	HandVal			pl_hv = 0, opp_hv = 0;
	int				dummy = 0;
	int				deck[k_number_of_cards_per_deck] = {0}, x = 0;
	int				numberOfCards = 0;

	int				betround = p_betround_calculator->betround();
	int				sym_playersplayingbits = p_symbol_engine_active_dealt_playing->playersplayingbits();
	double			sym_nbetsround = p_symbol_engine_history->nbetsround(betround);
	int				sym_bblindbits = p_symbol_engine_blinds->bblindbits();
	bool			sym_didcall = p_symbol_engine_history->didcall(betround);
	int				sym_nopponents = p_symbol_engine_prwin->nopponents_for_prwin();

	bool			hand_lost;

	// Seed the RNG
	srand((unsigned)GetTickCount());

	//
	// Main iterator loop
	//
	write_log(prefs.debug_prwin(), "[PrWinThread] Start of main loop.\n");

	// "f$prwin_number_of_iterations" has to be declared outside of the loop,
	// as we check afterwards, if the loop terminated successfully.
	unsigned int nit;
	AdjustPrwinVariablesIfNecessary(pParent);
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
		
		if (_prw1326.useme==1326 && (betround>=k_betround_flop || _prw1326.preflop==1326))
		{
			write_log(prefs.debug_prwin(), "[PrWinThread] Using Matrix's enhanced prwin.\n");

			//prw1326 active  Matrix 2008-05-08
			k = sym_nopponents = 0; //k is used as an index into ocard[] 

			// loop through active opponents
			for(int i=0; i<k_max_number_of_players; i++) 
			{
				if (i==(int) p_symbol_engine_userchair->userchair())
					continue; //skip our own chair!

				if (!((sym_playersplayingbits) & (1<<i)))
					continue; //skip inactive chairs 

				sym_nopponents++; //we have to use actual opponents for prw1326 calculations

				// first deal with the special non-weighted cases
				// player who is marked 'ignore' or one who is BB and has not VPIP'd
				if (_prw1326.chair[i].ignore || 
					(_prw1326.bblimp && sym_nbetsround<1.1 && (sym_bblindbits&(1<<i))) )
				{
					do 
					{
						card = rand() & 63;
					}
					while (card>51 || CardMask_CARD_IS_SET(usedCards, card));
					
					CardMask_SET(usedCards, card);
					ocard[k++] = card;

					do 
					{
						card = rand() & 63;
					}
					while (card>51 || CardMask_CARD_IS_SET(usedCards, card));
					
					CardMask_SET(usedCards, card);
					ocard[k++] = card;

					continue;
				} // end of special non-weighted cases

				randfix=(RAND_MAX/_prw1326.chair[i].limit) * _prw1326.chair[i].limit;


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
			for (int i=0; i<(k_number_of_community_cards - pParent->_ncomCards); i++)
			{
				do 
				{
					card = rand() & 63;
				}
				while (card>51 ||CardMask_CARD_IS_SET(usedCards, card));
				CardMask_SET(usedCards, card);
				CardMask_SET(addlcomCards, card);
			}
		} //end of prw1326 code
		
		else
		{ 
			// normal prwin opponent card selection
			write_log(prefs.debug_prwin(), "[PrWinThread] Using standard prwin.\n");

			// if f$prwin_number_of_opponents<=13 then deal with random replacement algorithm, otherwise deal with swap algorithm
			if (sym_nopponents <= 13)
			{
				write_log(prefs.debug_prwin(), "[PrWinThread] Using random algorithm, as f$prwin_number_of_opponents <= 13\n");
				// random replacement algorithm
				// opponent cards
				if (sym_nopponents < 1)
				{
					write_log(prefs.debug_prwin(), "[PrWinThread] No opponents.\n");
				}
				for (int i=0; i<sym_nopponents*2; i+=2)
				{
					temp_usedCards=usedCards;
					do
					{
						usedCards=temp_usedCards; //reset the card mask to clear settings from failed card assignments

						do {
							card = rand() & 63;
						}
						while (card>51 || CardMask_CARD_IS_SET(usedCards, card));
						CardMask_SET(usedCards, card);
						ocard[i] = card;

						do {
							card = rand() & 63;
						}
						while (card>51 || CardMask_CARD_IS_SET(usedCards, card));
						CardMask_SET(usedCards, card);
						ocard[i+1] = card;

						if (!pParent->_willplay)
						{
							write_log(prefs.debug_prwin(), "[PrWinThread] Weighting disabled. Willplay is 0.\n");
							break; //0 disables weighting
						}

						//put break for i=0 and opponent unraised BB case (cannot assume anything about his cards)
						//In round 1 we should really do an analysis of chairs to find out how many have still to
						//place a bet. Not implemented since accuracy of prwin pre-flop is less critical.
						if (!i)
						{
							//if we called then we are not BB, BB limped to flop,
							//BB still playing, so do not weight his cards
							if (sym_nbetsround<1.1 && sym_didcall && (sym_playersplayingbits&sym_bblindbits) )
							{
								break;
							}
						}
					}
					while (!pParent->InRange(ocard[i], ocard[i+1], pParent->_willplay, 
							pParent->_wontplay, pParent->_topclip, pParent->_mustplay));
				}
				// additional common cards
				CardMask_RESET(addlcomCards);
				for (int i=0; i<(k_number_of_community_cards - pParent->_ncomCards); i++)
				{
					do {
						card = rand() & 63;
					}
					while (card>51 ||CardMask_CARD_IS_SET(usedCards, card));
					CardMask_SET(usedCards, card);
					CardMask_SET(addlcomCards, card);
				}
			}

			else
			{
				write_log(prefs.debug_prwin(), "[PrWinThread] Useing swap-algorithm, as f$prwin_number_of_opponents > 13\n");
				// swap alogorithm
				//weighted prwin not implemented for this case
				numberOfCards=52;
				for (int i=0; i<numberOfCards; i++)
					deck[i] = i;

				while (numberOfCards>=1)
				{
					x = rand() % numberOfCards;
					numberOfCards--;
					if (x != numberOfCards)
					{
						SwapInts(&deck[x], &deck[numberOfCards]);
					}
				}

				// opponent cards
				x = 0;
				for (int i=0; i<sym_nopponents*2; i++)
				{
					while (CardMask_CARD_IS_SET(usedCards, deck[x]) && x<=51) {
						x++;
					}
					ocard[i] = deck[x++];
				}

				// additional common cards
				CardMask_RESET(addlcomCards);
				for (int i=0; i<(k_number_of_community_cards - pParent->_ncomCards); i++)
				{
					while (CardMask_CARD_IS_SET(usedCards, deck[x]) && x<=51) {
						x++;
					}
					CardMask_SET(addlcomCards, deck[x++]);
				}
			}
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
				pParent->_los++;
				hand_lost = true;
				break;
			}
			else
			{
				if (opp_pokval > opp_pokvalmax)
					opp_pokvalmax = opp_pokval;
			}
		}
		if (!hand_lost)
		{
			if (pl_pokval > opp_pokvalmax)
				pParent->_win++;
			else
				pParent->_tie++;
		}

		// Update display once every 1000 iterations
		if ((nit % 1000 == 0) && nit >= 1000)
		{
			write_log(prefs.debug_prwin(), "[PrWinThread] Progress: %d %.3f %.3f %.3f\n", nit, pParent->_win / (double) nit, pParent->_tie / (double) nit, pParent->_los / (double) nit);
			iter_vars.set_iterator_thread_progress(nit);
			iter_vars.set_prwin(pParent->_win / (double) nit);
			iter_vars.set_prtie(pParent->_tie / (double) nit);
			iter_vars.set_prlos(pParent->_los / (double) nit);
		}
	}

	write_log(prefs.debug_prwin(), "[PrWinThread] End of main loop.\n");

	if (nit >= iter_vars.nit())
	{
		iter_vars.set_iterator_thread_running(false);
		iter_vars.set_iterator_thread_complete(true);
		iter_vars.set_iterator_thread_progress(nit);
		iter_vars.set_prwin(pParent->_win / (double) iter_vars.nit());
		iter_vars.set_prtie(pParent->_tie / (double) iter_vars.nit());
		iter_vars.set_prlos(pParent->_los / (double) iter_vars.nit());
	}
	else
	{
		iter_vars.set_iterator_thread_running(false);
		iter_vars.set_iterator_thread_complete(false);
		iter_vars.set_iterator_thread_progress(0);
		iter_vars.set_nit(0);
		iter_vars.set_f$p(0);
		iter_vars.set_br(0);

		for (int i=0; i<k_number_of_cards_per_player; i++)
			iter_vars.set_pcard(i, CARD_NOCARD);

		for (int i=0; i<k_number_of_community_cards; i++)
			iter_vars.set_ccard(i, CARD_NOCARD);

		iter_vars.set_prwin(0);
		iter_vars.set_prtie(0);
		iter_vars.set_prlos(0);
	}

	::SetEvent(pParent->_m_wait_thread);
	StopIteratorThread();

	return 0;
}

void CIteratorThread::InitIteratorLoop()
{
	int			e = SUCCESS;
	CGrammar	gram;

	write_log(prefs.debug_prwin(), "[PrWinThread] Initializing iterator loop\n");
	// Set starting status and parameters
	iter_vars.set_iterator_thread_running(true);
	iter_vars.set_iterator_thread_complete(false);
	iter_vars.set_iterator_thread_progress(0);
	iter_vars.set_nit(10000); //!! f$prwin_number_of_iterations")
	iter_vars.set_f$p(p_symbol_engine_prwin->nopponents_for_prwin());
	iter_vars.set_br(p_betround_calculator->betround());

	for (int i=0; i<k_number_of_cards_per_player; i++)
		iter_vars.set_pcard(i, p_scraper->card_player((int) p_symbol_engine_userchair->userchair(), i));

	for (int i=0; i<k_number_of_community_cards; i++)
		iter_vars.set_ccard(i, p_scraper->card_common(i));

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
		if (iter_vars.pcard(i) != CARD_BACK && iter_vars.pcard(i) != CARD_NOCARD)
		{
			CardMask_SET(_plCards, iter_vars.pcard(i));
			_nplCards++;
		}
	}
	for (int i=0; i<k_number_of_community_cards; i++)
	{
		if (iter_vars.ccard(i) != CARD_BACK && iter_vars.ccard(i) != CARD_NOCARD)
		{
			CardMask_SET(_comCards, iter_vars.ccard(i));
			_ncomCards++;
		}
	}

	//Weighted prwin only for nopponents <=13
	e = SUCCESS;
	_willplay = (int) gram.CalcF$symbol(p_formula, "f$willplay", &e);
	e = SUCCESS;
	_wontplay = (int) gram.CalcF$symbol(p_formula, "f$wontplay", &e);
	e = SUCCESS;
	_mustplay = (int) gram.CalcF$symbol(p_formula, "f$mustplay", &e);
	e = SUCCESS;
	_topclip = (int) gram.CalcF$symbol(p_formula, "f$topclip", &e);

	// Call prw1326 callback if needed
	if (_prw1326.useme==1326 && 
		_prw1326.usecallback==1326 && 
		(p_betround_calculator->betround()!=1 || _prw1326.preflop==1326) )
	{
		_prw1326.prw_callback(); //Matrix 2008-05-09
	}
}

int CIteratorThread::InRange(const int card1, const int card2, const int willplay, 
							 const int wontplay, const int topclip, const int mustplay)
{
	//returns non-zero if card pair acceptable for prwin calculation

	/*
	p |---|	---------
	r |   |   /      \
	o |   |  /        \
	b |   | /          \
	a |   |/            \
	b |                  \
	l -----------------------------------------------------
	e	 1	2	   3	 4	 handrank169 ->
	1=mustplay
	2=topclip
	3=willplay
	4=wontplay
	(with the current statistics available in OH only willplay and wontplay should be used.
	 mustplay and topclip are debatable constructs at the best of times 2008-01-29)
	*/
	
	extern int pair2ranko[170], pair2ranks[170];
	int i = card1%13;
	int j = card2%13;
	int	hrank=0;

	if (j>i) //normalise the card order
	{
		i=j;
		j=card1%13;
	}
	i=i*13+j; //offset into handrank table

	if (card1/13==card2/13)  //same suit
		hrank=pair2ranks[i]; //suited
	else
		hrank=pair2ranko[i]; //not suited

	if (hrank>=wontplay)return 0; //no good, never play these

	if (hrank<=mustplay) return 1; //OK, assumed opponent will always play these

	if ((hrank>topclip)&&(hrank<=willplay))return 1;//OK, in the 100% region

	//we now handle the probability slopes
	if (hrank<=topclip) //handle the good cards not likely to be one-betted with
	{
		j=topclip-rand()%topclip;
		if (hrank>j)return 1; //OK
		else return 0; //nogood
	}

	//now finish with poorish hands of reduced probability
	j=wontplay-willplay;
	if (j<3)
		return 1; //protect ourselves from effect of bad wontplay

	j=willplay+rand()%j;

	if (hrank<j)
		return 1; //OK

	return 0; //no good
}

void CIteratorThread::InitHandranktTableForPrwin()
{
	int		i = 0, j = 0, k = 0; 
	int		vndx = 0;
	char	*ptr = NULL;

	//Initialise the handrank tables used by prwin
	vndx=0; //used to provide an offset into the vanilla table
	for (i=0;i<169;i++)
	{
		//normal weighted prwin table
		ptr = prwin_handrank_table_169[i];
		j=(strchr(ctonum,*ptr)-ctonum)*13 + (strchr(ctonum,*(ptr+1))-ctonum);
		if (*(ptr+2)=='s')pair2ranks[j]=i+1;
		else pair2ranko[j]=i+1;
		//prw1326 vanilla table
		j=strchr(ctonum,*ptr)-ctonum;
		k=strchr(ctonum,*(ptr+1))-ctonum;
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
			_prw1326.vanilla_chair.rankhi[vndx+11]=j+39; //s Matrix corrected typo
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

	// now assign a weight table. Assume upper third fully probable, next third reducing
	// probability, lowest third not played.
	for(i=0;i<442;i++)
		_prw1326.vanilla_chair.weight[i]=_prw1326.vanilla_chair.level;
	for(i=442;i<884;i++)
		_prw1326.vanilla_chair.weight[i]=_prw1326.vanilla_chair.level*(884-i)/442;
	for(i=884;i<1326;i++)
		_prw1326.vanilla_chair.weight[i]=0;

	//finally copy the vanilla to all user chairs so that someone who just turns on prw1326
	//experimentally does not cause a crash
	for(i=0;i<10;i++)
		_prw1326.chair[i]=_prw1326.vanilla_chair ;

	//end of handrank initialisation
}