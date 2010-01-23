#include "stdafx.h"
#include <tchar.h> 
#include <Windows.h>
#include <sstream>

#include "CHandHistory.h"

#include "CSymbols.h"
#include "CScraper.h"
#include "CGameState.h"
#include "CSessionCounter.h"
#include "OpenHoldem.h"
#include "poker_defs.h"
#include "enumerate.h"
#include "inlines/eval.h"
#include "inlines/eval_type.h"

using namespace std;

CHandHistory *p_handhistory = NULL;

CHandHistory::CHandHistory()
{
	time_t seconds;
	time(&seconds);
	srand((unsigned int) seconds);
	double randomnum = rand();
	//Attempt to create a random starting game number
	gameNumber = ((int)pow(randomnum,2))%10000000;	
	newRoundFlag=false;
	for(int i=0;i<10;i++)player[i].currentBalance=0;
}
void CHandHistory::makeHistory()
{
	updateSymbols();
	writeHistory();
	setPreviousActions();
}
void CHandHistory::writeHistory()
{
	int				betround = (int) p_symbols->sym()->betround;
	int				dealerchair = (int) p_symbols->sym()->dealerchair;

	//Precondition: New round
	if(prevdealerchair!=dealerchair&&betround==1)
		newRoundFlag=true;

	//Precondition: New round flag has been set and cards dealt
	if(newRoundFlag==true&&cardsDealt()==true)
		roundStart();

	checkBetround();

	//Precondition: Cards have been dealt and the round summary has not
	//been printed
	if(showdownSeen==false&&roundStarted==true)
		scanPlayerChanges(); 

	if(isShowdown())processShowdown();
}
void CHandHistory::updateSymbols()
{
	int				nchairs = (int)p_symbols->sym()->nchairs;
	int				betround = (int) p_symbols->sym()->betround;
	int				cbits = (int) p_symbols->sym()->playersplayingbits;
	int				userchair = (int) p_symbols->sym()->userchair;

	if (p_symbols->sym()->pot>=pot||betround<=1)
		pot = p_symbols->sym()->pot;

	for(int i=0;i<nchairs;i++)	//Set individual player variables
	{
		if(p_scraper->card_player(i,0)!=254||(i!=userchair&&userchair!=0))
			GetPCstring(player[i].card_player, p_scraper->card_player(i,0), p_scraper->card_player(i,1));

		player[i].playersPlayingBits=(cbits>>i)&1;

		//If the player is playing, update the symbol. This condition used for muck detection.
		if(player[i].playersPlayingBits!=0)
			player[i].currentBet= p_symbols->sym()->currentbet[i];

		player[i].currentBalance= p_symbols->sym()->balance[i];

		char	playername [16];
		strncpy_s(playername, 16, p_scraper->player_name(i).GetString(), _TRUNCATE);
		playerName[i] = playername;
	}
}
void CHandHistory::roundStart()
{
	double			bblind = p_symbols->sym()->bblind;
	int				userchair = (int) p_symbols->sym()->userchair;
	int				nchairs = (int)p_symbols->sym()->nchairs;
	int				dealerchair = (int) p_symbols->sym()->dealerchair;

	resetVars();

	//Records if there is no small blind
	if(p_symbols->sym()->currentbet[sblindpos]==0)
		SBfound=false;

	for(int i=1;i<=nchairs;i++)
	{
		int m = (i+userchair-1)%nchairs;
		if(isPlaying(m))
		{
			player[m].startBalance = player[m].currentBalance + player[m].currentBet;
			if(player[m].startBalance>=0)
			{
				player[m].dealt=true;
			}
		}
	}

	for(int i=0;i<nchairs;i++)
		if(player[i].currentBet==bblind&&i!=(bblindpos+1)%nchairs&&i!=bblindpos)
			player[i].postBlind=true;

	roundStarted=true;
}
void CHandHistory::checkBetround()
{
	double			rake = p_symbols->sym()->rake;
	int				userchair = (int) p_symbols->sym()->userchair;
	int				nchairs = (int) p_symbols->sym()->nchairs;
	int				dealerchair = (int) p_symbols->sym()->dealerchair;
	char			card_common[5][5];

	for(int i=0;i<5;i++)	//Set board card letters
		GetBCstring(card_common[i], p_scraper->card_common(i));

	for(int i=0;i<5;i++)
		if(card_common[i][1]!=NULL)
		{
			for(int j=0;j<5;j++)
				card_global[i][j]=card_common[i][j];
		}

		//Precondition: Has iterated through all players OR bblind has checked and hasn't been run and flop cards visible
		if((whosturn==((lpta+1)%nchairs)||allChecks[0]==true)
			&&flopSeen==false
			&&showdownReady==false
			&&showdownSeen==false
			&&card_common[0][1]!=NULL&&card_common[1][1]!=NULL&&card_common[2][1]!=NULL)
		{
			if(allChecks[0]==true) SetAction(bblindpos, 2, 0, 1);
			whosturn=postflopstart;
			lpta = -5;
			flopSeen=true;
			passChecks=false;
			maxBet=0;
			for(int i=0;i<nchairs;i++) player[i].actionCount=0;
		}
		//Precondition: Has iterated through all players OR players checked and hasn't been run and turn card visible
		if((whosturn==((lpta+1)%nchairs)||allChecks[1]==true)
			&&flopSeen==true
			&&turnSeen==false
			&&showdownReady==false
			&&showdownSeen==false
			&&card_common[3][1]!=NULL)
		{
			whosturn=postflopstart;
			lpta = -5;
			turnSeen=true;
			passChecks=false;
			maxBet=0;
			for(int i=0;i<nchairs;i++) player[i].actionCount=0;
		}
		//Precondition: Has iterated through all players OR players checked and hasn't been run and river card visible
		if((whosturn==((lpta+1)%nchairs)||allChecks[2]==true)
			&&turnSeen==true
			&&riverSeen==false
			&&showdownReady==false
			&&showdownSeen==false
			&&card_common[4][1]!=NULL)
		{
			whosturn=postflopstart;
			lpta = -5;
			riverSeen=true;
			passChecks=false;
			maxBet=0;
			for(int i=0;i<nchairs;i++) player[i].actionCount=0;
		}
		//Precondition: Has iterated through all players, hasn't been run, and is showdown
		if((whosturn==((lpta+1)%nchairs)||allChecks[3]==true)
			&&riverSeen==true
			&&showdownReady==false
			&&showdownSeen==false&&isShowdown())
		{
			showdownReady=true;
		}
}
void CHandHistory::scanPlayerChanges()
{
	double			bblind = p_symbols->sym()->bblind;
	double			potplayer = p_symbols->sym()->potplayer;
	int				betround = (int) p_symbols->sym()->betround;
	int				nchairs = (int) p_symbols->sym()->nchairs;
	int				raischair = (int) p_symbols->sym()->raischair;

	//Iterates through all chairs
	for(int i=0;i<nchairs;i++) 
	{
		//Precondition: It is the player's turn, and they are not past the
		//last to act
		if(whosturn==i&&whosturn!=(lpta+1)%nchairs)
		{
			//Precondition: Player is playing
			if(player[i].playersPlayingBits!=0)
			{
				//Prevents loops during first betround if utg called and dealer checked
				if(betround==1&&i==bblindpos&&lpta==-5)
					lpta = bblindpos;
				/* 
				Detects previous checks; if it is not preflop, and amount in the
				the player pot is not zero, checks have not been passed and someone has
				raised, then set the turn to the raiser
				*/
				if(betround!=1&&potplayer!=0&&passChecks!=true&&raischair!=postflopstart) 
				{
					whosturn=raischair;
					passChecks=true;
				}
				//Pass players who are allin
				if(allin[i]==true)
					whosturn=(whosturn+1)%nchairs;
				//Precondition: Player has raised
				else if(player[i].currentBet>maxBet)
				{
					//If there were players who checked, output checks
					if(betround!=1&&i!=postflopstart&&allChecks[betround-1]==true)
					{
						int j=postflopstart;
						checkSeats(i, j);
					}
					double temp = maxBet;
					maxBet = player[i].currentBet;

					if((int)p_symbols->sym()->lim!=2)checkAllin(i);

					if(allin[i]==true)
						SetAction(i, 3, (player[i].currentBet-player[i].prevprevBet), betround);
					else if(temp==0)
						SetAction(i, 3, player[i].currentBet, betround);
					else 
						SetAction(i, 3, (player[i].currentBet-player[i].prevprevBet), betround);

					lpta = (i-1)%nchairs;	//Set lpta to seat behind raiser
					if(lpta==-1)lpta=nchairs-1;
					whosturn=(whosturn+1)%nchairs;	//Increment whosturn
					allChecks[betround-1]=false;
					passChecks=true;
				} 
				//Precondition: Player has called
				else if(
					(player[i].currentBet==maxBet&&maxBet!=0&&!isBigBlind(i)&&
					!(player[i].postBlind==true&&player[i].currentBet==bblind&&betround==1))
					||
					(prevround!=betround&&betround!=1&&maxBet!=0))
				{
					if((int)p_symbols->sym()->lim!=2)
						checkAllin(i);

					if(allin[i]==true)
						SetAction(i, 2, player[i].prevBalance, prevround);
					else
						SetAction(i, 2, (maxBet-player[i].prevprevBet), prevround);

					whosturn=(whosturn+1)%nchairs;	//Increment whosturn
				}
				/* 
				Attempts to check players who have posted the blind early.
				Precondition: Betround one, player posted blinds early, and no one
				has raised
				*/
				else if(betround==1&&player[i].postBlind==true&&maxBet==bblind)
				{
					int nta=-5;	//Next to act, initialized to safe value
					int count=0;
					while (nta<0) //While next to act hasn't been found
					{
						int j=(i+count)%nchairs;
						if(isPlaying(j)==true)
							nta=j;	//If player is playing, set next to act
						else
							count++;
					}
					if(player[nta].currentBet!=0)
					{
						SetAction( i, 2, 0, betround);
						whosturn=(whosturn+1)%nchairs;	//Increment whosturn
					}
				}
			}
			//Precondition: Player has folded
			else if(!hasMucked(i))
			{
				//If they have not been reported as folding, output a fold
				if(player[i].seatIsPlaying==true&&isPlaying(i))
				{
					SetAction( i, 1, 0, prevround);
				}
				player[i].seatIsPlaying=false;
				whosturn=(whosturn+1)%nchairs;	//Increment whosturn
				int nplayersin=0;
				for(int i=0;i<nchairs;i++)
					if(player[i].seatIsPlaying==true) nplayersin++;

				//If there is only one player left, run outputUncontested function
				if(nplayersin==1)
				{
					for(int j=0;j<nchairs;j++)
					{
						if(player[j].seatIsPlaying==true)
						{
							SetAction(j, 4, 0, betround);
							if(showdownSeen==false)
							{
								showdownSeen=true;
								ReconstructHand(false);
							}
						}
					}
				}
			}
		}
	}
}
void CHandHistory::setPreviousActions()
{
	int				betround = (int) p_symbols->sym()->betround;
	int				nchairs = (int) p_symbols->sym()->nchairs;
	int				dealerchair = (int) p_symbols->sym()->dealerchair;

	prevdealerchair = dealerchair;
	prevround = betround;
	prevpot = pot;

	if(isShowdown())prevround = 5;

	for(int i=0;i<nchairs;i++) 
	{
		if(player[i].currentBalance!=0)
			player[i].prevBalance = player[i].currentBalance;
		player[i].prevprevBet = player[i].prevBet;
		player[i].prevBet = player[i].currentBet;
	}
}
const string CHandHistory::setDate()
{
	SYSTEMTIME			st;

	GetSystemTime(&st);
	stringstream ss;
	string s;
	ss<<st.wYear<<"-"<<st.wMonth<<"-"<<st.wDay<<" "<<st.wHour<<":"<<st.wMinute<<":"<<st.wSecond;
	s = ss.str()+" ";

	return s;
}
const bool CHandHistory::isShowdown()
{
	int				userchair = (int) p_symbols->sym()->userchair;
	int				nchairs = (int) p_symbols->sym()->nchairs;
	int				betround = (int) p_symbols->sym()->betround;

	//If a player other than the user has cards showing, it is showdown
	for(int i=0;i<nchairs;i++)
		if(player[i].card_player[1]!=NULL&&(i!=userchair||userchair==0)
			&&player[i].playersPlayingBits!=0&&betround==4)
			return true;

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
const int CHandHistory::DealPosition (const int chairnum)
{
	//DealPosition function used elsewhere in OH, haven't gotten around to accessing
	//the private function
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
	int				nchairs = (int)p_symbols->sym()->nchairs;
	int				nplayersplaying = (int) p_symbols->sym()->nplayersplaying;

	for(int i=0;i<nchairs;i++)
	{
		//Precondition: Player is playing, their cards haven't been processed yet
		//and the cards are visible
		if(player[i].playersPlayingBits!=0&&player[i].cardsSeen==false&&player[i].card_player[1]!=NULL)
		{
			stringstream ss;
			ss<<playerName[i]<<": Shows [ "
				<<player[i].card_player[1]
			<<player[i].card_player[0]<<" "
				<<player[i].card_player[3]
			<<player[i].card_player[2]<<" ]";
			handText[i] = ss.str()+" ";	//Stores hand output to be displayed later

			//Creates a complete hand mask for each player
			for(int j=0;j<2;j++)
				CardMask_SET(player[i].hand,p_scraper->card_player(i,j));
			for(int j=2;j<7;j++)
				CardMask_SET(player[i].hand,p_scraper->card_common(j-2));

			player[i].handval = Hand_EVAL_N(player[i].hand, 7);	//Evaluate cards

			pCardsSeen++;
			player[i].cardsSeen=true;	
		}
	}
	if(showdownReady==true&&showdownSeen==false)
	{
		//Precondition: All players' hole cards have been seen and
		//the showdown results haven't been displayed
		if(pCardsSeen==nplayersplaying&&showdownSeen==false)
		{
			showdownSeen=true;
			ReconstructHand(true);
		}
	}
}
const bool CHandHistory::isBigBlind(const int i)
{
	int			betround = (int) p_symbols->sym()->betround;

	//Determines if chair passed is the big blind
	return(player[i].currentBet==bet[betround-1]&&player[i].ac_dealpos==2&&betround==1);
}
const bool CHandHistory::cardsDealt()
{
	int				nchairs = (int) p_symbols->sym()->nchairs;
	int				playersdealt = 0;

	//If any cardbacks are showing, cards have been dealt
	for(int i=0;i<nchairs;i++)
		if(p_scraper->card_player(i, 0)==CARD_BACK&&p_scraper->card_player(i, 1)==CARD_BACK)
			playersdealt++;

	if(playersdealt>=2)
		return true;
	else 
		return false;
}
const string CHandHistory::findLimit()
{
	int				lim = (int) p_symbols->sym()->lim;
	string			str = "";

	if(lim==0)str="NL";
	else if(lim==1)str="PL";
	else if(lim==2)str="FL";
	return str;
}
const bool CHandHistory::isPlaying(const int i)
{
	int				dbits = (int) p_symbols->sym()->playersdealtbits;
	int				playersdealtbits = (dbits>>i)&1;

	if(playersdealtbits==1&&player[i].currentBalance>=0)
		return true;

	else 
		return false;
}
const bool CHandHistory::hasMucked(const int i)
{
	int				betround = (int) p_symbols->sym()->betround;

	//Precondition: Betround is 4, player has called and has not folded
	if(betround==4&&(player[i].currentBet==maxBet||player[i].currentBalance==(player[i].prevBalance-maxBet))&&player[i].seatIsPlaying==true)
		return true;

	return false;
}
void CHandHistory::checkSeats(const int i, int j)
{
	int				nchairs = (int) p_symbols->sym()->nchairs;

	do
	{
		if(player[j].playersPlayingBits!=0)
		{
			SetAction(j, 2, 0, prevround);
		}
		j=(j+1)%nchairs;
	}
	while(j!=i);
}
const double CHandHistory::getSB(const double i)
{
	if(i==.05) return .02;
	else if(i==.25) return .1;
	else return i/2;
}
void CHandHistory::checkAllin(const int i)
{
	int			betround = (int) p_symbols->sym()->betround;

	//Precondition: Player has gone allin, and this hasn't been caught yet
	if((player[i].currentBalance<=maxBet||player[i].currentBalance==0)&&allin[i]==false) 
		allin[i]=true;	
}
void CHandHistory::resetVars()
{
	double			bblind = p_symbols->sym()->bblind;
	int				nchairs = (int) p_symbols->sym()->nchairs;

	SBfound=true;
	newRoundFlag=false;
	flopSeen=false;
	turnSeen=false;
	riverSeen=false;
	showdownSeen=false;
	roundStarted=false;
	showdownReady=false;
	pCardsSeen = 0;
	prevround = 0;
	gameNumber++;
	maxBet=bblind;
	passChecks=false;
	lpta = -5;

	for(int i=0;i<4;i++)
	{
		allChecks[i]=true;
		bet[i] = p_symbols->sym()->bet[i];
	}
	for(int i=0;i<nchairs;i++)
	{
		player[i].ac_dealpos = DealPosition(i);
		if(player[i].ac_dealpos==1)sblindpos=i;
		else if(player[i].ac_dealpos==2)bblindpos=i;
		else if(player[i].ac_dealpos==3)utg=i;
		allin[i]=false;
		CardMask_RESET(player[i].hand);
		player[i].postBlind=false;
		player[i].seatIsPlaying=true;
		player[i].cardsSeen=false;
		player[i].dealt=false;
		player[i].hasFolded=false;
		player[i].handval=0;
		handText[i]="";
		player[i].prevBet=0;
		player[i].prevprevBet=0;
		player[i].bets=0;
		player[i].calls=0;
		player[i].actionCount=0;
		for(int j=0;j<4;j++)
		{
			for(int k=0;k<4;k++)
			{
				player[i].bet[j][k]=0;
				player[i].action[j][k]=0;
			}
		}
	}
	whosturn=utg;
	postflopstart=sblindpos;
}
void CHandHistory::SetAction(int pnum, int action, double amount, int br)
{
	/*CreateDirectory("handhistory",NULL);
	stringstream ss;
	ss<<"handhistory/handhistory_"<<p_sessioncounter->session_id()<<".txt";
	string s=ss.str()+" ";
	outfile.open(s.c_str(), fstream::app);*/

	int				count = player[pnum].actionCount;
	br--;

	if(br>3)
		br=3;
	else if(br<0)
		br=0;

	if(action==2)
		player[pnum].calls++;
	else if(action==3)
		player[pnum].bets++;

	player[pnum].action[br][count]=action;
	//outfile<<player[pnum].name<<" action: "<<action<<" br: "<<br<<" count: "<<count<<" prevround: "<<prevround<<endl;
	player[pnum].bet[br][count]=amount;

	if(player[pnum].actionCount<4)
		player[pnum].actionCount++;

	//outfile.close();
}
void CHandHistory::ReconstructHand(bool contested)
{
	double			rake = p_symbols->sym()->rake;
	double			bblind = p_symbols->sym()->bblind;
	int				userchair = (int) p_symbols->sym()->userchair;
	int				nchairs = (int) p_symbols->sym()->nchairs;
	int				dealerchair = (int) p_symbols->sym()->dealerchair;
	char			card_common[5][5];
	int				betround = 0;
	int				prevround = 0;
	int				wt = utg;

	for(int i=0;i<5;i++)	//Set board card letters
		GetBCstring(card_common[i], p_scraper->card_common(i));

	for(int i=0;i<nchairs;i++)
		player[i].actionCount=0;

	//Append to (or create if it does not exist) a handhistory file
	//using the session id as the name
	CreateDirectory("handhistory",NULL);
	stringstream ss;
	ss<<"handhistory/handhistory_"<<p_sessioncounter->session_id()<<".txt";
	string s=ss.str()+" ";
	outfile.open(s.c_str(), fstream::app);

	//------------------DEALER--------------------//
	outfile<<"\nGAME #"<<gameNumber<<": Texas Hold'em "<<findLimit()<<" $"<<bblind<<"/$"<<(bblind*2)<<" "<<setDate()<<endl;
	outfile<<"Table "<<p_scraper->title()<<endl;
	for(int i=0;i<nchairs;i++)
	{
		if(player[i].dealt)
		{
			outfile<<"Seat "<<(i+1)<<": "<<playerName[i]<<" ( $"<<player[i].startBalance<<" in chips)";
			if (i==dealerchair) outfile<<" DEALER"<<endl;
			else outfile<<endl;
		}
	}
	if(SBfound)
		outfile<<playerName[sblindpos]<<": Post SB $"<<getSB(bblind)<<endl;
	outfile<<playerName[bblindpos]<<": Post BB $"<<bblind<<endl;
	for(int i=0;i<nchairs;i++)
	{
		if(player[i].currentBet==bblind&&i!=(bblindpos+1)%nchairs&&i!=bblindpos)
		{
			player[i].postBlind=true;
			outfile<<playerName[i]<<": Post BB $"<<bblind<<endl;
		}
	}
	//------------------ACTIONS--------------------//
	outfile<<"*** HOLE CARDS ***"<<endl;
	if(player[userchair].dealt&&userchair!=0)
	{
		outfile<<"Dealt to "<<playerName[userchair]<<" [ "<<player[userchair].card_player[1]<<player[userchair].card_player[0]
		<<" "<<player[userchair].card_player[3]<<player[userchair].card_player[2]<<" ]"<<endl;
	}
	while(player[wt].action[betround][player[wt].actionCount]!=4&&betround<4)
	{
		string name = playerName[wt];
		int count = player[wt].actionCount;
		int action = player[wt].action[betround][count];
		double bet = player[wt].bet[betround][count];
		if(player[wt].dealt)
		{
			if(action==1)
			{
				outfile<<name<<": Fold"<<endl;
				player[wt].hasFolded=true;
			}
			else if(action==2)
				if(bet==0)
					outfile<<name<<": Check"<<endl;
				else
					outfile<<name<<": Call $"<<bet<<endl;
			else if(action==3)
				if(count==0)
					outfile<<name<<": Bet $"<<bet<<endl;
				else
					outfile<<name<<": Raise $"<<bet<<endl;
			else if(action==0)
			{
				if(!player[wt].hasFolded)
				{
					betround++;
					wt = postflopstart;
					for(int i=0;i<nchairs;i++) player[i].actionCount=0;
				}
			}
		}

		if(prevround!=betround)
		{
			if(betround==1)
				outfile<<"*** FLOP *** [ "
				<<card_global[0][1]<<card_global[0][0]<<" "
				<<card_global[1][1]<<card_global[1][0]<<" "
				<<card_global[2][1]<<card_global[2][0]<<" ]"<<endl;
			else if(betround==2)
				outfile<<"*** TURN *** [ "<<card_global[3][1]<<card_global[3][0]<<" ]"<<endl;
			else if(betround==3)
				outfile<<"*** RIVER *** [ "<<card_global[4][1]<<card_global[4][0]<<" ]"<<endl;
		}
		else
		{
			player[wt].actionCount++;
			wt = (wt+1)%nchairs;
		}
		prevround = betround;
	}
	//------------------SHOWDOWN--------------------//
	outfile<<"*** SUMMARY ***"<<endl;
		outfile<<"Total pot $"<<(prevpot-bet[betround])<<" Rake $"<<(rake*pot)<<endl;
	if(contested)
	{
		for(int i=0;i<nchairs;i++)
			if(handText[i]!="")
				outfile<<handText[i]<<endl;
		HandVal handhigh = {0};
		int highest=0;
		for(int i=0;i<nchairs;i++)
		{
			if(player[i].handval>handhigh)		//Find hand w/ highest value
			{
				handhigh=player[i].handval;
				highest = i;
			}
		}
		outfile<<playerName[highest]<<": wins $"<<(prevpot-bet[betround])<<" from the main pot with ";
		int currentType = Hand_EVAL_TYPE(player[highest].hand,7);	//Find hand type
		switch(currentType)
		{
		case StdRules_HandType_NOPAIR: outfile<<"no Pair."<<endl; break;
		case StdRules_HandType_ONEPAIR: outfile<<"One Pair."<<endl; break;
		case StdRules_HandType_TWOPAIR: outfile<<"Two Pair."<<endl; break;
		case StdRules_HandType_TRIPS: outfile<<"Trips."<<endl; break;
		case StdRules_HandType_STRAIGHT: outfile<<"a Straight."<<endl; break;
		case StdRules_HandType_FLUSH: outfile<<"a Flush."<<endl; break;
		case StdRules_HandType_FULLHOUSE: outfile<<"a Fullhouse."<<endl; break;
		case StdRules_HandType_QUADS: outfile<<"Quads."<<endl; break;
		case StdRules_HandType_STFLUSH: outfile<<"a Straight Flush."<<endl; break;
		}
	}
	else if(!contested)
	{
		for(int i=0;i<nchairs;i++)
			if(player[i].seatIsPlaying==true)
				outfile<<playerName[i]<<": wins $"<<(prevpot-bet[betround])<<endl;
	}
	outfile.close();
}