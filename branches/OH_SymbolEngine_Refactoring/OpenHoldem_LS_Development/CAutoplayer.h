#ifndef INC_CAUTOPLAYER_H
#define INC_CAUTOPLAYER_H

#include "../CCritSec/CCritSec.h"
#include "../CTablemap/CTablemap.h"
#include "CTableMapAccess.h"

extern class CAutoplayer 
{
public:
	// public functions
	CAutoplayer(BOOL bInitiallyOwn, LPCTSTR lpszName);
	~CAutoplayer(void);
	void DoAutoplayer(void);
	void ResetHand(void);
	void ResetRound(void);
	void DoRebuyIfNeccessary(void);
	bool TimeToHandleSitinSitoutLeave();

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
	void GetNeccessaryTablemapObjects();
	int NumberOfVisibleButtons();
	void DoAllin(void);
	void DoSwag(void);
	void DoARCCF(void);
	void DoSlider(void);
	void DoPrefold(void);
	void CheckBringKeyboard(void);
	void CAutoplayer::DoF$Sitin_Sitout_Leave(void);
	void DoI86(void);
	void DoChat(void);
	bool	_autopost_state, _sitin_state, _sitout_state;
	bool	_i86_state, _i86X_state[10];

	// Mutex used for cross-instance autoplayer coordination
	CMutex		_mutex;

	CCritSec	m_critsec;
private:
	bool allin_button_defined;
	bool raise_button_defined;
	bool call_button_defined;
	bool check_button_defined;
	bool fold_button_defined;
	bool prefold_button_defined;
	bool i3_button_defined;
	bool sitin_button_defined;
	bool sitout_button_defined;
	bool leave_button_defined;
	bool autopost_button_defined;
	bool i86_button_defined;
	bool i86X_buttons_defined[k_number_of_i86X_buttons];
private:
	RECT allin_button;
	RECT raise_button;
	RECT call_button;
	RECT check_button;
	RECT fold_button;
	RECT prefold_button;
	RECT i3_button;     // Old WH-Standard; i3 is for swag
	RECT sitin_button;
	RECT sitout_button;
	RECT leave_button;
	RECT autopost_button;
	RECT i86_button;
	RECT i86X_buttons[k_number_of_i86X_buttons];
private:
	bool i3_edit_defined;
	bool i3_slider_defined;
	bool i3_handle_defined;
private:
	RECT i3_edit_region;
	RECT i3_slider_region;
	RECT i3_handle_region;
} *p_autoplayer;


#endif //INC_CAUTOPLAYER_H