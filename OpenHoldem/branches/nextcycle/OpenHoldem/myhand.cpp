#include "stdafx.h"

#include "myhand.h"
#include "symbols.h"
#include "debug.h"

double	MyHand::process_query(const char * pquery, int *e)
{
#ifdef SEH_ENABLE
	try {
#endif
		if (memcmp(pquery,"mh_3straight", 12) == 0)			return three_straight((pquery[12]-'0')!=0, (pquery[13]-'0')!=0);
		if (memcmp(pquery,"mh_bottomsd", 11) == 0)			return bottomsd();
		if (memcmp(pquery,"mh_nsuitedbetter", 16) == 0)		return nsuitedbetter();
		if (memcmp(pquery,"mh_kickerbetter", 15) == 0)		return kickerbetter();
		if (memcmp(pquery,"mh_kickerrank", 13) == 0)		return kickerrank();
		if (memcmp(pquery,"mh_nouts", 9) == 0)				return m_nouts();
		if (memcmp(pquery,"mh_str_strflush", 15) == 0)		return strength_straightflush();
		if (memcmp(pquery,"mh_str_quads", 12) == 0)			return strength_quads();
		if (memcmp(pquery,"mh_str_fullhouse", 16) == 0)		return strength_fullhouse();
		if (memcmp(pquery,"mh_str_flush", 12) == 0)			return strength_flush();
		if (memcmp(pquery,"mh_str_straight", 15) == 0)		return strength_straight();
		if (memcmp(pquery,"mh_str_trips", 12) == 0)			return strength_trips();
		if (memcmp(pquery,"mh_str_twopair", 14) == 0)		return strength_twopair();
		if (memcmp(pquery,"mh_str_onepair", 14) == 0)		return strength_onepair();

		*e = ERR_INVALID_SYM;
		return 0.0;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::process_query\n"); 
		throw; 
	}
#endif
}

void MyHand::myhand(char * hand) 
{
#ifdef SEH_ENABLE
	try {
#endif
		switch ((int) symbols.sym.$$pr[0] >= (int) symbols.sym.$$pr[1] ? (int) symbols.sym.$$pr[0] : (int) symbols.sym.$$pr[1]) 
		{
			case 10: hand[0]='T'; break;
			case 11: hand[0]='J'; break;
			case 12: hand[0]='Q'; break;
			case 13: hand[0]='K'; break;
			case 14: hand[0]='A'; break;
			default: hand[0]='0'+(int) symbols.sym.$$pr[0]; break;
		}

		switch ((int) symbols.sym.$$pr[0] >= (int) symbols.sym.$$pr[1] ? (int) symbols.sym.$$pr[1] : (int) symbols.sym.$$pr[0])
		{
			case 10: hand[1]='T'; break;
			case 11: hand[1]='J'; break;
			case 12: hand[1]='Q'; break;
			case 13: hand[1]='K'; break;
			case 14: hand[1]='A'; break;
			default: hand[1]='0'+(int) symbols.sym.$$pr[1]; break;
		}

		if (symbols.sym.issuited) 
		{
			hand[2]='s';
			hand[3]='\0';
		} 
		else 
		{
			hand[2]=' ';
			hand[3]='\0';
		}

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::myhand\n"); 
		throw; 
	}
#endif
}

bool MyHand::three_straight (bool wheel, bool broadway) 
{
#ifdef SEH_ENABLE
	try {
#endif
		if (bitcount(((int) symbols.sym.rankbitscommon)&0x003e)>=3)
		{
			if (wheel)
				return true;
			else
				return false;
		}

		if (bitcount(((int) symbols.sym.rankbitscommon)&0x7c00)>=3) 
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

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::three_straight\n"); 
		throw; 
	}
#endif
}

bool MyHand::bottomsd (void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		if (symbols.sym.nstraight==4 && 
			symbols.sym.nstraightfillcommon-symbols.sym.nstraightfill==1 &&
			( 
			 ( (((int) symbols.sym.rankbits)>>((int) symbols.sym.rankloplayer)&0x1f)==0xf && 
			   !(symbols.sym.rankhiplayer==symbols.sym.$$cr[0] || 
			     symbols.sym.rankhiplayer==symbols.sym.$$cr[1] || 
				 symbols.sym.rankhiplayer==symbols.sym.$$cr[2] || 
				 symbols.sym.rankhiplayer==symbols.sym.$$cr[3] || 
				 symbols.sym.rankhiplayer==symbols.sym.$$cr[4])
			 ) 
			 || 
			 ( (((int) symbols.sym.rankbits)>>((int) symbols.sym.rankhiplayer)&0x1f)==0xf && 
			   !(symbols.sym.rankloplayer==symbols.sym.$$cr[0] || 
				 symbols.sym.rankloplayer==symbols.sym.$$cr[1] || 
				 symbols.sym.rankloplayer==symbols.sym.$$cr[2] || 
				 symbols.sym.rankloplayer==symbols.sym.$$cr[3] || 
				 symbols.sym.rankloplayer==symbols.sym.$$cr[4])
			 ) 
			)
		   ) return true;

		return false;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::bottomsd\n"); 
		throw; 
	}
#endif
}

int MyHand::nsuitedbetter (void) 
{
#ifdef SEH_ENABLE
	try {
#endif

		return bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer));

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::nsuitedbetter\n"); 
		throw; 
	}
#endif
}

int MyHand::kickerbetter (void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		// quads, board quads
		if (symbols.sym.isfourofakind && symbols.sym.nrankedcommon==4) 
		{
			return bitcount((~((int) symbols.sym.rankbits)&0x7ffe)>>((int) symbols.sym.rankhiplayer));
		}

		// trips, board trips
		else if (symbols.sym.isthreeofakind && symbols.sym.nrankedcommon==3) 
		{
			return bitcount((~((int) symbols.sym.rankbits)&0x7ffe)>>((int) symbols.sym.rankhiplayer));
		}

		// trips, board paired
		else if (symbols.sym.isthreeofakind && symbols.sym.nrankedcommon==2) 
		{
			if (symbols.sym.trank==symbols.sym.rankhiplayer)
				return bitcount((~((int) symbols.sym.rankbits)&0x7ffe)>>((int) symbols.sym.rankloplayer));

			else 
				return bitcount((~((int) symbols.sym.rankbits)&0x7ffe)>>((int) symbols.sym.rankhiplayer));
		}

		// trips, board not paired
		// in this case, both hole cards participate in the set, thus kicker is shared
		else if (symbols.sym.isthreeofakind && symbols.sym.nrankedcommon==1) 
		{
			return 0;
		}

		// two pair, board has two pair
		else if (symbols.sym.istwopair && (((int) symbols.sym.pokervalcommon)&((int) symbols.sym.twopair))) 
		{
			return bitcount((~((int) symbols.sym.rankbits)&0x7ffe)>>((int) symbols.sym.rankhiplayer));
		}

		// two pair, board paired
		else if (symbols.sym.istwopair && !(((int) symbols.sym.pokervalcommon)&((int) symbols.sym.twopair)) && symbols.sym.nrankedcommon==2) 
		{
			if (((int) symbols.sym.pcbits)&1)  
			{ 
				if (((int) symbols.sym.$$pr[0])==(((int) symbols.sym.pokerval)&0xf))
					return bitcount((~((int) symbols.sym.rankbits)&0x7ffe)>>((int) symbols.sym.$$pr[0]));

				if (((int) symbols.sym.$$pr[1])==(((int) symbols.sym.pokerval)&0xf))
					return bitcount((~((int) symbols.sym.rankbits)&0x7ffe)>>((int) symbols.sym.$$pr[1]));
			}
			else  
			{ 
				return 0; 
			}
		}

		// two pair, board not paired
		// in this case, both hole cards participate in the two pair, thus kicker is shared
		else if (symbols.sym.istwopair && symbols.sym.nrankedcommon==1) 
		{
			return 0;
		}

		// one pair, board paired
		else if (symbols.sym.isonepair && symbols.sym.nrankedcommon==2) 
		{
			return bitcount((~((int) symbols.sym.rankbits)&0x7ffe)>>((int) symbols.sym.rankhiplayer));
		}

		// one pair, board not paired
		else if (symbols.sym.npcbits==2 && !symbols.sym.ispair && symbols.sym.isonepair && symbols.sym.nrankedcommon==1) 
		{
			if (symbols.sym.trank==symbols.sym.rankhiplayer)
				return bitcount((~((int) symbols.sym.rankbits)&0x7ffe)>>((int) symbols.sym.rankloplayer));

			else
				return bitcount((~((int) symbols.sym.rankbits)&0x7ffe)>>((int) symbols.sym.rankhiplayer));
		}

		// hi card
		else if (symbols.sym.ishicard) 
		{
			return bitcount((~((int) symbols.sym.rankbits)&0x7ffe)>>((int) symbols.sym.rankhiplayer));
		}

		return 0;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::kickerbetter\n"); 
		throw; 
	}
