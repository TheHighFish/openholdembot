#include "stdafx.h"

#include "CMyHand.h"

#include "CSymbols.h"
#include "..\CTransform\CTransform.h"

CMyHand::CMyHand()
{
}

CMyHand::~CMyHand()
{
}

double	CMyHand::ProcessQuery(const char * pquery, int *e)
{
	if (memcmp(pquery,"mh_3straight", 12) == 0)			return ThreeStraight((pquery[12]-'0')!=0, (pquery[13]-'0')!=0);
	if (memcmp(pquery,"mh_bottomsd", 11) == 0)			return BottomStraightDraw();
	if (memcmp(pquery,"mh_nsuitedbetter", 16) == 0)		return NSuitedBetter();
	if (memcmp(pquery,"mh_kickerbetter", 15) == 0)		return KickerBetter();
	if (memcmp(pquery,"mh_kickerrank", 13) == 0)		return KickerRank();
	if (memcmp(pquery,"mh_nouts", 9) == 0)				return NOuts();
	if (memcmp(pquery,"mh_str_strflush", 15) == 0)		return StrengthStraightFlush();
	if (memcmp(pquery,"mh_str_quads", 12) == 0)			return StrengthQuads();
	if (memcmp(pquery,"mh_str_fullhouse", 16) == 0)		return StrengthFullHouse();
	if (memcmp(pquery,"mh_str_flush", 12) == 0)			return StrengthFlush();
	if (memcmp(pquery,"mh_str_straight", 15) == 0)		return StrengthStraight();
	if (memcmp(pquery,"mh_str_trips", 12) == 0)			return StrengthTrips();
	if (memcmp(pquery,"mh_str_twopair", 14) == 0)		return StrengthTwoPair();
	if (memcmp(pquery,"mh_str_onepair", 14) == 0)		return StrengthOnePair();

	*e = ERR_INVALID_SYM;
	return 0.0;
}

void CMyHand::MyHand(char * hand)
{
	int		$$pr0 = (int) p_symbols->sym()->$$pr[0];
	int		$$pr1 = (int) p_symbols->sym()->$$pr[1];
	bool	issuited = p_symbols->sym()->issuited;

	switch ($$pr0 >= $$pr1 ? $$pr0 : $$pr1)
	{
	case 10: hand[0]='T';
		break;
	case 11:
		hand[0]='J';
		break;
	case 12:
		hand[0]='Q';
		break;
	case 13:
		hand[0]='K';
		break;
	case 14:
		hand[0]='A';
		break;
	default:
		hand[0]='0'+$$pr0;
		break;
	}

	switch ($$pr0 >= $$pr1 ? $$pr1 : $$pr0)
	{
	case 10: hand[1]='T';
		break;
	case 11:
		hand[1]='J';
		break;
	case 12:
		hand[1]='Q';
		break;
	case 13:
		hand[1]='K';
		break;
	case 14:
		hand[1]='A';
		break;
	default:
		hand[1]='0'+$$pr1;
		break;
	}

	if (issuited)
	{
		hand[2]='s';
		hand[3]='\0';
	}
	else
	{
		hand[2]=' ';
		hand[3]='\0';
	}
}

const bool CMyHand::ThreeStraight(bool wheel, bool broadway)
{
	int		rankbitscommon = (int) p_symbols->sym()->rankbitscommon;

	if (bitcount(rankbitscommon&0x003e)>=3)
	{
		if (wheel)
			return true;
		else
			return false;
	}

	if (bitcount(rankbitscommon&0x7c00)>=3)
	{
		if (broadway)
			return true;
		else
			return false;
	}

	if (!wheel && !broadway)
		return true;
	else
		return false ;
}

const bool CMyHand::BottomStraightDraw(void)
{
	int		nstraight = (int) p_symbols->sym()->nstraight;
	int		nstraightfillcommon = (int) p_symbols->sym()->nstraightfillcommon;
	int		nstraightfill = (int) p_symbols->sym()->nstraightfill;
	int		rankbits = (int) p_symbols->sym()->rankbits;
	int		rankloplayer = (int) p_symbols->sym()->rankloplayer;
	int		rankhiplayer = (int) p_symbols->sym()->rankhiplayer;
	int		$$cr0 = (int) p_symbols->sym()->$$cr[0];
	int		$$cr1 = (int) p_symbols->sym()->$$cr[1];
	int		$$cr2 = (int) p_symbols->sym()->$$cr[2];
	int		$$cr3 = (int) p_symbols->sym()->$$cr[3];
	int		$$cr4 = (int) p_symbols->sym()->$$cr[4];

	if (nstraight==4 && nstraightfillcommon - nstraightfill==1 &&
		(
			( (rankbits >> rankloplayer&0x1f)==0xf &&
			  !(rankhiplayer==$$cr0 ||
				rankhiplayer==$$cr1 ||
				rankhiplayer==$$cr2 ||
				rankhiplayer==$$cr3 ||
				rankhiplayer==$$cr4)
			)
			||
			( (rankbits >> rankhiplayer&0x1f)==0xf &&
			  !(rankloplayer==$$cr0 ||
				rankloplayer==$$cr1 ||
				rankloplayer==$$cr2 ||
				rankloplayer==$$cr3 ||
				rankloplayer==$$cr4)
			)
		)
	) return true;

	return false;
}

const int CMyHand::NSuitedBetter(void)
{
	int		srankbits = (int) p_symbols->sym()->srankbits;
	int		srankhiplayer = (int) p_symbols->sym()->srankhiplayer;

	return bitcount(((~srankbits)&0x7ffe)>>srankhiplayer);
}

const int CMyHand::KickerBetter(void)
{
	int result = 0;

	int		rankhiplayer = (int) p_symbols->sym()->rankhiplayer;
	int		rankloplayer = (int) p_symbols->sym()->rankloplayer;
	int		rankbits = (int) p_symbols->sym()->rankbits;
	int		srankbits = (int) p_symbols->sym()->srankbits;
	int		nrankedcommon = (int) p_symbols->sym()->nrankedcommon;
	int		trank = (int) p_symbols->sym()->trank;
	int		twopair = (int) p_symbols->sym()->twopair;
	bool	ishicard = (bool) p_symbols->sym()->ishicard;
	bool	ispair = (bool) p_symbols->sym()->ispair;
	bool	isonepair = (bool) p_symbols->sym()->isonepair;
	bool	istwopair = (bool) p_symbols->sym()->istwopair;
	bool	isthreeofakind = (bool) p_symbols->sym()->isthreeofakind;
	bool	isfourofakind = (bool) p_symbols->sym()->isfourofakind;
	int		$$pr0 = (int) p_symbols->sym()->$$pr[0];
	int		$$pr1 = (int) p_symbols->sym()->$$pr[1];
	int		pokerval = (int) p_symbols->sym()->pokerval;
	int		pokervalcommon = (int) p_symbols->sym()->pokervalcommon;
	int		pcbits = (int) p_symbols->sym()->pcbits;
	int		npcbits = (int) p_symbols->sym()->npcbits;

	// quads, board quads
	if (isfourofakind && nrankedcommon==4)
	{
		result = bitcount(((~rankbits)&0x7ffe)>>rankhiplayer);
	}

	// trips, board trips
	else if (isthreeofakind && nrankedcommon==3)
	{
		result = bitcount(((~rankbits)&0x7ffe)>>rankhiplayer);
	}

	// trips, board paired
	else if (isthreeofakind && nrankedcommon==2)
	{
		if (trank==rankhiplayer)
			result = bitcount(((~rankbits)&0x7ffe)>>rankloplayer);

		else
			result = bitcount(((~rankbits)&0x7ffe)>>rankhiplayer);
	}

	// trips, board not paired
	// in this case, both hole cards participate in the set, thus kicker is shared
	else if (isthreeofakind && nrankedcommon==1)
	{
		result = 0;
	}

	// two pair, board has two pair
	else if (istwopair && (pokervalcommon&twopair))
	{
		result = bitcount(((~rankbits)&0x7ffe)>>rankhiplayer);
	}

	// two pair, board paired
	else if (istwopair && !(pokervalcommon&twopair) && nrankedcommon==2)
	{
		if (pcbits&1)
		{
			if ($$pr0==(pokerval&0xf))
				result = bitcount(((~rankbits)&0x7ffe)>>$$pr0);

			if ($$pr1==(pokerval&0xf))
				result = bitcount(((~rankbits)&0x7ffe)>>$$pr1);
		}
		else
		{
			result = 0;
		}
	}

	// two pair, board not paired
	// in this case, both hole cards participate in the two pair, thus kicker is shared
	else if (istwopair && nrankedcommon==1)
	{
		result = 0;
	}

	// one pair, board paired
	else if (isonepair && nrankedcommon==2)
	{
		result = bitcount(((~rankbits)&0x7ffe)>>rankhiplayer);
	}

	// one pair, board not paired
	else if (npcbits==2 && !ispair && isonepair && nrankedcommon==1)
	{
		if (trank==rankhiplayer)
			result = bitcount(((~rankbits)&0x7ffe)>>rankloplayer);

		else
			result = bitcount(((~rankbits)&0x7ffe)>>rankhiplayer);
	}

	// hi card
	else if (ishicard)
	{
		result = bitcount(((~rankbits)&0x7ffe)>>rankhiplayer);
	}

	return result;
}

