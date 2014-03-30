#ifndef INC_CAUTOPLAYER_H
#define INC_CAUTOPLAYER_H

extern class CAutoplayer 
{
public:
	// Critical section used in public mutators and private shared variable writes
	static CRITICAL_SECTION		cs_autoplayer;

public:
	// public functions
	CAutoplayer(BOOL bInitiallyOwn, LPCTSTR lpszName);
	~CAutoplayer(void);
	void DoAutoplayer(void);

public:
	// public accessors
	const int prevaction() { return _prevaction; }
	const int didchec(const int n) { if (n>=0 && n<=4) return _didchec[n]; else return 0; }
	const int didcall(const int n) { if (n>=0 && n<=4) return _didcall[n]; else return 0; }
	const int didrais(const int n) { if (n>=0 && n<=4) return _didrais[n]; else return 0; }
	const int didswag(const int n) { if (n>=0 && n<=4) return _didswag[n]; else return 0; }

public:
#define ENT EnterCriticalSection(&cs_autoplayer);
#define LEA LeaveCriticalSection(&cs_autoplayer);
	// public mutators
	// **These are only needed by the PokerPro code to push it's updates into the CAutoplayer structures**
	void set_prevaction(const int i) { ENT _prevaction = i; LEA };
	void set_didchec(const int n, const int i) { ENT if (n>=0 && n<=4) _didchec[n] = i; LEA };
	void set_didcall(const int n, const int i) { ENT if (n>=0 && n<=4) _didcall[n] = i; LEA };
	void set_didrais(const int n, const int i) { ENT if (n>=0 && n<=4) _didrais[n] = i; LEA };
	void set_didswag(const int n, const int i) { ENT if (n>=0 && n<=4) _didswag[n] = i; LEA };
	// **End PokerPro-specific mutators

#undef ENT
#undef LEA

private:
	// private variables - use public accessors and public mutators to address these
	int _prevaction;
	int _didchec[5];				// "didchec" is held in element 4, round specific in elements 0-3
	int _didcall[5];				// "didcall" is held in element 4, round specific in elements 0-3
	int _didrais[5];				// "didrais" is held in element 4, round specific in elements 0-3
	int _didswag[5];				// "didswag" is held in element 4, round specific in elements 0-3


private:
	// private functions and variables - not available via accessors or mutators
	int		_alli_but, _rais_but, _call_but, _chec_but, _fold_but;
	int		_autopost_but, _sitin_but, _sitout_but, _leave_but, _pre_fold_but;
	bool	_autopost_state, _sitin_state, _sitout_state;
	int		_i86_but, _i86X_but[10];
	bool	_i86_state, _i86X_state[10];

	// Mutex used for cross-instance autoplayer coordination
	CMutex		_mutex;

	const int CountSameScrape(void);
	void DoSwag(void);
	void DoARCCF(void);
	void DoSlider(void);
	void DoPrefold(void);
	int GetR$ButtonIndices(void);
	void CheckBringKeyboard(void);
	void DoF$play(void);
	void DoI86(void);
	void DoChat(void);
	const POINT RandomizeClickLocation(const int left, const int top, const int right, const int bottom);
	const void GetClickPoint(const int x, const int y, const int rx, const int ry, POINT *p);
	const double RandomNormalScaled(const double scale, const double m, const double s);
	const double RandomNormal(const double m, const double s);
} *p_autoplayer;


#endif //INC_CAUTOPLAYER_H