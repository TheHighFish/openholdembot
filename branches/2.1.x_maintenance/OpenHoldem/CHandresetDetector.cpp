#include "stdafx.h"
#include "CHandresetDetector.h"

#include "CScraper.h"
#include "CSymbols.h"
#include "..\CTablemap\CTablemap.h"


CHandresetDetector *p_handreset_detector = NULL;


CHandresetDetector::CHandresetDetector()
{
	playercards[0] = CARD_NOCARD;
	playercards[1] = CARD_NOCARD;
	dealerchair = -1;
	handnumber = "";
}

CHandresetDetector::~CHandresetDetector()
{
}


CString CHandresetDetector::GetHandNumber()
{
	return handnumber;
}

bool CHandresetDetector::IsHandreset()
{
	return (IsHandresetByDealerChair()
		|| IsHandresetByCards()
		|| IsHandresetByHandNumber());
}


bool CHandresetDetector::IsHandresetByDealerChair()
{
	if ((HandResetMethod() &  HANDRESET_DEALER) == 0)
	{
		// We don't want to use this method
		return false;
	}
	return (IsValidDealerChair(dealerchair) && (dealerchair != last_dealerchair));
}


bool CHandresetDetector::IsHandresetByCards()
{
	if ((HandResetMethod() &  HANDRESET_CARDS) == 0)
	{
		// We don't want to use this method
		return false;
	}
	return ((playercards[0] != CARD_NOCARD && playercards[0] != CARD_BACK && playercards[0] != last_playercards[0]) 
		||  (playercards[1] != CARD_NOCARD && playercards[1] != CARD_BACK && playercards[1] != last_playercards[1]));
}


bool CHandresetDetector::IsHandresetByHandNumber()
{
	if ((HandResetMethod() &  HANDRESET_HANDNUM) == 0)
	{
		// We don't want to use this method
		return false;
	}
	return (IsValidHandNumber(handnumber) && (handnumber != last_handnumber));
}


int CHandresetDetector::HandResetMethod()
{
	return (p_tablemap->handresetmethod());
}


bool CHandresetDetector::IsValidHandNumber(CString handnumber)
{
	int length_of_handnumber = handnumber.GetLength();
	int max_length_of_handnumber = p_tablemap->HandNumberMaxExpectedDigits();
	int min_length_of_handnumber = p_tablemap->HandNumberMinExpectedDigits(); 

	if ((min_length_of_handnumber > 0) && (max_length_of_handnumber > 0))
	{
		return ((length_of_handnumber >= min_length_of_handnumber)
			&& (length_of_handnumber <= max_length_of_handnumber));
	}
	else
	{
		// No extra requirements for handnumber: always valid
		return true;
	}
}


bool CHandresetDetector::IsValidDealerChair(int dealerchair)
{
	// Dealerchair should be -1, if not found (occlusion).
	return ((dealerchair >= 0) && (dealerchair < p_tablemap->nchairs()));
}


void CHandresetDetector::OnNewHeartbeat()
{
	// Store old values...
	last_dealerchair = dealerchair;
	for (int i=0; i<k_number_of_cards_per_player; i++)
	{
		last_playercards[i] = playercards[i];
	}
	last_handnumber = handnumber;
	// ...and set new ones (if good).
	if (IsValidDealerChair(p_symbols->sym()->dealerchair))
	{
		dealerchair = p_symbols->sym()->dealerchair;	
	}
	if (IsValidHandNumber(p_scraper->s_limit_info()->handnumber))
	{
		write_log(3, "Setting handnumber to [%s]\n", handnumber);
		handnumber = p_scraper->s_limit_info()->handnumber;	
	}
	else
	{
		write_log(3, "Setting handnumber to [%s] was skipped. Reason: [digits number not in range]\n", handnumber);
	}
	int userchair = (int) p_symbols->sym()->userchair;
	for (int i=0; i<k_number_of_cards_per_player; i++)
	{
		if ((userchair >= 0) && (userchair < p_tablemap->nchairs()))
		{
			playercards[i] = p_scraper->card_player(userchair, i);
		}
		else
		{
			playercards[i] = CARD_NOCARD;
		}
	}
}
