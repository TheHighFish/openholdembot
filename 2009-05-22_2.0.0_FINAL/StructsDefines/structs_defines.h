#ifndef INC_STRUCTS_DEFINES_H
#define INC_STRUCTS_DEFINES_H

#include <boost/spirit/tree/ast.hpp>

typedef boost::spirit::node_val_data_factory<const char *> int_factory_t;

// thread timeout
#define THREAD_WAIT					3000

// numeric constants
#define M_E			2.7182818284590452354
#define M_LOG2E		1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2		0.69314718055994530942
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_SQRT2		1.41421356237309504880

// Error Codes
#define SUCCESS						0
#define ERR_EOF						1
#define ERR_SYNTAX					2
#define ERR_VERSION					3
#define ERR_NOTMASTER				4
#define ERR_HASH_COLL				5
#define ERR_REGION_SIZE				6
#define ERR_UNK_LN_TYPE				7
#define ERR_INV_HASH_TYPE			8
#define ERR_DEBUG_NOEQ				9
#define ERR_INVALID_SYM				10
#define ERR_INVALID_FUNC_SYM		11
#define ERR_INVALID_DLL_SYM			12
#define ERR_INVALID_EXPR			13
#define ERR_DIV_ZERO				15
#define ERR_INVALID_F$$_REF			16
#define ERR_BAD_PARSE				17
#define ERR_INCOMPLETEMASTER		18

// Scraper error codes
#define ERR_FIELD_TOO_LARGE			-1
#define ERR_NOTHING_TO_SCRAPE		-2
#define ERR_NO_HASH_MATCH			-3
#define ERR_TEXT_SCRAPE_NOMATCH		-4
#define ERR_INVALID_TRANSFORM_TYPE	-5
#define ERR_NO_IMAGE_MATCH			-6
#define	ERR_GOOD_SCRAPE_GENERAL		1

// Profile version codes
#define VER_WINSCRAPE			".wsdb1"
#define VER_OPENHOLDEM_1		".ohdb1"  // no longer valid, due to changing of hash keys for types 1-3
#define VER_OPENHOLDEM_2		".ohdb2"  // legacy version, still supported, although openscrape table maps are preferred
#define VER_OPENSCRAPE_1		".osdb1"
#define VER_OPENSCRAPE_2		".osdb2"
#define VER_WINSCRAPE_DATE1		"2006 Aug 7 13:11:37"
#define VER_WINSCRAPE_DATE2		"2007 Nov 1 08:32:55"
#define VER_WINSCRAPE_DATE3		"2007 Nov 3 09:54:04"
#define VER_WINSCRAPE_DATE4		"2007 Nov 6 11:52:24"

// Cards
#define CARD_BACK		0xff
#define CARD_NOCARD		0xfe
#define WH_CARDBACK		0xff
#define WH_NOCARD		0

// Limits
#define LIMIT_NL		0
#define LIMIT_PL		1
#define LIMIT_FL		2

// Hand Lists
#define MAX_HAND_LISTS	1000		// e.g. islist0 - islist999 Matrix 2008-05-14

// Preferences
#define MAX_OPPONENTS		22	
#define MAX_FRAMEDELAY		9	
#define MAX_SWAGDELAY1		5000	
#define MAX_SWAGDELAY2		5000	
#define MAX_SWAGDELAY3		5000	
#define MAX_ALLINDELAY		5000	
#define MAX_SCRAPEDELAY		5000
#define MIN_SCRAPEDELAY		100

// WH SUIT Defs
#define WH_SUIT_CLUBS		1
#define WH_SUIT_DIAMONDS	2
#define WH_SUIT_HEARTS		3
#define WH_SUIT_SPADES		4

// Chipstacks
#define MAX_CHIPS_PER_STACK	20
#define MAX_CHIP_STACKS		10


// SWAG options
#define TEXTSEL_SINGLECLICK	1
#define TEXTSEL_DOUBLECLICK	2
#define TEXTSEL_CLICKDRAG	3
#define TEXTSEL_NOTHING		4
#define TEXTDEL_DELETE		1
#define TEXTDEL_BACKSPACE	2
#define TEXTDEL_NOTHING		3
#define BETCONF_ENTER		1
#define BETCONF_CLICKBET	2
#define BETCONF_NOTHING		3

