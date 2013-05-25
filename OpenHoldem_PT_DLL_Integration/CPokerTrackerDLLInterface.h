#ifndef INC_CPOKERTRACKERDLLINTERFACE_H
#define INC_CPOKERTRACKERDLLINTERFACE_H

extern class CPokerTrackerDLLInterface
{
public:
	CPokerTrackerDLLInterface();
	~CPokerTrackerDLLInterface();
public:
	void ExtendListOfSymbolsForEditor(CString *keywords);
	void ExtendListOfSymbolDescriptionsForEditor();
} *p_pokertracker_dll_interface;

#endif INC_CPOKERTRACKERDLLINTERFACE_H