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
	void LockBlindsManually();
private:
	void SetSmallBlind();
	void SetBigBlind();
	void SetSmallBet();
	void SetBigBet();
	// public accessors
public:
	// private functions
private:
	void AutoLockBlindsForCashgamesAfterNHands();
	void LockBlinds();
	// private data members
private:
} *p_tablelimits;

#endif // INC_CTABLELIMITS_H