#include "stdafx.h"

#include "action.h"
#include "debug.h"
#include "symbols.h"
#include "pokertracker.h"
#include "GameState.h"

double Action::process_query(const char * pquery, int *e)
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


    __SEH_LOGFATAL("Action::process_query > %s\n", pquery);

}

int Action::preflop_pos (void)
{
    __SEH_HEADER
    //SB=1 BB=2 Early=3 Middle=4 Late=5 Dealer=6
    return
        symbols.sym.nplayersdealt==10 ? (symbols.sym.dealposition==1 ? 1 :
                                         symbols.sym.dealposition==2 ? 2 :
                                         symbols.sym.dealposition==3 ? 3 :
                                         symbols.sym.dealposition==4 ? 3 :
                                         symbols.sym.dealposition==5 ? 3 :
                                         symbols.sym.dealposition==6 ? 4 :
                                         symbols.sym.dealposition==7 ? 4 :
                                         symbols.sym.dealposition==8 ? 5 :
                                         symbols.sym.dealposition==9 ? 5 :
                                         symbols.sym.dealposition==10 ? 6 : 0):
                symbols.sym.nplayersdealt==9  ? (symbols.sym.dealposition==1 ? 1 :
                                                 symbols.sym.dealposition==2 ? 2 :
                                                 symbols.sym.dealposition==3 ? 3 :
                                                 symbols.sym.dealposition==4 ? 3 :
                                                 symbols.sym.dealposition==5 ? 4 :
                                                 symbols.sym.dealposition==6 ? 4 :
                                                 symbols.sym.dealposition==7 ? 5 :
                                                 symbols.sym.dealposition==8 ? 5 :
                                                 symbols.sym.dealposition==9 ? 6 : 0):
                symbols.sym.nplayersdealt==8  ? (symbols.sym.dealposition==1 ? 1 :
                                                 symbols.sym.dealposition==2 ? 2 :
                                                 symbols.sym.dealposition==3 ? 3 :
                                                 symbols.sym.dealposition==4 ? 4 :
                                                 symbols.sym.dealposition==5 ? 4 :
                                                 symbols.sym.dealposition==6 ? 5 :
                                                 symbols.sym.dealposition==7 ? 5 :
                                                 symbols.sym.dealposition==8 ? 6 : 0):
                symbols.sym.nplayersdealt==7  ? (symbols.sym.dealposition==1 ? 1 :
                                                 symbols.sym.dealposition==2 ? 2 :
                                                 symbols.sym.dealposition==3 ? 4 :
                                                 symbols.sym.dealposition==4 ? 4 :
                                                 symbols.sym.dealposition==5 ? 5 :
                                                 symbols.sym.dealposition==6 ? 5 :
                                                 symbols.sym.dealposition==7 ? 6 : 0):
                symbols.sym.nplayersdealt==6  ? (symbols.sym.dealposition==1 ? 1 :
                                                 symbols.sym.dealposition==2 ? 2 :
                                                 symbols.sym.dealposition==3 ? 4 :
                                                 symbols.sym.dealposition==4 ? 5 :
                                                 symbols.sym.dealposition==5 ? 5 :
                                                 symbols.sym.dealposition==6 ? 6 : 0):
                symbols.sym.nplayersdealt==5  ? (symbols.sym.dealposition==1 ? 1 :
                                                 symbols.sym.dealposition==2 ? 2 :
                                                 symbols.sym.dealposition==3 ? 5 :
                                                 symbols.sym.dealposition==4 ? 5 :
                                                 symbols.sym.dealposition==5 ? 6 : 0):
                symbols.sym.nplayersdealt==4  ? (symbols.sym.dealposition==1 ? 1 :
                                                 symbols.sym.dealposition==2 ? 2 :
                                                 symbols.sym.dealposition==3 ? 5 :
                                                 symbols.sym.dealposition==4 ? 6 : 0):
                symbols.sym.nplayersdealt==3  ? (symbols.sym.dealposition==1 ? 1 :
                                                 symbols.sym.dealposition==2 ? 2 :
                                                 symbols.sym.dealposition==3 ? 6 : 0):
                symbols.sym.nplayersdealt==2  ? (
                    // reversed blinds - dealer is BB
                    (PT.getsiteid()==11 || symbols.sym.isppro) ?
                    (symbols.sym.dealposition==1 ? 1 :
                     symbols.sym.dealposition==2 ? 6 : 0 ) :
                    // Normal blinds - dealposition==1 is BB
                    (symbols.sym.dealposition==1 ? 2 :
                     symbols.sym.dealposition==2 ? 6 : 0)) :0;

    __SEH_LOGFATAL("Action::preflop_pos\n");

}

