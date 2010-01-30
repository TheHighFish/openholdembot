#ifndef INC_CTABLELIMITS_H
#define INC_CTABLELIMITS_H

#include "MagicNumbers.h"

struct STableLimit
{
	double sblind;
	double bblind;
	double bbet;
	// We don't deal with antes for the moment,
	// as they are rarely used and of minor importance 
	// for correct table-limits, blind-sizes, etc.
};

extern class CTableLimits
{
public:
	CTableLimits();
	~CTableLimits();
	// public mutators
public:
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetEachHeartBeatCycle();
	void CalcTableLimits();
	void LockBlindsManually(double small_blind, double big_blind, double big_bet, double ante, int gametype);
	void UnLockBlindsManually();
public:
	// public accessors, formerly part of the symbol-structure.
	double sblind();
	double bblind();
	double bbet();
	double ante();
	double bet(int betround);
	double bet(); // for current betting round
public:
	bool BlindsLockedManually() { return blinds_locked_manually; }
public:
	int gametype()		{ return _gametype; }	// former p_symbols->sym()->lim
	double isnl()		{ return (gametype() == k_gametype_NL); }
	double ispl()		{ return (gametype() == k_gametype_PL); }
	double isfl()		{ return (gametype() == k_gametype_FL); }
private:
	void SetSmallBlind(double small_blind);
	void SetBigBlind(double big_blind);
	void SetBigBet(double big_bet);
	void SetAnte(double ante);
	void SetGametype(int gametype); 
	void SetBet(int betround, double bet);
	// public accessors
public:
	// private functions
private:
	void AutoLockBlinds();
	void AutoLockBlindsForCurrentHand();
	void AutoLockBlindsForCashgamesAfterNHands();
	bool ReasonableBlindsForCurrentHand();
	void RememberBlindsForCashgames();
	bool IsCalculationNeccessary();
	void CalcTableLimits_NL_PL();
	void CalcTableLimits_FL_AndUnknownGametype();
	void CalcTableLimitsFromPostedBets();
	void AdjustForReasonableness();
	void ResetBets();
	// private data members
private:
	bool blinds_locked_for_current_hand; 
	bool blinds_locked_for_complete_session;
	bool blinds_locked_manually;
private:
	STableLimit tablelimit_unreliable_input;
	STableLimit tablelimit_locked_for_current_hand;
	STableLimit tablelimit_locked_for_complete_session;
	STableLimit tablelimit_locked_manually;
private:
	static const int k_number_of_hands_to_autolock_blinds_for_cashgames = 5;
	int number_of_saved_tablelimits;
private:
	// We store the table-limits for the first N hands in separate arrays
	// instead of TableLimit structures, as we need to operate on arrays later (median function).
	double tablelimits_first_N_hands_sblind[k_number_of_hands_to_autolock_blinds_for_cashgames];
	double tablelimits_first_N_hands_bblind[k_number_of_hands_to_autolock_blinds_for_cashgames];
	double tablelimits_first_N_hands_bbet[k_number_of_hands_to_autolock_blinds_for_cashgames];
private:
	int		_gametype;
	double	_ante;
	// Index 1..4 is for current bettinground, 0 is unused
	double _betsizes_for_all_bettingrounds[k_number_of_betrounds+1]; 
} *p_tablelimits;

#endif // INC_CTABLELIMITS_H