#ifndef INC_CAUTOPLAYER_H
#define INC_CAUTOPLAYER_H

#include "../CCritSec/CCritSec.h"
#include "MainFrm.h"
#include "../CTablemap/CTablemap.h"
#include "../CTablemap/CTableMapAccess.h"
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
#define ENT CSLock lock(m_critsec);
	void set_autoplayer_engaged(const bool to_be_enabled_or_not);
#undef ENT

private:
	// private variables - use public accessors and public mutators to address these
	int		_prevaction;
	bool	_autoplayer_engaged;

private:
	// private functions and variables - not available via accessors or mutators
	void GetNeccessaryTablemapObjects();
	int NumberOfVisibleButtons();
	void DoAllin(void);
	void DoBetPot(void);
	void DoSwag(void);
	void DoARCCF(void);
	void DoSlider(void);
	void DoPrefold(void);
	void CheckBringKeyboard(void);
	bool TimeToHandleSitinSitoutLeave();
	void CAutoplayer::DoF$Sitin_Sitout_Leave(void);
	void DoI86(void);
	void DoChat(void);

	RMapCI	_alli_but, _rais_but, _call_but, _chec_but, _fold_but;
	RMapCI	_autopost_but, _sitin_but, _sitout_but, _leave_but, _pre_fold_but;
	bool	_autopost_state, _sitin_state, _sitout_state;
	bool	_i86_state, _i86X_state[10];

	// Mutex used for cross-instance autoplayer coordination
	CMutex		_mutex;

	CCritSec	m_critsec;

// tablemap regions
private:
	RECT allin_button;
	RECT raise_button;
	RECT call_button;
	RECT check_button;
	RECT fold_button;
	RECT prefold_button;
	RECT sitin_button;
	RECT sitout_button;
	RECT leave_button;
	RECT autopost_button;
	RECT i86_button;
	RECT i86X_buttons[k_max_number_of_i86X_buttons];
	RECT i3_button;     // Old WH-Standard; i3 is for swag
	RECT i3_edit_region;
	RECT i3_slider_region;
	RECT i3_handle_region;
// regions defined
private:
	bool allin_button_defined;
	bool raise_button_defined;
	bool call_button_defined;
	bool check_button_defined;
	bool fold_button_defined;
	bool prefold_button_defined;
	bool sitin_button_defined;
	bool sitout_button_defined;
	bool leave_button_defined;
	bool autopost_button_defined;
	bool i86_button_defined;
	bool i86X_buttons_defined[k_max_number_of_i86X_buttons];
	bool i3_button_defined;
	bool i3_edit_defined;
	bool i3_slider_defined;
	bool i3_handle_defined;
// regions available
private:
	bool allin_option_available;
	bool allin_button_available;
	bool raise_button_available;
	bool call_button_available;
	bool check_button_available;
	bool fold_button_available;
	bool prefold_button_available;
	bool sitin_button_available;
	bool sitout_button_available;
	bool leave_button_available;
	bool autopost_button_available;
	bool i86_button_available;
	bool i86X_buttons_available[k_max_number_of_i86X_buttons];
	bool i3_button_available;

} *p_autoplayer;


#endif //INC_CAUTOPLAYER_H