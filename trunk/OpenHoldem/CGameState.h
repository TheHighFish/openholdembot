#ifndef INC_CGAMESTATE_H
#define INC_CGAMESTATE_H

#include "CDllExtension.h"

extern class CGameState 
{
public:
	// Critical section used in public mutators and private shared variable writes
	static CRITICAL_SECTION	cs_gamestate;

public:
	// public functions
	CGameState();
	~CGameState();
	void ProcessGameState(SHoldemState *pstate);
	void ProcessFtr(SHoldemState *pstate);
	const int LastRaised(const int round);
	const int RaisBits(const int round);
	const int CallBits(const int round);
	const int FoldBits(const int round);
	const double FlopPct(void);
	const double TurnPct(void);
	const double RiverPct(void);
	const double AvgBetsPf(void);
	const double TablePfr(void);
	const double SortedBalance(const int rank);
	const double OHSymHist(const char * sym, const int round);

public:
	// public accessors
	const int oppdealt() { return _oppdealt; }
	const int max_balance() { return _max_balance; }
	const int hands_played() { return _hands_played; }

public:
#define ENT EnterCriticalSection(&cs_gamestate);
#define LEA LeaveCriticalSection(&cs_gamestate);
	// public mutators
	void	set_new_hand(const bool b) { ENT _new_hand = b; LEA }

#undef ENT
#undef LEA

private:
	// private variables - use public accessors and public mutators to address these
	int					_oppdealt;
	double				_max_balance;
	int					_hands_played;
	bool				_new_hand;

private:
	// private functions and variables - not available via accessors or mutators
	void ProcessStateEngine(SHoldemState *pstate, bool pstate_changed) ;
	void ProcessFtrEngine(SHoldemState *pstate) ;
	void DumpState(void) ;

	SHoldemState		_m_holdem_state[256];
	unsigned char		_m_ndx;
	ftr_info			_m_ftr[256];
	int					_m_ftr_ndx;
	int					_nopponentsplaying_last;
	bool				_process_game_state;
	bool				_safe_to_process_state;
	bool				_end_of_hand;
	bool				_first_pass_capture;
	int					_oh_br_last;
	bool				_pot_raised;
	int					_pf_limpers_n;
	bool				_my_first_action_this_round;
	bool				_my_first_action_this_hand;
	bool				_small_blind_posted;
	bool				_big_blind_posted;
	double				_bets_last;


	SHoldemState		_m_game_state[256];
	unsigned char		_m_game_ndx;
	bool				_chair_actions[10][4][w_num_action_types];   // 10 chairs, 4 rounds, number of action types
	int					_ftr_dealer_chair_last;
	int					_ftr_nflopc_last;
	int					_ftr_nplayersdealt_last;


	double				_hist_sym[200][4];
	static const int	_hist_sym_count=121;
	static unsigned int	_exponents[11];
	static const char	*_hist_sym_strings[200];

} *p_game_state;

#endif //INC_CGAMESTATE_H