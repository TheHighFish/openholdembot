#include "stdafx.h"
#include "CHandHistory.h"

#include <tchar.h> 
#include <Windows.h>
#include <sstream>
#include <iomanip>
#include "CSymbols.h"
#include "CScraper.h"
#include "CGameState.h"
#include "CSessionCounter.h"
#include "MagicNumbers.h"
#include "OpenHoldem.h"
#include "poker_defs.h"

#include "enumerate.h"
#include "inlines/eval.h"
#include "inlines/eval_type.h"

using namespace std;


CHandHistory *p_handhistory = NULL;


CHandHistory::CHandHistory()
{
	// Attempt to create a random starting game number
	unsigned long seconds = time(NULL);
	gameNumber = (100000000 * p_sessioncounter->session_id()) + seconds/8; 

	newRoundFlag = false;
	for (int i=0; i<k_max_number_of_players; i++)
	{
		_history.chair[i].currentBalance = 0;
	}
}


void CHandHistory::MakeHistory()
{
	UpdateSymbols();
	WriteHistory();
	SetPreviousActions();
}


void CHandHistory::WriteHistory()
{
	int	betround    = (int) p_symbols->sym()->betround;
	int	dealerchair = (int) p_symbols->sym()->dealerchair;

	// !!! this should depend on OHs handresetmethod
	// !!! A change in handsplayed should be required,
	// !!! as dealerchair might stay the same.
	// Precondition: New round
	if (prevdealerchair != dealerchair && betround==1)
	{
		newRoundFlag = true;
	}
	// Precondition: New round flag has been set and cards dealt
	if (newRoundFlag == true && cardsDealt() == true)
	{
		roundStart();
	}
	checkBetround();
	// Precondition: Cards have been dealt and the round summary has not
	// been printed
	if (showdownSeen == false && roundStarted == true)
	{
		scanPlayerChanges(); 
	}
	if (isShowdown())
	{
		processShowdown();
	}
}
void CHandHistory::UpdateSymbols()
{
	int	nchairs   = (int) p_symbols->sym()->nchairs;
	int	betround  = (int) p_symbols->sym()->betround;
	int	userchair = (int) p_symbols->sym()->userchair;
	int	players_playing_bits = (int) p_symbols->sym()->playersplayingbits; 
	

	// Reset pot
	if ((p_symbols->sym()->pot >= pot) || (betround <= k_betround_preflop))
	{
		pot = p_symbols->sym()->pot;
	}
	//Set individual player variables
	for (int i=0; i<nchairs; i++)	
	{
		if ((p_scraper->card_player(i, 0) != CARD_NOCARD)
			|| ((i != userchair) && (userchair != 0)))
		{
			GetPCstring(_history.chair[i].card_player, p_scraper->card_player(i,0), p_scraper->card_player(i,1));
		}

		_history.chair[i].playersPlayingBits = (players_playing_bits >> i) & 1;

		// If the player is playing, update the symbol. 
		// This condition gets used for muck detection.
		if(_history.chair[i].playersPlayingBits != 0)
		{
			_history.chair[i].currentBet = p_symbols->sym()->currentbet[i];
		}
		_history.chair[i].currentBalance = p_symbols->sym()->balance[i];

		char	playername [16]; // !!! Get rid of hard-coded constant
		strncpy_s(playername, 16, p_scraper->player_name(i).GetString(), 
			_TRUNCATE);
		playerName[i] = playername;
	}
}