int Action::prefloprais_pos (void)
{
    //SB=1 BB=2 Early=3 Middle=4 Late=5 Dealer=6
    __SEH_HEADER
    return
        symbols.sym.nplayersdealt==10 ? (symbols.sym.dealpositionrais==1 ? 1 :
                                         symbols.sym.dealpositionrais==2 ? 2 :
                                         symbols.sym.dealpositionrais==3 ? 3 :
                                         symbols.sym.dealpositionrais==4 ? 3 :
                                         symbols.sym.dealpositionrais==5 ? 3 :
                                         symbols.sym.dealpositionrais==6 ? 4 :
                                         symbols.sym.dealpositionrais==7 ? 4 :
                                         symbols.sym.dealpositionrais==8 ? 5 :
                                         symbols.sym.dealpositionrais==9 ? 5 :
                                         symbols.sym.dealpositionrais==10 ? 6 : 0):
                symbols.sym.nplayersdealt==9  ? (symbols.sym.dealpositionrais==1 ? 1 :
                                                 symbols.sym.dealpositionrais==2 ? 2 :
                                                 symbols.sym.dealpositionrais==3 ? 3 :
                                                 symbols.sym.dealpositionrais==4 ? 3 :
                                                 symbols.sym.dealpositionrais==5 ? 4 :
                                                 symbols.sym.dealpositionrais==6 ? 4 :
                                                 symbols.sym.dealpositionrais==7 ? 5 :
                                                 symbols.sym.dealpositionrais==8 ? 5 :
                                                 symbols.sym.dealpositionrais==9 ? 6 : 0):
                symbols.sym.nplayersdealt==8  ? (symbols.sym.dealpositionrais==1 ? 1 :
                                                 symbols.sym.dealpositionrais==2 ? 2 :
                                                 symbols.sym.dealpositionrais==3 ? 3 :
                                                 symbols.sym.dealpositionrais==4 ? 4 :
                                                 symbols.sym.dealpositionrais==5 ? 4 :
                                                 symbols.sym.dealpositionrais==6 ? 5 :
                                                 symbols.sym.dealpositionrais==7 ? 5 :
                                                 symbols.sym.dealpositionrais==8 ? 6 : 0):
                symbols.sym.nplayersdealt==7  ? (symbols.sym.dealpositionrais==1 ? 1 :
                                                 symbols.sym.dealpositionrais==2 ? 2 :
                                                 symbols.sym.dealpositionrais==3 ? 4 :
                                                 symbols.sym.dealpositionrais==4 ? 4 :
                                                 symbols.sym.dealpositionrais==5 ? 5 :
                                                 symbols.sym.dealpositionrais==6 ? 5 :
                                                 symbols.sym.dealpositionrais==7 ? 6 : 0):
                symbols.sym.nplayersdealt==6  ? (symbols.sym.dealpositionrais==1 ? 1 :
                                                 symbols.sym.dealpositionrais==2 ? 2 :
                                                 symbols.sym.dealpositionrais==3 ? 4 :
                                                 symbols.sym.dealpositionrais==4 ? 5 :
                                                 symbols.sym.dealpositionrais==5 ? 5 :
                                                 symbols.sym.dealpositionrais==6 ? 6 : 0):
                symbols.sym.nplayersdealt==5  ? (symbols.sym.dealpositionrais==1 ? 1 :
                                                 symbols.sym.dealpositionrais==2 ? 2 :
                                                 symbols.sym.dealpositionrais==3 ? 5 :
                                                 symbols.sym.dealpositionrais==4 ? 5 :
                                                 symbols.sym.dealpositionrais==5 ? 6 : 0):
                symbols.sym.nplayersdealt==4  ? (symbols.sym.dealpositionrais==1 ? 1 :
                                                 symbols.sym.dealpositionrais==2 ? 2 :
                                                 symbols.sym.dealpositionrais==3 ? 5 :
                                                 symbols.sym.dealpositionrais==4 ? 6 : 0):
                symbols.sym.nplayersdealt==3  ? (symbols.sym.dealpositionrais==1 ? 1 :
                                                 symbols.sym.dealpositionrais==2 ? 2 :
                                                 symbols.sym.dealpositionrais==3 ? 6 : 0):
                symbols.sym.nplayersdealt==2  ? (
                    // reversed blinds - dealer is BB
                    (PT.getsiteid()==11 || symbols.sym.isppro) ?
                    (symbols.sym.dealpositionrais==1 ? 1 :
                     symbols.sym.dealpositionrais==2 ? 6 : 0 ) :
                    // Normal blinds - dealposition==1 is BB
                    (symbols.sym.dealpositionrais==1 ? 2 :
                     symbols.sym.dealpositionrais==2 ? 6 : 0)) :0;

    __SEH_LOGFATAL("Action::prefloprais_pos\n");

}

