#ifndef INC_STRUCTS_DEFINES_H
#define INC_STRUCTS_DEFINES_H

#include "hash/stdint.h"

#define VERSION_NUMBER				1.10
#define VERSION_TEXT				"1.1.0"  // change version number in OpenHoldem.rc also, when needed

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
#define ERR_INCOMPLETEMASTER		9

// Scraper error codes
#define ERR_FIELD_TOO_LARGE			-1
#define ERR_NOTHING_TO_SCRAPE		-2
#define ERR_NO_HASH_MATCH			-3
#define ERR_TEXT_SCRAPE_NOMATCH		-4
#define ERR_INVALID_TRANSFORM_TYPE	-5
#define ERR_NO_IMAGE_MATCH			-6
#define	ERR_GOOD_SCRAPE_GENERAL		1

// Table map version codes
#define VER_WINSCRAPE			".wsdb1"
#define VER_OPENHOLDEM_1		".ohdb1"
#define VER_OPENHOLDEM_2		".ohdb2"
#define VER_OPENSCRAPE_1		".osdb1"
#define VER_WINSCRAPE_DATE1		"2006 Aug 7 13:11:37"
#define VER_WINSCRAPE_DATE2		"2007 Oct 25 19:26:57"
#define VER_WINSCRAPE_DATE3		"2007 Nov 1 08:32:55"
#define VER_WINSCRAPE_DATE4		"2007 Nov 3 09:54:04"
#define VER_WINSCRAPE_DATE5		"2007 Nov 6 11:52:24"

// Hash seeds
#define HASH_SEED_0		0xf0e1d2c3
#define HASH_SEED_1		0x13245768
#define HASH_SEED_2		0xfdecb9a8
#define HASH_SEED_3		0xaaeeff33

// WH SUIT Defs
#define WH_SUIT_CLUBS		1
#define WH_SUIT_DIAMONDS	2
#define WH_SUIT_HEARTS		3
#define WH_SUIT_SPADES		4

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

// For scraper engine
#define MAX_CHAR_WIDTH		200
#define	MAX_CHAR_HEIGHT		150

#define MAX_HASH_WIDTH		200
#define MAX_HASH_HEIGHT		150

#define MAX_SINGLE_CHAR_WIDTH	31
#define MAX_SINGLE_CHAR_HEIGHT	32


// Bitmap display
#define BITMAP_WIDTH		344
#define BITMAP_HEIGHT		164

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Stablemap_size {
	CString			name;
	unsigned int	width;
	unsigned int	height;
};

struct Stablemap_symbol {
	CString			name;
	CString			text;
};

struct Stablemap_region {
	CString			name;
	unsigned int	left;
	unsigned int	top;
	unsigned int	right;
	unsigned int	bottom;
	COLORREF		color;
	int				radius;
	CString			transform;
};

struct Stablemap_font {
	char			ch;
	int				x_count;
	unsigned int	x[MAX_SINGLE_CHAR_WIDTH];
	CString			hexmash;	// mashed up x[MAX_SINGLE_CHAR_WIDTH] for bsearch purposes
	int				group;
};

struct Stablemap_hash_point {
	unsigned int	number;
	unsigned int	x;
	unsigned int	y;
};

struct Stablemap_hash_value {
	unsigned int	number;
	CString			name;
	uint32_t		hash;
};

struct Stablemap_image {
	CString			name;
	unsigned int	width;
	unsigned int	height;
	uint32_t		pixel[200*150];
};

struct STableMap {
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

};

struct STableList {
	HWND		hwnd;
	CString		title;
	CString		path;
	RECT		crect;
};


#endif /* INC_STRUCTS_DEFINES_H */
