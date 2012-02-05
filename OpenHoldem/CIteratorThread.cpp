#include "stdafx.h"
#include "CIteratorThread.h"

#include <process.h>
#include "CGrammar.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSymbols.h"
#include "inlines/eval.h"
#include "MagicNumbers.h"

CIteratorThread		*p_iterator_thread = NULL;

CIteratorVars		iter_vars;

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

	p_iterator_thread = NULL;

	write_log(prefs.debug_prwin(), "[PrWinThread] Iterator Thread ended.\n");
}

UINT CIteratorThread::IteratorThreadFunction(LPVOID pParam)
{
	CIteratorThread *pParent = static_cast<CIteratorThread*>(pParam);

	// Loop-variables j, k get used inside and outside loops.
	// It is a bit messy, nearly impossible to fix it.
	// At least the outer loops ("nit" and "i") could be improved.
	int				j = 0, k = 0;
	int				randfix = 0;
	CardMask		addlcomCards = {0}, evalCards = {0}, opp_evalCards = {0}, usedCards = {0}, temp_usedCards = {0};
	unsigned int	ocard[MAX_OPPONENTS*2] = {0}, card = 0, pl_pokval = 0, opp_pokval = 0, opp_pokvalmax = 0;
	HandVal			pl_hv = 0, opp_hv = 0;
	double			dummy = 0;
	unsigned int	deck[52] = {0}, x = 0, swap = 0;
	int				numberOfCards = 0;

	int				sym_br = (int) p_symbols->sym()->br;
	int				sym_playersplayingbits = (int) p_symbols->sym()->playersplayingbits;
	double			sym_nbetsround = p_symbols->sym()->nbetsround[0];
	int				sym_bblindbits = (int) p_symbols->sym()->bblindbits;
	bool			sym_didcall = (bool) p_symbols->sym()->didcall[0];
	int				sym_nopponents = (int) p_symbols->sym()->nopponents;

	int				nopp = sym_nopponents <= MAX_OPPONENTS ? sym_nopponents : MAX_OPPONENTS;
	bool			hand_lost;

	// Seed the RNG
	srand((unsigned)GetTickCount());

	//
	// Main iterator loop
	//
	write_log(prefs.debug_prwin(), "[PrWinThread] Start of main loop.\n");

	// "nit" has to be declared outside of the loop,
	// as we check afterwards, if the loop terminated successfully.
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
		//Correct the protection aganst low f$willplay/f$wontplay - Matrix 2008-12-22
		if (pParent->_willplay && (pParent->_willplay<nopp*2+1) )
		{
			write_log(prefs.debug_prwin(), "[PrWinThread] Adjusting willplay (too low)\n");
			pParent->_willplay=nopp*2+1; //too low a value can give lockup
		}
		if (pParent->_wontplay<pParent->_willplay)
		{
			write_log(prefs.debug_prwin(), "[PrWinThread] Adjusting wontplay (too low)\n");
			pParent->_wontplay=pParent->_willplay; //wontplay cannot safely be less than willplay
		}


		if (p_symbols->prw1326()->useme==1326 && (sym_br!=1 || p_symbols->prw1326()->preflop==1326))
		{
			write_log(prefs.debug_prwin(), "[PrWinThread] Using Matrix's enhanced prwin.\n");

			//prw1326 active  Matrix 2008-05-08
			k = nopp = 0; //k is used as an index into ocard[] 

			// loop through active opponents
			for(int i=0; i<k_max_number_of_players; i++) 
			{
				if (i==(int) p_symbols->sym()->userchair)
					continue; //skip our own chair!

				if (!((sym_playersplayingbits) & (1<<i)))
					continue; //skip inactive chairs 

				nopp++; //we have to use actual opponents for prw1326 calculations

				// first deal with the special non-weighted cases
				// player who is marked 'ignore' or one who is BB and has not VPIP'd
				if ( p_symbols->prw1326()->chair[i].ignore || 
					(p_symbols->prw1326()->bblimp && sym_nbetsround<1.1 && (sym_bblindbits&(1<<i))) )
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

				randfix=(RAND_MAX/p_symbols->prw1326()->chair[i].limit) * p_symbols->prw1326()->chair[i].limit;


				while (true)
				{ //find a possible hand for this chair NOTE: may want to put in loop limits to prevent hanging
					do 
					{
						j=rand();
					} while (j>=randfix);

					j = j % p_symbols->prw1326()->chair[i].limit; //j is now any one of the allowed hands

					if(CardMask_CARD_IS_SET(usedCards, p_symbols->prw1326()->chair[i].rankhi[j] ))
						continue; //hand contains dead card

					if(CardMask_CARD_IS_SET(usedCards, p_symbols->prw1326()->chair[i].ranklo[j] ))
						continue; //hand contains dead card

//					if(symbols.prw1326.chair[i].ignore)break; //chair marked as not to be weighted

					if(p_symbols->prw1326()->chair[i].level <= p_symbols->prw1326()->chair[i].weight[j])
						break; //hand marked as always uae

					//check if we want a player who is BB and has not VPIP'd to be analysed further
//					if(symbols.prw1326.bblimp)
//					{
//					if ((symbols.sym.nbetsround[0]<1.1) && ((int)symbols.sym.bblindbits&(1<<i)))break;
//					}

					//we should really do a 'randfix' here for the case where RAND_MAX is not an integral
					//multiple of .level, but the bias introduced is trivial compared to other uncertainties.
					if(rand() % p_symbols->prw1326()->chair[i].level < p_symbols->prw1326()->chair[i].weight[j])
						break; //allowable

					//if we reach here we will loop again to find a suitable hand
				} //end of possible hand find

				ocard[k++] = p_symbols->prw1326()->chair[i].rankhi[j];
				ocard[k++] = p_symbols->prw1326()->chair[i].ranklo[j];

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

			// if f$P<=13 then deal with random replacement algorithm, otherwise deal with swap algorithm
			if (nopp <= 13)
			{
				write_log(prefs.debug_prwin(), "[PrWinThread] Using random algorithm, as f$P <= 13\n");
				// random replacement algorithm
				// opponent cards
				if (nopp < 1)
				{
					write_log(prefs.debug_prwin(), "[PrWinThread] No opponents.\n");
				}
				for (int i=0; i<nopp*2; i+=2)
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
				write_log(prefs.debug_prwin(), "[PrWinThread] Useing swap-algorithm, as f$P > 13\n");
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
						swap = deck[x];
						deck[x] = deck[numberOfCards];
						deck[numberOfCards] = swap;
					}
				}

				// opponent cards
				x = 0;
				for (int i=0; i<nopp*2; i++)
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
		pl_pokval = p_symbols->CalcPokerval(pl_hv, 7, &dummy, CARD_NOCARD, CARD_NOCARD);

		// Scan through opponents' handvals/pokervals
		// - if we find one better than ours, then we are done, increment los
		// - for win/tie, we need to wait until we scan them all
		opp_pokvalmax = 0;
		hand_lost = false;
		for (int i=0; i<nopp; i++)
		{
			CardMask_RESET(opp_evalCards);
			CardMask_OR(opp_evalCards, pParent->_comCards, addlcomCards);
			CardMask_SET(opp_evalCards, ocard[i*2]);
			CardMask_SET(opp_evalCards, ocard[(i*2)+1]);
			opp_hv = Hand_EVAL_N(opp_evalCards, 7);
			opp_pokval = p_symbols->CalcPokerval(opp_hv, 7, &dummy, CARD_NOCARD, CARD_NOCARD);

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

		if ((nit/1000 == (int) nit/1000) && nit>=1000)
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
	delete p_iterator_thread;
	p_iterator_thread = NULL;

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
	iter_vars.set_nit((int) p_symbols->sym()->nit);
	iter_vars.set_f$p((int) p_symbols->sym()->nopponents);
	iter_vars.set_br((int) p_symbols->sym()->br);

	for (int i=0; i<k_number_of_cards_per_player; i++)
		iter_vars.set_pcard(i, p_scraper->card_player((int) p_symbols->sym()->userchair, i));

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
	if (p_symbols->prw1326()->useme==1326 && 
		p_symbols->prw1326()->usecallback==1326 && 
		(p_symbols->sym()->br!=1 || p_symbols->prw1326()->preflop==1326) )
	{
		p_symbols->prw1326()->prw_callback(); //Matrix 2008-05-09
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