const int CMyHand::KickerRank(void)
{
	int result = 0;

	int		rankhiplayer = (int) p_symbols->sym()->rankhiplayer;
	int		rankloplayer = (int) p_symbols->sym()->rankloplayer;
	int		nrankedcommon = (int) p_symbols->sym()->nrankedcommon;
	int		trank = (int) p_symbols->sym()->trank;
	bool	ispair = (bool) p_symbols->sym()->ispair;
	bool	ishicard = (bool) p_symbols->sym()->ishicard;
	bool	isonepair = (bool) p_symbols->sym()->isonepair;
	bool	istwopair = (bool) p_symbols->sym()->istwopair;
	bool	isthreeofakind = (bool) p_symbols->sym()->isthreeofakind;
	bool	isfourofakind = (bool) p_symbols->sym()->isfourofakind;
	int		pokerval = (int) p_symbols->sym()->pokerval;
	int		pokervalcommon = (int) p_symbols->sym()->pokervalcommon;
	int		npcbits = (int) p_symbols->sym()->npcbits;
	int		twopair = (int) p_symbols->sym()->twopair;

	// quads, board quads
	if (isfourofakind && nrankedcommon==4)
	{
		result =  (pokerval&0xf)>(pokervalcommon&0xf) ? pokerval&0xf : 0;
	}

	// trips, board trips
	else if (isthreeofakind && nrankedcommon==3)
	{
		result = ((pokerval>>4)&0xf)>((pokervalcommon>>4)&0xf) ? (pokerval>>4)&0xf : 0;
	}

	// trips, board paired
	else if (isthreeofakind && nrankedcommon==2)
	{
		result = trank==rankhiplayer ? rankloplayer : rankhiplayer;
	}

	// two pair, board has two pair
	else if (istwopair && (pokervalcommon&twopair))
	{
		result = rankhiplayer>(pokervalcommon&0xf) ? rankhiplayer : 0;
	}

	// one pair, board paired
	else if (isonepair && nrankedcommon==2)
	{
		result = rankhiplayer>((pokervalcommon>>8)&0xf) ? rankhiplayer : 0;
	}

	// one pair, board not paired, not pocket pair
	else if (isonepair && nrankedcommon==1 && !ispair)
	{
		if (trank==rankhiplayer)
			result = rankloplayer==((pokerval>>8)&0xf) ? rankloplayer : 0;

		else
			result = rankhiplayer==((pokerval>>8)&0xf) ? rankhiplayer : 0;
	}

	// hi card
	else if (ishicard)
	{
		if (npcbits==0)
			result = 0;

		else if (rankhiplayer!=((pokerval>>16)&0xf))
			result = rankhiplayer;

		else if (npcbits==2)
			result = rankloplayer;

		else
			result = 0;
	}

	return result;
}