int Action::postflop_pos (void)
{
    //first=1 early=2 middle=3 late=4 last=5
    __SEH_HEADER
    return
        symbols.sym.nplayersplaying==10 ? (symbols.sym.betposition==1 ? 1 :
                                           symbols.sym.betposition==2 ? 2 :
                                           symbols.sym.betposition==3 ? 2 :
                                           symbols.sym.betposition==4 ? 2 :
                                           symbols.sym.betposition==5 ? 3 :
                                           symbols.sym.betposition==6 ? 3 :
                                           symbols.sym.betposition==7 ? 3 :
                                           symbols.sym.betposition==8 ? 4 :
                                           symbols.sym.betposition==9 ? 4 :
                                           symbols.sym.betposition==10 ? 5 : 0):
                symbols.sym.nplayersplaying==9  ? (symbols.sym.betposition==1 ? 1 :
                                                   symbols.sym.betposition==2 ? 2 :
                                                   symbols.sym.betposition==3 ? 2 :
                                                   symbols.sym.betposition==4 ? 3 :
                                                   symbols.sym.betposition==5 ? 3 :
                                                   symbols.sym.betposition==6 ? 3 :
                                                   symbols.sym.betposition==7 ? 4 :
                                                   symbols.sym.betposition==8 ? 4 :
                                                   symbols.sym.betposition==9 ? 5 : 0):
                symbols.sym.nplayersplaying==8  ? (symbols.sym.betposition==1 ? 1 :
                                                   symbols.sym.betposition==2 ? 2 :
                                                   symbols.sym.betposition==3 ? 2 :
                                                   symbols.sym.betposition==4 ? 3 :
                                                   symbols.sym.betposition==5 ? 3 :
                                                   symbols.sym.betposition==6 ? 4 :
                                                   symbols.sym.betposition==7 ? 4 :
                                                   symbols.sym.betposition==8 ? 5 : 0):
                symbols.sym.nplayersplaying==7  ? (symbols.sym.betposition==1 ? 1 :
                                                   symbols.sym.betposition==2 ? 2 :
                                                   symbols.sym.betposition==3 ? 3 :
                                                   symbols.sym.betposition==4 ? 3 :
                                                   symbols.sym.betposition==5 ? 4 :
                                                   symbols.sym.betposition==6 ? 4 :
                                                   symbols.sym.betposition==7 ? 5 : 0):
                symbols.sym.nplayersplaying==6  ? (symbols.sym.betposition==1 ? 1 :
                                                   symbols.sym.betposition==2 ? 2 :
                                                   symbols.sym.betposition==3 ? 3 :
                                                   symbols.sym.betposition==4 ? 4 :
                                                   symbols.sym.betposition==5 ? 4 :
                                                   symbols.sym.betposition==6 ? 5 : 0):
                symbols.sym.nplayersplaying==5  ? (symbols.sym.betposition==1 ? 1 :
                                                   symbols.sym.betposition==2 ? 2 :
                                                   symbols.sym.betposition==3 ? 3 :
                                                   symbols.sym.betposition==4 ? 4 :
                                                   symbols.sym.betposition==5 ? 5 : 0):
                symbols.sym.nplayersplaying==4  ? (symbols.sym.betposition==1 ? 1 :
                                                   symbols.sym.betposition==2 ? 3 :
                                                   symbols.sym.betposition==3 ? 4 :
                                                   symbols.sym.betposition==4 ? 5 : 0):
                symbols.sym.nplayersplaying==3  ? (symbols.sym.betposition==1 ? 1 :
                                                   symbols.sym.betposition==2 ? 4 :
                                                   symbols.sym.betposition==3 ? 5 : 0):
                symbols.sym.nplayersplaying==2  ? (symbols.sym.betposition==1 ? 1 :
                                                   symbols.sym.betposition==2 ? 5 : 0): 0;

    __SEH_LOGFATAL("Action::postflop_pos\n");

}