#endif
}
 
int MyHand::kickerrank (void) 
{
#ifdef SEH_ENABLE
	try {
#endif

		// quads, board quads
		if (symbols.sym.isfourofakind && symbols.sym.nrankedcommon==4) 
		{
			return ((int) symbols.sym.pokerval&0xf)>((int) symbols.sym.pokervalcommon&0xf) ? 
				(int) symbols.sym.pokerval&0xf : 
				0;
		}

		// trips, board trips
		else if (symbols.sym.isthreeofakind && symbols.sym.nrankedcommon==3) 
		{
			return (((int) symbols.sym.pokerval>>4)&0xf)>(((int) symbols.sym.pokervalcommon>>4)&0xf) ? 
				((int) symbols.sym.pokerval>>4)&0xf : 
				0;
		}

		// trips, board paired
		else if (symbols.sym.isthreeofakind && symbols.sym.nrankedcommon==2) 
		{
			return symbols.sym.trank==symbols.sym.rankhiplayer ? 
				symbols.sym.rankloplayer : 
				symbols.sym.rankhiplayer;
		}

		// two pair, board has two pair
		else if (symbols.sym.istwopair && (((int) symbols.sym.pokervalcommon)&((int) symbols.sym.twopair))) 
		{
			return symbols.sym.rankhiplayer>((int) symbols.sym.pokervalcommon&0xf) ? 
				symbols.sym.rankhiplayer : 
				0;
		}

		// one pair, board paired
		else if (symbols.sym.isonepair && symbols.sym.nrankedcommon==2) 
		{
			return symbols.sym.rankhiplayer>(((int) symbols.sym.pokervalcommon>>8)&0xf) ? 
				symbols.sym.rankhiplayer : 
				0;
		}

		// one pair, board not paired, not pocket pair
		else if (symbols.sym.isonepair && symbols.sym.nrankedcommon==1 && !symbols.sym.ispair) 
		{
			if (symbols.sym.trank==symbols.sym.rankhiplayer)
				return symbols.sym.rankloplayer==(((int) symbols.sym.pokerval>>8)&0xf) ? 
				symbols.sym.rankloplayer : 
				0;

			else
				return symbols.sym.rankhiplayer==(((int) symbols.sym.pokerval>>8)&0xf) ? 
				symbols.sym.rankhiplayer : 
				0;
		}

		// hi card
		else if (symbols.sym.ishicard) 
		{
			if (symbols.sym.npcbits==0) 
				return 0;

			if (symbols.sym.rankhiplayer!=((((int) symbols.sym.pokerval)>>16)&0xf))  
				return symbols.sym.rankhiplayer;

			else if (symbols.sym.npcbits==2) 
				return symbols.sym.rankloplayer;

			else 
				return 0;
		}

		return 0;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::kickerrank\n"); 
		throw; 
	}
#endif
}

