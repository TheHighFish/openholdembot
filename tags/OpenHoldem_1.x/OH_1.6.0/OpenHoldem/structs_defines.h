#ifndef INC_STRUCTS_DEFINES_H
#define INC_STRUCTS_DEFINES_H

#include "hash/stdint.h"
#include <boost/spirit/tree/ast.hpp>

#define VERSION_NUMBER				1.60
#define VERSION_TEXT				"1.6.0"  // change version number in OpenHoldem.rc also, when needed

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
#define ERR_INCOMPLETEMASTER		14
#define ERR_DIV_ZERO				15

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
#define VER_WINSCRAPE_DATE1		"2006 Aug 7 13:11:37"
#define VER_WINSCRAPE_DATE2		"2007 Nov 1 08:32:55"
#define VER_WINSCRAPE_DATE3		"2007 Nov 3 09:54:04"
#define VER_WINSCRAPE_DATE4		"2007 Nov 6 11:52:24"

// Hash seeds
#define HASH_SEED_0		0xf0e1d2c3
#define HASH_SEED_1		0x13245768
#define HASH_SEED_2		0xfdecb9a8
#define HASH_SEED_3		0xaaeeff33

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
#define MAX_HAND_LISTS	100		// e.g. islist0 - islist99

// Preferences
#define MAX_OPPONENTS		22	
#define MAX_FRAMEDELAY		9	
#define MAX_SWAGDELAY1		5000	
#define MAX_SWAGDELAY2		5000	
#define MAX_SWAGDELAY3		5000	
#define MAX_ALLINDELAY		5000	
#define MAX_SCRAPEDELAY		5000
#define MIN_SCRAPEDELAY		100
#define MAX_NAMESCRAPES		9
#define MAX_BALANCESCRAPES	9

// WH SUIT Defs
#define WH_SUIT_CLUBS		1
#define WH_SUIT_DIAMONDS	2
#define WH_SUIT_HEARTS		3
#define WH_SUIT_SPADES		4

// Chipstacks
#define MAX_CHIPS_PER_STACK	20
#define MAX_CHIP_STACKS		10


// SWAG options
#define TEXTSEL_DOUBLECLICK	1
#define TEXTSEL_CLICKDRAG	2
#define TEXTDEL_DELETE		1
#define TEXTDEL_BACKSPACE	2
#define BETCONF_ENTER		1
#define BETCONF_CLICKBET	2

// Button click options
#define BUTTON_SINGLECLICK	1
#define BUTTON_DOUBLECLICK	2

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
#define COLOR_GRAY		RGB(182,182,182)
#define COLOR_YELLOW	RGB(255,255,0)

// For scraper engine
#define MAX_CHAR_WIDTH		200
#define	MAX_CHAR_HEIGHT		150

#define MAX_HASH_WIDTH		200
#define MAX_HASH_HEIGHT		150

#define MAX_SINGLE_CHAR_WIDTH	31
#define MAX_SINGLE_CHAR_HEIGHT	32

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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Structs
struct SHandList 
{
	CString list;
	CString list_text;
};

struct SFunction 
{
	CString		func;
	CString		func_text;
	bool		dirty;
	boost::spirit::tree_parse_info<>	tpi;
	bool		fresh;
	double		cache;
};

struct SFormula 
{
	CArray <SFunction, SFunction> mFunction;

	double dBankroll, dDefcon, dRake, dNit;

	CArray <SHandList, SHandList> mHandList;

	bool	inlist[1000][13][13];  // list number, rank0/rank1 - rank0>=rank1 == suited, rank0<rank1 == unsuited
};

enum FileType { FTnone=1, FTbankroll, FTdefcon, FTrake, FTnit, FTlist, FTfunc };

struct Stablemap_size 
{
	CString			name;
	unsigned int	width;
	unsigned int	height;
};

struct Stablemap_symbol 
{
	CString			name;
	CString			text;
};

struct Stablemap_region 
{
	CString			name;
	unsigned int	left;
	unsigned int	top;
	unsigned int	right;
	unsigned int	bottom;
	COLORREF		color;
	int				radius;
	CString			transform;

	HBITMAP			cur_bmp;
	HBITMAP			last_bmp;
};

struct Stablemap_font 
{
	char			ch;
	int				x_count;
	unsigned int	x[MAX_SINGLE_CHAR_WIDTH];
	CString			hexmash;	// mashed up x[MAX_SINGLE_CHAR_WIDTH] for bsearch purposes
	int				group;
};

