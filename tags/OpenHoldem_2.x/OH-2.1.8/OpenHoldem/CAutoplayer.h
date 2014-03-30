#ifndef INC_CAUTOPLAYER_H
#define INC_CAUTOPLAYER_H

#include "../CCritSec/CCritSec.h"
#include "../CTablemap/CTablemap.h"
#include "MainFrm.h"
#include "OpenHoldem.h"

extern class CAutoplayer 
{
public:
	// public functions
	CAutoplayer(BOOL bInitiallyOwn, LPCTSTR lpszName);
	~CAutoplayer(void);
	void DoAutoplayer(void);
	void DoRebuyIfNeccessary(void);

public:
	// public accessors
	const bool autoplayer_engaged() { return _autoplayer_engaged; }

public:
	// public mutators
	void set_autoplayer_engaged(const bool to_be_enabled_or_not);

private:
	// private variables - use public accessors and public mutators to address these
	int		_prevaction;
	bool	_autoplayer_engaged;

private:
	// private functions and variables - not available via accessors or mutators
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