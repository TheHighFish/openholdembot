//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 


#ifndef INC_CTABLEMAP_H
#define INC_CTABLEMAP_H

#include <map>
#include "../CTransform/hash/stdint.h"
#include "../CTransform/pdiff/RGBAImage.h"
#include "../CCritSec/CCritSec.h"
#include "../OpenHoldem/MagicNumbers.h"

///////////////////////////////
// structs
///////////////////////////////
struct STablemapSize 
{
	CString			name;
	unsigned int	width;
	unsigned int	height;
};
typedef std::pair<CString, STablemapSize> ZPair;
typedef std::map<CString, STablemapSize> ZMap;
typedef ZMap::iterator ZMapI;
typedef ZMap::const_iterator ZMapCI;

struct STablemapSymbol 
{
	CString			name;
	CString			text;
};
typedef std::pair<CString, STablemapSymbol> SPair;
typedef std::map<CString, STablemapSymbol> SMap;
typedef SMap::iterator SMapI;
typedef SMap::const_iterator SMapCI;

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
typedef std::pair<CString, STablemapRegion> RPair;
typedef std::map<CString, STablemapRegion> RMap;
typedef RMap::iterator RMapI;
typedef RMap::const_iterator RMapCI;

struct STablemapFont 
{
	char			ch;
	int				x_count;
	unsigned int	x[MAX_SINGLE_CHAR_WIDTH];
	CString			hexmash;	// mashed up x[MAX_SINGLE_CHAR_WIDTH] for lookup purposes
};
typedef std::pair<CString, STablemapFont> TPair;
typedef std::map<CString, STablemapFont> TMap;
typedef TMap::iterator TMapI;
typedef TMap::const_iterator TMapCI;

struct STablemapHashPoint 
{
	unsigned int	x;
	unsigned int	y;
};
typedef std::pair<uint32_t, STablemapHashPoint> PPair;
typedef std::map<uint32_t, STablemapHashPoint> PMap;
typedef PMap::iterator PMapI;
typedef PMap::const_iterator PMapCI;

struct STablemapHashValue 
{
	CString			name;
	uint32_t		hash;
};
typedef std::pair<uint32_t, STablemapHashValue> HPair;
typedef std::map<uint32_t, STablemapHashValue> HMap;
typedef HMap::iterator HMapI;
typedef HMap::const_iterator HMapCI;

struct STablemapImage
{
	CString			name;
	unsigned int	width;
	unsigned int	height;
	uint32_t		pixel[MAX_HASH_WIDTH*MAX_HASH_HEIGHT];
	RGBAImage		*image;
};
typedef std::pair<uint32_t, STablemapImage> IPair;
typedef std::map<uint32_t, STablemapImage> IMap;
typedef IMap::iterator IMapI;
typedef IMap::const_iterator IMapCI;

struct SWholeMap
{
	const ZMap	*z$;
	const SMap	*s$;
	const RMap	*r$;
	const TMap	*t$[k_max_number_of_font_groups_in_tablemap];
	const PMap	*p$[k_max_number_of_hash_groups_in_tablemap];
	const HMap	*h$[k_max_number_of_hash_groups_in_tablemap];
	const IMap	*i$;
	CString filepath;
	CString sitename;
	CString titletext;
};

