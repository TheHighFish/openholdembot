#include "stdafx.h"
#include <io.h>
#include <fcntl.h>

#include "versus.h"
#include "debug.h"
#include "global.h"
#include "scraper.h"
#include "symbols.h"
#include "inlines/eval.h"

class CVersus	versus;

CVersus::CVersus() {
#ifdef SEH_ENABLE
    // Set exception handler
    __SEH_SET_EXCEPTION_HANDLER(MyUnHandledExceptionFilter);
#endif

    __SEH_HEADER

    __SEH_LOGFATAL("CVersus::Constructor :\n");
}

double CVersus::get_symbol(const char *a, int *e) {
    __SEH_HEADER
    int			n;
	char		*b;		//Matrix 2008-05-21

    if (!global.versus_enabled)
        return 0.0;

    if (memcmp(a, "vs$nhands", 9)==0 && strlen(a)==9)			return nhands;
    else if (memcmp(a, "vs$nhandshi", 11)==0 && strlen(a)==11)	return nhandshi;
    else if (memcmp(a, "vs$nhandsti", 11)==0 && strlen(a)==11)	return nhandsti;
    else if (memcmp(a, "vs$nhandslo", 11)==0 && strlen(a)==11)	return nhandslo;
    else if (memcmp(a, "vs$prwin", 8)==0 && strlen(a)==8)		return vsprwin;
    else if (memcmp(a, "vs$prtie", 8)==0 && strlen(a)==8)		return vsprtie;
    else if (memcmp(a, "vs$prlos", 8)==0 && strlen(a)==8)		return vsprlos;
    else if (memcmp(a, "vs$prwinhi", 10)==0 && strlen(a)==10)	return vsprwinhi;
    else if (memcmp(a, "vs$prtiehi", 10)==0 && strlen(a)==10)	return vsprtiehi;
    else if (memcmp(a, "vs$prloshi", 10)==0 && strlen(a)==10)	return vsprloshi;
    else if (memcmp(a, "vs$prwinti", 10)==0 && strlen(a)==10)	return vsprwinti;
    else if (memcmp(a, "vs$prtieti", 10)==0 && strlen(a)==10)	return vsprtieti;
    else if (memcmp(a, "vs$prlosti", 10)==0 && strlen(a)==10)	return vsprlosti;
    else if (memcmp(a, "vs$prwinlo", 10)==0 && strlen(a)==10)	return vsprwinlo;
    else if (memcmp(a, "vs$prtielo", 10)==0 && strlen(a)==10)	return vsprtielo;
    else if (memcmp(a, "vs$prloslo", 10)==0 && strlen(a)==10)	return vsprloslo;

    else if (memcmp(a, "vs$", 3)==0 &&
             ((memcmp(&a[4], "$pr", 3)==0 && strlen(a)==10) ||
			 (memcmp(&a[5], "$pr", 3)==0 && strlen(a)==11) ||		// Matrix 2008-05-21
              (memcmp(&a[6], "$pr", 3)==0 && strlen(a)==12)))
    {

        // Get the list num we need
		n = atoi(a+3);		// Matrix 2008-05-21

        // win, los or tie?
		b = (char *)strstr(a+3,"$pr")+3;	// Matrix 2008-05-21

		if (!memcmp(b,"win",3))
            return (double) nlistwin[n] / ((double) nlistwin[n] + (double) nlisttie[n] + (double) nlistlos[n]);

        else if (!memcmp(b,"tie",3))
            return (double) nlisttie[n] / ((double) nlistwin[n] + (double) nlisttie[n] + (double) nlistlos[n]);

        else if (!memcmp(b,"los",3))
            return (double) nlistlos[n] / ((double) nlistwin[n] + (double) nlisttie[n] + (double) nlistlos[n]);
    }

    *e = ERR_INVALID_SYM;
    return 0.0;


    __SEH_LOGFATAL("CVersus::get_symbol :\n");
}

