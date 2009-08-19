#include "stdafx.h"

#include "CGameState.h"

#include "CSymbols.h"
#include "CScraper.h"
#include "CPreferences.h"

CGameState			*p_game_state = NULL;

CGameState::CGameState()
{
	_m_ndx = 0;
	_hands_played = 0;
	_oppdealt = 0;
	_m_game_ndx = 0;
	_m_ftr_ndx = 0;
	_new_hand = true;
	_nopponentsplaying_last = 0;
	_process_game_state = false;
	_safe_to_process_state = false;
	_end_of_hand = false;
	_first_pass_capture = false;
	_oh_br_last = 1;
	_pot_raised = false;
	_pf_limpers_n = 0;
	_my_first_action_this_round = true;
	_my_first_action_this_hand = true;
	_small_blind_posted = false;
	_big_blind_posted = false;
	_bets_last = 0.0;

	_ftr_dealer_chair_last = 0;
	_ftr_nflopc_last = 0;
	_ftr_nplayersdealt_last = 0;
}

CGameState::~CGameState()
{
}

void CGameState::ProcessGameState(const SHoldemState *pstate)
{
	int			i = 0, j = 0;
	bool		pstate_changed = false;

	int			sym_br = (int) p_symbols->sym()->br;
	int			sym_nopponentsdealt = (int) p_symbols->sym()->nopponentsdealt;
	int			sym_nopponentsplaying = (int) p_symbols->sym()->nopponentsplaying;
	bool		sym_ismyturn = (bool) p_symbols->sym()->ismyturn;
	bool		sym_ismanual = (bool) p_symbols->sym()->ismanual;

	// tracking of nopponentsdealt
	if (sym_br==2 || sym_nopponentsdealt>_oppdealt)
		_oppdealt = sym_nopponentsdealt;

	// Has the number of opponents changed?
	if (sym_nopponentsplaying != _nopponentsplaying_last)
		_nopponentsplaying_last = sym_nopponentsplaying;

	// see if the new pstate has changed in an interesting way
	pstate_changed = false;
	if (pstate->m_dealer_chair != _m_holdem_state[(_m_ndx)&0xff].m_dealer_chair)
		pstate_changed = true;

	for (i=0; i<10; i++)
	{
		if (pstate->m_player[i].m_balance != _m_holdem_state[(_m_ndx)&0xff].m_player[i].m_balance)
			pstate_changed = true;

		if (pstate->m_player[i].m_currentbet != _m_holdem_state[(_m_ndx)&0xff].m_player[i].m_currentbet)
			pstate_changed = true;

		if (pstate->m_player[i].m_cards[0] != _m_holdem_state[(_m_ndx)&0xff].m_player[i].m_cards[0])
			pstate_changed = true;

		if (pstate->m_player[i].m_cards[1] != _m_holdem_state[(_m_ndx)&0xff].m_player[i].m_cards[1])
			pstate_changed = true;

		if (pstate->m_player[i].m_balance_known != _m_holdem_state[(_m_ndx)&0xff].m_player[i].m_balance_known &&
				pstate->m_player[i].m_cards[0] != 0 && pstate->m_player[i].m_cards[1] != 0)
			pstate_changed = true;
	}
	for (i=0; i<=4; i++)
	{
		if (pstate->m_cards[i] != _m_holdem_state[(_m_ndx)&0xff].m_cards[i])
			pstate_changed = true;
	}

	// only process wh state if something interesting within the structure changes
	if (pstate!=NULL && (pstate_changed || sym_ismyturn))
		ProcessStateEngine(pstate, pstate_changed);

	// reset wh symbol GameState if button moves
	if (_new_hand)
	{
		for (i=0; i<_hist_sym_count; i++)
		{
			for (j=0; j<4; j++)
			{
				_hist_sym[i][j] = 0.0;
			}
		}
	}

	set_new_hand(false);

	// collect symbol if it ismyturn, or if ismanual
	if (sym_ismyturn || sym_ismanual)
	{
		for (i=0; i<_hist_sym_count; i++)
		{
			int e = SUCCESS;
			_hist_sym[i][sym_br-1] = p_symbols->GetSymbolVal(_hist_sym_strings[i], &e);
		}
	}
}

void CGameState::ProcessFtr(const SHoldemState *pstate)
{
	ProcessFtrEngine(pstate);
}


