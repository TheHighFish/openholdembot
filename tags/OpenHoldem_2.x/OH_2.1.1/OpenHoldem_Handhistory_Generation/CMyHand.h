#ifndef INC_CMYHAND_H
#define INC_CMYHAND_H

class CMyHand 
{
public:
	// public functions
	CMyHand();
	~CMyHand();
	double	ProcessQuery(const char * pquery, int *e);

private:
	// private functions and variables - not available via accessors or mutators
	void MyHand(char * hand);
	const bool ThreeStraight(const bool wheel, const bool broadway);
	const bool BottomStraightDraw(void);
	const int NSuitedBetter(void);
	const int KickerBetter(void);
	const int KickerRank(void);
	const double NOuts(void);
	const bool SafeBoard(void);
	const int StrengthStraightFlush(void);
	const int StrengthQuads(void);
	const int StrengthFullHouse(void);
	const int StrengthFlush(void);
	const int StrengthStraight(void);
	const int StrengthTrips(void);
	const int StrengthTwoPair(void);
	const int StrengthOnePair(void);
};

#endif //INC_CMYHAND_H