/*
	   Author	:	spud
	   Date		:	2007-MAR-08

      Notation: P(i,n)=probability that player i finishes in nth place
      P(w|z)=probability that w occurs given that z occurs
      I=i's stack
      T=SUM[I]
      
      P(i,1)=I/T
      P(i,n)=SUM(j!=i)[P(j,1)*P(i,n|j,1)]

      ICM assumes that P(i,n|j,1) = (P(i,n-1) in a game where j has been removed)
*/

#include "stdafx.h"

#include "symbols.h"
#include "global.h"
#include "ICMCalculator.h"


double P(int i, int n, double *s, int N)
{
   double p = 0.;

   if (n==1)
   {
      double I = s[i];
      double T = 0;
   
      for (int k=0; k < N; k++) T+=s[k];
      
      p = (T > 0.) ? I / T : 0.;
   }
   else
   {
      for (int j=0; j < N; j++)
      {
         if (j != i)
         {
            double *ss = (double *) calloc(N-1,sizeof(double));
            int ii = (i > j) ? i-1 : i;
            memcpy(ss, s, j * sizeof(double));
            memcpy(ss + j, s + (j + 1), (N - j -1) * sizeof(double));
            
            p += P(j, 1, s, N) * P(ii, n-1, ss, N-1);
            
            free(ss);
         }
      }
   }
   return p;
}

ICM::ICM ()
{
}

ICM::~ICM ()
{
}