void CGameState::CaptureState(const char *title)
{
	int					i = 0, j = 0;
	bool				playing = true;
	unsigned char		card = CARD_NOCARD;

	// figure out if I am playing
	int sym_chair = (int) p_symbols->sym()->chair;
	if (!p_symbols->user_chair_confirmed())
	{
		playing = false;
	}
	else if (p_scraper->card_player(sym_chair, 0) == CARD_BACK || 
			 p_scraper->card_player(sym_chair, 1) == CARD_BACK ||
			 p_scraper->card_player(sym_chair, 0) == CARD_NOCARD || 
			 p_scraper->card_player(sym_chair, 1) == CARD_NOCARD)
	{
		playing = false;
	}

	// Poker window title
	strncpy_s(_state[_state_index&0xff].m_title, 64, title, _TRUNCATE);
	_state[_state_index&0xff].m_title[63] = '\0';

	// Pot information
	for (i=0; i<=9; i++)
		_state[_state_index&0xff].m_pot[i] = p_scraper->pot(i);

	// Common cards
	for (i=0; i<=4; i++)
	{
		if (p_scraper->card_common(i) == CARD_BACK)
		{
			card = WH_CARDBACK;
		}
		else if (p_scraper->card_common(i) == CARD_NOCARD)
		{
			card = WH_NOCARD;
		}
		else
		{
			card = ((StdDeck_RANK(p_scraper->card_common(i))+2)<<4) |
					(StdDeck_SUIT(p_scraper->card_common(i)) == StdDeck_Suit_CLUBS ? WH_SUIT_CLUBS :
					 StdDeck_SUIT(p_scraper->card_common(i)) == StdDeck_Suit_DIAMONDS ? WH_SUIT_DIAMONDS :
					 StdDeck_SUIT(p_scraper->card_common(i)) == StdDeck_Suit_HEARTS ? WH_SUIT_HEARTS :
					 StdDeck_SUIT(p_scraper->card_common(i)) == StdDeck_Suit_SPADES ? WH_SUIT_SPADES : 0) ;
		}

		_state[_state_index&0xff].m_cards[i] = card;
	}

	// playing, posting, dealerchair
	int sym_dealerchair = (int) p_symbols->sym()->dealerchair;
	bool sym_isautopost = (bool) p_symbols->sym()->isautopost;
	_state[_state_index&0xff].m_is_playing = playing;
	_state[_state_index&0xff].m_is_posting = sym_isautopost;
	_state[_state_index&0xff].m_fillerbits = 0;
	_state[_state_index&0xff].m_fillerbyte = 0;
	_state[_state_index&0xff].m_dealer_chair = sym_dealerchair;

	// loop through all 10 player chairs
	for (i=0; i<=9; i++)
	{

		// player name, balance, currentbet
		strncpy_s(_state[_state_index&0xff].m_player[i].m_name, 16, p_scraper->player_name(i).GetString(), _TRUNCATE);
		_state[_state_index&0xff].m_player[i].m_balance = p_symbols->sym()->balance[i];
		_state[_state_index&0xff].m_player[i].m_currentbet = p_symbols->sym()->currentbet[i];

		// player cards
		for (j=0; j<=1; j++)
		{
			if (p_scraper->card_player(i, j) == CARD_BACK)
			{
				card = WH_CARDBACK;
			}
			else if (p_scraper->card_player(i, j) == CARD_NOCARD)
			{
				card = WH_NOCARD;
			}
			else
			{
				card = ((StdDeck_RANK(p_scraper->card_player(i, j))+2)<<4) |
					   (StdDeck_SUIT(p_scraper->card_player(i, j)) == StdDeck_Suit_CLUBS ? WH_SUIT_CLUBS :
						StdDeck_SUIT(p_scraper->card_player(i, j)) == StdDeck_Suit_DIAMONDS ? WH_SUIT_DIAMONDS :
						StdDeck_SUIT(p_scraper->card_player(i, j)) == StdDeck_Suit_HEARTS ? WH_SUIT_HEARTS :
						StdDeck_SUIT(p_scraper->card_player(i, j)) == StdDeck_Suit_SPADES ? WH_SUIT_SPADES : 0) ;
			}

			_state[_state_index&0xff].m_player[i].m_cards[j] = card;
		}

		// player name known, balance known
		_state[_state_index&0xff].m_player[i].m_name_known = p_scraper->name_good_scrape(i) ? 1 : 0;
		_state[_state_index&0xff].m_player[i].m_balance_known = p_scraper->balance_good_scrape(i) ? 1 : 0;
		_state[_state_index&0xff].m_player[i].m_fillerbits = 0;
		_state[_state_index&0xff].m_player[i].m_fillerbyte = 0;
	}

	_state_index++;
}

const int CGameState::LastRaised(const int round)
{
	int last_raised = -1;
	int i = 0;

	if (round<1 || round>4)
		return last_raised;

	for (i=_m_game_state[(_m_game_ndx)&0xff].m_dealer_chair+1; i<=_m_game_state[(_m_game_ndx)&0xff].m_dealer_chair+10; i++)
	{
		if (_chair_actions[i%10][round-1][w_reraised])
		{
			last_raised=i%10;
		}
	}

	if (last_raised==-1)
	{
		for (i=_m_game_state[(_m_game_ndx)&0xff].m_dealer_chair+1; i<=_m_game_state[(_m_game_ndx)&0xff].m_dealer_chair+10; i++)
		{
			if (_chair_actions[i%10][round-1][w_raised])
			{
				last_raised=i%10;
			}
		}
	}

	return last_raised;
}

