#ifndef INC_STRUCTS_DEFINES_H
#define INC_STRUCTS_DEFINES_H

#include <boost/spirit/tree/ast.hpp>
//#include "..\pokereval\include\handval.h" //!!!
#include "..\OpenHoldem\MagicNumbers.h"

typedef boost::spirit::node_val_data_factory<const char *> int_factory_t;

enum FileType { FTnone=1, FTbankroll, FTdefcon, FTrake, FTnit, FTlist, FTfunc };

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
	int playersPlayingBits;
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
	int lpta;
	phistory_chair chair[k_max_number_of_players];
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
