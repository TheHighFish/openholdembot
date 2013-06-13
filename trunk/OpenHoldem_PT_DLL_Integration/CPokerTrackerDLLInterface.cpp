#include "stdafx.h"
#include "CPokerTrackerDLLInterface.h"

#include "CPokerTrackerThread.h"

class CPokerTrackerDLLInterface *p_pokertracker_dll_interface = NULL;

CPokerTrackerDLLInterface::CPokerTrackerDLLInterface()
{}

CPokerTrackerDLLInterface::~CPokerTrackerDLLInterface()
{}

void CPokerTrackerDLLInterface::ExtendListOfSymbolsForEditor(CString *keywords)
{
	//!!!!!
	*keywords += "pt_icon pt_hands pt_pfr pt_aggp pt_aggf pt_aggt pt_aggr "
				  "pt_aggtot pt_aggtotnopf pt_floppct pt_turnpct pt_riverpct pt_vpip pt_pf_rfi "
				  "pt_pf_cr pt_pfats pt_wsdp pt_wssd pt_fbbts pt_fsbts ";
	*keywords += "pt_ricon pt_rhands pt_rpfr pt_raggp pt_raggf pt_raggt pt_raggr "
				  "pt_raggtot pt_raggtotnopf pt_rfloppct pt_rturnpct pt_rriverpct pt_rvpip pt_rpf_rfi "
				  "pt_rpf_cr pt_rpfats pt_rwsdp pt_rwssd pt_rfbbts pt_rfsbts ";
}

void CPokerTrackerDLLInterface::ExtendListOfSymbolDescriptionsForEditor()
{
	//!!!!!	
}


double CPokerTrackerDLLInterface::GetStat(int chr, int stat)
{
	assert(chr >= k_first_chair); 
	assert(chr <= k_last_chair);

	return 0;
	//!!!return _player_stats[chr].stat[stat];	
}

bool CPokerTrackerDLLInterface::IsFound(int chair)
{
	return true; //!!!_player_stats[chair].found;
}


void CPokerTrackerDLLInterface::SetStat(int chair, int stat, double value)
{
//!!!
}
