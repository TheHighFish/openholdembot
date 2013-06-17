#ifndef INC_CPOKERTRACKERDLLINTERFACE_H
#define INC_CPOKERTRACKERDLLINTERFACE_H

//!!!const int k_max_number_of_supported_pokertracker_stats = 64;

extern class CPokerTrackerDLLInterface
{
public:
	CPokerTrackerDLLInterface();
	~CPokerTrackerDLLInterface();
public:
	void	ExtendListOfSymbolsForEditor(CString *keywords);
	void	ExtendListOfSymbolDescriptionsForEditor(HTREEITEM hCatItem);
	double	GetStat(CString symbol_without_prefix, int chair);
	void	SetStat(int stat, int chair, double value);
} *p_pokertracker_dll_interface;

#endif INC_CPOKERTRACKERDLLINTERFACE_H