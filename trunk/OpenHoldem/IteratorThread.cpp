#include "stdafx.h"
#include <process.h>

#include "debug.h"
#include "scraper.h"
#include "global.h"
#include "inlines/eval.h"
#include "symbols.h"
#include "grammar.h"
#include "IteratorThread.h"

CRITICAL_SECTION	cs_iterator;

CIteratorThread		*p_iterator_thread = NULL;

bool				iterator_thread_running = false;
bool				iterator_thread_complete = false;
unsigned int		iterator_thread_progress=0;
double				iterator_run_with_nit=0;
double				iterator_run_with_f$p=0;
double				iterator_run_with_br = 0;
unsigned int		iterator_run_with_playercards[2] = {CARD_NOCARD};
unsigned int		iterator_run_with_commoncards[5] = {CARD_NOCARD};


CIteratorThread::CIteratorThread()
{
    __SEH_SET_EXCEPTION_HANDLER(MyUnHandledExceptionFilter);

    __SEH_HEADER

	int i;

	// Create events
	m_StopThread = CreateEvent(0, TRUE, FALSE, 0);
	m_WaitThread = CreateEvent(0, TRUE, FALSE, 0);
	m_NormalExitThread = CreateEvent(0, TRUE, FALSE, 0);

	// Initialize variables
	// Get exclusive access to CIteratorThread variables
	EnterCriticalSection(&cs_iterator);

	iterator_thread_running = false;
	iterator_thread_complete = false;
	iterator_thread_progress=0;
	iterator_run_with_nit=0;
	iterator_run_with_f$p=0;
	iterator_run_with_br=0;

    for (i=0; i<=1; i++)
        iterator_run_with_playercards[i] = CARD_NOCARD;

    for (i=0; i<=4; i++)
        iterator_run_with_commoncards[i] = CARD_NOCARD;

	// Allow other threads to use CIterator variables
	LeaveCriticalSection(&cs_iterator);

	// Start thread
	AfxBeginThread(iterator_thread_function, this);

	// Get exclusive access to CIteratorThread variables
	EnterCriticalSection(&cs_iterator);

	iterator_thread_running = true;

	// Allow other threads to use CIterator variables
	LeaveCriticalSection(&cs_iterator);

	__SEH_LOGFATAL("CIteratorThread::CIteratorThread :\n");
}

CIteratorThread::~CIteratorThread()
{
    __SEH_HEADER

	// Check for normal thread exit
	if(::WaitForSingleObject(m_NormalExitThread, 0) != WAIT_OBJECT_0)
	{
		// Trigger thread to stop
		::SetEvent(m_StopThread);

		// Wait until thread finished
		::WaitForSingleObject(m_WaitThread, INFINITE);
	}

	// Close handles
	::CloseHandle(m_StopThread);
	::CloseHandle(m_WaitThread);

	// Get exclusive access to CIteratorThread variables
	EnterCriticalSection(&cs_iterator);

	iterator_thread_running = false;

	// Allow other threads to use CIterator variables
	LeaveCriticalSection(&cs_iterator);

	__SEH_LOGFATAL("CIteratorThread::~CIteratorThread :\n");
}

