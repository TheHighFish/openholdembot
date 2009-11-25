#include "stdafx.h"

#include <sstream>
#include <Windows.h>
#include "CHandHistory.h"

#include "CGameState.h"
#include "CPokerAction.h"
#include "CMemory.h"
#include "CSymbols.h"
#include "CScraper.h"

using namespace std;

void CHandHistory::makeHistory()
{
	updateSymbols();

	outfile.open("HandHistoryTest.txt", fstream::app);

	if(prevdealerchair!=dealerchair) 
	{
		roundStart();
	}

	checkBetround();
	scanPlayerChanges();

	outfile.close();
	setPreviousActions();
}
void CHandHistory::updateSymbols()
{
	postflopstart=(dealerchair+1)%10;
	userchair = (int)p_symbols->sym()->userchair;
	cbits = (int)p_symbols->sym()->playersplayingbits;
	betround = (int)p_symbols->sym()->betround;
	potplayer = p_symbols->sym()->potplayer;
	sblind = p_symbols->sym()->sblind;
	bblind = p_symbols->sym()->bblind;
	nplayersactive = (int) p_symbols->sym()->nplayersactive;
	raischair = (int) p_symbols->sym()->raischair;
	dealerchair = (int) p_symbols->sym()->dealerchair;
	for(int i=0;i<5;i++)
		card_common[i]=p_scraper->card_common(i);
	for(int i=0;i<10;i++)
		for(int j=0;j<2;j++)
			card_player[i][j]=p_scraper->card_player(i,j);
	for(int i=0;i<10;i++)
	{
		playersplayingbits[i]=(cbits>>i)&1;
		currentbetx[i]= p_symbols->sym()->currentbet[i];
		playerbalance[i]= p_symbols->sym()->balance[i];
		strncpy_s(playername, 16, p_scraper->player_name(i).GetString(), _TRUNCATE);
		splayername[i]=playername;
		//ac_dealpos[i] = action.DealPosition(i);
	}
	for(int i=0;i<10;i++)
	{
		if(ac_dealpos[i]==1)sblindpos=i;
		else if(ac_dealpos[i]==2)bblindpos=i;
	}
}
void CHandHistory::roundStart()
{
	maxBet=1;
	for(int i=0;i<=4;i++)allChecks[i]=true;
	whosturn=(dealerchair+3)%10;
	for(int i=0;i<=10;i++)seatsPlaying[i]=1;
	for(int i=0;i<=50;i++)alreadySet[i]=false;
	passChecks=1;
	outfile<<"#Game No : "<<gameNumber<<endl;
	outfile<<"***** Hand History for Game "<<gameNumber<<" *****"<<endl;
	outfile<<"$"<<bblind<<"/$"<<(bblind*2)<<" Hold'em - "<<setDate()<<endl;
	outfile<<"Table Full Ring Beginners (Real Money)"<<endl;
	outfile<<"Seat "<<dealerchair<<" is the button"<<endl;
	outfile<<"Total number of players : "<<nplayersactive<<endl;
	for(int i=1;i<=10;i++)
	{
		int m = (i+3)%10;
		outfile<<"Seat "<<i<<": "<<splayername[m]<<" ( $"<<playerbalance[m]<<" )"<<endl;
	}
	outfile<<splayername[sblindpos]<<" posts small blind ($"<<sblind<<")"<<endl;
	outfile<<splayername[bblindpos]<<" posts big blind ($"<<bblind<<")"<<endl;
	outfile<<"** Dealing down cards **"<<endl;
}
void CHandHistory::checkBetround()
{
	if(alreadySet[40]==false&&card_player[userchair][0]!=CARD_NOCARD&&card_player[userchair][1]!=CARD_NOCARD)
	{
		outfile<<"Dealt to "<<splayername[4]<<" [ "<<userCards[0]<<userCards[1]
		<<" "<<userCards[2]<<userCards[3]<<" ]"<<endl;
		passChecks=0;
		alreadySet[40]=true;
	}
	if(alreadySet[41]==false&&card_common[0]!=CARD_NOCARD&&card_common[1]!=CARD_NOCARD&&card_common[2]!=CARD_NOCARD)
	{
		outfile<<"** Dealing Flop ** : [ "
			<<totalCards[7]<<totalCards[8]<<", "
			<<totalCards[9]<<totalCards[10]<<", "
			<<totalCards[11]<<totalCards[12]<<" ]"<<endl;
		whosturn=postflopstart;
		alreadySet[41]=true;
		passChecks=0;
		maxBet=0;
	}
	if(alreadySet[42]==false&&card_common[3]!=CARD_NOCARD)
	{
		if(allChecks[1]==true)
		{
			int i=postflopstart;
			do
			{
				if(playersplayingbits[i]!=0)outfile<<splayername[i]<<" checks."<<endl;
				i=(i+1)%10;
			}while(i!=dealerchair);
		}
		outfile<<"** Dealing Turn ** : [ "<<totalCards[13]<<totalCards[14]<<" ]"<<endl;
		whosturn=postflopstart;
		alreadySet[42]=true;
		passChecks=0;
		maxBet=0;
	}
	if(alreadySet[43]==false&&card_common[4]!=CARD_NOCARD)
	{
		if(allChecks[2]==true)
		{
			int i=postflopstart;
			do
			{
				if(playersplayingbits[i]!=0)outfile<<splayername[i]<<" checks."<<endl;
				i=(i+1)%10;
			}while(i!=dealerchair);
		}
		outfile<<"** Dealing River ** : [ "<<totalCards[15]<<totalCards[16]<<" ]"<<endl;
		whosturn=postflopstart;
		alreadySet[43]=true;
		passChecks=0;
		maxBet=0;
	}
}
void CHandHistory::scanPlayerChanges()
{
	if(((prevround==1&&alreadySet[40]==true)||
		(prevround==2&&alreadySet[41]==true)||
		(prevround==3&&alreadySet[42]==true)||
		(prevround==4&&alreadySet[43]==true))&&!isShowdown())
		{
		for(int i=0;i<=10;i++)
		{
			if(whosturn==i)
			{
				if(playersplayingbits[i]!=0)
				{
					if(currentbetx[i]>maxBet)
					{
						if(betround!=1&&i!=(postflopstart)&&allChecks[betround-1]==true)
						{
							int j=postflopstart;
							while(j!=i)
							{
								if(playersplayingbits[j]!=0)outfile<<splayername[j]<<" checks."<<endl;
								j=(j+1)%10;
							}
						}
						if(maxBet==0)outfile<<splayername[i]<<" bets [$"<<currentbetx[i]<<"]."<<endl;
						else outfile<<splayername[i]<<" raises [$"<<currentbetx[i]<<"]."<<endl;
						whosturn=(whosturn+1)%10;
						allChecks[betround-1]=false;
						maxBet = currentbetx[i];
						passChecks=1;
					}
					else if(currentbetx[i]==maxBet&&maxBet!=0)
					{
						outfile<<splayername[i]<<" calls [$"<<maxBet<<"]."<<endl;
						whosturn=(whosturn+1)%10;
					}
					else if(betround!=1&&potplayer!=0&&passChecks==0&&i!=postflopstart) 
					{
						whosturn=raischair;
						passChecks=1;
					}
				}
				else
				{
					if(seatsPlaying[i]!=0)outfile<<splayername[i]<<" folds."<<endl;
					seatsPlaying[i]=0;
					whosturn=(whosturn+1)%10;
				}
			}
		}
	}
}
void CHandHistory::setPreviousActions()
{
	prevdealerchair = dealerchair;
	prevround = betround;
	for(int i=0;i<10;i++)
	{
		prevplayerbalance[i] = playerbalance[i];
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
	for(int i=0;i<10;i++)
//		if(pCards[i][0]!=pCards[i][1]&&i!=4&&playersplayingbits[i]!=0)return true;

	return false;
}
