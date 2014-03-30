#include "stdafx.h"

#include "debug.h"
#include "tablemap.h"
#include "structs_defines.h"
#include "../../CTransform/hash/lookup3.h"
#include "global.h"
#include "scraper.h"
#include "../../CTransform/Transform.h"

void clear_tablemap(STableMap *map) 
{
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif

#ifdef SEH_ENABLE
	try {
#endif
		map->valid = false;
		map->filepath = "";
		map->z$.RemoveAll();	
		map->s$.RemoveAll();
		map->r$.RemoveAll();
		map->t$.RemoveAll();
		map->p$.RemoveAll();
		map->h$.RemoveAll();
		map->i$.RemoveAll();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("::clear_tablemap :\n"); 
		throw;
	}
#endif
}

int load_tablemap(char *filename, STableMap *map, char *version, bool check_ws_date, int *linenum) {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif

#ifdef SEH_ENABLE
	try {
#endif
		CString				strLine, strLineType, token, s, e, hexval, t;
		int					i, pos, insert_point, P, j, x, y;
		bool				supported_version;

		// temp
		Stablemap_size			hold_size;
		Stablemap_symbol		hold_symbol;
		Stablemap_region		hold_region;
		Stablemap_font			hold_font;
		Stablemap_hash_point	hold_hash_point;
		Stablemap_hash_value	hold_hash_value;
		Stablemap_image			hold_image;

		// Clean up the global.profile structure
		clear_tablemap(map);

		// Open the selected file
		CFile cfFile(filename, CFile::modeRead | CFile::shareDenyNone);
		map->filepath = filename;

		// Load its contents into a CArchive
		CArchive ar (&cfFile, CArchive::load); 

		// Read the first line of the CArchive into strLine
		strLine = "";
		*linenum = 1;
		if(!ar.ReadString(strLine)) {
			// Failed, so quit
			return ERR_EOF;
		}

		// skip any blank lines
		while (strLine.GetLength() == 0) {
			linenum++;
			if(!ar.ReadString(strLine))  {
				// Failed, so quit
				return ERR_EOF;
			}
		}

		//
		// Validate WinScrape file version, if passed in
		// check the ".wsdb1"/".ohdb1"/".ohdb2"/".osdb1" line
		if (strlen(version)) {
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

		// check the date line
		if (check_ws_date) {
			supported_version = false;
			do {
				if (strLine.Find(VER_WINSCRAPE_DATE1) != -1 ||
					strLine.Find(VER_WINSCRAPE_DATE2) != -1 ||
					strLine.Find(VER_WINSCRAPE_DATE3) != -1 ||
					strLine.Find(VER_WINSCRAPE_DATE4) != -1) {
					supported_version = true;
				}
			}
			while(ar.ReadString(strLine) && !supported_version);
			if (!supported_version) {
				return ERR_VERSION;
			}
		}

		// Repeat while there are lines in the file left to process
		do {
			// Trim the line
			strLine.Trim();

			// If the line is empty, skip it
			if(strLine.GetLength() == 0) {
				continue;
			}

			// Extract the line type
			pos=0;
			strLineType = strLine.Tokenize(" \t", pos);

			// Skip comment lines
			if (strLineType.Left(2) == "//") {
				continue;
			}

			// Skip version lines
			if (strLineType == VER_WINSCRAPE || 
				strLineType == VER_OPENHOLDEM_1 || 
				strLineType == VER_OPENHOLDEM_2 ||
				strLineType == VER_OPENSCRAPE_1) {
				continue;
			}

			// Handle z$ lines (sizes)
			if (strLineType.Left(2) == "z$") {
				// name
				hold_size.name = strLineType.Mid(2);

				// width
				token = strLine.Tokenize(" \t", pos);
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				hold_size.width = atol(token.GetString());

				// height
				token = strLine.Tokenize(" \t", pos);
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				hold_size.height = atol(token.GetString());

				map->z$.Add(hold_size);
			}

			// Handle s$ lines (symbols/string)
			else if (strLineType.Left(2) == "s$") {
				// name
				hold_symbol.name = strLineType.Mid(2);

				// text
				hold_symbol.text = strLine.Mid(strLineType.GetLength());
				hold_symbol.text.Trim();
				if (hold_symbol.text.GetLength()==0) { return ERR_SYNTAX; }

				// Skip s$hXtype lines
				if (strLineType.Left(3) == "s$h" && strLineType.Mid(4,4) == "type") 
				{
				}
				else
				{
					map->s$.Add(hold_symbol);
				}
			}

			// Handle r$ lines (regions)
			else if (strLineType.Left(2) == "r$") {
				// name
				hold_region.name = strLineType.Mid(2);

				// left
				token = strLine.Tokenize(" \t", pos);
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				hold_region.left = atol(token.GetString());

				// top
				token = strLine.Tokenize(" \t", pos);
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				hold_region.top = atol(token.GetString());

				// right
				token = strLine.Tokenize(" \t", pos);
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				hold_region.right = atol(token.GetString());

				// bottom
				token = strLine.Tokenize(" \t", pos);
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				hold_region.bottom = atol(token.GetString());

				// color
				token = strLine.Tokenize(" \t", pos);
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				hold_region.color = strtoul(token.GetString(), 0, 16);

				// radius
				token = strLine.Tokenize(" \t", pos);
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				hold_region.radius = atol(token.GetString());

				// transform
				hold_region.transform = strLine.Tokenize(" \t", pos);
				if (hold_region.transform.GetLength()==0) { return ERR_SYNTAX; }

				// flags
				//token = strLine.Tokenize(" \t", pos);
				//if (token.GetLength()==0) { return ERR_SYNTAX; }
				//hold_region.flags = atol(token.GetString());

				map->r$.Add(hold_region);
			}

			// Handle t$ lines (fonts)
			else if (strLineType.Left(2) == "t$" ||
				     (strLineType.Left(1) == 't' && 
					  strLineType.Mid(1,1) >= "0" &&
					  strLineType.Mid(1,1) <= "9" &&
					  strLineType.Mid(2,1) == "$")) {

				// Old style t$ records (no font groups) - pre "2007 Nov 1 08:32:55" WinScrape release
				if (strLineType.Left(2) == "t$") {
					t = strLineType.Mid(2,1);
					hold_font.ch = t.GetString()[0];
					hold_font.group = 0;
				}
				// New style t$ records (font groups 0-3) - "2007 Nov 1 08:32:55" WinScrape release and later
				else {
					t = strLineType.Mid(3,1);
					hold_font.ch = t.GetString()[0];
					hold_font.group = strLineType.GetString()[1] - '0';
				}

				i = 0;
				hold_font.hexmash = "";
				while ((token = strLine.Tokenize(" \t", pos))!="" && i<=30 && pos!=-1) {
					hold_font.x[i++] = strtoul(token, 0, 16);
					hold_font.hexmash.Append(token);
				}
				hold_font.x_count = i;

				// Insert into array so it is sorted alphabetically
				P = (int) map->t$.GetSize();
				insert_point = -1;
				for (j=0; j<P; j++) {
					if (hold_font.hexmash < map->t$[j].hexmash) 
					{
						insert_point = j;
						j=P+1;
					}
				}
				if (insert_point==-1) {
					map->t$.Add(hold_font);
				}
				else {
					map->t$.InsertAt(insert_point, hold_font);
				}
			}

			// Handle p$ lines (hash points)
			else if (strLineType.Left(1) == "p" &&
					 strLineType.Mid(1,1) >= "0" && 
					 strLineType.Mid(1,1) <= "9" &&
					 strLineType.Mid(2,1) == "$") {
				// number
				token = strLineType.Mid(1,1);
				hold_hash_point.number = atol(token);

				// x
				token = strLine.Tokenize(" \t", pos);
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				hold_hash_point.x = atol(token.GetString());

				// y
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				token = strLine.Tokenize(" \t", pos);
				hold_hash_point.y = atol(token.GetString());

				// Find the right spot to insert this p$ record (so list is sorted - needed for binary searches)
				P = (int) map->p$.GetSize();
				insert_point = -1;
				for (j=0; j<P; j++) {
					if ( hold_hash_point.number == map->p$[j].number &&
						 ((hold_hash_point.x == map->p$[j].x && hold_hash_point.y < map->p$[j].y) ||
						  (hold_hash_point.x < map->p$[j].x)) ) 
					{
						insert_point = j;
						j=P+1;
					}
				}
				if (insert_point==-1) {
					map->p$.Add(hold_hash_point);
				}
				else {
					map->p$.InsertAt(insert_point, hold_hash_point);
				}
			}

			// Handle h$ lines (hash values)
			else if (strLineType.Left(1) == "h" &&
					 strLineType.Mid(1,1) >= "0" && 
					 strLineType.Mid(1,1) <= "9" &&
					 strLineType.Mid(2,1) == "$") {
				// number
				hold_hash_value.number = strLineType.GetString()[1] - '0';

				// name
				hold_hash_value.name = strLineType.Mid(3);

				// value
				token = strLine.Tokenize(" \t", pos);
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				hold_hash_value.hash = strtoul(token.GetString(), 0, 16);

				// Insert into array so it is sorted
				P = (int) map->h$.GetSize();
				insert_point = -1;
				for (j=0; j<P; j++) {
					if (hold_hash_value.hash < map->h$[j].hash) 
					{
						insert_point = j;
						j=P+1;
					}
				}
				if (insert_point==-1) {
					map->h$.Add(hold_hash_value);
				}
				else {
					map->h$.InsertAt(insert_point, hold_hash_value);
				}
			}

			// Handle i$ lines (images)
			else if (strLineType.Left(2) == "i$") {
				// name
				hold_image.name = strLineType.Mid(2);

				// width
				token = strLine.Tokenize(" \t", pos);
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				hold_image.width = atol(token.GetString());

				// height
				token = strLine.Tokenize(" \t", pos);
				if (token.GetLength()==0) { return ERR_SYNTAX; }
				hold_image.height = atol(token.GetString());

				// Check size of region
				if (hold_image.width * hold_image.height > 200*150) { return ERR_REGION_SIZE; }

				//new_elem = (int) map->i$.Add(hold_image);

				// read next "height" lines
				for (x=0; x < (int) hold_image.height; x++) {
					linenum++;
					if (!ar.ReadString(strLine)) { return ERR_SYNTAX; }
					// scan across "width" of line to get values
					for (y=0; y < (int) hold_image.width; y++) {
						// unreverse bgra to abgr
						hexval = strLine.Mid(y*8+6, 2) + strLine.Mid(y*8, 6);
						//map->i$[new_elem].pixel[x*map->i$[new_elem].width + y] = strtoul(hexval, 0, 16);
					}
				}
			}

			// Unknown line type
			else {
				MessageBox(NULL, strLine, "Unknown Line Type", MB_OK);
				return ERR_UNK_LN_TYPE;
			}

			linenum++;
		}
		while(ar.ReadString(strLine));

		// Populate t$ hexmash char array for bsearches later on
		P = (int) map->t$.GetSize();
		for (j=0; j<P; j++)
			strcpy(map->hexmash[j], map->t$[j].hexmash);

		// Populate h$ hashes array for bsearches later on
		P = (int) map->h$.GetSize();
		for (j=0; j<P; j++)
			map->hashes[j] = map->h$[j].hash;

		map->valid = true;
		return SUCCESS;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("::load_tablemap :\n"); 
		throw;
	}
#endif
}

BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam) {
	CString				title, winclass;
	char				text[512];
	RECT				crect;
	STableList			tablelisthold;
	bool				profile_match;
	int					i;
	s_tablemaps			*maps = (s_tablemaps *) (lparam);

	// If this is not a top level window, then return
	if (GetParent(hwnd) != NULL) {
		return true;
	}

	// If this window is not visible, then return
	if (!IsWindowVisible(hwnd)) {
		return true;
	}

	// If there is no caption on this window, then return
	GetWindowText(hwnd, text, sizeof(text));
	if (strlen(text)==0) {
		return true;
	}
	title = text;

	// Found a candidate window, get client area rect
	GetClientRect(hwnd, &crect);

	// See if it matches a profile in the list
	profile_match = false;
	for (i=0; i < maps->n_elem && !profile_match; i++)
	{
		if (check_window_match(&maps->tablemap[i], hwnd, crect, title)) 
		{
			tablelisthold.hwnd = hwnd;
			tablelisthold.title = title;
			tablelisthold.path = maps->tablemap[i].filepath;
			tablelisthold.crect.left = crect.left;
			tablelisthold.crect.top = crect.top;
			tablelisthold.crect.right = crect.right;
			tablelisthold.crect.bottom = crect.bottom;
			global.g_tlist.Add(tablelisthold);
			profile_match = true;
		}
	}

	return true;  // keep processing through entire list of windows
}

