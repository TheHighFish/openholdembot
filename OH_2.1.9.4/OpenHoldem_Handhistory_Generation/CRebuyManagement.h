#ifndef __LIB_CREBUYMANAGEMENT_H

#define __LIB_CREBUYMANAGEMENT_H


#include <time.h>


extern class CRebuyManagement
{
public:
	CRebuyManagement();
	~CRebuyManagement();	
	
public:
	// IMPORTANT!
	//
	// This function has to be protected against mouse-
	// and keyboard collisions, e.g by a mutex.
	// We expect the autoplayer to do this job!
	void TryToRebuy();

private:
	bool MinimumDelayElapsed();
	bool ChangeInHandNumber();
	bool NoCards();
	bool OcclusionCheck();
	bool RebuyPossible();
	// CAUTION! DO NOT USE THIS FUNCTION DIRECTLY!
	// It has to be protected by a mutex.
	// We assume, the autoplayer does that.
	void ExecuteRebuyScript();

private:
	static const unsigned int UnknownCard = 0;
	static const unsigned int RebuyMinimumTimeDifference = 30;

private:
	time_t	RebuyLastTime, CurrentTime;
	int		PreviousRebuyHandNumber;
} *p_rebuymanagement;

#endif	