// Button click options
#define BUTTON_SINGLECLICK	1
#define BUTTON_DOUBLECLICK	2

// Hand reset detection options
#define HANDRESET_DEALER	0x0001
#define HANDRESET_HANDNUM	0x0010
#define HANDRESET_CARDS		0x0100

// Autoplayer prevaction
#define PREVACT_FOLD		-1
#define PREVACT_CHEC		0
#define PREVACT_CALL		1
#define PREVACT_RAIS		2
#define PREVACT_SWAG		3
#define PREVACT_ALLI		4

// Game state engine
#define w_noaction			0			//chair has not acted
#define	w_folded			1			//fold action
#define	w_checked			2			//chec action
#define	w_called			3			//call action
#define	w_raised			4			//rais action
#define	w_posted_sb			5			//sb post action
#define	w_posted_bb			6			//bb post action
//#define w_called_bb			7			//the sb chair called the bb in an unraised pot
#define	w_reraised			8			//re-rais action
#define	w_num_action_types	9			//number of action types

// For drawing
// Card sizes
#define CARDSIZEX		26
#define	CARDSIZEY		52
// Colors
// (more color codes here: http://htmlhelp.com/cgi-bin/color.cgi)
#define	COLOR_RED		RGB(255,0,0)
#define	COLOR_BLACK		RGB(0,0,0)
#define	COLOR_BLUE		RGB(0,0,255)
#define	COLOR_GREEN		RGB(0,255,0)
#define	COLOR_WHITE		RGB(255,255,255)
#define COLOR_LT_GRAY	RGB(212,208,200)
#define COLOR_GRAY		RGB(182,182,182)
#define COLOR_YELLOW	RGB(255,255,0)

// Bitmap display
#define BITMAP_WIDTH		344
#define BITMAP_HEIGHT		164

// For scraper engine
#define NOTHING_CHANGED		0
#define	COM_CARD_CHANGED	(1<<1)
#define PL_CARD_CHANGED		(1<<2)
#define	SEATED_CHANGED		(1<<3)
#define	ACTIVE_CHANGED		(1<<4)
#define	DEALER_CHANGED		(1<<5)
#define	NAME_CHANGED		(1<<6)
#define	BALANCE_CHANGED		(1<<7)
#define	PLAYERBET_CHANGED	(1<<8)
#define	BUTTONSTATE_CHANGED	(1<<9)
#define BUTTONLABEL_CHANGED	(1<<10)
#define	POT_CHANGED			(1<<11)
#define	LIMITS_CHANGED		(1<<12)

// Tablemap/scraper limits
#define MAX_CHAR_WIDTH		200
#define	MAX_CHAR_HEIGHT		150
#define MAX_HASH_WIDTH		200
#define MAX_HASH_HEIGHT		150
#define MAX_IMAGE_WIDTH		200
#define MAX_IMAGE_HEIGHT	150
#define MAX_SINGLE_CHAR_WIDTH	31
#define MAX_SINGLE_CHAR_HEIGHT	32

#define MAX_WINDOW_TITLE	512



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum FileType { FTnone=1, FTbankroll, FTdefcon, FTrake, FTnit, FTlist, FTfunc };

struct STableList 
{
	HWND		hwnd;
	CString		title;
	CString		path;
	RECT		crect;
};

#define nTraceFunctions 6
#define nTraceAlli 0
#define nTraceSwag 1
#define nTraceRais 2
#define nTraceCall 3
#define nTracePlay 4
#define nTracePrefold 5

struct SPreferences 
{
	// Analyzer
	unsigned int	max_opponents;

	// Autoplayer
	unsigned int	frame_delay;
	unsigned int	swag_delay_1;
	unsigned int	swag_delay_2;
	unsigned int	swag_delay_3;
	unsigned int	text_selection_method;
	unsigned int	text_deletion_method;
	unsigned int	bet_confirmation_method;
	unsigned int	button_click_method;
	bool			ap_auto;
	bool			focus_detect;

	// Dll extension
	bool			dll_always_send_state;
	bool			load_dll_on_startup;
	CString			dll_name;

	// Scraper
	unsigned int	scrape_delay;

