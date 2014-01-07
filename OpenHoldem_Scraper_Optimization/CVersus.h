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

#ifndef INC_CVERSUS_H
#define INC_CVERSUS_H

#include "..\CCritSec\CCritSec.h"

extern class CVersus 
{
public:
	// public functions
	CVersus();
	~CVersus();
	bool GetCounts();
	double GetSymbol(const char *a, int *e);
	bool VersusBinLoaded()	{ return !versus_bin_not_loaded; }

private:
	// private functions and variables - not available via accessors or mutators
	void CheckForLoadedVersusBin();
	void DoCalc(const CardMask plCards, const CardMask oppCards, const CardMask comCards, 
				unsigned int *wintemp, unsigned int *tietemp, unsigned int *lostemp);

	int				_nhands, _nwin, _ntie, _nlos, _nhandshi, _nhandsti, _nhandslo;
	double			_vsprwin, _vsprtie, _vsprlos;
	double			_vsprwinhi, _vsprtiehi, _vsprloshi;
	double			_vsprwinti, _vsprtieti, _vsprlosti;
	double			_vsprwinlo, _vsprtielo, _vsprloslo;
	int				_nhandshinow, _nhandstinow, _nhandslonow;
	double			_vsprwinhinow, _vsprtiehinow, _vsprloshinow;
	double			_vsprwintinow, _vsprtietinow, _vsprlostinow;
	double			_vsprwinlonow, _vsprtielonow, _vsprloslonow;
	int				_nlistwin[MAX_HAND_LISTS], _nlisttie[MAX_HAND_LISTS], _nlistlos[MAX_HAND_LISTS];
	int				_versus_fh;
	bool			versus_bin_not_loaded;

	CCritSec		m_critsec;

} *p_versus;

#endif INC_CVERSUS_H