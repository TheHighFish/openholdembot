#ifndef INC_CTABLELIMITS_H
#define INC_CTABLELIMITS_H

extern class CTableLimits
{
public:
	CTableLimits();
	~CTableLimits();
	// public mutators
public:
	void ResetOnConnection();
	void ResetOnHandreset();
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
	// public accessors
public:
	// private functions
private:
	void AutoLockBlindsForCashgamesAfterNHands();
	void LockBlinds(double small_blind, double big_blind, double big_bet);
	void ComputeStableTableLimits();
	bool FirstActionThisHandAndEnoughStableFrames();
	// private data members
private:
	bool blinds_locked;
private:
	// "Stable" values for the current hand.
	double stable_sblind; 
	double stable_bblind;
	double stable_bigbet;
	// We don't deal with antes for the moment,
	// as they are rarely used and of minor importance 
	// for correct table-limits, blind-sizes, etc.
private:
	static const int k_number_of_hands_to_autolock_blinds = 10;
	int number_of_stable_tablelimits;
	double stable_sblind_first_n_hands[k_number_of_hands_to_autolock_blinds]; 
	double stable_bblind_first_n_hands[k_number_of_hands_to_autolock_blinds];
	double stable_bigbet_first_n_hands[k_number_of_hands_to_autolock_blinds];
private:
	double sblind_current_scrape; 
	double bblind_current_scrape;
	double bigbet_current_scrape;
} *p_tablelimits;

#endif // INC_CTABLELIMITS_H