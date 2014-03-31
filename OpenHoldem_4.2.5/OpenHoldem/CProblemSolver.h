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

#ifndef INC_CPROBLEMSOLVER_H
#define INC_CPROBLEMSOLVER_H

class CProblemSolver
{
public:
	CProblemSolver();
	~CProblemSolver();
public:
	void TryToDetectBeginnersProblems();
private:
	bool NoTableMapsInScraperFolder();
	bool NotConnected();
	bool UserChairUnknown();
	bool NoOpponents();
	bool AutoPlayerDidActAtLeastOnce();
	bool NoCardsVisible();
	bool NotEnoughButtonsVisible();
};

#endif