//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#ifndef INC_CHANDRESETDETECTOR_H
#define INC_CHANDRESETDETECTOR_H

#include "MagicNumbers.h"

extern class CHandresetDetector
{
public:
	CHandresetDetector();
	~CHandresetDetector();
public:
	// OnNewHeartbeat(): to be called on every new heartbeat
	// BEFORE IsHandreset() gets called.
	void OnNewHeartbeat();
	bool IsHandreset()		{ return _is_handreset_on_this_heartbeat; }
public:
	// Only for output in the log
	CString GetHandNumber();
private: 
	bool CalculateIsHandreset();
	bool IsHandresetByDealerChair();
	bool IsHandresetByCards();
	bool IsHandresetByHandNumber();
private:
	bool IsValidHandNumber(CString handnumber);
	bool IsValidDealerChair(int dealerchair);
private:
	void GetNewSymbolValues();
	void StoreOldValuesForComparisonOnNextHeartbeat();
private:
	int dealerchair;
	int last_dealerchair;
	int playercards[k_number_of_cards_per_player];
	int last_playercards[k_number_of_cards_per_player];
	// Handnumber should be a string, as
	//   * it may contain characters
	//   * its lengths my exceed the precision of double
	CString handnumber;
	CString last_handnumber;
private:
	bool _is_handreset_on_this_heartbeat;
} *p_handreset_detector;

#endif INC_CHANDRESETDETECTOR_H