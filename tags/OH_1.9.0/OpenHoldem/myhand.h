#pragma once

class MyHand 
{
public:
	double	process_query(const char * pquery, int *e);

private:
	void	myhand(char * hand);
	bool	three_straight(bool wheel, bool broadway);
	bool	bottomsd(void);
	int		nsuitedbetter(void);
	int		kickerbetter(void);
	int		kickerrank(void);
	double	m_nouts(void);
	bool	safeboard(void);
	int		strength_straightflush(void);
	int		strength_quads(void);
	int		strength_fullhouse(void);
	int		strength_flush(void);
	int		strength_straight(void);
	int		strength_trips(void);
	int		strength_twopair(void);
	int		strength_onepair(void);
};
