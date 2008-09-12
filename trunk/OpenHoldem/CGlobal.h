#ifndef INC_CGLOBAL_H
#define INC_CGLOBAL_H
	
#include "CGlobal.h"

#include "CFormula.h"

#include <boost/spirit/tree/ast.hpp>
#include "CDllExtension.h"
#include "CFormula.h"

extern class CGlobal 
{
public:
	// Critical section used in public mutators and private shared variable writes
	static CRITICAL_SECTION	cs_global;

public:
	// public functions
	CGlobal(void);
	~CGlobal(void);
	void CreateReplayFrame(void);

public:
	// public variables <<< need to make these private eventually

	// Used for holding table list by callback function
	CArray <STableList, STableList>		g_tlist;

	CString			mm_network;

public:
	// public accessors
	const bool				m_wait_cursor() { return _m_wait_cursor; }
	const bool				replay_recorded_this_turn() { return _replay_recorded_this_turn; }
	const unsigned long int session_id() { return _session_id; }
	const HWND				h_main_frame() { return _h_main_frame; }
	const HWND				attached_hwnd() { return _attached_hwnd; }
	const int				next_replay_frame() { return _next_replay_frame; }
	const bool				flags(const int n) { if (n>=0 && n<=9) return _flags[n]; else return false; }

public:
#define ENT EnterCriticalSection(&cs_global);
#define LEA LeaveCriticalSection(&cs_global);
	// public mutators 
	void set_m_wait_cursor(const bool b) { ENT _m_wait_cursor = b; LEA }
	void set_replay_recorded_this_turn(const bool b) { ENT _replay_recorded_this_turn = b; LEA }
	void set_session_id(const unsigned long int li) { ENT _session_id = li; LEA }
	void set_h_main_frame(const HWND h) { ENT _h_main_frame = h; LEA }
	void set_attached_hwnd(const HWND h) { ENT _attached_hwnd = h; LEA }
	void set_next_replay_frame(const int i) { ENT _next_replay_frame = i; LEA }
	void set_flags(const int n, const bool b) { ENT if (n>=0 && n<=9) _flags[n] = b; LEA }

#undef ENT
#undef LEA

private:
	// private variables - use public accessors and public mutators to address these
	bool				_m_wait_cursor;  // Used if we need to display a wait cursor anywhere
	bool				_replay_recorded_this_turn;
	unsigned long int	_session_id;
	HWND				_h_main_frame;	// hwnd for main frame
	HWND				_attached_hwnd;	 // Table that we are attached to
	int					_next_replay_frame;	// Next replay frame number
	bool				_flags[10];	// _flags button status


private:
	// private functions and variables - not available via accessors or mutators
	CString GetCardHtml(unsigned int card);

} *p_global;

#endif /* INC_CGLOBAL_H */