#include "stdafx.h"

#include "action.h"

#include "CSymbols.h"
#include "CPokerTrackerThread.h"

#include "GameState.h"

PokerAction::PokerAction()
{
    __SEH_SET_EXCEPTION_HANDLER
}

double PokerAction::process_query(const char * pquery, int *e)
{
    __SEH_HEADER

    if (memcmp(pquery,"ac_aggressor",12)==0)				return aggressor_chair();
    if (memcmp(pquery,"ac_agchair_after", 16) == 0)			return agchair_after();
    if (memcmp(pquery,"ac_preflop_pos", 14) == 0)			return preflop_pos();
    if (memcmp(pquery,"ac_prefloprais_pos", 18) == 0)		return prefloprais_pos();
    if (memcmp(pquery,"ac_postflop_pos", 15) == 0)			return postflop_pos();
    if (memcmp(pquery,"ac_pf_bets", 10) == 0)				return pf_bets();
    if (memcmp(pquery,"ac_first_into_pot", 17) == 0)		return first_into_pot();
    if (memcmp(pquery,"ac_betpos", 9) == 0)					return m_betposition(pquery[9]-'0');
    if (memcmp(pquery,"ac_dealpos", 10) == 0)				return m_dealposition(pquery[10]-'0');

    *e = ERR_INVALID_SYM;
    return 0.0;


    __SEH_LOGFATAL("PokerAction::process_query > %s\n", pquery);
}

int PokerAction::preflop_pos (void)
{
    __SEH_HEADER

	int		sym_nplayersdealt = (int) p_symbols->sym()->nplayersdealt;
	int		sym_dealposition = (int) p_symbols->sym()->dealposition;
	bool	sym_isppro = (bool) p_symbols->sym()->isppro;
	

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
								(p_symbols->GetSiteId()==11 || sym_isppro) ?
								(sym_dealposition==1 ? 1 :
								sym_dealposition==2 ? 6 : 0 ) :
								// Normal blinds - dealposition==1 is BB
								(sym_dealposition==1 ? 2 :
								 sym_dealposition==2 ? 6 : 0)) :0;

	__SEH_LOGFATAL("PokerAction::preflop_pos\n");
}

int PokerAction::prefloprais_pos (void)
{
    __SEH_HEADER

	int		e = SUCCESS;
	int		sym_nplayersdealt = (int) p_symbols->sym()->nplayersdealt;
	int		sym_dealpositionrais = (int) p_symbols->sym()->dealpositionrais;
	bool	sym_isppro = (bool) p_symbols->sym()->isppro;
	
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
								(p_symbols->GetSiteId()==11 || sym_isppro) ?
								(sym_dealpositionrais==1 ? 1 :
								sym_dealpositionrais==2 ? 6 : 0 ) :
								// Normal blinds - dealposition==1 is BB
								(sym_dealpositionrais==1 ? 2 :
								sym_dealpositionrais==2 ? 6 : 0)) :0;

	__SEH_LOGFATAL("PokerAction::prefloprais_pos\n");
}

int PokerAction::postflop_pos (void)
{
    __SEH_HEADER

	int		e = SUCCESS;
	int		sym_nplayersplaying = (int) p_symbols->sym()->nplayersplaying;
	int		sym_betposition = (int) p_symbols->sym()->betposition;

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

	__SEH_LOGFATAL("PokerAction::postflop_pos\n");
}

int PokerAction::pf_bets (void)
{
    __SEH_HEADER

	int result;

	int		e = SUCCESS;

	if (p_symbols->sym()->br!=1)
        result = 0;

	else result = 
			(first_into_pot())				? 1 :   //No Callers - no callers or blinds only (on preflop).
			(p_symbols->sym()->ncallbets==1)		? 2 :   //Called Pot - 1 bet to call.
			(p_symbols->sym()->nbetstocall==1 && 
				(p_symbols->sym()->didcall[4] || p_symbols->sym()->didrais[4] ))
			? 3 :   //Raised Back - 1 more bet to call because someone behind you raised after you've already bet/called/raised.
			(p_symbols->sym()->ncallbets==2)		? 4 :   //Raised Pot - 2 bets to call.
			(p_symbols->sym()->ncallbets>=3)		? 5 :0; //Reraised Pot - 3+ bets to call.

	return result;

	__SEH_LOGFATAL("PokerAction::pf_bets\n");
}