	// Symbols
	double			av_time;
	CString			handrank_value;
	bool			disable_caching;

	// ICM
	double			icm_prize1;
	double			icm_prize2;
	double			icm_prize3;
	double			icm_prize4;

	// Replay Frames
	bool			replay_record;
	bool			replay_record_every_change;
	int				replay_max_frames;

	// Poker Tracker
	struct SPTPrefs
	{
		CString		ip_addr;
		CString		port;
		CString		dbname;
		CString		user;
		CString		pass;
		bool		disable;
		int			update_delay;
		int			cache_refresh;
	} pt_prefs;

	//  2008.02.27 by THF
	//  Perl
	//  (same as registry keys for Perl)
	CString			perl_default_formula;
	CString			perl_editor;
	bool			perl_load_default_formula;
	bool			perl_load_interpreter;

	//  2008.02.27 by THF
	//  PokerChat
	//  (same as registry keys for PokerChat)
	bool			chat_enabled;
	int				chat_min_delay;
	int				chat_random_delay;

    // Log Symbol
    bool            log_symbol_enabled;
    int             log_symbol_max_log;

    // Tracing
	bool			trace_enabled;
	bool			trace_functions[nTraceFunctions];
};

struct SDebugTabInfo 
{
	bool				valid; // Does it have an "="
	int					error; 
	CString				exp;
	boost::spirit::tree_parse_info<const char *, int_factory_t>	tree;
	double				ret;
};

struct ftr_info 
{
	unsigned int	elapsed_time;			//time this hand started
	unsigned int	n_pl_dealt;             //number of players dealt this hand
	unsigned int	n_pl_saw_flop;          //number of players that saw the flop this hand
	unsigned int	n_pl_saw_turn;          //number of players that saw the turn this hand
	unsigned int	n_pl_saw_river;         //number of players that saw the river this hand
	double			n_bets_preflop;         //number of bets preflop (1-4)
};


//prwin 1326 chair structure    Matrix 2008-04-29

struct sprw1326_chair
{
	int			level;			//indicates significance level for 'always consider'
	int			limit;			//max index into significance array - used for computational efficiency
	int			ignore;			//if non-zero no weighting will be applied to this chair
	int			rankhi[1326];	//hi card number in pocket cards
	int			ranklo[1326];	//lo card number in pocket cards
	int			weight[1326];	//the significance value for this hand
	double		scratch;		//for future reference

};

//prwin 1326 structure			Matrix 2008-04-29

struct sprw1326
{
	int			useme;			//unless set to 1326 the normal OH prwin will be used
	int			preflop;		//unless set to 1326 the normal OH prwin will be used pre-flop
	int			usecallback;	//unless set to 1326 the callback function will not be invoked
	double		(*prw_callback)(void); //if enabled will be invoked before the prwin calculation pass
	double		scratch;		//for future reference
	int			bblimp;			//if non-zero no weighting will be applied if a chair has put nothing in the pot
	sprw1326_chair	vanilla_chair;	   //will be precalculated by OH at startup - convenience values
	sprw1326_chair  chair[10];  //structures for each chair
};

// PokerTracker support
// This enum is used for both PT2 and PT3
enum PT_Stats 
{
	pt_icon, pt_hands, pt_pfr, pt_aggp, pt_aggf,
	pt_aggt, pt_aggr, pt_aggtot, pt_aggtotnopf, pt_floppct,
	pt_turnpct, pt_riverpct, pt_vpip, pt_pf_rfi, pt_pf_cr,
	pt_pfats, pt_wsdp, pt_wssd, pt_fbbts, pt_fsbts,
	ptt_icon, ptt_hands, ptt_pfr, ptt_aggp, ptt_aggf,
	ptt_aggt, ptt_aggr, ptt_aggtot, ptt_aggtotnopf, ptt_floppct,
	ptt_turnpct, ptt_riverpct, ptt_vpip, ptt_pf_rfi, ptt_pf_cr,
	ptt_pfats, ptt_wsdp, ptt_wssd, ptt_fbbts, ptt_fsbts, 
	pt_min = pt_icon,
	pt_max = ptt_fsbts
};




#endif /* INC_STRUCTS_DEFINES_H */
