#ifndef INC_CICMCALCULATOR_H
#define INC_CICMCALCULATOR_H

#define MAX_PLAYERS		10
#define MAX_DOUBLE		1.e+200

class CICMCalculator
{
public:
	// public functions
	CICMCalculator();
	~CICMCalculator();
	const double ProcessQueryICM(const char* pquery, int *e);

private:
	// private functions and variables - not available via accessors or mutators
	double EquityICM(double *stacks, double *prizes, int playerNB, int player);
	double GetPlayerCurrentBet(int pos);
	int GetChairFromDealPos(const char* pquery);

	//double	_icm_prob[MAX_PLAYERS];
};


#endif //INC_CICMCALCULATOR_H