const double CMyHand::NOuts(void)
{
	double nouts = 0.0;

	int		onepair = (int) p_symbols->sym()->onepair;
	int		twopair = (int) p_symbols->sym()->twopair;
	int		br = (int) p_symbols->sym()->br;
	int		pokerval = (int) p_symbols->sym()->pokerval;
	int		pokervalcommon = (int) p_symbols->sym()->pokervalcommon;
	bool	ispair = (bool) p_symbols->sym()->ispair;
	bool	issuited = (bool) p_symbols->sym()->issuited;
	int		ranklo = (int) p_symbols->sym()->ranklo;
	int		rankbits = (int) p_symbols->sym()->rankbits;
	int		srankbits = (int) p_symbols->sym()->srankbits;
	int		rankloplayer = (int) p_symbols->sym()->rankloplayer;
	int		rankhiplayer = (int) p_symbols->sym()->rankhiplayer;
	int		ranklocommon = (int) p_symbols->sym()->ranklocommon;
	int		rankhicommon = (int) p_symbols->sym()->rankhicommon;
	int		nsuited = (int) p_symbols->sym()->nsuited;
	int		nsuitedcommon = (int) p_symbols->sym()->nsuitedcommon;
	int		nrankedcommon = (int) p_symbols->sym()->nrankedcommon;
	int		srankhiplayer = (int) p_symbols->sym()->srankhiplayer;
	int		nstraight = (int) p_symbols->sym()->nstraight;
	int		nstraightcommon = (int) p_symbols->sym()->nstraightcommon;
	int		nstraightfill = (int) p_symbols->sym()->nstraightfill;
	int		nstraightfillcommon = (int) p_symbols->sym()->nstraightfillcommon;
	int		nopponentsplaying = (int) p_symbols->sym()->nopponentsplaying;
	int		nranked = (int) p_symbols->sym()->nranked;
	int		nhandshi = (int) p_symbols->sym()->nhandshi;
	int		$$cr0 = (int) p_symbols->sym()->$$cr[0];
	int		$$cr1 = (int) p_symbols->sym()->$$cr[1];
	int		$$cr2 = (int) p_symbols->sym()->$$cr[2];
	int		$$cr3 = (int) p_symbols->sym()->$$cr[3];
	int		$$cr4 = (int) p_symbols->sym()->$$cr[4];
	int		$$cs0 = (int) p_symbols->sym()->$$cs[0];
	int		$$cs1 = (int) p_symbols->sym()->$$cs[1];
	int		$$cs2 = (int) p_symbols->sym()->$$cs[2];
	int		$$cs3 = (int) p_symbols->sym()->$$cs[3];
	int		$$cs4 = (int) p_symbols->sym()->$$cs[4];
	

	if (br == 2)
	{
		//FLUSH OUTS
		if (nsuited==4 && nsuitedcommon==2 && bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)<=4)
		{
			nouts+=9;
		}

		else if (nsuited==4 && nsuitedcommon==2 && bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)>4)
		{
			nouts+=8;
		}

		else if (nsuited==4 && nsuitedcommon==3 && bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)<=1)
		{
			nouts+=9;
		}

		else if (nsuited==4 && nsuitedcommon==3 &&
				 bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)>1 &&
				 bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)<4)
		{
			nouts+=4;
		}

		if (nsuited==4 && nsuitedcommon==2 && (pokerval&onepair) && nrankedcommon==1)
		{
			nouts+=4;
		}

		if (nsuited==4 && nsuitedcommon==3 && (pokerval&onepair) && nrankedcommon==1 &&
				bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)<=1)
		{
			nouts+=4;
		}

		//STRAIGHT OUTS
		if (nstraight==4 && nstraightfillcommon>1 && (rankbits&16398)!=16398 && (rankbits&28674)!=28674)
		{
			nouts+=8;
		}

		else if ((rankbits>>ranklo)==0x5d)
		{
			nouts+=9;
		}

		else if (nstraight==4 && nstraightcommon==3 && rankloplayer>rankhicommon &&
				 rankhiplayer-rankloplayer<=2 && (rankbits&16398)!=16398 && (rankbits&28674)!=28674)
		{
			nouts+=8;
		}

		else if (nstraightfill==1 && nstraightfillcommon>=2)
		{
			nouts+=4;
		}

		//OVERCARD OUTS
		if (rankloplayer>rankhicommon && rankloplayer>=11 && nopponentsplaying==1 && nsuitedcommon<=2 &&
			nsuited>nsuitedcommon && nstraightfillcommon>1 && !ispair)
		{
			nouts+=4;
		}

		if (rankloplayer>rankhicommon && rankloplayer>=12 && !(nsuitedcommon<=2 && nsuited==nsuitedcommon) &&
			nstraightfillcommon>2)
		{
			nouts+=4.5;
		}

		if (nsuitedcommon<=2 && rankhiplayer>rankhicommon && rankhiplayer>11 && rankloplayer>9 && nstraightfillcommon>=2)
		{
			nouts+=2;
		}

		else if (nsuited==3 && nsuitedcommon==2 && rankloplayer>rankhicommon && nstraightfillcommon>=2)
		{
			nouts+=1.5;
		}

		else if (nsuited==3 && nsuitedcommon==2 && rankhiplayer>rankhicommon && rankloplayer>rankhicommon && 
				 nstraightfillcommon>=2)
		{
			nouts+=1.5;
		}

		else if (nsuited==3 && nsuited>nsuitedcommon && nstraightfillcommon>2 && rankloplayer>rankhicommon &&
				 rankloplayer>=12)
		{
			nouts+=2;
		}

		//BACKDOOR OUTS
		if (issuited && nsuited==3)
		{
			nouts+=1.5;
		}

		if (nstraight==3 && nstraightcommon==1)
		{
			nouts+=1.5;
		}

		if (nstraight==2 && nstraightfill==2 && nstraightcommon==1)
		{
			nouts+=1;
		}

		if (nstraight==1 && nstraightfill==2 && nstraightcommon==1)
		{
			nouts+=0.5;
		}

		if (nstraightfillcommon>=2 && nsuitedcommon==1 && nranked==2 && nrankedcommon==1 && rankhiplayer==14)
		{
			nouts+=5;
		}

		//DISCOUNT OUTS FOR DANGEROUS BOARDS
		if (nrankedcommon==2)
		{
			nouts-=3.5;
		}

		else if (nrankedcommon==3)
		{
			nouts-=10;
		}

		if (rankloplayer>rankhicommon && rankloplayer>=12 && 
			( (nsuitedcommon>1 && nsuited<4) ||
			  (nstraightcommon>1 && nstraight<4) ) )
		{
			nouts-=nsuitedcommon+nstraightcommon;
		}

		if ( (nstraight==4 && nstraightfillcommon>2 && (rankbits&16398)!=16398 && (rankbits&28674)!=28674) ||
			 (nstraightfill==1 && nstraightfillcommon>=2) ||
			 (rankbits>>ranklo)==0x5d )
		{
			if (nsuitedcommon==2)
			{
				nouts-=2;
			}

			else if (nsuitedcommon==3 && nsuited>nsuitedcommon &&
					 bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)<=2)
			{
				nouts-=2;
			}

			else if (nsuitedcommon==3)
			{
				nouts-=8;
			}
		}
	}

	else if (br == 3)
	{
		//FLUSH OUTS
		if (nsuited==4 && nsuitedcommon==2 && bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)<=4)
		{
			nouts+=9;
		}

		else if (nsuited==4 && nsuitedcommon==2 && bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)>4)
		{
			nouts+=8;
		}

		else if (nsuited==4 && nsuitedcommon==3 && bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)<=1)
		{
			nouts+=9;
		}

		else if (nsuited==4 && nsuitedcommon==3 &&
				 bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)>1 &&
				 bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)<4)
		{
			nouts+=3;
		}

		if (nsuited==4 && nsuitedcommon==2 && (pokerval&onepair) && nrankedcommon==1)
		{
			nouts+=4;
		}

		if (nsuited==4 && nsuitedcommon==3 &&
			(pokerval&onepair) && nrankedcommon==1 &&
			bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)<=1)
		{
			nouts+=4;
		}

		//STRAIGHT OUTS
		if (nstraight==4 && nstraightfillcommon>1 &&
				(rankbits&16398)!=16398 && (rankbits&28674)!=28674)
		{
			nouts+=8;
		}

		else if ((rankbits>>ranklo)==0x5d)
		{
			nouts+=9;
		}

		else if (nstraight==4 && nstraightcommon==3 && rankloplayer>rankhicommon &&
				 rankhiplayer-rankloplayer<=2 &&
				 (rankbits&16398)!=16398 && (rankbits&28674)!=28674)
		{
			nouts+=8;
		}

		else if (nstraightfill==1 && nstraightfillcommon>=2)
		{
			nouts+=4;
		}

		//OVERCARD OUTS
		if (rankloplayer>rankhicommon && rankloplayer>=12 && nsuitedcommon<=2 && nsuited>nsuitedcommon &&
			nstraightfillcommon>2)
		{
			nouts+=3;
		}

		if (nsuitedcommon<=2 && rankhiplayer>rankhicommon && rankhiplayer>11 && rankloplayer>9 && 
			nstraightfillcommon>=2)
		{
			nouts+=2;
		}

		if (nsuited==4 && nsuitedcommon==2 && rankloplayer>rankhicommon && nstraightfillcommon>=2)
		{
			nouts+=2;
		}

		else if (nsuited==4 && nsuitedcommon==2 && rankhiplayer>rankhicommon &&
				 rankloplayer<rankhicommon && nstraightfillcommon>=2)
		{
			nouts+=1;
		}

		//DISCOUNT OUTS FOR DANGEROUS BOARDS
		if (pokervalcommon&twopair)
		{
			nouts-=12;
		}

		else if (nrankedcommon==2 && ($$cr0==$$cr3 || $$cr1==$$cr3 || $$cr2==$$cr3))
		{
			nouts-=1;
		}

		else if (nrankedcommon==2)
		{
			nouts-=3;
		}

		else if (nrankedcommon==3)
		{
			nouts-=12;
		}

		if (rankloplayer>rankhicommon && rankloplayer>=12 &&
			( (nsuitedcommon>1 && nsuited<4) ||
			  (nstraightcommon>1 && nstraight<4) ) )

		{
			nouts-=nsuitedcommon+nstraightcommon;
		}

		if ((nstraight==4 && nstraightfillcommon>2 &&
			(rankbits&16398)!=16398 && (rankbits&28674)!=28674) ||
			(nstraightfill==1 && nstraightfillcommon>=2) ||
			((rankbits>>ranklo)==0x5d) )
		{
			if (nsuitedcommon==2 && ($$cs0==$$cs3 || $$cs1==$$cs3 || $$cs2==$$cs3))
			{
				nouts-=0;
			}

			else if (nsuitedcommon==2)
			{
				nouts-=2;
			}

			else if (nsuitedcommon==3 && nsuited>nsuitedcommon && bitcount(((~srankbits)&0x7ffe)>>srankhiplayer)<=2)
			{
				nouts-=2;
			}

			else if (nsuitedcommon==3)
			{
				nouts-=8;
			}
		}
	}

	if (nouts <= 0.0)
		nouts = 0.0;

	return nouts;
}

