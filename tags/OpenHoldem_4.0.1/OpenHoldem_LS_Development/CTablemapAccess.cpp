#include "stdafx.h"
#include "CTableMapAccess.h"

#include "../CTablemap/CTablemap.h"
#include "CScraper.h"

CTablemapAccess *p_tablemap_access = NULL;

CTablemapAccess::CTablemapAccess()
{
}

CTablemapAccess::~CTablemapAccess()
{
}

bool CTablemapAccess::GetTableMapRegionByName(CString name, RMapCI &wanted_region)
{
	wanted_region = p_tablemap->r$()->find(name);
	return (wanted_region != p_tablemap->r$()->end());
}

bool CTablemapAccess::DoesTableMapRegionExist(CString name)
{
	RMapCI wanted_region;
	return (GetTableMapRegionByName(name, wanted_region));
}

// Checks the existence of a button, where "button_code"
// is a constant like "k_button_check", but not the buttons index in the TM.
bool CTablemapAccess::DoesButtonExist(int button_code)
{
	printf("DoesButtonExist: %d\n", button_code); 
	for (int i=0; i<k_max_number_of_buttons; i++)
	{
		if (DoesButtonLabelMatch(p_scraper->button_label(i), button_code))
		{
			printf("Button %d matches\n", i); 
			return true;
		}
	}
	// No button with appropriate label found.
	return false;
}

// Checks, if a certain label ("allin" or "check")
// does fit to the button_code in question (e.g. "k_button_check")
bool CTablemapAccess::DoesButtonLabelMatch(CString Label, int button_code)
{
	switch (button_code)
	{
	case k_button_i3:
		// i3-button and i86-button are a bit special,
		// as they can only be accessed by name, not by label.
		return false;
	case k_button_fold:
		return(p_scraper->IsStringFold(Label));
	case k_button_allin:
		return(p_scraper->IsStringAllin(Label));			
	case k_button_raise:
		return(p_scraper->IsStringRaise(Label));
	case k_button_call:
		return(p_scraper->IsStringCall(Label));
	case k_button_check:
		return(p_scraper->IsStringCheck(Label));
	case k_button_sitin:
		return(p_scraper->IsStringSitin(Label));
	case k_button_sitout:
		return(p_scraper->IsStringSitout(Label));
	case k_button_leave:
		return(p_scraper->IsStringLeave(Label));
	case k_button_i86:
		// i3-button and i86-button are a bit special,
		// as they can only be accessed by name, not by label.
		return false;
	default:
		// This should not happen
		printf("Unexpected button_code in CTablemapAccess::DoesButtonLabelMatch(): %d\n", button_code);
		assert(false);
		break;
	}
	return false;
}

// Returns something like "i0button", 
// when you want to get the name of "k_button_check".
CString CTablemapAccess::GetButtonName(int button_code)
{
	CString button_name = "";
	for (int i=0; i<k_max_number_of_buttons; i++)
	{
		if (DoesButtonLabelMatch(p_scraper->button_label(i), button_code))
		{
			button_name.Format("i%dbutton", button_code);
			break;
		}
	}
	printf("CTablemapAccess::GetButtonName (%d): %s\n", button_code, button_name);
	return button_name;
}

// Returns the coordinates (rectangle) of a button, if it exists.
bool CTablemapAccess::GetButtonRect(int button_code, RECT *_rect)
{
	CString button_name = GetButtonName(button_code);
	if (button_name != "")
	{
		return GetTableMapRect(button_name, _rect);
	}
	return false;
}

bool CTablemapAccess::GetButtonState(int button_code)
{
	printf("GetButtonState: %d\n", button_code); 
	for (int i=0; i<k_max_number_of_buttons; i++)
	{
		if (DoesButtonLabelMatch(p_scraper->button_label(i), button_code))
		{
			printf("Button %d matches\n", i); 
			if (p_scraper->GetButtonState(i))
			{
				printf("Buttonstate true\n"); 
				return true;
			}
			else
			{
				printf("Buttonstate false\n");
				return false;
			}
		}
	}
	// No button with appropriate label found.
	return false;
}

bool CTablemapAccess::GetTableMapRect(CString name, RECT *_rect)
{
	RMapCI wanted_region = p_tablemap->r$()->find(name);
	if (wanted_region != p_tablemap->r$()->end())
	{
		_rect->left   = wanted_region->second.left;
		_rect->top    = wanted_region->second.top;
		_rect->right  = wanted_region->second.right;
		_rect->bottom = wanted_region->second.bottom;
		return true;
	}
	return false;
}

bool CTablemapAccess::IsValidCard(int Card)
{
	return ((Card != CARD_BACK) && (Card != CARD_NOCARD) && (Card != WH_NOCARD));
}