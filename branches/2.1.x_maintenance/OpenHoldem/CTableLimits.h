#ifndef INC_CTABLELIMITS_H
#define INC_CTABLELIMITS_H

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
	void CalcTableLimits();
	void LockBlindsManually(double small_blind, double big_blind, double big_bet);
public:
	// public accessors, formerly part of the symbol-structure.
	double sblind();
	double bblind();
	double bbet();
private:
	void SetSmallBlind(double small_blind);
	void SetBigBlind(double big_blind);
	void SetBigBet(double big_bet);
	void SetAnte(double ante);
	void SetGametype(int gametype);
	// public accessors
public:
	// private functions
private:
	void AutoLockBlinds();
	void AutoLockBlindsForCurrentHand();
	void AutoLockBlindsForCashgamesAfterNHands();
	bool ReasonableBlindsForCurrentHand();
	void RememberBlindsForCashgames();
	void UnLockBlindsManually();
	bool FirstActionThisHandAndEnoughStableFrames();
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
	double tablelimits_first_N_hands_sblind[k_number_of_hands_to_autolock_blinds_for_cashgames];
	double tablelimits_first_N_hands_bblind[k_number_of_hands_to_autolock_blinds_for_cashgames];
	double tablelimits_first_N_hands_bbet[k_number_of_hands_to_autolock_blinds_for_cashgames];
private:
	int gametype;
} *p_tablelimits;

#endif // INC_CTABLELIMITS_H