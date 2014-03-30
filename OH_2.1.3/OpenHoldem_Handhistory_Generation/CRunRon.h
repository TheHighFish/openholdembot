#ifndef INC_CRUNRON_H
#define INC_CRUNRON_H

#include "..\CCritSec\CCritSec.h"

struct SCounts
{
	double royfl, strfl, fourkind, fullh, flush, strai, threekind, twopair;
	double onepair, hcard, total, pokervalmax, prnuts, prbest, clocks, pokervalmaxcount;
};

class CRunRon 
{
public:
	// public functions
	CRunRon(void);
	~CRunRon(void);
	void GetCounts(void);

public:
	// public accessors
	const SCounts * sron() { return &_sron; }
	const SCounts * srun() { return &_srun; }

private:
	// private functions and variables - not available via accessors or mutators
	SCounts	_sron;
	SCounts	_srun;

private:
	double _run_count[13][13][10], _run_max_pokval[13][13], _run_max_pokval_n[13][13];
	double _ron_count[13][13][10], _ron_max_pokval[13][13], _ron_max_pokval_n[13][13];

	CCritSec		m_critsec;
};


#endif //INC_CRUNRON_H