const int CGameState::RaisBits(const int round)
{
	int i = 0, bits = 0;

	for (i=0; i<=9; i++)
	{
		if (_chair_actions[i][round-1][w_raised] ||
				_chair_actions[i][round-1][w_reraised])
		{
			bits|=_exponents[i];
		}
	}
	return bits;
}

const int CGameState::CallBits(const int round)
{
	int i = 0, bits = 0;

	for (i=0; i<=9; i++)
	{
		if (_chair_actions[i][round-1][w_called] ||
				_chair_actions[i][round-1][w_posted_bb])
		{
			bits|=_exponents[i];
		}
	}
	return bits;
}

const int CGameState::FoldBits(const int round)
{
	int i = 0, bits = 0;

	for (i=0; i<=9; i++)
	{
		if (_chair_actions[i][round-1][w_folded])
		{
			bits|=_exponents[i];
		}
	}
	return bits;
}

const double CGameState::FlopPct(void)
{
	int		i = 0;
	int		elapsed_start = 0, num_dealt = 0, hands = 0, num_saw_cards = 0;
	double	percent = 0.;

	if (_m_ftr_ndx>1)
	{
		for (i=_m_ftr_ndx-1; i>=1; i--)
		{
			if (_m_ftr[i&0xff].elapsed_time!=0 && _m_ftr[i&0xff].n_pl_dealt>1)
			{
				if (elapsed_start==0)
					elapsed_start=_m_ftr[i&0xff].elapsed_time;

				if (elapsed_start-_m_ftr[i&0xff].elapsed_time<prefs.sym_av_time()*60)
				{
					num_dealt+=_m_ftr[i&0xff].n_pl_dealt;
					hands++;
					num_saw_cards+=_m_ftr[i&0xff].n_pl_saw_flop;
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
		percent=0;

	return percent;
}

const double CGameState::TurnPct(void)
{
	int		i = 0;
	int		elapsed_start = 0, num_dealt = 0, hands = 0, num_saw_cards = 0;
	double	percent = 0.;

	if (_m_ftr_ndx>1)
	{
		for (i=_m_ftr_ndx-1; i>=1; i--)
		{
			if (_m_ftr[i&0xff].elapsed_time!=0 && _m_ftr[i&0xff].n_pl_dealt>1)
			{
				if (elapsed_start==0)
					elapsed_start=_m_ftr[i&0xff].elapsed_time;

				if (elapsed_start-_m_ftr[i&0xff].elapsed_time<prefs.sym_av_time()*60)
				{
					num_dealt+=_m_ftr[i&0xff].n_pl_dealt;
					hands++;
					num_saw_cards+=_m_ftr[i&0xff].n_pl_saw_turn;
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
		percent=0;

	return percent;
}

const double CGameState::RiverPct(void)
{
	int		i = 0;
	int		elapsed_start = 0, num_dealt = 0, hands = 0, num_saw_cards = 0;
	double	percent = 0.;

	if (_m_ftr_ndx>1)
	{
		for (i=_m_ftr_ndx-1; i>=1; i--)
		{
			if (_m_ftr[i&0xff].elapsed_time!=0 && _m_ftr[i&0xff].n_pl_dealt>1)
			{
				if (elapsed_start==0)
					elapsed_start=_m_ftr[i&0xff].elapsed_time;

				if (elapsed_start-_m_ftr[i&0xff].elapsed_time<prefs.sym_av_time()*60)
				{
					num_dealt+=_m_ftr[i&0xff].n_pl_dealt;
					hands++;
					num_saw_cards+=_m_ftr[i&0xff].n_pl_saw_river;
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
		percent=0;

	return percent;
}

const double CGameState::AvgBetsPf(void)
{
	int		i = 0;
	int		elapsed_start = 0, num_dealt = 0, hands = 0, num_saw_cards = 0;
	double	percent = 0., bets_preflop = 0.;

	if (_m_ftr_ndx>1)
	{
		for (i=_m_ftr_ndx-1; i>=1; i--)
		{
			if (_m_ftr[i&0xff].elapsed_time!=0 && _m_ftr[i&0xff].n_pl_dealt>1)
			{
				if (elapsed_start==0)
					elapsed_start=_m_ftr[i&0xff].elapsed_time;

				if (elapsed_start-_m_ftr[i&0xff].elapsed_time<prefs.sym_av_time()*60)
				{
					num_dealt+=_m_ftr[i&0xff].n_pl_dealt;
					hands++;
					bets_preflop+=_m_ftr[i&0xff].n_bets_preflop;
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
		percent=0;

	return percent;
}

const double CGameState::TablePfr(void)
{
	int		i = 0;
	int		elapsed_start = 0, num_dealt = 0, hands = 0, num_saw_cards = 0, bets_preflop = 0;
	double	percent = 0.;

	if (_m_ftr_ndx>1) 
	{

		for (i=_m_ftr_ndx-1; i>=1; i--)
		{
			if (_m_ftr[i&0xff].elapsed_time!=0 && _m_ftr[i&0xff].n_pl_dealt>1)
			{
				if (elapsed_start==0)
					elapsed_start=_m_ftr[i&0xff].elapsed_time;

				if (elapsed_start-_m_ftr[i&0xff].elapsed_time<prefs.sym_av_time()*60)
				{
					num_dealt+=_m_ftr[i&0xff].n_pl_dealt;
					hands++;

					//Either there was more than one raise or no one saw a flop (hence a raise)
					if (_m_ftr[i&0xff].n_bets_preflop > 1 || _m_ftr[i&0xff].n_pl_saw_flop == 0)
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
		percent=0;

	return percent;
}

const double CGameState::OHSymHist(const char * sym, const int round)
{
	int		i = 0;

	for (i=0; i<_hist_sym_count; i++)
	{
		if (memcmp(sym, _hist_sym_strings[i], strlen(sym))==0 && strlen(sym)==strlen(_hist_sym_strings[i]))
		{
			return _hist_sym[i][round-1];
		}
	}

	return 0.0;
}

const double CGameState::SortedBalance(const int rank)
{
	int		i = 0, n = 0;
	double	stacks[10] = {0.};
	double	temp = 0.;

	for (int i=0; i<=9; i++)
		stacks[i] = _m_holdem_state[_m_ndx].m_player[i].m_balance + _m_holdem_state[_m_ndx].m_player[i].m_currentbet;

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
}

// Return true if the game state processor should process the current frame.
// Also used to stall the autoplayer until after the game state processor has
// processed the current frame.
bool CGameState::ProcessThisFrame (void)
{
	bool			balance_stability = false;
	int				i;
	int				sym_br = (int) p_symbols->sym()->br;
	bool			sym_ismanual = (bool) p_symbols->sym()->ismanual;

	// check if all balances are known (indicates stability of info passed to DLL)
	balance_stability = true;
	for (i=0; i<10; i++)
	{
		if (_m_holdem_state[(_m_ndx)&0xff].m_player[i].m_cards[0] != 0 && 
			_m_holdem_state[(_m_ndx)&0xff].m_player[i].m_cards[1] != 0 &&
			_m_holdem_state[(_m_ndx)&0xff].m_player[i].m_balance_known != 1 &&
			prefs.need_balance_stability()==true)
		{
			balance_stability = false;
			break;
		}
	}

	// only process further if _safe_to_process_state==true, userchair is identified (br!=0),
	// and m_balance_known is true for all players with cards, and I am actually in the hand
	return ((sym_br != 0 && _safe_to_process_state && (balance_stability || sym_ismanual))
			|| _m_holdem_state[(_m_ndx)&0xff].m_dealer_chair != _m_holdem_state[(_m_ndx-1)&0xff].m_dealer_chair);
}

void CGameState::ProcessStateEngine(const SHoldemState *pstate, const bool pstate_changed)
{
	int				from_chair = 0, to_chair = 0;
	int				i = 0, j = 0, k = 0;
	int				sym_br = (int) p_symbols->sym()->br;
	int				sym_userchair = (int) p_symbols->sym()->userchair;
	bool			sym_ismyturn = (bool) p_symbols->sym()->ismyturn;
	double			sym_balance = p_symbols->sym()->balance[10];
	double			sym_handnumber = p_symbols->sym()->handnumber;
	double			sym_sblind = p_symbols->sym()->sblind;
	double			sym_bblind = p_symbols->sym()->bblind;

	_m_holdem_state[ (++_m_ndx)&0xff ] = *pstate;

	// Only start processing state info, if we have one solid frame recorded (many calcs require _m_ndx-1)
	if (_safe_to_process_state == false && _m_ndx>1)
		_safe_to_process_state = true;

	if (CGameState::ProcessThisFrame ())
	{

		// Check for end of hand situation
		for (i=0; i<10; i++)
		{
			// if new card fronts have appeared, then players are showing down, and its the end of the hand
			if ( _m_holdem_state[(_m_ndx-1)&0xff].m_player[i%10].m_cards[0] == 255 &&		// was card backs last scrape
					_m_holdem_state[(_m_ndx-1)&0xff].m_player[i%10].m_cards[1] == 255 &&		// was card backs last scrape
					_m_holdem_state[(_m_ndx)&0xff].m_player[i%10].m_cards[0] != 255 &&		// is not card backs this scrape
					_m_holdem_state[(_m_ndx)&0xff].m_player[i%10].m_cards[1] != 255 &&		// is not card backs this scrape
					_m_holdem_state[(_m_ndx)&0xff].m_player[i%10].m_cards[0] != 0 &&			// is not 'no cards' this scrape
					_m_holdem_state[(_m_ndx)&0xff].m_player[i%10].m_cards[1] != 0 &&			// is not 'no cards' this scrape
					i != sym_userchair &&												// it's not me
					_end_of_hand==false )
			{
				_end_of_hand = true;
				write_log(1, ">>> SHOWDOWN\n");
			}
		}

		// save state information if one of the following is true:
		//   1. a new hand has started
		//	 2. it's my turn
		//   3. the betting round has changed
		//   4. we are in manual mode

		// 1. new hand has started
		if (_new_hand || _first_pass_capture)
		{

			// save the game state
			_m_game_state[ (++_m_game_ndx)&0xff ] = *pstate;

			//reset some vars
			_first_pass_capture = false;
			_oh_br_last=1;
			_pot_raised = false;
			_pf_limpers_n = 0;
			_my_first_action_this_round = true;
			_my_first_action_this_hand = true;
			_small_blind_posted = false;
			_big_blind_posted = false;
			_bets_last = 0.0;
			_end_of_hand = false;
			for (i=0; i<10; i++)
			{
				for (j=0; j<4; j++)
				{
					for (k=0; k<w_num_action_types; k++)
						_chair_actions[i][j][k] = w_noaction;
				}
			}

			// Track some stats
			_hands_played++;

			write_log(1, ">>> New hand %.0f\n", sym_handnumber);
		}

		// first time to act in the hand//
		if (sym_ismyturn && _my_first_action_this_hand == true)
		{
			_my_first_action_this_hand = false;
		}

		// 2. it's my turn
		if ((sym_ismyturn && pstate_changed) ||
				(sym_ismyturn && _my_first_action_this_round == true) ||
				(((sym_br == 1 &&
				   _m_holdem_state[(_m_ndx)&0xff].m_player[sym_userchair].m_currentbet == sym_sblind) ||
				  (sym_br == 1 &&
				   _m_holdem_state[(_m_ndx)&0xff].m_player[sym_userchair].m_currentbet == sym_bblind)) &&
				 _my_first_action_this_round == true) )
		{

			// save the game state
			_m_game_state[ (++_m_game_ndx)&0xff ] = *pstate;

			//reset some vars
			if (_m_holdem_state[(_m_ndx)&0xff].m_player[sym_userchair].m_cards[0] != 0 &&
				_m_holdem_state[(_m_ndx)&0xff].m_player[sym_userchair].m_cards[1] != 0)
			{
				_process_game_state = true;
				write_log(1, ">>> My turn, br=%d\n", sym_br);
			}
			else
			{
				_process_game_state = false;
			}
		}

		// 3. the betting round has changed
		if (sym_br > _oh_br_last)
		{
			// save the game state
			_m_game_state[ (++_m_game_ndx)&0xff ] = *pstate;

			//reset some vars
			_oh_br_last = sym_br;
			_pot_raised = false;
			_my_first_action_this_round = true;
			_bets_last = 0.0;

			//write_log(">>> Betting round: %d\n", (int) sym_br);
		}

		// it's my turn, so I need to figure out what everyone did before me
		if (_process_game_state)
		{
			_process_game_state = false;

			// figure out how to iterate through the chairs
			// if it is my first action this round
			if (_my_first_action_this_round == true)
			{
				_my_first_action_this_round=false;

				// if i am the sb, then iterate just mychair
				if (sym_br == 1 && _m_game_state[(_m_game_ndx)&0xff].m_player[sym_userchair].m_currentbet == sym_sblind)
				{
					from_chair = sym_userchair;
					to_chair = sym_userchair-1+10;
				}

				// if i am the bb, then iterate from dlr+1 to mychair
				else if (sym_br == 1 && _m_game_state[(_m_game_ndx)&0xff].m_player[sym_userchair].m_currentbet == sym_bblind)
				{
					from_chair = _m_game_state[(_m_game_ndx)&0xff].m_dealer_chair+1;
					to_chair = sym_userchair-1+10;
				}

				// if the dlr chair < mychair, then iterate from dlr+1 to mychair-1
				else if (_m_game_state[(_m_game_ndx)&0xff].m_dealer_chair < sym_userchair)
				{
					from_chair = _m_game_state[(_m_game_ndx)&0xff].m_dealer_chair+1;
					to_chair = sym_userchair-1;
				}

				// if the dlr chair >= mychair, then iterate from dlr+1 to mychair-1+10 (to loop around)
				else
				{
					from_chair = _m_game_state[(_m_game_ndx)&0xff].m_dealer_chair+1;
					to_chair = sym_userchair-1+10;
				}

			}
			// if it is not my first action this round, then iterate from mychair+1 to mychair-1+10
			else
			{
				from_chair = sym_userchair+1;
				to_chair = sym_userchair-1+10;
			}

			// now iterate through the chairs and see what everybody did
			for (i = from_chair; i <= to_chair; i++)
			{
				// if the currentbet for the chair is the sb and the last bet was zero and br==1
				// and the player has cards, then we know the chair ***POSTED THE SMALL BLIND***
				if (_m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet == sym_sblind &&
						_bets_last==0 &&
						_m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[0]!=0 &&
						_m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[1]!=0 &&
						sym_br == 1)
				{
					_chair_actions[i%10][sym_br-1][w_posted_sb] = true;
					_bets_last = _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet;
					write_log(1, ">>> Chair %d (%s) posted the sb: $%.2f\n", i%10,
							  _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_name,
							  _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet);
				}

				// if the currentbet for the chair is the bb and the last bet was the sb and br==1
				// and the player has cards, then we know the chair ***POSTED THE BIG BLIND***
				else if (_m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet == sym_bblind &&
						 _bets_last == sym_sblind &&
						 _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[0]!=0 &&
						 _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[1]!=0 &&
						 sym_br == 1)
				{
					_chair_actions[i%10][(int) sym_br-1][w_posted_bb] = true;
					_bets_last = _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet;
					write_log(1, ">>> Chair %d (%s) posted the bb: $%.2f\n", i%10,
							  _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_name,
							  _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet);
				}

				// if the currentbet for the chair is greater than the last bet and it's not the end of the hand,
				// and the player has cards, then we know the chair ***RAISED or RE-RAISED***
				else if (_m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet > _bets_last &&
						 _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[0]!=0 &&
						 _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[1]!=0 &&
						 !_end_of_hand)
				{
					_bets_last = _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet;
					if (_pot_raised == false)
					{
						_chair_actions[i%10][sym_br-1][w_raised] = true;
						_pot_raised = true;
						write_log(1, ">>> Chair %d (%s) raised to $%.2f\n", i%10,
								  _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_name,
								  _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet);
					}
					else
					{
						_chair_actions[i%10][sym_br-1][w_reraised] = true;
						write_log(1, ">>> Chair %d (%s) re-raised to $%.2f\n", i%10,
								  _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_name,
								  _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet);
					}
				}

				// if the currentbet for the chair is not zero and is <= the last bet and it's not the end of the hand,
				// and the player has cards, then we know the chair ***CALLED***
				else if (_m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet != 0 &&
						 _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet <= _bets_last &&
						 _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[0]!=0 &&
						 _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[1]!=0 &&
						 !_end_of_hand)
				{
					_chair_actions[i%10][sym_br-1][w_called] = true;
					if (_pot_raised == false)
					{
						_pf_limpers_n += 1;
					}
					write_log(1, ">>> Chair %d (%s) called\n", i%10, _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_name);
				}

				// if cards have disappeared, and they were card backs last scrape,
				// then we know the chair has ***FOLDED***
				// unless i=userchair, in which case they could be card fronts last scrape
				// in any case, if the balance in front of the chair has increased, it is not a fold, but a holdover from showdown
				else if ( ( _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[0]==0 &&			// no cards this scrape
							_m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[1]==0 &&			// no cards this scrape
							_m_game_state[(_m_game_ndx-1)&0xff].m_player[i%10].m_cards[0]==255 &&		// card backs last scrape
							_m_game_state[(_m_game_ndx-1)&0xff].m_player[i%10].m_cards[1]==255 &&		// card backs last scrape
							_m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_balance==
							_m_game_state[(_m_game_ndx-1)&0xff].m_player[i%10].m_balance)
						  ||
						  ( i%10 == sym_userchair &&												// this is my chair
							_m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[0]==0 &&			// no cards this scrape
							_m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[1]==0 &&			// no cards this scrape
							_m_game_state[(_m_game_ndx-1)&0xff].m_player[i%10].m_cards[0]!=0 &&		// card fronts/backs last scrape
							_m_game_state[(_m_game_ndx-1)&0xff].m_player[i%10].m_cards[1]!=0 &&		// card fronts/backs last scrape
							_m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_balance==
							_m_game_state[(_m_game_ndx-1)&0xff].m_player[i%10].m_balance) )
				{
					_chair_actions[i%10][sym_br-1][w_folded] = true;
					write_log(1, ">>> Chair %d (%s) folded\n", i%10,
							  _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_name);
				}

				// if br!=1 and the player's current bet is zero, and that player has cards in front of them
				// then we know the chair has ***CHECKED***
				else if (sym_br!=1 && _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_currentbet == 0 &&
						 _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[0]!=0 &&
						 _m_game_state[(_m_game_ndx)&0xff].m_player[i%10].m_cards[1]!=0)
				{
					_chair_actions[i%10][sym_br-1][w_checked] = true;

					write_log(1, ">>> Chair %d (%s) checked\n", i%10, _m_holdem_state[(_m_ndx)&0xff].m_player[i%10].m_name);
				}
			}  // end of "for (i = from_chair; i <= to_chair; i++)"
		} // end of "if (br != 0 &&..."
	} // end of "if (_process_game_state)"
}

void CGameState::ProcessFtrEngine(const SHoldemState *pstate)
{
	double			sym_elapsed = p_symbols->sym()->elapsed;
	double			sym_nbetsround1 = p_symbols->sym()->nbetsround[0];
	int				sym_nplayersdealt = (int) p_symbols->sym()->nplayersdealt;
	int				sym_br = (int) p_symbols->sym()->br;
	int				sym_nflopc = (int) p_symbols->sym()->nflopc;
	int				sym_nplayersplaying = (int) p_symbols->sym()->nplayersplaying;

	// if a new hand has started setup the next element in the ftr tracker array
	if (pstate->m_dealer_chair != _ftr_dealer_chair_last)
	{
		_m_ftr_ndx++;
		_m_ftr[_m_ftr_ndx&0xff].elapsed_time = sym_elapsed;
		_m_ftr[_m_ftr_ndx&0xff].n_pl_dealt = 0;
		_m_ftr[_m_ftr_ndx&0xff].n_pl_saw_flop = 0;
		_m_ftr[_m_ftr_ndx&0xff].n_pl_saw_turn = 0;
		_m_ftr[_m_ftr_ndx&0xff].n_pl_saw_river = 0;
		_m_ftr[_m_ftr_ndx&0xff].n_bets_preflop = 0;
		_ftr_nplayersdealt_last=0;
		_ftr_nflopc_last=0;
		_ftr_dealer_chair_last=pstate->m_dealer_chair;
	}

	// if nplayersdealt has incremented and it is br1, update the ftr tracker stats
	if (sym_nplayersdealt != _ftr_nplayersdealt_last &&
		sym_nplayersdealt > _ftr_nplayersdealt_last &&
		sym_br==1)
	{
		_m_ftr[_m_ftr_ndx&0xff].n_pl_dealt = sym_nplayersdealt;
		_ftr_nplayersdealt_last = sym_nplayersdealt;
	}

	// if the betting round has changed update the ftr tracker stats
	if (sym_nflopc > _ftr_nflopc_last)
	{
		if (sym_nflopc==3 && _m_ftr[_m_ftr_ndx&0xff].n_pl_saw_flop==0)
		{
			_m_ftr[_m_ftr_ndx&0xff].n_pl_saw_flop = sym_nplayersplaying;
			_m_ftr[_m_ftr_ndx&0xff].n_bets_preflop = sym_nbetsround1;
		}

		if (sym_nflopc==4 && _m_ftr[_m_ftr_ndx&0xff].n_pl_saw_turn==0)
		{
			_m_ftr[_m_ftr_ndx&0xff].n_pl_saw_turn = sym_nplayersplaying;
		}

		if (sym_nflopc==5 && _m_ftr[_m_ftr_ndx&0xff].n_pl_saw_river==0)
		{
			_m_ftr[_m_ftr_ndx&0xff].n_pl_saw_river = sym_nplayersplaying;
		}

		_ftr_nflopc_last = sym_nflopc;
	}
}

void CGameState::DumpState(void)
{
	int			i = 0;

	write_log(3, "_m_ndx: %d\n", _m_ndx);
	write_log(3, "m_title: %s\n", _m_holdem_state[(_m_ndx)&0xff].m_title);
	write_log(3, "m_pot: %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n", _m_holdem_state[(_m_ndx)&0xff].m_pot[0], _m_holdem_state[(_m_ndx)&0xff].m_pot[1],
			  _m_holdem_state[(_m_ndx)&0xff].m_pot[2], _m_holdem_state[(_m_ndx)&0xff].m_pot[3], _m_holdem_state[(_m_ndx)&0xff].m_pot[4],
			  _m_holdem_state[(_m_ndx)&0xff].m_pot[5], _m_holdem_state[(_m_ndx)&0xff].m_pot[6], _m_holdem_state[(_m_ndx)&0xff].m_pot[7],
			  _m_holdem_state[(_m_ndx)&0xff].m_pot[8], _m_holdem_state[(_m_ndx)&0xff].m_pot[9]);
	write_log(3, "m_cards: %d %d %d %d %d\n", _m_holdem_state[(_m_ndx)&0xff].m_cards[0], _m_holdem_state[(_m_ndx)&0xff].m_cards[1],
			  _m_holdem_state[(_m_ndx)&0xff].m_cards[2], _m_holdem_state[(_m_ndx)&0xff].m_cards[3], _m_holdem_state[(_m_ndx)&0xff].m_cards[4]);
	write_log(3, "m_is_playing: %d\n", _m_holdem_state[(_m_ndx)&0xff].m_is_playing);
	write_log(3, "m_is_posting: %d\n", _m_holdem_state[(_m_ndx)&0xff].m_is_posting);
	write_log(3, "m_dealer_chair: %d\n", _m_holdem_state[(_m_ndx)&0xff].m_dealer_chair);
	for (i=0; i<10; i++) {
		write_log(3, "m_player[%d].m_name:%s  ", i, _m_holdem_state[(_m_ndx)&0xff].m_player[i].m_name);
		write_log(3, "m_balance:%.2f  ", _m_holdem_state[(_m_ndx)&0xff].m_player[i].m_balance);
		write_log(3, "m_currentbet:%.2f  ", _m_holdem_state[(_m_ndx)&0xff].m_player[i].m_currentbet);
		write_log(3, "m_cards:%d/%d  ", _m_holdem_state[(_m_ndx)&0xff].m_player[i].m_cards[0],
				  _m_holdem_state[(_m_ndx)&0xff].m_player[i].m_cards[1]);
		write_log(3, "m_name_known:%d  ", _m_holdem_state[(_m_ndx)&0xff].m_player[i].m_name_known);
		write_log(3, "m_balance_known:%d\n", _m_holdem_state[(_m_ndx)&0xff].m_player[i].m_balance_known);
	}
}

unsigned int CGameState::_exponents[11]={1,2,4,8,16,32,64,128,256,512,1024};

const char *CGameState::_hist_sym_strings[200] = 
{
	//PROBABILITIES (3)
	"prwin", "prlos", "prtie", 
	
	//CHIP AMOUNTS (21)
	"balance", "balance0", "balance1", "balance2", "balance3", "balance4", "balance5", 
	"balance6", "balance7", "balance8", "balance9", "stack0", "stack1", "stack2", "stack3", 
	"stack4", "stack5", "stack6", "stack7", "stack8", "stack9", 

	//POKER VALUES (5)
	"pokerval", "pokervalplayer", "pokervalcommon", "pcbits", "npcbits", 

	//HAND TESTS (13)
	"ishandup", "ishandupcommon", "ishicard", "isonepair", "istwopair", "isthreeofakind", 
	"isstraight", "isflush", "isfullhouse", "isfourofakind", "isstraightflush", "isroyalflush", 
	"isfiveofakind", 

	//POCKET/COMMON TESTS (5)
	"ishipair", "islopair", "ismidpair", "ishistraight", "ishiflush", 

	//(UN)KNOWN CARDS (2)
	"nouts", "ncardsbetter", 

	//NHANDS (6)
	"nhands", "nhandshi", "nhandslo", "nhandsti", "prwinnow", "prlosnow", 

	//FLUSHES SETS STRAIGHTS (16)
	"nsuited", 	"nsuitedcommon", "tsuit", "tsuitcommon", "nranked", "nrankedcommon", "trank", 
	"trankcommon", "nstraight", "nstraightcommon", "nstraightfill", "nstraightfillcommon", 
	"nstraightflush", "nstraightflushcommon", "nstraightflushfill", "nstraightflushfillcommon", 

	//RANK BITS (8)
	"rankbits", "rankbitscommon", "rankbitsplayer", "rankbitspoker", "srankbits", 
	"srankbitscommon", "srankbitsplayer", "srankbitspoker", 

	//RANK HI (8)
	"rankhi", "rankhicommon", "rankhiplayer", "rankhipoker", "srankhi", "srankhicommon", 
	"srankhiplayer", "srankhipoker", 

	//RANK LO (8)
	"ranklo", "ranklocommon", "rankloplayer", "ranklopoker", "sranklo", "sranklocommon", 
	"srankloplayer", "sranklopoker", 

	//run$ ron$ (26)
	"ron$royfl", "ron$strfl", "ron$4kind", "ron$fullh", "ron$strai", "ron$3kind", "ron$2pair", 
	"ron$1pair", "ron$hcard", "ron$total", "ron$pokervalmax", "ron$prnuts", "ron$prbest", 
	"run$royfl", "run$strfl", "run$4kind", "run$fullh", "run$strai", "run$3kind", "run$2pair", 
	"run$1pair", "run$hcard", "run$total", "run$pokervalmax", "run$prnuts", "run$prbest"
};