class CTablemap 
{
	friend class CTablemapAccess;
public:
	// public functions
	CTablemap();
	~CTablemap();
	void ClearTablemap();
	int LoadTablemap(const CString _fname);
	int SaveTablemap(CArchive& ar, const char *version_text);
	int UpdateHashes(const HWND hwnd, const char *startup_path);
	uint32_t CTablemap::CalculateHashValue(IMapCI i_iter, const int type);
	CString CreateH$Index(const unsigned int number, const CString name);
	uint32_t CreateI$Index(const CString name, const int width, const int height, const uint32_t *pixels);

public:
	// public accessors
	const ZMap *z$() { return &_z$; }
	const SMap *s$() { return &_s$; }
	const TMap *t$(const int i) { if (i>=0 && i<k_max_number_of_font_groups_in_tablemap) return &_t$[i]; else return NULL; }
	const PMap *p$(const int i) { if (i>=0 && i<k_max_number_of_hash_groups_in_tablemap) return &_p$[i]; else return NULL; }
	const HMap *h$(const int i) { if (i>=0 && i<k_max_number_of_hash_groups_in_tablemap) return &_h$[i]; else return NULL; }
	const IMap *i$() { return &_i$; }
	// Ongoing work: Making all the iterators private and providing
	// accessor-functions in CTablemapAccess.
	const RMap *r$() { return &_r$; }
public:
  // For TM-verification
  bool ItemExists(CString name);
  bool FontGroupInUse(int font_index);
public:
	// commonly used strings 
	inline const int nchairs()		{ int n = GetTMSymbol("nchairs", 10);
									  return (n>=2 && n<=10) ? n : 10; }
	inline int LastChair()			{ return (nchairs() - 1); }
	const int swagtextmethod()		{ return GetTMSymbol("swagtextmethod", 0); }
	const int potmethod()			{ return GetTMSymbol("potmethod", 0); }
	const int activemethod()		{ return GetTMSymbol("activemethod", 0); }
	const int allinmethod()			{ return GetTMSymbol("allinmethod", 0); }
	const int swagselectionmethod() { return GetTMSymbol("swagselectionmethod", TEXTSEL_DOUBLECLICK); }
	const int swagdeletionmethod()	{ return GetTMSymbol("swagdeletionmethod", TEXTDEL_DELETE); }
	const int swagconfirmationmethod() { return GetTMSymbol("swagconfirmationmethod", BETCONF_ENTER); }
	const int buttonclickmethod()	{ return GetTMSymbol("buttonclickmethod", BUTTON_SINGLECLICK); }
	const int betpotmethod()		{ return GetTMSymbol("betpotmethod", BETPOT_DEFAULT); }
	const int HandNumberMinExpectedDigits()	{ return GetTMSymbol("handnumber_min_expected_digits", 0); }
	const int HandNumberMaxExpectedDigits() { return GetTMSymbol("handnumber_max_expected_digits", 0); }
	const bool balancenumbersonly()	{ return GetTMSymbol("balancenumbersonly", false); }
	const CString sitename()		{ return GetTMSymbol("sitename"); } 
	const CString titletext()		{ return GetTMSymbol("titletext"); } 
	const CString network()			{ return GetTMSymbol("network"); } 
	const CString chipscrapemethod(){ return GetTMSymbol("chipscrapemethod"); }
	const CString scraperdll()		{ return GetTMSymbol("scraperdll"); }
public:
	bool HandResetMethodDealer()	{ return (handresetmethod() &  HANDRESET_DEALER); }
	bool HandResetMethodCards()		{ return (handresetmethod() &  HANDRESET_CARDS); }
	bool HandResetMethodHandNumber(){ return (handresetmethod() &  HANDRESET_HANDNUM); }
public:
	const CString filename() { return _filename; }
	const CString filepath() { return _filepath; }
	const bool valid() { return _valid; }

public:
#define ENT CSLock lock(m_critsec);
	// public mutators 

	// These are used by OpenScrape
	void		h$_clear(const int i) { ENT if (i>=0 && i<k_max_number_of_hash_groups_in_tablemap) _h$[i].clear(); }
	void		p$_clear(const int i) { ENT if (i>=0 && i<k_max_number_of_hash_groups_in_tablemap) _p$[i].clear(); }

