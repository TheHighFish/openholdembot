#ifndef INC_STRUCTS_DEFINES_H
#define INC_STRUCTS_DEFINES_H

#include <boost/spirit/tree/ast.hpp>
#include "..\pokereval\include\poker_defs.h"
#include "..\OpenHoldem\MagicNumbers.h"

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
// No error code 14
#define ERR_DIV_ZERO				15
#define ERR_INVALID_F$$_REF			16
#define ERR_BAD_PARSE				17
#define ERR_INCOMPLETEMASTER		18

const CString k_tablemap_errors_and_parse_errors_explained[19] =	
	{"success", "unexpected end of file", "invalid syntax", "incorrect version",
	"not a master file", "hash collision", "invalid region size", "unknown line type",
	"invalid hash type", "missing equal sign", "invalid symbol", "invalid function symbol",
	"invalid DLL symbol", "invalid expression", "this can't happen, as this error-code is unused", "division by zero", 
	"invalid f$$-reference", "bad parse", "incomplete master file"};

// Scraper error codes
#define ERR_FIELD_TOO_LARGE			-1
#define ERR_NOTHING_TO_SCRAPE		-2
#define ERR_NO_HASH_MATCH			-3
#define ERR_TEXT_SCRAPE_NOMATCH		-4
#define ERR_INVALID_TRANSFORM_TYPE	-5
#define ERR_NO_IMAGE_MATCH			-6
#define	ERR_GOOD_SCRAPE_GENERAL		1

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
#define MAX_CLICKDELAY		5000
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

// Button click options
#define BETPOT_DEFAULT	0
#define BETPOT_RAISE	1

// Hand reset detection options
#define HANDRESET_DEALER	0x01	// 0b0001
#define HANDRESET_HANDNUM	0x02	// 0b0010
#define HANDRESET_CARDS		0x04	// 0b0100

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

enum FileType { FTnone=1, FTlist, FTfunc };

struct STableList 
{
	HWND		hwnd;
	CString		title;
	CString		path;
	RECT		crect;
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
	int			level;			// indicates significance level for 'always consider'
	int			limit;			// max index into significance array - used for computational efficiency
	int			ignore;			// if non-zero no weighting will be applied to this chair
	int			rankhi[k_number_of_pocketcard_combinations_without_order];	// hi card number in pocket cards
	int			ranklo[k_number_of_pocketcard_combinations_without_order];	// lo card number in pocket cards
	int			weight[k_number_of_pocketcard_combinations_without_order];	// the significance value for this hand
	double		scratch;		// for future reference

};

//prwin 1326 structure			Matrix 2008-04-29
struct sprw1326
{
	int			useme;				// unless set to 1326 the normal OH prwin will be used
	int			preflop;			// unless set to 1326 the normal OH prwin will be used pre-flop
	int			usecallback;		// unless set to 1326 the callback function will not be invoked
	double		(*prw_callback)(void); // if enabled will be invoked before the prwin calculation pass
	double		scratch;			// for future reference
	int			bblimp;				// if non-zero no weighting will be applied if a chair has put nothing in the pot
	sprw1326_chair	vanilla_chair;	// will be precalculated by OH at startup - convenience values
	sprw1326_chair  chair[k_max_number_of_players];  // structures for each chair
};

//player history structure		Demonthus 2010-02-05
// !!! undocumented
struct phistory_chair
{
	char card_player[(2 * k_number_of_cards_per_player) + 1]; // 2 characters per card + NULL char
	bool dealt;
	bool postBlind;
	bool seatIsPlaying;
	bool cardsSeen;
	double startBalance;
	double currentBalance;
	double prevBalance;
	double endBalance;
	double currentBet; 
	double prevBet;
	double totalIn[k_number_of_betrounds];
	double bet[k_number_of_betrounds][k_assumed_max_number_of_actions_per_player_and_round];
	int action[k_number_of_betrounds][k_assumed_max_number_of_actions_per_player_and_round];
	int ac_dealpos;
	bool playersPlayingBits;
	int bets;
	int calls;
	int actionCount;
	CardMask hand;
    HandVal handval; 
};

//player history structure		Demonthus 2010-02-05

struct phistory
{
	int whosturn;
	int sblindpos;
	int bblindpos;
	int utg;
	int last_player_to_act;
	phistory_chair chair[k_max_number_of_players];
};

#endif /* INC_STRUCTS_DEFINES_H */
