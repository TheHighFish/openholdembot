#ifndef INC_CPOKERTRACKERDLLINTERFACE_H
#define INC_CPOKERTRACKERDLLINTERFACE_H

//!!!const int k_max_number_of_supported_pokertracker_stats = 64;

extern class CPokerTrackerDLLInterface
{
public:
	CPokerTrackerDLLInterface();
	~CPokerTrackerDLLInterface();
public:
	void ExtendListOfSymbolsForEditor(CString *keywords);
	void ExtendListOfSymbolDescriptionsForEditor();
	double GetStat(int chair, int stat);
	void SetStat(int chair, int stat, double value);
	bool IsFound(int chair);
} *p_pokertracker_dll_interface;

#endif INC_CPOKERTRACKERDLLINTERFACE_H