double MyHand::m_nouts(void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		double m_nouts=0.0;

		if (symbols.sym.br == 2) 
		{
			//FLUSH OUTS 
			if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==2 && 
				bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer))<=4) 
			{
				m_nouts+=9;
			}

			else if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==2 && 
				bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer))>4)  
			{
				m_nouts+=8; 
			}
			
			else if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==3 && 
				bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer))<=1)  
			{ 
				m_nouts+=9; 
			}
			
			else if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==3 && 
				bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer))>1 && 
				bitcount(((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer)))<4)  
			{ 
				m_nouts+=4; 
			}
			
			if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==2 && 
				((int) symbols.sym.pokerval)&((int) symbols.sym.onepair) && symbols.sym.nrankedcommon==1)  
			{ 
				m_nouts+=4; 
			}
			
			if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==3 && 
				((int) symbols.sym.pokerval)&((int) symbols.sym.onepair) && symbols.sym.nrankedcommon==1 && 
				bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer))<=1)  
			{ 
				m_nouts+=4; 
			}

			//STRAIGHT OUTS 
			if (symbols.sym.nstraight==4 && symbols.sym.nstraightfillcommon>1 && 
				(((int) symbols.sym.rankbits)&16398)!=16398 && (((int) symbols.sym.rankbits)&28674)!=28674)  
			{ 
				m_nouts+=8; 
			}
			
			else if ((((int) symbols.sym.rankbits)>>((int) symbols.sym.ranklo))==0x5d)  
			{ 
				m_nouts+=9; 
			}
			
			else if (symbols.sym.nstraight==4 && symbols.sym.nstraightcommon==3 && 
				symbols.sym.rankloplayer>symbols.sym.rankhicommon && 
				symbols.sym.rankhiplayer-symbols.sym.rankloplayer<=2 && 
				(((int) symbols.sym.rankbits)&16398)!=16398 && (((int) symbols.sym.rankbits)&28674)!=28674)  
			{ 
				m_nouts+=8; 
			}
			
			else if (symbols.sym.nstraightfill==1 && symbols.sym.nstraightfillcommon>=2)  
			{ 
				m_nouts+=4; 
			}

			//OVERCARD OUTS 
			if (symbols.sym.rankloplayer>symbols.sym.rankhicommon && symbols.sym.rankloplayer>=11 && 
				symbols.sym.nopponentsplaying==1 && symbols.sym.nsuitedcommon<=2 && 
				symbols.sym.nsuited>symbols.sym.nsuitedcommon && symbols.sym.nstraightfillcommon>1 && 
				!symbols.sym.ispair)  
			{ 
				m_nouts+=4; 
			}
			
			if (symbols.sym.rankloplayer>symbols.sym.rankhicommon && symbols.sym.rankloplayer>=12 && 
				!(symbols.sym.nsuitedcommon<=2 && symbols.sym.nsuited==symbols.sym.nsuitedcommon) && 
				symbols.sym.nstraightfillcommon>2)  
			{ 
				m_nouts+=4.5; 
			}
			
			if (symbols.sym.nsuitedcommon<=2 && symbols.sym.rankhiplayer>symbols.sym.rankhicommon && 
				symbols.sym.rankhiplayer>11 && symbols.sym.rankloplayer>9 && symbols.sym.nstraightfillcommon>=2)  
			{ 
				m_nouts+=2; 
			}
			
			else if (symbols.sym.nsuited==3 && symbols.sym.nsuitedcommon==2 && 
				symbols.sym.rankloplayer>symbols.sym.rankhicommon && symbols.sym.nstraightfillcommon>=2)  
			{ 
				m_nouts+=1.5; 
			}
			
			else if (symbols.sym.nsuited==3 && symbols.sym.nsuitedcommon==2 && 
				symbols.sym.rankhiplayer>symbols.sym.rankhicommon && 
				symbols.sym.rankloplayer>symbols.sym.rankhicommon && symbols.sym.nstraightfillcommon>=2)  
			{ 
				m_nouts+=1.5; 
			}
			
			else if (symbols.sym.nsuited==3 && symbols.sym.nsuited>symbols.sym.nsuitedcommon && 
				symbols.sym.nstraightfillcommon>2 && symbols.sym.rankloplayer>symbols.sym.rankhicommon && 
				symbols.sym.rankloplayer>=12)  
			{ 
				m_nouts+=2; 
			}

			//BACKDOOR OUTS 
			if (symbols.sym.issuited && symbols.sym.nsuited==3)  
			{ 
				m_nouts+=1.5; 
			}
			
			if (symbols.sym.nstraight==3 && symbols.sym.nstraightcommon==1)  
			{ 
				m_nouts+=1.5; 
			}
			
			if (symbols.sym.nstraight==2 && symbols.sym.nstraightfill==2 && symbols.sym.nstraightcommon==1)  
			{ 
				m_nouts+=1; 
			}
			
			if (symbols.sym.nstraight==1 && symbols.sym.nstraightfill==2 && symbols.sym.nstraightcommon==1)  
			{ 
				m_nouts+=0.5; 
			} 
			
			if (symbols.sym.nstraightfillcommon>=2 && symbols.sym.nsuitedcommon==1 && symbols.sym.nranked==2 && 
				symbols.sym.nrankedcommon==1 && symbols.sym.rankhiplayer==14)  
			{ 
				m_nouts+=5; 
			} 

			//DISCOUNT OUTS FOR DANGEROUS BOARDS 
			if (symbols.sym.nrankedcommon==2)  
			{ 
				m_nouts-=3.5; 
			}
			
			else if (symbols.sym.nrankedcommon==3)  
			{ 
				m_nouts-=10; 
			} 
			
			if (symbols.sym.rankloplayer>symbols.sym.rankhicommon && symbols.sym.rankloplayer>=12 && 
				( (symbols.sym.nsuitedcommon>1 && symbols.sym.nsuited<4) || 
				  (symbols.sym.nstraightcommon>1 && symbols.sym.nstraight<4) ) )
			{ 
				m_nouts-=symbols.sym.nsuitedcommon+symbols.sym.nstraightcommon; 
			}
			
			if ( (symbols.sym.nstraight==4 && symbols.sym.nstraightfillcommon>2 && 
				   (((int) symbols.sym.rankbits)&16398)!=16398 && (((int) symbols.sym.rankbits&28674))!=28674) ||
				 (symbols.sym.nstraightfill==1 && symbols.sym.nstraightfillcommon>=2) || 
				   ((((int) symbols.sym.rankbits)>>((int) symbols.sym.ranklo))==0x5d) )
			{
				if (symbols.sym.nsuitedcommon==2)  
				{ 
					m_nouts-=2; 
				}
				
				else if (symbols.sym.nsuitedcommon==3 && symbols.sym.nsuited>symbols.sym.nsuitedcommon && 
					bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer))<=2)  
				{ 
					m_nouts-=2; 
				}
				
				else if (symbols.sym.nsuitedcommon==3)  
				{ 
					m_nouts-=8; 
				}
			}
		}

		else if (symbols.sym.br == 3) 
		{
			//FLUSH OUTS 
			if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==2 && 
				bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer))<=4)  
			{ 
				m_nouts+=9; 
			}
			
			else if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==2 && 
				bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer))>4)  
			{ 
				m_nouts+=8; 
			}
			
			else if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==3 && 
				bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer))<=1)  
			{ 
				m_nouts+=9; 
			}
			
			else if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==3 && 
				bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer))>1 && 
				bitcount(((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer)))<4)  
			{ 
				m_nouts+=3; 
			}
			
			if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==2 && 
				((int) symbols.sym.pokerval)&((int) symbols.sym.onepair) && symbols.sym.nrankedcommon==1)  
			{ 
				m_nouts+=4; 
			}
			
			if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==3 && 
				((int) symbols.sym.pokerval)&((int) symbols.sym.onepair) && symbols.sym.nrankedcommon==1 && 
				bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer))<=1)  
			{ 
				m_nouts+=4; 
			}
			
			//STRAIGHT OUTS 
			if (symbols.sym.nstraight==4 && symbols.sym.nstraightfillcommon>1 && 
				(((int) symbols.sym.rankbits)&16398)!=16398 && (((int) symbols.sym.rankbits)&28674)!=28674)  
			{ 
				m_nouts+=8; 
			}
			
			else if ((((int) symbols.sym.rankbits)>>((int) symbols.sym.ranklo))==0x5d)  
			{ 
				m_nouts+=9; 
			}
			
			else if (symbols.sym.nstraight==4 && symbols.sym.nstraightcommon==3 && 
				symbols.sym.rankloplayer>symbols.sym.rankhicommon && 
				symbols.sym.rankhiplayer-symbols.sym.rankloplayer<=2 && 
				(((int) symbols.sym.rankbits)&16398)!=16398 && (((int) symbols.sym.rankbits)&28674)!=28674)  
			{ 
				m_nouts+=8; 
			}
			
			else if (symbols.sym.nstraightfill==1 && symbols.sym.nstraightfillcommon>=2)  
			{ 
				m_nouts+=4; 
			}
			
			//OVERCARD OUTS 
			if (symbols.sym.rankloplayer>symbols.sym.rankhicommon && symbols.sym.rankloplayer>=12 && 
				symbols.sym.nsuitedcommon<=2 && symbols.sym.nsuited>symbols.sym.nsuitedcommon && 
				symbols.sym.nstraightfillcommon>2)  
			{
				m_nouts+=3; 
			}
			
			if (symbols.sym.nsuitedcommon<=2 && symbols.sym.rankhiplayer>symbols.sym.rankhicommon && 
				symbols.sym.rankhiplayer>11 && symbols.sym.rankloplayer>9 && symbols.sym.nstraightfillcommon>=2)  
			{ 
				m_nouts+=2; 
			}
			
			if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==2 && 
				symbols.sym.rankloplayer>symbols.sym.rankhicommon && symbols.sym.nstraightfillcommon>=2)  
			{ 
				m_nouts+=2; 
			}
			
			else if (symbols.sym.nsuited==4 && symbols.sym.nsuitedcommon==2 && 
				symbols.sym.rankhiplayer>symbols.sym.rankhicommon && 
				symbols.sym.rankloplayer<symbols.sym.rankhicommon && symbols.sym.nstraightfillcommon>=2)  
			{ 
				m_nouts+=1; 
			}
			
			//DISCOUNT OUTS FOR DANGEROUS BOARDS 
			if (((int) symbols.sym.pokervalcommon)&((int) symbols.sym.twopair))  
			{ 
				m_nouts-=12; 
			} 
			
			else if (symbols.sym.nrankedcommon==2 && 
				(symbols.sym.$$cr[0]==symbols.sym.$$cr[3] || 
				 symbols.sym.$$cr[1]==symbols.sym.$$cr[3] || 
				 symbols.sym.$$cr[2]==symbols.sym.$$cr[3]))  
			{ 
				m_nouts-=1; 
			}
			
			else if (symbols.sym.nrankedcommon==2)  
			{ 
				m_nouts-=3; 
			}
			
			else if (symbols.sym.nrankedcommon==3)  
			{ 
				m_nouts-=12; 
			} 
			
			if (symbols.sym.rankloplayer>symbols.sym.rankhicommon && symbols.sym.rankloplayer>=12 && 
				( (symbols.sym.nsuitedcommon>1 && symbols.sym.nsuited<4) || 
				  (symbols.sym.nstraightcommon>1 && symbols.sym.nstraight<4) ) )
				
			{ 
				m_nouts-=symbols.sym.nsuitedcommon+symbols.sym.nstraightcommon; 
			} 
			
			if ( (symbols.sym.nstraight==4 && symbols.sym.nstraightfillcommon>2 && 
				 (((int) symbols.sym.rankbits)&16398)!=16398 && (((int) symbols.sym.rankbits)&28674)!=28674) ||
				   (symbols.sym.nstraightfill==1 && symbols.sym.nstraightfillcommon>=2) || 
				 ((((int) symbols.sym.rankbits)>>((int) symbols.sym.ranklo))==0x5d) )		
			{
				if (symbols.sym.nsuitedcommon==2 && 
					(symbols.sym.$$cs[0]==symbols.sym.$$cs[3] || 
					 symbols.sym.$$cs[1]==symbols.sym.$$cs[3] || 
					 symbols.sym.$$cs[2]==symbols.sym.$$cs[3]))  
				{ 
					m_nouts-=0; 
				}
				
				else if (symbols.sym.nsuitedcommon==2)  
				{ 
					m_nouts-=2; 
				}
				
				else if (symbols.sym.nsuitedcommon==3 && symbols.sym.nsuited>symbols.sym.nsuitedcommon && 
					bitcount((~((int) symbols.sym.srankbits)&0x7ffe)>>((int) symbols.sym.srankhiplayer))<=2)  
				{ 
					m_nouts-=2; 
				}
				
				else if (symbols.sym.nsuitedcommon==3)  
				{ 
					m_nouts-=8; 
				}
			}
		}

		if (m_nouts <= 0.0)
			m_nouts = 0.0;

		return m_nouts;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::nouts\n"); 
		throw; 
	}
