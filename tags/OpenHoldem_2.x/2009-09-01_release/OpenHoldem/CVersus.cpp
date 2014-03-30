#include "stdafx.h"
#include <io.h>
#include <fcntl.h>

#include "CVersus.h"

#include "CScraper.h"
#include "CSymbols.h"
#include "CPreferences.h"
#include "CFormula.h"
#include "CDllExtension.h"

#include "inlines/eval.h"

CVersus		*p_versus = NULL;

CVersus::CVersus() 
{
	// Find the versus data.  First check in the current directory
	// then in the path provided by the registry.  If both fail,
	// disable versus.
	_sopen_s(&_versus_fh, "versus.bin", _O_RDONLY | _O_BINARY, _SH_DENYWR, NULL);
	if (_versus_fh == -1)
	{
		_sopen_s(&_versus_fh, prefs.versus_path(), _O_RDONLY | _O_BINARY, _SH_DENYWR, NULL);
	}

	if (_versus_fh == -1 && !prefs.disable_msgbox())
	{
		MessageBox(NULL, "Could not open versus.bin.\nVersus functions will be disabled.\n", "Versus Error", MB_OK | MB_TOPMOST);
	}
}

CVersus::~CVersus() 
{
}

double CVersus::GetSymbol(const char *a, int *e) 
{
	int			n = 0;
	char		*b = 0;

	if (_versus_fh == -1)
		return 0.0;

	if (memcmp(a, "vs$nhands", 9)==0 && strlen(a)==9)			return _nhands;
	else if (memcmp(a, "vs$nhandshi", 11)==0 && strlen(a)==11)	return _nhandshi;
	else if (memcmp(a, "vs$nhandsti", 11)==0 && strlen(a)==11)	return _nhandsti;
	else if (memcmp(a, "vs$nhandslo", 11)==0 && strlen(a)==11)	return _nhandslo;
	else if (memcmp(a, "vs$prwin", 8)==0 && strlen(a)==8)		return _vsprwin;
	else if (memcmp(a, "vs$prtie", 8)==0 && strlen(a)==8)		return _vsprtie;
	else if (memcmp(a, "vs$prlos", 8)==0 && strlen(a)==8)		return _vsprlos;
	else if (memcmp(a, "vs$prwinhi", 10)==0 && strlen(a)==10)	return _vsprwinhi;
	else if (memcmp(a, "vs$prtiehi", 10)==0 && strlen(a)==10)	return _vsprtiehi;
	else if (memcmp(a, "vs$prloshi", 10)==0 && strlen(a)==10)	return _vsprloshi;
	else if (memcmp(a, "vs$prwinti", 10)==0 && strlen(a)==10)	return _vsprwinti;
	else if (memcmp(a, "vs$prtieti", 10)==0 && strlen(a)==10)	return _vsprtieti;
	else if (memcmp(a, "vs$prlosti", 10)==0 && strlen(a)==10)	return _vsprlosti;
	else if (memcmp(a, "vs$prwinlo", 10)==0 && strlen(a)==10)	return _vsprwinlo;
	else if (memcmp(a, "vs$prtielo", 10)==0 && strlen(a)==10)	return _vsprtielo;
	else if (memcmp(a, "vs$prloslo", 10)==0 && strlen(a)==10)	return _vsprloslo;

	else if (memcmp(a, "vs$", 3)==0 &&
			 ((memcmp(&a[4], "$pr", 3)==0 && strlen(a)==10) ||
			  (memcmp(&a[5], "$pr", 3)==0 && strlen(a)==11) ||
			  (memcmp(&a[6], "$pr", 3)==0 && strlen(a)==12)))
	{

		// Get the list num we need
		n = atoi(a+3);

		// win, los or tie?
		b = (char *)strstr(a+3,"$pr")+3;

		if (!memcmp(b,"win",3))
			return (double) _nlistwin[n] / ((double) _nlistwin[n] + (double) _nlisttie[n] + (double) _nlistlos[n]);

		else if (!memcmp(b,"tie",3))
			return (double) _nlisttie[n] / ((double) _nlistwin[n] + (double) _nlisttie[n] + (double) _nlistlos[n]);

		else if (!memcmp(b,"los",3))
			return (double) _nlistlos[n] / ((double) _nlistwin[n] + (double) _nlisttie[n] + (double) _nlistlos[n]);
	}

	*e = ERR_INVALID_SYM;
	return 0.0;
}

