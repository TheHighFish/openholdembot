#ifndef INC_GLOBAL_H
#define INC_GLOBAL_H

#include "structs_defines.h"
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
	CGlobal(void);
	~CGlobal(void);

	void ClearFormula(SFormula *f);
	void CopyFormula(SFormula *f, SFormula *t);
	bool ParseAllFormula(HWND hwnd, SFormula *f);
	static void ParseHandList(CString &list_text, bool inlist[13][13]);
	void create_hand_list_matrices(SFormula *f);
	void capture_state(const char *title);
	void clear_r$indexes(void);
	void save_r$indexes(void);
	void save_s$indexes(void);
	void save_s$strings(void);
	void create_replay_frame(void);
	CString get_card_html(unsigned int card);

	// Table map and transformation engine
    CTransform		trans;

	// Formula being used for execution
	SFormula		formula;
	CString			formula_name;

	// Table that we are attached to
	HWND			attached_hwnd;
	RECT			attached_rect;

	// hwnd for main frame
	HWND			hMainFrame;

	// Flags button status
	bool			flags[10];

	// Preferences
	SPreferences	preferences;

	// Autoplayer engaged status
	bool			autoplay;
	bool			auto_autoplay_pressed;

	// Update in process status
	bool			update_in_process;

	// Versus
	bool			versus_enabled;
	CString			versus_path;

	// Poker Pro
	bool			ppro_is_connected;
	int				ppro_isppro;
	int				ppro_tid;

	// Game state array
	holdem_state	state[256];
	unsigned char	state_index;

	// Used if we need to display a wait cursor anywhere
	bool			m_WaitCursor;

	// Next replay frame number
	int				next_replay_frame;
	bool			replay_recorded_this_turn;

	// instance/session number
	int				sessionnum;

	// Used for holding table list by callback function
	CArray <STableList, STableList>		g_tlist;

	// Startup path
	char			startup_path[MAX_PATH];

	CString			mm_network;

} global;

bool parse_loop(const CUPDUPDATA* pCUPDUPData);

#endif /* INC_GLOBAL_H */