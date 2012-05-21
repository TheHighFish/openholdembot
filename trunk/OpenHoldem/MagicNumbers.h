#ifndef _INC_MAGICNUMBERS_H 
#define _INC_MAGICNUMBERS_H

#include "resource.h"

// ATTENTION!
//   Do only use elementary data-types here.
//   Complex data-types like objects (especially CStrings)
//   will cause problems during initialization and termination,
//   if they get accessed but are not yet initialized or no longer valid.
//   http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=11724
//   (Same problem like the singletons in OpenHoldem.cpp)

// Used for bit-calculations: 2^N
const int k_exponents[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

// Betting rounds
// Some data-structures use an additional value for the current round.
// This is IMO (THF) superfluous and  dangerous, but lots of work
// and even more dangerous to change.
const int k_number_of_betrounds = 4;
const int k_betround_current	= 0;
const int k_betround_preflop	= 1;
const int k_betround_flop		= 2;
const int k_betround_turn		= 3;
const int k_betround_river		= 4;

// Players
const int k_max_number_of_players    = 10;
const int k_max_length_of_playername = 30;

// Chairs
const int k_min_chair_number = 0;
const int k_max_chair_number = k_max_number_of_players - 1;

// Number of cards
const int k_number_of_community_cards  = 5;
const int k_number_of_flop_cards       = 3;  
const int k_number_of_cards_per_player = 2;
const int k_number_of_cards_per_deck = 52;

// Game type
const int k_gametype_unknown = -1;
const int k_gametype_NL      =  0;
const int k_gametype_PL  	 =  1;
const int k_gametype_FL      =  2;

// Session IDS (max number of tables)
static const int MAX_SESSION_IDS = 25;

// buttons and other scraper constants
const int k_max_number_of_buttons = 10;
const int k_max_number_of_i86X_buttons = 10;
const int k_max_betpot_buttons = 7;  // 2/1, 1/1, 3/4, 2/3, 1/2, 1/3, 1/4

// button codes
const int k_button_i3		= 3;
const int k_button_fold		= 10;
const int k_button_allin	= 11;
const int k_button_raise	= 12;
const int k_button_call		= 13;
const int k_button_check	= 14;
const int k_button_sitin	= 20;
const int k_button_sitout	= 21;
const int k_button_leave	= 22;
const int k_button_prefold	= 23;
const int k_button_autopost	= 24;
const int k_button_i86		= 86;
const int k_button_undefined = -1;

const int k_double_click_delay = 100; // ms

int DefaultButtonNumber(int button_code);

// Tablemap constants
#define RGB_MASK	0x00FFFFFF
const int k_max_number_of_font_groups_in_tablemap = 8;
const int k_max_number_of_hash_groups_in_tablemap = 4;
const int k_max_number_of_titletexts = 10;

// Auto-connector
const int k_max_nmber_of_tablemaps = 25;

// File menu
const int k_number_of_last_recently_used_files_in_file_menu = 4;

// Number of (side)pots
const int k_max_number_of_pots = 10;

// DLL
const int k_number_of_holdem_states_for_DLL = 256;

// Flags
const int k_number_of_flags = 20;

// Suits and ranks
const int k_number_of_suits_per_deck = 4;
const int k_number_of_ranks_per_deck = 13;

// Regions for the region cloner of OpenScrape
// may be used for players, pots, community-cards, etc.
// So we take the maximum.
const int k_max_number_of_regions_to_clone = 10;

// Memory symbols
const int k_max_number_of_memory_symbols = 2048;
const int k_max_length_of_memory_symbols = 64;

// Table stats
const int k_time_frame_for_table_stats_in_seconds = 15 * 60;

// debug tab
const int k_precision_for_debug_tab      = 6;
const int k_integer_places_for_debug_tab = 6;

// Hand history generator
const double k_hand_history_rake = 0.05;

//  File accessable?
//  (<unistd.h> is not contained in MSCVPP)
const int F_OK = 0;

// thread timeout
#define THREAD_WAIT					3000
// "This must not happen."
// It is better to have a named constant then to write "assert(false);".
const bool k_this_must_not_happen = false;

const int k_mutex_lock_time = 500;

// Autoplayer-functions
enum AutoplayerConstant
{
	// Primary autoplayer-functions
	k_autoplayer_function_allin,
	k_autoplayer_function_betpot_2_1,
	k_autoplayer_function_betpot_1_1,
	k_autoplayer_function_betpot_3_4,
	k_autoplayer_function_betpot_2_3,
	k_autoplayer_function_betpot_1_2,
	k_autoplayer_function_betpot_1_3,
	k_autoplayer_function_betpot_1_4,
	k_autoplayer_function_betsize,
	k_autoplayer_function_raise,
	k_autoplayer_function_call,
	k_autoplayer_function_check,
	k_autoplayer_function_fold,
	// Secondary autoplayer-functions
	k_autoplayer_function_prefold,
	k_autoplayer_function_sitin,
	k_autoplayer_function_sitout,
	k_autoplayer_function_leave,
	k_autoplayer_function_autopost,
	k_autoplayer_function_close,
	k_autoplayer_function_rebuy,
	k_autoplayer_function_chat,
	// other functions
	k_autoplayer_function_delay,
	// Attention Always keep this at last place.
	k_number_of_autoplayer_functions
};

const int k_betpot_min = k_autoplayer_function_betpot_2_1;
const int k_betpot_max = k_autoplayer_function_betpot_1_4;
const int k_max_length_betpot_button_name = 17;

#define k_max_betpot_buttons (k_betpot_max - k_betpot_min + 1)
#define k_betpot_index(i) (i - k_betpot_min)

const char k_betpot_button_name[k_max_betpot_buttons][k_max_length_betpot_button_name] =
{
	"betpot_2_1",
	"betpot_1_1",
	"betpot_3_4",
	"betpot_2_3",
	"betpot_1_2",
	"betpot_1_3",
	"betpot_1_4"
};

const int first_primary_autoplayer_function = k_autoplayer_function_allin;
const int last_primary_autoplayer_function = k_autoplayer_function_call;
const int first_secondary_autoplayer_function = k_autoplayer_function_prefold;
const int last_secondary_autoplayer_function = k_autoplayer_function_chat;

// Autoplayer-function names
const int k_max_length_of_autoplayer_functionname = 13;  // +1 for NULL
const char k_autoplayer_functionname[k_number_of_autoplayer_functions][k_max_length_of_autoplayer_functionname] =
{
	"f$alli",
	"f$betpot_2_1",
	"f$betpot_1_1",
	"f$betpot_3_4",
	"f$betpot_2_3",
	"f$betpot_1_2",
	"f$betpot_1_3",
	"f$betpot_1_5",
	"f$betsite",
	"f$rais",
	"f$call",
	"f$prefold",
	"f$sitin",
	"f$sitout",
	"f$leave",
	"f$close",
	"f$rebuy",
	"f$delay",
	"f$chat"
};

enum table_positioner_options
{
	k_position_tables_never,
	k_position_tables_tiled,
	k_position_tables_cascaded
};

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

// Cards
#define CARD_BACK		0xff
#define CARD_NOCARD		0xfe
#define WH_CARDBACK		0xff
#define WH_NOCARD		0

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
#define HANDRESET_DEALER	0x01	// 0b0001
#define HANDRESET_HANDNUM	0x02	// 0b0010
#define HANDRESET_CARDS		0x04	// 0b0100

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

// Autoplayer-trace
// Action constants for the autoplayer
// have to be backward compatible to WinHoldem
// http://www.maxinmontreal.com/wiki/index.php5?title=OpenHoldem:EndUserDocumentation:Symbols#History
// (-1=fold 0=chec 1=call 2=rais 3=swag 4=alli)
enum ActionConstant
{
	k_action_undefined = -2,
	k_action_fold = -1,
	k_action_check = 0,		// new in OpenHoldem
	k_action_call = 1,
	k_action_raise = 2,		// min-raise
	k_action_betsize = 3,	// "stated wager", i.e. using f$betsize
	k_action_allin = 4,
	k_action_jam = 5        // not really an action, but required for correctly logging the slider.
};

// Function to access the name of the action constants.
// As fold is negative we can no longer use the constants 
// as indices for an array.
const char* ActionConstantNames(int action_constant);

// for rank to card translation
const static char *k_card_chars = "23456789TJQKA";
const int k_number_of_different_cardranks = 13;

// Index for current betting round in arrays
// we prefer to store values for preflop..river in 1..4
// and use index 0 for the current round.
// The old code did it differently (4 for the current round)
const int k_storage_index_for_current_round = 0;

// Functions names, both user-defined and standard, e.g. "f$alli"
const int k_max_size_of_function_name = 256;

// Prefix of PPL-symbols that got translated to OH-script.
// Also prefix of symbols in the OpenPPL function library.
const static char *k_open_ppl_symbol_prefix = "f$OpenPPL_";
const int k_lenth_of_open_ppl_symbol_prefix = 10;

// ??? Any longer needed?
const int nTraceFunctions = 9;
const int nTraceAlli      = 0;
const int nTraceSwag      = 1;
const int nTraceRais      = 2;
const int nTraceCall      = 3;
const int nTracePrefold   = 4;
const int nTraceRebuy     = 5;
const int nTraceSitIn     = 6;
const int nTraceSitOut    = 7;
const int nTraceLeave     = 8;

// Player history struct
const int k_assumed_max_number_of_actions_per_player_and_round = 8;

// PrWin
const int k_number_of_pocketcard_combinations_without_order = 1326;

// For assertions of codes, that should never be executed
const bool k_ThisMustNotHappen = false;

// For loggong
const bool k_always_log_errors = true;
const bool k_always_log_basic_information = true;

// Statusbar
static UINT basic_statusba_indicators[] =
{
	ID_INDICATOR_GENERAL_HINT // Do you need "Help -> Problem solver?"
};

static UINT indicators[] = 
{
	ID_SEPARATOR,		   // status line indicator !!! Bad name
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
	ID_INDICATOR_STATUS_PLCARDS,
	ID_INDICATOR_STATUS_COMCARDS,
	ID_INDICATOR_STATUS_POKERHAND,
	ID_INDICATOR_STATUS_HANDRANK,
	ID_INDICATOR_STATUS_PRWIN,
	ID_INDICATOR_STATUS_NOPP,
	ID_INDICATOR_STATUS_nit,
	ID_INDICATOR_STATUS_ACTION,
};

enum PokerTrackerGameTypes
{
	k_pokertracker_cashgame,
	k_pokertracker_sng_mtt
};

enum PokerTrackerBasicStatIDs
{
	// voluntary put in preflop	
	k_pt_VPIP = 0,
	//...,
	k_number_of_basic_queries
};

enum PokerTrackerPositionalPreflopStatIDs
{
	// voluntary put in preflop from BU
	k_pt_VPIP_BU,
	//...
};

enum PokerTrackerPositionalAdvancedStatIDs
{

};






#endif // _INC_MAGICNUMBERS_H