bool PokerAction::first_into_pot (void)
{
    __SEH_HEADER

	bool result;

	int		e = SUCCESS;
	result = p_symbols->sym()->br==1 ? 
		p_symbols->sym()->potplayer <= p_symbols->sym()->sblind + p_symbols->sym()->bblind : 
		p_symbols->sym()->potplayer <= 0.1 ;

	return result;

    __SEH_LOGFATAL("PokerAction::first_into_pot\n");
}

int PokerAction::m_betposition (int chairnum)
{

    __SEH_HEADER
    int		i;
    int		betpos = 0 ;

	int		e = SUCCESS;
	int		sym_dealerchair = (int) p_symbols->sym()->dealerchair;
	int		sym_playersplayingbits = (int) p_symbols->sym()->playersplayingbits;

	if (chairnum<0 || chairnum>9)
        return betpos;

    if (!((sym_playersplayingbits>>chairnum)&1))
        return betpos;

    for (i=sym_dealerchair+1; i<=sym_dealerchair+10; i++)
    {
        if ((sym_playersplayingbits>>(i%10))&1)
            betpos++;

        if (i%10==chairnum)
            i=99;
    }

    return betpos;

    __SEH_LOGFATAL("PokerAction::m_betposition\n");
}

int PokerAction::m_dealposition (int chairnum)
{
    __SEH_HEADER

	int		i;
    int		dealposchair = 0 ;

	int		e = SUCCESS;
	int		sym_dealerchair = (int) p_symbols->sym()->dealerchair;
	int		sym_playersdealtbits = (int) p_symbols->sym()->playersdealtbits;

	if (chairnum<0 || chairnum>9)
        return dealposchair;

    for (i=sym_dealerchair+1; i<=sym_dealerchair+10; i++)
    {
        if ((sym_playersdealtbits>>(i%10))&1)
            dealposchair++;

        if (i%10==chairnum)
            i=99;
    }
    return ((sym_playersdealtbits>>chairnum)&1) ? dealposchair : 0 ;

    __SEH_LOGFATAL("PokerAction::m_dealposition\n");
}

int PokerAction::aggressor_chair (void)
{
    __SEH_HEADER

	int		sym_br = (int) p_symbols->sym()->br;
	int		sym_raischair = (int) p_symbols->sym()->raischair;

	// br1, no raises
    if (sym_br==1 && p_symbols->sym()->nbetsround[0]==1)
        return sym_raischair;

    // br1, someone raised
    if (sym_br==1 && p_symbols->sym()->nbetsround[0]>1)
        return game_state.lastraised(1)!=-1 ? game_state.lastraised(1) : sym_raischair;

    // br2, no raises
    if (sym_br==2 && p_symbols->sym()->nbetsround[1]==0)
        return game_state.lastraised(1)!=-1 ? game_state.lastraised(1) : sym_raischair;

    // br2, someone raised
    if (sym_br==2 && p_symbols->sym()->nbetsround[1]>0)
        return game_state.lastraised(2)!=-1 ? game_state.lastraised(2) : sym_raischair;

    // br3, no raises
    if (sym_br==3 && p_symbols->sym()->nbetsround[2]==0)
        return game_state.lastraised(2)!=-1 ? game_state.lastraised(2) : sym_raischair;

    // br3, someone raised
    if (sym_br==3 && p_symbols->sym()->nbetsround[2]>0)
        return game_state.lastraised(3)!=-1 ? game_state.lastraised(3) : sym_raischair;

    // br4, no raises
    if (sym_br==4 && p_symbols->sym()->nbetsround[3]==0)
        return game_state.lastraised(3)!=-1 ? game_state.lastraised(3) : sym_raischair;

    // br4, someone raised
    if (sym_br==4 && p_symbols->sym()->nbetsround[3]>0)
        return game_state.lastraised(4)!=-1 ? game_state.lastraised(4) : sym_raischair;

    return sym_raischair;

    __SEH_LOGFATAL("PokerAction::aggressor_chair\n");
}

bool PokerAction::agchair_after (void)
{
    __SEH_HEADER

	bool	result;
	int		e = SUCCESS;

	if (!p_symbols->user_chair_confirmed())
        result = false;

    if (aggressor_chair()>=0)
		result = m_betposition(aggressor_chair()) > p_symbols->sym()->betposition;

    else
        result = false ;

	return result;

    __SEH_LOGFATAL("PokerAction::agchair_after\n");
}
