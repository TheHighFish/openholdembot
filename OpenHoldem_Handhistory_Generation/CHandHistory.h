/*
Does NOT compile, I'm still in the process of transferring functions.
*/
/*
#include <fstream>
#include <sstream>
#include <Windows.h>
#include "CGameState.h"
#include "CPokerAction.h"

using namespace std;

void makeHistory();
void setPreviousActions();
string setDate();
bool isShowdown();

bool allChecks[4];
int prevround;
int prevdealerchair;
int prevplayerbalance[10]
int prevbetx[10];
bool alreadySet[50];
int passChecks;
int maxBet;
int whosturn;

void makeHistory()
{
	CPokerAction	action;
	ofstream outfile;
	int seatsPlaying[10];
	int ac_dealpos[10];
	char playername[16];
	string splayername[10];
	int playersplayingbits[10];
	double currentbetx[10];
	double playerbalance[10];
	int gameNumber=1209;
	int sblindpos;
	int bblindpos;
	int cbits = (int)p_symbols->sym()->playersplayingbits;
	int betround = (int)p_symbols->sym()->betround;
	double potplayer = p_symbols->sym()->potplayer;
	double sblind = p_symbols->sym()->sblind;
	double bblind = p_symbols->sym()->bblind;
	int nplayersactive = (int) p_symbols->sym()->nplayersactive;
	int	raischair = (int) p_symbols->sym()->raischair;
	int	dealerchair = (int) p_symbols->sym()->dealerchair;
//========================================================
	for(int i=0;i<10;i++)
	{
		playersplayingbits[i]=(cbits>>i)&1;
		currentbetx[i]= p_symbols->sym()->currentbet[i];
		playerbalance[i]= p_symbols->sym()->balance[i];
		strncpy_s(playername, 16, p_scraper->player_name(i).GetString(), _TRUNCATE);
		splayername[i]=playername;
		//ac_dealpos[i] = action.DealPosition(i);
	}
//========================================================
	int postflopstart=(dealerchair+1)%10;
	for(int i=0;i<10;i++)
	{
		if(ac_dealpos[i]==1)sblindpos=i;
		else if(ac_dealpos[i]==2)bblindpos=i;
	}

	outfile.open("HandHistoryTest.txt", fstream::app);

	if(prevdealerchair!=dealerchair) 
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
//========================================================
	if(alreadySet[40]==false&&userCards[0]!=NULL&&userCards[1]!=NULL)
	{
		outfile<<"Dealt to "<<splayername[4]<<" [ "<<userCards[0]<<userCards[1]
		<<" "<<userCards[2]<<userCards[3]<<" ]"<<endl;
		passChecks=0;
		alreadySet[40]=true;
	}
	if(alreadySet[41]==false&&totalCards[7]!=NULL&&totalCards[8]!=NULL&&totalCards[9]!=NULL&&totalCards[10]!=NULL&&totalCards[11]!=NULL&&totalCards[12]!=NULL)
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
	if(alreadySet[42]==false&&totalCards[13]!=NULL&&totalCards[14]!=NULL)
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
	if(alreadySet[43]==false&&totalCards[15]!=NULL&&totalCards[16]!=NULL)
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
//========================================================
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
	outfile.close();
	setPreviousActions();
}
void setPreviousActions()
{
	prevdealerchair = (int) p_symbols->sym()->dealerchair;
	prevround = (int)p_symbols->sym()->betround;
	for(int i=0;i<=10;i++)
	{
		prevplayerbalance[i] = p_symbols->sym()->balance[i];
		prevbetx[i]= p_symbols->sym()->currentbet[i];
	}
}
string setDate()
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
bool isShowdown()
{
	for(int i=0;i<10;i++)
		if(pCards[i][0]!=pCards[i][1]&&i!=4&&playersplayingbits[i]!=0)return true;

	return false;
}
*/