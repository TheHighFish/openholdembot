#pragma once
#include "structs_defines.h"

#define DEFAULT_FUZZY_TOLERANCE	0.25

#ifndef LIMIT_NL
#define LIMIT_NL		0
#endif

#ifndef LIMIT_PL
#define LIMIT_PL		1
#endif

#ifndef LIMIT_FL
#define LIMIT_FL		2
#endif

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
	int do_plain_font_scan(STableMap *map, int width, int height, bool bg[], bool (*ch)[MAX_CHAR_HEIGHT], CString *text);
	int do_fuzzy_font_scan(STableMap *map, Stablemap_region *region, int width, int height, 
						   bool bg[], bool (*ch)[MAX_CHAR_HEIGHT], double tolerance, CString *text);
	int get_best_hd(STableMap *map, Stablemap_region *region, int width, int height, bool bg[], 
					bool (*ch)[MAX_CHAR_HEIGHT], int left, double tolerance);
	int hamdist(unsigned int x, unsigned int y);
};

int compare_font( char *hexmash1, char *hexmash2);
int compare_hash( uint32_t *hash1, uint32_t *hash2);
int	bitcount(unsigned int u);
