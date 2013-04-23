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