bool check_window_match(STableMap *map, HWND h, RECT r, CString title) {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif

#ifdef SEH_ENABLE
	try {
#endif
		int				i;
		bool			good_size, good_pos_title, good_neg_title, good_table_points;
		int				width, height, x, y;
		HDC				hdcScreen, hdcCompatible;
		HBITMAP			hbmScreen, hOldScreenBitmap;
		BYTE			*pBits, alpha, red, green, blue;
		int				exact_width, exact_height, min_width, min_height, max_width, max_height;
		CTransform		trans;

		exact_width = exact_height = min_width = min_height = max_width = max_height = 0;
		for (i=0; i<(int) map->z$.GetSize(); i++) 
		{
			if (map->z$[i].name == "clientsize")
			{
				exact_width = map->z$[i].width;
				exact_height = map->z$[i].height;
			}
			else if (map->z$[i].name == "clientsizemin")
			{
				min_width = map->z$[i].width;
				min_height = map->z$[i].height;
			}
			else if (map->z$[i].name == "clientsizemax")
			{
				max_width = map->z$[i].width;
				max_height = map->z$[i].height;
			}
		}

		// Check for exact match on client size
		good_size = false;
		if (r.right == exact_width && r.bottom == exact_height) 
		{
			good_size = true;
		}

		// Check for client size that falls within min/max
		if (min_width != 0 && min_height != 0 && max_width != 0 &&  max_height != 0 &&
			r.right >= (int) min_width && r.right <= (int) max_width &&
			r.bottom >= (int) min_height && r.bottom <= (int) max_height) 
		{
			good_size = true;
		}

		if (!good_size)  
			return false;

		// Check title text for match
		good_pos_title = false;
		for (i=0; i<(int) map->s$.GetSize(); i++) 
		{
			if (map->s$[i].name.Left(9) == "titletext"  && title.Find(map->s$[i].text)!=-1) 
			{
				good_pos_title = true;
				i=(int) map->s$.GetSize()+1;
			}
		}

		if (!good_pos_title)  
			return false;

		// Check for no negative title text matches
		good_neg_title = true;
		for (i=0; i<(int) map->s$.GetSize(); i++) 
		{
			if (map->s$[i].name.Left(10) == "!titletext" && title.Find(map->s$[i].text)!=-1) 
			{
				good_neg_title = false;
				i=(int) map->s$.GetSize()+1;
			}
		}

		if (!good_neg_title)  
			return false;

		// Allocate heap space for BITMAPINFO
		BITMAPINFO	*bmi;
		int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
		bmi = (BITMAPINFO *) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

		// Check table points for match
		width = r.right - r.left;
		height = r.bottom - r.top;
		hdcScreen = GetDC(h);
		hdcCompatible = CreateCompatibleDC(hdcScreen);
		hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);
		hOldScreenBitmap = (HBITMAP) SelectObject(hdcCompatible, hbmScreen);
		BitBlt(hdcCompatible, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

		// Populate BITMAPINFOHEADER
		bmi->bmiHeader.biSize = sizeof(bmi->bmiHeader);
		bmi->bmiHeader.biBitCount = 0;
		GetDIBits(hdcCompatible, hbmScreen, 0, 0, NULL, bmi, DIB_RGB_COLORS);

		// Get the actual argb bit information
		bmi->bmiHeader.biHeight = -bmi->bmiHeader.biHeight;
		pBits = new BYTE[bmi->bmiHeader.biSizeImage];
		GetDIBits(hdcCompatible, hbmScreen, 0, height, pBits, bmi, DIB_RGB_COLORS);

		good_table_points = true;
		for (i=0; i<(int) map->r$.GetSize(); i++) 
		{
			if (map->r$[i].name.Find("tablepoint") != -1 && 
				map->r$[i].right - map->r$[i].left == 1 &&
				map->r$[i].bottom - map->r$[i].top == 1 &&
				map->r$[i].transform == "C") 
			{

				x = map->r$[i].left;
				y = map->r$[i].top;

				int pbits_loc = y*width*4 + x*4;
				alpha = pBits[pbits_loc + 3];
				red = pBits[pbits_loc + 2];
				green = pBits[pbits_loc + 1];
				blue = pBits[pbits_loc + 0];

				// positive radius
				if (map->r$[i].radius >= 0) 
				{
					if (!trans.is_in_ARGB_color_cube((map->r$[i].color>>24)&0xff,
												  GetRValue(map->r$[i].color), 
												  GetGValue(map->r$[i].color), 
												  GetBValue(map->r$[i].color), 
												  map->r$[i].radius, 
												  alpha, red, green, blue)) 
					{
						good_table_points = false;
						i=(int) map->r$.GetSize()+1;
					}
				}
				// negative radius (logical not)
				else 
				{
					if (trans.is_in_ARGB_color_cube((map->r$[i].color>>24)&0xff,
												  GetRValue(map->r$[i].color), 
												  GetGValue(map->r$[i].color), 
												  GetBValue(map->r$[i].color), 
												  -map->r$[i].radius, 
												  alpha, red, green, blue)) 
					{
						good_table_points = false;
						i=(int) map->r$.GetSize()+1;
					}
				}

			}
		}

		// Clean up
		HeapFree(GetProcessHeap(), NULL, bmi);
		delete []pBits;
		SelectObject(hdcCompatible, hOldScreenBitmap);
		DeleteObject(hbmScreen);
		DeleteDC(hdcCompatible);
		ReleaseDC(h, hdcScreen);

		if (!good_table_points)  
			return false;

		return true;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("::check_window_match :\n"); 
		throw;
	}
#endif
}

