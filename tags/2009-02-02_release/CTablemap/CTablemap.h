#ifndef INC_CTABLEMAP_H
#define INC_CTABLEMAP_H

#include <map>
#include "../CTransform/hash/stdint.h"
#include "../CTransform/pdiff/RGBAImage.h"
#include "..\CCritSec\CCritSec.h"

///////////////////////////////
// structs
///////////////////////////////
struct STablemapSize 
{
	CString			name;
	unsigned int	width;
	unsigned int	height;
};

struct STablemapSymbol 
{
	CString			name;
	CString			text;
};

struct STablemapRegion 
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

struct STablemapFont 
{
	char			ch;
	int				x_count;
	unsigned int	x[MAX_SINGLE_CHAR_WIDTH];
	CString			hexmash;	// mashed up x[MAX_SINGLE_CHAR_WIDTH] for lookup purposes
	int				group;
};

struct STablemapHashPoint 
{
	unsigned int	number;
	unsigned int	x;
	unsigned int	y;
};

struct STablemapHashValue 
{
	unsigned int	number;
	CString			name;
	uint32_t		hash;
};

struct STablemapImage 
{
	CString			name;
	unsigned int	width;
	unsigned int	height;
	uint32_t		pixel[MAX_HASH_WIDTH*MAX_HASH_HEIGHT];
	RGBAImage		*image;
};

struct SR$Indexes
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Holds indexes to _r$ entries for fast lookup - populated by save_r$indexes()
	// common card info
	int		r$c0cardfaceX_index[5];
	int		r$c0cardfaceXrank_index[5];
	int		r$c0cardfaceXsuit_index[5];

	//  Chat support
	//  2008.02.21 by THF
	int		r$chatbox;

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
	int 	r$c0potXchipYZ_index[10][10][10];

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
};

struct SS$Indexes
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Holds indexes to _s$ entries for fast lookup - populated by save_s$indexes()
	int		s$ttlimits_index;
	int		s$ttlimitsX_index[10];
	int 	s$c0limits_index;
	int		s$c0limitsX_index[10];
};

struct SS$Items
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	// Holds contents of _s$ entries for fast lookup - populated by save_s$strings()
	// player info
	int		num_chairs;
	int		swagtextmethod;
	int		potmethod;
	int		activemethod;
	CString	sitename;
	CString network;
	CString	chairconfig;
	CString	ttype[4];
};

struct SWholeMap
{
	const CArray <STablemapSize, STablemapSize>			*z$;
	const CArray <STablemapSymbol, STablemapSymbol>		*s$;
	const CArray <STablemapRegion, STablemapRegion> 		*r$;
	const CArray <STablemapFont, STablemapFont>			*t$;
	const CArray <STablemapHashPoint, STablemapHashPoint>	*p$;
	const CArray <STablemapHashValue, STablemapHashValue>	*h$;
	const CArray <STablemapImage, STablemapImage>			*i$;
	CString filepath;
};