void CHandHistory::roundStart()
{
	double			bblind = p_tablelimits->bblind();
	int				userchair = (int) p_symbols->sym()->userchair;
	int				nchairs = (int)p_symbols->sym()->nchairs;
	int				dealerchair = (int) p_symbols->sym()->dealerchair;

	resetVars();

	//Records if there is no small blind
	if(p_symbols->sym()->currentbet[_history.sblindpos]==0)
		SBfound=false;
	else
	{
		_history.chair[_history.sblindpos].totalIn[0]=getSB(bblind);
	}

	//Assign put value of bblind in total pot
	_history.chair[_history.bblindpos].totalIn[0]=bblind;

	for(int i=1;i<=nchairs;i++)
	{
		int m = (i+userchair-1)%nchairs;
		if(isPlaying(m))
		{
			_history.chair[m].startBalance = _history.chair[m].currentBalance + _history.chair[m].currentBet;
			if(_history.chair[m].startBalance>=0)
			{
				_history.chair[m].dealt=true;
			}
		}
	}

	for(int i=0;i<nchairs;i++)
		if(_history.chair[i].currentBet==bblind&&i!=(_history.bblindpos+1)%nchairs&&i!=_history.bblindpos)
		{
			_history.chair[i].postBlind=true;
			_history.chair[i].totalIn[0]=bblind;
		}

	roundStarted=true;
}
void CHandHistory::checkBetround()
{
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
		if((_history.whosturn==((_history.lpta+1)%nchairs)||allChecks[0]==true)
			&&flopSeen==false
			&&showdownReady==false
			&&showdownSeen==false
			&&card_common[0][1]!=NULL&&card_common[1][1]!=NULL&&card_common[2][1]!=NULL)
		{
			if(allChecks[0]==true) SetAction(_history.bblindpos, 2, 0, 1);
			_history.whosturn=postflopstart;
			_history.lpta = -5; // !!! WTF is lpta?
			flopSeen=true;
			passChecks=false;
			maxBet=0;
			for(int i=0;i<nchairs;i++)
			{
				_history.chair[i].actionCount=0;
				middleBet[i]=0;
				_history.chair[i].prevBet=0;
			}
		}
		//Precondition: Has iterated through all players OR players checked and hasn't been run and turn card visible
		if((_history.whosturn==((_history.lpta+1)%nchairs)||allChecks[1]==true)
			&&flopSeen==true
			&&turnSeen==false
			&&showdownReady==false
			&&showdownSeen==false
			&&card_common[3][1]!=NULL)
		{
			_history.whosturn=postflopstart;
			_history.lpta = -5;
			turnSeen=true;
			passChecks=false;
			maxBet=0;
			for(int i=0;i<nchairs;i++)
			{
				_history.chair[i].actionCount=0;
				middleBet[i]=0;
				_history.chair[i].prevBet=0;
			}
		}
		//Precondition: Has iterated through all players OR players checked and hasn't been run and river card visible
		if((_history.whosturn==((_history.lpta+1)%nchairs)||allChecks[2]==true)
			&&turnSeen==true
			&&riverSeen==false
			&&showdownReady==false
			&&showdownSeen==false
			&&card_common[4][1]!=NULL)
		{
			_history.whosturn=postflopstart;
			_history.lpta = -5;
			riverSeen=true;
			passChecks=false;
			maxBet=0;
			for(int i=0;i<nchairs;i++)
			{
				_history.chair[i].actionCount=0;
				middleBet[i]=0;
				_history.chair[i].prevBet=0;
			}
		}
		//Precondition: Has iterated through all players, hasn't been run, and is showdown
		if((_history.whosturn==((_history.lpta+1)%nchairs)||allChecks[3]==true)
			&&riverSeen==true
			&&showdownReady==false
			&&showdownSeen==false&&isShowdown())
		{
			showdownReady=true;
		}
}
void CHandHistory::scanPlayerChanges()
{
	double			bblind = p_tablelimits->bblind();
	double			potplayer = p_symbols->sym()->potplayer;
	int				betround = (int) p_symbols->sym()->betround;
	int				nchairs = (int) p_symbols->sym()->nchairs;
	int				raischair = (int) p_symbols->sym()->raischair;

	//Iterates through all chairs
	for(int i=0;i<nchairs;i++) 
	{
		//Precondition: It is the player's turn, and they are not past the
		//last to act
		if(_history.whosturn==i&&_history.whosturn!=(_history.lpta+1)%nchairs)
		{
			//Precondition: Player is playing
			if(_history.chair[i].playersPlayingBits!=0)
			{
				//Prevents loops during first betround if _history.utg called and dealer checked
				if(betround==1&&i==_history.bblindpos&&_history.lpta==-5)
					_history.lpta = _history.bblindpos;
				/* 
				Detects previous checks; if it is not preflop, and amount in the
				the player pot is not zero, checks have not been passed and someone has
				raised, then set the turn to the raiser
				*/
				if(betround!=1&&potplayer!=0&&passChecks!=true&&raischair!=postflopstart) 
				{
					_history.whosturn=raischair;
					passChecks=true;
				}
				//Pass players who are allin
				if(allin[i]==true)
					_history.whosturn=(_history.whosturn+1)%nchairs;
				//Precondition: Player has raised
				else if(_history.chair[i].currentBet>maxBet)
				{
					//If there were players who checked, output checks
					if(betround!=1&&i!=postflopstart&&allChecks[betround-1]==true)
					{
						int j=postflopstart;
						checkSeats(i, j);
					}
					double temp = maxBet;
					maxBet = _history.chair[i].currentBet;

					if((int)p_tablelimits->gametype()!=2)checkAllin(i);

					if(allin[i]==true)
						SetAction(i, 3, (_history.chair[i].currentBet-_history.chair[i].prevBet), betround);
					else if(temp==0)
						SetAction(i, 3, _history.chair[i].currentBet, betround);
					else 
						SetAction(i, 3, (_history.chair[i].currentBet-_history.chair[i].totalIn[betround-1]), betround);

					_history.lpta = (i-1)%nchairs;	//Set _history.lpta to seat behind raiser
					if(_history.lpta==-1)_history.lpta=nchairs-1;
					_history.whosturn=(_history.whosturn+1)%nchairs;	//Increment _history.whosturn
					allChecks[betround-1]=false;
					passChecks=true;
				} 
				//Precondition: Player has called
				else if(
					(_history.chair[i].currentBet==maxBet&&maxBet!=0&&!isBigBlind(i)&&
					!(_history.chair[i].postBlind==true&&_history.chair[i].currentBet==bblind&&betround==1))
					||
					(prevround!=betround&&betround!=1&&maxBet!=0))
				{
					if((int)p_tablelimits->gametype()!=2)
						checkAllin(i);

					if(allin[i]==true)
						SetAction(i, 2, _history.chair[i].prevBalance, prevround);
					else
					{
						int br = prevround -1;
const int k_betround_turn		= 3;
						if(br>k_betround_turn)
							br=k_betround_turn;
						SetAction(i, 2, (maxBet-_history.chair[i].totalIn[br]), prevround);
					}

					_history.whosturn=(_history.whosturn+1)%nchairs;	//Increment _history.whosturn
				}
				/* 
				Attempts to check players who have posted the blind early.
				Precondition: Betround one, player posted blinds early, and no one
				has raised
				*/
				else if(betround==1&&_history.chair[i].postBlind==true&&maxBet==bblind)
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
					if(_history.chair[nta].currentBet!=0)
					{
						SetAction( i, 2, 0, betround);
						_history.whosturn=(_history.whosturn+1)%nchairs;	//Increment _history.whosturn
					}
				}
			}
			//Precondition: Player has folded
			else if(!hasMucked(i))
			{
				//If they have not been reported as folding, output a fold
				if(_history.chair[i].seatIsPlaying==true&&isPlaying(i))
				{
					SetAction( i, 1, 0, prevround);
				}
				_history.chair[i].seatIsPlaying=false;
				_history.whosturn=(_history.whosturn+1)%nchairs;	//Increment _history.whosturn
				int nplayersin=0;
				for(int i=0;i<nchairs;i++)
					if(_history.chair[i].seatIsPlaying==true) nplayersin++;

				//If there is only one player left, run outputUncontested function
				if(nplayersin==1)
				{
					for(int j=0;j<nchairs;j++)
					{
						if(_history.chair[j].seatIsPlaying==true)
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
void CHandHistory::SetPreviousActions()
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
		if(_history.chair[i].currentBalance!=0)
			_history.chair[i].prevBalance = _history.chair[i].currentBalance;
		if(middleBet[i] != _history.chair[i].currentBet)
		{
			_history.chair[i].prevBet = middleBet[i];
		}
		middleBet[i] = _history.chair[i].currentBet;
	}
}
const string CHandHistory::setDate()
{
	SYSTEMTIME			st;

	GetLocalTime(&st);
	stringstream ss;
	string s;
	ss<<st.wYear<<"-"<< setfill ('0') << setw (2)<<st.wMonth<<"-"<< setfill ('0') << setw (2)<<st.wDay<<" "
		<< setfill ('0') << setw (2)<<st.wHour<<":"<< setfill ('0') << setw (2)<<st.wMinute<<":"
		<< setfill ('0') << setw (2)<<st.wSecond;
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
		if(_history.chair[i].card_player[1]!=NULL&&(i!=userchair||userchair==0)
			&&_history.chair[i].playersPlayingBits!=0&&betround==4)
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
		if(_history.chair[i].playersPlayingBits!=0&&_history.chair[i].cardsSeen==false&&_history.chair[i].card_player[1]!=NULL)
		{
			stringstream ss;
			ss<<playerName[i]<<": Shows [ "
				<<_history.chair[i].card_player[1]
			<<_history.chair[i].card_player[0]<<" "
				<<_history.chair[i].card_player[3]
			<<_history.chair[i].card_player[2]<<" ]";
			handText[i] = ss.str()+" ";	//Stores hand output to be displayed later

			//Creates a complete hand mask for each player
			for(int j=0;j<2;j++)
				CardMask_SET(_history.chair[i].hand,p_scraper->card_player(i,j));
			for(int j=2;j<7;j++)
				CardMask_SET(_history.chair[i].hand,p_scraper->card_common(j-2));

			_history.chair[i].handval = Hand_EVAL_N(_history.chair[i].hand, 7);	//Evaluate cards

			pCardsSeen++;
			_history.chair[i].cardsSeen=true;	
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
	return(_history.chair[i].currentBet==bet[betround-1]&&_history.chair[i].ac_dealpos==2&&betround==1);
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
	int				lim = (int) p_tablelimits->gametype();
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

	if(playersdealtbits==1&&_history.chair[i].currentBalance>=0)
		return true;

	else 
		return false;
}
const bool CHandHistory::hasMucked(const int i)
{
	int				betround = (int) p_symbols->sym()->betround;

	//Precondition: Betround is 4, player has called and has not folded
	if(betround==4&&(_history.chair[i].currentBet==maxBet||_history.chair[i].currentBalance==(_history.chair[i].prevBalance-maxBet))&&_history.chair[i].seatIsPlaying==true)
		return true;

	return false;
}
void CHandHistory::checkSeats(const int i, int j)
{
	int				nchairs = (int) p_symbols->sym()->nchairs;

	do
	{
		if(_history.chair[j].playersPlayingBits!=0)
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
	if((_history.chair[i].currentBalance<=maxBet||_history.chair[i].currentBalance==0)&&allin[i]==false) 
		allin[i]=true;	
}
void CHandHistory::resetVars()
{
	double			bblind = p_tablelimits->bblind();
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
	_history.lpta = -5;

	for(int i=k_betround_preflop; i<k_betround_river; i++)
	{
		allChecks[i]=true;
		bet[i] = p_tablelimits->bet(i); 
	}
	for(int i=0;i<nchairs;i++)
	{
		_history.chair[i].ac_dealpos = DealPosition(i);
		if(_history.chair[i].ac_dealpos==1)_history.sblindpos=i;
		else if(_history.chair[i].ac_dealpos==2)_history.bblindpos=i;
		else if(_history.chair[i].ac_dealpos==3)_history.utg=i;
		allin[i]=false;
		CardMask_RESET(_history.chair[i].hand);
		_history.chair[i].postBlind=false;
		_history.chair[i].seatIsPlaying=true;
		_history.chair[i].cardsSeen=false;
		_history.chair[i].dealt=false;
		_history.chair[i].handval=0;
		handText[i]="";
		middleBet[i]=0;
		_history.chair[i].prevBet=0;
		_history.chair[i].bets=0;
		_history.chair[i].calls=0;
		_history.chair[i].actionCount=0;
		middleBet[i]=0;
		for(int j=0;j<4;j++) // should be 1..4 !!!
		{
			_history.chair[i].totalIn[j]=0;
			for(int k=0;k<8;k++) 
			{
				_history.chair[i].bet[j][k]=0;
				_history.chair[i].action[j][k]=0;
			}
		}
	}
	_history.whosturn=_history.utg;
	postflopstart=_history.sblindpos;
}
void CHandHistory::SetAction(int pnum, int action, double amount, int br)
{
	int				count = _history.chair[pnum].actionCount;
	br--;

	if(br>3) // should be 1..4 !!!
		br=3;
	else if(br<0)
		br=0;

	if(action==2)
		_history.chair[pnum].calls++;
	else if(action==3)
		_history.chair[pnum].bets++;

	if(action!=1&&amount!=0)
	{
		if(count==0)
			_history.chair[pnum].totalIn[br] = amount;
		else
			_history.chair[pnum].totalIn[br] = maxBet;
	}

	_history.chair[pnum].action[br][count]=action;
	_history.chair[pnum].bet[br][count]=amount;

	if(_history.chair[pnum].actionCount<8)
		_history.chair[pnum].actionCount++;
}
void CHandHistory::ReconstructHand(bool contested)
{
	bool			potbetinto = true;
	bool			hasFolded[10];
	double			bblind = p_tablelimits->bblind();
	double			calculatedPot = 0;
	int				userchair = (int) p_symbols->sym()->userchair;
	int				nchairs = (int) p_symbols->sym()->nchairs;
	int				dealerchair = (int) p_symbols->sym()->dealerchair;
	int				betround = 0;
	int				prevround = 0;
	int				nplayersin = 0;
	int				wt = _history.utg;

	for(int i=0;i<nchairs;i++)
	{
		hasFolded[i] = false;
		_history.chair[i].actionCount=0;
	}

	//Append to (or create if it does not exist) a handhistory file
	//using the session id as the name
	CreateDirectory("handhistory",NULL);
	stringstream ss;
	ss<<"handhistory/handhistory_"<<p_sessioncounter->session_id()<<".txt";
	string s=ss.str()+" ";
	outfile.open(s.c_str(), fstream::app);
	outfile.precision(2);
	outfile.setf(ios::fixed,ios::floatfield);

	for(int i=0;i<nchairs;i++)
		for(int j=0;j<4;j++)
			calculatedPot+=_history.chair[i].totalIn[j];

	//------------------DEALER--------------------//
	outfile<<"\nGAME #"<<gameNumber<<": Texas Hold'em "<<findLimit()<<" $"<<bblind<<"/$"<<(bblind*2)<<" "<<setDate()<<endl;
	outfile<<"Table "<<p_scraper->title()<<endl;
	for(int i=0;i<nchairs;i++)
	{
		if(_history.chair[i].dealt)
		{
			outfile<<"Seat "<<(i+1)<<": "<<playerName[i]<<" ( $"<<_history.chair[i].startBalance<<" in chips)";
			if (i==dealerchair) outfile<<" DEALER"<<endl;
			else outfile<<endl;
			nplayersin++;
		}
	}
	if(SBfound)
		outfile<<playerName[_history.sblindpos]<<": Post SB $"<<getSB(bblind)<<endl;
	outfile<<playerName[_history.bblindpos]<<": Post BB $"<<bblind<<endl;
	for(int i=0;i<nchairs;i++)
	{
		if(_history.chair[i].postBlind&&i!=(_history.bblindpos+1)%nchairs&&i!=_history.bblindpos)
		{
			outfile<<playerName[i]<<": Post BB $"<<bblind<<endl;
		}
	}
	//------------------ACTIONS--------------------//
	outfile<<"*** HOLE CARDS ***"<<endl;
	if(_history.chair[userchair].dealt&&userchair!=0)
	{
		outfile<<"Dealt to "<<playerName[userchair]<<" [ "<<_history.chair[userchair].card_player[1]<<_history.chair[userchair].card_player[0]
		<<" "<<_history.chair[userchair].card_player[3]<<_history.chair[userchair].card_player[2]<<" ]"<<endl;
	}
	while(betround<4&&nplayersin>1)
	{
		string name = playerName[wt];
		int count = _history.chair[wt].actionCount;
		int action = _history.chair[wt].action[betround][count];
		double bet = _history.chair[wt].bet[betround][count];
		if(_history.chair[wt].dealt)
		{
			if(action==1)
			{
				outfile<<name<<": Fold"<<endl;
				hasFolded[wt]=true;
				nplayersin--;
			}
			else if(action==2)
				if(bet==0)
					outfile<<name<<": Check"<<endl;
				else
					outfile<<name<<": Call $"<<bet<<endl;
			else if(action==3)
				if(!potbetinto)
				{
					outfile<<name<<": Bet $"<<bet<<endl;
					potbetinto = true;
				}
				else
					outfile<<name<<": Raise $"<<bet<<endl;
			else if(action==0)
			{
				if(!hasFolded[wt])
				{
					betround++;
					wt = postflopstart;
					for(int i=0;i<nchairs;i++) _history.chair[i].actionCount=0;
				}
			}
		}

		if(prevround!=betround)
		{
			potbetinto = false;
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
			_history.chair[wt].actionCount++;
			wt = (wt+1)%nchairs;
		}
		prevround = betround;
	}
	//------------------SHOWDOWN--------------------//
	outfile<<"*** SUMMARY ***"<<endl;
		outfile<<"Total pot $"<<calculatedPot<<" Rake $"<<(k_hand_history_rake * calculatedPot)<<endl;
	if(contested)
	{
		for(int i=0;i<nchairs;i++)
			if(handText[i]!="")
				outfile<<handText[i]<<endl;
		HandVal handhigh = {0};
		int highest=0;
		for(int i=0;i<nchairs;i++)
		{
			if(_history.chair[i].handval>handhigh)		//Find hand w/ highest value
			{
				handhigh=_history.chair[i].handval;
				highest = i;
			}
		}
		outfile<<playerName[highest]<<": wins $"<<calculatedPot<<" from the main pot with ";
		int currentType = Hand_EVAL_TYPE(_history.chair[highest].hand,7);	//Find hand type
		switch(currentType)
		{
			case StdRules_HandType_STFLUSH: 
				outfile<<"a Straight Flush."<<endl; 
				break;
			case StdRules_HandType_QUADS: 
				outfile<<"Quads."<<endl; 
				break;
			case StdRules_HandType_FULLHOUSE: 
				outfile<<"a Fullhouse."<<endl; 
				break;
			case StdRules_HandType_FLUSH: 
				outfile<<"a Flush."<<endl; 
				break;
			case StdRules_HandType_STRAIGHT: 
				outfile<<"a Straight."<<endl; 
				break;
			case StdRules_HandType_TRIPS: 
				outfile<<"Trips."<<endl; 
				break;
			case StdRules_HandType_TWOPAIR: 
				outfile<<"Two Pair."<<endl; 
				break;
			case StdRules_HandType_ONEPAIR: 
				outfile<<"One Pair."<<endl; 
				break;
			case StdRules_HandType_NOPAIR: 
				outfile<<"no Pair."<<endl; 
				break;
		}
	}
	else if(!contested)
	{
		for(int i=0;i<nchairs;i++)
			if(_history.chair[i].seatIsPlaying==true)
				outfile<<playerName[i]<<": wins $"<<calculatedPot<<endl;
	}

	outfile.close();
}