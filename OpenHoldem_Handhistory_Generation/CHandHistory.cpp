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
	for(int i=0;i<9;i++)betroundSet[i]=false;
	for(int i=0;i<10;i++)playerbalance[i]=0;
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

	//Append to (or create if it does not exist) a handhistory file
	//using the session id as the name
	CreateDirectory("handhistory",NULL);
	stringstream ss;
	ss<<"handhistory/handhistory_"<<p_sessioncounter->session_id()<<".txt";
	string s=ss.str()+" ";
	outfile.open(s.c_str(), fstream::app);

	//Precondition: New round
	if(prevdealerchair!=dealerchair&&betround==1) {
		newRoundFlag=true;
		for(int i=0;i<9;i++)betroundSet[i]=false;
	}
	//Precondition: New round flag has been set and cards dealt
	if(newRoundFlag==true&&betroundSet[7]==true)
		roundStart();
	checkBetround();

	//Precondition: Cards have been dealt and the round summary has not
	//been printed
	if(betroundSet[5]==false&&
		betroundSet[6]==false&&
		betroundSet[8]==true)scanPlayerChanges();

	if(isShowdown())processShowdown();

	outfile.close();
}
void CHandHistory::updateSymbols()
{
	int				nchairs = (int)p_symbols->sym()->nchairs;
	int				betround = (int) p_symbols->sym()->betround;
	int				cbits = (int) p_symbols->sym()->playersplayingbits;
	int				dbits = (int) p_symbols->sym()->playersdealtbits;

	if (p_symbols->sym()->pot!=0||betround<=1)
		pot = p_symbols->sym()->pot;
	postflopstart=sblindpos;
	for(int i=0;i<4;i++)
		bet[i] = p_symbols->sym()->bet[i];

	for(int i=0;i<nchairs;i++)	//Set individual player variables
	{
		GetPCstring(card_player[i], p_scraper->card_player(i,0), p_scraper->card_player(i,1));
		playersplayingbits[i]=(cbits>>i)&1;
		playersdealtbits[i]=(dbits>>i)&1;
		//If the player is playing, update the symbol. This condition used for muck detection.
		if(playersplayingbits[i]!=0)
			currentbetx[i]= p_symbols->sym()->currentbet[i];
		playerbalance[i]= p_symbols->sym()->balance[i];
		char	playername [16];
		strncpy_s(playername, 16, p_scraper->player_name(i).GetString(), _TRUNCATE);
		splayername[i] = playername;
	}
}
void CHandHistory::roundStart()
{
	double			bblind = p_symbols->sym()->bblind;
	int				userchair = (int) p_symbols->sym()->userchair;
	int				nchairs = (int)p_symbols->sym()->nchairs;
	int				dealerchair = (int) p_symbols->sym()->dealerchair;
	bool			SBfound = true;

	resetVars();
	//Records if there is no small blind
	if(p_symbols->sym()->currentbet[sblindpos]==0)
		SBfound=false;

	outfile<<"\nGAME #"<<gameNumber<<": Texas Hold'em "<<findLimit()<<" $"<<bblind<<"/$"<<(bblind*2)<<" "<<setDate()<<endl;
	outfile<<"Table "<<p_scraper->title()<<endl;
	for(int i=1;i<=nchairs;i++)
	{
		int m = (i+userchair-1)%nchairs;
		//Precondition: Player chair has been dealt
		if(isPlaying(m))
		{
			double balance = playerbalance[m] + currentbetx[m];
			if(balance>=0)
			{
				outfile<<"Seat "<<i<<": "<<splayername[m]<<" ( $"<<balance<<" in chips)";
				if (m==dealerchair) outfile<<" DEALER"<<endl;
				else outfile<<endl;
			}
		}
	}
	if(SBfound)
		outfile<<splayername[sblindpos]<<": Post SB $"<<getSB(bblind)<<endl;
	outfile<<splayername[bblindpos]<<": Post BB $"<<bblind<<endl;
	for(int i=0;i<nchairs;i++)
		if(currentbetx[i]==bblind&&i!=(bblindpos+1)%nchairs&&i!=bblindpos)
		{
			postBlind[i]=true;
			outfile<<splayername[i]<<": Post BB $"<<bblind<<endl;
		}
	outfile<<"*** HOLE CARDS ***"<<endl;
	betroundSet[8]=true;
}
void CHandHistory::checkBetround()
{
	double		rake = p_symbols->sym()->rake;
	int			userchair = (int) p_symbols->sym()->userchair;
	int			nchairs = (int) p_symbols->sym()->nchairs;
	char		card_common[5][5];

	for(int i=0;i<5;i++)	//Set board card letters
		GetBCstring(card_common[i], p_scraper->card_common(i));

	//Precondition: Has not been run, user cards visible
	if(betroundSet[0]==false&&betroundSet[8]==true&&userchair!=0&&
		card_player[userchair][0]!=NULL&&card_player[userchair][1]!=NULL)
	{
		outfile<<"Dealt to "<<splayername[userchair]<<" [ "<<card_player[userchair][1]<<card_player[userchair][0]
		<<" "<<card_player[userchair][3]<<card_player[userchair][2]<<" ]"<<endl;
		passChecks=0;
		betroundSet[0]=true;
		lpta = -5;
	}
	//Precondition: Has iterated through all players OR bblind has checked and hasn't been run and flop cards visible
	if((whosturn==((lpta+1)%nchairs)||allChecks[0]==true)
		&&betroundSet[1]==false
		&&betroundSet[4]==false
		&&betroundSet[6]==false
		&&card_common[0][1]!=NULL&&card_common[1][1]!=NULL&&card_common[2][1]!=NULL)
	{
		if(allChecks[0]==true) outfile<<splayername[bblindpos]<<": Check"<<endl;
		outfile<<"*** FLOP *** [ "
			<<card_common[0][1]<<card_common[0][0]<<" "
			<<card_common[1][1]<<card_common[1][0]<<" "
			<<card_common[2][1]<<card_common[2][0]<<" ]"<<endl;
		whosturn=postflopstart;
		lpta = -5;
		betroundSet[1]=true;
		passChecks=0;
		maxBet=0;
	}
	//Precondition: Has iterated through all players OR players checked and hasn't been run and turn card visible
	if((whosturn==((lpta+1)%nchairs)||allChecks[1]==true)
		&&betroundSet[1]==true
		&&betroundSet[2]==false
		&&betroundSet[4]==false
		&&betroundSet[6]==false
		&&card_common[3][1]!=NULL)
	{
		outfile<<"*** TURN *** [ "<<card_common[3][1]<<card_common[3][0]<<" ]"<<endl;
		whosturn=postflopstart;
		lpta = -5;
		betroundSet[2]=true;
		passChecks=0;
		maxBet=0;
	}
	//Precondition: Has iterated through all players OR players checked and hasn't been run and river card visible
	if((whosturn==((lpta+1)%nchairs)||allChecks[2]==true)
		&&betroundSet[2]==true
		&&betroundSet[3]==false
		&&betroundSet[4]==false
		&&betroundSet[6]==false
		&&card_common[4][1]!=NULL)
	{
		outfile<<"*** RIVER *** [ "<<card_common[4][1]<<card_common[4][0]<<" ]"<<endl;
		whosturn=postflopstart;
		lpta = -5;
		betroundSet[3]=true;
		passChecks=0;
		maxBet=0;
	}
	//Precondition: Has iterated through all players, hasn't been run, and is showdown
	if((whosturn==((lpta+1)%nchairs)||allChecks[3]==true)
		&&betroundSet[3]==true
		&&betroundSet[4]==false
		&&betroundSet[6]==false&&isShowdown())
	{
		outfile<<"*** SUMMARY ***"<<endl;
		outfile<<"Total pot $"<<pot<<" Rake $"<<(rake*pot)<<endl;
		betroundSet[4]=true;
	}
	//Precondition: Cards have been dealt out
	if(betroundSet[7]==false&&cardsDealt())
	{
		betroundSet[7]=true;
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
			if(playersplayingbits[i]!=0)
			{
				//Prevents loops during first betround if utg called and dealer checked
				if(betround==1&&i==bblindpos&&lpta==-5)
					lpta = bblindpos;
				/* 
				Detects previous checks; if it is not preflop, and amount in the
				the player pot is not zero, checks have not been passed and someone has
				raised, then set the turn to the raiser
				*/
				if(betround!=1&&potplayer!=0&&passChecks!=1&&raischair!=postflopstart) 
				{
					whosturn=raischair;
					passChecks=1;
				}
				//Pass players who are allin
				if(allin[i]==true)
					whosturn=(whosturn+1)%nchairs;
				//Precondition: Player has raised
				else if(currentbetx[i]>maxBet)
				{
					//If there were players who checked, output checks
					if(betround!=1&&i!=postflopstart&&allChecks[betround-1]==true)
					{
						int j=postflopstart;
						checkSeats(i, j);
					}
					double temp = maxBet;
					maxBet = currentbetx[i];
					playerbet[i][betround-1]=maxBet;
					if((int)p_symbols->sym()->lim!=2)potUpdate(i);
					if(allin[i]==true)
						outfile<<splayername[i]<<": Allin $"<<(currentbetx[i]-prevprevbetx[i])<<endl;
					else if(temp==0)
						if(currentbetx[i]!=prevbetx[i])
							outfile<<splayername[i]<<": Bet $"<<(currentbetx[i]-prevbetx[i])<<endl;
						else
							outfile<<splayername[i]<<": Bet $"<<(currentbetx[i]-prevprevbetx[i])<<endl;
					else 
						outfile<<splayername[i]<<": Raise $"<<(currentbetx[i]-prevprevbetx[i])<<endl;
					lpta = (i-1)%nchairs;	//Set lpta to seat behind raiser
					if(lpta==-1)lpta=nchairs-1;
					whosturn=(whosturn+1)%nchairs;	//Increment whosturn
					allChecks[betround-1]=false;
					passChecks=1;
				} 
				//Precondition: Player has called
				else if(
				(currentbetx[i]==maxBet&&maxBet!=0&&!isBigBlind(i)&&
				!(postBlind[i]==true&&currentbetx[i]==bblind&&betround==1))
				||
				(prevround!=betround&&betround!=1&&maxBet!=0))
				{
					if((int)p_symbols->sym()->lim!=2)potUpdate(i);
					if(allin[i]==true)
						outfile<<splayername[i]<<": Allin $"<<prevplayerbalance[i]<<endl;
					else
						outfile<<splayername[i]<<": Call $"<<(maxBet-prevprevbetx[i])<<endl;
					playerbet[i][betround-1]=maxBet;
					whosturn=(whosturn+1)%nchairs;	//Increment whosturn
				}
				/* 
				Attempts to check players who have posted the blind early.
				Precondition: Betround one, player posted blinds early, and no one
				has raised
				*/
				else if(betround==1&&postBlind[i]==true&&maxBet==bblind)
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
					if(currentbetx[nta]!=0)
					{
						outfile<<splayername[i]<<": Check"<<endl;
						whosturn=(whosturn+1)%nchairs;	//Increment whosturn
					}
				}
			}
			//Precondition: Player has folded
			else if(!hasMucked(i))
			{
				//If they have not been reported as folding, output a fold
				if(seatsPlaying[i]==true&&isPlaying(i))
					outfile<<splayername[i]<<": Fold"<<endl;
				seatsPlaying[i]=false;
				whosturn=(whosturn+1)%nchairs;	//Increment whosturn
				int nplayersin=0;
				for(int i=0;i<nchairs;i++)
					if(seatsPlaying[i]==true) nplayersin++;

				//If there is only one player left, run outputUncontested function
				if(nplayersin==1)
					for(int j=0;j<nchairs;j++)
						if(seatsPlaying[j]==true)
							outputUncontested(j);
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
		if(playerbalance[i]!=0)
			prevplayerbalance[i] = playerbalance[i];
		prevprevbetx[i] = prevbetx[i];
		prevbetx[i] = currentbetx[i];
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

	//If a player other than the user has cards showing, it is showdown
	for(int i=0;i<nchairs;i++)
		if(card_player[i][1]!=NULL&&(i!=userchair||userchair==0)&&playersplayingbits[i]!=0)
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

	for(int i=0;i<nchairs;i++)
	{
		//Precondition: Player is playing, their cards haven't been processed yet
		//and the cards are visible
		if(playersplayingbits[i]!=0&&cardsSeen[i]==false&&card_player[i][1]!=NULL)
		{
			stringstream ss;
			ss<<splayername[i]<<": Shows [ "
			<<card_player[i][1]
			<<card_player[i][0]<<" "
			<<card_player[i][3]
			<<card_player[i][2]<<" ]";
			handText[i] = ss.str()+" ";	//Stores hand output to be displayed later

			//Creates a complete hand mask for each player
			for(int j=0;j<2;j++)
				CardMask_SET(hand[i],p_scraper->card_player(i,j));
			for(int j=2;j<7;j++)
				CardMask_SET(hand[i],p_scraper->card_common(j-2));
			
			handval[i] = Hand_EVAL_N(hand[i], 7);	//Evaluate cards

			pCardsSeen++;
			cardsSeen[i]=true;	
		}
	}
	if(betroundSet[4]==true&&betroundSet[6]==false)
		showdownResults();
}
void CHandHistory::showdownResults()
{ 
	int				nchairs = (int) p_symbols->sym()->nchairs;
	int				nplayersplaying = (int) p_symbols->sym()->nplayersplaying;

	//Precondition: All players' hole cards have been seen and
	//the showdown results haven't been displayed
	if(pCardsSeen==nplayersplaying&&betroundSet[5]==false)
	{
		for(int i=0;i<nchairs;i++)
			if(handText[i]!="")
				outfile<<handText[i]<<endl;

		HandVal handhigh = {0};
		int highest=0;
		for(int i=0;i<nchairs;i++)
			if(handval[i]>handhigh)		//Find hand w/ highest value
			{
				handhigh=handval[i];
				highest = i;
			}
			outfile<<splayername[highest]<<": wins $"<<pot<<" from the main pot with ";
		int currentType = Hand_EVAL_TYPE(hand[highest],7);	//Find hand type
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
void CHandHistory::outputUncontested(const int j)
{
	double			rake = p_symbols->sym()->rake;

	if(betroundSet[6]==false)
	{
		outfile<<"*** SUMMARY ***"<<endl;
		outfile<<"Total pot $"<<prevpot<<" Rake $"<<(rake*prevpot)<<endl;
		outfile<<splayername[j]<<": wins $"<<prevpot<<endl;
		betroundSet[6]=true;
	}
}
const bool CHandHistory::isBigBlind(const int i)
{
	int			betround = (int) p_symbols->sym()->betround;

	//Determines if chair passed is the big blind
	return(currentbetx[i]==bet[betround-1]&&ac_dealpos[i]==2&&betround==1);
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
	if(playersdealtbits[i]==1&&playerbalance[i]>=0)
		return true;

	else 
		return false;
}
const bool CHandHistory::hasMucked(const int i)
{
	int				betround = (int) p_symbols->sym()->betround;

	//Precondition: Betround is 4, player has called and has not folded
	if(betround==4&&(currentbetx[i]==maxBet||playerbalance[i]==(prevplayerbalance[i]-maxBet))&&seatsPlaying[i]==true)
		return true;

	return false;
}
void CHandHistory::checkSeats(const int i, int j)
{
	int				nchairs = (int) p_symbols->sym()->nchairs;

	while(j!=i)
	{
		if(playersplayingbits[j]!=0)outfile<<splayername[j]<<": Check"<<endl;
		j=(j+1)%nchairs;
	}
}
const double CHandHistory::getSB(const double i)
{
	if(i==.05) return .02;
	else if(i==.25) return .1;
	else return i/2;
}
void CHandHistory::potUpdate(const int i)
{
	int			betround = (int) p_symbols->sym()->betround;

	//Precondition: Player has gone allin, and this hasn't been caught yet
	if((playerbalance[i]<=maxBet||playerbalance[i]==0)&&allin[i]==false) 
	{
		//Flags current betting round
		allinflag[betround-1]=true;
		//Amount to call=current bet of allin player
		allincall[betround-1]=currentbetx[i];	
		activepot[potnum]=pot;	//Set current pot to pot
		potnum++;		//Advance pots
		allin[i]=true;	
	}
	if((allinflag[prevround-1]==true||allinflag[betround-1]==true)&&allin[i]==false)
	{
		if(currentbetx[i]>0)
		{
			if(currentbetx[i]<=allincall[betround-1])
				//Add to previous pot
				activepot[potnum-1]+=currentbetx[i]-prevbetx[i];
			else if(currentbetx[i]>allincall[betround-1])
			{
				//Add to previous pot
				activepot[potnum-1]+=allincall[betround-1];
				//Add to current pot
				activepot[potnum]+=currentbetx[i]-allincall[betround-1];
			}
		}
		else
			activepot[potnum-1]+=maxBet-playerbet[i][prevround];
	}
	//Active pot= Total pot - other pots
	activepot[potnum]=pot;
	for(int i=1;i<(potnum+1);i++)
		activepot[potnum]-=activepot[potnum-i];
}
void CHandHistory::resetVars()
{
	double			bblind = p_symbols->sym()->bblind;
	int				nchairs = (int) p_symbols->sym()->nchairs;
	int				utg = 0;

	newRoundFlag=false;
	pCardsSeen = 0;
	gameNumber++;
	maxBet=bblind;
	passChecks=1;
	lpta = -5;
	potnum = 0;
	for(int i=0;i<4;i++)
	{
		allChecks[i]=true;
		activepot[i]=0;
		allincall[i]=0;
		allinflag[i]=false;
	}
	for(int i=0;i<nchairs;i++)
	{
		for(int j=0;j<4;j++)
			playerbet[i][j]=0;
		ac_dealpos[i] = DealPosition(i);
		if(ac_dealpos[i]==1)sblindpos=i;
		else if(ac_dealpos[i]==2)bblindpos=i;
		else if(ac_dealpos[i]==3)utg=i;
		allin[i]=false;
		postBlind[i]=false;
		seatsPlaying[i]=true;
		handval[i]=0;
		CardMask_RESET(hand[i]);
		cardsSeen[i]=false;
		handText[i]="";
		prevbetx[i]=0;
		prevprevbetx[i]=0;
	}
	whosturn=utg;
}
const bool CHandHistory::NothingChanged()
{
	int				betround = (int) p_symbols->sym()->betround;
	int				nchairs = (int) p_symbols->sym()->nchairs;
	int				dealerchair = (int) p_symbols->sym()->dealerchair;
	bool			same = true;

	for(int i=0;i<nchairs;i++)
		if(prevplayerbalance[i]!=playerbalance[i]||prevbetx[i]!=currentbetx[i])
			same = false;

	if(pot!=prevpot||betround!=prevround||prevdealerchair!=dealerchair)
		same = false;

	return same;
}