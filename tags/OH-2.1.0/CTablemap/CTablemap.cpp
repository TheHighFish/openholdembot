#include "StdAfx.h"
#include <Math.h>

#include "CTablemap.h"
#include "../CTransform/CTransform.h"

#ifdef OPENHOLDEM_PROGRAM
#include "../OpenHoldem/debug.h"
#endif

#ifdef OPENSCRAPE_PROGRAM
#include "../OpenScrape/debug.h"
#endif

CTablemap			*p_tablemap = NULL;

CTablemap::CTablemap(void)
{
    ClearTablemap();
}

CTablemap::~CTablemap(void)
{
	ClearTablemap();
}

void CTablemap::ClearIMap()
{
	IMap::iterator iter;
	// For each image in the map: delete.
	for(iter = _i$.begin(); iter != _i$.end(); iter++ ) 
	{
		// First value in the map is some index, internally created in i$_insert()
		// Second value in the map is our image.
		iter->second.image->~RGBAImage();
	}
	// Remove the contents of the map.
	_i$.clear();
}

void CTablemap::ClearTablemap()
{
	CSLock lock(m_critsec);

	_valid = false;
	_filepath = "";
	_filename = "";

	_z$.clear();
	_s$.clear();
	_r$.clear();

	for (int i=0; i<=3; i++)
	{
		_t$[i].clear();
		_p$[i].clear();
		_h$[i].clear();
	}

	// Clear _i$-map and its contents
	ClearIMap();
}

