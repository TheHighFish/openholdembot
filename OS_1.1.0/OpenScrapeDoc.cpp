// OpenScrapeDoc.cpp : implementation of the COpenScrapeDoc class
//

#include "stdafx.h"
#include "OpenScrape.h"
#include "MainFrm.h"
#include "OpenScrapeDoc.h"
#include "structs_defines.h"
#include "hash/lookup3.h"
#include "DialogTableMap.h"
#include "debug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COpenScrapeDoc
IMPLEMENT_DYNCREATE(COpenScrapeDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenScrapeDoc, CDocument)
END_MESSAGE_MAP()


// COpenScrapeDoc construction/destruction

COpenScrapeDoc::COpenScrapeDoc()
{
	ClearTableMap();
	attached_hwnd = NULL;
	ZeroMemory(&attached_rect, sizeof(RECT));
	attached_bitmap = NULL;
	attached_pBits = NULL;
	blink_on = false;
	valid_open = false;
	is_dirty = false;
}

COpenScrapeDoc::~COpenScrapeDoc()
{
}

COpenScrapeDoc * COpenScrapeDoc::GetDocument() {
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	return (COpenScrapeDoc *) pFrame->GetActiveDocument();
}

BOOL COpenScrapeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	ClearTableMap();
	if (theApp.m_TableMapDlg)  theApp.m_TableMapDlg->create_tree();

	return TRUE;
}

BOOL COpenScrapeDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if (!valid_open)
	{
		ClearTableMap();
		SetTitle("");
	}
	
	SetModifiedFlag(is_dirty);

	// Create tree on TableMap dialog
	if (theApp.m_TableMapDlg)  theApp.m_TableMapDlg->create_tree();

	// Force re-draw
	InvalidateRect(theApp.m_pMainWnd->GetSafeHwnd(), NULL, true);
	theApp.m_TableMapDlg->Invalidate(true);

	return valid_open;
}

// COpenScrapeDoc serialization

void COpenScrapeDoc::Serialize(CArchive& ar)
{
	CString			version, s;
	CMainFrame		*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	int				ret;

	if (ar.IsStoring())
	{
		SaveTableMap(ar);
	}

	else
	{
		ClearTableMap();
		version = "";
		ret = LoadTableMap(ar, &version);
		if (version == VER_WINSCRAPE && ret == SUCCESS)
		{
			if (MessageBox(pMyMainWnd->GetSafeHwnd(), "This is a WinScrape table 'profile'.  Would you like to convert to OpenScrape table map format?", 
						   "Convert table 'profile'?", MB_YESNO) == IDYES)
			{
				ret = ConvertTableMap();
				if (ret != SUCCESS && ret != ERR_INCOMPLETEMASTER)
				{
					s.Format("Error %d converting table 'profile'.", ret);
					MessageBox(pMyMainWnd->GetSafeHwnd(), s.GetString(), "Table 'profile' conversion error.", MB_OK);
					is_dirty = false;
					valid_open = false;
				}
				else
				{
					is_dirty = true;
					valid_open = true;
				}
			}
			else
			{
				is_dirty = false;
				valid_open = false;
			}
		}

		else if (version == VER_WINSCRAPE && ret != SUCCESS)
		{
			MessageBox(pMyMainWnd->GetSafeHwnd(), "This is a WinScrape 'profile', but cannot be converted,\n"\
												  "as it does not have a version date of\n"\
												  "\"2006 Aug 7 13:11:37\" or\n"\
												  "\"2007 Oct 25 19:26:57\" or\n"\
												  "\"2007 Nov 1 08:32:55\" or\n"\
												  "\"2007 Nov 3 09:54:04\" or\n"\
												  "\"2007 Nov 6 11:52:24\" or\n",  "Table map load error", MB_OK);
			is_dirty = false;
			valid_open = false;
		}

		else if (ret != SUCCESS)
		{
			s.Format("Error %d loading table map.", ret);
			MessageBox(pMyMainWnd->GetSafeHwnd(), s.GetString(), "Table map load error", MB_OK);
			is_dirty = false;
			valid_open = false;
		}
		else 
		{
			is_dirty = false;
			valid_open = true;
		}
	}
}

