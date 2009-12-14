#include "stdafx.h"
#include "CHandHistory.h"

using namespace std;

CHandHistory *p_handhistory = NULL;

CHandHistory::CHandHistory()
{
	gameNumber = 83910;	//Temporary starting game number
	nchairs = 10;	//Initialized to max value, updated later
}
void CHandHistory::makeHistory()
{
	updateSymbols();

	//Append to (or create if it does not exist) a handhistory file
	//using the session id as the name
	stringstream ss;
	ss<<"handhistory_"<<p_sessioncounter->session_id()<<".txt";
	string s=ss.str()+" ";
	char filename[20];
	strcpy(filename, s.c_str());
	outfile.open(filename, fstream::app);

	//Precondition: New round
	if(prevdealerchair!=dealerchair&&betround==1) 
	{
		roundStart(); 
	}

	checkBetround();

	//Precondition: Cards have been dealt and the round summary has not
	//been printed
	if(betroundSet[5]==false&&betroundSet[7]==true)scanPlayerChanges();

	if(isShowdown())processShowdown();

	outfile.close();

	setPreviousActions();
}
void CHandHistory::updateSymbols()
{
	pot = p_symbols->sym()->pot;
	rake = p_symbols->sym()->rake;
	userchair = (int)p_symbols->sym()->userchair;
	cbits = (int)p_symbols->sym()->playersplayingbits;
	dbits = (int)p_symbols->sym()->playersdealtbits;
	betround = (int)p_symbols->sym()->betround;
	potplayer = p_symbols->sym()->potplayer;
	sblind = p_symbols->sym()->sblind;
	bblind = p_symbols->sym()->bblind;
	nplayersactive = (int) p_symbols->sym()->nplayersactive;
	nplayersplaying = (int) p_symbols->sym()->nplayersplaying;
	raischair = (int) p_symbols->sym()->raischair;
	dealerchair = (int) p_symbols->sym()->dealerchair;
	postflopstart=sblindpos;
	for(int i=0;i<4;i++)
		bet[i] = p_symbols->sym()->bet[i];
	for(int i=0;i<5;i++)	//Set board card letters
		GetBCstring(card_common[i], p_scraper->card_common(i));
	for(int i=0;i<nchairs;i++)	//Set individual player variables
	{
		GetPCstring(card_player[i], p_scraper->card_player(i,0), p_scraper->card_player(i,1));
		playersplayingbits[i]=(cbits>>i)&1;
		playersdealtbits[i]=(dbits>>i)&1;
		currentbetx[i]= p_symbols->sym()->currentbet[i];
		playerbalance[i]= p_symbols->sym()->balance[i];
		strncpy_s(playername, 16, p_scraper->player_name(i).GetString(), _TRUNCATE);
		splayername[i] = playername;
	}

}
void CHandHistory::roundStart()
{
	nchairs = (int)p_symbols->sym()->nchairs;
	pCardsSeen = 0;
	gameNumber++;
	maxBet=bblind;
	passChecks=1;
	lpta = -5;
	int utg;
	for(int i=0;i<8;i++)betroundSet[i]=false;
	for(int i=0;i<4;i++)allChecks[i]=true;
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
		//Precondition: Player chair has been dealt
		if(playersdealtbits[m]==1)
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
	//Precondition: Has not been run, user cards visible
	if(betroundSet[0]==false&&card_player[userchair][0]!=NULL&&card_player[userchair][1]!=NULL)
	{
		outfile<<"*** HOLE CARDS ***"<<endl;
		outfile<<"Dealt to "<<splayername[4]<<" [ "<<card_player[userchair][1]<<card_player[userchair][0]
		<<" "<<card_player[userchair][3]<<card_player[userchair][2]<<" ]"<<endl;
		passChecks=0;
		betroundSet[0]=true;
		lpta = -5;
	}
	//Precondition: Has iterated through all players OR bblind has checked and hasn't been run and flop cards visible
	if((whosturn==((lpta+1)%nchairs)||allChecks[0]==true)
		&&betroundSet[1]==false&&card_common[0][1]!=NULL&&card_common[1][1]!=NULL&&card_common[2][1]!=NULL)
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
		&&betroundSet[1]==true&&betroundSet[2]==false&&card_common[3][1]!=NULL)
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
		&&betroundSet[2]==true&&betroundSet[3]==false&&card_common[4][1]!=NULL)
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
		&&betroundSet[3]==true&&betroundSet[4]==false&&betroundSet[6]==false&&isShowdown())
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
	//Iterates through all chairs
	for(int i=0;i<nchairs;i++) 
	{
		//Precondition: It is the player's turn, and they are not past the
		//last to act
		if(whosturn==i&&whosturn!=(lpta+1)%10)
		{
			//Precondition: Player is playing
			if(playersplayingbits[i]!=0)
			{
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
				//Precondition: Player has raised
				if(currentbetx[i]>maxBet)
				{
					//If there were players who checked, output checks
					if(betround!=1&&i!=postflopstart&&allChecks[betround-1]==true)
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
					lpta = (i-1)%nchairs;	//Set lpta to seat behind raiser
					if(lpta==-1)lpta=nchairs-1;
					whosturn=(whosturn+1)%nchairs;	//Increment whosturn
					allChecks[betround-1]=false;
					maxBet = currentbetx[i];
					passChecks=1;
				}
				//Precondition: Player has called
				else if(
				(currentbetx[i]==maxBet&&maxBet!=0&&!isBigBlind(i))
				||
				(prevround!=betround&&betround!=1&&maxBet!=0)
				)
				{
					outfile<<splayername[i]<<": Call $"<<maxBet<<endl;
					whosturn=(whosturn+1)%nchairs;	//Increment whosturn
				}
			}
			//Precondition: Player is not playing
			else
			{
				//If they have not been reported as folding, output a fold
				if(seatsPlaying[i]==true&&playersdealtbits[i]==1)
					outfile<<splayername[i]<<": Fold"<<endl;
				seatsPlaying[i]=false;
				whosturn=(whosturn+1)%nchairs;	//Increment whosturn
				int nplayersin=0;
				for(int i=0;i<nchairs;i++)
				{
					if(seatsPlaying[i]==true) nplayersin++;
				}
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
	prevdealerchair = dealerchair;
	prevround = betround;
	prevpot = pot;
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
	//If a player other than the user has cards showing, it is showdown
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
			outfile<<splayername[highest]<<": wins $"<<prevpot<<" from the main pot with ";
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
void CHandHistory::outputUncontested(int j)
{
	if(betroundSet[6]==false)
	{
		outfile<<"*** SUMMARY ***"<<endl;
		outfile<<"Total pot $"<<prevpot<<" Rake $"<<(rake*prevpot)<<endl;
		outfile<<splayername[j]<<": wins $"<<prevpot<<endl;
		betroundSet[6]=true;
	}
}
bool CHandHistory::isBigBlind(int i)
{
	//Determines if chair passed is the big blind
	return(currentbetx[i]==bet[betround-1]&&ac_dealpos[i]==2&&betround==1);
}
bool CHandHistory::cardsDealt()
{
	//If any cardbacks are showing, cards have been dealt
	for(int i=0;i<nchairs;i++)
		if(p_scraper->card_player(i, 0)==CARD_BACK) return true;

	return false;
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