struct Stablemap_hash_point 
{
	unsigned int	number;
	unsigned int	x;
	unsigned int	y;
};

struct Stablemap_hash_value 
{
	unsigned int	number;
	CString			name;
	uint32_t		hash;
};

struct Stablemap_image 
{
	CString			name;
	unsigned int	width;
	unsigned int	height;
	uint32_t		pixel[200*150];
};

struct STableMap 
{
	bool		valid;

	CString		filename;
	CString		filepath;

	CArray <Stablemap_size, Stablemap_size>				z$;
	CArray <Stablemap_symbol, Stablemap_symbol>			s$;
	CArray <Stablemap_region, Stablemap_region>			r$;
	uint32_t											hashes[512];	// Region hashes for bsearches
	CArray <Stablemap_font, Stablemap_font>				t$;
	char 												hexmash[512][MAX_SINGLE_CHAR_WIDTH*8 + 1];	// Font hexmashes for bsearches
	CArray <Stablemap_hash_point, Stablemap_hash_point>	p$;
	CArray <Stablemap_hash_value, Stablemap_hash_value>	h$;
	CArray <Stablemap_image, Stablemap_image>			i$;


	///////////////////////////////////////////////////////////////////////////////////////////////
	// Holds indexes to r$ entries for fast lookup - populated by save_r$indexes()
	// common card info
	int		r$c0cardfaceX_index[5];
	int		r$c0cardfaceXrank_index[5];
	int		r$c0cardfaceXsuit_index[5];

	// player info
	int		r$uXcardfaceY_index[10][2];
	int		r$pXcardfaceY_index[10][2];
	int		r$pXcardfaceYrank_index[10][2];
	int		r$pXcardfaceYsuit_index[10][2];
	int		r$pXcardback_index[10];
	int		r$uXseated_index[10];
	int		r$pXseated_index[10];
	int		r$uXactive_index[10];
	int		r$pXactive_index[10];
	int		r$pXdealer_index[10];
	int		r$uname_index;
	int		r$uXname_index[10];
	int		r$pXname_index[10];
	int		r$ubalance_index;
	int		r$uXbalance_index[10];
	int		r$pXbalance_index[10];
	int		r$pXbet_index[10];
	int		r$pXchip_index[10][10][10];

	// button info
	int		r$iXbutton_index[10];
	int		r$iXstate_index[10];
	int		r$iXlabel_index[10];
	int		r$iXlabelY_index[10][10];
	int		r$iXslider_index[10];
	int		r$iXhandle_index[10];
	int		r$iXedit_index[10];
	int		r$i86Xbutton_index[10];
	int		r$i86button_index;
	int		r$i86Xstate_index[10];
	int		r$i86state_index;

	// pots
	int 	r$c0potX_index[10];
	int 	r$c0potXchipYZ_index[5][10][10];

	// limits
	int 	r$c0limits_index;
	int		r$c0limitsX_index[10];
	int		r$c0istournament_index;
	int		r$c0sblind_index;
	int		r$c0bblind_index;
	int		r$c0bigbet_index;
	int		r$c0ante_index;
	int		r$c0handnumber_index;
	int		r$c0handnumberX_index[10];

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Holds indexes to s$ entries for fast lookup - populated by save_s$indexes()
	int		s$ttlimits_index;
	int		s$ttlimitsX_index[10];
	int 	s$c0limits_index;
	int		s$c0limitsX_index[10];

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Holds contents of s$ entries for fast lookup - populated by save_s$strings()
	// player info
	int		num_chairs;
	int		swagtextmethod;
	CString	sitename;
	CString	chairconfig;
	CString	ttype[4];
};

struct STableList 
{
	HWND		hwnd;
	CString		title;
	CString		path;
	RECT		crect;
};

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

	// Dll extension
	bool			dll_always_send_state;
	bool			load_dll_on_startup;
	CString			dll_name;

	// Scraper
	unsigned int	scrape_delay;
	unsigned int	name_scrapes;
	unsigned int	balance_scrapes;

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

};

struct SDebugTabInfo 
{
	bool				valid;
	int					error;
	CString				exp;
	boost::spirit::tree_parse_info<>	tree;
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


struct s_tablemaps								// used to pass loaded profiles to enumwindows callback
{
	STableMap	tablemap[15];
	int			n_elem;
};
#define	MAX_MAPS_IN_STABLEMAPS  15

// PokerTracker support
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
