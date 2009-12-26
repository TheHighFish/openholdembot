#ifndef INC_CAUTOPLAYER_H
#define INC_CAUTOPLAYER_H

#include "../CCritSec/CCritSec.h"
#include "../CTablemap/CTablemap.h"

extern class CAutoplayer 
{
public:
	// public functions
	CAutoplayer(BOOL bInitiallyOwn, LPCTSTR lpszName);
	~CAutoplayer(void);
	void DoAutoplayer(void);
	void ResetHand(void);
	void ResetRound(void);

public:
	// public accessors
	const int prevaction() { return _prevaction; }
	const int didchec(const int n) { if (n>=0 && n<=4) return _didchec[n]; else return 0; }
	const int didcall(const int n) { if (n>=0 && n<=4) return _didcall[n]; else return 0; }
	const int didrais(const int n) { if (n>=0 && n<=4) return _didrais[n]; else return 0; }
	const int didswag(const int n) { if (n>=0 && n<=4) return _didswag[n]; else return 0; }
	const bool autoplayer_engaged() { return _autoplayer_engaged; }

public:
#define ENT CSLock lock(m_critsec);
	// public mutators
	void set_prevaction(const int i) { ENT _prevaction = i; }
	void set_didchec(const int n, const int i) { ENT if (n>=0 && n<=4) _didchec[n] = i; }
	void set_didcall(const int n, const int i) { ENT if (n>=0 && n<=4) _didcall[n] = i; }
	void set_didrais(const int n, const int i) { ENT if (n>=0 && n<=4) _didrais[n] = i; }
	void set_didswag(const int n, const int i) { ENT if (n>=0 && n<=4) _didswag[n] = i; }
	void set_autoplayer_engaged(const bool b) { ENT _autoplayer_engaged = b; }
#undef ENT

private:
	// private variables - use public accessors and public mutators to address these
	int		_prevaction;
	int		_didchec[5];				// "didchec" is held in element 4, round specific in elements 0-3
	int		_didcall[5];				// "didcall" is held in element 4, round specific in elements 0-3
	int		_didrais[5];				// "didrais" is held in element 4, round specific in elements 0-3
	int		_didswag[5];				// "didswag" is held in element 4, round specific in elements 0-3
	bool	_autoplayer_engaged;

private:
	// private functions and variables - not available via accessors or mutators
	const int CountSameScrape(void);
	void DoAllin(void);
	void DoSwag(void);
	void DoARCCF(void);
	void DoSlider(void);
	void DoPrefold(void);
	int GetR$ButtonIndices(void);
	void CheckBringKeyboard(void);
	void DoF$play(void);
	void DoI86(void);
	void DoChat(void);
	void DoRebuy(void);

	RMapCI	_alli_but, _rais_but, _call_but, _chec_but, _fold_but;
	RMapCI	_autopost_but, _sitin_but, _sitout_but, _leave_but, _pre_fold_but;
	bool	_autopost_state, _sitin_state, _sitout_state;
	RMapCI	_i86_but, _i86X_but[10];
	bool	_i86_state, _i86X_state[10];

	// Mutex used for cross-instance autoplayer coordination
	CMutex		_mutex;

	CCritSec	m_critsec;

} *p_autoplayer;


#endif //INC_CAUTOPLAYER_H