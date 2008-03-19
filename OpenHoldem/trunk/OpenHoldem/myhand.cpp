#include "stdafx.h"

/*
#include "myhand.h"
#include "wh_symbols.h"
#include "debug.h"

double	MyHand::process_query(const char * pquery) {
	try {
		if (memcmp(pquery,"dll$mh_3straight", 16) == 0) { return three_straight((pquery[16]-'0')!=0, (pquery[17]-'0')!=0); } 
		else if (memcmp(pquery,"dll$mh_bottomsd", 15) == 0) { return bottomsd(); } 
		else if (memcmp(pquery,"dll$mh_nsuitedbetter", 20) == 0) { return nsuitedbetter(); } 
		else if (memcmp(pquery,"dll$mh_kickerbetter", 19) == 0) { return kickerbetter(); } 
		else if (memcmp(pquery,"dll$mh_kickerrank", 17) == 0) { return kickerrank(); } 
		else if (memcmp(pquery,"dll$mh_nouts", 13) == 0) { return m_nouts(); } 
		return 0.0;
	}
	catch (...) { 
		logfatal("MyHand::process_query\n"); 
		throw; 
	}
}

void MyHand::myhand(char * hand) {
	try {
		switch (pr0) {
			case 10: hand[0]='T'; break;
			case 11: hand[0]='J'; break;
			case 12: hand[0]='Q'; break;
			case 13: hand[0]='K'; break;
			case 14: hand[0]='A'; break;
			default: hand[0]='0'+pr0; break;
		}
		switch (pr1) {
			case 10: hand[1]='T'; break;
			case 11: hand[1]='J'; break;
			case 12: hand[1]='Q'; break;
			case 13: hand[1]='K'; break;
			case 14: hand[1]='A'; break;
			default: hand[1]='0'+pr1; break;
		}
		if (issuited) {
			hand[2]='s';
			hand[3]='\0';
		} else {
			hand[2]=' ';
			hand[3]='\0';
		}
	}
	catch (...) { 
		logfatal("MyHand::myhand\n"); 
		throw; 
	}
}

bool MyHand::three_straight (bool wheel, bool broadway) {
	try {
		if (nstraightcommon==3) { return true; } 

		if (wheel && bitcount((rankbitscommon)&0x003c)>=3 && !((rankbitscommon)&0x4002)) { return true; } 

		if (broadway && bitcount((rankbitscommon)&0x7c00)>=3)  { return true; } 

		return false ;
	}
	catch (...) { 
		logfatal("MyHand::three_straight\n"); 
		throw; 
	}
}

bool MyHand::bottomsd (void) {
	try {
		if (nstraight==4 && 
			nstraightfillcommon-nstraightfill==1 &&
			( 
			 ( ((rankbits)>>(rankloplayer)&0x1f)==0xf && 
			   !(rankhiplayer==cr0 || 
			     rankhiplayer==cr1 || 
				 rankhiplayer==cr2 || 
				 rankhiplayer==cr3 || 
				 rankhiplayer==cr4)
			 ) 
			 || 
			 ( ((rankbits)>>(rankhiplayer)&0x1f)==0xf && 
			   !(rankloplayer==cr0 || 
				 rankloplayer==cr1 || 
				 rankloplayer==cr2 || 
				 rankloplayer==cr3 || 
				 rankloplayer==cr4)
			 ) 
			)
		   ) return true;
		return false;
	}
	catch (...) { 
		logfatal("MyHand::bottomsd\n"); 
		throw; 
	}
}

int MyHand::nsuitedbetter (void) {
	try {
		return bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer));
	}
	catch (...) { 
		logfatal("MyHand::nsuitedbetter\n"); 
		throw; 
	}
}

int MyHand::kickerbetter (void) {
	try {
		// quads, board quads
		if (isfourofakind && nrankedcommon==4) {
			return bitcount((~(rankbits)&0x7ffe)>>(rankhiplayer))+1;
		}

		// trips, board trips
		else if (isthreeofakind && nrankedcommon==3) {
			return bitcount((~(rankbits)&0x7ffe)>>(rankhiplayer))+1;
		}

		// trips, board paired
		else if (isthreeofakind && nrankedcommon==2) {
			if (trank==rankhiplayer)  { return bitcount((~(rankbits)&0x7ffe)>>(rankloplayer))+1; }
			else  { return bitcount((~(rankbits)&0x7ffe)>>(rankhiplayer))+1; }
		}

		// trips, board not paired
		// in this case, both hole cards participate in the set, thus kicker is shared
		else if (isthreeofakind && nrankedcommon==1) {
			return 0;
		}

		// two pair, board has two pair
		else if (istwopair && ((pokervalcommon)&(twopair))) {
			return bitcount((~(rankbits)&0x7ffe)>>(rankhiplayer))+1;
		}

		// two pair, board paired
		else if (istwopair && !((pokervalcommon)&(twopair)) && nrankedcommon==2) {
			if ((pcbits)&1)  { 
				if ((pr0)==((pokerval)&0xf)) { return bitcount((~(rankbits)&0x7ffe)>>(pr0))+1; }
				if ((pr1)==((pokerval)&0xf)) { return bitcount((~(rankbits)&0x7ffe)>>(pr1))+1; }
			}
			else  { 
				return 0; 
			}
		}

		// two pair, board not paired
		// in this case, both hole cards participate in the two pair, thus kicker is shared
		else if (istwopair && nrankedcommon==1) {
			return 0;
		}

		// one pair, board paired
		else if (isonepair && nrankedcommon==2) {
			return bitcount((~(rankbits)&0x7ffe)>>(rankhiplayer))+1;
		}

		// one pair, board not paired
		else if (npcbits==2 && !ispair && isonepair && nrankedcommon==1) {
			if (trank==rankhiplayer)  { return bitcount((~(rankbits)&0x7ffe)>>(rankloplayer))+1; }
			else  { return bitcount((~(rankbits)&0x7ffe)>>(rankhiplayer))+1; }
		}

		// hi card
		else if (ishicard) {
			return bitcount((~(rankbits)&0x7ffe)>>(rankhiplayer))+1;
		}

		return 0;
	}
	catch (...) { 
		logfatal("MyHand::kickerbetter\n"); 
		throw; 
	}
}
 
int MyHand::kickerrank (void) {
	try {

		// quads, board quads
		if (isfourofakind && nrankedcommon==4) {
			return (pokerval&0xf)>(pokervalcommon&0xf) ? pokerval&0xf : 0;
		}

		// trips, board trips
		else if (isthreeofakind && nrankedcommon==3) {
			return ((pokerval>>4)&0xf)>((pokervalcommon>>4)&0xf) ? (pokerval>>4)&0xf : 0;
		}

		// trips, board paired
		else if (isthreeofakind && nrankedcommon==2) {
			return trank==rankhiplayer ? rankloplayer : rankhiplayer;
		}

		// two pair, board has two pair
		else if (istwopair && ((pokervalcommon)&(twopair))) {
			return rankhiplayer>(pokervalcommon&0xf) ? rankhiplayer : 0;
		}

		// one pair, board paired
		else if (isonepair && nrankedcommon==2) {
			return rankhiplayer>((pokervalcommon>>8)&0xf) ? rankhiplayer : 0;
		}

		// one pair, board not paired, not pocket pair
		else if (isonepair && nrankedcommon==1 && !ispair) {
			if (trank==rankhiplayer) { 
				return rankloplayer==((pokerval>>8)&0xf) ? rankloplayer : 0;
			}
			else {
				return rankhiplayer==((pokerval>>8)&0xf) ? rankhiplayer : 0;
			}
		}

		// hi card
		else if (ishicard) {
			if (npcbits==0) return 0;
			if (rankhiplayer!=(((pokerval)>>16)&0xf))  return rankhiplayer;
			else if (npcbits==2) return rankloplayer;
			else return 0;
		}

		return 0;
	}
	catch (...) { 
		logfatal("MyHand::kickerrank\n"); 
		throw; 
	}
}

double MyHand::m_nouts(void) {
	double m_nouts=0.0;

	try {
		if (br == 2) {
			//FLUSH OUTS 
			if (nsuited==4 && nsuitedcommon==2 && bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer))<=4)  { m_nouts+=9; }
			else if (nsuited==4 && nsuitedcommon==2 && bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer))>4)  { m_nouts+=8; }
			else if (nsuited==4 && nsuitedcommon==3 && bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer))<=1)  { m_nouts+=9; }
			else if (nsuited==4 && nsuitedcommon==3 && bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer))>1 && 
				bitcount(((~(srankbits)&0x7ffe)>>(srankhiplayer)))<4)  { m_nouts+=4; }
			if (nsuited==4 && nsuitedcommon==2 && (pokerval)&(onepair) && nrankedcommon==1)  { m_nouts+=4; }
			if (nsuited==4 && nsuitedcommon==3 && (pokerval)&(onepair) && nrankedcommon==1 && 
				bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer))<=1)  { m_nouts+=4; }
			//STRAIGHT OUTS 
			if (nstraight==4 && nstraightfillcommon>1 && ((rankbits)&16398)!=16398 && ((rankbits)&28674)!=28674)  { m_nouts+=8; }
			else if (((rankbits)>>(ranklo))==0x5d)  { m_nouts+=9; }
			else if (nstraight==4 && nstraightcommon==3 && rankloplayer>rankhicommon && 
				rankhiplayer-rankloplayer<=2 && ((rankbits)&16398)!=16398 && ((rankbits)&28674)!=28674)  { m_nouts+=8; }
			else if (nstraightfill==1 && nstraightfillcommon>=2)  { m_nouts+=4; }
			//OVERCARD OUTS 
			if (rankloplayer>rankhicommon && rankloplayer>=11 && nopponentsplaying==1 && 
				nsuitedcommon<=2 && nsuited>nsuitedcommon && nstraightfillcommon>1 && 
				!ispair)  { m_nouts+=4; }
			if (rankloplayer>rankhicommon && rankloplayer>=12 && 
				!(nsuitedcommon<=2 && nsuited==nsuitedcommon) && nstraightfillcommon>2)  { m_nouts+=4.5; }
			if (nsuitedcommon<=2 && rankhiplayer>rankhicommon && rankhiplayer>11 && 
				rankloplayer>9 && nstraightfillcommon>=2)  { m_nouts+=2; }
			else if (nsuited==3 && nsuitedcommon==2 && rankloplayer>rankhicommon && 
				nstraightfillcommon>=2)  { m_nouts+=1.5; }
			else if (nsuited==3 && nsuitedcommon==2 && rankhiplayer>rankhicommon && 
				rankloplayer>rankhicommon && nstraightfillcommon>=2)  { m_nouts+=1.5; }
			else if (nsuited==3 && nsuited>nsuitedcommon && nstraightfillcommon>2 && 
				rankloplayer>rankhicommon && rankloplayer>=12)  { m_nouts+=2; }
			//BACKDOOR OUTS 
			if (issuited && nsuited==3)  { m_nouts+=1.5; }
			if (nstraight==3 && nstraightcommon==1)  { m_nouts+=1.5; }
			if (nstraight==2 && nstraightfill==2 && nstraightcommon==1)  { m_nouts+=1; }
			if (nstraight==1 && nstraightfill==2 && nstraightcommon==1)  { m_nouts+=0.5; } 
			if (nstraightfillcommon>=2 && nsuitedcommon==1 && nranked==2 && nrankedcommon==1 && 
				rankhiplayer==14)  { m_nouts+=5; } 
			//DISCOUNT OUTS FOR DANGEROUS BOARDS 
			if (nrankedcommon==2)  { m_nouts-=3.5; }
			else if (nrankedcommon==3)  { m_nouts-=10; } 
			if (rankloplayer>rankhicommon && rankloplayer>=12 && 
				( (nsuitedcommon>1 && nsuited<4) || (nstraightcommon>1 && nstraight<4) ) )
				{ m_nouts-=nsuitedcommon+nstraightcommon; } 
			if ( (nstraight==4 && nstraightfillcommon>2 && ((rankbits)&16398)!=16398 && ((rankbits&28674))!=28674) ||
				 (nstraightfill==1 && nstraightfillcommon>=2) || 
				 (((rankbits)>>(ranklo))==0x5d) )
			{
				if (nsuitedcommon==2)  { m_nouts-=2; }
				else if (nsuitedcommon==3 && nsuited>nsuitedcommon && 
					bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer))<=2)  { m_nouts-=2; }
				else if (nsuitedcommon==3)  { m_nouts-=8; }
			}
		}
		else if (br == 3) {
			//FLUSH OUTS 
			if (nsuited==4 && nsuitedcommon==2 && bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer))<=4)  { m_nouts+=9; }
			else if (nsuited==4 && nsuitedcommon==2 && bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer))>4)  { m_nouts+=8; }
			else if (nsuited==4 && nsuitedcommon==3 && bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer))<=1)  { m_nouts+=9; }
			else if (nsuited==4 && nsuitedcommon==3 && bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer))>1 && 
				bitcount(((~(srankbits)&0x7ffe)>>(srankhiplayer)))<4)  { m_nouts+=3; }
			if (nsuited==4 && nsuitedcommon==2 && (pokerval)&(onepair) && nrankedcommon==1)  { m_nouts+=4; }
			if (nsuited==4 && nsuitedcommon==3 && (pokerval)&(onepair) && nrankedcommon==1 && 
				bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer))<=1)  { m_nouts+=4; }
			//STRAIGHT OUTS 
			if (nstraight==4 && nstraightfillcommon>1 && ((rankbits)&16398)!=16398 && ((rankbits)&28674)!=28674)  { m_nouts+=8; }
			else if (((rankbits)>>(ranklo))==0x5d)  { m_nouts+=9; }
			else if (nstraight==4 && nstraightcommon==3 && rankloplayer>rankhicommon && 
				rankhiplayer-rankloplayer<=2 && ((rankbits)&16398)!=16398 && ((rankbits)&28674)!=28674)  { m_nouts+=8; }
			else if (nstraightfill==1 && nstraightfillcommon>=2)  { m_nouts+=4; }
			//OVERCARD OUTS 
			if (rankloplayer>rankhicommon && rankloplayer>=12 && 
				nsuitedcommon<=2 && nsuited>nsuitedcommon && nstraightfillcommon>2)  { m_nouts+=3; }
			if (nsuitedcommon<=2 && rankhiplayer>rankhicommon && rankhiplayer>11 && 
				rankloplayer>9 && nstraightfillcommon>=2)  { m_nouts+=2; }
			if (nsuited==4 && nsuitedcommon==2 && rankloplayer>rankhicommon && 
				nstraightfillcommon>=2)  { m_nouts+=2; }
			else if (nsuited==4 && nsuitedcommon==2 && rankhiplayer>rankhicommon && 
				rankloplayer<rankhicommon && nstraightfillcommon>=2)  { m_nouts+=1; }
			//DISCOUNT OUTS FOR DANGEROUS BOARDS 
			if ((pokervalcommon)&(twopair))  { m_nouts-=12; } 
			else if (nrankedcommon==2 && (cr0==cr3 || cr1==cr3 || 
				cr2==cr3))  { m_nouts-=1; }
			else if (nrankedcommon==2)  { m_nouts-=3; }
			else if (nrankedcommon==3)  { m_nouts-=12; } 
			if (rankloplayer>rankhicommon && rankloplayer>=12 && 
				( (nsuitedcommon>1 && nsuited<4) || (nstraightcommon>1 && nstraight<4) ) )
				{ m_nouts-=nsuitedcommon+nstraightcommon; } 
			if ( (nstraight==4 && nstraightfillcommon>2 && ((rankbits)&16398)!=16398 && ((rankbits)&28674)!=28674) ||
				 (nstraightfill==1 && nstraightfillcommon>=2) || 
				 (((rankbits)>>(ranklo))==0x5d) )		{
				if (nsuitedcommon==2 && (cs0==cs3 || cs1==cs3 || 
					cs2==cs3))  { m_nouts-=0; }
				else if (nsuitedcommon==2)  { m_nouts-=2; }
				else if (nsuitedcommon==3 && nsuited>nsuitedcommon && 
					bitcount((~(srankbits)&0x7ffe)>>(srankhiplayer))<=2)  { m_nouts-=2; }
				else if (nsuitedcommon==3)  { m_nouts-=8; }
			}
		}
		if (m_nouts <= 0.0) {
			m_nouts = 0.0000001 ;
		}
		return m_nouts;
	}
	catch (...) { 
		logfatal("MyHand::nouts\n"); 
		throw; 
	}
}

*/