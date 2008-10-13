#include "StdAfx.h"
#include <Math.h>

#include "CTablemap.h"
#include "..\CTransform\CTransform.h"

#ifdef OPENHOLDEM_PROGRAM
#include "../OpenHoldem/debug.h"
#endif

#ifdef OPENSCRAPE_PROGRAM
#include "../OpenScrape/debug.h"
#endif

CTablemap			*p_tablemap = NULL;

CTablemap::CTablemap(void)
{
	__SEH_SET_EXCEPTION_HANDLER

    ClearTablemap();

	ClearR$Indices();

	_s$items.num_chairs = 10;
}

CTablemap::~CTablemap(void)
{
}

void CTablemap::ClearTablemap()
{
	CSLock lock(m_critsec);

	_valid = false;
	_filepath = "";
	_filename = "";

	_z$.RemoveAll();
	_s$.RemoveAll();
	_r$.RemoveAll();
	_t$.RemoveAll();
	_p$.RemoveAll();
	_h$.RemoveAll();
	_i$.RemoveAll();
}

int CTablemap::LoadTablemap(const char *_filename, const char *version, const bool check_ws_date, int *linenum, CString *loaded_version) 
{
	CString				strLine = "", strLineType = "", token = "", s = "", e = "", hexval = "", t = "";
	int					i = 0, pos = 0, insert_point = 0, P = 0, j = 0, x = 0, y = 0, new_elem = 0;
	bool				supported_version = false;
	BYTE				alpha = 0, red = 0, green = 0, blue = 0;

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
	// Validate WinScrape file version, if passed in
	// check the ".wsdb1"/".ohdb1"/".ohdb2"/".osdb1" line
	if (strlen(version)) 
	{
		if (memcmp(version, VER_OPENSCRAPE_1, strlen(version)) == 0)
		{
			if (memcmp(strLine.GetString(), VER_OPENHOLDEM_2, strlen(VER_OPENHOLDEM_2)) != 0 &&
					memcmp(strLine.GetString(), VER_OPENSCRAPE_1, strlen(VER_OPENSCRAPE_1)) != 0)
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
				strLineType == VER_OPENSCRAPE_1) 
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

			_z$.Add(hold_size);
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
				_s$.Add(hold_symbol);
			}
		}

		// Handle r$indexes.r$ lines (regions)
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

			_r$.Add(hold_region);
		}

		// Handle t$ lines (fonts)
		else if (strLineType.Left(2) == "t$" ||
				 (strLineType.Left(1) == 't' &&
				  strLineType.Mid(1,1) >= "0" &&
				  strLineType.Mid(1,1) <= "9" &&
				  strLineType.Mid(2,1) == "$")) 
		{

			// Old style t$ records (no font groups) - pre "2007 Nov 1 08:32:55" WinScrape release
			if (strLineType.Left(2) == "t$") 
			{
				t = strLineType.Mid(2,1);
				hold_font.ch = t.GetString()[0];
				hold_font.group = 0;
			}
			// New style t$ records (font groups 0-3) - "2007 Nov 1 08:32:55" WinScrape release and later
			else 
			{
				t = strLineType.Mid(3,1);
				hold_font.ch = t.GetString()[0];
				hold_font.group = strLineType.GetString()[1] - '0';
			}

			if (hold_font.group<0 || hold_font.group>3)
			{
				MessageBox(NULL, strLine, "Invalid font group", MB_OK | MB_TOPMOST);
				return ERR_SYNTAX;
			}

			i = 0;
			hold_font.hexmash = "";
			while ((token = strLine.Tokenize(" \t", pos))!="" && i<=30 && pos!=-1) 
			{
				hold_font.x[i++] = strtoul(token, 0, 16);
				hold_font.hexmash.Append(token);
			}
			hold_font.x_count = i;

			// Insert into array so it is sorted alphabetically
			P = (int) _t$.GetSize();
			insert_point = -1;
			for (j=0; j<P; j++) 
			{
				if (hold_font.hexmash < _t$[j].hexmash)
				{
					insert_point = j;
					j=P+1;
				}
			}

			if (insert_point==-1)
				_t$.Add(hold_font);

			else
				_t$.InsertAt(insert_point, hold_font);
		}

		// Handle p$ lines (hash points)
		else if (strLineType.Left(1) == "p" &&
				 strLineType.Mid(1,1) >= "0" &&
				 strLineType.Mid(1,1) <= "9" &&
				 strLineType.Mid(2,1) == "$") 
		{
			// number
			token = strLineType.Mid(1,1);
			hold_hash_point.number = atol(token);

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

			// Find the right spot to insert this p$ record (so list is sorted - needed for binary searches)
			P = (int) _p$.GetSize();
			insert_point = -1;
			for (j=0; j<P; j++) 
			{
				if ( hold_hash_point.number == _p$[j].number &&
						((hold_hash_point.x == _p$[j].x && hold_hash_point.y < _p$[j].y) ||
						 (hold_hash_point.x < _p$[j].x)) )
				{
					insert_point = j;
					j=P+1;
				}
			}

			if (insert_point==-1)
				_p$.Add(hold_hash_point);

			else
				_p$.InsertAt(insert_point, hold_hash_point);
		}

		// Handle h$ lines (hash values)
		else if (strLineType.Left(1) == "h" &&
				 strLineType.Mid(1,1) >= "0" &&
				 strLineType.Mid(1,1) <= "9" &&
				 strLineType.Mid(2,1) == "$") 
		{
			// number
			hold_hash_value.number = strLineType.GetString()[1] - '0';
			if (hold_hash_value.number<0 || hold_hash_value.number>3)
			{
				MessageBox(NULL, strLine, "In_valid hash group", MB_OK | MB_TOPMOST);
				return ERR_SYNTAX;
			}

			// name
			hold_hash_value.name = strLineType.Mid(3);

			// value
			token = strLine.Tokenize(" \t", pos);
			if (token.GetLength()==0)
				return ERR_SYNTAX;

			hold_hash_value.hash = strtoul(token.GetString(), 0, 16);

			// Insert into array so it is sorted
			P = (int) _h$.GetSize();
			insert_point = -1;
			for (j=0; j<P; j++) 
			{
				if (hold_hash_value.hash < _h$[j].hash)
				{
					insert_point = j;
					j=P+1;
				}
			}

			if (insert_point==-1)
				_h$.Add(hold_hash_value);

			else
				_h$.InsertAt(insert_point, hold_hash_value);
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

			// Add the new i$ record to the internal array
			new_elem = (int) _i$.Add(hold_image);

			// Allocate space for "RGBAImage"
			t = _i$[new_elem].name + ".ppm";
			_i$[new_elem].image = new RGBAImage(_i$[new_elem].width, _i$[new_elem].height, t.GetString());

			// read next "height" lines
			for (y=0; y < (int) _i$[new_elem].height; y++) 
			{
				(*linenum)++;
				if (!ar.ReadString(strLine)) 
				{
					return ERR_SYNTAX;
				}
				// scan across "width" of line to get values
				for (x=0; x < (int) _i$[new_elem].width; x++) 
				{
					// unreverse bgra to abgr
					hexval = strLine.Mid(x*8+6, 2) + strLine.Mid(x*8, 6);
					_i$[new_elem].pixel[y*_i$[new_elem].width + x] = strtoul(hexval, 0, 16);
					alpha = (_i$[new_elem].pixel[y*_i$[new_elem].width + x] >> 24) &0xff;
					blue =  (_i$[new_elem].pixel[y*_i$[new_elem].width + x] >> 16) &0xff;
					green = (_i$[new_elem].pixel[y*_i$[new_elem].width + x] >>  8) &0xff;
					red =   (_i$[new_elem].pixel[y*_i$[new_elem].width + x] >>  0) &0xff;
					_i$[new_elem].image->Set(red, green, blue, alpha, y*_i$[new_elem].width + x);
				}
			}
		}

		// Unknown line type
		else 
		{
			MessageBox(NULL, strLine, "Unknown Line Type", MB_OK | MB_TOPMOST);
			return ERR_UNK_LN_TYPE;
		}

		(*linenum)++;
	}
	while (ar.ReadString(strLine));

	// Populate t$ hexmash std::map for fast lookups
	for (j=0; j<=3; j++)
		UpdateHexmashesHashes(j);

	_valid = true;

	return SUCCESS;
}

