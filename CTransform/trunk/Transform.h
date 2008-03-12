#pragma once

#include "hash/stdint.h"

///////////////////////////////
// defines
///////////////////////////////
#define DEFAULT_FUZZY_TOLERANCE	0.25

#define MAX_CHAR_WIDTH		200
#define	MAX_CHAR_HEIGHT		150

#define MAX_HASH_WIDTH		200
#define MAX_HASH_HEIGHT		150

#define MAX_SINGLE_CHAR_WIDTH	31
#define MAX_SINGLE_CHAR_HEIGHT	32

// Hash seeds
#define HASH_SEED_0		0xf0e1d2c3
#define HASH_SEED_1		0x13245768
#define HASH_SEED_2		0xfdecb9a8
#define HASH_SEED_3		0xaaeeff33

// Scraper error codes
#define ERR_FIELD_TOO_LARGE			-1
#define ERR_NOTHING_TO_SCRAPE		-2
#define ERR_NO_HASH_MATCH			-3
#define ERR_TEXT_SCRAPE_NOMATCH		-4
#define ERR_INVALID_TRANSFORM_TYPE	-5
#define ERR_NO_IMAGE_MATCH			-6
#define	ERR_GOOD_SCRAPE_GENERAL		1

#ifndef LIMIT_NL
#define LIMIT_NL		0
#endif

#ifndef LIMIT_PL
#define LIMIT_PL		1
#endif

#ifndef LIMIT_FL
#define LIMIT_FL		2
#endif


///////////////////////////////
// structs
///////////////////////////////
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

	//  Chat support
	//  2008.02.21 by THF
    int     r$chatbox;

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
	CString network;
	CString	chairconfig;
	CString	ttype[4];
};


///////////////////////////////
// classes
///////////////////////////////
class CTransform
{
public:
	CTransform(void);
	~CTransform(void);
	int do_transform(STableMap *map, Stablemap_region *region, HDC hdc, CString *text, 
					 CString *separation=NULL, COLORREF *cr_avg=0);
	int t_transform(STableMap *map, Stablemap_region *region, HDC hdc, CString *text, 
					CString *separation, bool background[], bool character[][MAX_CHAR_HEIGHT]);
	void parse_string_bsl(CString text, CString format, CString *results=NULL, 	
		double *handnumber=NULL, double *sblind=NULL, double *bblind=NULL, double *bbet=NULL, 
		double *ante=NULL, int *limit=NULL, double *sb_bb=NULL, double *bb_BB=NULL, bool *istournament=NULL,
		bool *found_handnumber=NULL, bool *found_sblind=NULL, bool *found_bblind=NULL, bool *found_bbet=NULL,
		bool *found_ante=NULL, bool *found_limit=NULL, bool *found_sb_bb=NULL, bool *found_bb_BB=NULL);
	bool is_in_ARGB_color_cube(int center_a, int center_r, int center_g, int center_b, 
							   int radius, int pix_a, int pix_r, int pix_g, int pix_b);
	bool is_in_RGB_color_cube(int center_r, int center_g, int center_b, int radius, int pix_r, int pix_g, int pix_b);
	void get_shift_left_down_indexes(int x_start, int width, int height, bool background[], bool character[][MAX_CHAR_HEIGHT], 
										  int *x_begin, int *x_end, int *y_begin, int *y_end);
	void calc_hexmash(int left, int right, int top, int bottom, bool (*ch)[MAX_CHAR_HEIGHT], char *hexmash, bool withspace=false);
	double string_to_money(CString str);

protected:
	int c_transform(Stablemap_region *region, HDC hdc, CString *text, COLORREF *cr_avg);
	int i_transform(Stablemap_region *region, HDC hdc, CString *text);
	int h_transform(STableMap *map, Stablemap_region *region, HDC hdc, CString *text);
	int do_plain_font_scan(STableMap *map, Stablemap_region *region, int width, int height, 
						   bool bg[], bool (*ch)[MAX_CHAR_HEIGHT], CString *text);
	int do_fuzzy_font_scan(STableMap *map, Stablemap_region *region, int width, int height, 
						   bool bg[], bool (*ch)[MAX_CHAR_HEIGHT], double tolerance, CString *text);
	int get_best_hd(STableMap *map, Stablemap_region *region, int width, int height, bool bg[], 
					bool (*ch)[MAX_CHAR_HEIGHT], int left, double tolerance);
	int hamdist(unsigned int x, unsigned int y);
};

int compare_font( char *hexmash1, char *hexmash2);
int compare_hash( uint32_t *hash1, uint32_t *hash2);
int	bitcount(unsigned int u);
