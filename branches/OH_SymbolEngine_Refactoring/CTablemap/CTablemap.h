#ifndef INC_CTABLEMAP_H
#define INC_CTABLEMAP_H

#include <map>
#include "../CTransform/hash/stdint.h"
#include "../CTransform/pdiff/RGBAImage.h"
#include "../CCritSec/CCritSec.h"
#include "../OpenHoldem/MagicNumbers.h"

#define _0b0111 0x07

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
};

extern class CTablemap 
{
	friend class CTablemapAccess;
public:
	// public functions
	CTablemap(void);
	~CTablemap(void);
	void ClearTablemap();
	int LoadTablemap(const char *_filename, const char *version, int *linenum, 
					 CString *loaded_version=NULL);
	int SaveTablemap(CArchive& ar, const char *version_text);
	int ConvertTablemap(const HWND hwnd=NULL, const char *startup_path="");
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
	/* commonly used strings */
	const int nchairs()				{ SMapCI it = _s$.find("nchairs"); 
									  if (it==_s$.end()) return 10;
									  int n = strtoul(it->second.text.GetString(), NULL, 10); 
									  return (n>=2 && n<=10) ? n : 10; }
	const int swagtextmethod()		{ SMapCI it = _s$.find("swagtextmethod"); if (it!=_s$.end()) return strtoul(it->second.text.GetString(), NULL, 10); else return 0; }
	const int potmethod()			{ SMapCI it = _s$.find("potmethod"); if (it!=_s$.end()) return strtoul(it->second.text.GetString(), NULL, 10); else return 0; }
	const int activemethod()		{ SMapCI it = _s$.find("activemethod"); if (it!=_s$.end()) return strtoul(it->second.text.GetString(), NULL, 10); else return 0; }
	const CString sitename()		{ SMapCI it = _s$.find("sitename"); if (it!=_s$.end()) return it->second.text; else return ""; }
	const CString network()			{ SMapCI it = _s$.find("network"); if (it!=_s$.end()) return it->second.text; else return ""; }

	const int allinmethod()			{ SMapCI it = _s$.find("allinmethod"); if (it!=_s$.end()) return strtoul(it->second.text.GetString(), NULL, 10); else return 0; } 

	const int swagselectionmethod() { SMapCI it = _s$.find("swagselectionmethod");
									  if (it==_s$.end()) return TEXTSEL_DOUBLECLICK;
									  CString s=it->second.text;
									  if (s.MakeLower() == "sgl click") return TEXTSEL_SINGLECLICK;
									  else if (s.MakeLower() == "dbl click") return TEXTSEL_DOUBLECLICK;
									  else if (s.MakeLower() == "click drag") return TEXTSEL_CLICKDRAG;
									  else if (s.MakeLower() == "nothing") return TEXTSEL_NOTHING;
									  else return TEXTSEL_DOUBLECLICK; }

	const int swagdeletionmethod()	{ SMapCI it = _s$.find("swagdeletionmethod");
									  if (it==_s$.end()) return TEXTDEL_DELETE;
									  CString s=it->second.text;
									  if (s.MakeLower() == "delete") return TEXTDEL_DELETE;
									  else if (s.MakeLower() == "backspace") return TEXTDEL_BACKSPACE; 
									  else if (s.MakeLower() == "nothing") return TEXTDEL_NOTHING;
									  else return TEXTDEL_DELETE; }

	const int swagconfirmationmethod() { SMapCI it = _s$.find("swagconfirmationmethod");
									  if (it==_s$.end()) return BETCONF_ENTER;
									  CString s=it->second.text;
									  if (s.MakeLower() == "enter") return BETCONF_ENTER;
									  else if (s.MakeLower() == "click bet") return BETCONF_CLICKBET; 
									  else if (s.MakeLower() == "nothing") return BETCONF_NOTHING;
									  else return BETCONF_ENTER; }

	const int buttonclickmethod()	{ SMapCI it = _s$.find("buttonclickmethod");
									  if (it==_s$.end()) return BUTTON_SINGLECLICK;
									  CString s=it->second.text;
									  if (s.MakeLower() == "single") return BUTTON_SINGLECLICK;
									  else if (s.MakeLower() == "double") return BUTTON_DOUBLECLICK; 
									  else return BUTTON_SINGLECLICK; }

	const int betpotmethod()		{ SMapCI it = _s$.find("betpotmethod");
									  if (it==_s$.end()) return BETPOT_DEFAULT;
									  CString s=it->second.text;
									  if (s.MakeLower() == "raise") return BETPOT_RAISE; 
									  else return BETPOT_DEFAULT; }

	const int handresetmethod()		{ SMapCI it = _s$.find("handresetmethod"); 
									  if (it==_s$.end()) return _0b0111;
									  int n = strtoul(it->second.text.GetString(), NULL, 10); 
									  return (n>=1 && n<=_0b0111) ? n : _0b0111; }

	const int HandNumberMinExpectedDigits() 
	{ 
		SMapCI it = _s$.find("handnumber_min_expected_digits"); 
		if (it!=_s$.end()) 
			return strtoul(it->second.text.GetString(), NULL, 10); 
		else 
			return 0; 
	} 

	const int HandNumberMaxExpectedDigits() 
	{ 
		SMapCI it = _s$.find("handnumber_max_expected_digits"); 
		if (it!=_s$.end()) 
			return strtoul(it->second.text.GetString(), NULL, 10); 
		else 
			return 0; 
	} 

	const bool balancenumbersonly()	{ SMapCI it = _s$.find("balancenumbersonly"); 
									  if (it==_s$.end()) return false;
									  CString s=it->second.text;
									  if (s.MakeLower() == "true") return true;
									  else if (s.MakeLower() == "yes") return true; 
									  else return false; }

	const CString chipscrapemethod(){ SMapCI it = _s$.find("chipscrapemethod"); 
									  if (it==_s$.end()) return "";
									  else return it->second.text; }

	const CString scraperdll()		{ SMapCI it = _s$.find("scraperdll"); if (it!=_s$.end()) return it->second.text; else return ""; }

	const CString scraperpreprocessor_dll()	{ SMapCI it = _s$.find("scraperpreprocessordll"); if (it!=_s$.end()) return it->second.text; else return ""; }

	const bool valid() { return _valid; }
	const CString filename() { return _filename; }
	const CString filepath() { return _filepath; }


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
	const bool	i$_insert(const STablemapImage s) { ENT std::pair<IMapI, bool> r=_i$.insert(IPair(CreateI$Index(s.name,s.width,s.height,s.pixel), s)); return r.second; }

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

} *p_tablemap;

#endif //INC_CTABLEMAP_H