UINT CIteratorThread::iterator_thread_function(LPVOID pParam)
{
    __SEH_HEADER

	CIteratorThread *pParent = static_cast<CIteratorThread*>(pParam);

	int				i,j,k,randfix, nit = 0; //modified by Matrix 2008-05-08
    CardMask		plCards, comCards, addlcomCards, evalCards, usedCards, temp_usedCards;
    int				nplCards, ncomCards, nopp;
    extern int		willplay,wontplay,mustplay,topclip;
    int				error;
    unsigned int	ocard[MAX_OPPONENTS*2], card, pl_pokval, opp_pokval, opp_pokvalmax;
    double			win, tie, los;
    HandVal			pl_hv, opp_hv;
    double			dummy = 0;
    unsigned int	deck[52], x, swap;
    int				numberOfCards;

	// variables we grab from the symbols class for use in the iterator, to avoid grabbing
	// too many critical sections in every iteration
	int			prw1326_useme, prw1326_usecallback, prw1326_preflop, prw1326_bblimp;
	int			sym_playersplayingbits, sym_bblindbits, prw1326_chair_ignore[10], prw1326_chair_limit[10];
	double		sym_br, sym_userchair, sym_nbetsround_0, sym_didcall_0, sym_nopp, sym_nit;

	
	// Seed RNG
    srand((unsigned)time( NULL ));

    // player cards
    CardMask_RESET(plCards);
    CardMask_RESET(comCards);
    nplCards = ncomCards = 0;

	// Counters
	win = tie = los = 0;

	// Get exclusive access to CIteratorThread variables
    EnterCriticalSection(&cs_iterator);

	iterator_thread_complete = false;
    iterator_thread_progress = 0;

	// Allow other threads to use CIteratorThread variables
    LeaveCriticalSection(&cs_iterator);

	// Get exclusive access to CScraper and CSymbol variables
	EnterCriticalSection(&cs_scrape_symbol);	

		// Get the symbol/scraper variables we need
		prw1326_useme = symbols.prw1326.useme;
		prw1326_usecallback = symbols.prw1326.usecallback;
		sym_br = symbols.sym.br;
		prw1326_preflop = symbols.prw1326.preflop;
		sym_userchair = symbols.sym.userchair;
		sym_playersplayingbits = (int) symbols.sym.playersplayingbits;
		prw1326_bblimp = symbols.prw1326.bblimp;
		sym_nbetsround_0 = symbols.sym.nbetsround[0];
		sym_bblindbits = (int) symbols.sym.bblindbits;
		sym_didcall_0 = symbols.sym.didcall[0];
		sym_nopp = symbols.sym.nopponents;
		sym_nit = symbols.sym.nit;

		// player cards
		for (i=0; i<=1; i++)
		{
			if (scraper.card_player[(int) symbols.sym.userchair][i] != CARD_BACK &&
					scraper.card_player[(int) symbols.sym.userchair][i] != CARD_NOCARD)
			{
				CardMask_SET(plCards, scraper.card_player[(int) symbols.sym.userchair][i]);
				iterator_run_with_playercards[i] = scraper.card_player[(int) symbols.sym.userchair][i];
				nplCards++;
			}
		}

		// common cards
		for (i=0; i<=4; i++)
		{
			if (scraper.card_common[i] != CARD_BACK &&
					scraper.card_common[i] != CARD_NOCARD)
			{
				CardMask_SET(comCards, scraper.card_common[i]);
				iterator_run_with_commoncards[i] = scraper.card_common[i];
				ncomCards++;
			}
		}

		// save settings of this run
		iterator_run_with_f$p = sym_nopp;
		iterator_run_with_nit = sym_nit;
		iterator_run_with_br = symbols.sym.br;

		//Weighted prwin only for nopponents <=13
		error = SUCCESS;
		willplay = (int) calc_f$symbol(&global.formula, "f$willplay", &error);
		error = SUCCESS;
		wontplay = (int) calc_f$symbol(&global.formula, "f$wontplay", &error);
		error = SUCCESS;
		mustplay = (int) calc_f$symbol(&global.formula, "f$mustplay", &error);
		error = SUCCESS;
		topclip = (int) calc_f$symbol(&global.formula, "f$topclip", &error);

		for (i=0; i<=9; i++)
		{
			prw1326_chair_ignore[i] = symbols.prw1326.chair[i].ignore;
			prw1326_chair_limit[i] = symbols.prw1326.chair[i].limit;
		}

	// Allow other threads to use CScraper and CSymbol variables
	LeaveCriticalSection(&cs_scrape_symbol);

	// Call prw1326 callback if needed
	if (prw1326_useme==1326 && prw1326_usecallback==1326 && (sym_br!=1 || prw1326_preflop==1326))
		(*symbols.prw1326.prw_callback)(); //Matrix 2008-05-09

	//
	// Main iterator loop
	//
    for (nit=0; nit < sym_nit; nit++)
    {
		// Check event for stop thread
		if(::WaitForSingleObject(pParent->m_StopThread, 0) == WAIT_OBJECT_0)
		{
			// Set event
			::SetEvent(pParent->m_WaitThread);
			AfxEndThread(0);
		}

        nopp = sym_nopp < MAX_OPPONENTS ? sym_nopp : MAX_OPPONENTS;

        CardMask_OR(usedCards, plCards, comCards);

        if (willplay && (willplay<nopp+1) )
			willplay=nopp+1; //too low a value can give lockup


		if (prw1326_useme==1326 && (sym_br!=1 || prw1326_preflop==1326))
		{
			//prw1326 active  Matrix 2008-05-08
			k = nopp = 0; //k is used as an index into ocard[] 

			// loop through active opponents
			for(i=0; i<=9 && ::WaitForSingleObject(pParent->m_StopThread, 0)!=WAIT_OBJECT_0; i++) 
			{
				if (i==sym_userchair)
					continue; //skip our own chair!

				if (!(sym_playersplayingbits & (1<<i)))
					continue; //skip inactive chairs 

				nopp++; //we have to use actual opponents for prw1326 calculations

				// first deal with the special non-weighted cases
				// player who is marked 'ignore' or one who is BB and has not VPIP'd
				if ( prw1326_chair_ignore[i] || 
					(prw1326_bblimp && (sym_nbetsround_0<1.1) && (sym_bblindbits&(1<<i))))
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

				randfix=(RAND_MAX/prw1326_chair_limit[i]) * prw1326_chair_limit[i];

				// Get exclusive access to CScraper and CSymbol variables
				EnterCriticalSection(&cs_scrape_symbol);

				while (::WaitForSingleObject(pParent->m_StopThread, 0)!=WAIT_OBJECT_0)
				{ //find a possible hand for this chair NOTE: may want to put in loop limits to prevent hanging
					do 
					{
						j=rand();
					} while (j>=randfix);

					j=j%symbols.prw1326.chair[i].limit; //j is now any one of the allowed hands

					if(CardMask_CARD_IS_SET(usedCards,symbols.prw1326.chair[i].rankhi[j] ))
						continue; //hand contains dead card

					if(CardMask_CARD_IS_SET(usedCards,symbols.prw1326.chair[i].ranklo[j] ))
						continue; //hand contains dead card

//					if(symbols.prw1326.chair[i].ignore)break; //chair marked as not to be weighted

					if(symbols.prw1326.chair[i].level<=symbols.prw1326.chair[i].weight[j])
						break; //hand marked as always uae

					//check if we want a player who is BB and has not VPIP'd to be analysed further
//					if(symbols.prw1326.bblimp)
//					{
//				    if ((symbols.sym.nbetsround[0]<1.1) && ((int)symbols.sym.bblindbits&(1<<i)))break;
//					}

					//we should really do a 'randfix' here for the case where RAND_MAX is not an integral
					//multiple of .level, but the bias introduced is trivial compared to other uncertainties.
					if(rand()%symbols.prw1326.chair[i].level<symbols.prw1326.chair[i].weight[j])
						break; //allowable

					//if we reach here we will loop again to find a suitable hand
				} //end of possible hand find

				ocard[k++]=symbols.prw1326.chair[i].rankhi[j];
				ocard[k++]=symbols.prw1326.chair[i].ranklo[j];

				// Allow other threads to use CScraper and CSymbol variables
				LeaveCriticalSection(&cs_scrape_symbol);

				CardMask_SET(usedCards, ocard[k-2]);
				CardMask_SET(usedCards, ocard[k-1]);

			} //end of active opponent loop

			// additional common cards
            CardMask_RESET(addlcomCards);
	        for (i=0; i<5-ncomCards; i++)
		    {
			    do {
				    card = rand() & 63;
                }
	            while (card>51 ||CardMask_CARD_IS_SET(usedCards, card));
		        CardMask_SET(usedCards, card);
			    CardMask_SET(addlcomCards, card);
            }
		} //end of prw1326 code
		
		else
		{ //normal prwin opponent card selection
	      // if f$P<=13 then deal with random replacement algorithm,
		    // otherwise deal with swap algorithm
			if (nopp <= 13)
			{
	            // random replacement algorithm
	            // opponent cards
	            for (i=0; i<nopp*2 && ::WaitForSingleObject(pParent->m_StopThread, 0)!=WAIT_OBJECT_0; i+=2)
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

		                if (!willplay)
			                break; //0 disables weighting

				        //put break for i=0 and opponent unraised BB case (cannot assume anything about his cards)
					    //In round 1 we should really do an analysis of chairs to find out how many have still to
	                    //place a bet. Not implemented since accuracy of prwin pre-flop is less critical.
		                if (!i)
			            {
				            //if we called then we are not BB, BB limped to flop,
					        //BB still playing, so do not weight his cards
						    if (sym_nbetsround_0<1.1 && sym_didcall_0 && (sym_playersplayingbits & sym_bblindbits))
			                {
				                break;
					        }
						}
	                }
		            while (!inrange(ocard[i],ocard[i+1]));
			    }
				// additional common cards
	            CardMask_RESET(addlcomCards);
		        for (i=0; i<5-ncomCards; i++)
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
		        // swap alogorithm
			    //weighted prwin not implemented for this case
	            numberOfCards=52;
		        for (i=0; i<numberOfCards; i++)
			        deck[i] = i;

	            while (numberOfCards>=1 && ::WaitForSingleObject(pParent->m_StopThread, 0)!=WAIT_OBJECT_0)
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
				for (i=0; i<nopp*2; i++)
	            {
		            while (CardMask_CARD_IS_SET(usedCards, deck[x]) && x<=51) {
			            x++;
				    }
					ocard[i] = deck[x++];
	            }

		        // additional common cards
			    CardMask_RESET(addlcomCards);
				for (i=0; i<5-ncomCards; i++)
				{
					while (CardMask_CARD_IS_SET(usedCards, deck[x]) && x<=51) {
	                    x++;
		            }
			        CardMask_SET(addlcomCards, deck[x++]);
				}
			}
		}

		// Get my handval/pokerval
        CardMask_OR(evalCards, plCards, comCards);
        CardMask_OR(evalCards, evalCards, addlcomCards);
        pl_hv = Hand_EVAL_N(evalCards, 7);
        pl_pokval = symbols.calc_pokerval(pl_hv, 7, &dummy, CARD_NOCARD, CARD_NOCARD);

        // Scan through opponents' handvals/pokervals
        // - if we find one better than ours, then we are done, increment los
        // - for win/tie, we need to wait until we scan them all
        opp_pokvalmax = 0;
        for (i=0; i<nopp; i++)
        {
            CardMask_OR(evalCards, comCards, addlcomCards);
            CardMask_SET(evalCards, ocard[i*2]);
            CardMask_SET(evalCards, ocard[(i*2)+1]);
            opp_hv = Hand_EVAL_N(evalCards, 7);
            opp_pokval = symbols.calc_pokerval(opp_hv, 7, &dummy, CARD_NOCARD, CARD_NOCARD);

            if (opp_pokval>pl_pokval)
            {
                los++;
                i = nopp+999;
            }
            else
            {
                if (opp_pokval > opp_pokvalmax)
                    opp_pokvalmax = opp_pokval;
            }
        }
        if (i!=nopp+1000)
        {
            if (pl_pokval > opp_pokvalmax)
                win++;
            else
                tie++;
        }

        // Matrix says "do we really want to update on each of first 1000 iterations?"
        // OpenHoldem answers: No we don't - good catch Matrix; added "&& nit>=1000" qualifier below
        if ((nit/1000 == (int) nit/1000) && nit>=1000)
        {
            EnterCriticalSection(&cs_iterator);
			iterator_thread_progress = nit;
            LeaveCriticalSection(&cs_iterator);

			EnterCriticalSection(&cs_scrape_symbol);	
            symbols.sym.prwin = win / nit;
            symbols.sym.prtie = tie / nit;
            symbols.sym.prlos = los / nit;
			LeaveCriticalSection(&cs_scrape_symbol);
        }
    }

	if (nit >= sym_nit)
    {
	    EnterCriticalSection(&cs_iterator);
        iterator_thread_complete = true;
        iterator_thread_progress = nit;
        LeaveCriticalSection(&cs_iterator);

		EnterCriticalSection(&cs_scrape_symbol);	
		symbols.sym.prwin = win / sym_nit;
        symbols.sym.prtie = tie / sym_nit;
        symbols.sym.prlos = los / sym_nit;
		LeaveCriticalSection(&cs_scrape_symbol);
    }
    else
    {
	    EnterCriticalSection(&cs_iterator);
        iterator_thread_complete = false;
        iterator_thread_progress = 0;
        iterator_run_with_nit = 0;
        iterator_run_with_f$p = 0;
        iterator_run_with_br = 0;
        for (i=0; i<=1; i++)
            iterator_run_with_playercards[i] = CARD_NOCARD;
        for (i=0; i<=4; i++)
            iterator_run_with_commoncards[i] = CARD_NOCARD;
        LeaveCriticalSection(&cs_iterator);

		EnterCriticalSection(&cs_scrape_symbol);	
        symbols.sym.prwin = 0;
        symbols.sym.prtie = 0;
        symbols.sym.prlos = 0;
		LeaveCriticalSection(&cs_scrape_symbol);
    }

    EnterCriticalSection(&cs_iterator);
	iterator_thread_running = false;
	LeaveCriticalSection(&cs_iterator);

	::SetEvent(pParent->m_NormalExitThread);

	delete p_iterator_thread;
	write_log("Iterator thread complete: %08x\n", p_iterator_thread);
	p_iterator_thread = NULL;
	
	AfxEndThread(0);

	return 0;

    __SEH_LOGFATAL("CIteratorThread::iterator_thread_function :\n");
}

int CIteratorThread::inrange(int card1, int card2)
{
    __SEH_HEADER

	//returns non-zero if card pair acceptable for prwin calculation

    /*
    p |---|    ---------
    r |   |   /         \
    o |   |  /           \
    b |   | /             \
    a |   |/               \
    b |                     \
    l -----------------------------------------------------
    e     1    2       3     4     handrank169 ->
    1=mustplay
    2=topclip
    3=willplay
    4=wontplay
    (with the current statistics available in OH only willplay and wontplay should be used.
     mustplay and topclip are debatable constructs at the best of times 2008-01-29)
    */

    extern int willplay, wontplay, topclip, mustplay;
    extern int pair2ranko[170], pair2ranks[170];
    int i, j, hrank;
    i = card1%13;
    j = card2%13;

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

	__SEH_LOGFATAL("CIteratorThread::inrange :\n");
}