bool CVersus::get_counts(void) {
    __SEH_HEADER
    int				i, j, fh;
    unsigned int	pcard[2];
    CardMask		plCards, oppCards, deadCards, comCardsScrape, comCardsEnum, comCardsAll, usedCards;
    unsigned int	wintemp, tietemp, lostemp, offset;
    unsigned int	nhiwin, nhitie, nhilos, ntiwin, ntitie, ntilos, nlowin, nlotie, nlolos;
    unsigned int	c0rank, c1rank, temprank;
    BYTE			byte[8];
    long			pos;
    int				listnum;
    int				card0_offset[52] = { 0, 62475, 123725, 183750, 242550, 300125, 356475, 411600, 465500, 518175, 569625,
                                619850, 668850, 716625, 763175, 808500, 852600, 895475, 937125, 977550, 1016750,
                                1054725, 1091475, 1127000, 1161300, 1194375, 1226225, 1256850, 1286250, 1314425,
                                1341375, 1367100, 1391600, 1414875, 1436925, 1457750, 1477350, 1495725, 1512875,
                                1528800, 1543500, 1556975, 1569225, 1580250, 1590050, 1598625, 1605975, 1612100,
                                1617000, 1620675, 1623125, 1624350
                              };

    if (!symbols.user_chair_confirmed)
        return false;

    if (!global.versus_enabled)
        return false;

    if (scraper.card_player[(int) symbols.sym.userchair][0] == CARD_NOCARD ||
            scraper.card_player[(int) symbols.sym.userchair][0] == CARD_BACK ||
            scraper.card_player[(int) symbols.sym.userchair][1] == CARD_NOCARD ||
            scraper.card_player[(int) symbols.sym.userchair][1] == CARD_BACK)
    {
        return false;
    }

    nwin = ntie = nlos = nhands = 0;
    nhandshi = nhandsti = nhandslo = 0;
    nhiwin = nhitie = nhilos = ntiwin = ntitie = ntilos = nlowin = nlotie = nlolos = 0;

    // Clear counters
    for (listnum=0; listnum<MAX_HAND_LISTS; listnum++)
        nlistwin[listnum] = nlisttie[listnum] = nlistlos[listnum] = 0;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // PREFLOP
    if (symbols.sym.br == 1)
    {
        if ((fh = _sopen(global.versus_path, _O_RDONLY | _O_BINARY, _SH_DENYWR, NULL))==-1)
        {
            return false;
        }

        // order cards properly
        if (scraper.card_player[(int) symbols.sym.userchair][0] < scraper.card_player[(int) symbols.sym.userchair][1])
        {
            pcard[0] = scraper.card_player[(int) symbols.sym.userchair][0];
            pcard[1] = scraper.card_player[(int) symbols.sym.userchair][1];
        }

        else
        {
            pcard[0] = scraper.card_player[(int) symbols.sym.userchair][1];
            pcard[1] = scraper.card_player[(int) symbols.sym.userchair][0];
        }

        // figure out offset into file
        offset = 0;
        //for (i=1; i<pcard[0]; i++)  offset += (52-i)*1225;
        offset += card0_offset[pcard[0]];
        offset += (pcard[1]-pcard[0]-1)*1225;
        offset *= sizeof(byte);

        // seek to right position in file
        if ((pos = _lseek(fh, offset, SEEK_SET)) == -1L)
        {
            _close(fh);
            return false;
        }

        wintemp = lostemp = 0;

        for (i=0; i<=50; i++)
        {
            for (j=i+1; j<=51; j++)
            {

                if (i!=pcard[0] && i!=pcard[1] && j!=pcard[0] && j!=pcard[1])
                {
                    _read(fh, &byte, sizeof(byte));
                    memcpy(&wintemp, &byte[0], sizeof(unsigned int));
                    memcpy(&lostemp, &byte[4], sizeof(unsigned int));

                    nwin += wintemp;
                    ntie += 1712304 - wintemp - lostemp;
                    nlos += lostemp;
                    nhands = nhands + 1;

                    if (wintemp<lostemp)
                    {
                        nhandshi = nhandshi + 1;
                        nhiwin += wintemp;
                        nhitie += 1712304 - wintemp - lostemp;
                        nhilos += lostemp;
                    }
                    else if (wintemp>lostemp)
                    {
                        nhandslo = nhandslo + 1;
                        nlowin += wintemp;
                        nlotie += 1712304 - wintemp - lostemp;
                        nlolos += lostemp;
                    }
                    else
                    {
                        nhandsti = nhandsti + 1;
                        ntiwin += wintemp;
                        ntitie += 1712304 - wintemp - lostemp;
                        ntilos += lostemp;
                    }

                    // Calculations for vs$xx$prwin, vs$xx$prtie, vs$xx$prlos
                    c0rank = StdDeck_RANK(i);
                    c1rank = StdDeck_RANK(j);
                    if (c0rank < c1rank)
                    {
                        temprank = c0rank;
                        c0rank = c1rank;
                        c1rank = temprank;
                    }

                    for (listnum=0; listnum<MAX_HAND_LISTS; listnum++)
                    {
                        if ((StdDeck_SUIT(i)==StdDeck_SUIT(j) && global.formula.inlist[listnum][c0rank][c1rank]) ||
                                (StdDeck_SUIT(i)!=StdDeck_SUIT(j) && global.formula.inlist[listnum][c1rank][c0rank]))
                        {
                            nlistwin[listnum] += wintemp;
                            nlisttie[listnum] += 1712304 - wintemp - lostemp;
                            nlistlos[listnum] += lostemp;
                        }
                    }
                }
            }
        }

        _close(fh);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // FLOP, TURN, RIVER
    else if (symbols.sym.br >= 2) {

        // Common cards
        CardMask_RESET(comCardsScrape);
        if (symbols.sym.br >= 2) CardMask_SET(comCardsScrape, scraper.card_common[0]);
        if (symbols.sym.br >= 2) CardMask_SET(comCardsScrape, scraper.card_common[1]);
        if (symbols.sym.br >= 2) CardMask_SET(comCardsScrape, scraper.card_common[2]);
        if (symbols.sym.br >= 3) CardMask_SET(comCardsScrape, scraper.card_common[3]);
        if (symbols.sym.br >= 4) CardMask_SET(comCardsScrape, scraper.card_common[4]);

        // player cards
        CardMask_RESET(plCards);
        CardMask_SET(plCards, scraper.card_player[(int) symbols.sym.userchair][0]);
        CardMask_SET(plCards, scraper.card_player[(int) symbols.sym.userchair][1]);

        // all used cards
        CardMask_OR(usedCards, comCardsScrape, plCards);

        // Enumerate through all possible opponent hands (excludes already used cards)
        for (i=0; i<=50; i++)
        {
            for (j=i+1; j<=51; j++)
            {
                if (!CardMask_CARD_IS_SET(usedCards, i) && !CardMask_CARD_IS_SET(usedCards, j))
                {

                    CardMask_RESET(oppCards);
                    CardMask_SET(oppCards, i);
                    CardMask_SET(oppCards, j);

                    // Enumerate through all possible river situations (exclude player cards and opponent cards)
                    CardMask_OR(deadCards, usedCards, oppCards);
                    wintemp = tietemp = lostemp = 0;

                    if (symbols.sym.br==2 || symbols.sym.br==3)
                    {
                        ENUMERATE_N_CARDS_D(comCardsEnum, symbols.sym.br==2 ? 2 : symbols.sym.br==3 ? 1 : 0, deadCards,
                        {
                            CardMask_OR(comCardsAll, comCardsScrape, comCardsEnum);
                            do_calc(plCards, oppCards, comCardsAll, &wintemp, &tietemp, &lostemp);
                        });
                    }
                    else
                    {
                        do_calc(plCards, oppCards, comCardsScrape, &wintemp, &tietemp, &lostemp);
                    }

                    nwin += wintemp;
                    ntie += tietemp;
                    nlos += lostemp;
                    nhands = nhands + 1;

                    if (wintemp<lostemp)
                    {
                        nhandshi = nhandshi + 1;
                        nhiwin += wintemp;
                        nhitie += tietemp;
                        nhilos += lostemp;
                    }
                    else if (wintemp>lostemp)
                    {
                        nhandslo = nhandslo + 1;
                        nlowin += wintemp;
                        nlotie += tietemp;
                        nlolos += lostemp;
                    }
                    else
                    {
                        nhandsti = nhandsti + 1;
                        ntiwin += wintemp;
                        ntitie += tietemp;
                        ntilos += lostemp;
                    }

                    // Calculations for vs$xx$prwin, vs$xx$prtie, vs$xx$prlos
                    c0rank = StdDeck_RANK(i);
                    c1rank = StdDeck_RANK(j);
                    if (c0rank < c1rank)
                    {
                        temprank = c0rank;
                        c0rank = c1rank;
                        c1rank = temprank;
                    }

                    for (listnum=0; listnum<MAX_HAND_LISTS; listnum++)
                    {
                        if ((StdDeck_SUIT(i)==StdDeck_SUIT(j) && global.formula.inlist[listnum][c0rank][c1rank]) ||
                                (StdDeck_SUIT(i)!=StdDeck_SUIT(j) && global.formula.inlist[listnum][c1rank][c0rank]))
                        {
                            nlistwin[listnum] += wintemp;
                            nlisttie[listnum] += tietemp;
                            nlistlos[listnum] += lostemp;
                        }
                    }
                }
            }
        }
    }

    // calc the stats
    vsprwin = (double) nwin / ((double)  nwin + (double) ntie + (double) nlos);
    vsprtie = (double) ntie / ((double)  nwin + (double) ntie + (double) nlos);
    vsprlos = (double) nlos / ((double)  nwin + (double) ntie + (double) nlos);

    vsprwinhi = (double) nhiwin / ((double)  nhiwin + (double) nhitie + (double) nhilos);
    vsprtiehi = (double) nhitie / ((double)  nhiwin + (double) nhitie + (double) nhilos);
    vsprloshi = (double) nhilos / ((double)  nhiwin + (double) nhitie + (double) nhilos);

    vsprwinti = (double) ntiwin / ((double)  ntiwin + (double) ntitie + (double) ntilos);
    vsprtieti = (double) ntitie / ((double)  ntiwin + (double) ntitie + (double) ntilos);
    vsprlosti = (double) ntilos / ((double)  ntiwin + (double) ntitie + (double) ntilos);

    vsprwinlo = (double) nlowin / ((double)  nlowin + (double) nlotie + (double) nlolos);
    vsprtielo = (double) nlotie / ((double)  nlowin + (double) nlotie + (double) nlolos);
    vsprloslo = (double) nlolos / ((double)  nlowin + (double) nlotie + (double) nlolos);

    return true;

    __SEH_LOGFATAL("CVersus::get_counts :\n");
}


void CVersus::do_calc(CardMask plCards, CardMask oppCards, CardMask comCards,
                      unsigned int *wintemp, unsigned int *tietemp, unsigned int *lostemp) {
    __SEH_HEADER
    CardMask		playerEvalCards, oppEvalCards;
    HandVal			player_hv, opp_hv;
    unsigned int	player_pokval, opp_pokval;
    double			dummy;

    CardMask_OR(playerEvalCards, plCards, comCards);
    CardMask_OR(oppEvalCards, oppCards, comCards);

    player_hv = Hand_EVAL_N(playerEvalCards, 7);
    opp_hv = Hand_EVAL_N(oppEvalCards, 7);

    player_pokval = symbols.calc_pokerval(player_hv, 7, &dummy, 0, 1);
    opp_pokval = symbols.calc_pokerval(opp_hv, 7, &dummy, 0, 1);

    if (player_pokval > opp_pokval)
        *wintemp = *wintemp + 1;

    else if (player_pokval < opp_pokval)
        *lostemp = *lostemp + 1;

    else
        *tietemp = *tietemp + 1;

    __SEH_LOGFATAL("CVersus::do_calc :\n");
}