int CTablemap::SaveTablemap(CArchive& ar, const char *version_text)
{
	CString		s = "", t = "", text = "";
	int			N = 0, i = 0, j = 0, x = 0, y = 0;
	char		nowtime[26] = {0};

	// Version
	s.Format("%s\r\n", VER_OPENSCRAPE_1); ar.WriteString(s);
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
	N = (int) _z$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("z$%-16s %d  %d\r\n", _z$[i].name, _z$[i].width, _z$[i].height); 
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// strings
	ar.WriteString("//\r\n");
	ar.WriteString("// strings\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) _s$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("s$%s", _s$[i].name.GetString());
		while (s.GetLength()<18) s.Append(" ");
		s.Append(_s$[i].text);
		s.Append("\r\n");
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// regions
	ar.WriteString("//\r\n");
	ar.WriteString("// regions\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) _r$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("r$%s", _r$[i].name.GetString());
		while (s.GetLength()<18) s.Append(" ");
		t.Format("%3d %3d %3d %3d %8x %4d ", 
			_r$[i].left, _r$[i].top, _r$[i].right, _r$[i].bottom,
			_r$[i].color, _r$[i].radius);
		s.Append(t);
		t = _r$[i].transform;
		while (t.GetLength()<2) { t.Append(" "); }
		s.Append(t);
		//t.Format("		  %d\r\n", _r$[i].flags);
		t.Format("\r\n");
		s.Append(t);
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// fonts
	ar.WriteString("//\r\n");
	ar.WriteString("// fonts\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) _t$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("t%d$%c", _t$[i].group, _t$[i].ch);
		for (j=0; j<_t$[i].x_count; j++) { 
			t.Format(" %x", _t$[i].x[j]);
			s.Append(t);
		}
		s.Append("\r\n");
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// points
	ar.WriteString("//\r\n");
	ar.WriteString("// points\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) _p$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("p%d$%4d %4d\r\n", _p$[i].number, _p$[i].x, _p$[i].y);  ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// hash
	ar.WriteString("//\r\n");
	ar.WriteString("// hash\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) _h$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("h%d$%s", _h$[i].number, _h$[i].name.GetString());
		while (s.GetLength()<19) { s.Append(" "); }
		t.Format("%08x\r\n", _h$[i].hash);
		s.Append(t);
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// images
	ar.WriteString("//\r\n");
	ar.WriteString("// images\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) _i$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("i$%-16s %-3d %-3d\n", _i$[i].name, _i$[i].width, _i$[i].height);
		ar.WriteString(s);
		for (y=0; y<(int) _i$[i].height; y++)
		{
			s = "";
			for (x=0; x<(int) _i$[i].width; x++)
			{
				text.Format("%08x", _i$[i].pixel[y*_i$[i].width + x]);
				t = text.Mid(2, 6) + text.Mid(0,2);
				//t = text;
				s.Append(t);
			}
			s.Append("\r\n");
			ar.WriteString(s);
		}
	}
	ar.WriteString("\r\n");

	ar.Close();

	return SUCCESS;	
}


int CTablemap::ConvertTablemap(const HWND hwnd, const char *startup_path) 
{
	int						N = 0, imageloop = 0, hashloop = 0, regionloop = 0, j = 0, insert_point = 0, hash_type = 0;
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
	CArray <STablemapHashValue, STablemapHashValue>	new_hashes;
	CArray <STablemapHashValue, STablemapHashValue>	unmatched_h$_records;

	// Get number of records of each type in this table map
	if ((int) _i$.GetSize()==0) 
	{
		MessageBox(hwnd, "No i$ records found - is this a master?", "'Profile' Error", MB_OK);
		return ERR_NOTMASTER;
	}

	// Loop through all the region (r$indexes.r$) records, and invert the colors to align with
	// Windows' COLORREF  ARGB->ABGR
	for (regionloop=0; regionloop<(int) _r$.GetSize(); regionloop++) 
	{
		_r$[regionloop].color = 
			(_r$[regionloop].color & 0xff000000) +
			((_r$[regionloop].color & 0x00ff0000)>>16) +
			((_r$[regionloop].color & 0x0000ff00)) +
			((_r$[regionloop].color & 0x000000ff)<<16);
	}

	// Loop through all the region (r$indexes.r$) records, and look for i3slider or i3handle
	// Warn if these are found and remove them
	//warning_displayed = false;
	//for (regionloop=0; regionloop<(int) r$.GetSize(); regionloop++) {
	//	if (r$[regionloop].name == "i3slider" ||
	//		r$[regionloop].name == "i3handle")
	//	{
	//		if (!warning_displayed)
	//		{
	//			MessageBox(pMyMainWnd->GetSafeHwnd(), "This WinScrape 'profile' contains either a '_r$indexes.r$i3slider'\n"\
	//												  "or '_r$indexes.r$i3handle' region definition.  OpenScrape/OpenHoldem\n"\
	//												  "do not make use of these region types and they will not be\n"\
	//												  "saved in the OpenScrape table map format.", "'Profile' Warning", MB_OK);
	//			warning_displayed = true;
	//		}

	//		r$indexes.r$.RemoveAt(regionloop);
	//	}
	//}

	// Loop through all the hash (h$) records, and check for a corresponding image (i$) record
	all_i$_found = true;
	unmatched_h$_records.RemoveAll();

	for (hashloop=0; hashloop<(int) _h$.GetSize(); hashloop++) 
	{
		this_i$_found = false;
		for (imageloop=0; imageloop<(int) _i$.GetSize(); imageloop++) 
		{
			if (_i$[imageloop].name == _h$[hashloop].name) 
			{
				this_i$_found = true;
				imageloop = (int) _i$.GetSize() + 1;
			}
		}
		if (!this_i$_found) 
		{
			all_i$_found = false;
			h$_record.name = _h$[hashloop].name;
			h$_record.hash = _h$[hashloop].hash;
			h$_record.number = _h$[hashloop].number;
			unmatched_h$_records.Add(h$_record);
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

			for (j=0; j<unmatched_h$_records.GetCount(); j++)
				fprintf(fp, "\t%3d. h$%s\n", j+1, unmatched_h$_records[j].name.GetString());

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

	// Init new hash array
	new_hashes.RemoveAll();

	// Loop through each of the image records and create _hashes
	for (imageloop=0; imageloop<(int) _i$.GetSize(); imageloop++) 
	{
		// Loop through the h$ records to find all the _hashes that we have to create for this image record
		for (hashloop=0; hashloop<(int) _h$.GetSize(); hashloop++) 
		{
			if (_i$[imageloop].name == _h$[hashloop].name) 
			{
				
				hash_type = _h$[hashloop].number;

				// create hash, based on type
				hold_hash_value.name = _i$[imageloop].name;
				hold_hash_value.number = hash_type;

				// Get count of pixels (p$ records)
				pixcount = 0;
				for (j=0; j<(int) _p$.GetSize(); j++) 
				{
					if (_p$[j].number == hash_type &&
						_p$[j].x <= _i$[imageloop].width &&
						_p$[j].y <= _i$[imageloop].height) 
					{
							pixels[pixcount++] = 
								_i$[imageloop].pixel[_p$[j].y * _i$[imageloop].width +
															 _p$[j].x];				
					}
				}

				if (hash_type>=0 && hash_type<=3) 
				{
				
					hash_seed = hash_type==0 ? HASH_SEED_0 :
								hash_type==1 ? HASH_SEED_1 :
								hash_type==2 ? HASH_SEED_2 :
								hash_type==3 ? HASH_SEED_3 : 0;

					// Create hash
					if (pixcount==0) 
					{
						hold_hash_value.hash = hashword(&_i$[imageloop].pixel[0], 
														_i$[imageloop].width * _i$[imageloop].height,
														hash_seed);
					}
					else 
					{
						hold_hash_value.hash = hashword(&pixels[0], pixcount, hash_seed);
					}
				}

				// bad hash type
				else 
				{
					e.Format("In_valid hash type (%d) for image record <%s>", hash_type, _i$[imageloop].name.GetString()); 
					MessageBox(hwnd, e, "In_valid hash type", MB_OK);
					return ERR_INV_HASH_TYPE;
				}

				// Check for hash collision
				N = (int) new_hashes.GetSize();
				for (j=0; j<N; j++) {
					if (hold_hash_value.hash == new_hashes[j].hash) 
					{
						e.Format("Hash collision:\n	<%s> and <%s>\n\nTalk to an OpenHoldem developer, please.\n%s %08x %08x", 
							_i$[imageloop].name.GetString(), 
							new_hashes[j].name.GetString(),
							_h$[hashloop].name,
							hold_hash_value.hash, new_hashes[j].hash
							); 
						MessageBox(hwnd, e, "Hash collision", MB_OK);
						return ERR_HASH_COLL;
					}
				}

				// Find the right spot to insert this hash record (so list is sorted at the end)
				N = (int) new_hashes.GetSize();
				insert_point = -1;
				for (j=0; j<N; j++) 
				{
					if (hold_hash_value.hash < new_hashes[j].hash) 
					{
						insert_point = j;
						j=N+1;
					}
				}

				if (insert_point==-1)
					new_hashes.Add(hold_hash_value);

				else
					new_hashes.InsertAt(insert_point, hold_hash_value);
			}
		}  // for (hashloop=0; hashloop<num_hrecs; hashloop++)
	} // for (imageloop=0; imageloop<num_irecs; imageloop++)


	CSLock lock(m_critsec);
	
	// Kill the existing h$ records, and replace with new_hashes records
	_h$.RemoveAll();
	for (j=0; j<(int) new_hashes.GetSize(); j++) 
	{
		hold_hash_value.name = new_hashes[j].name;
		hold_hash_value.number = new_hashes[j].number;
		hold_hash_value.hash = new_hashes[j].hash;
		_h$.Add(hold_hash_value);
	}

	// Populate h$ hashes std::map for fast lookups
	for (j=0; j<=3; j++)
		_hashes[j].clear();
	for (j=0; j<(int) _h$.GetSize(); j++)
		_hashes[_h$[j].number].insert(std::pair<uint32_t, int> (_h$[j].hash, j));

	return SUCCESS;
}

int CTablemap::UpdateHashes(const HWND hwnd, const char *startup_path)
{
	int						num_irecs = 0, num_hrecs = 0, num_precs = 0, num_rrecs = 0, N = 0;
	int						imageloop = 0, hashloop = 0, j = 0, insert_point = 0, hash_type = 0;
	CString					e = "", s = "";
	uint32_t				pixels[MAX_HASH_WIDTH*MAX_HASH_HEIGHT] = {0}, filtered_pix[MAX_HASH_WIDTH*MAX_HASH_HEIGHT] = {0};
	int						pixcount = 0;
	bool					all_i$_found = false, this_i$_found = false;
	FILE					*fp = NULL;
	char					timebuf[30] = {0};
	CString					logpath = "";
	STablemapHashValue		hold_hash_value;
	STablemapHashValue		h$_record;
	CArray <STablemapHashValue, STablemapHashValue>	new_hashes;
	CArray <STablemapHashValue, STablemapHashValue>	unmatched_h$_records;

	// Get number of records of each type in this table map
	num_irecs = (int) _i$.GetSize();
	if (num_irecs==0) 
	{
		MessageBox(hwnd, "No images found - cannot create hashes.", "Table Map Error", MB_OK);
		return ERR_NOTMASTER;
	}

	num_hrecs = (int) _h$.GetSize();
	num_precs = (int) _p$.GetSize();
	num_rrecs = (int) _r$.GetSize();

	// Loop through all the hash (h$) records, and check for a corresponding image (i$) record
	all_i$_found = true;
	unmatched_h$_records.RemoveAll();

	for (hashloop=0; hashloop<num_hrecs; hashloop++) 
	{
		this_i$_found = false;
		for (imageloop=0; imageloop<num_irecs; imageloop++) 
		{
			if (_i$[imageloop].name == _h$[hashloop].name) 
			{
				this_i$_found = true;
				imageloop = num_irecs + 1;
			}
		}
		if (!this_i$_found) 
		{
			all_i$_found = false;
			h$_record.name = _h$[hashloop].name;
			h$_record.hash = _h$[hashloop].hash;
			h$_record.number = _h$[hashloop].number;
			unmatched_h$_records.Add(h$_record);
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

			for (j=0; j<unmatched_h$_records.GetCount(); j++) 
				fprintf(fp, "\t%3d. h$%s\n", j+1, unmatched_h$_records[j].name.GetString());

			fprintf(fp, "=======================================================\n\n");
			fclose(fp);
		}

		MessageBox(hwnd, 
				   "Could not complete hash creation, due to missing images.\n"\
				   "Please see the \"hash creation log.txt\" file for details.", 
				   "Hash Creation Error", MB_OK);
	}

	// Init new hash array
	new_hashes.RemoveAll();

	// Loop through each of the image records and create _hashes
	for (imageloop=0; imageloop<num_irecs; imageloop++) 
	{
		// Loop through the h$ records to find all the _hashes that we have to create for this image record
		for (hashloop=0; hashloop<num_hrecs; hashloop++) 
		{
			if (_i$[imageloop].name == _h$[hashloop].name) 
			{
				
				hash_type = _h$[hashloop].number;

				// create hash, based on type
				hold_hash_value.name = _i$[imageloop].name;
				hold_hash_value.number = hash_type;

				// all pixel hash
				if (hash_type == 0) 
				{

					// only create hash based on rgb values - ignore alpha
					for (j=0; j < (int) (_i$[imageloop].width * _i$[imageloop].height); j++)
						filtered_pix[j] = _i$[imageloop].pixel[j] & 0x00ffffff;

					hold_hash_value.hash = hashword(&filtered_pix[0], _i$[imageloop].width * _i$[imageloop].height, HASH_SEED_0);
				}

				// selected pixel hash
				else if (hash_type>=1 && hash_type<=3) 
				{
					pixcount = 0;
					for (j=0; j<num_precs; j++) 
					{
						if (_p$[j].number == hash_type &&
							_p$[j].x <= _i$[imageloop].width &&
							_p$[j].y <= _i$[imageloop].height) 
						{
								// only create hash based on rgb values - ignore alpha
								pixels[pixcount++] = 
									_i$[imageloop].pixel[_p$[j].y * _i$[imageloop].width + _p$[j].x] & 0x00ffffff;
						}
					}

					if (hash_type==1)
							hold_hash_value.hash = hashword(&pixels[0], pixcount, HASH_SEED_1);
					if (hash_type==2)
							hold_hash_value.hash = hashword(&pixels[0], pixcount, HASH_SEED_2);
					if (hash_type==3)
							hold_hash_value.hash = hashword(&pixels[0], pixcount, HASH_SEED_3);

				}

				// bad hash type
				else 
				{
					e.Format("Invalid hash type (%d) for image record <%s>", hash_type, _i$[imageloop].name.GetString()); 
					MessageBox(hwnd, e, "Invalid hash type", MB_OK);
					return ERR_INV_HASH_TYPE;
				}

				// Check for hash collision
				N = (int) new_hashes.GetSize();
				for (j=0; j<N; j++) 
				{
					if (hold_hash_value.hash == new_hashes[j].hash) 
					{
						e.Format("Hash collision:\n	<%s> and <%s>\n\nTalk to an OpenHoldem developer, please.", 
							_i$[imageloop].name.GetString(), 
							new_hashes[j].name.GetString()); 
						MessageBox(hwnd, e, "Hash collision", MB_OK);
						return ERR_HASH_COLL;
					}
				}

				// Find the right spot to insert this hash record (so list is sorted at the end)
				N = (int) new_hashes.GetSize();
				insert_point = -1;
				for (j=0; j<N; j++) 
				{
					if (hold_hash_value.hash < new_hashes[j].hash) 
					{
						insert_point = j;
						j=N+1;
					}
				}
				if (insert_point==-1)
					new_hashes.Add(hold_hash_value);

				else
					new_hashes.InsertAt(insert_point, hold_hash_value);
			}
		}  // for (hashloop=0; hashloop<num_hrecs; hashloop++)
	} // for (imageloop=0; imageloop<num_irecs; imageloop++)


	CSLock lock(m_critsec);
	
	// Kill the existing h$ records, and replace with new_hashes records
	_h$.RemoveAll();
	for (j=0; j<(int) new_hashes.GetSize(); j++) 
	{
		hold_hash_value.name = new_hashes[j].name;
		hold_hash_value.number = new_hashes[j].number;
		hold_hash_value.hash = new_hashes[j].hash;
		_h$.Add(hold_hash_value);
	}

	// Populate h$ hashes std::map for fast lookups
	for (j=0; j<=3; j++)
		_hashes[j].clear();
	for (j=0; j<(int) _h$.GetSize(); j++)
		_hashes[_h$[j].number].insert(std::pair<uint32_t, int> (_h$[j].hash, j));

	if (!all_i$_found)  return ERR_INCOMPLETEMASTER;

	return SUCCESS;
}


void CTablemap::ClearR$Indices(void)
{
	int			i = 0, j = 0, k = 0;

	// common card info
	for (i=0; i<=4; i++)
	{
		_r$indexes.r$c0cardfaceX_index[i] = -1;
		_r$indexes.r$c0cardfaceXrank_index[i] = -1;
		_r$indexes.r$c0cardfaceXsuit_index[i] = -1;
	}

	// pots
	for (i=0; i<=9; i++)
		for (j=0; j<=9; j++)
			for (k=0; k<=9; k++)
				_r$indexes.r$c0potXchipYZ_index[i][j][k] = -1;

	for (i=0; i<=9; i++)
	{
		// player info
		_r$indexes.r$pXcardfaceY_index[i][0] = -1;
		_r$indexes.r$pXcardfaceY_index[i][1] = -1;
		_r$indexes.r$pXcardfaceYrank_index[i][0] = -1;
		_r$indexes.r$pXcardfaceYrank_index[i][1] = -1;
		_r$indexes.r$pXcardfaceYsuit_index[i][0] = -1;
		_r$indexes.r$pXcardfaceYsuit_index[i][1] = -1;
		_r$indexes.r$uXcardfaceY_index[i][0] = -1;
		_r$indexes.r$uXcardfaceY_index[i][1] = -1;
		_r$indexes.r$pXcardback_index[i] = -1;
		_r$indexes.r$pXseated_index[i] = -1;
		_r$indexes.r$uXseated_index[i] = -1;
		_r$indexes.r$pXactive_index[i] = -1;
		_r$indexes.r$uXactive_index[i] = -1;
		_r$indexes.r$pXdealer_index[i] = -1;
		_r$indexes.r$pXname_index[i] = -1;
		_r$indexes.r$uXname_index[i] = -1;
		_r$indexes.r$pXbalance_index[i] = -1;
		_r$indexes.r$uXbalance_index[i] = -1;
		_r$indexes.r$pXbet_index[i] = -1;

		for (j=0; j<=9; j++)
			for (k=0; k<=9; k++)
				_r$indexes.r$pXchip_index[i][j][k] = -1;

		// button info
		_r$indexes.r$iXbutton_index[i] = -1;
		_r$indexes.r$iXstate_index[i] = -1;
		_r$indexes.r$iXlabel_index[i] = -1;

		for (j=0; j<=9; j++)
			_r$indexes.r$iXlabelY_index[i][j] = -1;

		_r$indexes.r$iXslider_index[i] = -1;
		_r$indexes.r$iXhandle_index[i] = -1;
		_r$indexes.r$iXedit_index[i] = -1;
		_r$indexes.r$i86Xstate_index[i] = -1;
		_r$indexes.r$i86Xbutton_index[i] = -1;

		// limits
		_r$indexes.r$c0limitsX_index[i] = -1;
		_r$indexes.r$c0handnumberX_index[i] = -1;

		// pots
		_r$indexes.r$c0potX_index[i] = -1;
	}

	// limits
	_r$indexes.r$c0limits_index = -1;
	_r$indexes.r$c0istournament_index = -1;
	_r$indexes.r$c0sblind_index = -1;
	_r$indexes.r$c0bblind_index = -1;
	_r$indexes.r$c0bigbet_index = -1;
	_r$indexes.r$c0ante_index = -1;
	_r$indexes.r$c0handnumber_index = -1;

	// player info
	_r$indexes.r$uname_index = -1;
	_r$indexes.r$ubalance_index = -1;

	// button info
	_r$indexes.r$i86state_index = -1;
	_r$indexes.r$i86button_index = -1;
}

void CTablemap::SaveR$Indices(void)
{
	// _r$tablepointX not indexed, as it is only used for finding tables on green circle-click, and
	//  this function is not called until a table has been selected by the user

	int		i = 0;
	int		cardnum = 0, seatnum = 0, buttonnum = 0, vertstride = 0, horizstride = 0;
	int		potnum = 0, limitnum = 0, handnum = 0, indexnum = 0;

	ClearR$Indices();

	for (i=0; i<_r$.GetCount(); i++)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////
		// Player info
		// Player cards, r$pXcardfaceYrank, r$pXcardfaceYsuit
		if (_r$[i].name.Mid(0,1)=="p" &&
				_r$[i].name.Mid(2,8)=="cardface" &&
				_r$[i].name.Mid(11,4)=="rank")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			cardnum = _r$[i].name.GetString()[10] - '0';
			_r$indexes.r$pXcardfaceYrank_index[seatnum][cardnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="p" &&
				 _r$[i].name.Mid(2,8)=="cardface" &&
				 _r$[i].name.Mid(11,4)=="suit")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			cardnum = _r$[i].name.GetString()[10] - '0';
			_r$indexes.r$pXcardfaceYsuit_index[seatnum][cardnum] = i;
		}

		// Player cards,r$pXcardfaceY, r$uXcardfaceY
		else if (_r$[i].name.Mid(0,1)=="p" && _r$[i].name.Mid(2,8)=="cardface")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			cardnum = _r$[i].name.GetString()[10] - '0';
			_r$indexes.r$pXcardfaceY_index[seatnum][cardnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="u" && _r$[i].name.Mid(2,8)=="cardface")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			cardnum = _r$[i].name.GetString()[10] - '0';
			_r$indexes.r$uXcardfaceY_index[seatnum][cardnum] = i;
		}

		// Player card backs, r$pXcardback
		else if (_r$[i].name.Mid(0,1)=="p" && _r$[i].name.Mid(2,8)=="cardback")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$pXcardback_index[seatnum] = i;
		}

		// Seated, r$pXseated, _r$uXseated
		else if (_r$[i].name.Mid(0,1)=="p" && _r$[i].name.Mid(2,6)=="seated")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$pXseated_index[seatnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="u" && _r$[i].name.Mid(2,6)=="seated")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$uXseated_index[seatnum] = i;
		}

		// Active, r$pXactive, r$uXactive
		else if (_r$[i].name.Mid(0,1)=="p" && _r$[i].name.Mid(2,6)=="active")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$pXactive_index[seatnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="u" && _r$[i].name.Mid(2,6)=="active")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$uXactive_index[seatnum] = i;
		}

		// Dealer button, r$pXdealer
		else if (_r$[i].name.Mid(0,1)=="p" && _r$[i].name.Mid(2,6)=="dealer")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$pXdealer_index[seatnum] = i;
		}

		// Player name r$pXname, r$uXname, r$uname
		else if (_r$[i].name.Mid(0,1)=="p" && _r$[i].name.Mid(2,4)=="name")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$pXname_index[seatnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="u" && _r$[i].name.Mid(2,4)=="name")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$uXname_index[seatnum] = i;
		}
		else if (_r$[i].name=="uname")
		{
			_r$indexes.r$uname_index = i;
		}

		// Player balance, r$pXbalance, r$uXbalance, r$ubalance
		else if (_r$[i].name.Mid(0,1)=="p" && _r$[i].name.Mid(2,7)=="balance")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$pXbalance_index[seatnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="u" && _r$[i].name.Mid(2,7)=="balance")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$uXbalance_index[seatnum] = i;
		}
		else if (_r$[i].name=="ubalance")
		{
			_r$indexes.r$ubalance_index = i;
		}

		// Player bet, r$pXbet, r$pXchipYZ
		else if (_r$[i].name.Mid(0,1)=="p" && _r$[i].name.Mid(2,3)=="bet")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$pXbet_index[seatnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="p" && _r$[i].name.Mid(2,4)=="chip")
		{
			seatnum = _r$[i].name.GetString()[1] - '0';
			vertstride = _r$[i].name.GetString()[6] - '0';
			horizstride = _r$[i].name.GetString()[7] - '0';
			_r$indexes.r$pXchip_index[seatnum][vertstride][horizstride] = i;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		// Common card info
		// Common cards, r$c0cardfaceXrank, r$c0cardfaceXsuit
		else if (_r$[i].name.Mid(0,10)=="c0cardface" && _r$[i].name.Mid(11,4)=="rank")
		{
			cardnum = _r$[i].name.GetString()[10] - '0';
			_r$indexes.r$c0cardfaceXrank_index[cardnum] = i;
		}
		else if (_r$[i].name.Mid(0,10)=="c0cardface" && _r$[i].name.Mid(11,4)=="suit")
		{
			cardnum = _r$[i].name.GetString()[10] - '0';
			_r$indexes.r$c0cardfaceXsuit_index[cardnum] = i;
		}
		// Common cards, r$c0cardfaceX
		else if (_r$[i].name.Mid(0,10)=="c0cardface")
		{
			cardnum = _r$[i].name.GetString()[10] - '0';
			_r$indexes.r$c0cardfaceX_index[cardnum] = i;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		// Button info
		// r$iXbutton, r$iXstate, r$iXlabel, r$iXslider, r$iXhandle, r$iXedit, r$i86Xstate, r$i86Xbutton,
		// r$i86state, r$i86button
		else if (_r$[i].name.Mid(0,1)=="i" && _r$[i].name.Mid(2,6)=="button")
		{
			buttonnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$iXbutton_index[buttonnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="i" && _r$[i].name.Mid(2,5)=="state")
		{
			buttonnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$iXstate_index[buttonnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="i" && _r$[i].name.Mid(2,5)=="label" && _r$[i].name.GetLength()==7)
		{
			buttonnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$iXlabel_index[buttonnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="i" && _r$[i].name.Mid(2,5)=="label" && _r$[i].name.GetLength()==8)
		{
			buttonnum = _r$[i].name.GetString()[1] - '0';
			indexnum = _r$[i].name.GetString()[7] - '0';
			_r$indexes.r$iXlabelY_index[buttonnum][indexnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="i" && _r$[i].name.Mid(2,6)=="slider")
		{
			buttonnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$iXslider_index[buttonnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="i" && _r$[i].name.Mid(2,6)=="handle")
		{
			buttonnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$iXhandle_index[buttonnum] = i;
		}
		else if (_r$[i].name.Mid(0,1)=="i" && _r$[i].name.Mid(2,4)=="edit")
		{
			buttonnum = _r$[i].name.GetString()[1] - '0';
			_r$indexes.r$iXedit_index[buttonnum] = i;
		}
		else if (_r$[i].name=="i86button")
		{
			_r$indexes.r$i86button_index = i;
		}
		else if (_r$[i].name.Mid(0,3)=="i86" && _r$[i].name.Mid(4,6)=="button")
		{
			buttonnum = _r$[i].name.GetString()[3] - '0';
			_r$indexes.r$i86Xbutton_index[buttonnum] = i;
		}
		else if (_r$[i].name=="i86state")
		{
			_r$indexes.r$i86state_index = i;
		}
		else if (_r$[i].name.Mid(0,3)=="i86" && _r$[i].name.Mid(4,5)=="state")
		{
			buttonnum = _r$[i].name.GetString()[3] - '0';
			_r$indexes.r$i86Xstate_index[buttonnum] = i;
		}


		///////////////////////////////////////////////////////////////////////////////////////////////
		// Pots
		// Pots, r$c0potX, r$c0potXchipYZ
		else if (_r$[i].name.Mid(0,5)=="c0pot" && _r$[i].name.Find("chip")==-1)
		{
			potnum = _r$[i].name.GetString()[5] - '0';
			_r$indexes.r$c0potX_index[potnum] = i;
		}
		else if (_r$[i].name.Mid(0,5)=="c0pot" && _r$[i].name.Mid(6,4)=="chip")
		{
			potnum = _r$[i].name.GetString()[5] - '0';
			vertstride = _r$[i].name.GetString()[10] - '0';
			horizstride = _r$[i].name.GetString()[11] - '0';
			_r$indexes.r$c0potXchipYZ_index[potnum][vertstride][horizstride] = i;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		// Limits
		// r$c0limits, r$c0limitsX
		else if (_r$[i].name=="c0limits")
		{
			_r$indexes.r$c0limits_index = i;
		}
		else if (_r$[i].name.Mid(0,8)=="c0limits")
		{
			limitnum = _r$[i].name.GetString()[8] - '0';
			_r$indexes.r$c0limitsX_index[limitnum] = i;
		}

		// r$c0istournament
		else if (_r$[i].name == "c0istournament")
		{
			_r$indexes.r$c0istournament_index = i;
		}

		// r$c0sblind, r$c0bblind, r$c0bigbet, r$c0ante, r$c0handnumberX, r$c0handnumber
		else if (_r$[i].name == "c0sblind")
		{
			_r$indexes.r$c0sblind_index = i;
		}
		else if (_r$[i].name == "c0bblind")
		{
			_r$indexes.r$c0bblind_index = i;
		}
		else if (_r$[i].name == "c0bigbet")
		{
			_r$indexes.r$c0bigbet_index = i;
		}
		else if (_r$[i].name == "c0ante")
		{
			_r$indexes.r$c0ante_index = i;
		}
		else if (_r$[i].name=="c0handnumber")
		{
			_r$indexes.r$c0handnumber_index = i;
		}
		else if (_r$[i].name.Mid(0,12)=="c0handnumber")
		{
			handnum = _r$[i].name.GetString()[12] - '0';
			_r$indexes.r$c0handnumberX_index[handnum] = i;
		}

	}
}

void CTablemap::SaveS$Indices(void)
{
	// s$titletextX, s$!titletextX not indexed, as it is only used for finding tables on green circle-click, and
	// this function is not called until a table has been selected by the user
	// s$hXtype are not indexed, as those records are ignored in OH

	int		i = 0, num = 0;

	// Clear 'em first
	for (i=0; i<=9; i++)
	{
		_s$indexes.s$ttlimitsX_index[i] = -1;
		_s$indexes.s$c0limitsX_index[num] = -1;
	}

	_s$indexes.s$ttlimits_index = -1;
	_s$indexes.s$c0limits_index = -1;

	for (i=0; i<(int) _s$.GetSize(); i++)
	{

		// s$ttlimits, s$ttlimitsX
		if (_s$[i].name=="ttlimits")
		{
			_s$indexes.s$ttlimits_index = i;
		}
		else if (_s$[i].name.Mid(0,8)=="ttlimits")
		{
			num = _s$[i].name.GetString()[8] - '0';
			_s$indexes.s$ttlimitsX_index[num] = i;
		}
		else if (_s$[i].name=="c0limits")
		{
			_s$indexes.s$c0limits_index = i;
		}
		else if (_s$[i].name.Mid(0,8)=="c0limits")
		{
			num = _s$[i].name.GetString()[8] - '0';
			_s$indexes.s$c0limitsX_index[num] = i;
		}
	}
}

void CTablemap::SaveS$Strings(void)
{
	// s$reseller and s$mechanic are not saved, as they are only comments and not used in OH for any purpose

	int		i = 0;

	// Clear 'em first
	_s$items.num_chairs = 0;
	_s$items.swagtextmethod = 0;
	_s$items.potmethod = 0;
	_s$items.activemethod = 0;
	_s$items.sitename = "";
	_s$items.network = "";
	_s$items.chairconfig = "";
	for (i=0; i<=3; i++)
		_s$items.ttype[i] = "";

	for (i=0; i<(int) _s$.GetSize(); i++)
	{
		if (_s$[i].name == "nchairs")
			_s$items.num_chairs = strtoul(_s$[i].text.GetString(), NULL, 10);

		if (_s$[i].name == "swagtextmethod")
			_s$items.swagtextmethod = strtoul(_s$[i].text.GetString(), NULL, 10);

		if (_s$[i].name == "potmethod")
			_s$items.potmethod = strtoul(_s$[i].text.GetString(), NULL, 10);

		if (_s$[i].name == "activemethod")
			_s$items.activemethod = strtoul(_s$[i].text.GetString(), NULL, 10);

		if (_s$[i].name == "sitename")
			_s$items.sitename = _s$[i].text;

		if (_s$[i].name == "network")
			_s$items.network = _s$[i].text;

		if (_s$[i].name == "chairconfig")
			_s$items.chairconfig = _s$[i].text;

		if (_s$[i].name == "t0type")
			_s$items.ttype[0] = _s$[i].text;

		if (_s$[i].name == "t1type")
			_s$items.ttype[1] = _s$[i].text;

		if (_s$[i].name == "t2type")
			_s$items.ttype[2] = _s$[i].text;

		if (_s$[i].name == "t3type")
			_s$items.ttype[3] = _s$[i].text;

	}
}

void CTablemap::UpdateHexmashesHashes(const int group)
{
	int j = 0;

	// Populate t$ hexmash std::map for fast lookups
	_hexmashes[group].clear();
	for (j=0; j<(int) _t$.GetSize(); j++)
		if (_t$[j].group == group)
			_hexmashes[group].insert(std::pair<CString, int> (_t$[j].hexmash, j));

	// Populate h$ hashes std::map for fast lookups
	_hashes[group].clear();
	for (j=0; j<(int) _h$.GetSize(); j++)
		if (_h$[j].number == group)
			_hashes[group].insert(std::pair<uint32_t, int> (_h$[j].hash, j));
}