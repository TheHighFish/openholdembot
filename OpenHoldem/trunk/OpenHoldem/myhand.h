#pragma once

class MyHand 
{
public:
	double	process_query(const char * pquery);
	void	myhand(char * hand);
	bool	three_straight(bool wheel, bool broadway);
	bool	bottomsd(void);
	int		nsuitedbetter(void);
	int		kickerbetter(void);
	int		kickerrank(void);
	double	m_nouts(void);

private:
	static const int	RoyalFlush =		0x800EDCBA;
	static const int 	StraightFlush =		0x80000000;
	static const int 	FourOfAKind =		0x40000000;
	static const int 	FullHouse =			0x20000000;
	static const int 	Flush =				0x10000000;
	static const int 	Straight =			0x08000000;
	static const int 	ThreeOfAKind =		0x04000000;
	static const int 	TwoPair	=			0x02000000;
	static const int 	OnePair	=			0x01000000;
};
