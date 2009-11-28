#include "stdafx.h"
#include "CHandHistory.h"

using namespace std;

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
	//outfile<<"whoseturn: "<<whosturn<<endl;
	//outfile<<"lpta: "<<lpta<<endl;
	scanPlayerChanges();
	if(isShowdown())processShowdown();
	//if(!isShowdown()&&nplayersplaying==1)checkContest();

	outfile.close();
	setPreviousActions();
}
void CHandHistory::updateSymbols()
{
	pot = p_symbols->sym()->pot;
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
	postflopstart=(dealerchair+1)%nchairs;
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
		ac_dealpos[i] = DealPosition(i);
	}
	for(int i=0;i<nchairs;i++)
	{
		if(ac_dealpos[i]==1)sblindpos=i;
		else if(ac_dealpos[i]==2)bblindpos=i;
	}
}
void CHandHistory::roundStart()
{
	nchairs = (int)p_symbols->sym()->nchairs;
	pCardsSeen = 0;
	gameNumber++;
	maxBet=1;
	passChecks=1;
	for(int i=0;i<5;i++)betroundSet[i]=false;
	for(int i=0;i<4;i++)allChecks[i]=1;
	whosturn=(dealerchair+3)%nchairs;
	for(int i=0;i<nchairs;i++)
	{
		seatsPlaying[i]=1;
		handval[i]=0;
		CardMask_RESET(hand[i]);
		cardsSeen[i]=false;
	}
	outfile<<"\n#Game No : "<<gameNumber<<endl;
	outfile<<"***** Hand History for Game "<<gameNumber<<" *****"<<endl;
	outfile<<"$"<<bblind<<"/$"<<(bblind*2)<<" Hold'em - "<<setDate()<<endl;
	outfile<<"Table Full Ring Beginners (Real Money)"<<endl;
	outfile<<"Seat "<<dealerchair<<" is the button"<<endl;
	outfile<<"Total number of players : "<<nplayersactive<<endl;
	for(int i=1;i<=nchairs;i++)
	{
		int m = (i+userchair-1)%nchairs;
		outfile<<"Seat "<<i<<": "<<splayername[m]<<" ( $"<<playerbalance[m]<<" )"<<endl;
	}
	outfile<<splayername[sblindpos]<<" posts small blind ($"<<sblind<<")"<<endl;
	outfile<<splayername[bblindpos]<<" posts big blind ($"<<bblind<<")"<<endl;
	outfile<<"** Dealing down cards **"<<endl;
}
void CHandHistory::checkBetround()
{
	if(betroundSet[0]==false&&card_player[userchair][0]!=NULL&&card_player[userchair][1]!=NULL)
	{
		outfile<<"Dealt to "<<splayername[4]<<" [ "<<card_player[userchair][0]<<card_player[userchair][1]
		<<" "<<card_player[userchair][2]<<card_player[userchair][3]<<" ]"<<endl;
		passChecks=0;
		betroundSet[0]=true;
		lpta = 11;
	}
	if(whosturn==(lpta+1)&&betroundSet[1]==false&&card_common[0][1]!=NULL&&card_common[1][1]!=NULL&&card_common[2][1]!=NULL)
	{
		outfile<<"** Dealing Flop ** : [ "
			<<card_common[0][0]<<card_common[0][1]<<", "
			<<card_common[1][0]<<card_common[1][1]<<", "
			<<card_common[2][0]<<card_common[2][1]<<" ]"<<endl;
		whosturn=postflopstart;
		lpta = 11;
		betroundSet[1]=true;
		passChecks=0;
		maxBet=0;
	}
	if((whosturn==(lpta+1)||allChecks[1]==1)&&betroundSet[2]==false&&card_common[3][1]!=NULL)
	{
		if(allChecks[1]==1)
		{
			int i=postflopstart;
			do
			{
				if(playersplayingbits[i]!=0)outfile<<splayername[i]<<" checks."<<endl;
				i=(i+1)%nchairs;
			}while(i!=(dealerchair+1));
		}
		outfile<<"** Dealing Turn ** : [ "<<card_common[3][0]<<card_common[3][1]<<" ]"<<endl;
		whosturn=postflopstart;
		lpta = 11;
		betroundSet[2]=true;
		passChecks=0;
		maxBet=0;
	}
	if((whosturn==(lpta+1)||allChecks[2]==1)&&betroundSet[3]==false&&card_common[4][1]!=NULL)
	{
		if(allChecks[2]==1)
		{
			int i=postflopstart;
			do
			{
				if(playersplayingbits[i]!=0)outfile<<splayername[i]<<" checks."<<endl;
				i=(i+1)%nchairs;
			}while(i!=(dealerchair+1));
		}
		outfile<<"** Dealing River ** : [ "<<card_common[4][0]<<card_common[4][1]<<" ]"<<endl;
		whosturn=postflopstart;
		lpta = 11;
		betroundSet[3]=true;
		passChecks=0;
		maxBet=0;
	}
	if((whosturn==(lpta+1)||allChecks[3]==1)&&betroundSet[4]==false&&isShowdown())
	{
		if(allChecks[3]==1)
		{
			int i=postflopstart;
			do
			{
				if(playersplayingbits[i]!=0)outfile<<splayername[i]<<" checks."<<endl;
				i=(i+1)%nchairs;
			}while(i!=(dealerchair+1));
		}
		outfile<<"** Summary **"<<endl;
		whosturn=postflopstart;
		betroundSet[4]=true;
	}
}
void CHandHistory::scanPlayerChanges()
{
	if(((prevround<=1&&betroundSet[0]==true)||
		(prevround<=3&&betroundSet[1]==true)||
		(prevround<=4&&betroundSet[2]==true)||
		(prevround<=4&&betroundSet[3]==true))&&!isShowdown())
	{
		for(int i=0;i<nchairs;i++)
		{
			if(whosturn==i&&whosturn!=(lpta+1))
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
								if(playersplayingbits[j]!=0)outfile<<splayername[j]<<" checks."<<endl;
								j=(j+1)%nchairs;
							}
						}
						if(maxBet==0)outfile<<splayername[i]<<" bets [$"<<currentbetx[i]<<"]."<<endl;
						else outfile<<splayername[i]<<" raises [$"<<currentbetx[i]<<"]."<<endl;
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
						outfile<<splayername[i]<<" calls [$"<<maxBet<<"]."<<endl;
						whosturn=(whosturn+1)%nchairs;
					}
				}
				else
				{
					if(seatsPlaying[i]!=0)
						outfile<<splayername[i]<<" folds."<<endl;
					seatsPlaying[i]=0;
					whosturn=(whosturn+1)%nchairs;
				}
				prevplayerbalance[i] = playerbalance[i];
			}
		}
	}
}
void CHandHistory::setPreviousActions()
{
	prevdealerchair = dealerchair;
	prevround = betround;
	for(int i=0;i<nchairs;i++)
	{
		//prevplayerbalance[i] = playerbalance[i];
		prevbetx[i]= currentbetx[i];
	}
}
string CHandHistory::setDate()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	int iday=st.wDayOfWeek;
	int imonth=st.wMonth;
	string day;
	string month;
	if (iday==1) day="Sunday";
	else if(iday==2)day="Monday";
	else if(iday==3)day="Tuesday";
	else if(iday==4)day="Wednesday";
	else if(iday==5)day="Thursday";
	else if(iday==6)day="Friday";
	else if(iday==7)day="Saturday";

	if (imonth==1)month="January";
	else if(imonth==2)month="Febuary";
	else if(imonth==3)month="March";
	else if(imonth==4)month="April";
	else if(imonth==5)month="May";
	else if(imonth==6)month="June";
	else if(imonth==7)month="July";
	else if(imonth==8)month="August";
	else if(imonth==9)month="September";
	else if(imonth==10)month="October";
	else if(imonth==11)month="November";
	else if(imonth==12)month="December";

	stringstream ss;
	string s;
	ss<<day<<", "<<month<<" "<<st.wDay<<", "<<st.wHour<<":"<<st.wMinute<<":"<<st.wSecond<<" EDT "<<st.wYear;
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
	int		i = 0;
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
}
void CHandHistory::processShowdown()
{
	for(int i=0;i<nchairs;i++)
	{
		if(playersplayingbits[i]!=0&&cardsSeen[i]==false&&card_player[i][1]!=NULL)
		{
			outfile<<splayername[i]<<" shows [ "
			<<card_player[i][0]
			<<card_player[i][1]<<", "
			<<card_player[i][2]
			<<card_player[i][3]<<" ]"<<endl;

			for(int j=0;j<2;j++)
				CardMask_SET(hand[i],p_scraper->card_player(i,j));
			for(int j=2;j<7;j++)
				CardMask_SET(hand[i],p_scraper->card_common(j-2));

			handval[i] = Hand_EVAL_N(hand[i], 7);

			pCardsSeen++;
			cardsSeen[i]=true;	
			whosturn=(whosturn+1)%nchairs;
		}
	}
	//if(betroundSet[4]==true)
		showdownResults();
}
void CHandHistory::showdownResults()
{
	if(pCardsSeen==nplayersplaying)
	{
		HandVal handhigh = {0};
		int highest=0;
		for(int i=0;i<nchairs;i++)
			if(handval[i]>handhigh)
			{
				handhigh=handval[i];
				highest = i;
			}
		outfile<<splayername[highest]<<" wins $"<<pot<<" from the main pot with ";
		int currentType = Hand_EVAL_TYPE(hand[highest],7);
		switch(currentType)
		{
            case StdRules_HandType_NOPAIR: outfile<<"no pair."<<endl;; break;
            case StdRules_HandType_ONEPAIR: outfile<<"one pair."<<endl; break;
            case StdRules_HandType_TWOPAIR: outfile<<"two pair."<<endl; break;
            case StdRules_HandType_TRIPS: outfile<<"trips."<<endl; break;
            case StdRules_HandType_STRAIGHT: outfile<<"a straight."<<endl; break;
            case StdRules_HandType_FLUSH: outfile<<"a flush."<<endl; break;
            case StdRules_HandType_FULLHOUSE: outfile<<"a fullhouse."<<endl; break;
            case StdRules_HandType_QUADS: outfile<<"quads."<<endl; break;
            case StdRules_HandType_STFLUSH: outfile<<"a straight flush."<<endl; break;
		}
	}
}
void CHandHistory::checkContest()
{
	for(int i=0;i<nchairs;i++)
		if(playersplayingbits[i]!=0)outfile<<splayername[i]<<" wins $"<<pot<<" from the main pot uncontested.";
}
bool CHandHistory::isBigBlind(int i)
{
	return(currentbetx[i]==bet[betround-1]&&ac_dealpos[i]==2&&betround==1);
}