#endif
}

bool MyHand::safeboard(void)
{
#ifdef SEH_ENABLE
	try {
#endif

		if (symbols.sym.nrankedcommon<=1 && symbols.sym.nsuitedcommon<=2 && symbols.sym.nstraightcommon<=2 && symbols.sym.nstraightfillcommon>=2)
			return true;
		
		else
			return false;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::safeboard\n"); 
		throw; 
	}
#endif
}

int MyHand::strength_straightflush(void)
{
#ifdef SEH_ENABLE
	try {
#endif

		int		strength=0;

		if (!symbols.sym.isstraightflush) 
		{ strength=0; }

		// strength 5
		else if
		(
			// nuts
			(symbols.sym.isroyalflush && !((int) symbols.sym.pokervalcommon&(int) symbols.sym.royalflush)) ||
			(symbols.sym.isstraightflush && symbols.sym.nhandshi==0 && !((int) symbols.sym.pokervalcommon&(int) symbols.sym.straightflush))
		)  
		{ strength=5; }

		// strength 4
		else if
		(
			0
		)  
		{ strength=4; }
		
		// strength 3
		else if
		(
			// I may not have the highest, board does not have a str8flush
			(symbols.sym.isstraightflush && !((int) symbols.sym.pokervalcommon&(int) symbols.sym.straightflush))
		)  
		{ strength=3; }
		
		// strength 2
		else if
		(
			// I may not have the highest, board does have a str8flush
			(symbols.sym.isstraightflush && ((int) symbols.sym.pokervalcommon&(int) symbols.sym.straightflush)
			&& !((int) symbols.sym.pokervalcommon&(int) symbols.sym.royalflush))
		)  
		{ strength=2; }
		
		// strength 1
		else if
		(
			// Chop pot
			(symbols.sym.isroyalflush && ((int) symbols.sym.pokervalcommon&(int) symbols.sym.royalflush))
		)  
		{ strength=1; }

		return strength;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::strength_straightflush\n"); 
		throw; 
	}
#endif
}

int MyHand::strength_quads(void)
{
#ifdef SEH_ENABLE
	try {
#endif

		int		strength=0;

		if (!symbols.sym.isfourofakind) 
		{ strength=0; }

		// strength 5
		else if
		(
			// nuts
			(symbols.sym.isfourofakind && symbols.sym.nhandshi==0 && !((int) symbols.sym.pokervalcommon&(int) symbols.sym.fourofakind)) ||
			// Board quads, I have an Ace
			(symbols.sym.isfourofakind && ((int) symbols.sym.pokervalcommon&(int) symbols.sym.fourofakind) && symbols.IsHand("Ax") )
		)  
		{ strength=5; }
		
		// strength 4
		else if
		(
			0
		)  
		{ strength=4; }
		
		// strength 3
		else if
		(
			// I may not have the highest, board does not have quads
			(symbols.sym.isfourofakind && !((int) symbols.sym.pokervalcommon&(int) symbols.sym.fourofakind))
		)  
		{ strength=3; }
		
		// strength 2
		else if
		(
			// Board quads, I have a King
			(symbols.sym.isfourofakind && ((int) symbols.sym.pokervalcommon&(int) symbols.sym.fourofakind) && symbols.IsHand("Kx") )
		)  
		{ strength=2; }
		
		// strength 1
		else if
		(
			// Board has quads, I don't have best kicker
			(symbols.sym.isfourofakind && ((int) symbols.sym.pokervalcommon&(int) symbols.sym.fourofakind))
		)  
		{ strength=1; }

		return strength;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::strength_quads\n"); 
		throw; 
	}
#endif
}

