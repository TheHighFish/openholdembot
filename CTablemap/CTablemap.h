#ifndef INC_CTABLEMAP_H
#define INC_CTABLEMAP_H

#include <map>
#include "../CTransform/hash/stdint.h"
#include "../CTransform/pdiff/RGBAImage.h"
#include "../CCritSec/CCritSec.h"

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
	const CArray <STablemapFont, STablemapFont>			*t$;
	const CArray <STablemapHashPoint, STablemapHashPoint>	*p$;
	const HMap	*h$[4];
	const IMap	*i$;
	CString filepath;
};

extern class CTablemap 
{
public:
	// public functions
	CTablemap(void);
	~CTablemap(void);
	void ClearTablemap();
	int LoadTablemap(const char *_filename, const char *version, const bool check_ws_date, int *linenum, const bool disable_msgbox,
					 CString *loaded_version=NULL);
	int SaveTablemap(CArchive& ar, const char *version_text);
	int ConvertTablemap(const HWND hwnd=NULL, const char *startup_path="");
	int UpdateHashes(const HWND hwnd, const char *startup_path);
	void UpdateHexmashesHashes(const int group);
	CString CreateH$Index(const unsigned int number, const CString name);
	uint32_t CreateI$Index(const CString name, const int width, const int height, const uint32_t *pixels);


public:
	// public accessors
	const ZMap *z$() { return &_z$; }
	const SMap *s$() { return &_s$; }
	const RMap *r$() { return &_r$; }
	const CArray <STablemapFont, STablemapFont> * t$() { return &_t$; }
	const CArray <STablemapHashPoint, STablemapHashPoint> * p$() { return &_p$; }
	const HMap *h$(const int i) { if (i>=0 && i<=3) return &_h$[i]; else return NULL; }
	const IMap *i$() { return &_i$; }

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
	const int swagselectionmethod() { SMapCI it = _s$.find("swagselectionmethod");
									  if (it==_s$.end()) return TEXTSEL_DOUBLECLICK;
									  else if (it->second.text == "Sgl Click") return TEXTSEL_SINGLECLICK;
									  else if (it->second.text == "Dbl Click") return TEXTSEL_DOUBLECLICK;
									  else if (it->second.text == "Click Drag") return TEXTSEL_CLICKDRAG;
									  else return TEXTSEL_DOUBLECLICK; }
	const int swagdeletionmethod()	{ SMapCI it = _s$.find("swagdeletionmethod");
									  if (it==_s$.end()) return TEXTDEL_DELETE;
									  else if (it->second.text == "Delete") return TEXTDEL_DELETE;
									  else if (it->second.text == "Backspace") return TEXTDEL_BACKSPACE; 
									  else return TEXTDEL_DELETE; }
	const int swagconfirmationmethod() { SMapCI it = _s$.find("swagconfirmationmethod");
									  if (it==_s$.end()) return BETCONF_ENTER;
									  else if (it->second.text == "Enter") return BETCONF_ENTER;
									  else if (it->second.text == "Click Bet") return BETCONF_CLICKBET; 
									  else return BETCONF_ENTER; }
	const int buttonclickmethod()	{ SMapCI it = _s$.find("buttonclickmethod");
									  if (it==_s$.end()) return BUTTON_SINGLECLICK;
									  else if (it->second.text == "Single") return BUTTON_SINGLECLICK;
									  else if (it->second.text == "Double") return BUTTON_DOUBLECLICK; 
									  else return BUTTON_SINGLECLICK; }
	const int handresetmethod()		{ SMapCI it = _s$.find("handresetmethod"); 
									  if (it==_s$.end()) return 0x0111;
									  int n = strtoul(it->second.text.GetString(), NULL, 10); 
									  return (n>=1 && n<=0x0111) ? n : 0x0111; }

	const std::map<CString, int> * hexmashes(const int n) { return &(_hexmashes[n]); }
	const bool valid() { return _valid; }
	const CString filename() { return _filename; }
	const CString filepath() { return _filepath; }


public:
#define ENT CSLock lock(m_critsec);
	// public mutators 

	// These are used by OpenScrape
	void		h$_clear(const int i) { ENT if (i>=0 && i<=3) _h$[i].clear(); }

	const bool	z$_insert(const STablemapSize s) { ENT std::pair<ZMapI, bool> r=_z$.insert(ZPair(s.name, s)); return r.second;  }
	const bool	s$_insert(const STablemapSymbol s) { ENT std::pair<SMapI, bool> r=_s$.insert(SPair(s.name, s)); return r.second;  }
	const bool	r$_insert(const STablemapRegion s) { ENT std::pair<RMapI, bool> r=_r$.insert(RPair(s.name, s)); return r.second;  }
	const INT_PTR	set_t$_add(const STablemapFont s) { ENT return _t$.Add(s); }
	const INT_PTR	set_p$_add(const STablemapHashPoint s) { ENT return _p$.Add(s); }
	const bool	h$_insert(const int i, const STablemapHashValue s) { ENT if (i>=0 && i<=3) { std::pair<HMapI, bool> r=_h$[i].insert(HPair(s.hash, s)); return r.second; } else return false; }
	const bool	i$_insert(const STablemapImage s) { ENT std::pair<IMapI, bool> r=_i$.insert(IPair(CreateI$Index(s.name,s.width,s.height,s.pixel), s)); return r.second; }

	const size_t	z$_erase(CString s) { ENT std::map<int, int>::size_type c = _z$.erase(s); return c; }
	const size_t	s$_erase(CString s) { ENT std::map<int, int>::size_type c = _s$.erase(s); return c; }
	const size_t	r$_erase(CString s) { ENT std::map<int, int>::size_type c = _r$.erase(s); return c; }
	void	set_t$_removeat(const int n) { ENT if (n>=0 && n<=_t$.GetSize()) _t$.RemoveAt(n,1);  }
	void	set_p$_removeat(const int n) { ENT if (n>=0 && n<=_p$.GetSize()) _p$.RemoveAt(n,1);  }
	const size_t	h$_erase(const int i, uint32_t u) { ENT if (i>=0 && i<=3) { std::map<int, int>::size_type c = _h$[i].erase(u); return c; } else return 0; }
	const size_t	i$_erase(uint32_t u) { ENT std::map<int, int>::size_type c = _i$.erase(u); return c; }

	ZMap *set_z$() { return &_z$; }
	SMap *set_s$() { return &_s$; }
	RMap *set_r$() { return &_r$; }

	void	set_p$_removeall() { ENT _p$.RemoveAll();  }
	void	set_t$_insertat(const int n, const STablemapFont s) { ENT if (n>=0 && n<=_t$.GetSize()) _t$.InsertAt(n,s);  }

	void	set_network(const CString s) { ENT SMapI s_iter=_s$.find("network"); if (s_iter!=_s$.end()) s_iter->second.text=s; }
#undef ENT

private:
	// private variables - use public accessors and public mutators to address these
	bool		_valid;
	CString		_filename;
	CString		_filepath;
	ZMap		_z$; // indexed on name
	SMap		_s$; // indexed on name
	RMap		_r$; // indexed on name
	CArray <STablemapFont, STablemapFont>				_t$;
	CArray <STablemapHashPoint, STablemapHashPoint>		_p$;
	HMap		_h$[4]; // indexed on hash
	IMap		_i$; // indexed on a hash of: name+all pixels in RBGA hex format

	std::map<CString, int>	_hexmashes[4]; // Font _hexmashes for fast lookups - font type 0-3 are indexed in the array

private:
	// private functions and variables - not available via accessors or mutators
	CCritSec		m_critsec;

} *p_tablemap;

#endif //INC_CTABLEMAP_H