const bool CMyHand::SafeBoard(void)
{
	bool result = false;

	int		nrankedcommon = (int) p_symbols->sym()->nrankedcommon;
	int		nsuitedcommon = (int) p_symbols->sym()->nsuitedcommon;
	int		nstraightcommon = (int) p_symbols->sym()->nstraightcommon;
	int		nstraightfillcommon = (int) p_symbols->sym()->nstraightfillcommon;

	if (nrankedcommon<=1 && nsuitedcommon<=2 && nstraightcommon<=2 && nstraightfillcommon>=2)
		result = true;

	else
		result = false;

	return result;
}

const int CMyHand::StrengthStraightFlush(void)
{
	int		strength = 0;

	int		pokervalcommon = (int) p_symbols->sym()->pokervalcommon;
	int		nhandshi = (int) p_symbols->sym()->nhandshi;
	bool	isstraightflush = (bool) p_symbols->sym()->isstraightflush;
	bool	isroyalflush = (bool) p_symbols->sym()->isroyalflush;
	int		straightflush = (int) p_symbols->sym()->straightflush;
	int		royalflush = (int) p_symbols->sym()->royalflush;

	if (!isstraightflush)
	{
		strength=0;
	}

	// strength 5
	else if
	(
		// nuts
		(isroyalflush && !(pokervalcommon&royalflush)) ||
		(isstraightflush && nhandshi==0 && !(pokervalcommon&straightflush))
	)
	{
		strength=5;
	}

	// strength 4
	else if
	(
		0
	)
	{
		strength=4;
	}

	// strength 3
	else if
	(
		// I may not have the highest, board does not have a str8flush
		(isstraightflush && !(pokervalcommon&straightflush))
	)
	{
		strength=3;
	}

	// strength 2
	else if
	(
		// I may not have the highest, board does have a str8flush
		(isstraightflush && (pokervalcommon&straightflush) && !(pokervalcommon&royalflush))
	)
	{
		strength=2;
	}

	// strength 1
	else if
	(
		// Chop pot
		(isroyalflush && (pokervalcommon&royalflush))
	)
	{
		strength=1;
	}

	return strength;
}

const int CMyHand::StrengthQuads(void)
{
	int		strength = 0;

	int		pokervalcommon = (int) p_symbols->sym()->pokervalcommon;
	bool	isfourofakind = (bool) p_symbols->sym()->isfourofakind;
	int		fourofakind = (int) p_symbols->sym()->fourofakind;
	int		nhandshi = (int) p_symbols->sym()->nhandshi;

	if (!isfourofakind)
	{
		strength=0;
	}

	// strength 5
	else if
	(
		// nuts
		(isfourofakind && nhandshi==0 && !(pokervalcommon&fourofakind)) ||
		// Board quads, I have an Ace
		(isfourofakind && (pokervalcommon&fourofakind) && p_symbols->IsHand("$Ax") )
	)
	{
		strength=5;
	}

	// strength 4
	else if
	(
		0
	)
	{
		strength=4;
	}

	// strength 3
	else if
	(
		// I may not have the highest, board does not have quads
		(isfourofakind && !(pokervalcommon&fourofakind))
	)
	{
		strength=3;
	}

	// strength 2
	else if
	(
		// Board quads, I have a King
		(isfourofakind && (pokervalcommon&fourofakind) && p_symbols->IsHand("$Kx") )
	)
	{
		strength=2;
	}

	// strength 1
	else if
	(
		// Board has quads, I don't have best kicker
		(isfourofakind && (pokervalcommon&fourofakind))
	)
	{
		strength=1;
	}

	return strength;
}

const int CMyHand::StrengthFullHouse(void)
{
	int			strength = 0;

	bool	isfullhouse = (bool) p_symbols->sym()->isfullhouse;
	int		fullhouse = (int) p_symbols->sym()->fullhouse;
	bool	ispair = (bool) p_symbols->sym()->ispair;
	int		twopair = (int) p_symbols->sym()->twopair;
	int		nhandshi = (int) p_symbols->sym()->nhandshi;
	int		nrankedcommon = (int) p_symbols->sym()->nrankedcommon;
	int		pokerval = (int) p_symbols->sym()->pokerval;
	int		pokervalcommon = (int) p_symbols->sym()->pokervalcommon;
	int		pcbits = (int) p_symbols->sym()->pcbits;
	int		trank = (int) p_symbols->sym()->trank;
	int		$$pr0 = (int) p_symbols->sym()->$$pr[0];
	int		$$pr1 = (int) p_symbols->sym()->$$pr[1];


	if (!isfullhouse)
	{
		strength=0;
	}

	// strength 5
	else if
	(
		// nuts
		// Board paired, I have pocket pair and top side full house, pocket pair is higher than non-participating board cards
		// e.g. QQ/QTT53
		(isfullhouse && ispair && nrankedcommon==2 && !(pokervalcommon&twopair) &&
		 ((pokerval>>8)&15)>(pokerval&15) && (pcbits&0x1c) &&
		 trank==((pokervalcommon>>8)&0xf) ) ||
		
		// Board trips, I have pocket pair and top side full house, pocket pair is higher than non-participating board card
		// e.g. QQ/QTTT3
		(isfullhouse && ispair && nrankedcommon==3 &&
		 ((pokerval>>8)&15)>(pokerval&15) && (pcbits&0x1c) &&
		 trank==((pokervalcommon>>4)&0xf) ) ||
		
		// Board paired, I have pocket pair and top side full house, pocket pair is lower than non-participating board cards
		// e.g. QQ/QTTA3
		(isfullhouse && ispair && nrankedcommon==2 && !(pokervalcommon&twopair) &&
		 ((pokerval>>8)&15)>(pokerval&15) && (pcbits&0x1c) &&
		 trank<((pokervalcommon>>8)&0xf)) ||

		// Board paired, I have pocket pair and bottom side full house, pocket pair is higher than non-participating board cards
		// e.g. QQ/QKKT6
		(isfullhouse && ispair && nrankedcommon==2 && !(pokervalcommon&twopair) &&
		 ((pokerval>>8)&15)<(pokerval&15) && (pcbits&0x1c) &&
		 trank==((pokervalcommon>>8)&0xf) ) ||

		// Board paired, I have pocket pair and bottom side full house, pocket pair is lower than non-participating board cards
		// e.g. QQ/QKKA6
		(isfullhouse && ispair && nrankedcommon==2 && !(pokervalcommon&twopair) &&
		 ((pokerval>>8)&15)<(pokerval&15) && (pcbits&0x1c) &&
		 trank<((pokervalcommon>>8)&0xf) )
		
	)
	{
		strength=5;
	}

	// strength 4
	else if
	(
		// Board trips, I have pocket pair and top side full house, pocket pair is lower than non-participating board card
		// e.g. QQ/QTTTA
		(isfullhouse && ispair && nrankedcommon==3 &&
		 ((pokerval>>8)&15)>(pokerval&15) && (pcbits&0x1c) &&
		 trank<((pokervalcommon>>4)&0xf) ) ||

		// Board paired, I don't have pocket pair, I have top side full house, pair is higher than non-participating board cards
		// e.g. QJ/QQJ53
		(isfullhouse && !ispair && nrankedcommon==2 && !(pokervalcommon&twopair) &&
		 ((pokerval>>8)&15)>(pokerval&15) && (pokerval&0xf)==((pokervalcommon>>8)&0xf)) ||

		// Board paired, I don't have pocket pair, I have top side full house, pair is lower than non-participating board cards
		// e.g. QJ/QQJA3
		(isfullhouse && !ispair && nrankedcommon==2 && !(pokervalcommon&twopair) &&
		 ((pokerval>>8)&15)>(pokerval&15) && (pokerval&0xf)<((pokervalcommon>>8)&0xf)) ||

		// Board paired, I don't have pocket pair, I have bottom side full house, pair is higher than non-participating board cards
		// e.g. QJ/QJJ53
		(isfullhouse && !ispair && nrankedcommon==2 && !(pokervalcommon&twopair) &&
		 ((pokerval>>8)&15)<(pokerval&15) && (pokerval&0xf)==((pokervalcommon>>8)&0xf)) ||

		// Board paired, I don't have pocket pair, I have bottom side full house, pair is lower than non-participating board cards
		// e.g. QJ/QJJ53
		(isfullhouse && !ispair && nrankedcommon==2 && !(pokervalcommon&twopair) &&
		 ((pokerval>>8)&15)<(pokerval&15) && (pokerval&0xf)<((pokervalcommon>>8)&0xf)) ||

		// Board has two pair, I have top trips
		// e.g. QT/QQJJ8
		(isfullhouse && (pokervalcommon&twopair) &&
		 ( ((pokervalcommon>>16)&0x0f)==$$pr0 || ((pokervalcommon>>16)&0x0f)==$$pr1 ))
	)
	{
		strength=4;
	}

	// strength 3
	else if
	(
		0
	)
	{
		strength=3;
	}

	// strength 2
	else if
	(
		// Board trips, I have pocket pair higher than non-participating board cards
		// e.g. QQ/JJJ92
		(isfullhouse && ispair && nrankedcommon==3 && (pcbits&0x03) && trank>((pokervalcommon>>4)&0xf)) ||

		// Board trips, I have pocket pair lower than non-participating board cards
		// e.g. QQ/JJJK2
		(isfullhouse && ispair && nrankedcommon==3 && (pcbits&0x03) && trank<((pokervalcommon>>4)&0xf)) ||

		// Board trips, only one of my hole cards participates, the pair rank is higher than non-participating board cards
		// e.g. QT/999T8
		(isfullhouse && nrankedcommon==3 && bitcount(pcbits&0x03)==1 && (pokerval&0xf)==((pokervalcommon>>4)&0xf)) ||

		// Board trips, only one of my hole cards participates, the pair rank is lower than non-participating board cards
		// e.g. QT/999TK
		(isfullhouse && nrankedcommon==3 && bitcount(pcbits&0x03)==1 && (pokerval&0xf)<((pokervalcommon>>4)&0xf))
	)
	{
		strength=2;
	}

	// strength 1
	else if
	(
		// Board has full house
		(isfullhouse && (pokervalcommon&fullhouse))
	)
	{
		strength=1;
	}

	return strength;
}