int MyHand::strength_fullhouse(void)
{
#ifdef SEH_ENABLE
	try {
#endif

		int			strength=0;

		if (!symbols.sym.isfullhouse) 
		{ strength=0; }

		// strength 5
		else if 
		( 
			// nuts
			// Board paired, I have pocket pair and top side full house, pocket pair is higher than non-participating board cards  
			// e.g. QQ/QTT53
			(symbols.sym.isfullhouse && symbols.sym.ispair && symbols.sym.nrankedcommon==2 && 
			 !((int) symbols.sym.pokervalcommon&(int) symbols.sym.twopair) && 
			 (((int) symbols.sym.pokerval>>8)&15)>((int) symbols.sym.pokerval&15) && ((int) symbols.sym.pcbits&0x1c) && 
			 symbols.sym.trank==(((int) symbols.sym.pokervalcommon>>8)&0xf)) ||

			// Board paired, I have pocket pair and top side full house, pocket pair is lower than non-participating board cards  
			// e.g. QQ/QTTA3
			(symbols.sym.isfullhouse && symbols.sym.ispair && symbols.sym.nrankedcommon==2 && 
			 !((int) symbols.sym.pokervalcommon&(int) symbols.sym.twopair) &&
			 (((int) symbols.sym.pokerval>>8)&15)>((int) symbols.sym.pokerval&15) && ((int) symbols.sym.pcbits&0x1c) && 
			 symbols.sym.trank<(((int) symbols.sym.pokervalcommon>>8)&0xf)) ||

			// Board paired, I have pocket pair and bottom side full house, pocket pair is higher than non-participating board cards
			// e.g. QQ/QKKT6
			(symbols.sym.isfullhouse && symbols.sym.ispair && symbols.sym.nrankedcommon==2 && 
			 !((int) symbols.sym.pokervalcommon&(int) symbols.sym.twopair) &&
			 (((int) symbols.sym.pokerval>>8)&15)<((int) symbols.sym.pokerval&15) && ((int) symbols.sym.pcbits&0x1c) &&
			 symbols.sym.trank==(((int) symbols.sym.pokervalcommon>>8)&0xf) ) ||

			// Board paired, I have pocket pair and bottom side full house, pocket pair is lower than non-participating board cards
			// e.g. QQ/QKKA6
			(symbols.sym.isfullhouse && symbols.sym.ispair && symbols.sym.nrankedcommon==2 && 
			 !((int) symbols.sym.pokervalcommon&(int) symbols.sym.twopair) &&
			 (((int) symbols.sym.pokerval>>8)&15)<((int) symbols.sym.pokerval&15) && ((int) symbols.sym.pcbits&0x1c) &&
			 symbols.sym.trank<(((int) symbols.sym.pokervalcommon>>8)&0xf))
		)  
		{ strength=5; }

		// strength 4
		else if
		(
			// Board paired, I don't have pocket pair, I have top side full house, pair is higher than non-participating board cards
			// e.g. QJ/QQJ53
			(symbols.sym.isfullhouse && !symbols.sym.ispair && symbols.sym.nrankedcommon==2 && 
			 !((int) symbols.sym.pokervalcommon&(int) symbols.sym.twopair) &&
			 (((int) symbols.sym.pokerval>>8)&15)>((int) symbols.sym.pokerval&15) && 
			  ((int) symbols.sym.pokerval&0xf)==(((int) symbols.sym.pokervalcommon>>8)&0xf)) ||
			
			// Board paired, I don't have pocket pair, I have top side full house, pair is lower than non-participating board cards  
			// e.g. QJ/QQJA3
			(symbols.sym.isfullhouse && !symbols.sym.ispair && symbols.sym.nrankedcommon==2 && 
			 !((int) symbols.sym.pokervalcommon&(int) symbols.sym.twopair) &&
			 (((int) symbols.sym.pokerval>>8)&15)>((int) symbols.sym.pokerval&15) && 
			  ((int) symbols.sym.pokerval&0xf)<(((int) symbols.sym.pokervalcommon>>8)&0xf)) ||
			
			// Board paired, I don't have pocket pair, I have bottom side full house, pair is higher than non-participating board cards
			// e.g. QJ/QJJ53
			(symbols.sym.isfullhouse && !symbols.sym.ispair && symbols.sym.nrankedcommon==2 && 
			 !((int) symbols.sym.pokervalcommon&(int) symbols.sym.twopair) &&
			 (((int) symbols.sym.pokerval>>8)&15)<((int) symbols.sym.pokerval&15) && 
			  ((int) symbols.sym.pokerval&0xf)==(((int) symbols.sym.pokervalcommon>>8)&0xf)) ||
			
			// Board paired, I don't have pocket pair, I have bottom side full house, pair is lower than non-participating board cards
			// e.g. QJ/QJJ53
			(symbols.sym.isfullhouse && !symbols.sym.ispair && symbols.sym.nrankedcommon==2 && 
			 !((int) symbols.sym.pokervalcommon&(int) symbols.sym.twopair) &&
			 (((int) symbols.sym.pokerval>>8)&15)<((int) symbols.sym.pokerval&15) && 
			  ((int) symbols.sym.pokerval&0xf)<(((int) symbols.sym.pokervalcommon>>8)&0xf)) ||
			
			// Board has two pair, I have top trips 
			// e.g. QT/QQJJ8 
			(symbols.sym.isfullhouse && ((int) symbols.sym.pokervalcommon&(int) symbols.sym.twopair) && 
			( (((int) symbols.sym.pokervalcommon>>16)&0x0f)==symbols.sym.$$pr[0] || 
			  (((int) symbols.sym.pokervalcommon>>16)&0x0f)==symbols.sym.$$pr[1] ))
		)  
		{ strength=4; }

		// strength 3
		else if
		(
			0
		)  
		{ strength=3; }

		// strength 2
		else if
		(
			// Board trips, I have pocket pair higher than non-participating board cards
			// e.g. QQ/JJJ92
			(symbols.sym.isfullhouse && symbols.sym.ispair && symbols.sym.nrankedcommon==3 && ((int) symbols.sym.pcbits&0x03) && 
			symbols.sym.trank>(((int) symbols.sym.pokervalcommon>>4)&0xf)) ||
			
			// Board trips, I have pocket pair lower than non-participating board cards
			// e.g. QQ/JJJK2
			(symbols.sym.isfullhouse && symbols.sym.ispair && symbols.sym.nrankedcommon==3 && ((int) symbols.sym.pcbits&0x03) &&
			symbols.sym.trank<(((int) symbols.sym.pokervalcommon>>4)&0xf)) ||
			
			// Board trips, only one of my hole cards participates, the pair rank is higher than non-participating board cards
			// e.g. QT/999T8
			(symbols.sym.isfullhouse && symbols.sym.nrankedcommon==3 && bitcount((int) symbols.sym.pcbits&0x03)==1 && 
			((int) symbols.sym.pokerval&0xf)==(((int) symbols.sym.pokervalcommon>>4)&0xf)) ||
			
			// Board trips, only one of my hole cards participates, the pair rank is lower than non-participating board cards
			// e.g. QT/999TK
			(symbols.sym.isfullhouse && symbols.sym.nrankedcommon==3 && bitcount((int) symbols.sym.pcbits&0x03)==1 &&
			((int) symbols.sym.pokerval&0xf)<(((int) symbols.sym.pokervalcommon>>4)&0xf))
		)  
		{ strength=2; }

		// strength 1
		else if
		(
			// Board has full house
			(symbols.sym.isfullhouse && ((int) symbols.sym.pokervalcommon&(int) symbols.sym.fullhouse))
		)  
		{ strength=1; }

		return strength;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::strength_fullhouse\n"); 
		throw; 
	}
#endif
}

int MyHand::strength_flush(void)
{
#ifdef SEH_ENABLE
	try {
#endif

		int		strength=0;

		if (!symbols.sym.isflush) 
		{ strength=0; }

		// strength 5
		else if	
		(
			// nuts
			// Big raise
			(symbols.sym.ishiflush && 
			 (!((int) symbols.sym.pokervalcommon&(int) symbols.sym.flush) || symbols.sym.pokerval>symbols.sym.pokervalcommon) && 
				 symbols.sym.nrankedcommon<=2 && symbols.sym.nstraightflushfillcommon>=3 )
		)	
		{ strength=5; }
		
		// strength 4
		else if
		(
			// Medium raise
			// one card can beat my flush
			(symbols.sym.isflush && 
			 bitcount((~(int) symbols.sym.srankbits&0x7fff)>>(int) symbols.sym.srankhiplayer)<=1 && 
			 symbols.sym.nrankedcommon<=2 && symbols.sym.nsuitedcommon<=4) || 
			
			(symbols.sym.ishiflush && (!((int) symbols.sym.pokervalcommon&(int) symbols.sym.flush) || 
			 symbols.sym.pokerval>symbols.sym.pokervalcommon) && symbols.sym.nrankedcommon<=2 && 
			 symbols.sym.nstraightflushfillcommon>=2)
		)  
		{ strength=4; }

		// strength 3
		else if
		(
			// Small raise
			// two or cards can beat my flush, board not paired
			(symbols.sym.isflush && bitcount((~(int) symbols.sym.srankbits&0x7fff)>>(int) symbols.sym.srankhiplayer)<=2 && 
			 symbols.sym.nrankedcommon==1 && symbols.sym.nsuitedcommon<=4) ||

			// three of more cards can beat my flush, board may be paired, flush came on turn
			(symbols.sym.isflush && bitcount((~(int) symbols.sym.srankbits&0x7fff)>>(int) symbols.sym.srankhiplayer)>=3 && 
			symbols.sym.nrankedcommon<=2 && symbols.sym.nsuitedcommon==3 && symbols.sym.$$cs[3]==symbols.sym.tsuitcommon)
		)  
		{ strength=3; }

		// strength 2
		else if
		(
			// Call
			// three or less cards can beat my flush
			(symbols.sym.isflush && bitcount((~(int) symbols.sym.srankbits&0x7fff)>>(int) symbols.sym.srankhiplayer)<=3 && 
			 symbols.sym.nrankedcommon<=2 && symbols.sym.nsuitedcommon<=4) ||

			// two or less cards can beat my flush, board paired
			(symbols.sym.isflush && bitcount((~(int) symbols.sym.srankbits&0x7fff)>>(int) symbols.sym.srankhiplayer)<=2 && 
			 symbols.sym.nrankedcommon==2 && symbols.sym.nsuitedcommon<=4)
		)  
		{ strength=2; }

		// strength 1
		else if
		(
			// Don't call these
			// four or more cards can beat my flush
			(symbols.sym.isflush && bitcount((~(int) symbols.sym.srankbits&0x7fff)>>(int) symbols.sym.srankhiplayer)>=4 && 
			 symbols.sym.nsuitedcommon<=4) ||
			
			// board has trips or more
			(symbols.sym.isflush && symbols.sym.nrankedcommon>=3) ||
			
			// board has flush, my cards don't help
			(symbols.sym.isflush && symbols.sym.pokerval<symbols.sym.pokervalcommon && 
			 ((int) symbols.sym.pokervalcommon&(int) symbols.sym.flush))
		)  
		{ strength=1; }

		// FlushDiscount
		if (symbols.sym.isflush)
		{
			if ((int) symbols.sym.pokervalcommon&(int) symbols.sym.twopair)  
				strength-=2;
		}

		return strength;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::strength_flush\n"); 
		throw; 
	}
#endif
}

