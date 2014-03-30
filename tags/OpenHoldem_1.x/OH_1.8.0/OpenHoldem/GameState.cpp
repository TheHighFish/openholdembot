#include "stdafx.h"
#include "GameState.h"
#include "debug.h"
#include "global.h"
#include "symbols.h"

GameState		game_state;

GameState::GameState() 
{
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif

#ifdef SEH_ENABLE
	try {
#endif

		m_ndx = 0;
		hands_played = 0;
		max_balance = 0;
		oppdealt = 0;
		m_game_ndx = 0;
		m_ftr_ndx = 0;
		new_hand = true;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("GameState::GameState\n"); 
		throw;
	}
#endif
}

double GameState::wh_sym_hist (char * sym, int round) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int		i;

		for (i=0; i<hist_sym_count; i++) 
		{
			if (memcmp(sym, hist_sym_strings[i], strlen(sym))==0 && strlen(sym)==strlen(hist_sym_strings[i])) 
			{
				return hist_sym[i][round-1];
			}
		}

		return 0.0;
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::wh_sym_hist\n"); 
		throw; 
	}
#endif
}

void GameState::process_game_state (holdem_state* pstate) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int			i, j;
		int			e;
		bool		pstate_changed;
		static int	nopponentsplaying_last=0;
		static int	dealerchair_last=-1;

		// tracking of nopponentsdealt
		if (symbols.sym.br==2 || symbols.sym.nopponentsdealt>oppdealt) 
		{
			oppdealt=symbols.sym.nopponentsdealt;
		}

		// Has the number of opponents changed?
		if (symbols.sym.nopponentsplaying != nopponentsplaying_last)	
		{
			nopponentsplaying_last=symbols.sym.nopponentsplaying;
		}

		// see if the new pstate has changed in an interesting way
		pstate_changed = false;
		if (pstate->m_dealer_chair != m_holdem_state[(m_ndx)&0xff].m_dealer_chair)
			pstate_changed = true;

		for (i=0; i<10; i++) 
		{
			if (pstate->m_player[i].m_balance != m_holdem_state[(m_ndx)&0xff].m_player[i].m_balance)
				pstate_changed = true;

			if (pstate->m_player[i].m_currentbet != m_holdem_state[(m_ndx)&0xff].m_player[i].m_currentbet)
				pstate_changed = true;

			if (pstate->m_player[i].m_cards[0] != m_holdem_state[(m_ndx)&0xff].m_player[i].m_cards[0])
				pstate_changed = true;

			if (pstate->m_player[i].m_cards[1] != m_holdem_state[(m_ndx)&0xff].m_player[i].m_cards[1])
				pstate_changed = true;

			if (pstate->m_player[i].m_balance_known != m_holdem_state[(m_ndx)&0xff].m_player[i].m_balance_known &&
				pstate->m_player[i].m_cards[0] != 0 && pstate->m_player[i].m_cards[1] != 0)
				pstate_changed = true;
		}
		for (i=0; i<5; i++)	
		{
			if (pstate->m_cards[i] != m_holdem_state[(m_ndx)&0xff].m_cards[i])
				pstate_changed = true;
		}

		// only process wh state if something interesting within the structure changes
		if (pstate!=NULL && (pstate_changed || symbols.sym.ismyturn))
			process_state_engine(pstate, pstate_changed);

		// reset wh symbol GameState if button moves
		if (symbols.sym.dealerchair != dealerchair_last) 
		{
			dealerchair_last = symbols.sym.dealerchair;
			for (i=0; i<hist_sym_count; i++) 
			{
				for (j=0; j<4; j++) 
				{
					hist_sym[i][j] = 0.0;
				}
			}
		}

		// collect symbol if it ismyturn, or if ismanual
		if (symbols.sym.ismyturn || symbols.sym.ismanual) 
		{
			for (i=0; i<hist_sym_count; i++) 
			{
				e = SUCCESS;
				hist_sym[i][(int) symbols.sym.br-1] = symbols.GetSymbolVal(hist_sym_strings[i], &e);
			}
		}
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::process_game_state\n"); 
		throw; 
	}