bool CVersus::GetCounts(void) 
{
	if (_versus_fh == -1)
		return false;

	int				i = 0, j = 0;
	CardMask		plCards, oppCards, deadCards, comCardsScrape, comCardsEnum, comCardsAll, usedCards;
	unsigned int	wintemp = 0, tietemp = 0, lostemp = 0, offset = 0;
	unsigned int	nhiwin = 0, nhitie = 0, nhilos = 0, ntiwin = 0, ntitie = 0, ntilos = 0, nlowin = 0, nlotie = 0, nlolos = 0;
	unsigned int	c0rank = 0, c1rank = 0, temprank = 0;
	BYTE			byte[8] = {0};
	long			pos = 0;
	int				listnum = 0;
	int				card0_offset[52] = { 0, 62475, 123725, 183750, 242550, 300125, 356475, 411600, 465500, 518175, 569625,
								619850, 668850, 716625, 763175, 808500, 852600, 895475, 937125, 977550, 1016750,
								1054725, 1091475, 1127000, 1161300, 1194375, 1226225, 1256850, 1286250, 1314425,
								1341375, 1367100, 1391600, 1414875, 1436925, 1457750, 1477350, 1495725, 1512875,
								1528800, 1543500, 1556975, 1569225, 1580250, 1590050, 1598625, 1605975, 1612100,
								1617000, 1620675, 1623125, 1624350
							  };
	int sym_br = (int) p_symbols->sym()->br;
	int sym_userchair = (int) p_symbols->sym()->userchair;

	unsigned int	pcard[2] = {0};
	for (i=0; i<=1; i++)
		pcard[i] = CARD_NOCARD;

	unsigned int	card_player[2] = {0}, card_common[5] = {0};
	for (i=0; i<=1; i++)
		card_player[i] = p_scraper->card_player(sym_userchair, i);
	for (i=0; i<=4; i++)
		card_common[i] = p_scraper->card_common(i);

	// Get the lock
	CSLock lock(m_critsec);

	if (!p_symbols->user_chair_confirmed())
		return false;


	if (card_player[0] == CARD_NOCARD || card_player[0] == CARD_BACK ||
		card_player[1] == CARD_NOCARD || card_player[1] == CARD_BACK)
	{
		return false;
	}

	_nwin = _ntie = _nlos = _nhands = 0;
	_nhandshi = _nhandsti = _nhandslo = 0;
	nhiwin = nhitie = nhilos = ntiwin = ntitie = ntilos = nlowin = nlotie = nlolos = 0;

	// Clear counters
	for (listnum=0; listnum<MAX_HAND_LISTS; listnum++)
		_nlistwin[listnum] = _nlisttie[listnum] = _nlistlos[listnum] = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PREFLOP
	if (sym_br == 1)
	{
		// order cards properly
		if (card_player[0] < card_player[1])
		{
			pcard[0] = card_player[0];
			pcard[1] = card_player[1];
		}

		else
		{
			pcard[0] = card_player[1];
			pcard[1] = card_player[0];
		}

		// figure out offset into file
		offset = 0;
		//for (i=1; i<pcard[0]; i++)  offset += (52-i)*1225;
		offset += card0_offset[pcard[0]];
		offset += (pcard[1]-pcard[0]-1)*1225;
		offset *= sizeof(byte);

		// seek to right position in file
		if ((pos = _lseek(_versus_fh, offset, SEEK_SET)) == -1L)
		{
			return false;
		}

		wintemp = lostemp = 0;

		for (i=0; i<=50; i++)
		{
			for (j=i+1; j<=51; j++)
			{

				if (i!=pcard[0] && i!=pcard[1] && j!=pcard[0] && j!=pcard[1])
				{
					_read(_versus_fh, &byte, sizeof(byte));
					memcpy(&wintemp, &byte[0], sizeof(unsigned int));
					memcpy(&lostemp, &byte[4], sizeof(unsigned int));

					_nwin += wintemp;
					_ntie += 1712304 - wintemp - lostemp;
					_nlos += lostemp;
					_nhands = _nhands + 1;

					if (wintemp<lostemp)
					{
						_nhandshi = _nhandshi + 1;
						nhiwin += wintemp;
						nhitie += 1712304 - wintemp - lostemp;
						nhilos += lostemp;
					}
					else if (wintemp>lostemp)
					{
						_nhandslo = _nhandslo + 1;
						nlowin += wintemp;
						nlotie += 1712304 - wintemp - lostemp;
						nlolos += lostemp;
					}
					else
					{
						_nhandsti = _nhandsti + 1;
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
						if ((StdDeck_SUIT(i)==StdDeck_SUIT(j) && p_formula->formula()->inlist[listnum][c0rank][c1rank]) ||
								(StdDeck_SUIT(i)!=StdDeck_SUIT(j) && p_formula->formula()->inlist[listnum][c1rank][c0rank]))
						{
							_nlistwin[listnum] += wintemp;
							_nlisttie[listnum] += 1712304 - wintemp - lostemp;
							_nlistlos[listnum] += lostemp;
						}
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// FLOP, TURN, RIVER
	else if (sym_br >= 2) 
	{

		// Common cards
		CardMask_RESET(comCardsScrape);
		if (sym_br >= 2) CardMask_SET(comCardsScrape, card_common[0]);
		if (sym_br >= 2) CardMask_SET(comCardsScrape, card_common[1]);
		if (sym_br >= 2) CardMask_SET(comCardsScrape, card_common[2]);
		if (sym_br >= 3) CardMask_SET(comCardsScrape, card_common[3]);
		if (sym_br >= 4) CardMask_SET(comCardsScrape, card_common[4]);

		// player cards
		CardMask_RESET(plCards);
		CardMask_SET(plCards, card_player[0]);
		CardMask_SET(plCards, card_player[1]);

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

					if (sym_br==2 || sym_br==3)
					{
						ENUMERATE_N_CARDS_D(comCardsEnum, sym_br==2 ? 2 : sym_br==3 ? 1 : 0, deadCards,
						{
							CardMask_OR(comCardsAll, comCardsScrape, comCardsEnum);
							DoCalc(plCards, oppCards, comCardsAll, &wintemp, &tietemp, &lostemp);
						});
					}
					else
					{
						DoCalc(plCards, oppCards, comCardsScrape, &wintemp, &tietemp, &lostemp);
					}

					_nwin += wintemp;
					_ntie += tietemp;
					_nlos += lostemp;
					_nhands = _nhands + 1;

					if (wintemp<lostemp)
					{
						_nhandshi = _nhandshi + 1;
						nhiwin += wintemp;
						nhitie += tietemp;
						nhilos += lostemp;
					}
					else if (wintemp>lostemp)
					{
						_nhandslo = _nhandslo + 1;
						nlowin += wintemp;
						nlotie += tietemp;
						nlolos += lostemp;
					}
					else
					{
						_nhandsti = _nhandsti + 1;
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
						if ((StdDeck_SUIT(i)==StdDeck_SUIT(j) && p_formula->formula()->inlist[listnum][c0rank][c1rank]) ||
								(StdDeck_SUIT(i)!=StdDeck_SUIT(j) && p_formula->formula()->inlist[listnum][c1rank][c0rank]))
						{
							_nlistwin[listnum] += wintemp;
							_nlisttie[listnum] += tietemp;
							_nlistlos[listnum] += lostemp;
						}
					}
				}
			}
		}
	}

	// calc the stats
	_vsprwin = (double) _nwin / ((double)  _nwin + (double) _ntie + (double) _nlos);
	_vsprtie = (double) _ntie / ((double)  _nwin + (double) _ntie + (double) _nlos);
	_vsprlos = (double) _nlos / ((double)  _nwin + (double) _ntie + (double) _nlos);

	_vsprwinhi = (double) nhiwin / ((double)  nhiwin + (double) nhitie + (double) nhilos);
	_vsprtiehi = (double) nhitie / ((double)  nhiwin + (double) nhitie + (double) nhilos);
	_vsprloshi = (double) nhilos / ((double)  nhiwin + (double) nhitie + (double) nhilos);

	_vsprwinti = (double) ntiwin / ((double)  ntiwin + (double) ntitie + (double) ntilos);
	_vsprtieti = (double) ntitie / ((double)  ntiwin + (double) ntitie + (double) ntilos);
	_vsprlosti = (double) ntilos / ((double)  ntiwin + (double) ntitie + (double) ntilos);

	_vsprwinlo = (double) nlowin / ((double)  nlowin + (double) nlotie + (double) nlolos);
	_vsprtielo = (double) nlotie / ((double)  nlowin + (double) nlotie + (double) nlolos);
	_vsprloslo = (double) nlolos / ((double)  nlowin + (double) nlotie + (double) nlolos);

	return true;
}

void CVersus::DoCalc(const CardMask plCards, const CardMask oppCards, const CardMask comCards,
					 unsigned int *wintemp, unsigned int *tietemp, unsigned int *lostemp) 
{
	CardMask		playerEvalCards, oppEvalCards;
	HandVal			player_hv = 0, opp_hv = 0;
	unsigned int	player_pokval = 0, opp_pokval = 0;
	double			dummy = 0.;

	CardMask_OR(playerEvalCards, plCards, comCards);
	CardMask_OR(oppEvalCards, oppCards, comCards);

	player_hv = Hand_EVAL_N(playerEvalCards, 7);
	opp_hv = Hand_EVAL_N(oppEvalCards, 7);

	player_pokval = p_symbols->CalcPokerval(player_hv, 7, &dummy, 0, 1);
	opp_pokval = p_symbols->CalcPokerval(opp_hv, 7, &dummy, 0, 1);

	if (player_pokval > opp_pokval)
		*wintemp = *wintemp + 1;

	else if (player_pokval < opp_pokval)
		*lostemp = *lostemp + 1;

	else
		*tietemp = *tietemp + 1;
}