int MyHand::strength_straight(void)
{
#ifdef SEH_ENABLE
	try {
#endif

		int		strength=0;

		if (!symbols.sym.isstraight) 
		{ strength=0; }

		// strength 5
		else if	
		(
			//Board 3 to a straight, cards in rank 1 and rank 2 (high end)
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==2 && 
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>16)&0xf)) && (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>12)&0xf)) ) || 
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>16)&0xf)) && (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>12)&0xf)) ) ) ) ||
			
			 //Board 3 to a straight, cards in rank 1 and rank 3
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==2 && 
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>16)&0xf)) && (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>8)&0xf)) ) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>16)&0xf)) && (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>8)&0xf)) ) ) ) ||
			
			//Board 3 to a straight, cards in rank 1 and rank 4
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==2 &&
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>16)&0xf)) && (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>4)&0xf)) ) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>16)&0xf)) && (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>4)&0xf)) ) ) ) ||
			
			//Board 4 to a straight, card used in rank 1, nut straight with an ace
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==1 &&
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>16)&0xf)) && (symbols.sym.$$pr[0]==14)) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>16)&0xf)) && (symbols.sym.$$pr[1]==14)) ) ) ||

			//Board 4 to a straight, card used in rank 2, nut straight with a king
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==1 &&
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>12)&0xf)) && (symbols.sym.$$pr[0]==13)) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>12)&0xf)) && (symbols.sym.$$pr[1]==13)) ) ) ||

			//Board 4 to a straight, card used in rank 3, nut straight with a queen
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==1 &&
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>8)&0xf)) && (symbols.sym.$$pr[0]==12)) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>8)&0xf)) && (symbols.sym.$$pr[1]==12)) ) ) ||

			//Board 4 to a straight, card used in rank 4, nut straight with a jack
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==1 &&
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>4)&0xf)) && (symbols.sym.$$pr[0]==11)) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>4)&0xf)) && (symbols.sym.$$pr[1]==11)) ) ) ||

			//Board 4 to a straight, card used in rank 5, nut straight with a ten
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==1 &&
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>0)&0xf)) && (symbols.sym.$$pr[0]==10)) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>0)&0xf)) && (symbols.sym.$$pr[1]==10)) ) ) ||

			//Board 3 to a straight, nut straights
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==2 && symbols.sym.rankloplayer>=10 )
		)  
		{ strength=5; }

		// strength 4
		else if
		(
			//Board 3 to a straight, cards in rank 1 and rank 5
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==2 &&
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>16)&0xf)) && (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>0)&0xf))) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>16)&0xf)) && (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>0)&0xf))) ) ) ||

			//Board 3 to a straight, cards in rank 2 and rank 3
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==2 &&
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>12)&0xf)) && (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>8)&0xf))) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>12)&0xf)) && (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>4)&0xf))) ) ) ||
			
			//Board 3 to a straight, cards in rank 2 and rank 4
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==2 &&
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>12)&0xf)) && (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>4)&0xf))) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>12)&0xf)) && (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>4)&0xf))) ) )
		)  
		{ strength=4; }

		// strength 3
		else if
		(
			//Board 3 to a straight, cards in rank 2 and rank 5
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==2 &&
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>12)&0xf)) && (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>0)&0xf))) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>12)&0xf)) && (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>0)&0xf))) ) ) ||
			
			//Board 3 to a straight, cards in rank 3 and rank 4
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==2 &&
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>8)&0xf)) && (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>4)&0xf))) || 
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>8)&0xf)) && (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>4)&0xf))) ) ) ||
			
			//Board 3 to a straight, cards in rank 3 and rank 5
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==2 && 
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>8)&0xf)) && (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>0)&0xf))) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>8)&0xf)) && (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>0)&0xf))) ) ) ||
			
			//Board 3 to a straight, cards in rank 4 and rank 5 (idiot end)
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==2 &&
			( ( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>4)&0xf)) && (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>0)&0xf))) ||
			  ( (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>4)&0xf)) && (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>0)&0xf))) ) ) ||
			
			//Board 4 to a straight, card used in rank 1
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==1 &&
			( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>16)&0xf)) || (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>16)&0xf)) ) ) ||
			
			//Board 4 to a straight, card used in rank 2
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==1 &&
			( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>12)&0xf)) || (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>12)&0xf)) ) )
		)  
		{ strength=3; }

		// strength 2
		else if 
		(
			//Board 4 to a straight, card used in rank 3
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==1 &&
			( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>8)&0xf)) || (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>8)&0xf)) ) ) ||
			
			//Board 4 to a straight, card used in rank 4
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==1 &&
			( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>4)&0xf)) || (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>4)&0xf)) ) )
		)  
		{ strength=2; }

		// strength 1
		else if
		(
			//Board 4 to a straight, card used in rank 5
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==1 &&
			( (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>0)&0xf)) || (symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>0)&0xf)) ) ) ||
			
			//Board has straight
			(symbols.sym.isstraight && symbols.sym.nstraightfillcommon==0)
		)  
		{ strength=1; }

		// StraightDiscount
		if (symbols.sym.isstraight)
		{
			if (symbols.sym.nsuitedcommon==3)  
				strength-=1;

			if (symbols.sym.nsuitedcommon>=4)  
				strength-=2;

			if (symbols.sym.nrankedcommon>=3)
				strength-=1;

			if ((int) symbols.sym.pokervalcommon&(int) symbols.sym.twopair)
				strength-=2;

			// Negate flush discount if we have a good flush draw 
			if (symbols.sym.nsuitedcommon==3 && symbols.sym.nsuited==4 && 
				bitcount((~(int) symbols.sym.srankbits&0x7fff)>>(int) symbols.sym.srankhiplayer)<=2 )
				strength+=1;
		}
		return strength;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::strength_straight\n"); 
		throw; 
	}
#endif
}