#endif
}

void GameState::process_ftr (holdem_state* pstate) 
{
#ifdef SEH_ENABLE
	try {
#endif
		process_ftr_engine(pstate);
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::process_ftr\n"); 
		throw; 
	}
#endif
}

int GameState::lastraised (int round) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int lastraised=-1;
		int i;

		if (round<1 || round>4) 
			return lastraised;

		for (i=m_game_state[(m_game_ndx)&0xff].m_dealer_chair+1; i<=m_game_state[(m_game_ndx)&0xff].m_dealer_chair+10; i++) 
		{		
			if (chair_actions[i%10][round-1][w_reraised]) 
			{ 
				lastraised=i%10; 
			}
		}

		if (lastraised==-1) 
		{
			for (i=m_game_state[(m_game_ndx)&0xff].m_dealer_chair+1; i<=m_game_state[(m_game_ndx)&0xff].m_dealer_chair+10; i++) 
			{		
				if (chair_actions[i%10][round-1][w_raised]) 
				{ 
					lastraised=i%10; 
				}
			}
		}

		return lastraised;
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::lastraised\n"); 
		throw; 
	}
#endif
}

int GameState::raisbits (int round) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int i, bits=0;
		static unsigned int	exponents[]={1,2,4,8,16,32,64,128,256,512,1024};

		for (i=0; i<=9; i++) 
		{
			if (chair_actions[i][round-1][w_raised] || 
				chair_actions[i][round-1][w_reraised]) 
			{
				bits|=exponents[i]; 
			}
		}
		return bits;
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::raisbits\n"); 
		throw; 
	}
#endif
}

int GameState::callbits (int round) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int i, bits=0;
		static unsigned int	exponents[]={1,2,4,8,16,32,64,128,256,512,1024};

		for (i=0; i<=9; i++) 
		{
				if (chair_actions[i][round-1][w_called] || 
					chair_actions[i][round-1][w_posted_bb]) 
				{
					bits|=exponents[i]; 
				}
			}
		return bits;
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::callbits\n"); 
		throw; 
	}
#endif
}

int GameState::foldbits (int round) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int i, bits=0;
		static unsigned int	exponents[]={1,2,4,8,16,32,64,128,256,512,1024};

		for (i=0; i<=9; i++) 
		{
			if (chair_actions[i][round-1][w_folded]) 
			{
				bits|=exponents[i]; 
			}
		}
		return bits;
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::foldbits\n"); 
		throw; 
	}
#endif
}

double GameState::floppct (void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int i;
		int elapsed_start=0, num_dealt=0, hands=0, num_saw_cards=0;
		double percent;

		if (m_ftr_ndx>1) 
		{
			for (i=m_ftr_ndx-1; i>=1; i--) 
			{
				if (m_ftr[i&0xff].elapsed_time!=0 && m_ftr[i&0xff].n_pl_dealt>1) 
				{
					if (elapsed_start==0) 
						elapsed_start=m_ftr[i&0xff].elapsed_time;

					if (elapsed_start-m_ftr[i&0xff].elapsed_time<global.preferences.av_time*60) 
					{
						num_dealt+=m_ftr[i&0xff].n_pl_dealt;
						hands++;
						num_saw_cards+=m_ftr[i&0xff].n_pl_saw_flop;
					}
					else 
					{ 
						i=0; 
					}
				}
			}
		}

		if (num_dealt!=0)
			percent=(double) num_saw_cards/(double) num_dealt;

		else
			percent=-1;

		return percent;
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::floppct\n"); 
		throw; 
	}
#endif
}