const int CMyHand::StrengthFlush(void)
{
	int		strength = 0;

	bool	ishiflush = (bool) p_symbols->sym()->ishiflush;
	bool	isflush = (bool) p_symbols->sym()->isflush;
	int		flush = (int) p_symbols->sym()->flush;
	int		twopair = (int) p_symbols->sym()->twopair;
	int		pokerval = (int) p_symbols->sym()->pokerval;
	int		pokervalcommon = (int) p_symbols->sym()->pokervalcommon;
	int		$$cs3 = (int) p_symbols->sym()->$$cs[3];
	int		srankbits = (int) p_symbols->sym()->srankbits;
	int		srankhiplayer = (int) p_symbols->sym()->srankhiplayer;
	int		nrankedcommon = (int) p_symbols->sym()->nrankedcommon;
	int		nsuitedcommon = (int) p_symbols->sym()->nsuitedcommon;
	int		nstraightflushfillcommon = (int) p_symbols->sym()->nstraightflushfillcommon;
	int		tsuitcommon = (int) p_symbols->sym()->tsuitcommon;

	if (!isflush)
	{
		strength=0;
	}

	// strength 5
	else if
	(
		// nuts
		// Big raise
		(ishiflush && (!(pokervalcommon&flush) || pokerval>pokervalcommon) &&
		 nrankedcommon<=2 && nstraightflushfillcommon>=3 )
	)
	{
		strength=5;
	}

	// strength 4
	else if
	(
		// Medium raise
		// one card can beat my flush
		(isflush && bitcount(((~srankbits)&0x7fff)>>srankhiplayer)<=1 && nrankedcommon<=2 && nsuitedcommon<=4) ||

		(ishiflush && (!(pokervalcommon&flush) || pokerval>pokervalcommon) && nrankedcommon<=2 &&
		 nstraightflushfillcommon>=2)
	)
	{
		strength=4;
	}

	// strength 3
	else if
	(
		// Small raise
		// two or cards can beat my flush, board not paired
		(isflush && bitcount(((~srankbits)&0x7fff)>>srankhiplayer)<=2 && nrankedcommon==1 && nsuitedcommon<=4) ||

		// three of more cards can beat my flush, board may be paired, flush came on turn
		(isflush && bitcount(((~srankbits)&0x7fff)>>srankhiplayer)>=3 &&
		 nrankedcommon<=2 && nsuitedcommon==3 && $$cs3==tsuitcommon)
	)
	{
		strength=3;
	}

	// strength 2
	else if
	(
		// Call
		// three or less cards can beat my flush
		(isflush && bitcount(((~srankbits)&0x7fff)>>srankhiplayer)<=3 && nrankedcommon<=2 && nsuitedcommon<=4) ||

		// two or less cards can beat my flush, board paired
		(isflush && bitcount(((~srankbits)&0x7fff)>>srankhiplayer)<=2 && nrankedcommon==2 && nsuitedcommon<=4)
	)
	{
		strength=2;
	}

	// strength 1
	else if
	(
		// Don't call these
		// four or more cards can beat my flush
		(isflush && bitcount(((~srankbits)&0x7fff)>>srankhiplayer)>=4 && nsuitedcommon<=4) ||

		// board has trips or more
		(isflush && nrankedcommon>=3) ||

		// board has flush, my cards don't help
		(isflush && pokerval<pokervalcommon && (pokervalcommon&flush))
	)
	{
		strength=1;
	}

	// FlushDiscount
	if (isflush)
	{
		if (pokervalcommon&twopair)
			strength-=2;
	}

	return strength;
}