void COpenScrapeDoc::ClearTableMap(void)
{
	tablemap.filename = "";
	tablemap.filepath = "";
	tablemap.z$.RemoveAll();
	tablemap.s$.RemoveAll();
	tablemap.r$.RemoveAll();
	tablemap.t$.RemoveAll();
	tablemap.p$.RemoveAll();
	tablemap.h$.RemoveAll();
	tablemap.i$.RemoveAll();
}

int COpenScrapeDoc::LoadTableMap(CArchive& ar, CString *version)
{
	CMainFrame			*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	CString				strLine, strLineType, token, s, e, hexval, t;
	int					linenum, i, x, y, pos, new_elem, insert_point, P, j;
	bool				supported_version;
	CFile				*cf = ar.GetFile();

	// temp
	Stablemap_size			hold_size;
	Stablemap_symbol		hold_symbol;
	Stablemap_region		hold_region;
	Stablemap_font			hold_font;
	Stablemap_hash_point	hold_hash_point;
	Stablemap_hash_value	hold_hash_value;
	Stablemap_image			hold_image;

	// Save path of this file
	tablemap.filename = cf->GetFileName();
	tablemap.filepath = cf->GetFilePath();

	// Read the first line of the CArchive into strLine
	strLine = "";
	linenum = 1;
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
	// Make sure we know how to read this file
	supported_version = false;
	if (memcmp(strLine.GetString(), VER_WINSCRAPE, strlen(VER_WINSCRAPE)) == 0)
	{
		version->Format("%s", VER_WINSCRAPE);
		supported_version = true;
	}
	else if (memcmp(strLine.GetString(), VER_OPENHOLDEM_1, strlen(VER_OPENHOLDEM_1)) == 0)
	{
		version->Format("%s", VER_OPENHOLDEM_1);
		supported_version = true;
	}
	else if (memcmp(strLine.GetString(), VER_OPENHOLDEM_2, strlen(VER_OPENHOLDEM_2)) == 0)
	{
		version->Format("%s", VER_OPENHOLDEM_2);
		supported_version = true;
	}
	else if (memcmp(strLine.GetString(), VER_OPENSCRAPE_1, strlen(VER_OPENSCRAPE_1)) == 0)
	{
		version->Format("%s", VER_OPENSCRAPE_1);
		supported_version = true;
	}

	if (!supported_version) {
		return ERR_VERSION;
	}

	// check the date line
	if (memcmp(strLine.GetString(), VER_WINSCRAPE, strlen(VER_WINSCRAPE))==0) 
	{
		supported_version = false;
		do {
			if (strLine.Find(VER_WINSCRAPE_DATE1) != -1 ||
				strLine.Find(VER_WINSCRAPE_DATE2) != -1 ||
				strLine.Find(VER_WINSCRAPE_DATE3) != -1 ||
				strLine.Find(VER_WINSCRAPE_DATE4) != -1 ||
				strLine.Find(VER_WINSCRAPE_DATE5) != -1) {
				supported_version = true;
			}
		}
		while(ar.ReadString(strLine) && !supported_version);
		if (!supported_version) {
			return ERR_VERSION;
		}
	}
	else
	{
		ar.ReadString(strLine);
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
		if (strLineType == VER_WINSCRAPE || strLineType == VER_OPENHOLDEM_1 || strLineType == VER_OPENHOLDEM_2) {
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

			tablemap.z$.Add(hold_size);
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
				tablemap.s$.Add(hold_symbol);
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
			if (hold_region.transform == "T")  hold_region.transform = "T0";

			// flags
			//token = strLine.Tokenize(" \t", pos);
			//if (token.GetLength()==0) { return ERR_SYNTAX; }
			//hold_region.flags = atol(token.GetString());

			tablemap.r$.Add(hold_region);
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
			P = (int) tablemap.t$.GetSize();
			insert_point = -1;
			for (j=0; j<P; j++) {
				if (hold_font.hexmash < tablemap.t$[j].hexmash) 
				{
					insert_point = j;
					j=P+1;
				}
			}
			if (insert_point==-1) {
				tablemap.t$.Add(hold_font);
			}
			else {
				tablemap.t$.InsertAt(insert_point, hold_font);
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
			P = (int) tablemap.p$.GetSize();
			insert_point = -1;
			for (j=0; j<P; j++) {
				if ( hold_hash_point.number == tablemap.p$[j].number &&
					 ((hold_hash_point.x == tablemap.p$[j].x && hold_hash_point.y < tablemap.p$[j].y) ||
					  (hold_hash_point.x < tablemap.p$[j].x)) ) 
				{
					insert_point = j;
					j=P+1;
				}
			}
			if (insert_point==-1) {
				tablemap.p$.Add(hold_hash_point);
			}
			else {
				tablemap.p$.InsertAt(insert_point, hold_hash_point);
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
			P = (int) tablemap.h$.GetSize();
			insert_point = -1;
			for (j=0; j<P; j++) {
				if (hold_hash_value.hash < tablemap.h$[j].hash) 
				{
					insert_point = j;
					j=P+1;
				}
			}
			if (insert_point==-1) {
				tablemap.h$.Add(hold_hash_value);
			}
			else {
				tablemap.h$.InsertAt(insert_point, hold_hash_value);
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

			new_elem = (int) tablemap.i$.Add(hold_image);

			// read next "height" lines
			for (y=0; y < (int) tablemap.i$[new_elem].height; y++) {
				linenum++;
				if (!ar.ReadString(strLine)) { return ERR_SYNTAX; }
				// scan across "width" of line to get values
				for (x=0; x < (int) tablemap.i$[new_elem].width; x++) {
					// unreverse bgra to abgr
					hexval = strLine.Mid(x*8+6, 2) + strLine.Mid(x*8, 6);
					tablemap.i$[new_elem].pixel[y*tablemap.i$[new_elem].width + x] = strtoul(hexval, 0, 16);
				}
			}
		}

		// Unknown line type
		else {
			MessageBox(pMyMainWnd->GetSafeHwnd(), strLine, "Unknown Line Type", MB_OK);
			return ERR_UNK_LN_TYPE;
		}

		linenum++;
	}
	while(ar.ReadString(strLine));

	// Populate t$ hexmash char array for bsearches later on
	P = (int) tablemap.t$.GetSize();
	for (j=0; j<P; j++) {
		strcpy(tablemap.hexmash[j], tablemap.t$[j].hexmash);
	}

	// Populate h$ hashes array for bsearches later on
	P = (int) tablemap.h$.GetSize();
	for (j=0; j<P; j++) {
		tablemap.hashes[j] = tablemap.h$[j].hash;
	}

	return SUCCESS;
}

int COpenScrapeDoc::SaveTableMap(CArchive& ar)
{
	CString s, t, text;
	int N, i, j, x, y;
	char nowtime[26];

	// Version
	s.Format("%s\r\n", VER_OPENSCRAPE_1); ar.WriteString(s);
	ar.WriteString("\r\n");

	// Comment header
	s.Format("// OpenScrape %s\r\n", VERSION_TEXT); ar.WriteString(s);
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
	N = (int) tablemap.z$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("z$%-16s %d  %d\r\n", tablemap.z$[i].name, tablemap.z$[i].width, tablemap.z$[i].height); 
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// strings
	ar.WriteString("//\r\n");
	ar.WriteString("// strings\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) tablemap.s$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("s$%s", tablemap.s$[i].name.GetString());
		while (s.GetLength()<18) s.Append(" ");
		s.Append(tablemap.s$[i].text);
		s.Append("\r\n");
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// regions
	ar.WriteString("//\r\n");
	ar.WriteString("// regions\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) tablemap.r$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("r$%s", tablemap.r$[i].name.GetString());
		while (s.GetLength()<18) s.Append(" ");
		t.Format("%3d %3d %3d %3d %8x %4d ", 
			tablemap.r$[i].left, tablemap.r$[i].top, tablemap.r$[i].right, tablemap.r$[i].bottom,
			tablemap.r$[i].color, tablemap.r$[i].radius);
		s.Append(t);
		t = tablemap.r$[i].transform;
		while (t.GetLength()<2) { t.Append(" "); }
		s.Append(t);
		//t.Format("          %d\r\n", tablemap.r$[i].flags);
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
	N = (int) tablemap.t$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("t%d$%c", tablemap.t$[i].group, tablemap.t$[i].ch);
		for (j=0; j<tablemap.t$[i].x_count; j++) { 
			t.Format(" %x", tablemap.t$[i].x[j]);
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
	N = (int) tablemap.p$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("p%d$%4d %4d\r\n", tablemap.p$[i].number, tablemap.p$[i].x, tablemap.p$[i].y);  ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// hash
	ar.WriteString("//\r\n");
	ar.WriteString("// hash\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) tablemap.h$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("h%d$%s", tablemap.h$[i].number, tablemap.h$[i].name.GetString());
		while (s.GetLength()<19) { s.Append(" "); }
		t.Format("%08x\r\n", tablemap.h$[i].hash);
		s.Append(t);
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// images
	ar.WriteString("//\r\n");
	ar.WriteString("// images\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) tablemap.i$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("i$%-16s %-3d %-3d\n", tablemap.i$[i].name, tablemap.i$[i].width, tablemap.i$[i].height);
		ar.WriteString(s);
		for (y=0; y<(int) tablemap.i$[i].height; y++)
		{
			s = "";
			for (x=0; x<(int) tablemap.i$[i].width; x++)
			{
				text.Format("%08x", tablemap.i$[i].pixel[y*tablemap.i$[i].width + x]);
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


int COpenScrapeDoc::ConvertTableMap(void) {
	CMainFrame				*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	Stablemap_hash_value	hold_hash_value;
	int						N, imageloop, hashloop, regionloop, j, insert_point, hash_type;
	CString					e, s;
	uint32_t				pixels[200*150];
	int						pixcount;
	bool					all_i$_found, this_i$_found;
	FILE					*fp;
	Stablemap_hash_value	h$_record;
	char					timebuf[30];
	CString					logpath;
	uint32_t				hash_seed;
	bool					warning_displayed;
	CArray <Stablemap_hash_value, Stablemap_hash_value>	new_hashes;
	CArray <Stablemap_hash_value, Stablemap_hash_value>	unmatched_h$_records;

	// Get number of records of each type in this table map
	if ((int) tablemap.i$.GetSize()==0) {
		MessageBox(pMyMainWnd->GetSafeHwnd(), "No i$ records found - is this a master?", "'Profile' Error", MB_OK);
		return ERR_NOTMASTER;
	}

	// Loop through all the region (r$) records, and invert the colors to align with
	// Windows' COLORREF  ARGB->ABGR
	for (regionloop=0; regionloop<(int) tablemap.r$.GetSize(); regionloop++) {
		tablemap.r$[regionloop].color = 
			(tablemap.r$[regionloop].color & 0xff000000) +
			((tablemap.r$[regionloop].color & 0x00ff0000)>>16) +
			((tablemap.r$[regionloop].color & 0x0000ff00)) +
			((tablemap.r$[regionloop].color & 0x000000ff)<<16);
	}

	// Loop through all the region (r$) records, and look for i3slider or i3handle
	// Warn if these are found and remove them
	warning_displayed = false;
	for (regionloop=0; regionloop<(int) tablemap.r$.GetSize(); regionloop++) {
		if (tablemap.r$[regionloop].name == "i3slider" ||
			tablemap.r$[regionloop].name == "i3handle")
		{
			if (!warning_displayed)
			{
				MessageBox(pMyMainWnd->GetSafeHwnd(), "This WinScrape 'profile' contains either a 'r$i3slider'\n"\
													  "or 'r$i3handle' region definition.  OpenScrape/OpenHoldem\n"\
													  "do not make use of these region types and they will not be\n"\
													  "saved in the OpenScrape table map format.", "'Profile' Warning", MB_OK);
				warning_displayed = true;
			}

			tablemap.r$.RemoveAt(regionloop);
		}
	}

	// Loop through all the hash (h$) records, and check for a corresponding image (i$) record
	all_i$_found = true;
	unmatched_h$_records.RemoveAll();

	for (hashloop=0; hashloop<(int) tablemap.h$.GetSize(); hashloop++) {
		this_i$_found = false;
		for (imageloop=0; imageloop<(int) tablemap.i$.GetSize(); imageloop++) {
			if (tablemap.i$[imageloop].name == tablemap.h$[hashloop].name) {
				this_i$_found = true;
				imageloop = (int) tablemap.i$.GetSize() + 1;
			}
		}
		if (!this_i$_found) {
			all_i$_found = false;
			h$_record.name = tablemap.h$[hashloop].name;
			h$_record.hash = tablemap.h$[hashloop].hash;
			h$_record.number = tablemap.h$[hashloop].number;
			unmatched_h$_records.Add(h$_record);
		}

	}

	if (!all_i$_found) {
		logpath.Format("%s\\tablemap conversion log.txt", startup_path);
		fp = fopen(logpath.GetString(), "a");
		get_now_time(timebuf);
		fprintf(fp, "<%s>\nConverting from: %s\n", timebuf, tablemap.filepath);
		fprintf(fp, "h$ records with no matching i$ record:\n");
		for (j=0; j<unmatched_h$_records.GetCount(); j++) {
			fprintf(fp, "\t%3d. h$%s\n", j+1, unmatched_h$_records[j].name.GetString());
		}
		fprintf(fp, "=======================================================\n\n");
		MessageBox(pMyMainWnd->GetSafeHwnd(), 
				   "Could not complete conversion of this table map, due to missing Image\n"\
				   "records. Please see the \"tablemap conversion log.txt\" file for details.\n\n"\
				   "The Image records will need to be recreated and the Hash records\n"\
				   "will need to be updated (Edit/Update Hashes) before this table map\n"\
				   "can be used in OpenHoldem.", 
				   "Conversion Error", MB_OK);
		fclose(fp);
		return ERR_INCOMPLETEMASTER;
	}

	// Init new hash array
	new_hashes.RemoveAll();

	// Loop through each of the image records and create hashes
	for (imageloop=0; imageloop<(int) tablemap.i$.GetSize(); imageloop++) {
		// Loop through the h$ records to find all the hashes that we have to create for this image record
		for (hashloop=0; hashloop<(int) tablemap.h$.GetSize(); hashloop++) {
			if (tablemap.i$[imageloop].name == tablemap.h$[hashloop].name) {
				
				hash_type = tablemap.h$[hashloop].number;

				// create hash, based on type
				hold_hash_value.name = tablemap.i$[imageloop].name;
				hold_hash_value.number = hash_type;

				// Get count of pixels (p$ records)
				pixcount = 0;
				for (j=0; j<(int) tablemap.p$.GetSize(); j++) {
					if (tablemap.p$[j].number == hash_type &&
						tablemap.p$[j].x <= tablemap.i$[imageloop].width &&
						tablemap.p$[j].y <= tablemap.i$[imageloop].height) {
							pixels[pixcount++] = 
								tablemap.i$[imageloop].pixel[tablemap.p$[j].y * tablemap.i$[imageloop].width +
															 tablemap.p$[j].x];				
					}
				}

				if (hash_type>=0 && hash_type<=3) {
				
					hash_seed = hash_type==0 ? HASH_SEED_0 :
								hash_type==1 ? HASH_SEED_1 :
								hash_type==2 ? HASH_SEED_2 :
								hash_type==3 ? HASH_SEED_3 : 0;

					// Create hash
					if (pixcount==0) {
						hold_hash_value.hash = hashword(&tablemap.i$[imageloop].pixel[0], 
														tablemap.i$[imageloop].width * tablemap.i$[imageloop].height,
														hash_seed);
					}
					else {
						hold_hash_value.hash = hashword(&pixels[0], pixcount, hash_seed);
					}
				}

				// bad hash type
				else {
					e.Format("Invalid hash type (%d) for image record <%s>", hash_type, tablemap.i$[imageloop].name.GetString()); 
					MessageBox(pMyMainWnd->GetSafeHwnd(), e, "Invalid hash type", MB_OK);
					return ERR_INV_HASH_TYPE;
				}

				// Check for hash collision
				N = (int) new_hashes.GetSize();
				for (j=0; j<N; j++) {
					if (hold_hash_value.hash == new_hashes[j].hash) {
						e.Format("Hash collision:\n    <%s> and <%s>\n\nTalk to an OpenHoldem developer, please.\n%s %08x %08x", 
							tablemap.i$[imageloop].name.GetString(), 
							new_hashes[j].name.GetString(),
							tablemap.h$[hashloop].name,
							hold_hash_value.hash, new_hashes[j].hash
							); 
						MessageBox(pMyMainWnd->GetSafeHwnd(), e, "Hash collision", MB_OK);
						return ERR_HASH_COLL;
					}
				}

				// Find the right spot to insert this hash record (so list is sorted at the end)
				N = (int) new_hashes.GetSize();
				insert_point = -1;
				for (j=0; j<N; j++) {
					if (hold_hash_value.hash < new_hashes[j].hash) {
						insert_point = j;
						j=N+1;
					}
				}
				if (insert_point==-1) {
					new_hashes.Add(hold_hash_value);
				}
				else {
					new_hashes.InsertAt(insert_point, hold_hash_value);
				}
			}
		}  // for (hashloop=0; hashloop<num_hrecs; hashloop++)
	} // for (imageloop=0; imageloop<num_irecs; imageloop++)


	// Kill the existing h$ records, and replace with new_hashes records
	tablemap.h$.RemoveAll();
	N = (int) new_hashes.GetSize();
	for (j=0; j<N; j++) {
		hold_hash_value.name = new_hashes[j].name;
		hold_hash_value.number = new_hashes[j].number;
		hold_hash_value.hash = new_hashes[j].hash;
		tablemap.h$.Add(hold_hash_value);
	}

	return SUCCESS;
}

int COpenScrapeDoc::UpdateHashes(void)
{
	CMainFrame				*pMyMainWnd  = (CMainFrame *) (theApp.m_pMainWnd);
	Stablemap_hash_value	hold_hash_value;
	int						num_irecs, num_hrecs, num_precs, num_rrecs, N;
	int						imageloop, hashloop, j, insert_point, hash_type;
	CString					e, s;
	uint32_t				pixels[MAX_HASH_WIDTH*MAX_HASH_HEIGHT], filtered_pix[MAX_HASH_WIDTH*MAX_HASH_HEIGHT];;
	int						pixcount;
	bool					all_i$_found, this_i$_found;
	FILE					*fp;
	Stablemap_hash_value	h$_record;
	char					timebuf[30];
	CString					logpath;

	CArray <Stablemap_hash_value, Stablemap_hash_value>	new_hashes;
	CArray <Stablemap_hash_value, Stablemap_hash_value>	unmatched_h$_records;

	// Get number of records of each type in this table map
	num_irecs = (int) tablemap.i$.GetSize();
	if (num_irecs==0) {
		MessageBox(pMyMainWnd->GetSafeHwnd(), "No images found - cannot create hashes.", "Table Map Error", MB_OK);
		return ERR_NOTMASTER;
	}
	num_hrecs = (int) tablemap.h$.GetSize();
	num_precs = (int) tablemap.p$.GetSize();
	num_rrecs = (int) tablemap.r$.GetSize();

	// Loop through all the hash (h$) records, and check for a corresponding image (i$) record
	all_i$_found = true;
	unmatched_h$_records.RemoveAll();

	for (hashloop=0; hashloop<num_hrecs; hashloop++) {
		this_i$_found = false;
		for (imageloop=0; imageloop<num_irecs; imageloop++) {
			if (tablemap.i$[imageloop].name == tablemap.h$[hashloop].name) {
				this_i$_found = true;
				imageloop = num_irecs + 1;
			}
		}
		if (!this_i$_found) {
			all_i$_found = false;
			h$_record.name = tablemap.h$[hashloop].name;
			h$_record.hash = tablemap.h$[hashloop].hash;
			h$_record.number = tablemap.h$[hashloop].number;
			unmatched_h$_records.Add(h$_record);
		}

	}

	if (!all_i$_found) {
		logpath.Format("%s\\hash creation log.txt", startup_path);
		fp = fopen(logpath.GetString(), "a");
		get_now_time(timebuf);
		fprintf(fp, "<%s>\nCreating hashes\n", timebuf);
		fprintf(fp, "Hashes with no matching image:\n");
		for (j=0; j<unmatched_h$_records.GetCount(); j++) {
			fprintf(fp, "\t%3d. h$%s\n", j+1, unmatched_h$_records[j].name.GetString());
		}
		fprintf(fp, "=======================================================\n\n");
		MessageBox(pMyMainWnd->GetSafeHwnd(), 
				   "Could not complete hash creation, due to missing images.\n"\
				   "Please see the \"hash creation log.txt\" file for details.", 
				   "Hash Creation Error", MB_OK);
		fclose(fp);
	}

	// Init new hash array
	new_hashes.RemoveAll();

	// Loop through each of the image records and create hashes
	for (imageloop=0; imageloop<num_irecs; imageloop++) {
		// Loop through the h$ records to find all the hashes that we have to create for this image record
		for (hashloop=0; hashloop<num_hrecs; hashloop++) {
			if (tablemap.i$[imageloop].name == tablemap.h$[hashloop].name) {
				
				hash_type = tablemap.h$[hashloop].number;

				// create hash, based on type
				hold_hash_value.name = tablemap.i$[imageloop].name;
				hold_hash_value.number = hash_type;

				// all pixel hash
				if (hash_type == 0) 
				{

					// only create hash based on rgb values - ignore alpha
					for (j=0; j < (int) (tablemap.i$[imageloop].width * tablemap.i$[imageloop].height); j++)
						filtered_pix[j] = tablemap.i$[imageloop].pixel[j] & 0x00ffffff;

					hold_hash_value.hash = hashword(&filtered_pix[0], tablemap.i$[imageloop].width * tablemap.i$[imageloop].height, HASH_SEED_0);
				}

				// selected pixel hash
				else if (hash_type>=1 && hash_type<=3) 
				{
					pixcount = 0;
					for (j=0; j<num_precs; j++) {
						if (tablemap.p$[j].number == hash_type &&
							tablemap.p$[j].x <= tablemap.i$[imageloop].width &&
							tablemap.p$[j].y <= tablemap.i$[imageloop].height) 
						{
								// only create hash based on rgb values - ignore alpha
								pixels[pixcount++] = 
									tablemap.i$[imageloop].pixel[tablemap.p$[j].y * tablemap.i$[imageloop].width + tablemap.p$[j].x] & 0x00ffffff;
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
					e.Format("Invalid hash type (%d) for image record <%s>", hash_type, tablemap.i$[imageloop].name.GetString()); 
					MessageBox(pMyMainWnd->GetSafeHwnd(), e, "Invalid hash type", MB_OK);
					return ERR_INV_HASH_TYPE;
				}

				// Check for hash collision
				N = (int) new_hashes.GetSize();
				for (j=0; j<N; j++) {
					if (hold_hash_value.hash == new_hashes[j].hash) {
						e.Format("Hash collision:\n    <%s> and <%s>\n\nTalk to an OpenHoldem developer, please ;-)", 
							tablemap.i$[imageloop].name.GetString(), 
							new_hashes[j].name.GetString()); 
						MessageBox(pMyMainWnd->GetSafeHwnd(), e, "Hash collision", MB_OK);
						return ERR_HASH_COLL;
					}
				}

				// Find the right spot to insert this hash record (so list is sorted at the end)
				N = (int) new_hashes.GetSize();
				insert_point = -1;
				for (j=0; j<N; j++) {
					if (hold_hash_value.hash < new_hashes[j].hash) {
						insert_point = j;
						j=N+1;
					}
				}
				if (insert_point==-1) {
					new_hashes.Add(hold_hash_value);
				}
				else {
					new_hashes.InsertAt(insert_point, hold_hash_value);
				}
			}
		}  // for (hashloop=0; hashloop<num_hrecs; hashloop++)
	} // for (imageloop=0; imageloop<num_irecs; imageloop++)


	// Kill the existing h$ records, and replace with new_hashes records
	tablemap.h$.RemoveAll();
	N = (int) new_hashes.GetSize();
	for (j=0; j<N; j++) {
		hold_hash_value.name = new_hashes[j].name;
		hold_hash_value.number = new_hashes[j].number;
		hold_hash_value.hash = new_hashes[j].hash;
		tablemap.h$.Add(hold_hash_value);
	}

	// Redraw the tree
	theApp.m_TableMapDlg->update_tree("");

	if (!all_i$_found)  return ERR_INCOMPLETEMASTER;

	return SUCCESS;
}

// COpenScrapeDoc diagnostics

#ifdef _DEBUG
void COpenScrapeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COpenScrapeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COpenScrapeDoc commands

