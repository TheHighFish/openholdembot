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

#ifndef __LIB_COCCLUSIONCHECK_H

#define __LIB_COCCLUSIONCHECK_H

// Heuristic occlusion-check for rebuy.

class COcclusionCheck
{
public:
	COcclusionCheck();
	~COcclusionCheck();
public:
	bool UserBalanceOccluded();
private:
	bool UserChairKnown();
	bool UserBalanceNonZero();
	bool UserNameKnown();
	bool AnyOpponentNameKnown();
	bool AnyApponentBalanceNonZero();
};

extern COcclusionCheck *p_occlusioncheck;

#endif // __LIB_COCCLUSIONCHECK_H