double GameState::turnpct (void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int i;
		int elapsed_start=0, num_dealt=0, hands=0, num_saw_cards=0;
		double percent;

		if (m_ftr_ndx>1) 
		{
			for (i=m_ftr_ndx-1; i>=1; i--) 
			{
				if (m_ftr[i&0xff].elapsed_time!=0 && m_ftr[i&0xff].n_pl_dealt>1) 
				{
					if (elapsed_start==0)
						elapsed_start=m_ftr[i&0xff].elapsed_time;

					if (elapsed_start-m_ftr[i&0xff].elapsed_time<global.preferences.av_time*60) 
					{
						num_dealt+=m_ftr[i&0xff].n_pl_dealt;
						hands++;
						num_saw_cards+=m_ftr[i&0xff].n_pl_saw_turn;
					}
					else 
					{ 
						i=0; 
					}
				}
			}
		}

		if (num_dealt!=0)
			percent=(double) num_saw_cards/(double) num_dealt;

		else
			percent=-1;

		return percent;
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::turnpct\n"); 
		throw; 
	}
#endif
}

double GameState::riverpct (void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int i;
		int elapsed_start=0, num_dealt=0, hands=0, num_saw_cards=0;
		double percent;

		if (m_ftr_ndx>1) 
		{
			for (i=m_ftr_ndx-1; i>=1; i--) 
			{
				if (m_ftr[i&0xff].elapsed_time!=0 && m_ftr[i&0xff].n_pl_dealt>1) 
				{
					if (elapsed_start==0)
						elapsed_start=m_ftr[i&0xff].elapsed_time;

					if (elapsed_start-m_ftr[i&0xff].elapsed_time<global.preferences.av_time*60) 
					{
						num_dealt+=m_ftr[i&0xff].n_pl_dealt;
						hands++;
						num_saw_cards+=m_ftr[i&0xff].n_pl_saw_river;
					}
					else 
					{
						i=0; 
					}
				}
			}
		}

		if (num_dealt!=0)
			percent=(double) num_saw_cards/(double) num_dealt;

		else
			percent=-1;

		return percent;
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::riverpct\n"); 
		throw; 
	}
#endif
}

double GameState::avgbetspf (void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int i;
		int elapsed_start=0, num_dealt=0, hands=0, num_saw_cards=0;
		double percent, bets_preflop=0.0;

		if (m_ftr_ndx>1) 
		{
			for (i=m_ftr_ndx-1; i>=1; i--) 
			{
				if (m_ftr[i&0xff].elapsed_time!=0 && m_ftr[i&0xff].n_pl_dealt>1) 
				{
					if (elapsed_start==0)
						elapsed_start=m_ftr[i&0xff].elapsed_time;

					if (elapsed_start-m_ftr[i&0xff].elapsed_time<global.preferences.av_time*60) 
					{
						num_dealt+=m_ftr[i&0xff].n_pl_dealt;
						hands++;
						bets_preflop+=m_ftr[i&0xff].n_bets_preflop;
					}
					else 
					{ 
						i=0; 
					}
				}
			}
		}

		if (num_dealt!=0)
			percent=(double) bets_preflop/(double) hands;

		else
			percent=-1;

		return percent;
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::avgbetspf\n"); 
		throw; 
	}
#endif
}

double GameState::tablepfr (void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int i;
		int elapsed_start=0, num_dealt=0, hands=0, num_saw_cards=0, bets_preflop=0;
		double percent;

		if (m_ftr_ndx>1) {

			for (i=m_ftr_ndx-1; i>=1; i--) 
			{
				if (m_ftr[i&0xff].elapsed_time!=0 && m_ftr[i&0xff].n_pl_dealt>1) 
				{
					if (elapsed_start==0)
						elapsed_start=m_ftr[i&0xff].elapsed_time;

					if (elapsed_start-m_ftr[i&0xff].elapsed_time<global.preferences.av_time*60) 
					{
						num_dealt+=m_ftr[i&0xff].n_pl_dealt;
						hands++;

						//Either there was more than one raise or no one saw a flop (hence a raise)
						if (m_ftr[i&0xff].n_bets_preflop > 1 || m_ftr[i&0xff].n_pl_saw_flop == 0) 
							bets_preflop++;

					}
					else 
					{ 
						i=0; 
					}
				}
			}
		}

		if (num_dealt!=0)
			percent=(double) bets_preflop/(double) hands;

		else
			percent=-1;

		return percent;
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::tablepfr\n"); 
		throw; 
	}
