#ifndef INC_ACTION_H
#define INC_ACTION_H

class PokerAction 
{
public:
	PokerAction::PokerAction();
	double	process_query(const char * pquery, int *e);

private:
	int		preflop_pos(void);
	int		prefloprais_pos(void);
	int		postflop_pos(void);
	int		pf_bets(void);
	bool	first_into_pot(void);	
	int		m_betposition(int chairnum);
	int		m_dealposition(int chairnum);
	int		aggressor_chair(void);
	bool	agchair_after(void);
};


#endif //INC_ACTION_H