double ICM::ProcessQueryICM(const char* pquery, int *e)
{
   double		prizes[4] = {0};
   double		stacks[10] = {0};
   int			i, j;
   int			mychair = (int) symbols.sym.userchair;
   double		pot = symbols.sym.pot;
   int			opponentsplayingbits = (int) symbols.sym.opponentsplayingbits;
   int			nopponentsplaying = (int) symbols.sym.nopponentsplaying; 
   int			nplayersseated =  (int) symbols.sym.nplayersseated; 
   int			playersseatedbits =  (int) symbols.sym.playersseatedbits;

   prizes[0] = global.preferences.icm_prize1;
   prizes[1] = global.preferences.icm_prize2;
   prizes[2] = global.preferences.icm_prize3;
   prizes[3] = global.preferences.icm_prize4;

   for (i = 0; i < MAX_PLAYERS; i++)
   {
      if ((playersseatedbits>>i)&1)
      {
		  stacks[i] = symbols.stacks_at_hand_start[i] - symbols.sym.currentbet[i];
      }
   }

   if (strncmp(pquery,"_fold",5)==0)
   {
      double win = pot / nopponentsplaying;

      for (i = 0; i < MAX_PLAYERS; i++)
      {
         if ((opponentsplayingbits>>i)&1)
         {
            stacks[i] += win;
         }
      }
   }

   else if (strncmp(pquery,"_callwin",8)==0)
   {
      double call = symbols.sym.call;
      
      if (stacks[mychair] < call)
      {
		  double myTotalBet = symbols.sym.currentbet[mychair] + stacks[mychair];

         for (i = 0; i < MAX_PLAYERS; i++)
         {
            if ((opponentsplayingbits>>i)&1 && myTotalBet < symbols.sym.currentbet[i])
            {
               double extra = symbols.sym.currentbet[i] - myTotalBet;
            
               stacks[i] += extra;
               pot -= extra;
            }
         }
      }
      stacks[mychair] += pot;
   }

   else if (strncmp(pquery,"_calllose",9)==0)
   { 
      double call = symbols.sym.call;
      double mycall = min(call,stacks[mychair]);
      double win = (pot + mycall) / nopponentsplaying;

      stacks[mychair] -= mycall;

      for (i = 0; i < MAX_PLAYERS; i++)
      {
         if ((opponentsplayingbits>>i)&1)
         {
            stacks[i] += win;
         }
      }
   }

   else if (strncmp(pquery,"_calltie",8)==0)
   {
      double call = symbols.sym.call;
      double win = 0.;
      
      if (stacks[mychair] < call)
      {
         double myTotalBet = symbols.sym.currentbet[mychair] + stacks[mychair];

         for (i = 0; i < MAX_PLAYERS; i++)
         {
            if ((opponentsplayingbits>>i)&1 && myTotalBet<symbols.sym.currentbet[i])
            {
               double extra = symbols.sym.currentbet[i] - myTotalBet;
            
               stacks[i] += extra;
               pot -= extra;
            }
         }
      }
      pot += min(call, stacks[mychair]);
      win = pot / (nopponentsplaying +1);
      stacks[mychair] += win;
      for (i = 0; i < MAX_PLAYERS; i++)
      {
         if ((opponentsplayingbits>>i)&1)
         {
            stacks[i] += win;
         }
      }
   }

   else if (strncmp(pquery,"_alliwin",8)==0)
   {
      if (isdigit(pquery[8]))
      {
         //assume callers are n smallest stacks
         bool callers[MAX_PLAYERS] = {0};
         int ncallers = min(pquery[8]-'0', nopponentsplaying);

         for (i = 0; i < ncallers; i++)
         {
            int jsmallest = -1;
            double smalleststack = MAX_DOUBLE;

            for (j = 0; j < MAX_PLAYERS; j++)
            {
               if ((opponentsplayingbits>>j)&1)
               {
                  if (!callers[j] && (symbols.sym.currentbet[j] + stacks[j]) < smalleststack)
                  {
                     smalleststack = symbols.sym.currentbet[j] + stacks[j];
                     jsmallest = j;
                  }
               }
            }
            if (jsmallest > -1)
            {
               double oppCurrentBet = symbols.sym.currentbet[jsmallest];
               double myTotalBet = symbols.sym.currentbet[mychair] + stacks[mychair];
               double extra = ((oppCurrentBet + stacks[jsmallest]) > myTotalBet) ? myTotalBet - oppCurrentBet :stacks[jsmallest];
               stacks[jsmallest] -= extra;
               callers[jsmallest] = true;
               pot+=extra;
            }
         }
         stacks[mychair]+=pot;
      }
      else
      {
         //assume only one particular caller
         int oppChair = getChairFromDealPos(pquery + 8);

         if (oppChair >= 0)
         {
            double oppCurrentBet = symbols.sym.currentbet[oppChair];
            double myTotalBet = symbols.sym.currentbet[mychair] + stacks[mychair];
            double extra = ((oppCurrentBet + stacks[oppChair]) > myTotalBet) ? myTotalBet - oppCurrentBet :stacks[oppChair];
            
            stacks[oppChair] -= extra;
            stacks[mychair]+= (pot + extra);
         }
      }
   }

   else if (strncmp(pquery,"_allilose",9)==0)
   {
      if (isdigit(pquery[9]))
      {
         //assume callers are n biggest stacks
         int ncallers = min(pquery[9]-'0', nopponentsplaying);

         if (ncallers > 0)
         {
            bool callers[MAX_PLAYERS] = {0};
            int *biggest =(int *) calloc(ncallers, sizeof(int));
            double *sidepots = (double *)calloc(ncallers, sizeof(double));
            double mybet = 0.;

            memset(sidepots,0, ncallers * sizeof(double));
      
            for (i = 0; i < ncallers; i++)
            {
               int jbiggest = -1;
               double biggeststack = 0.;

               for (j = 0; j < MAX_PLAYERS; j++)
               {
                  if ((opponentsplayingbits>>j)&1)
                  {
                     if (!callers[j] && (symbols.sym.currentbet[j] + stacks[j]) > biggeststack)
                     {
                        biggeststack = symbols.sym.currentbet[j] + stacks[j];
                        jbiggest = j;
                     }
                  }
               }
               biggest[i] = jbiggest;
               if (jbiggest > -1)
               {
                  callers[jbiggest] = true;
                  stacks[jbiggest] += symbols.sym.currentbet[jbiggest];
                  pot -= symbols.sym.currentbet[jbiggest];
               }
            }
            stacks[mychair] += symbols.sym.currentbet[mychair];
            pot -= symbols.sym.currentbet[mychair];
            for (i = (ncallers -1); i>=0; i--)
            {
               sidepots[i] = min(stacks[biggest[i]], stacks[mychair]) - mybet;
               mybet += sidepots[i];
            }
            stacks[mychair] -= mybet;
            sidepots[ncallers - 1] += pot;
            for (i = 0; i<ncallers; i++)
            {
               double win = sidepots[i] / (i + 1);

               for (j = 0; j<=i; j++)
               {
                  stacks[biggest[j]] += win;
               }
            }
            free(biggest);
            free(sidepots);
         }
      }
      else
      {
         //assume only one particular caller
         int oppChair = getChairFromDealPos(pquery + 9);

         if (oppChair >= 0)
         {
            double oppTotalBet = symbols.sym.currentbet[oppChair] + stacks[oppChair];
            double myTotalBet = symbols.sym.currentbet[mychair] + stacks[mychair];
            double extra = (oppTotalBet > myTotalBet) ? stacks[mychair] : oppTotalBet - symbols.sym.currentbet[mychair];
            
            stacks[oppChair]+= (pot + extra);
            stacks[mychair]-= extra;
         }
      }
   }

   else if (strncmp(pquery,"_allitie",8)==0)
   {
      //assume only one particular caller
      int oppChair = getChairFromDealPos(pquery + 8);

      if (oppChair >= 0)
      {
         stacks[oppChair]+= pot / 2;
         stacks[mychair]+= pot / 2;
      }
   }

   else
	{
		for(i = 0; i < MAX_PLAYERS; i++)
		{
			if((playersseatedbits>>i)&1)
			{
				stacks[i] += symbols.sym.currentbet[i];
			}
		}
	}

   return EquityICM(stacks, prizes, MAX_PLAYERS, mychair);
}