#endif
}

void GameState::process_state_engine(holdem_state* pstate, bool pstate_changed) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int				from_chair, to_chair;
		bool			balance_stability;
		int				i, j, k;
		static bool		process_game_state=false;
		static bool		safe_to_process_state=false;
		static bool		end_of_hand=false;
		static bool		first_pass_capture=false;
		static int		wh_br_last=1;
		static bool		pot_raised=false;
		static int		pf_limpers_n=0;
		static bool		my_first_action_this_round=true;
		static bool		my_first_action_this_hand=true;
		static bool		small_blind_posted=false;
		static bool		biblind_posted=false;
		static double	bets_last=0.0;

		m_holdem_state[ (++m_ndx)&0xff ] = *pstate;

		// Only start processing state info, if we have one solid frame recorded (many calcs require m_ndx-1)
		if (safe_to_process_state == false && m_ndx>1)
			safe_to_process_state = true;

		// check if all balances are known (indicates stability of info passed to DLL)
		balance_stability = true;
		for (i=0; i<10; i++) 
		{
			if (m_holdem_state[(m_ndx)&0xff].m_player[i].m_cards[0] != 0 && 
				m_holdem_state[(m_ndx)&0xff].m_player[i].m_cards[1] != 0 &&
				m_holdem_state[(m_ndx)&0xff].m_player[i].m_balance_known != 1) 
			{
				balance_stability = false;
			}
		}

		// only process further if safe_to_process_state==true, userchair is identified (br!=0),
		// and m_balance_known is true for all players with cards, and I am actually in the hand
		if ((symbols.sym.br != 0 && safe_to_process_state && (balance_stability || symbols.sym.ismanual))
			|| m_holdem_state[(m_ndx)&0xff].m_dealer_chair != m_holdem_state[(m_ndx-1)&0xff].m_dealer_chair) 
		{

			// Check for end of hand situation
			for (i=0; i<10; i++) 
			{
				// if new card fronts have appeared, then players are showing down, and its the end of the hand
				if ( m_holdem_state[(m_ndx-1)&0xff].m_player[i%10].m_cards[0] == 255 &&		// was card backs last scrape
					 m_holdem_state[(m_ndx-1)&0xff].m_player[i%10].m_cards[1] == 255 &&		// was card backs last scrape
					 m_holdem_state[(m_ndx)&0xff].m_player[i%10].m_cards[0] != 255 &&		// is not card backs this scrape
					 m_holdem_state[(m_ndx)&0xff].m_player[i%10].m_cards[1] != 255 &&		// is not card backs this scrape
					 m_holdem_state[(m_ndx)&0xff].m_player[i%10].m_cards[0] != 0 &&			// is not 'no cards' this scrape
					 m_holdem_state[(m_ndx)&0xff].m_player[i%10].m_cards[1] != 0 &&			// is not 'no cards' this scrape
					 i != symbols.sym.userchair &&												// it's not me
					 end_of_hand==false ) 
				{
					end_of_hand = true;
					write_log(">>> SHOWDOWN\n");
				}
			}

			// save state information if one of the following is true:
			//   1. a new hand has started
			//	 2. it's my turn
			//   3. the betting round has changed
			//   4. we are in manual mode

			// 1. new hand has started
			if (new_hand || first_pass_capture) 
			{

				// save the game state
				m_game_state[ (++m_game_ndx)&0xff ] = *pstate;

				//reset some vars
				first_pass_capture = false;
				new_hand = false;
				wh_br_last=1;
				pot_raised = false;
				pf_limpers_n = 0;
				my_first_action_this_round = true;
				my_first_action_this_hand = true;
				small_blind_posted = false;
				biblind_posted = false;
				bets_last = 0.0;
				end_of_hand = false;
				for (i=0; i<10; i++) 
				{
					for (j=0; j<4; j++) 
					{
						for (k=0; k<w_num_action_types; k++)
							chair_actions[i][j][k] = w_noaction;
					}
				}

				// Track some stats
				if (symbols.sym.balance[10] > max_balance) 
				{ 
					max_balance = symbols.sym.balance[10] ; 
				}
				hands_played++;

				write_log(">>> New hand %.0f\n", symbols.sym.handnumber);
			}

			// first time to act in the hand//
			if (symbols.sym.ismyturn && my_first_action_this_hand == true) 
			{
				my_first_action_this_hand = false;
			}

			// 2. it's my turn
			if ((symbols.sym.ismyturn && pstate_changed) ||
				(symbols.sym.ismyturn && my_first_action_this_round == true) ||
				(((symbols.sym.br == 1 && 
				   m_holdem_state[(m_ndx)&0xff].m_player[(int) symbols.sym.userchair].m_currentbet == symbols.sym.sblind) ||
				  (symbols.sym.br == 1 && 
				   m_holdem_state[(m_ndx)&0xff].m_player[(int) symbols.sym.userchair].m_currentbet == symbols.sym.bblind)) &&
					my_first_action_this_round == true) ) 
			{

				// save the game state
				m_game_state[ (++m_game_ndx)&0xff ] = *pstate;

				//reset some vars
				if (m_holdem_state[(m_ndx)&0xff].m_player[(int) symbols.sym.userchair].m_cards[0] != 0 &&
					m_holdem_state[(m_ndx)&0xff].m_player[(int) symbols.sym.userchair].m_cards[1] != 0) 
				{
					process_game_state = true;
					write_log( ">>> My turn, br=%d\n", (int) symbols.sym.br); 	
				}
				else 
				{
					process_game_state = false;
				}
			}

			// 3. the betting round has changed
			if (symbols.sym.br > wh_br_last) 
			{
				// save the game state
				m_game_state[ (++m_game_ndx)&0xff ] = *pstate;

				//reset some vars
				wh_br_last = symbols.sym.br;	
				pot_raised = false;
				my_first_action_this_round = true;
				bets_last = 0.0;

				//write_log(">>> Betting round: %d\n", (int) symbols.sym.br);
			}

			// it's my turn, so I need to figure out what everyone did before me
			if (process_game_state) 
			{
				process_game_state = false;
				
				// figure out how to iterate through the chairs
				// if it is my first action this round 
				if (my_first_action_this_round == true) 
				{
					my_first_action_this_round=false;

					// if i am the sb, then iterate just mychair
					if (symbols.sym.br == 1 && 
						m_game_state[(m_game_ndx)&0xff].m_player[(int) symbols.sym.userchair].m_currentbet == symbols.sym.sblind) 
					{
						from_chair = symbols.sym.userchair;
						to_chair = symbols.sym.userchair-1+10;
					}

					// if i am the bb, then iterate from dlr+1 to mychair
					else if (symbols.sym.br == 1 && 
						m_game_state[(m_game_ndx)&0xff].m_player[(int) symbols.sym.userchair].m_currentbet == symbols.sym.bblind) 
					{

						from_chair = m_game_state[(m_game_ndx)&0xff].m_dealer_chair+1;
						to_chair = symbols.sym.userchair-1+10;
					}

					// if the dlr chair < mychair, then iterate from dlr+1 to mychair-1
					else if (m_game_state[(m_game_ndx)&0xff].m_dealer_chair < symbols.sym.userchair) 
					{
						from_chair = m_game_state[(m_game_ndx)&0xff].m_dealer_chair+1;
						to_chair = symbols.sym.userchair-1;
					}

					// if the dlr chair >= mychair, then iterate from dlr+1 to mychair-1+10 (to loop around)
					else 
					{
						from_chair = m_game_state[(m_game_ndx)&0xff].m_dealer_chair+1;
						to_chair = symbols.sym.userchair-1+10;
					}

				}
				// if it is not my first action this round, then iterate from mychair+1 to mychair-1+10
				else 
				{
					from_chair = symbols.sym.userchair+1;
					to_chair = symbols.sym.userchair-1+10;
				}

				// now iterate through the chairs and see what everybody did
				for (i = from_chair; i <= to_chair; i++) 
				{
					// if the currentbet for the chair is the sb and the last bet was zero and br==1
					// and the player has cards, then we know the chair ***POSTED THE SMALL BLIND***
					if (m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet == symbols.sym.sblind &&
						bets_last==0 &&
						 m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[0]!=0 &&
						 m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[1]!=0 &&
						 symbols.sym.br == 1) 
					{
						chair_actions[i%10][(int) symbols.sym.br-1][w_posted_sb] = true;
						bets_last = m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet;
						write_log(">>> Chair %d (%s) posted the sb: $%.2f\n", i%10, 
							m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_name,
							m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet);
					}

					// if the currentbet for the chair is the bb and the last bet was the sb and br==1
					// and the player has cards, then we know the chair ***POSTED THE BIG BLIND***
					else if (m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet == symbols.sym.bblind && 
							 bets_last == symbols.sym.sblind &&
							 m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[0]!=0 &&
							 m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[1]!=0 &&
							 symbols.sym.br == 1) 
					{
						chair_actions[i%10][(int) symbols.sym.br-1][w_posted_bb] = true;
						bets_last = m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet;
						write_log(">>> Chair %d (%s) posted the bb: $%.2f\n", i%10, 
							m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_name, 
							m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet);
					}

					// if the currentbet for the chair is greater than the last bet and it's not the end of the hand, 
					// and the player has cards, then we know the chair ***RAISED or RE-RAISED***
					else if (m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet > bets_last &&
							 m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[0]!=0 &&
							 m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[1]!=0 &&
							 !end_of_hand) 
					{
						bets_last = m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet;
						if (pot_raised == false) 
						{
							chair_actions[i%10][(int) symbols.sym.br-1][w_raised] = true;
							pot_raised = true;
							write_log(">>> Chair %d (%s) raised to $%.2f\n", i%10, 
								m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_name, 
								m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet);
						}
						else 
						{
							chair_actions[i%10][(int) symbols.sym.br-1][w_reraised] = true;
							write_log(">>> Chair %d (%s) re-raised to $%.2f\n", i%10, 
								m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_name, 
								m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet);
						}
					}

					// if the currentbet for the chair is not zero and is <= the last bet and it's not the end of the hand,
					// and the player has cards, then we know the chair ***CALLED***
					else if (m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet != 0 &&
							 m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet <= bets_last &&
							 m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[0]!=0 &&
							 m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[1]!=0 &&
							 !end_of_hand) 
					{
						chair_actions[i%10][(int) symbols.sym.br-1][w_called] = true;
						if (pot_raised == false) 
						{
							pf_limpers_n += 1;
						}
						write_log(">>> Chair %d (%s) called\n", i%10, m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_name);
					}

					// if cards have disappeared, and they were card backs last scrape, 
					// then we know the chair has ***FOLDED***
					// unless i=userchair, in which case they could be card fronts last scrape
					// in any case, if the balance in front of the chair has increased, it is not a fold, but a holdover from showdown
					else if ( ( m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[0]==0 &&			// no cards this scrape
								m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[1]==0 &&			// no cards this scrape
								m_game_state[(m_game_ndx-1)&0xff].m_player[i%10].m_cards[0]==255 &&		// card backs last scrape
								m_game_state[(m_game_ndx-1)&0xff].m_player[i%10].m_cards[1]==255 &&		// card backs last scrape
								m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_balance==
								   m_game_state[(m_game_ndx-1)&0xff].m_player[i%10].m_balance)
							  ||
							  ( i%10 == symbols.sym.userchair &&										// this is my chair
								m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[0]==0 &&			// no cards this scrape
								m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[1]==0 &&			// no cards this scrape
								m_game_state[(m_game_ndx-1)&0xff].m_player[i%10].m_cards[0]!=0 &&		// card fronts/backs last scrape
								m_game_state[(m_game_ndx-1)&0xff].m_player[i%10].m_cards[1]!=0 &&		// card fronts/backs last scrape
								m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_balance==
								   m_game_state[(m_game_ndx-1)&0xff].m_player[i%10].m_balance) ) 
					{			
						chair_actions[i%10][(int) symbols.sym.br-1][w_folded] = true;
						write_log(">>> Chair %d (%s) folded\n", i%10, 
							m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_name);
					}

					// if br!=1 and the player's current bet is zero, and that player has cards in front of them 
					// then we know the chair has ***CHECKED***
					else if (symbols.sym.br!=1 && m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_currentbet == 0 &&
							 m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[0]!=0 &&
							 m_game_state[(m_game_ndx)&0xff].m_player[i%10].m_cards[1]!=0) 
					{
						chair_actions[i%10][(int) symbols.sym.br-1][w_checked] = true;
									
						write_log( ">>> Chair %d (%s) checked\n", i%10, m_holdem_state[(m_ndx)&0xff].m_player[i%10].m_name);
					}
				}  // end of "for (i = from_chair; i <= to_chair; i++)"
			} // end of "if (br != 0 &&..."
		} // end of "if (process_game_state)"
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::process_state_engine\n"); 
		throw; 
	}