extern class CTablemap 
{
public:
	// public functions
	CTablemap(void);
	~CTablemap(void);
	void ClearTablemap();
	int LoadTablemap(const char *_filename, const char *version, const bool check_ws_date, int *linenum, CString *loaded_version=NULL);
	int SaveTablemap(CArchive& ar, const char *version_text);
	int ConvertTablemap(const HWND hwnd=NULL, const char *startup_path="");
	int UpdateHashes(const HWND hwnd, const char *startup_path);
	void ClearR$Indices(void);
	void SaveR$Indices(void);
	void SaveS$Indices(void);
	void SaveS$Strings(void);
	void UpdateHexmashesHashes(const int group);

public:
	// public accessors
	const CArray <STablemapSize, STablemapSize>	* z$() { return &_z$; }
	const CArray <STablemapSymbol, STablemapSymbol> * s$() { return &_s$; }
	const CArray <STablemapRegion, STablemapRegion> * r$() { return &_r$; }
	const CArray <STablemapFont, STablemapFont> * t$() { return &_t$; }
	const CArray <STablemapHashPoint, STablemapHashPoint> * p$() { return &_p$; }
	const CArray <STablemapHashValue, STablemapHashValue> * h$() { return &_h$; }
	const CArray <STablemapImage, STablemapImage> * i$() { return &_i$; }
	const std::map<uint32_t, int> * hashes(const int n) { return &(_hashes[n]); }
	const std::map<CString, int> * hexmashes(const int n) { return &(_hexmashes[n]); }
	const SR$Indexes * r$indexes() { return &_r$indexes; }
	const SS$Indexes * s$indexes() { return &_s$indexes; }
	const SS$Items * s$items() { return &_s$items; }
	const bool valid() { return _valid; }
	const CString filename() { return _filename; }
	const CString filepath() { return _filepath; }


public:
#define ENT CSLock lock(m_critsec);
	// public mutators 

	// These are used by OpenScrape
	void	set_r$_left(const int n, const unsigned int i) { ENT if (n>=0 && n<=_r$.GetSize()) _r$[n].left = i;  }
	void	set_r$_right(const int n, const unsigned int i) { ENT if (n>=0 && n<=_r$.GetSize()) _r$[n].right = i;  }
	void	set_r$_top(const int n, const unsigned int i) { ENT if (n>=0 && n<=_r$.GetSize()) _r$[n].top = i;  }
	void	set_r$_bottom(const int n, const unsigned int i) { ENT if (n>=0 && n<=_r$.GetSize()) _r$[n].bottom = i;  }
	void	set_r$_color(const int n, const COLORREF c) { ENT if (n>=0 && n<=_r$.GetSize()) _r$[n].color = c;  }
	void	set_r$_radius(const int n, const unsigned int i) { ENT if (n>=0 && n<=_r$.GetSize()) _r$[n].radius = i;  }
	void	set_r$_transform(const int n, const CString s) { ENT if (n>=0 && n<=_r$.GetSize()) _r$[n].transform = s;  }

	void	set_z$_removeat(const int n) { ENT if (n>=0 && n<=_z$.GetSize()) _z$.RemoveAt(n,1);  }
	void	set_s$_removeat(const int n) { ENT if (n>=0 && n<=_s$.GetSize()) _s$.RemoveAt(n,1);  }
	void	set_r$_removeat(const int n) { ENT if (n>=0 && n<=_r$.GetSize()) _r$.RemoveAt(n,1);  }
	void	set_t$_removeat(const int n) { ENT if (n>=0 && n<=_t$.GetSize()) _t$.RemoveAt(n,1);  }
	void	set_p$_removeat(const int n) { ENT if (n>=0 && n<=_p$.GetSize()) _p$.RemoveAt(n,1);  }
	void	set_h$_removeat(const int n) { ENT if (n>=0 && n<=_h$.GetSize()) _h$.RemoveAt(n,1);  }
	void	set_i$_removeat(const int n) { ENT if (n>=0 && n<=_i$.GetSize()) _i$.RemoveAt(n,1);  }

	void	set_z$_setat(const int n, const STablemapSize s) { ENT if (n>=0 && n<=_z$.GetSize()) _z$.SetAt(n,s);  }
	void	set_s$_setat(const int n, const STablemapSymbol s) { ENT if (n>=0 && n<=_s$.GetSize()) _s$.SetAt(n,s);  }
	void	set_r$_setat(const int n, const STablemapRegion s) { ENT if (n>=0 && n<=_r$.GetSize()) _r$.SetAt(n,s);  }
	void	set_t$_setat(const int n, const STablemapFont s) { ENT if (n>=0 && n<=_t$.GetSize()) _t$.SetAt(n,s);  }
	void	set_p$_setat(const int n, const STablemapHashPoint s) { ENT if (n>=0 && n<=_p$.GetSize()) _p$.SetAt(n,s);  }

	void	set_p$_removeall() { ENT _p$.RemoveAll();  }
	void	set_i$_image_pixel(const int n1, const int n2, const uint32_t p) { ENT if (n1>=0 && n1<=_i$.GetSize()) _i$[n1].pixel[n2] = p;  }
	void	set_i$_image_set(const int n, const BYTE r, const BYTE g, const BYTE b, const BYTE a, const int p) 
				{ ENT if (n>=0 && n<=_i$.GetSize()) _i$[n].image->Set(r,g,b,a,p);  }
	void	set_i$_image(const int n, RGBAImage *i) { ENT if (n>=0 && n<=_i$.GetSize()) _i$[n].image = i;  }
	void	set_t$_insertat(const int n, const STablemapFont s) { ENT if (n>=0 && n<=_t$.GetSize()) _t$.InsertAt(n,s);  }

	void	set_r$_lastbmp(const int n, const HBITMAP h) { ENT if (n>=0 && n<=_r$.GetSize()) _r$[n].last_bmp = h;  }
	void	set_r$_curbmp(const int n, const HBITMAP h) { ENT if (n>=0 && n<=_r$.GetSize()) _r$[n].cur_bmp = h;  }
	void	delete_r$_lastbmp(const int n) { ENT if (n>=0 && n<=_r$.GetSize()) { DeleteObject(_r$[n].last_bmp); _r$[n].last_bmp=NULL; }  }
	void	delete_r$_curbmp(const int n) { ENT if (n>=0 && n<=_r$.GetSize()) { DeleteObject(_r$[n].cur_bmp); _r$[n].cur_bmp=NULL; }   }
	void	set_s$items_network(const CString s) { ENT _s$items.network=s; }

	const INT_PTR	set_z$_add(const STablemapSize s) { ENT return _z$.Add(s); }
	const INT_PTR	set_s$_add(const STablemapSymbol s) { ENT return _s$.Add(s); }
	const INT_PTR	set_r$_add(const STablemapRegion s) { ENT return _r$.Add(s); }
	const INT_PTR	set_t$_add(const STablemapFont s) { ENT return _t$.Add(s); }
	const INT_PTR	set_p$_add(const STablemapHashPoint s) { ENT return _p$.Add(s); }
	const INT_PTR	set_h$_add(const STablemapHashValue s) { ENT return _h$.Add(s); }
	const INT_PTR	set_i$_add(const STablemapImage s) { ENT return _i$.Add(s); }

#undef ENT

private:
	// private variables - use public accessors and public mutators to address these
	bool		_valid;
	CString		_filename;
	CString		_filepath;
	CArray <STablemapSize, STablemapSize>				_z$;
	CArray <STablemapSymbol, STablemapSymbol>			_s$;
	CArray <STablemapRegion, STablemapRegion>			_r$;
	CArray <STablemapFont, STablemapFont>				_t$;
	std::map<CString, int>								_hexmashes[4]; // Font _hexmashes for fast lookups - font type 0-3 are indexed in the array
	CArray <STablemapHashPoint, STablemapHashPoint>		_p$;
	CArray <STablemapHashValue, STablemapHashValue>		_h$;
	std::map<uint32_t, int>								_hashes[4]; // Region _hashes for fast lookups - hash type 0-3 are indexed in the array
	CArray <STablemapImage, STablemapImage>				_i$;
	SR$Indexes	_r$indexes;
	SS$Indexes	_s$indexes;
	SS$Items	_s$items;

private:
	// private functions and variables - not available via accessors or mutators
	CCritSec		m_critsec;

} *p_tablemap;

#endif //INC_CTABLEMAP_H