double ICM::EquityICM(double *stacks, double *prizes, int playerNB, int player)
{
   double ICM = 0.;
   int i = 0;
   
   for(i = 0; i < playerNB; i++)
   {
     //printf("player %d  stack = %1.2f \n", i, stacks[i]);
   }
   
   i = 0;
   while(i < playerNB && prizes[i] > 0.)
   {
      double p = P(player, i + 1, stacks, playerNB);

      //printf("=> prob place %d = %1.4f \n", i + 1, p);
      ICM += prizes[i] * p;
      i++;
   }

   return ICM;
}

double ICM::GetPlayerCurrentBet(int pos)
{
	return symbols.sym.currentbet[pos];
}

int ICM::getChairFromDealPos(const char* pquery)
{
   int chair = -1;
   int nplayersseated = (int) symbols.sym.nplayersseated;

   if (nplayersseated > 0)
   {
      int playersseatedbits = (int) symbols.sym.playersseatedbits;
      int dealPos = -1;

      if (strcmp(pquery,"SB")==0)
         dealPos = 1;
      else if (strcmp(pquery,"BB")==0)
         dealPos = 2;
      else if (strcmp(pquery,"UTG")==0)
         dealPos = 3;
      else if (strcmp(pquery,"UTG1")==0)
         dealPos = 4;
      else if (strcmp(pquery,"UTG2")==0)
         dealPos = 5;
      else if (strcmp(pquery,"UTG3")==0)
         dealPos = 6;
      else if (strcmp(pquery,"UTG4")==0)
         dealPos = 7;
      else if (strcmp(pquery,"UTG5")==0)
         dealPos = 8;
      else if (strcmp(pquery,"UTG6")==0)
         dealPos = 9;
      else if (strcmp(pquery,"D")==0)
         dealPos = 0;
      else if (strcmp(pquery,"CO")==0)
         dealPos = nplayersseated - 1;

      if (dealPos >= 0)
      {
         chair = (int) symbols.sym.dealerchair;

         while (dealPos > 0)
         {
            chair = (chair + 1) % MAX_PLAYERS;

            if ((playersseatedbits>>chair)&1)
               dealPos--;
         }
      }
   }
   return chair;
}