const int CMyHand::StrengthStraight(void)
{
	int		strength = 0;

	bool	isstraight = (bool) p_symbols->sym()->isstraight;
	int		nstraightfillcommon = (int) p_symbols->sym()->nstraightfillcommon;
	int		$$pr0 = (int) p_symbols->sym()->$$pr[0];
	int		$$pr1 = (int) p_symbols->sym()->$$pr[1];
	int		rankloplayer = (int) p_symbols->sym()->rankloplayer;
	int		pokerval = (int) p_symbols->sym()->pokerval;
	int		pokervalcommon = (int) p_symbols->sym()->pokervalcommon;
	int		twopair = (int) p_symbols->sym()->twopair;
	int		nsuited = (int) p_symbols->sym()->nsuited;
	int		nsuitedcommon = (int) p_symbols->sym()->nsuitedcommon;
	int		nrankedcommon = (int) p_symbols->sym()->nrankedcommon;
	int		srankbits = (int) p_symbols->sym()->srankbits;
	int		srankhiplayer = (int) p_symbols->sym()->srankhiplayer;

	if (!isstraight)
	{
		strength=0;
	}

	// strength 5
	else if
	(
		//Board 3 to a straight, cards in rank 1 and rank 2 (high end)
		(isstraight && nstraightfillcommon==2 &&
		 ( ( ($$pr0==((pokerval>>16)&0xf)) && ($$pr1==((pokerval>>12)&0xf)) ) ||
		   ( ($$pr1==((pokerval>>16)&0xf)) && ($$pr0==((pokerval>>12)&0xf)) ) ) ) ||

		//Board 3 to a straight, cards in rank 1 and rank 3
		(isstraight && nstraightfillcommon==2 &&
		 ( ( ($$pr0==((pokerval>>16)&0xf)) && ($$pr1==((pokerval>>8)&0xf)) ) ||
		   ( ($$pr1==((pokerval>>16)&0xf)) && ($$pr0==((pokerval>>8)&0xf)) ) ) ) ||

		//Board 3 to a straight, cards in rank 1 and rank 4
		(isstraight && nstraightfillcommon==2 &&
		 ( ( ($$pr0==((pokerval>>16)&0xf)) && ($$pr1==((pokerval>>4)&0xf)) ) ||
		   ( ($$pr1==((pokerval>>16)&0xf)) && ($$pr0==((pokerval>>4)&0xf)) ) ) ) ||

		//Board 4 to a straight, card used in rank 1, nut straight with an ace
		(isstraight && nstraightfillcommon==1 &&
		 ( ( ($$pr0==((pokerval>>16)&0xf)) && ($$pr0==14)) ||
		   ( ($$pr1==((pokerval>>16)&0xf)) && ($$pr1==14)) ) ) ||

		//Board 4 to a straight, card used in rank 2, nut straight with a king
		(isstraight && nstraightfillcommon==1 &&
		 ( ( ($$pr0==((pokerval>>12)&0xf)) && ($$pr0==13)) ||
		   ( ($$pr1==((pokerval>>12)&0xf)) && ($$pr1==13)) ) ) ||

		//Board 4 to a straight, card used in rank 3, nut straight with a queen
		(isstraight && nstraightfillcommon==1 &&
		 ( ( ($$pr0==((pokerval>>8)&0xf)) && ($$pr0==12)) ||
		   ( ($$pr1==((pokerval>>8)&0xf)) && ($$pr1==12)) ) ) ||

		//Board 4 to a straight, card used in rank 4, nut straight with a jack
		(isstraight && nstraightfillcommon==1 &&
		 ( ( ($$pr0==((pokerval>>4)&0xf)) && ($$pr0==11)) ||
		   ( ($$pr1==((pokerval>>4)&0xf)) && ($$pr1==11)) ) ) ||

		//Board 4 to a straight, card used in rank 5, nut straight with a ten
		(isstraight && nstraightfillcommon==1 &&
		 ( ( ($$pr0==((pokerval>>0)&0xf)) && ($$pr0==10)) ||
		   ( ($$pr1==((pokerval>>0)&0xf)) && ($$pr1==10)) ) ) ||

		//Board 3 to a straight, nut straights
		(isstraight && nstraightfillcommon==2 && rankloplayer>=10 )
	)
	{
		strength=5;
	}

	// strength 4
	else if
	(
		//Board 3 to a straight, cards in rank 1 and rank 5
		(isstraight && nstraightfillcommon==2 &&
		 ( ( ($$pr0==((pokerval>>16)&0xf)) && ($$pr1==((pokerval>>0)&0xf))) ||
		   ( ($$pr1==((pokerval>>16)&0xf)) && ($$pr0==((pokerval>>0)&0xf))) ) ) ||

		//Board 3 to a straight, cards in rank 2 and rank 3
		(isstraight && nstraightfillcommon==2 &&
		 ( ( ($$pr0==((pokerval>>12)&0xf)) && ($$pr1==((pokerval>>8)&0xf))) ||
		   ( ($$pr1==((pokerval>>12)&0xf)) && ($$pr0==((pokerval>>8)&0xf))) ) ) ||

		//Board 3 to a straight, cards in rank 2 and rank 4
		(isstraight && nstraightfillcommon==2 &&
		 ( ( ($$pr0==((pokerval>>12)&0xf)) && ($$pr1==((pokerval>>4)&0xf))) ||
		   ( ($$pr1==((pokerval>>12)&0xf)) && ($$pr0==((pokerval>>4)&0xf))) ) )
	)
	{
		strength=4;
	}

	// strength 3
	else if
	(
		//Board 3 to a straight, cards in rank 2 and rank 5
		(isstraight && nstraightfillcommon==2 &&
		 ( ( ($$pr0==((pokerval>>12)&0xf)) && ($$pr1==((pokerval>>0)&0xf))) ||
		   ( ($$pr1==((pokerval>>12)&0xf)) && ($$pr0==((pokerval>>0)&0xf))) ) ) ||

		//Board 3 to a straight, cards in rank 3 and rank 4
		(isstraight && nstraightfillcommon==2 &&
		 ( ( ($$pr0==((pokerval>>8)&0xf)) && ($$pr1==((pokerval>>4)&0xf))) ||
		   ( ($$pr1==((pokerval>>8)&0xf)) && ($$pr0==((pokerval>>4)&0xf))) ) ) ||

		//Board 3 to a straight, cards in rank 3 and rank 5
		(isstraight && nstraightfillcommon==2 &&
		 ( ( ($$pr0==((pokerval>>8)&0xf)) && ($$pr1==((pokerval>>0)&0xf))) ||
		   ( ($$pr1==((pokerval>>8)&0xf)) && ($$pr0==((pokerval>>0)&0xf))) ) ) ||

		//Board 3 to a straight, cards in rank 4 and rank 5 (idiot end)
		(isstraight && nstraightfillcommon==2 &&
		 ( ( ($$pr0==((pokerval>>4)&0xf)) && ($$pr1==((pokerval>>0)&0xf))) ||
		   ( ($$pr1==((pokerval>>4)&0xf)) && ($$pr0==((pokerval>>0)&0xf))) ) ) ||

		//Board 4 to a straight, card used in rank 1
		(isstraight && nstraightfillcommon==1 &&
		 ( ($$pr0==((pokerval>>16)&0xf)) || ($$pr1==((pokerval>>16)&0xf)) ) ) ||

		//Board 4 to a straight, card used in rank 2
		(isstraight && nstraightfillcommon==1 &&
		 ( ($$pr0==((pokerval>>12)&0xf)) || ($$pr1==((pokerval>>12)&0xf)) ) )
	)
	{
		strength=3;
	}

	// strength 2
	else if
	(
		//Board 4 to a straight, card used in rank 3
		(isstraight && nstraightfillcommon==1 &&
		 ( ($$pr0==((pokerval>>8)&0xf)) || ($$pr1==((pokerval>>8)&0xf)) ) ) ||

		//Board 4 to a straight, card used in rank 4
		(isstraight && nstraightfillcommon==1 &&
		 ( ($$pr0==((pokerval>>4)&0xf)) || ($$pr1==((pokerval>>4)&0xf)) ) )
	)
	{
		strength=2;
	}

	// strength 1
	else if
	(
		//Board 4 to a straight, card used in rank 5
		(isstraight && nstraightfillcommon==1 &&
		 ( ($$pr0==((pokerval>>0)&0xf)) || ($$pr1==((pokerval>>0)&0xf)) ) ) ||

		//Board has straight
		(isstraight && nstraightfillcommon==0)
	)
	{
		strength=1;
	}

	// StraightDiscount
	if (isstraight)
	{
		if (nsuitedcommon==3)
			strength-=1;

		if (nsuitedcommon>=4)
			strength-=2;

		if (nrankedcommon>=3)
			strength-=1;

		if (pokervalcommon&twopair)
			strength-=2;

		// Negate flush discount if we have a good flush draw
		if (nsuitedcommon==3 && nsuited==4 &&
				bitcount(((~srankbits)&0x7fff)>>srankhiplayer)<=2 )
			strength+=1;
	}

	return strength;
}

