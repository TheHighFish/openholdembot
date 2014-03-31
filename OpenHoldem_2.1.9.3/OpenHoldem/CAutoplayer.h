#ifndef INC_CAUTOPLAYER_H
#define INC_CAUTOPLAYER_H

#include "../CCritSec/CCritSec.h"
#include "MainFrm.h"
#include "../CTablemap/CTablemap.h"
#include "CTableMapAccess.h"
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
	void set_autoplayer_engaged(const bool to_be_enabled_or_not) { ENT _autoplayer_engaged = to_be_enabled_or_not; }
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
	void DoSwag(void);
	void DoARCCF(void);
	void DoSlider(void);
	void DoPrefold(void);
	void CheckBringKeyboard(void);
	bool TimeToHandleSitinSitoutLeave();
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