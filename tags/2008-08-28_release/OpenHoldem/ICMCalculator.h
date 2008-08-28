#ifndef INC_ICMCALCULATOR_H
#define INC_ICMCALCULATOR_H

#define MAX_PLAYERS		10
#define MAX_DOUBLE		1.e+200

class ICM
{
public:	
	ICM();
	~ICM();
	double ProcessQueryICM(const char* pquery, int *e);

	double	icm_prob[MAX_PLAYERS];

private:
	double EquityICM(double *stacks, double *prizes, int playerNB, int player);
	double GetPlayerCurrentBet(int pos);
	int getChairFromDealPos(const char* pquery);

};


#endif //INC_ICMCALCULATOR_H