const int CMyHand::StrengthTrips(void)
{
	int		strength = 0;
 
	bool	isthreeofakind = (bool) p_symbols->sym()->isthreeofakind;
	int		nrankedcommon = (int) p_symbols->sym()->nrankedcommon;
	int		nsuitedcommon = (int) p_symbols->sym()->nsuitedcommon;
	bool	ispair = (bool) p_symbols->sym()->ispair;
	int		pokerval = (int) p_symbols->sym()->pokerval;
	int		ranklocommon = (int) p_symbols->sym()->ranklocommon;
	int		rankhicommon = (int) p_symbols->sym()->rankhicommon;
	int		rankloplayer = (int) p_symbols->sym()->rankloplayer;
	int		rankhiplayer = (int) p_symbols->sym()->rankhiplayer;
	int		trank = (int) p_symbols->sym()->trank;
	int		$$pr0 = (int) p_symbols->sym()->$$pr[0];
	int		$$pr1 = (int) p_symbols->sym()->$$pr[1];
	int		nstraightfillcommon = (int) p_symbols->sym()->nstraightfillcommon;
	int		nstraightcommon = (int) p_symbols->sym()->nstraightcommon;

	if (!isthreeofakind)
	{
		strength=0;
	}

	// strength 5
	else if
	(
		//Board has No Pair and you have top set
		(nrankedcommon==1 && ispair && isthreeofakind && rankhiplayer==rankhicommon)
	)
	{
		strength=5;
	}

	// strength 4
	else if
	(
		//Board has No Pair and you have middle set
		(nrankedcommon==1 && ispair && isthreeofakind && rankhiplayer<rankhicommon &&
		 rankhiplayer>ranklocommon ) ||

		//Board has Pair and you have Trips w/ good kicker (used as 4th ranked card)
		(nrankedcommon==2 && isthreeofakind &&
		 ((trank==$$pr0 && $$pr1==((pokerval>>4)&0xf)) ||
		  (trank==$$pr1 && $$pr0==((pokerval>>4)&0xf)) ) )
	)
	{
		strength=4;
	}

	// strength 3
	else if
	(
		//Board has Pair and you have Trips w/ moderate kicker (used as 5th ranked card)
		(nrankedcommon==2 && isthreeofakind &&
		 ((trank==$$pr0 && $$pr1==((pokerval>>0)&0xf)) ||
		  (trank==$$pr1 && $$pr0==((pokerval>>0)&0xf)) ) ) ||

		//Board has No Pair and you have bottom set
		(nrankedcommon==1 && ispair && isthreeofakind && rankhiplayer==ranklocommon ) ||

		//Board not paired catch all
		(isthreeofakind && nrankedcommon==1 )
	)
	{
		strength=3;
	}

	// strength 2
	else if
	(
		//Board has Pair and you have Trips w/ poor kicker (not used)
		(nrankedcommon==2 && isthreeofakind &&
		 ((trank==$$pr0 && $$pr1<((pokerval>>0)&0xf)) ||
		  (trank==$$pr1 && $$pr0<((pokerval>>0)&0xf)) ) ) ||

		//Board has 3OAK good kickers
		(isthreeofakind && nrankedcommon==3 && rankhiplayer>=13 && rankloplayer>=10 ) ||

		//Board has pair catch all
		(isthreeofakind && nrankedcommon==2 )
	)
	{
		strength=2;
	}

	// strength 1
	else if
	(
		//Board has 3OAK catch all
		(isthreeofakind && nrankedcommon==3 )
	)
	{
		strength=1;
	}

	// TripsDiscount
	if (isthreeofakind)
	{
		if (nsuitedcommon>=3)
			strength-=1;

		if (nsuitedcommon>=4)
			strength-=1;

		if (nstraightfillcommon<=1 || nstraightcommon>=4)
			strength-=1;
	}

	return strength;
}

const int CMyHand::StrengthTwoPair(void)
{
	int		strength = 0;

	bool	istwopair = (bool) p_symbols->sym()->istwopair;
	int		twopair = (int) p_symbols->sym()->twopair;
	int		pokerval = (int) p_symbols->sym()->pokerval;
	int		pokervalcommon = (int) p_symbols->sym()->pokervalcommon;
	int		nrankedcommon = (int) p_symbols->sym()->nrankedcommon;
	int		nsuitedcommon = (int) p_symbols->sym()->nsuitedcommon;
	int		rankloplayer = (int) p_symbols->sym()->rankloplayer;
	int		ranklocommon = (int) p_symbols->sym()->ranklocommon;
	int		$$pr0 = (int) p_symbols->sym()->$$pr[0];
	int		$$pr1 = (int) p_symbols->sym()->$$pr[1];
	int		trankcommon = (int) p_symbols->sym()->trankcommon;
	bool	ispair = (bool) p_symbols->sym()->ispair;
	int		nstraightcommon = (int) p_symbols->sym()->nstraightcommon;
	int		nstraightfillcommon = (int) p_symbols->sym()->nstraightfillcommon;

	if (!istwopair)
	{
		strength=0;
	}

	// strength 5
	else if
	(
		//Board has No Pair and you have Top two pairs
		(nrankedcommon==1 && istwopair && (pokerval&15)<((pokerval>>4)&15) )
	)
	{
		strength=5;
	}

	// strength 4
	else if
	(
		//Board has No Pair and you have Top and bottom pair
		(nrankedcommon==1 && istwopair &&
		 (pokerval&15)<((pokerval>>16)&15) &&
		 (pokerval&15)>((pokerval>>4)&15) ) ||

		//Board has No Pair and you have Middle two pairs
		(nrankedcommon==1 && istwopair &&
		 (pokerval&15)>((pokerval>>8)&15) &&
		 (pokerval&15)>((pokerval>>4)&15) &&
		 !(rankloplayer==ranklocommon) ) ||

		//Rainbow board
		(istwopair && SafeBoard() )
	)
	{
		strength=4;
	}
	
	// strength 3
	else if
	(
		//Board has Pair and you have Overpair
		(nrankedcommon==2 && istwopair &&
		 ($$pr0==((pokerval>>16)&0xf) ||
		  $$pr1==((pokerval>>16)&0xf)) ) ||
		(nrankedcommon==2 && istwopair && ispair && $$pr0>trankcommon) ||

		//Board has No Pair and you have Bottom two pairs
		(nrankedcommon==1 && istwopair && (pokerval&15)>((pokerval>>16)&15) )
	)
	{
		strength=3;
	}

	// strength 2
	else if
	(
		//Board has Pair and you have Underpair
		(nrankedcommon==2 && istwopair && ispair && (pokervalcommon<twopair) && $$pr0<trankcommon ) ||

		//Board has Pair and you have Two pairs w/ good kicker (A-T)
		(nrankedcommon==2 && istwopair && !ispair && (pokervalcommon<twopair) && (pokerval&15)>=10 )
	)
	{
		strength=2;
	}

	// strength 1
	else if
	(
		//Board has two pair and you have only two pair
		(nrankedcommon==2 && istwopair && (pokervalcommon&twopair) ) ||

		//Board has Pair and you have Two pairs w/ poor kicker (9-2)
		(nrankedcommon==2 && istwopair && !ispair && (pokervalcommon<twopair) && (pokerval&15)<=9 ) ||

		//Catch all
		(istwopair)
	)
	{
		strength=1;
	}

	// TwoPairDiscount
	if (istwopair)
	{
		if (nsuitedcommon>=3)
			strength-=1;

		if (nsuitedcommon>=4)
			strength-=1;

		if (nstraightfillcommon<=1 || nstraightcommon>=4)
			strength-=1;
	}

	return strength;
}

