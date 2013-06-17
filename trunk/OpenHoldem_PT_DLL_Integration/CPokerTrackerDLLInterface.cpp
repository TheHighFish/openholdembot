#include "stdafx.h"
#include "CPokerTrackerDLLInterface.h"

#include "..\PokerTracker_Query_Definitions\pokertracker_query_definitions.h"
#include "CPokerTrackerThread.h"
#include "DialogFormulaScintilla.h"

class CPokerTrackerDLLInterface *p_pokertracker_dll_interface = NULL;

CPokerTrackerDLLInterface::CPokerTrackerDLLInterface()
{}

CPokerTrackerDLLInterface::~CPokerTrackerDLLInterface()
{}

void CPokerTrackerDLLInterface::ExtendListOfSymbolsForEditor(CString *keywords)
{
	/*	Extends a list of known symbol like 
		"pt_icon pt_hands pt_pfr pt_aggp pt_aggf "
		Trailing space is necessary.
	*/
	for (int i=0; i<PT_DLL_GetNumberOfStats(); ++i)
	{
		CString pt_symbol_name = PT_DLL_GetBasicSymbolNameWithoutPTPrefix(i);
		assert(pt_symbol_name != "");
		CString new_keyword = "pt_" + pt_symbol_name + " ";
		*keywords += new_keyword;
		new_keyword = "pt_r_" + pt_symbol_name + " ";
		*keywords += new_keyword;
			
	}
}

void CPokerTrackerDLLInterface::ExtendListOfSymbolDescriptionsForEditor(
	HTREEITEM hCatItem)
{/*!!!
	HTREEITEM mainParent, parent;
	mainParent = parent = CDlgFormulaScintilla::AddSymbolTitle(
		"Poker Tracker symbols", NULL, hCatItem);
	parent = CDlgFormulaScintilla::AddSymbolSubTitle(
		mainParent, "Ring symbols");
	
	for (int i=0; i<PT_DLL_GetNumberOfStats(); ++i)
	{
		CString new_symbol  = "pt_
		CString description = PT_DLL_GetDescription(i);
		CDlgFormulaScintilla::AddSymbol(parent, new_symbol, description);
	}*/
}

double CPokerTrackerDLLInterface::GetStat(CString symbol_without_prefix, int chair)
{
	assert(chair >= k_first_chair); 
	assert(chair <= k_last_chair);
	return PT_DLL_GetStat(symbol_without_prefix, chair);	
}

void CPokerTrackerDLLInterface::SetStat(int stat, int chair, double value)
{
	assert(chair >= k_first_chair); 
	assert(chair <= k_last_chair);
	PT_DLL_SetStat(stat, chair, value);
}
