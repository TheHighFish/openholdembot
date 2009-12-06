#include "stdafx.h"
#include "CHandHistory.h"

using namespace std;

CHandHistory *p_handhistory = NULL;

CHandHistory::CHandHistory()
{
	gameNumber = 83910;
	nchairs = 10;
}
void CHandHistory::makeHistory()
{
	updateSymbols();

	outfile.open("HandHistoryTest.txt", fstream::app);

	if(prevdealerchair!=dealerchair) 
	{
		roundStart(); 
	}

	checkBetround();

	if(betroundSet[5]==false)scanPlayerChanges();

	if(isShowdown())processShowdown();

	outfile.close();

	setPreviousActions();
}
void CHandHistory::updateSymbols()
{
	pot = p_symbols->sym()->pot;
	rake = p_symbols->sym()->rake;
	for(int i=0;i<4;i++)
		bet[i] = p_symbols->sym()->bet[i];
	userchair = (int)p_symbols->sym()->userchair;
	cbits = (int)p_symbols->sym()->playersplayingbits;
	betround = (int)p_symbols->sym()->betround;
	potplayer = p_symbols->sym()->potplayer;
	sblind = p_symbols->sym()->sblind;
	bblind = p_symbols->sym()->bblind;
	nplayersactive = (int) p_symbols->sym()->nplayersactive;
	nplayersplaying = (int) p_symbols->sym()->nplayersplaying;
	raischair = (int) p_symbols->sym()->raischair;
	dealerchair = (int) p_symbols->sym()->dealerchair;
	postflopstart=sblindpos;
	for(int i=0;i<5;i++)
		GetBCstring(card_common[i], p_scraper->card_common(i));
	for(int i=0;i<nchairs;i++)
	{
		GetPCstring(card_player[i], p_scraper->card_player(i,0), p_scraper->card_player(i,1));
		playersplayingbits[i]=(cbits>>i)&1;
		currentbetx[i]= p_symbols->sym()->currentbet[i];
		playerbalance[i]= p_symbols->sym()->balance[i];
		strncpy_s(playername, 16, p_scraper->player_name(i).GetString(), _TRUNCATE);
		splayername[i] = playername;
		playerSeated[i]=false;
		if(splayername[i]!="")playerSeated[i]=true;
	}

}
void CHandHistory::roundStart()
{
	nchairs = (int)p_symbols->sym()->nchairs;
	pCardsSeen = 0;
	gameNumber++;
	maxBet=1;
	passChecks=1;
	int utg;
	for(int i=0;i<7;i++)betroundSet[i]=false;
	for(int i=0;i<4;i++)allChecks[i]=1;
	for(int i=0;i<nchairs;i++)
		ac_dealpos[i] = DealPosition(i);
	for(int i=0;i<nchairs;i++)
	{
		if(ac_dealpos[i]==1)sblindpos=i;
		else if(ac_dealpos[i]==2)bblindpos=i;
		else if(ac_dealpos[i]==3)utg=i;
	}
	whosturn=utg;
	for(int i=0;i<nchairs;i++)
	{
		seatsPlaying[i]=true;
		handval[i]=0;
		CardMask_RESET(hand[i]);
		cardsSeen[i]=false;
		handText[i]="";
	}
	outfile<<"\nGAME #"<<gameNumber<<": Texas Hold'em "<<findLimit()<<" $"<<bblind<<"/$"<<(bblind*2)<<" "<<setDate()<<endl;
	outfile<<"Table "<<p_scraper->title()<<endl;
	for(int i=1;i<=nchairs;i++)
	{
		int m = (i+userchair-1)%nchairs;
		if(splayername[m]!="")
		{
			outfile<<"Seat "<<i<<": "<<splayername[m]<<" ( $"<<playerbalance[m]<<" in chips)";
			if (m==dealerchair) outfile<<" DEALER"<<endl;
			else outfile<<endl;
		}
	}
	outfile<<splayername[sblindpos]<<": Post SB $"<<sblind<<endl;
	outfile<<splayername[bblindpos]<<": Post BB $"<<bblind<<endl;
}
void CHandHistory::checkBetround()
{
	if(betroundSet[0]==false&&card_player[userchair][0]!=NULL&&card_player[userchair][1]!=NULL)
	{
		outfile<<"*** HOLE CARDS ***"<<endl;
		outfile<<"Dealt to "<<splayername[4]<<" [ "<<card_player[userchair][0]<<card_player[userchair][1]
		<<" "<<card_player[userchair][2]<<card_player[userchair][3]<<" ]"<<endl;
		passChecks=0;
		betroundSet[0]=true;
		lpta = -5;
	}
	if(whosturn==((lpta+1)%nchairs)&&betroundSet[1]==false&&card_common[0][1]!=NULL&&card_common[1][1]!=NULL&&card_common[2][1]!=NULL)
	{
		outfile<<"*** FLOP *** [ "
			<<card_common[0][0]<<card_common[0][1]<<" "
			<<card_common[1][0]<<card_common[1][1]<<" "
			<<card_common[2][0]<<card_common[2][1]<<" ]"<<endl;
		whosturn=postflopstart;
		lpta = -5;
		betroundSet[1]=true;
		passChecks=0;
		maxBet=0;
	}
	if((whosturn==((lpta+1)%nchairs)||allChecks[1]==1)&&betroundSet[2]==false&&card_common[3][1]!=NULL)
	{
		if(allChecks[1]==1)
		{
			checkAll();
		}
		outfile<<"*** TURN *** [ "<<card_common[3][0]<<card_common[3][1]<<" ]"<<endl;
		whosturn=postflopstart;
		lpta = -5;
		betroundSet[2]=true;
		passChecks=0;
		maxBet=0;
	}
	if((whosturn==((lpta+1)%nchairs)||allChecks[2]==1)&&betroundSet[3]==false&&card_common[4][1]!=NULL)
	{
		if(allChecks[2]==1)
		{
			checkAll();
		}
		outfile<<"*** RIVER *** [ "<<card_common[4][0]<<card_common[4][1]<<" ]"<<endl;
		whosturn=postflopstart;
		lpta = -5;
		betroundSet[3]=true;
		passChecks=0;
		maxBet=0;
	}
	if((whosturn==((lpta+1)%nchairs)||allChecks[3]==1)&&betroundSet[4]==false&&isShowdown())
	{
		if(allChecks[3]==1)
		{
			checkAll();
		}
		outfile<<"*** SUMMARY ***"<<endl;
		outfile<<"Total pot $"<<pot<<" Rake $"<<(rake*pot)<<endl;
		betroundSet[4]=true;
	}
}
void CHandHistory::scanPlayerChanges()
{
	if((prevround<=2&&betroundSet[0]==true)||
		(prevround<=3&&betroundSet[1]==true)||
		(prevround<=4&&betroundSet[2]==true)||
		(prevround<=5&&betroundSet[3]==true))
	{
		for(int i=0;i<nchairs;i++) 
		{
			if(whosturn==i&&whosturn!=(lpta+1)%10)
			{
				if(playersplayingbits[i]!=0)
				{
					if(isBigBlind(i)&&allChecks[betround-1]==1)
					{
						lpta=(dealerchair+2)%nchairs;
					}
					if(betround!=1&&potplayer!=0&&passChecks!=1&&raischair!=postflopstart) 
					{
						whosturn=raischair;
						passChecks=1;
					}
					if(currentbetx[i]>maxBet)
					{
						if(betround!=1&&i!=postflopstart&&allChecks[betround-1]==1)
						{
							int j=postflopstart;
							while(j!=i)
							{
								if(playersplayingbits[j]!=0)outfile<<splayername[j]<<" Check"<<endl;
								j=(j+1)%nchairs;
							}
						}
						if(maxBet==0)outfile<<splayername[i]<<": Bet $"<<currentbetx[i]<<endl;
						else outfile<<splayername[i]<<": Raise $"<<currentbetx[i]<<endl;
						lpta = (i-1)%nchairs;
						if(lpta==-1)lpta=nchairs-1;
						whosturn=(whosturn+1)%nchairs;
						allChecks[betround-1]=0;
						maxBet = currentbetx[i];
						passChecks=1;
					}
					else if(
					(currentbetx[i]==maxBet&&maxBet!=0)
					||
					(prevround!=betround&&betround!=1&&maxBet!=0)
					)
					{
						if(isBigBlind(i))
							outfile<<splayername[i]<<": Check"<<endl;
						else
							outfile<<splayername[i]<<": Call $"<<maxBet<<endl;
						
						whosturn=(whosturn+1)%nchairs;
					}
				}
				else
				{
					if(seatsPlaying[i]==true&&playerSeated[i]==true)
						outfile<<splayername[i]<<": Fold"<<endl;
					seatsPlaying[i]=false;
					whosturn=(whosturn+1)%nchairs;
					int nplayersin=0;
					for(int i=0;i<nchairs;i++)
					{
						if(seatsPlaying[i]==true) nplayersin++;
					}
					if(nplayersin==1)
						for(int j=0;j<nchairs;j++)
							if(seatsPlaying[j]==true)
								outputUncontested(j);
				}
			}
		}
	}
}
void CHandHistory::setPreviousActions()
{
	prevdealerchair = dealerchair;
	prevround = betround;
	if(isShowdown())prevround = 5;
	for(int i=0;i<nchairs;i++)
	{
		prevplayerbalance[i] = playerbalance[i];
		prevbetx[i]= currentbetx[i];
	}
}
string CHandHistory::setDate()
{
	SYSTEMTIME st;
	GetSystemTime(&st);

	stringstream ss;
	string s;
	ss<<st.wYear<<"-"<<st.wMonth<<"-"<<st.wDay<<" "<<st.wHour<<":"<<st.wMinute<<":"<<st.wSecond;
	s = ss.str()+" ";

	return s;
}
bool CHandHistory::isShowdown()
{
	for(int i=0;i<nchairs;i++)
		if(card_player[i][1]!=NULL&&i!=userchair&&playersplayingbits[i]!=0)return true;

	return false;
}
void CHandHistory::GetPCstring(char *c, unsigned int c0, unsigned int c1)
{
	char		card0[10] = {0}, card1[10] = {0};

	// figure out the card string to search for
	if (StdDeck_RANK(c0) >= StdDeck_RANK(c1))
	{
		StdDeck_cardToString(c0, card0);
		StdDeck_cardToString(c1, card1);
	}
	else
	{
		StdDeck_cardToString(c1, card0);
		StdDeck_cardToString(c0, card1);
	}
	c[0] = card0[0];
	c[1] = card0[1];
	c[2] = card1[0];
	c[3] = card1[1];

	c[4]='\0';
}
void CHandHistory::GetBCstring(char *c, unsigned int c0)
{
	char		card0[10] = {0};

	// figure out the card string to search for
		StdDeck_cardToString(c0, card0);

	c[0] = card0[0];
	c[1] = card0[1];

	c[3]='\0';
}
int CHandHistory::DealPosition (const int chairnum)
{
	int		dealposchair = 0;
	int		count = 1;
	int		i = (dealerchair+1)%nchairs;

	while(i!=chairnum)
	{
		if(playerSeated[i]==true) count++;
		i=(i+1)%nchairs;
	}

	return count;
}
void CHandHistory::processShowdown()
{
	for(int i=0;i<nchairs;i++)
	{
		if(playersplayingbits[i]!=0&&cardsSeen[i]==false&&card_player[i][1]!=NULL)
		{
			stringstream ss;
			ss<<splayername[i]<<": Shows [ "
			<<card_player[i][0]
			<<card_player[i][1]<<" "
			<<card_player[i][2]
			<<card_player[i][3]<<" ]";
			handText[i] = ss.str()+" ";

			for(int j=0;j<2;j++)
				CardMask_SET(hand[i],p_scraper->card_player(i,j));
			for(int j=2;j<7;j++)
				CardMask_SET(hand[i],p_scraper->card_common(j-2));

			handval[i] = Hand_EVAL_N(hand[i], 7);

			pCardsSeen++;
			cardsSeen[i]=true;	
		}
	}
	if(betroundSet[4]==true)
		showdownResults();
}
void CHandHistory::showdownResults()
{
	if(pCardsSeen==nplayersplaying&&betroundSet[5]==false)
	{
		for(int i=0;i<nchairs;i++)
			if(handText[i]!="")
				outfile<<handText[i]<<endl;

		HandVal handhigh = {0};
		int highest=0;
		for(int i=0;i<nchairs;i++)
			if(handval[i]>handhigh)
			{
				handhigh=handval[i];
				highest = i;
			}
			outfile<<splayername[highest]<<": wins $"<<pot<<" from the main pot with ";
		int currentType = Hand_EVAL_TYPE(hand[highest],7);
		switch(currentType)
		{
            case StdRules_HandType_NOPAIR: outfile<<"no Pair."<<endl;; break;
            case StdRules_HandType_ONEPAIR: outfile<<"One Pair."<<endl; break;
            case StdRules_HandType_TWOPAIR: outfile<<"Two Pair."<<endl; break;
            case StdRules_HandType_TRIPS: outfile<<"Trips."<<endl; break;
            case StdRules_HandType_STRAIGHT: outfile<<"a Straight."<<endl; break;
            case StdRules_HandType_FLUSH: outfile<<"a Flush."<<endl; break;
            case StdRules_HandType_FULLHOUSE: outfile<<"a Fullhouse."<<endl; break;
            case StdRules_HandType_QUADS: outfile<<"Quads."<<endl; break;
            case StdRules_HandType_STFLUSH: outfile<<"a Straight Flush."<<endl; break;
		}
		betroundSet[5]=true;
	}
}
void CHandHistory::outputUncontested(int j)
{
	if(betroundSet[6]==false)
	{
		outfile<<"*** SUMMARY ***"<<endl;
		outfile<<"Total pot $"<<pot<<" Rake $"<<(rake*pot)<<endl;
		outfile<<splayername[j]<<": wins $"<<pot<<endl;
		betroundSet[6]=true;
	}
}
bool CHandHistory::isBigBlind(int i)
{
	return(currentbetx[i]==bet[betround-1]&&ac_dealpos[i]==2&&betround==1);
}
void CHandHistory::checkAll()
{
	int i=postflopstart;
	do
	{
		if(playersplayingbits[i]!=0)outfile<<splayername[i]<<": Check"<<endl;
		i=(i+1)%nchairs;
	}while(i!=(dealerchair+1)%nchairs);
}
string CHandHistory::findLimit()
{
	int lim = (int)p_symbols->sym()->lim;
	string str;
	if(lim==0)str="NL";
	else if(lim==1)str="PL";
	else if(lim==2)str="FL";
	return str;
}