int MyHand::strength_trips(void)
{
#ifdef SEH_ENABLE
	try {
#endif

		int		strength=0;

		if (!symbols.sym.isthreeofakind) 
		{ strength=0; }

		// strength 5
		else if	
		(
			//Board has No Pair and you have top set
			(symbols.sym.nrankedcommon==1 && symbols.sym.ispair && symbols.sym.isthreeofakind && 
			 symbols.sym.rankhiplayer==symbols.sym.rankhicommon)
		)  
		{ strength=5; }
		
		// strength 4
		else if	
		(
			//Board has No Pair and you have middle set
			(symbols.sym.nrankedcommon==1 && symbols.sym.ispair && symbols.sym.isthreeofakind && 
			symbols.sym.rankhiplayer<symbols.sym.rankhicommon && 
			symbols.sym.rankhiplayer>symbols.sym.ranklocommon ) ||

			//Board has Pair and you have Trips w/ good kicker (used as 4th ranked card)
			(symbols.sym.nrankedcommon==2 && symbols.sym.isthreeofakind &&
			  ((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>4)&0xf)) ||
			   (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>4)&0xf)) ) )
		)  
		{ strength=4; }
		
		// strength 3
		else if	
		(
			//Board has Pair and you have Trips w/ moderate kicker (used as 5th ranked card)
			(symbols.sym.nrankedcommon==2 && symbols.sym.isthreeofakind &&
			  ((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>0)&0xf)) ||
			   (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>0)&0xf)) ) ) ||

			//Board has No Pair and you have bottom set
			(symbols.sym.nrankedcommon==1 && symbols.sym.ispair && symbols.sym.isthreeofakind && 
			 symbols.sym.rankhiplayer==symbols.sym.ranklocommon ) ||

			//Board not paired catch all
			(symbols.sym.isthreeofakind && symbols.sym.nrankedcommon==1 )
		)  
		{ strength=3; }
		
		// strength 2
		else if	
		(
			//Board has Pair and you have Trips w/ poor kicker (not used)
			(symbols.sym.nrankedcommon==2 && symbols.sym.isthreeofakind &&
			  ((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]<(((int) symbols.sym.pokerval>>0)&0xf)) ||
			   (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]<(((int) symbols.sym.pokerval>>0)&0xf)) ) ) ||

			//Board has 3OAK good kickers
			(symbols.sym.isthreeofakind && symbols.sym.nrankedcommon==3 && symbols.sym.rankhiplayer>=13 && 
			 symbols.sym.rankloplayer>=10 ) ||

			//Board has pair catch all
			(symbols.sym.isthreeofakind && symbols.sym.nrankedcommon==2 )
		)  
		{ strength=2; }
		
		// strength 1
		else if	
		(
			//Board has 3OAK catch all
			(symbols.sym.isthreeofakind && symbols.sym.nrankedcommon==3 )
		)  
		{ strength=1; }

		// TripsDiscount
		if (symbols.sym.isthreeofakind)
		{
			if (symbols.sym.nsuitedcommon>=3)  
				strength-=1;

			if (symbols.sym.nsuitedcommon>=4)  
				strength-=1;

			if (symbols.sym.nstraightfillcommon<=1 || symbols.sym.nstraightcommon>=4)  
				strength-=1;
		}

		return strength;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::strength_trips\n"); 
		throw; 
	}
#endif
}

int MyHand::strength_twopair(void)
{
#ifdef SEH_ENABLE
	try {
#endif

		int		strength=0;

	
		if (!symbols.sym.istwopair) 
		{ strength=0; }
	
		// strength 5
		else if	
		(
			 //Board has No Pair and you have Top two pairs
			(symbols.sym.nrankedcommon==1 && symbols.sym.istwopair && 
			 ((int) symbols.sym.pokerval&15)<(((int) symbols.sym.pokerval>>4)&15) ) 
		)  
		{ strength=5; }

		// strength 4
		else if	
		(
			//Board has No Pair and you have Top and bottom pair
			(symbols.sym.nrankedcommon==1 && symbols.sym.istwopair && 
			 ((int) symbols.sym.pokerval&15)<(((int) symbols.sym.pokerval>>16)&15) && 
			 ((int) symbols.sym.pokerval&15)>(((int) symbols.sym.pokerval>>4)&15) ) || 

			//Board has No Pair and you have Middle two pairs
			(symbols.sym.nrankedcommon==1 && symbols.sym.istwopair && 
			 ((int) symbols.sym.pokerval&15)>(((int) symbols.sym.pokerval>>8)&15) &&
			 ((int) symbols.sym.pokerval&15)>(((int) symbols.sym.pokerval>>4)&15) && 
			 !(symbols.sym.rankloplayer==symbols.sym.ranklocommon) ) ||
			
			//Rainbow board
			(symbols.sym.istwopair && safeboard() )
		)  
		{ strength=4; }

		// strength 3
		else if	
		(
			//Board has Pair and you have Overpair
			(symbols.sym.nrankedcommon==2 && symbols.sym.istwopair && 
			 (symbols.sym.$$pr[0]==(((int) symbols.sym.pokerval>>16)&0xf) || 
			  symbols.sym.$$pr[1]==(((int) symbols.sym.pokerval>>16)&0xf)) ) ||
			 (symbols.sym.nrankedcommon==2 && symbols.sym.istwopair && symbols.sym.ispair && 
			  symbols.sym.$$pr[0]>symbols.sym.trankcommon) ||

			//Board has No Pair and you have Bottom two pairs
			(symbols.sym.nrankedcommon==1 && symbols.sym.istwopair && 
			 ((int) symbols.sym.pokerval&15)>(((int) symbols.sym.pokerval>>16)&15) )
		)  
		{ strength=3; }

		// strength 2
		else if	
		(
			//Board has Pair and you have Underpair
			(symbols.sym.nrankedcommon==2 && symbols.sym.istwopair && symbols.sym.ispair && 
			 (symbols.sym.pokervalcommon < symbols.sym.twopair) && 
			 symbols.sym.$$pr[0]<symbols.sym.trankcommon ) ||

			//Board has Pair and you have Two pairs w/ good kicker (A-T)
			(symbols.sym.nrankedcommon==2 && (symbols.sym.pokervalcommon < symbols.sym.twopair) && 
			 symbols.sym.istwopair && !symbols.sym.ispair && 
			 ((int) symbols.sym.pokerval&15)>=10 ) 
		)  
		{ strength=2; }

		// strength 1
		else if	
		(
			//Board has two pair and you have only two pair
			(symbols.sym.nrankedcommon==2 && symbols.sym.istwopair && 
			 ((int) symbols.sym.pokervalcommon&(int) symbols.sym.twopair) ) ||

			//Board has Pair and you have Two pairs w/ poor kicker (9-2)
			(symbols.sym.nrankedcommon==2 && (symbols.sym.pokervalcommon < symbols.sym.twopair) && symbols.sym.istwopair && 
			!symbols.sym.ispair && ((int) symbols.sym.pokerval&15)<=9 ) ||

			//Catch all
			(symbols.sym.istwopair )
		)  
		{ strength=1; }

		// TwoPairDiscount
		if (symbols.sym.istwopair)
		{
			if (symbols.sym.nsuitedcommon>=3)  
				strength-=1;

			if (symbols.sym.nsuitedcommon>=4)  
				strength-=1;

			if (symbols.sym.nstraightfillcommon<=1 || symbols.sym.nstraightcommon>=4)  
				strength-=1;
		}

		return strength;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::strength_twopair\n"); 
		throw; 
	}
#endif
}