int CTablemap::LoadTablemap(const char *_filename, const char *version, const bool check_ws_date, int *linenum, const bool disable_msgbox,
							CString *loaded_version) 
{
	CString				strLine = "", strLineType = "", token = "", s = "", e = "", hexval = "", t = "";
	int					pos = 0, x = 0, y = 0;
	bool				supported_version = false;

	// temp
	STablemapSize			hold_size;
	STablemapSymbol			hold_symbol;
	STablemapRegion			hold_region;
	STablemapFont			hold_font;
	STablemapHashPoint		hold_hash_point;
	STablemapHashValue		hold_hash_value;
	STablemapImage			hold_image;

	// Clean up the global.profile structure
	ClearTablemap();

	CSLock lock(m_critsec);
	
	// Open the selected file
	CFile cfFile(_filename, CFile::modeRead | CFile::shareDenyNone);
	_filepath = _filename;

	// Load its contents into a CArchive
	CArchive ar (&cfFile, CArchive::load);

	// Read the first line of the CArchive into strLine
	strLine = "";
	*linenum = 1;

	// Failed, so quit
	if (!ar.ReadString(strLine))
		return ERR_EOF;

	// skip any blank lines
	while (strLine.GetLength() == 0) 
	{
		(*linenum)++;

		// Failed, so quit
		if (!ar.ReadString(strLine)) 
			return ERR_EOF;
	}

	
	//
	// Validate file version, if passed in
	// check the ".wsdb1"/".ohdb1"/".ohdb2"/".osdb1"/".osdb2" line
	if (strlen(version)) 
	{
		if (memcmp(version, VER_OPENSCRAPE_1, strlen(version)) == 0 ||
			memcmp(version, VER_OPENSCRAPE_2, strlen(version)) == 0)
		{
			if (memcmp(strLine.GetString(), VER_OPENHOLDEM_2, strlen(VER_OPENHOLDEM_2)) != 0 &&
				memcmp(strLine.GetString(), VER_OPENSCRAPE_1, strlen(VER_OPENSCRAPE_1)) != 0 &&
				memcmp(strLine.GetString(), VER_OPENSCRAPE_2, strlen(VER_OPENSCRAPE_2)) != 0)
			{
				return ERR_VERSION;
			}
		}
		else
		{
			if (memcmp(strLine.GetString(), version, strlen(version)) != 0)
			{
				return ERR_VERSION;
			}
		}
	}

	// Save the version of the file we are loading
	if (loaded_version != NULL)
	{
		if (memcmp(strLine.GetString(), VER_WINSCRAPE, strlen(VER_WINSCRAPE)) == 0)
			loaded_version->Format("%s", VER_WINSCRAPE);

		else if (memcmp(strLine.GetString(), VER_OPENHOLDEM_1, strlen(VER_OPENHOLDEM_1)) == 0)
			loaded_version->Format("%s", VER_OPENHOLDEM_1);

		else if (memcmp(strLine.GetString(), VER_OPENHOLDEM_2, strlen(VER_OPENHOLDEM_2)) == 0)
			loaded_version->Format("%s", VER_OPENHOLDEM_2);

		else if (memcmp(strLine.GetString(), VER_OPENSCRAPE_1, strlen(VER_OPENSCRAPE_1)) == 0)
			loaded_version->Format("%s", VER_OPENSCRAPE_1);

		else if (memcmp(strLine.GetString(), VER_OPENSCRAPE_2, strlen(VER_OPENSCRAPE_2)) == 0)
			loaded_version->Format("%s", VER_OPENSCRAPE_2);
	}


	// check the date line
	if (check_ws_date) 
	{
		supported_version = false;
		do 
		{
			if (strLine.Find(VER_WINSCRAPE_DATE1) != -1 ||
				strLine.Find(VER_WINSCRAPE_DATE2) != -1 ||
				strLine.Find(VER_WINSCRAPE_DATE3) != -1 ||
				strLine.Find(VER_WINSCRAPE_DATE4) != -1) 
			{
				supported_version = true;
			}
		}
		while (ar.ReadString(strLine) && !supported_version);

		if (!supported_version) 
			return ERR_VERSION;
	}

	// Repeat while there are lines in the file left to process
	do {
		// Trim the line
		strLine.Trim();

		// If the line is empty, skip it
		if (strLine.GetLength() == 0)
			continue;

		// Extract the line type
		pos=0;
		strLineType = strLine.Tokenize(" \t", pos);

		// Skip comment lines
		if (strLineType.Left(2) == "//")
			continue;

		// Skip version lines
		if (strLineType == VER_WINSCRAPE ||
				strLineType == VER_OPENHOLDEM_1 ||
				strLineType == VER_OPENHOLDEM_2 ||
				strLineType == VER_OPENSCRAPE_1 ||
				strLineType == VER_OPENSCRAPE_2) 
		{
			continue;
		}

		// Handle z$ lines (sizes)
		if (strLineType.Left(2) == "z$") 
		{
			// name
			hold_size.name = strLineType.Mid(2);

			// width
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_size.width = atol(token.GetString());

			// height
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_size.height = atol(token.GetString());

			if (!z$_insert(hold_size))
			{
				if (!disable_msgbox)
				{
					ZMapCI z_iter = _z$.find(hold_size.name);

					if (z_iter != _z$.end())
					{
						t.Format("'%s' skipped, as this size record already exists.", strLine);
						MessageBox(NULL, t.GetString(), "ERROR adding size record", MB_OK | MB_TOPMOST);			
					}
					else
					{
						MessageBox(NULL, strLine, "ERROR adding size record", MB_OK | MB_TOPMOST);			
					}
				}
			}
		}

		// Handle s$ lines (symbols/string)
		else if (strLineType.Left(2) == "s$") 
		{
			// name
			hold_symbol.name = strLineType.Mid(2);

			// text
			hold_symbol.text = strLine.Mid(strLineType.GetLength());
			hold_symbol.text.Trim();
			if (hold_symbol.text.GetLength()==0)
				return ERR_SYNTAX;

			// Skip _s$hXtype lines
			if (strLineType.Left(3) == "s$h" && strLineType.Mid(4,4) == "type")
			{
			}
			else
			{
				// Assure, sitename and network are lowercases.
				if ((strLineType.Left(10) == "s$sitename") || (strLineType.Left(9) == "s$network"))
				{
					CString temp = hold_symbol.text.MakeLower();
					hold_symbol.text = temp;
				}

				if (!s$_insert(hold_symbol))
				{
					if (!disable_msgbox)
					{
						SMapCI s_iter = _s$.find(hold_symbol.name);
						if (s_iter != _s$.end())
						{
							t.Format("'%s' skipped, as this string/symbol record already exists.", strLine);
							MessageBox(NULL, t.GetString(), "ERROR adding string/symbol record", MB_OK | MB_TOPMOST);			
						}
						else
						{
							MessageBox(NULL, strLine, "ERROR adding string/symbol record", MB_OK | MB_TOPMOST);			
						}
					}
				}
			}
		}

		// Handle r$ lines (regions)
		else if (strLineType.Left(2) == "r$") 
		{
			// name
			hold_region.name = strLineType.Mid(2);

			// left
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_region.left = atol(token.GetString());

			// top
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_region.top = atol(token.GetString());

			// right
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_region.right = atol(token.GetString());

			// bottom
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_region.bottom = atol(token.GetString());

			// color
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_region.color = strtoul(token.GetString(), 0, 16);

			// radius
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_region.radius = atol(token.GetString());

			// transform
			hold_region.transform = strLine.Tokenize(" \t", pos);
			if (hold_region.transform.GetLength()==0)
				return ERR_SYNTAX;

			// flags
			//token = strLine.Tokenize(" \t", pos);
			//if (token.GetLength()==0) { return ERR_SYNTAX; }
			//hold_region.flags = atol(token.GetString());

			if (!r$_insert(hold_region))
			{
				if (!disable_msgbox)
				{
					RMapCI r_iter = _r$.find(hold_region.name);
					if (r_iter != _r$.end())
					{
						t.Format("'%s' skipped, as this region record already exists.", strLine);
						MessageBox(NULL, t.GetString(), "ERROR adding region record", MB_OK | MB_TOPMOST);			
					}
					else
					{
						MessageBox(NULL, strLine, "ERROR adding region record", MB_OK | MB_TOPMOST);			
					}
				}
			}
		}

		// Handle t$ lines (fonts)
		else if (strLineType.Left(2) == "t$" ||
				 (strLineType.Left(1) == 't' &&
				  strLineType.Mid(1,1) >= "0" &&
				  strLineType.Mid(1,1) <= "3" &&
				  strLineType.Mid(2,1) == "$")) 
		{

			int font_group = 0;

			// Old style t$ records (no font groups) - pre "2007 Nov 1 08:32:55" WinScrape release
			if (strLineType.Left(2) == "t$") 
			{
				t = strLineType.Mid(2,1);
				hold_font.ch = t.GetString()[0];
				font_group = 0;
			}
			// New style t$ records (font groups 0-3) - "2007 Nov 1 08:32:55" WinScrape release and later & OpenScrape
			else 
			{
				t = strLineType.Mid(3,1);
				hold_font.ch = t.GetString()[0];
				font_group = strLineType.GetString()[1] - '0';
			}

			if (font_group<0 || font_group>3)
			{
				if (!disable_msgbox)
					MessageBox(NULL, strLine, "Invalid font group", MB_OK | MB_TOPMOST);

				return ERR_SYNTAX;
			}

			int i = 0;
			hold_font.hexmash = "";
			while ((token = strLine.Tokenize(" \t", pos))!="" && i<=30 && pos!=-1) 
			{
				hold_font.x[i++] = strtoul(token, 0, 16);
				hold_font.hexmash.Append(token);
			}
			hold_font.x_count = i;

			// Add the new t$ record to the internal array
			if (!t$_insert(font_group, hold_font))
			{
				if (!disable_msgbox)
				{
					TMapCI t_iter = _t$[font_group].find(hold_font.hexmash);
					if (t_iter != _t$[font_group].end())
					{
						t.Format("'%s' skipped, as this character already exists in group %d as '%c'.",
							strLine, font_group, t_iter->second.ch);
						MessageBox(NULL, t.GetString(), "ERROR adding font record", MB_OK | MB_TOPMOST);			
					}
					else
					{
						MessageBox(NULL, strLine, "ERROR adding font record", MB_OK | MB_TOPMOST);			
					}
				}
			}
		}

		// Handle p$ lines (hash points)
		else if (strLineType.Left(1) == "p" &&
				 strLineType.Mid(1,1) >= "0" &&
				 strLineType.Mid(1,1) <= "3" &&
				 strLineType.Mid(2,1) == "$") 
		{
			// number
			token = strLineType.Mid(1,1);
			int hashpoint_group = atol(token);
			if (hashpoint_group<0 || hashpoint_group>3)
			{
				if (!disable_msgbox)
					MessageBox(NULL, strLine, "Invalid hash point group", MB_OK | MB_TOPMOST);
	
				return ERR_SYNTAX;
			}

			// x
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_hash_point.x = atol(token.GetString());

			// y
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			token = strLine.Tokenize(" \t", pos);
			hold_hash_point.y = atol(token.GetString());

			// Add the new p$ record to the internal array
			if (!p$_insert(hashpoint_group, hold_hash_point))
			{
				if (!disable_msgbox)
				{
					PMapCI p_iter = _p$[hashpoint_group].find(((hold_hash_point.x&0xffff)<<16) | (hold_hash_point.y&0xffff));
					if (p_iter != _p$[hashpoint_group].end())
					{
						t.Format("'%s' skipped, as hash point (%d, %d) already exists in group %d.", 
							strLine, hold_hash_point.x, hold_hash_point.y, hashpoint_group);
						MessageBox(NULL, t.GetString(), "ERROR adding hash point record", MB_OK | MB_TOPMOST);			
					}
					else
					{
						MessageBox(NULL, strLine, "ERROR adding hash point record", MB_OK | MB_TOPMOST);			
					}
				}
			}
		}

		// Handle h$ lines (hash values)
		else if (strLineType.Left(1) == "h" &&
				 strLineType.Mid(1,1) >= "0" &&
				 strLineType.Mid(1,1) <= "3" &&
				 strLineType.Mid(2,1) == "$") 
		{
			// number
			int hash_group = strLineType.GetString()[1] - '0';
			if (hash_group<0 || hash_group>3)
			{
				if (!disable_msgbox)
					MessageBox(NULL, strLine, "Invalid hash group", MB_OK | MB_TOPMOST);
	
				return ERR_SYNTAX;
			}

			// name
			hold_hash_value.name = strLineType.Mid(3);

			// value
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_hash_value.hash = strtoul(token.GetString(), 0, 16);

			// Add the new h$ record to the internal array
			if (!h$_insert(hash_group, hold_hash_value))
			{
				if (!disable_msgbox)
				{
					HMapCI h_iter = _h$[hash_group].find(hold_hash_value.hash);
					if (h_iter != _h$[hash_group].end())
					{
						t.Format("'%s' skipped, as hash %08x already exists in group %d.", 
							strLine, hold_hash_value.hash, hash_group);
						MessageBox(NULL, t.GetString(), "ERROR adding hash record", MB_OK | MB_TOPMOST);			
					}
					else
					{
						MessageBox(NULL, strLine, "ERROR adding hash record", MB_OK | MB_TOPMOST);			
					}
				}
			}
		}

		// Handle i$ lines (images)
		else if (strLineType.Left(2) == "i$") 
		{
			// name
			hold_image.name = strLineType.Mid(2);

			// width
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_image.width = atol(token.GetString());

			// height
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_image.height = atol(token.GetString());

			// Check size of region
			if (hold_image.width * hold_image.height > MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT)
				return ERR_REGION_SIZE;

			// Allocate space for "RGBAImage"
			t = hold_image.name + ".ppm";
			hold_image.image = new RGBAImage(hold_image.width, hold_image.height, t.GetString());

			// read next "height" lines
			for (y=0; y < (int) hold_image.height; y++) 
			{
				(*linenum)++;
				if (!ar.ReadString(strLine)) 
				{
					return ERR_SYNTAX;
				}
				// scan across "width" of line to get values
				for (x=0; x < (int) hold_image.width; x++) 
				{
					// unreverse bgra to abgr
					hexval = strLine.Mid(x*8+6, 2) + strLine.Mid(x*8, 6);
					hold_image.pixel[y*hold_image.width + x] = strtoul(hexval, 0, 16);
					BYTE alpha = (hold_image.pixel[y*hold_image.width + x] >> 24) &0xff;
					BYTE blue =  (hold_image.pixel[y*hold_image.width + x] >> 16) &0xff;
					BYTE green = (hold_image.pixel[y*hold_image.width + x] >>  8) &0xff;
					BYTE red =   (hold_image.pixel[y*hold_image.width + x] >>  0) &0xff;
					hold_image.image->Set(red, green, blue, alpha, y*hold_image.width + x);
				}
			}

			// Add the new i$ record to the internal array
			if (!i$_insert(hold_image))
			{
				if (!disable_msgbox)
				{
					IMapCI i_iter = _i$.find(CreateI$Index(hold_image.name, hold_image.width, hold_image.height, hold_image.pixel));
					if (i_iter != _i$.end())
					{
						t.Format("'%s' skipped, as image already exists as '%s', with identical width, height and pixels.", 
							strLineType, i_iter->second.name);
						MessageBox(NULL, t.GetString(), "ERROR adding image record", MB_OK | MB_TOPMOST);			
					}
					else
					{
						MessageBox(NULL, strLine, "ERROR adding image record", MB_OK | MB_TOPMOST);			
					}
				}
			}
			// Delete images later, when they are no longer needed,
			// probably best in the destructor, otherwise we get a memory-leak.
			// We can't delete them here, as we added a referece some line above: i$_insert(hold_image).
			// hold_image.image->~RGBAImage();
		}

		// Unknown line type
		else 
		{
			if (!disable_msgbox)
				MessageBox(NULL, strLine, "ERROR Unknown Line Type", MB_OK | MB_TOPMOST);
	
			return ERR_UNK_LN_TYPE;
		}

		(*linenum)++;
	}
	while (ar.ReadString(strLine));

	_valid = true;

	return SUCCESS;
}