#endif
}

void GameState::dump_state(void) 
{
#ifdef SEH_ENABLE
	try {
#endif
		int			i;
	
		write_log("m_ndx: %d\n", m_ndx);
		write_log("m_title: %s\n", m_holdem_state[(m_ndx)&0xff].m_title);
		write_log("m_pot: %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n", m_holdem_state[(m_ndx)&0xff].m_pot[0], m_holdem_state[(m_ndx)&0xff].m_pot[1],
			m_holdem_state[(m_ndx)&0xff].m_pot[2], m_holdem_state[(m_ndx)&0xff].m_pot[3], m_holdem_state[(m_ndx)&0xff].m_pot[4],
			m_holdem_state[(m_ndx)&0xff].m_pot[5], m_holdem_state[(m_ndx)&0xff].m_pot[6], m_holdem_state[(m_ndx)&0xff].m_pot[7],
			m_holdem_state[(m_ndx)&0xff].m_pot[8], m_holdem_state[(m_ndx)&0xff].m_pot[9]);
		write_log("m_cards: %d %d %d %d %d\n", m_holdem_state[(m_ndx)&0xff].m_cards[0], m_holdem_state[(m_ndx)&0xff].m_cards[1], 
			m_holdem_state[(m_ndx)&0xff].m_cards[2], m_holdem_state[(m_ndx)&0xff].m_cards[3], m_holdem_state[(m_ndx)&0xff].m_cards[4]);
		write_log("m_is_playing: %d\n", m_holdem_state[(m_ndx)&0xff].m_is_playing);
		write_log("m_is_posting: %d\n", m_holdem_state[(m_ndx)&0xff].m_is_posting);
		write_log("m_dealer_chair: %d\n", m_holdem_state[(m_ndx)&0xff].m_dealer_chair);
		for (i=0; i<10; i++) {
			write_log("m_player[%d].m_name:%s  ", i, m_holdem_state[(m_ndx)&0xff].m_player[i].m_name);
			write_log("m_balance:%.2f  ", m_holdem_state[(m_ndx)&0xff].m_player[i].m_balance);
			write_log("m_currentbet:%.2f  ", m_holdem_state[(m_ndx)&0xff].m_player[i].m_currentbet);
			write_log("m_cards:%d/%d  ", m_holdem_state[(m_ndx)&0xff].m_player[i].m_cards[0],
				m_holdem_state[(m_ndx)&0xff].m_player[i].m_cards[1]);
			write_log("m_name_known:%d  ", m_holdem_state[(m_ndx)&0xff].m_player[i].m_name_known);
			write_log("m_balance_known:%d\n", m_holdem_state[(m_ndx)&0xff].m_player[i].m_balance_known);
		}
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::dump_state\n"); 
		throw; 
	}
#endif
}

