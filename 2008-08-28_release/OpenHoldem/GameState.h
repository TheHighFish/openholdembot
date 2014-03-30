#include "dll_extension.h"

extern class GameState {
public:

	GameState();
	void	process_game_state(holdem_state* pstate);
	void	process_ftr(holdem_state* pstate);
	int		lastraised(int round);
	int		raisbits(int round);
	int		callbits(int round);
	int		foldbits(int round);

	double	floppct(void);
	double	turnpct(void);
	double	riverpct(void);
	double	avgbetspf(void);
	double	tablepfr(void);
	double	wh_sym_hist(char * sym, int round);
	double	sortedbalance(int rank);

	holdem_state		m_holdem_state[256];
	unsigned char		m_ndx;
	int					oppdealt;
	double				max_balance;
	int					hands_played;
	bool				new_hand;

private:
	void	dump_state(void) ;
	void	process_state_engine(holdem_state* pstate, bool pstate_changed) ;
	void	process_ftr_engine(holdem_state* pstate) ;

	ftr_info			m_ftr[256];
	int					m_ftr_ndx;
	holdem_state		m_game_state[256];
	unsigned char		m_game_ndx;
	bool				chair_actions[10][4][w_num_action_types];   // 10 chairs, 4 rounds, number of action types
	double				hist_sym[200][4];
	static const int	hist_sym_count=121;
	unsigned int		avtime;

} game_state;

char * const hist_sym_strings[200] = {
	//PROBABILITIES (3)
	"prwin", "prlos", "prtie", 
	
	//CHIP AMOUNTS (21)
	"balance", "balance0", "balance1", "balance2", "balance3", "balance4", "balance5", 
	"balance6", "balance7", "balance8", "balance9", "stack0", "stack1", "stack2", "stack3", 
	"stack4", "stack5", "stack6", "stack7", "stack8", "stack9", 

	//POKER VALUES (5)
	"pokerval", "pokervalplayer", "pokervalcommon", "pcbits", "npcbits", 

	//HAND TESTS (13)
	"ishandup", "ishandupcommon", "ishicard", "isonepair", "istwopair", "isthreeofakind", 
	"isstraight", "isflush", "isfullhouse", "isfourofakind", "isstraightflush", "isroyalflush", 
	"isfiveofakind", 

	//POCKET/COMMON TESTS (5)
	"ishipair", "islopair", "ismidpair", "ishistraight", "ishiflush", 

	//(UN)KNOWN CARDS (2)
	"nouts", "ncardsbetter", 

	//NHANDS (6)
	"nhands", "nhandshi", "nhandslo", "nhandsti", "prwinnow", "prlosnow", 

	//FLUSHES SETS STRAIGHTS (16)
	"nsuited", 	"nsuitedcommon", "tsuit", "tsuitcommon", "nranked", "nrankedcommon", "trank", 
	"trankcommon", "nstraight", "nstraightcommon", "nstraightfill", "nstraightfillcommon", 
	"nstraightflush", "nstraightflushcommon", "nstraightflushfill", "nstraightflushfillcommon", 

	//RANK BITS (8)
	"rankbits", "rankbitscommon", "rankbitsplayer", "rankbitspoker", "srankbits", 
	"srankbitscommon", "srankbitsplayer", "srankbitspoker", 

	//RANK HI (8)
	"rankhi", "rankhicommon", "rankhiplayer", "rankhipoker", "srankhi", "srankhicommon", 
	"srankhiplayer", "srankhipoker", 

	//RANK LO (8)
	"ranklo", "ranklocommon", "rankloplayer", "ranklopoker", "sranklo", "sranklocommon", 
	"srankloplayer", "sranklopoker", 

	//run$ ron$ (26)
	"ron$royfl", "ron$strfl", "ron$4kind", "ron$fullh", "ron$strai", "ron$3kind", "ron$2pair", 
	"ron$1pair", "ron$hcard", "ron$total", "ron$pokervalmax", "ron$prnuts", "ron$prbest", 
	"run$royfl", "run$strfl", "run$4kind", "run$fullh", "run$strai", "run$3kind", "run$2pair", 
	"run$1pair", "run$hcard", "run$total", "run$pokervalmax", "run$prnuts", "run$prbest"
};