int CTablemap::SaveTablemap(CArchive& ar, const char *version_text)
{
	CString		s = "", t = "", text = "";
	char		nowtime[26] = {0};

	// Version
	s.Format("%s\r\n", VER_OPENSCRAPE_2); ar.WriteString(s);
	ar.WriteString("\r\n");

	// Comment header
	s.Format("// OpenScrape %s\r\n", version_text); ar.WriteString(s);
	ar.WriteString("\r\n");

	// Date/Time
	s.Format("// %s\r\n", get_time(nowtime)); ar.WriteString(s);
	ar.WriteString("// 32 bits per pixel\r\n");
	ar.WriteString("\r\n");

	// sizes
	ar.WriteString("//\r\n");
	ar.WriteString("// sizes\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	
	for (ZMapCI z_iter=_z$.begin(); z_iter!=_z$.end(); z_iter++)
	{
		s.Format("z$%-16s %d  %d\r\n", z_iter->second.name, z_iter->second.width, z_iter->second.height); 
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// strings
	ar.WriteString("//\r\n");
	ar.WriteString("// strings\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");

	for (SMapCI s_iter=_s$.begin(); s_iter!=_s$.end(); s_iter++)
	{
		s.Format("s$%-25s %s\r\n", s_iter->second.name.GetString(), s_iter->second.text.GetString());
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// regions
	ar.WriteString("//\r\n");
	ar.WriteString("// regions\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	for (RMapCI r_iter=_r$.begin(); r_iter!=_r$.end(); r_iter++)
	{
		s.Format("r$%-18s %3d %3d %3d %3d %8x %4d %s\r\n", r_iter->second.name.GetString(),
			r_iter->second.left, r_iter->second.top, r_iter->second.right, r_iter->second.bottom,
			r_iter->second.color, r_iter->second.radius, r_iter->second.transform);
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// fonts
	ar.WriteString("//\r\n");
	ar.WriteString("// fonts\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");

	for (int i=0; i<=3; i++)
	{
		for (TMapCI t_iter =_t$[i].begin(); t_iter != _t$[i].end(); t_iter++)
		{
			s.Format("t%d$%c", i, t_iter->second.ch);
			for (int j=0; j<t_iter->second.x_count; j++) 
			{ 
				t.Format(" %x", t_iter->second.x[j]);
				s.Append(t);
			}
			s.Append("\r\n");
			ar.WriteString(s);
		}
	}
	ar.WriteString("\r\n");

	// points
	ar.WriteString("//\r\n");
	ar.WriteString("// points\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");

	for (int i=0; i<=3; i++)
	{
		for (PMapCI p_iter=_p$[i].begin(); p_iter!=_p$[i].end(); p_iter++)
		{
			s.Format("p%d$%4d %4d\r\n", i, p_iter->second.x, p_iter->second.y);  
			ar.WriteString(s);
		}
	}
	ar.WriteString("\r\n");

	// hash
	ar.WriteString("//\r\n");
	ar.WriteString("// hash\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");

	for (int i=0; i<=3; i++)
	{
		for (HMapCI h_iter=_h$[i].begin(); h_iter!=_h$[i].end(); h_iter++)
		{
			s.Format("h%d$%-18s %08x\r\n", i, h_iter->second.name.GetString(), h_iter->second.hash);
			ar.WriteString(s);
		}
	}
	ar.WriteString("\r\n");

	// images
	ar.WriteString("//\r\n");
	ar.WriteString("// images\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	for (IMapCI i_iter=_i$.begin(); i_iter!=_i$.end(); i_iter++)
	{
		int width = i_iter->second.width;
		int height = i_iter->second.height;

		s.Format("i$%-16s %-3d %-3d\r\n", i_iter->second.name, width, height);
		ar.WriteString(s);
		for (int y=0; y<height; y++)
		{
			s = "";
			for (int x=0; x<width; x++)
			{
				text.Format("%08x", i_iter->second.pixel[y*width + x]);
				s.Append(text.Mid(2, 6));
				s.Append(text.Mid(0,2));
			}
			s.Append("\r\n");
			ar.WriteString(s);
		}
	}
	ar.WriteString("\r\n");

	return SUCCESS;	
}


int CTablemap::ConvertTablemap(const HWND hwnd, const char *startup_path) 
{
	int						i = 0, j = 0;
	CString					e = "", s = "";
	uint32_t				pixels[200*150] = {0};
	int						pixcount = 0;
	bool					all_i$_found = false, this_i$_found = false;
	FILE					*fp = NULL;
	char					timebuf[30] = {0};
	CString					logpath = "";
	uint32_t				hash_seed = 0;
	STablemapHashValue		hold_hash_value;
	STablemapHashValue		h$_record;
	CArray <STablemapHashValue, STablemapHashValue> new_hashes[4];
	CArray <STablemapHashValue, STablemapHashValue>	unmatched_h$_records[4];

	// Get number of records of each type in this table map
	if (_i$.begin()==_i$.end()) 
	{
		MessageBox(hwnd, "No i$ records found - is this a master?", "'Profile' Error", MB_OK);
		return ERR_NOTMASTER;
	}

	// Loop through all the region records, and invert the colors to align with
	// Windows' COLORREF  ARGB->ABGR
	for (RMapI r_iter=_r$.begin(); r_iter!=_r$.end(); r_iter++) 
	{
		r_iter->second.color = 
			(r_iter->second.color & 0xff000000) +
			((r_iter->second.color & 0x00ff0000)>>16) +
			((r_iter->second.color & 0x0000ff00)) +
			((r_iter->second.color & 0x000000ff)<<16);
	}

	// Loop through all the hash (h$) records, and check for a corresponding image (i$) record
	// Log missing records, display message and error out if we can't find them all
	all_i$_found = true;
	for (i=0; i<=3; j++)
		unmatched_h$_records[j].RemoveAll();

	for (i=0; i<=3; j++)
	{
		for (HMapCI h_iter=_h$[i].begin(); h_iter!=_h$[i].end(); h_iter++)
		{
			this_i$_found = false;
			for (IMapCI i_iter=_i$.begin(); i_iter!=_i$.end(); i_iter++)
			{
				if (i_iter->second.name == h_iter->second.name) 
					this_i$_found = true;
			}

			if (!this_i$_found) 
			{
				all_i$_found = false;
				h$_record.name = h_iter->second.name;
				h$_record.hash = h_iter->second.hash;
				unmatched_h$_records[i].Add(h$_record);
			}

		}
	}

	if (!all_i$_found) 
	{
		logpath.Format("%s\\tablemap conversion log.txt", startup_path);

		if (fopen_s(&fp, logpath.GetString(), "a")==0)
		{
			get_now_time(timebuf);
			fprintf(fp, "<%s>\nConverting from: %s\n", timebuf, _filepath);
			fprintf(fp, "h$ records with no matching i$ record:\n");

			for (i=0; i<=3; i++)
				for (j=0; j<unmatched_h$_records[i].GetCount(); j++)
					fprintf(fp, "\t%3d. h$%s\n", j+1, unmatched_h$_records[i].GetAt(j).name.GetString());

			fprintf(fp, "=======================================================\n\n");
			fclose(fp);
		}

		MessageBox(hwnd, 
				   "Could not complete conversion of this table map, due to missing Image\n"\
				   "records. Please see the \"tablemap conversion log.txt\" file for details.\n\n"\
				   "The Image records will need to be recreated and the Hash records\n"\
				   "will need to be updated (Edit/Update Hashes) before this table map\n"\
				   "can be used in OpenHoldem.", 
				   "Conversion Error", MB_OK);

		return ERR_INCOMPLETEMASTER;
	}

	// Prepare CArray that holds temporary hash records
	for (i=0; i<=3; i++)
		new_hashes[i].RemoveAll();

	// Loop through each of the image records and create hashes
	for (IMapCI i_iter=_i$.begin(); i_iter!=_i$.end(); i_iter++) 
	{
		// Loop through the h$ records to find all the hashes that we have to create for this image record
		for (i=0; i<=3; i++)
		{
			for (HMapCI h_iter=_h$[i].begin(); h_iter!=_h$[i].end(); h_iter++) 
			{
				if (i_iter->second.name == h_iter->second.name) 
				{
					// create hash, based on type
					hold_hash_value.name = i_iter->second.name;

					// Get count of pixels (p$ records)
					pixcount = 0;
					for (PMapCI p_iter=_p$[i].begin(); p_iter!=_p$[i].end(); p_iter++) 
					{
						if (p_iter->second.x <= i_iter->second.width &&
							p_iter->second.y <= i_iter->second.height) 
						{
								pixels[pixcount++] = i_iter->second.pixel[(p_iter->second.y * i_iter->second.width) + p_iter->second.x];				
						}
					}

					hash_seed = i==0 ? HASH_SEED_0 :
								i==1 ? HASH_SEED_1 :
								i==2 ? HASH_SEED_2 :
								i==3 ? HASH_SEED_3 : 0;

					// Create hash
					if (pixcount==0) 
						hold_hash_value.hash = hashword(&i_iter->second.pixel[0], i_iter->second.width * i_iter->second.height, hash_seed);
					else 
						hold_hash_value.hash = hashword(&pixels[0], pixcount, hash_seed);

					// Check for hash collision
					for (j=0; j<(int) new_hashes[i].GetSize(); j++) 
					{
						if (hold_hash_value.hash == new_hashes[i].GetAt(j).hash) 
						{
							e.Format("Hash collision:\n	<%s> and <%s>\n\nTalk to an OpenHoldem developer, please.\n%s %08x %08x", 
								hold_hash_value.name.GetString(), 
								new_hashes[i].GetAt(j).name.GetString(),
								hold_hash_value.name,
								hold_hash_value.hash, new_hashes[i].GetAt(j).hash
								); 
							MessageBox(hwnd, e, "Hash collision", MB_OK);
							return ERR_HASH_COLL;
						}
					}

					// Store this new hash in the temporary holding CArray
					new_hashes[i].Add(hold_hash_value);
				}
			}  // for (HMapCI h_iter=_h$.begin(); h_iter!=_h$.end(); h_iter++) 
		} // for (i=0; i<=3; i++)
	} // for (IMapCI i_iter=_i$.begin(); i_iter!=_i$.end(); i_iter++) 


	// Copy the temporary new hash CArray to the internal std::map
	for (i=0; i<=3; i++)
		h$_clear(i);

	for (i=0; i<=3; i++)
	{
		for (j=0; j<(int) new_hashes[i].GetSize(); j++)
		{
			if (!h$_insert(i, new_hashes[i].GetAt(j)))
			{
				e.Format("Hash record: %d %s %08x", i, new_hashes[i].GetAt(j).name, new_hashes[i].GetAt(j).hash);
				MessageBox(NULL, e, "ERROR adding hash value record: " + new_hashes[i].GetAt(j).name, MB_OK | MB_TOPMOST);	
			}
		}
	}

	return SUCCESS;
}

int CTablemap::UpdateHashes(const HWND hwnd, const char *startup_path)
{
	int						i = 0, j = 0;
	CString					e = "", s = "";
	uint32_t				pixels[MAX_HASH_WIDTH*MAX_HASH_HEIGHT] = {0}, filtered_pix[MAX_HASH_WIDTH*MAX_HASH_HEIGHT] = {0};
	int						pixcount = 0;
	bool					all_i$_found = false, this_i$_found = false;
	FILE					*fp = NULL;
	char					timebuf[30] = {0};
	CString					logpath = "";
	STablemapHashValue		hold_hash_value;
	STablemapHashValue		h$_record;
	CArray <STablemapHashValue, STablemapHashValue>	new_hashes[4];
	CArray <STablemapHashValue, STablemapHashValue>	unmatched_h$_records[4];

	// Get number of records of each type in this table map
	if (_i$.begin()==_i$.end()) 
	{
		MessageBox(hwnd, "No images found - cannot create hashes.", "Table Map Error", MB_OK);
		return ERR_NOTMASTER;
	}

	// Loop through all the hash (h$) records, and check for a corresponding image (i$) record
	// Log missing records and display message if we can't find them all
	all_i$_found = true;
	for (i=0; i<=3; i++)
		unmatched_h$_records[i].RemoveAll();

	for (i=0; i<=3; i++)
	{
		for (HMapCI h_iter=_h$[i].begin(); h_iter!=_h$[i].end(); h_iter++)
		{
			this_i$_found = false;
			for (IMapCI i_iter=_i$.begin(); i_iter!=_i$.end(); i_iter++) 
			{
				if (i_iter->second.name == h_iter->second.name) 
					this_i$_found = true;
			}

			if (!this_i$_found) 
			{
				all_i$_found = false;
				h$_record.name = h_iter->second.name;
				h$_record.hash = h_iter->second.hash;
				unmatched_h$_records[i].Add(h$_record);
			}
		}
	}

	if (!all_i$_found) 
	{
		logpath.Format("%s\\hash creation log.txt", startup_path);
		if (fopen_s(&fp, logpath.GetString(), "a")==0)
		{
			get_now_time(timebuf);
			fprintf(fp, "<%s>\nCreating _hashes\n", timebuf);
			fprintf(fp, "Hashes with no matching image:\n");

			for (i=0; i<=3; i++)
				for (j=0; j<unmatched_h$_records[i].GetCount(); j++) 
					fprintf(fp, "\t%3d. h$%s\n", j+1, unmatched_h$_records[i].GetAt(j).name.GetString());

			fprintf(fp, "=======================================================\n\n");
			fclose(fp);
		}

		MessageBox(hwnd, 
				   "Could not complete hash creation, due to missing images.\n"\
				   "Please see the \"hash creation log.txt\" file for details.", 
				   "Hash Creation Error", MB_OK);
	}

	// Init new hash array
	for (i=0; i<=3; i++)
		new_hashes[i].RemoveAll();

	// Loop through each of the image records and create hashes
	for (IMapCI i_iter=_i$.begin(); i_iter!=_i$.end(); i_iter++) 
	{
		// Loop through the h$ records to find all the hashes that we have to create for this image record
		for (i=0; i<=3; i++)
		{
			for (HMapCI h_iter=_h$[i].begin(); h_iter!=_h$[i].end(); h_iter++) 
			{
				if (i_iter->second.name == h_iter->second.name) 
				{
					// create hash, based on type
					hold_hash_value.name = i_iter->second.name;

					// all pixel hash
					if (i == 0) 
					{
						// only create hash based on rgb values - ignore alpha
						for (j=0; j < (int) (i_iter->second.width * i_iter->second.height); j++)
							filtered_pix[j] = i_iter->second.pixel[j] & 0x00ffffff;

						hold_hash_value.hash = hashword(&filtered_pix[0], i_iter->second.width * i_iter->second.height, HASH_SEED_0);
					}

					// selected pixel hash
					else if (i>=1 && i<=3) 
					{
						pixcount = 0;
						for (PMapCI p_iter=_p$[i].begin(); p_iter!=_p$[i].end(); p_iter++)
						{
							if (p_iter->second.x <= i_iter->second.width &&
								p_iter->second.y <= i_iter->second.height) 
							{
								// only create hash based on rgb values - ignore alpha
								pixels[pixcount++] = 
									i_iter->second.pixel[(p_iter->second.y * i_iter->second.width) + p_iter->second.x] & 0x00ffffff;
							}
						}

						if (i==1)
								hold_hash_value.hash = hashword(&pixels[0], pixcount, HASH_SEED_1);
						if (i==2)
								hold_hash_value.hash = hashword(&pixels[0], pixcount, HASH_SEED_2);
						if (i==3)
								hold_hash_value.hash = hashword(&pixels[0], pixcount, HASH_SEED_3);

					}

					// Check for hash collision
					for (j=0; j<(int) new_hashes[i].GetSize(); j++) 
					{
						if (hold_hash_value.hash == new_hashes[i].GetAt(j).hash) 
						{
							e.Format("Hash collision:\n	<%s> and <%s>\n\nTalk to an OpenHoldem developer, please.", 
								i_iter->second.name.GetString(), 
								new_hashes[i].GetAt(j).name.GetString()); 
							MessageBox(hwnd, e, "Hash collision", MB_OK);
							return ERR_HASH_COLL;
						}
					}

					// Store this new hash in the temporary holding CArray
					new_hashes[i].Add(hold_hash_value);
				}
			}  // for (HMapCI h_iter=_h$.begin(); h_iter!=_h$.end(); h_iter++)
		} // for (i=0; i<=3; i++)
	} // for (IMapCI i_iter=_i$.begin(); i_iter!=_i$.end(); i_iter++)


	// Copy the temporary new hash CArray to the internal std::map
	for (i=0; i<=3; i++)
		h$_clear(i);

	for (i=0; i<=3; i++)
	{
		for (j=0; j<(int) new_hashes[i].GetSize(); j++)
		{
			if (!h$_insert(i, new_hashes[i].GetAt(j)))
			{
				e.Format("Hash record: %d %s %08x", i, new_hashes[i].GetAt(j).name, new_hashes[i].GetAt(j).hash);
				MessageBox(NULL, e, "ERROR adding hash value record: " + hold_hash_value.name, MB_OK | MB_TOPMOST);	
			}
		}
	}

	if (!all_i$_found)  return ERR_INCOMPLETEMASTER;

	return SUCCESS;
}

// Used by OpenScrape to calculate initial hash when first creating it with "Calculate Hash" button
uint32_t CTablemap::CalculateHashValue(IMapCI i_iter, const int type)
{
	uint32_t pixels[MAX_HASH_WIDTH*MAX_HASH_HEIGHT] = {0}, filtered_pix[MAX_HASH_WIDTH*MAX_HASH_HEIGHT] = {0};

	// all pixel hash
	if (type == 0) 
	{
		// only create hash based on rgb values - ignore alpha
		for (int j=0; j < (int) (i_iter->second.width * i_iter->second.height); j++)
			filtered_pix[j] = i_iter->second.pixel[j] & 0x00ffffff;

		return hashword(&filtered_pix[0], i_iter->second.width * i_iter->second.height, HASH_SEED_0);
	}

	// selected pixel hash
	else if (type>=1 && type<=3) 
	{
		int pixcount = 0;
		for (PMapCI p_iter=_p$[type].begin(); p_iter!=_p$[type].end(); p_iter++)
		{
			if (p_iter->second.x <= i_iter->second.width &&
				p_iter->second.y <= i_iter->second.height) 
			{
				// only create hash based on rgb values - ignore alpha
				pixels[pixcount++] = 
					i_iter->second.pixel[(p_iter->second.y * i_iter->second.width) + p_iter->second.x] & 0x00ffffff;
			}
		}

		if (type==1)
				return hashword(&pixels[0], pixcount, HASH_SEED_1);
		if (type==2)
				return hashword(&pixels[0], pixcount, HASH_SEED_2);
		if (type==3)
				return hashword(&pixels[0], pixcount, HASH_SEED_3);

	}

	return 0;
}

// Creates the 32bit hash for an image record using name and pixels
uint32_t CTablemap::CreateI$Index(const CString name, const int width, const int height, const uint32_t *pixels)
{
	uint32_t *uints = new uint32_t[MAX_HASH_WIDTH*MAX_HASH_HEIGHT + name.GetLength()];
	int c = 0, i = 0;

	// Put the ascii value of each letter into a uint32_t
	for (i=0; i<name.GetLength(); i++)
		uints[c++] = name.Mid(i,1).GetString()[0];

	// Now the pixels
	for (i=0; i<(int) (height * width); i++)
		uints[c++] = pixels[i];

	uint32_t index = hashword(&uints[0], height * width + name.GetLength(), 0x71e9ff36);

	delete [] uints;

	return index;
}
