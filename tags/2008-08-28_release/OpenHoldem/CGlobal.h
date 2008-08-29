#ifndef INC_CGLOBAL_H
#define INC_CGLOBAL_H
	
#include "CGlobal.h"

#include <boost/spirit/tree/ast.hpp>
#include "dll_extension.h"
#include "UPDialog.h"

// For parsing progress dialog
struct	sData 
{
	bool		all_parsed;
	HWND		calling_hwnd;
	SFormula	*f;
};

extern class CGlobal 
{
public:
	// Critical section used in public mutators and private shared variable writes
	static CRITICAL_SECTION	cs_global;

public:
	// public functions
	CGlobal(void);
	~CGlobal(void);
	void ClearFormula(SFormula *f);
	void CreateHandListMatrices(SFormula *f);
	bool ParseAllFormula(HWND hwnd, SFormula *f);
	void CreateReplayFrame(void);
	void ClearR$Indices(void);
	void SaveR$Indices(void);
	void SaveS$Indices(void);
	void SaveS$Strings(void);
	void CaptureState(const char *title);
	void ParseHandList(CString &list_text, bool inlist[13][13]);
	void CopyFormula(SFormula *f, SFormula *t);

public:
	// public variables <<< need to make these private eventually
	// Table map and transformation engine
	CTransform		trans;

	// Formula being used for execution
	SFormula		formula;
	CString			formula_name;

	// Preferences
	SPreferences	preferences;

	// Versus
	int				versus_fh;

	// Poker Pro
	bool			ppro_is_connected;
	int				ppro_isppro;
	int				ppro_tid;

	// Game state array
	holdem_state	state[256];
	unsigned char	state_index;

	// Used for holding table list by callback function
	CArray <STableList, STableList>		g_tlist;

	// Formula set currently being parsed, and result of that parse (this is for symbol validation)
	SFormula		*parse_symbol_formula;
	CArray<std::string, std::string>		parse_symbol_stop_strs;

	CString			mm_network;

	// autoplayer engaged status
	bool			autoplay;
	bool			autoplay_pressed;


public:
	// public accessors
	const bool				m_wait_cursor() { return _m_wait_cursor; }
	const bool				replay_recorded_this_turn() { return _replay_recorded_this_turn; }
	const unsigned long int session_id() { return _session_id; }
	const HWND				h_main_frame() { return _h_main_frame; }
	const HWND				attached_hwnd() { return _attached_hwnd; }
	//const RECT			attached_rect() { return _attached_rect; }
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
	RECT * set_attached_rect() { return &_attached_rect; }
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
	RECT				_attached_rect;  // Table that we are attached to
	int					_next_replay_frame;	// Next replay frame number
	bool				_flags[10];	// _flags button status


private:
	// private functions and variables - not available via accessors or mutators
	CString GetCardHtml(unsigned int card);

} *p_global;

bool parse_loop(const CUPDUPDATA* pCUPDUPData);

#endif /* INC_CGLOBAL_H */