int Action::pf_bets (void)
{
    __SEH_HEADER
    if (symbols.sym.br!=1)
        return 0;

    return (first_into_pot())			? 1 :   //No Callers - no callers or blinds only (on preflop).
           (symbols.sym.ncallbets==1)		? 2 :   //Called Pot - 1 bet to call.
           (symbols.sym.nbetstocall==1 && (symbols.sym.didcall||symbols.sym.didrais))
           ? 3 :   //Raised Back - 1 more bet to call because someone behind you raised after you've already bet/called/raised.
           (symbols.sym.ncallbets==2)		? 4 :   //Raised Pot - 2 bets to call.
           (symbols.sym.ncallbets>=3)		? 5 :0; //Reraised Pot - 3+ bets to call.

    __SEH_LOGFATAL("Action::pf_bets\n");

}

bool Action::first_into_pot (void)
{
    __SEH_HEADER
    return symbols.sym.br==1 ? (symbols.sym.potplayer<=symbols.sym.sblind+symbols.sym.bblind) : symbols.sym.potplayer<=0.1 ;

    __SEH_LOGFATAL("Action::first_into_pot\n");

}

int Action::m_betposition (int chairnum)
{
    __SEH_HEADER
    int i;
    int betpos = 0 ;

    if (chairnum<0 || chairnum>9)
        return betpos;

    if (!((((int)symbols.sym.playersplayingbits)>>chairnum)&1))
        return betpos;

    for (i=symbols.sym.dealerchair+1; i<=symbols.sym.dealerchair+10; i++)
    {
        if ((((int)symbols.sym.playersplayingbits)>>(i%10))&1)
            betpos++;

        if (i%10==chairnum)
            i=99;
    }

    return betpos;

    __SEH_LOGFATAL("Action::m_betposition\n");

}

int Action::m_dealposition (int chairnum)
{
    __SEH_HEADER
    int i;
    int dealposchair = 0 ;

    if (chairnum<0 || chairnum>9)
        return dealposchair;

    for (i=symbols.sym.dealerchair+1; i<=symbols.sym.dealerchair+10; i++)
    {
        if ((((int) symbols.sym.playersdealtbits)>>(i%10))&1)
            dealposchair++;

        if (i%10==chairnum)
            i=99;
    }
    return ((((int) symbols.sym.playersdealtbits)>>chairnum)&1) ? dealposchair : 0 ;

    __SEH_LOGFATAL("Action::m_dealposition\n");

}

int Action::aggressor_chair (void)
{
    __SEH_HEADER
    // br1, no raises
    if (symbols.sym.br==1 && symbols.sym.nbetsround[0]==1)
        return symbols.sym.raischair;

    // br1, someone raised
    if (symbols.sym.br==1 && symbols.sym.nbetsround[0]>1)
        return game_state.lastraised(1)!=-1 ? game_state.lastraised(1) : symbols.sym.raischair;

    // br2, no raises
    if (symbols.sym.br==2 && symbols.sym.nbetsround[1]==0)
        return game_state.lastraised(1)!=-1 ? game_state.lastraised(1) : symbols.sym.raischair;

    // br2, someone raised
    if (symbols.sym.br==2 && symbols.sym.nbetsround[1]>0)
        return game_state.lastraised(2)!=-1 ? game_state.lastraised(2) : symbols.sym.raischair;

    // br3, no raises
    if (symbols.sym.br==3 && symbols.sym.nbetsround[2]==0)
        return game_state.lastraised(2)!=-1 ? game_state.lastraised(2) : symbols.sym.raischair;

    // br3, someone raised
    if (symbols.sym.br==3 && symbols.sym.nbetsround[2]>0)
        return game_state.lastraised(3)!=-1 ? game_state.lastraised(3) : symbols.sym.raischair;

    // br4, no raises
    if (symbols.sym.br==4 && symbols.sym.nbetsround[3]==0)
        return game_state.lastraised(3)!=-1 ? game_state.lastraised(3) : symbols.sym.raischair;

    // br4, someone raised
    if (symbols.sym.br==4 && symbols.sym.nbetsround[3]>0)
        return game_state.lastraised(4)!=-1 ? game_state.lastraised(4) : symbols.sym.raischair;

    return symbols.sym.raischair;

    __SEH_LOGFATAL("Action::aggressor_chair\n");

}

bool Action::agchair_after (void)
{
    __SEH_HEADER
    if (!symbols.user_chair_confirmed)
        return false;

    if (aggressor_chair()>=0)
        return m_betposition(aggressor_chair()) > symbols.sym.betposition ;

    else
        return false ;


    __SEH_LOGFATAL("Action::agchair_after\n");

}