int MyHand::strength_onepair(void)
{
#ifdef SEH_ENABLE
	try {
#endif

		int		strength=0;

		if (!symbols.sym.isonepair) 
		{ strength=0; }

		// strength 5
		else if	
		(
			//Board has No Pair and you have Overpair 10's or better
			(symbols.sym.nrankedcommon==1 && symbols.sym.ispair && symbols.sym.pokerval<symbols.sym.twopair && 
			symbols.sym.trank>symbols.sym.trankcommon && symbols.sym.rankloplayer>9) ||
			
			//Board has No Pair and you have Top pair Ace w/ good kicker (K-T) 
			(symbols.sym.nrankedcommon==1 && symbols.sym.ishipair && symbols.sym.pokerval<symbols.sym.twopair && symbols.sym.trank==14 && 
			((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]>=10) || 
			 (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]>=10)) ) ||
			
			 //Board has No Pair and you have Top pair King w/ good kicker (A-T) 
			(symbols.sym.nrankedcommon==1 && symbols.sym.ishipair && symbols.sym.pokerval<symbols.sym.twopair && symbols.sym.trank==13 && 
			((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]>=10) ||
			 (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]>=10)) )
		)  
		{ strength=5; }

		// strength 4
		else if	
		(
			//Board has No Pair and you have Overpair 
			(symbols.sym.nrankedcommon==1 && symbols.sym.ispair && symbols.sym.pokerval<symbols.sym.twopair && 
			symbols.sym.trank>symbols.sym.trankcommon ) ||
			
			//Board has No Pair and you have Top pair Queen or lower w/ good kicker (A-T) 
			(symbols.sym.nrankedcommon==1 && symbols.sym.ishipair && symbols.sym.pokerval<symbols.sym.twopair && symbols.sym.trank<=12 && 
			((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]>=10) ||
			 (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]>=10)) ) ||
			
			 //Board has No Pair and you have Top pair Ace w/ poor kicker (9-2) 
			(symbols.sym.nrankedcommon==1 && symbols.sym.ishipair && symbols.sym.pokerval<symbols.sym.twopair && symbols.sym.trank==14 && 
			((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]<=9) ||
			 (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]<=9)) )
		)  
		{ strength=4; }

		// strength 3
		else if	
		(
			//Board has No Pair and you have Midpair 1 overcard 
			(symbols.sym.nrankedcommon==1 && symbols.sym.ispair && symbols.sym.pokerval<symbols.sym.twopair && 
			(((int) symbols.sym.pokerval>>8)&15)>symbols.sym.trank && (((int) symbols.sym.pokerval>>4)&15)<symbols.sym.trank ) ||
			
			//Board has No Pair and you have Top pair King w/ poor kicker (9-2) 
			(symbols.sym.nrankedcommon==1 && symbols.sym.ishipair && symbols.sym.pokerval<symbols.sym.twopair && symbols.sym.trank==13 && 
			((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]<=9) ||
			 (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]<=9)) ) ||
			
			 //Board has No Pair and you have Top pair Queen or lower w/ poor kicker (9-2) 
			(symbols.sym.nrankedcommon==1 && symbols.sym.ishipair && symbols.sym.pokerval<symbols.sym.twopair && symbols.sym.trank<=12 && 
			((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]<=9) ||
			 (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]<=9)) ) ||
			
			 //Board has No Pair and you have Middle pair w/ overcard kicker 
			(symbols.sym.nrankedcommon==1 && symbols.sym.ismidpair && symbols.sym.pokerval<symbols.sym.twopair && !symbols.sym.ispair && 
			((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]>symbols.sym.trankcommon) ||
			 (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]>symbols.sym.trankcommon)) ) 
		)  
		{ strength=3; }

		// strength 2
		else if	
		(
			//Board has No Pair and you have Midpair 2 overcard 
			(symbols.sym.nrankedcommon==1 && symbols.sym.ispair && symbols.sym.pokerval<symbols.sym.twopair && 
			(((int) symbols.sym.pokerval>>8)&15)>symbols.sym.trank && (((int) symbols.sym.pokerval>>4)&15)>symbols.sym.trank ) ||
			
			//Board has No Pair and you have Bottom pair w/ overcard kicker 
			(symbols.sym.nrankedcommon==1 && symbols.sym.islopair && symbols.sym.pokerval<symbols.sym.twopair && !symbols.sym.ispair && 
			((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]>symbols.sym.trankcommon) ||
			 (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]>symbols.sym.trankcommon)) ) ||
			
			 //Board has No Pair and you have Underpair 
			(symbols.sym.nrankedcommon==1 && symbols.sym.ispair && symbols.sym.pokerval<symbols.sym.twopair && 
			symbols.sym.trank<((int) symbols.sym.pokerval&15) ) ||
			
			//Board has No Pair and you have Middle pair w/ poor kicker 
			(symbols.sym.nrankedcommon==1 && symbols.sym.ismidpair && symbols.sym.pokerval<symbols.sym.twopair && !symbols.sym.ispair && 
			((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]<symbols.sym.trankcommon) ||
			 (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]<symbols.sym.trankcommon)) )
		)  
		{ strength=2; }

		// strength 1
		else if	
		(
			//Board has Pair and you have "Two overcards w/Ace" 
			(symbols.sym.nrankedcommon==2 && symbols.sym.rankloplayer>symbols.sym.rankhicommon && symbols.sym.rankhiplayer==14 && 
			symbols.sym.rankloplayer>=10 && !symbols.sym.ispair ) ||
			
			//Board has Pair and you have "Two overcards w/ King" 
			(symbols.sym.nrankedcommon==2 && symbols.sym.rankloplayer>symbols.sym.rankhicommon && symbols.sym.rankhiplayer==13 && 
			symbols.sym.rankloplayer>=10 && !symbols.sym.ispair ) ||
			
			//Board has No Pair and you have Bottom pair w/ poor kicker 
			(symbols.sym.nrankedcommon==1 && symbols.sym.islopair && symbols.sym.pokerval<symbols.sym.twopair && !symbols.sym.ispair && 
			((symbols.sym.trank==symbols.sym.$$pr[0] && symbols.sym.$$pr[1]<symbols.sym.trankcommon) ||
			 (symbols.sym.trank==symbols.sym.$$pr[1] && symbols.sym.$$pr[0]<symbols.sym.trankcommon)) ) ||
			
			 //Board has Pair and you have Two overcards
			(symbols.sym.nrankedcommon==2 && symbols.sym.rankloplayer>symbols.sym.rankhicommon ) ||
			//Board has Pair and you have "One overcard w/Ace (A9-A2)" 
			
			(symbols.sym.nrankedcommon==2 && symbols.sym.rankhiplayer==14 && symbols.sym.pokerval<symbols.sym.twopair && 
			symbols.sym.rankloplayer<=9 && symbols.sym.rankloplayer<symbols.sym.rankhicommon ) ||
			
			//Board has Pair and you have "One overcard w/ King (K9-K2)" 
			(symbols.sym.nrankedcommon==2 && symbols.sym.rankhiplayer==13 && symbols.sym.pokerval<symbols.sym.twopair && 
			symbols.sym.rankloplayer<=9 && symbols.sym.rankloplayer<symbols.sym.rankhicommon ) ||
			
			//Board has Pair and you have Two small cards 
			(symbols.sym.nrankedcommon==2 && !symbols.sym.ispair && symbols.sym.pokerval<symbols.sym.twopair && symbols.sym.rankhiplayer<=9 )
		)  
		{ strength=1; }

		// OnePairDiscount
		if (symbols.sym.isonepair)
		{
			if (symbols.sym.nsuitedcommon>=3)  
				strength-=1;

			if (symbols.sym.nsuitedcommon>=4)  
				strength-=1;

			if (symbols.sym.nstraightfillcommon<=1 || symbols.sym.nstraightcommon>=4)   
				strength-=1;
		}

		return strength;

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("MyHand::strength_onepair\n"); 
		throw; 
	}
#endif
}
