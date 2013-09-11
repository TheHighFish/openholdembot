#ifndef INC_CPOKERACTION_H
#define INC_CPOKERACTION_H

class CPokerAction 
{
public:
	// public functions
	CPokerAction();
	~CPokerAction();
	const double	ProcessQuery(const char * pquery, int *e);
	const int		DealPosition(const int chairnum);
	
private:
	// private functions and variables - not available via accessors or mutators
	const int		PreflopPos(void);
	const int		PreflopRaisPos(void);
	const int		PostflopPos(void);
	const bool		FirstIntoPot(void);	
	const int		BetPosition(const int chairnum);
	const bool		AgchairAfter(void);
};


#endif //INC_CPOKERACTION_H