void GameState::process_ftr_engine(holdem_state* pstate) 
{
#ifdef SEH_ENABLE
	try {
#endif

		static int		ftr_dealer_chair_last=0;
		static int		ftr_nflopc_last=0;
		static int		ftr_nplayersdealt_last=0;

		// if a new hand has started setup the next element in the ftr tracker array	
		if (pstate->m_dealer_chair != ftr_dealer_chair_last) 
		{
			m_ftr_ndx++;
			m_ftr[m_ftr_ndx&0xff].elapsed_time = symbols.sym.elapsed;
			m_ftr[m_ftr_ndx&0xff].n_pl_dealt = 0;
			m_ftr[m_ftr_ndx&0xff].n_pl_saw_flop = 0;
			m_ftr[m_ftr_ndx&0xff].n_pl_saw_turn = 0;
			m_ftr[m_ftr_ndx&0xff].n_pl_saw_river = 0;
			m_ftr[m_ftr_ndx&0xff].n_bets_preflop = 0;
			ftr_nplayersdealt_last=0;
			ftr_nflopc_last=0;
			ftr_dealer_chair_last=pstate->m_dealer_chair;
		}

		// if nplayersdealt has incremented and it is br1, update the ftr tracker stats
		if ( symbols.sym.nplayersdealt != ftr_nplayersdealt_last && 
			 symbols.sym.nplayersdealt > ftr_nplayersdealt_last &&
			 symbols.sym.br==1) 
		{
			m_ftr[m_ftr_ndx&0xff].n_pl_dealt = symbols.sym.nplayersdealt;
			ftr_nplayersdealt_last = symbols.sym.nplayersdealt;
		}

		// if the betting round has changed update the ftr tracker stats 
		if (symbols.sym.nflopc > ftr_nflopc_last) 
		{
			if (symbols.sym.nflopc==3 && m_ftr[m_ftr_ndx&0xff].n_pl_saw_flop==0) 
			{ 
				m_ftr[m_ftr_ndx&0xff].n_pl_saw_flop = symbols.sym.nplayersplaying;
				m_ftr[m_ftr_ndx&0xff].n_bets_preflop = symbols.sym.nbetsround[0];
			}

			if (symbols.sym.nflopc==4 && m_ftr[m_ftr_ndx&0xff].n_pl_saw_turn==0) 
			{ 
				m_ftr[m_ftr_ndx&0xff].n_pl_saw_turn = symbols.sym.nplayersplaying; 
			}

			if (symbols.sym.nflopc==5 && m_ftr[m_ftr_ndx&0xff].n_pl_saw_river==0) 
			{ 
				m_ftr[m_ftr_ndx&0xff].n_pl_saw_river = symbols.sym.nplayersplaying; 
			}

			ftr_nflopc_last = symbols.sym.nflopc;
		}
#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::process_ftr_engine\n"); 
		throw; 
	}
#endif
}

double GameState::sortedbalance(int rank) 
{
#ifdef SEH_ENABLE
	try {
#endif

		int i, n;
		double stacks[10];
		double temp;

		for (int i=0; i<=9; i++) 
			stacks[i] = m_holdem_state[m_ndx].m_player[i].m_balance + m_holdem_state[m_ndx].m_player[i].m_currentbet;

		// bubble sort stacks
		for (i=0; i<=8; i++) 
		{
			for (n=i+1; n<=9; n++) 
			{
				if (stacks[i] < stacks[n]) 
				{
				   temp = stacks[i];
				   stacks[i] = stacks[n];
				   stacks[n] = temp;
				}
			}
		}

		return stacks[rank];

#ifdef SEH_ENABLE
	}
	catch (...) { 
		logfatal("GameState::sortedbalance\n"); 
		throw; 
	}
#endif
}