	const bool	z$_insert(const STablemapSize s) { ENT std::pair<ZMapI, bool> r=_z$.insert(ZPair(s.name, s)); return r.second;  }
	const bool	s$_insert(const STablemapSymbol s) { ENT std::pair<SMapI, bool> r=_s$.insert(SPair(s.name, s)); return r.second;  }
	const bool	r$_insert(const STablemapRegion s) { ENT std::pair<RMapI, bool> r=_r$.insert(RPair(s.name, s)); return r.second;  }
	const bool	t$_insert(const int i, const STablemapFont s) { ENT if (i>=0 && i<k_max_number_of_font_groups_in_tablemap) { std::pair<TMapI, bool> r=_t$[i].insert(TPair(s.hexmash, s)); return r.second; } else return false; }
	const bool	p$_insert(const int i, const STablemapHashPoint s) { ENT if (i>=0 && i<k_max_number_of_hash_groups_in_tablemap) { std::pair<PMapI, bool> r=_p$[i].insert(PPair(((s.x&0xffff)<<16) | (s.y&0xffff), s)); return r.second; } else return false; }
	const bool	h$_insert(const int i, const STablemapHashValue s) { ENT if (i>=0 && i<k_max_number_of_hash_groups_in_tablemap) { std::pair<HMapI, bool> r=_h$[i].insert(HPair(s.hash, s)); return r.second; } else return false; }
	const bool	i$_insert(const STablemapImage s);

	const size_t	z$_erase(CString s) { ENT std::map<int, int>::size_type c = _z$.erase(s); return c; }
	const size_t	s$_erase(CString s) { ENT std::map<int, int>::size_type c = _s$.erase(s); return c; }
	const size_t	r$_erase(CString s) { ENT std::map<int, int>::size_type c = _r$.erase(s); return c; }
	const size_t	t$_erase(const int i, CString s) { ENT if (i>=0 && i<k_max_number_of_font_groups_in_tablemap) { std::map<int, int>::size_type c = _t$[i].erase(s); return c; } else return 0; }
	const size_t	p$_erase(const int i, uint32_t u) { ENT if (i>=0 && i<k_max_number_of_hash_groups_in_tablemap) { std::map<int, int>::size_type c = _p$[i].erase(u); return c; } else return 0; }
	const size_t	h$_erase(const int i, uint32_t u) { ENT if (i>=0 && i<k_max_number_of_hash_groups_in_tablemap) { std::map<int, int>::size_type c = _h$[i].erase(u); return c; } else return 0; }
	const size_t	i$_erase(uint32_t u) { ENT std::map<int, int>::size_type c = _i$.erase(u); return c; }

	RMap *set_r$() { return &_r$; }

	void	set_network(const CString s) { ENT SMapI s_iter=_s$.find("network"); if (s_iter!=_s$.end()) s_iter->second.text=s; }
#undef ENT

private:
	// private functions
	void ClearIMap();
	void WriteSectionHeader(CArchive& ar, CString header);
	void WarnAboutGeneralTableMapError(int error_code, int line);
	int GetTMSymbol(CString name, int default);
	CString GetTMSymbol(CString name);
	const int handresetmethod()		{ return GetTMSymbol("handresetmethod", HANDRESET_ALL); }

private:
	// private variables - use public accessors and public mutators to address these
	bool		_valid;
	CString		_filename;
	CString		_filepath;
	ZMap		_z$; // indexed on name (as a CString)
	SMap		_s$; // indexed on name (as a CString)
	RMap		_r$; // indexed on name (as a CString)
	TMap		_t$[k_max_number_of_font_groups_in_tablemap]; // indexed on hexmash (as a CString)
	PMap		_p$[k_max_number_of_hash_groups_in_tablemap]; // indexed on "x<<16 | y" (as a uint32_t; x in high 16bits, y in low 16bits)
	HMap		_h$[k_max_number_of_hash_groups_in_tablemap]; // indexed on hash (as a uint32_t)
	IMap		_i$; // indexed on a uint32_t hash of: name+all pixels in RBGA hex format

private:
	// private functions and variables - not available via accessors or mutators
	CCritSec		m_critsec;

};

extern CTablemap *p_tablemap;

#endif //INC_CTABLEMAP_H