const int CMyHand::StrengthOnePair(void)
{
	int		strength = 0;

	bool	isonepair = (bool) p_symbols->sym()->isonepair;
	bool	ispair = (bool) p_symbols->sym()->ispair;
	bool	islopair = (bool) p_symbols->sym()->islopair;
	bool	ismidpair = (bool) p_symbols->sym()->ismidpair;
	bool	ishipair = (bool) p_symbols->sym()->ishipair;
	int		nrankedcommon = (int) p_symbols->sym()->nrankedcommon;
	int		nsuitedcommon = (int) p_symbols->sym()->nsuitedcommon;
	int		twopair = (int) p_symbols->sym()->twopair;
	int		pokerval = (int) p_symbols->sym()->pokerval;
	int		trank = (int) p_symbols->sym()->trank;
	int		trankcommon = (int) p_symbols->sym()->trankcommon;
	int		rankhicommon = (int) p_symbols->sym()->rankhicommon;
	int		rankhiplayer = (int) p_symbols->sym()->rankhiplayer;
	int		rankloplayer = (int) p_symbols->sym()->rankloplayer;
	int		$$pr0 = (int) p_symbols->sym()->$$pr[0];
	int		$$pr1 = (int) p_symbols->sym()->$$pr[1];
	int		nstraightcommon = (int) p_symbols->sym()->nstraightcommon;
	int		nstraightfillcommon = (int) p_symbols->sym()->nstraightfillcommon;

	if (!isonepair)
	{
		strength=0;
	}

	// strength 5
	else if
	(
		//Board has No Pair and you have Overpair 10's or better
		(nrankedcommon==1 && ispair && pokerval<twopair && trank>trankcommon && rankloplayer>9) ||

		//Board has No Pair and you have Top pair Ace w/ good kicker (K-T)
		(nrankedcommon==1 && ishipair && pokerval<twopair && trank==14 &&
		 ((trank==$$pr0 && $$pr1>=10) ||
		  (trank==$$pr1 && $$pr0>=10)) ) ||

		//Board has No Pair and you have Top pair King w/ good kicker (A-T)
		(nrankedcommon==1 && ishipair && pokerval<twopair && trank==13 &&
		 ((trank==$$pr0 && $$pr1>=10) ||
		  (trank==$$pr1 && $$pr0>=10)) )
	)
	{
		strength=5;
	}

	// strength 4
	else if
	(
		//Board has No Pair and you have Overpair
		(nrankedcommon==1 && ispair && pokerval<twopair && trank>trankcommon ) ||

		//Board has No Pair and you have Top pair Queen or lower w/ good kicker (A-T)
		(nrankedcommon==1 && ishipair && pokerval<twopair && trank<=12 &&
		 ((trank==$$pr0 && $$pr1>=10) ||
		  (trank==$$pr1 && $$pr0>=10)) ) ||

		//Board has No Pair and you have Top pair Ace w/ poor kicker (9-2)
		(nrankedcommon==1 && ishipair && pokerval<twopair && trank==14 &&
		 ((trank==$$pr0 && $$pr1<=9) ||
		  (trank==$$pr1 && $$pr0<=9)) )
	)
	{
		strength=4;
	}

	// strength 3
	else if
	(
		//Board has No Pair and you have Midpair 1 overcard
		(nrankedcommon==1 && ispair && pokerval<twopair &&
		 ((pokerval>>8)&15)>trank && ((pokerval>>4)&15)<trank ) ||

		//Board has No Pair and you have Top pair King w/ poor kicker (9-2)
		(nrankedcommon==1 && ishipair && pokerval<twopair && trank==13 &&
		 ((trank==$$pr0 && $$pr1<=9) ||
		  (trank==$$pr1 && $$pr0<=9)) ) ||

		//Board has No Pair and you have Top pair Queen or lower w/ poor kicker (9-2)
		(nrankedcommon==1 && ishipair && pokerval<twopair && trank<=12 &&
		 ((trank==$$pr0 && $$pr1<=9) ||
		  (trank==$$pr1 && $$pr0<=9)) ) ||

		//Board has No Pair and you have Middle pair w/ overcard kicker
		(nrankedcommon==1 && ismidpair && pokerval<twopair && !ispair &&
		 ((trank==$$pr0 && $$pr1>trankcommon) ||
		  (trank==$$pr1 && $$pr0>trankcommon)) )
	)
	{
		strength=3;
	}

	// strength 2
	else if
	(
		//Board has No Pair and you have Midpair 2 overcard
		(nrankedcommon==1 && ispair && pokerval<twopair &&
		 ((pokerval>>8)&15)>trank && ((pokerval>>4)&15)>trank ) ||

		//Board has No Pair and you have Bottom pair w/ overcard kicker
		(nrankedcommon==1 && islopair && pokerval<twopair && !ispair &&
		 ((trank==$$pr0 && $$pr1>trankcommon) ||
		  (trank==$$pr1 && $$pr0>trankcommon)) ) ||

		//Board has No Pair and you have Underpair
		(nrankedcommon==1 && ispair && pokerval<twopair && trank<(pokerval&15) ) ||

		//Board has No Pair and you have Middle pair w/ poor kicker
		(nrankedcommon==1 && ismidpair && pokerval<twopair && !ispair &&
		 ((trank==$$pr0 && $$pr1<trankcommon) ||
		  (trank==$$pr1 && $$pr0<trankcommon)) )
	)
	{
		strength=2;
	}

	// strength 1
	else if
	(
		//Board has Pair and you have "Two overcards w/Ace"
		(nrankedcommon==2 && rankloplayer>rankhicommon && rankhiplayer==14 && rankloplayer>=10 && !ispair ) ||

		//Board has Pair and you have "Two overcards w/ King"
		(nrankedcommon==2 && rankloplayer>rankhicommon && rankhiplayer==13 && rankloplayer>=10 && !ispair ) ||

		//Board has No Pair and you have Bottom pair w/ poor kicker
		(nrankedcommon==1 && islopair && pokerval<twopair && !ispair &&
		 ((trank==$$pr0 && $$pr1<trankcommon) ||
		  (trank==$$pr1 && $$pr0<trankcommon)) ) ||

		//Board has Pair and you have Two overcards
		(nrankedcommon==2 && rankloplayer>rankhicommon ) ||
		//Board has Pair and you have "One overcard w/Ace (A9-A2)"

		(nrankedcommon==2 && rankhiplayer==14 && pokerval<twopair && rankloplayer<=9 && rankloplayer<rankhicommon ) ||

		//Board has Pair and you have "One overcard w/ King (K9-K2)"
		(nrankedcommon==2 && rankhiplayer==13 && pokerval<twopair && rankloplayer<=9 && rankloplayer<rankhicommon ) ||

		//Board has Pair and you have Two small cards
		(nrankedcommon==2 && !ispair && pokerval<twopair && rankhiplayer<=9 )
	)
	{
		strength=1;
	}

	// OnePairDiscount
	if (isonepair)
	{
		if (nsuitedcommon>=3)
			strength-=1;

		if (nsuitedcommon>=4)
			strength-=1;

		if (nstraightfillcommon<=1 || nstraightcommon>=4)
			strength-=1;
	}

	if(strength < 0)
		strength = 0;

	return strength;
}
