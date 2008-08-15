#include "StdAfx.h"
#include <Math.h>
#include "Transform.h"
#include "hash/lookup3.h"
#include "pdiff/CompareArgs.h"
#include "pdiff/Metric.h"

#ifdef OPENHOLDEM_PROGRAM
#include "../OpenHoldem/debug.h"
#else
#include "../OpenScrape/debug.h"
#endif

CTransform::CTransform(void)
{
    __SEH_SET_EXCEPTION_HANDLER
}

CTransform::~CTransform(void)
{
}

void CTransform::clear_tablemap()
{
    __SEH_HEADER

    map.valid = false;
    map.filepath = "";
	map.filename = "";

    map.z$.RemoveAll();
    map.s$.RemoveAll();
    map.r$.RemoveAll();
    map.t$.RemoveAll();
    map.p$.RemoveAll();
    map.h$.RemoveAll();
    map.i$.RemoveAll();

    __SEH_LOGFATAL("CTransform::clear_tablemap :\n");
}

int CTransform::load_tablemap(char *filename, char *version, bool check_ws_date, int *linenum, CString *loaded_version) 
{
    __SEH_HEADER

	CString				strLine, strLineType, token, s, e, hexval, t;
    int					i, pos, insert_point, P, j, x, y, new_elem;
    bool				supported_version;
	BYTE				alpha, red, green, blue;

    // temp
    Stablemap_size			hold_size;
    Stablemap_symbol		hold_symbol;
    Stablemap_region		hold_region;
    Stablemap_font			hold_font;
    Stablemap_hash_point	hold_hash_point;
    Stablemap_hash_value	hold_hash_value;
    Stablemap_image			hold_image;

    // Clean up the global.profile structure
    clear_tablemap();

    // Open the selected file
    CFile cfFile(filename, CFile::modeRead | CFile::shareDenyNone);
    map.filepath = filename;

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

            map.z$.Add(hold_size);
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

            // Skip s$hXtype lines
            if (strLineType.Left(3) == "s$h" && strLineType.Mid(4,4) == "type")
            {
            }
            else
            {
                map.s$.Add(hold_symbol);
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

            map.r$.Add(hold_region);
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

            i = 0;
            hold_font.hexmash = "";
            while ((token = strLine.Tokenize(" \t", pos))!="" && i<=30 && pos!=-1) 
			{
                hold_font.x[i++] = strtoul(token, 0, 16);
                hold_font.hexmash.Append(token);
            }
            hold_font.x_count = i;

            // Insert into array so it is sorted alphabetically
            P = (int) map.t$.GetSize();
            insert_point = -1;
            for (j=0; j<P; j++) 
			{
                if (hold_font.hexmash < map.t$[j].hexmash)
                {
                    insert_point = j;
                    j=P+1;
                }
            }
            if (insert_point==-1)
                map.t$.Add(hold_font);

			else
                map.t$.InsertAt(insert_point, hold_font);
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
            P = (int) map.p$.GetSize();
            insert_point = -1;
            for (j=0; j<P; j++) 
			{
                if ( hold_hash_point.number == map.p$[j].number &&
                        ((hold_hash_point.x == map.p$[j].x && hold_hash_point.y < map.p$[j].y) ||
                         (hold_hash_point.x < map.p$[j].x)) )
                {
                    insert_point = j;
                    j=P+1;
                }
            }
            if (insert_point==-1)
                map.p$.Add(hold_hash_point);

			else
                map.p$.InsertAt(insert_point, hold_hash_point);
        }

        // Handle h$ lines (hash values)
        else if (strLineType.Left(1) == "h" &&
                 strLineType.Mid(1,1) >= "0" &&
                 strLineType.Mid(1,1) <= "9" &&
                 strLineType.Mid(2,1) == "$") 
		{
            // number
            hold_hash_value.number = strLineType.GetString()[1] - '0';

            // name
            hold_hash_value.name = strLineType.Mid(3);

            // value
            token = strLine.Tokenize(" \t", pos);
            if (token.GetLength()==0)
                return ERR_SYNTAX;

			hold_hash_value.hash = strtoul(token.GetString(), 0, 16);

            // Insert into array so it is sorted
            P = (int) map.h$.GetSize();
            insert_point = -1;
            for (j=0; j<P; j++) 
			{
                if (hold_hash_value.hash < map.h$[j].hash)
                {
                    insert_point = j;
                    j=P+1;
                }
            }
            if (insert_point==-1)
                map.h$.Add(hold_hash_value);

			else
                map.h$.InsertAt(insert_point, hold_hash_value);
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
            new_elem = (int) map.i$.Add(hold_image);

			// Allocate space for "RGBAImage"
			t = map.i$[new_elem].name + ".ppm";
			map.i$[new_elem].image = new RGBAImage(map.i$[new_elem].width, map.i$[new_elem].height, t.GetString());

            // read next "height" lines
            for (y=0; y < (int) map.i$[new_elem].height; y++) 
			{
                (*linenum)++;
                if (!ar.ReadString(strLine)) 
				{
                    return ERR_SYNTAX;
                }
                // scan across "width" of line to get values
                for (x=0; x < (int) map.i$[new_elem].width; x++) 
				{
                    // unreverse bgra to abgr
                    hexval = strLine.Mid(x*8+6, 2) + strLine.Mid(x*8, 6);
                    map.i$[new_elem].pixel[y*map.i$[new_elem].width + x] = strtoul(hexval, 0, 16);
					alpha = (map.i$[new_elem].pixel[y*map.i$[new_elem].width + x] >> 24) &0xff;
					blue =  (map.i$[new_elem].pixel[y*map.i$[new_elem].width + x] >> 16) &0xff;
					green = (map.i$[new_elem].pixel[y*map.i$[new_elem].width + x] >>  8) &0xff;
					red =   (map.i$[new_elem].pixel[y*map.i$[new_elem].width + x] >>  0) &0xff;
					map.i$[new_elem].image->Set(red, green, blue, alpha, y*map.i$[new_elem].width + x);
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

    // Populate t$ hexmash char array for bsearches later on
    P = (int) map.t$.GetSize();
    for (j=0; j<P; j++)
        strcpy(map.hexmash[j], map.t$[j].hexmash);

    // Populate h$ hashes array for bsearches later on
    P = (int) map.h$.GetSize();
    for (j=0; j<P; j++)
        map.hashes[j] = map.h$[j].hash;

    map.valid = true;
    return SUCCESS;

    __SEH_LOGFATAL("CTransform::load_tablemap :\n");

}

int CTransform::save_tablemap(CArchive& ar, char *version_text)
{
    __SEH_HEADER

	CString s, t, text;
	int N, i, j, x, y;
	char nowtime[26];

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
	N = (int) map.z$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("z$%-16s %d  %d\r\n", map.z$[i].name, map.z$[i].width, map.z$[i].height); 
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// strings
	ar.WriteString("//\r\n");
	ar.WriteString("// strings\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) map.s$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("s$%s", map.s$[i].name.GetString());
		while (s.GetLength()<18) s.Append(" ");
		s.Append(map.s$[i].text);
		s.Append("\r\n");
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// regions
	ar.WriteString("//\r\n");
	ar.WriteString("// regions\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) map.r$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("r$%s", map.r$[i].name.GetString());
		while (s.GetLength()<18) s.Append(" ");
		t.Format("%3d %3d %3d %3d %8x %4d ", 
			map.r$[i].left, map.r$[i].top, map.r$[i].right, map.r$[i].bottom,
			map.r$[i].color, map.r$[i].radius);
		s.Append(t);
		t = map.r$[i].transform;
		while (t.GetLength()<2) { t.Append(" "); }
		s.Append(t);
		//t.Format("          %d\r\n", map.r$[i].flags);
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
	N = (int) map.t$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("t%d$%c", map.t$[i].group, map.t$[i].ch);
		for (j=0; j<map.t$[i].x_count; j++) { 
			t.Format(" %x", map.t$[i].x[j]);
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
	N = (int) map.p$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("p%d$%4d %4d\r\n", map.p$[i].number, map.p$[i].x, map.p$[i].y);  ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// hash
	ar.WriteString("//\r\n");
	ar.WriteString("// hash\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) map.h$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("h%d$%s", map.h$[i].number, map.h$[i].name.GetString());
		while (s.GetLength()<19) { s.Append(" "); }
		t.Format("%08x\r\n", map.h$[i].hash);
		s.Append(t);
		ar.WriteString(s);
	}
	ar.WriteString("\r\n");

	// images
	ar.WriteString("//\r\n");
	ar.WriteString("// images\r\n");
	ar.WriteString("//\r\n");
	ar.WriteString("\r\n");
	N = (int) map.i$.GetSize();
	for (i=0; i<N; i++) {
		s.Format("i$%-16s %-3d %-3d\n", map.i$[i].name, map.i$[i].width, map.i$[i].height);
		ar.WriteString(s);
		for (y=0; y<(int) map.i$[i].height; y++)
		{
			s = "";
			for (x=0; x<(int) map.i$[i].width; x++)
			{
				text.Format("%08x", map.i$[i].pixel[y*map.i$[i].width + x]);
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

    __SEH_LOGFATAL("CTransform::save_tablemap :\n");
}


int CTransform::convert_tablemap(HWND hwnd, char *startup_path) 
{
    __SEH_HEADER

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
	CArray <Stablemap_hash_value, Stablemap_hash_value>	new_hashes;
	CArray <Stablemap_hash_value, Stablemap_hash_value>	unmatched_h$_records;

	// Get number of records of each type in this table map
	if ((int) map.i$.GetSize()==0) 
	{
		MessageBox(hwnd, "No i$ records found - is this a master?", "'Profile' Error", MB_OK);
		return ERR_NOTMASTER;
	}

	// Loop through all the region (r$) records, and invert the colors to align with
	// Windows' COLORREF  ARGB->ABGR
	for (regionloop=0; regionloop<(int) map.r$.GetSize(); regionloop++) 
	{
		map.r$[regionloop].color = 
			(map.r$[regionloop].color & 0xff000000) +
			((map.r$[regionloop].color & 0x00ff0000)>>16) +
			((map.r$[regionloop].color & 0x0000ff00)) +
			((map.r$[regionloop].color & 0x000000ff)<<16);
	}

	// Loop through all the region (r$) records, and look for i3slider or i3handle
	// Warn if these are found and remove them
	//warning_displayed = false;
	//for (regionloop=0; regionloop<(int) map.r$.GetSize(); regionloop++) {
	//	if (map.r$[regionloop].name == "i3slider" ||
	//		map.r$[regionloop].name == "i3handle")
	//	{
	//		if (!warning_displayed)
	//		{
	//			MessageBox(pMyMainWnd->GetSafeHwnd(), "This WinScrape 'profile' contains either a 'r$i3slider'\n"\
	//												  "or 'r$i3handle' region definition.  OpenScrape/OpenHoldem\n"\
	//												  "do not make use of these region types and they will not be\n"\
	//												  "saved in the OpenScrape table map format.", "'Profile' Warning", MB_OK);
	//			warning_displayed = true;
	//		}

	//		map.r$.RemoveAt(regionloop);
	//	}
	//}

	// Loop through all the hash (h$) records, and check for a corresponding image (i$) record
	all_i$_found = true;
	unmatched_h$_records.RemoveAll();

	for (hashloop=0; hashloop<(int) map.h$.GetSize(); hashloop++) 
	{
		this_i$_found = false;
		for (imageloop=0; imageloop<(int) map.i$.GetSize(); imageloop++) 
		{
			if (map.i$[imageloop].name == map.h$[hashloop].name) 
			{
				this_i$_found = true;
				imageloop = (int) map.i$.GetSize() + 1;
			}
		}
		if (!this_i$_found) 
		{
			all_i$_found = false;
			h$_record.name = map.h$[hashloop].name;
			h$_record.hash = map.h$[hashloop].hash;
			h$_record.number = map.h$[hashloop].number;
			unmatched_h$_records.Add(h$_record);
		}

	}

	if (!all_i$_found) 
	{
		logpath.Format("%s\\tablemap conversion log.txt", startup_path);
		fp = fopen(logpath.GetString(), "a");
		get_now_time(timebuf);
		fprintf(fp, "<%s>\nConverting from: %s\n", timebuf, map.filepath);
		fprintf(fp, "h$ records with no matching i$ record:\n");

		for (j=0; j<unmatched_h$_records.GetCount(); j++)
			fprintf(fp, "\t%3d. h$%s\n", j+1, unmatched_h$_records[j].name.GetString());

		fprintf(fp, "=======================================================\n\n");
		MessageBox(hwnd, 
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
	for (imageloop=0; imageloop<(int) map.i$.GetSize(); imageloop++) 
	{
		// Loop through the h$ records to find all the hashes that we have to create for this image record
		for (hashloop=0; hashloop<(int) map.h$.GetSize(); hashloop++) 
		{
			if (map.i$[imageloop].name == map.h$[hashloop].name) 
			{
				
				hash_type = map.h$[hashloop].number;

				// create hash, based on type
				hold_hash_value.name = map.i$[imageloop].name;
				hold_hash_value.number = hash_type;

				// Get count of pixels (p$ records)
				pixcount = 0;
				for (j=0; j<(int) map.p$.GetSize(); j++) 
				{
					if (map.p$[j].number == hash_type &&
						map.p$[j].x <= map.i$[imageloop].width &&
						map.p$[j].y <= map.i$[imageloop].height) 
					{
							pixels[pixcount++] = 
								map.i$[imageloop].pixel[map.p$[j].y * map.i$[imageloop].width +
															 map.p$[j].x];				
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
						hold_hash_value.hash = hashword(&map.i$[imageloop].pixel[0], 
														map.i$[imageloop].width * map.i$[imageloop].height,
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
					e.Format("Invalid hash type (%d) for image record <%s>", hash_type, map.i$[imageloop].name.GetString()); 
					MessageBox(hwnd, e, "Invalid hash type", MB_OK);
					return ERR_INV_HASH_TYPE;
				}

				// Check for hash collision
				N = (int) new_hashes.GetSize();
				for (j=0; j<N; j++) {
					if (hold_hash_value.hash == new_hashes[j].hash) 
					{
						e.Format("Hash collision:\n    <%s> and <%s>\n\nTalk to an OpenHoldem developer, please.\n%s %08x %08x", 
							map.i$[imageloop].name.GetString(), 
							new_hashes[j].name.GetString(),
							map.h$[hashloop].name,
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


	// Kill the existing h$ records, and replace with new_hashes records
	map.h$.RemoveAll();
	N = (int) new_hashes.GetSize();
	for (j=0; j<N; j++) 
	{
		hold_hash_value.name = new_hashes[j].name;
		hold_hash_value.number = new_hashes[j].number;
		hold_hash_value.hash = new_hashes[j].hash;
		map.h$.Add(hold_hash_value);
	}

	return SUCCESS;

    __SEH_LOGFATAL("CTransform::convert_tablemap :\n");
}

int CTransform::update_hashes(HWND hwnd, char *startup_path)
{
    __SEH_HEADER

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
	num_irecs = (int) map.i$.GetSize();
	if (num_irecs==0) 
	{
		MessageBox(hwnd, "No images found - cannot create hashes.", "Table Map Error", MB_OK);
		return ERR_NOTMASTER;
	}

	num_hrecs = (int) map.h$.GetSize();
	num_precs = (int) map.p$.GetSize();
	num_rrecs = (int) map.r$.GetSize();

	// Loop through all the hash (h$) records, and check for a corresponding image (i$) record
	all_i$_found = true;
	unmatched_h$_records.RemoveAll();

	for (hashloop=0; hashloop<num_hrecs; hashloop++) 
	{
		this_i$_found = false;
		for (imageloop=0; imageloop<num_irecs; imageloop++) 
		{
			if (map.i$[imageloop].name == map.h$[hashloop].name) 
			{
				this_i$_found = true;
				imageloop = num_irecs + 1;
			}
		}
		if (!this_i$_found) 
		{
			all_i$_found = false;
			h$_record.name = map.h$[hashloop].name;
			h$_record.hash = map.h$[hashloop].hash;
			h$_record.number = map.h$[hashloop].number;
			unmatched_h$_records.Add(h$_record);
		}

	}

	if (!all_i$_found) 
	{
		logpath.Format("%s\\hash creation log.txt", startup_path);
		fp = fopen(logpath.GetString(), "a");
		get_now_time(timebuf);
		fprintf(fp, "<%s>\nCreating hashes\n", timebuf);
		fprintf(fp, "Hashes with no matching image:\n");

		for (j=0; j<unmatched_h$_records.GetCount(); j++) 
			fprintf(fp, "\t%3d. h$%s\n", j+1, unmatched_h$_records[j].name.GetString());

		fprintf(fp, "=======================================================\n\n");
		MessageBox(hwnd, 
				   "Could not complete hash creation, due to missing images.\n"\
				   "Please see the \"hash creation log.txt\" file for details.", 
				   "Hash Creation Error", MB_OK);
		fclose(fp);
	}

	// Init new hash array
	new_hashes.RemoveAll();

	// Loop through each of the image records and create hashes
	for (imageloop=0; imageloop<num_irecs; imageloop++) 
	{
		// Loop through the h$ records to find all the hashes that we have to create for this image record
		for (hashloop=0; hashloop<num_hrecs; hashloop++) 
		{
			if (map.i$[imageloop].name == map.h$[hashloop].name) 
			{
				
				hash_type = map.h$[hashloop].number;

				// create hash, based on type
				hold_hash_value.name = map.i$[imageloop].name;
				hold_hash_value.number = hash_type;

				// all pixel hash
				if (hash_type == 0) 
				{

					// only create hash based on rgb values - ignore alpha
					for (j=0; j < (int) (map.i$[imageloop].width * map.i$[imageloop].height); j++)
						filtered_pix[j] = map.i$[imageloop].pixel[j] & 0x00ffffff;

					hold_hash_value.hash = hashword(&filtered_pix[0], map.i$[imageloop].width * map.i$[imageloop].height, HASH_SEED_0);
				}

				// selected pixel hash
				else if (hash_type>=1 && hash_type<=3) 
				{
					pixcount = 0;
					for (j=0; j<num_precs; j++) 
					{
						if (map.p$[j].number == hash_type &&
							map.p$[j].x <= map.i$[imageloop].width &&
							map.p$[j].y <= map.i$[imageloop].height) 
						{
								// only create hash based on rgb values - ignore alpha
								pixels[pixcount++] = 
									map.i$[imageloop].pixel[map.p$[j].y * map.i$[imageloop].width + map.p$[j].x] & 0x00ffffff;
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
					e.Format("Invalid hash type (%d) for image record <%s>", hash_type, map.i$[imageloop].name.GetString()); 
					MessageBox(hwnd, e, "Invalid hash type", MB_OK);
					return ERR_INV_HASH_TYPE;
				}

				// Check for hash collision
				N = (int) new_hashes.GetSize();
				for (j=0; j<N; j++) 
				{
					if (hold_hash_value.hash == new_hashes[j].hash) 
					{
						e.Format("Hash collision:\n    <%s> and <%s>\n\nTalk to an OpenHoldem developer, please ;-)", 
							map.i$[imageloop].name.GetString(), 
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


	// Kill the existing h$ records, and replace with new_hashes records
	map.h$.RemoveAll();
	N = (int) new_hashes.GetSize();
	for (j=0; j<N; j++) 
	{
		hold_hash_value.name = new_hashes[j].name;
		hold_hash_value.number = new_hashes[j].number;
		hold_hash_value.hash = new_hashes[j].hash;
		map.h$.Add(hold_hash_value);
	}

	if (!all_i$_found)  return ERR_INCOMPLETEMASTER;

	return SUCCESS;

    __SEH_LOGFATAL("CTransform::update_hashes :\n");
}

int CTransform::do_transform(Stablemap_region *region, HDC hdc, CString *text, CString *separation, COLORREF *cr_avg) 
{
    __SEH_HEADER

	bool				character[MAX_CHAR_WIDTH][MAX_CHAR_HEIGHT];	
	bool				background[MAX_CHAR_WIDTH];

	switch (region->transform.GetString()[0]) 
	{
		case 'C':
			return c_transform(region, hdc, text, cr_avg);
			break;
		
		case 'I':
			return i_transform(region, hdc, text);
			break;
		
		case 'H':
			return h_transform(region, hdc, text);
			break;

		case 'T':
			return t_transform(region, hdc, text, separation, background, character);
			break;

		case 'N':
			*text = "";
			return ERR_GOOD_SCRAPE_GENERAL;
			break;
	}

	return ERR_INVALID_TRANSFORM_TYPE;

    __SEH_LOGFATAL("CTransform::do_transform :\n");
}

int CTransform::c_transform(Stablemap_region *region, HDC hdc, CString *text, COLORREF *cr_avg) 
{
    __SEH_HEADER

	int					x, y;
	int					width, height;
	int					rr_sum, gg_sum, bb_sum, pix_cnt;
	double				rr_avg, gg_avg, bb_avg;
	HBITMAP				hbm;
	BYTE				*pBits, red, green, blue;

	// Allocate heap space for BITMAPINFO
	BITMAPINFO	*bmi;
	int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
	bmi = (BITMAPINFO *) ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

	// Get bitmap size
	width = region->right - region->left;
	height = region->bottom - region->top;

	// Get pixels
	// Populate BITMAPINFOHEADER
	hbm = (HBITMAP) GetCurrentObject(hdc, OBJ_BITMAP);
	bmi->bmiHeader.biSize = sizeof(bmi->bmiHeader);
	bmi->bmiHeader.biBitCount = 0;
	::GetDIBits(hdc, hbm, 0, 0, NULL, bmi, DIB_RGB_COLORS);

	// Get the actual argb bit information
	bmi->bmiHeader.biHeight = -bmi->bmiHeader.biHeight;
	pBits = new BYTE[bmi->bmiHeader.biSizeImage];
	::GetDIBits(hdc, hbm, 0, height, pBits, bmi, DIB_RGB_COLORS);

	// calculate average color of all pixels in region
	rr_sum = gg_sum = bb_sum = pix_cnt = 0;
	rr_avg = gg_avg = bb_avg = 0.;

	for (x = 0; x < width; x++) 
	{
		for (y = 0; y < height; y++) 
		{
			red = pBits[y*width*4 + x*4 + 2];
			green = pBits[y*width*4 + x*4 + 1];
			blue = pBits[y*width*4 + x*4 + 0];
			rr_sum += red;
			gg_sum += green;
			bb_sum += blue;
			pix_cnt++;
		}
	}
	if (pix_cnt != 0)
	{
		rr_avg = rr_sum/pix_cnt;
		gg_avg = gg_sum/pix_cnt;
		bb_avg = bb_sum/pix_cnt;
	}

	if (cr_avg!=NULL)
		*cr_avg = RGB(rr_avg, gg_avg, bb_avg);

	if (is_in_RGB_color_cube(GetRValue(region->color), 
						 GetGValue(region->color), 
						 GetBValue(region->color), 
						 region->radius,
						 (int) rr_avg, (int) gg_avg, (int) bb_avg)) 
	{
		*text = "true";
	}
	else {
		*text = "false";
	}

	// Clean up
	HeapFree(GetProcessHeap(), NULL, bmi);
	delete []pBits;

	return ERR_GOOD_SCRAPE_GENERAL;

    __SEH_LOGFATAL("CTransform::c_transform :\n");
}

int CTransform::i_transform(Stablemap_region *region, HDC hdc, CString *text) 
{
    __SEH_HEADER

	int					width, height, x, y, i, best_match, result;
	unsigned int		smallest_pix_diff;
	int					retval=ERR_NOTHING_TO_SCRAPE;
	HBITMAP				hbm;
	BYTE				*pBits, alpha, red, green, blue;
	CompareArgs			args;
	CString				s;


	// Allocate heap space for BITMAPINFO
	BITMAPINFO	*bmi;
	int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
	bmi = (BITMAPINFO *) ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

	width = region->right - region->left;
	height = region->bottom - region->top;
	
	// See if region size is too large
	if (width >= MAX_IMAGE_WIDTH || height>=MAX_IMAGE_HEIGHT) 
	{
		HeapFree(GetProcessHeap(), NULL, bmi);
		return ERR_FIELD_TOO_LARGE;
	}

	// If width or height is negative (left>right or bottom>top), then return
	if (width<0 || height<0)
	{
		HeapFree(GetProcessHeap(), NULL, bmi);
		return ERR_NOTHING_TO_SCRAPE;
	}

	// Set threshhold to 65% of available pixels
	args.ThresholdPixels = (width * height * 0.65) + 0.5;

	// Get pixels
	// Populate BITMAPINFOHEADER
	hbm = (HBITMAP) GetCurrentObject(hdc, OBJ_BITMAP);
	bmi->bmiHeader.biSize = sizeof(bmi->bmiHeader);
	bmi->bmiHeader.biBitCount = 0;
	::GetDIBits(hdc, hbm, 0, 0, NULL, bmi, DIB_RGB_COLORS);

	// Get the actual argb bit information
	bmi->bmiHeader.biHeight = -bmi->bmiHeader.biHeight;
	pBits = new BYTE[bmi->bmiHeader.biSizeImage];
	::GetDIBits(hdc, hbm, 0, height, pBits, bmi, DIB_RGB_COLORS);


	// load ImgA with current pixels from region
	args.ImgA = new RGBAImage(width, height, "imga.ppm");
	for (y = 0; y < height; y++) 
	{
		for (x = 0; x < width; x++) 
		{
			alpha = pBits[y*width*4 + x*4 + 3];
			red = pBits[y*width*4 + x*4 + 2];
			green = pBits[y*width*4 + x*4 + 1];
			blue = pBits[y*width*4 + x*4 + 0];
			args.ImgA->Set(red, green, blue, alpha, x + y * width);
		}
	}

	// scan through all i$ records and find the one that has the smallest pixel difference
	best_match = -1;
	smallest_pix_diff = 0xffffffff;
	for (i=0; i<(int) map.i$.GetSize(); i++)
	{	
		if (map.i$[i].width == width && map.i$[i].height == height)
		{
			// point ImgB to i$ record that was populated on table map load
			args.ImgB = map.i$[i].image;

			// Compare the two images
			result = Yee_Compare(args);
			if (args.PixelsFailed==0)
			{
				best_match = i;
				smallest_pix_diff = 0;
				break;
			}

			if (args.PixelsFailed < smallest_pix_diff)
			{
				best_match = i;
				smallest_pix_diff = args.PixelsFailed;
			}
		}
	}

	// Null ImgB so it is not deleted when args calls its destructor
	args.ImgB = NULL;

	// return the i$ record text, if the smallest pixel difference is less than the threshold
	if (smallest_pix_diff<args.ThresholdPixels)
	{
		*text = map.i$[best_match].name.GetString();
		retval = ERR_GOOD_SCRAPE_GENERAL;
	}
	else
	{
		*text = "";
		retval = ERR_NO_IMAGE_MATCH;
	}

	// Clean up
	HeapFree(GetProcessHeap(), NULL, bmi);
	delete []pBits;

	return retval;

    __SEH_LOGFATAL("CTransform::i_transform :\n");
}

int CTransform::h_transform(Stablemap_region *region, HDC hdc, CString *text) 
{
    __SEH_HEADER

	int					x, y, j;
	int					width, height;
	int					hash_type, num_precs, pixcount;
	uint32_t			*uresult, hash, pix[MAX_HASH_WIDTH*MAX_HASH_HEIGHT];
	int					retval=ERR_NOTHING_TO_SCRAPE;
	int					index;
	HBITMAP				hbm;
	BYTE				*pBits, red, green, blue;
	uint32_t			hashes[512];

	// Allocate heap space for BITMAPINFO
	BITMAPINFO	*bmi;
	int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
	bmi = (BITMAPINFO *) ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

	width = region->right - region->left;
	height = region->bottom - region->top;
	hash_type = region->transform.GetString()[1] - '0';
	
	// See if region size is too large
	if (width >= MAX_HASH_WIDTH || height>=MAX_HASH_HEIGHT) 
	{
		HeapFree(GetProcessHeap(), NULL, bmi);
		return ERR_FIELD_TOO_LARGE;
	}

	// If width or height is negative (left>right or bottom>top), then return
	if (width<0 || height<0)
	{
		HeapFree(GetProcessHeap(), NULL, bmi);
		return ERR_NOTHING_TO_SCRAPE;
	}

	// Get pixels
	// Populate BITMAPINFOHEADER
	hbm = (HBITMAP) GetCurrentObject(hdc, OBJ_BITMAP);
	bmi->bmiHeader.biSize = sizeof(bmi->bmiHeader);
	bmi->bmiHeader.biBitCount = 0;
	::GetDIBits(hdc, hbm, 0, 0, NULL, bmi, DIB_RGB_COLORS);

	// Get the actual argb bit information
	bmi->bmiHeader.biHeight = -bmi->bmiHeader.biHeight;
	pBits = new BYTE[bmi->bmiHeader.biSizeImage];
	::GetDIBits(hdc, hbm, 0, height, pBits, bmi, DIB_RGB_COLORS);

	// all pixel hash
	if (hash_type == 0) 
	{
		for (y = 0; y < height; y++) 
		{
			for (x = 0; x < width; x++) 
			{
				red = pBits[y*width*4 + x*4 + 2];
				green = pBits[y*width*4 + x*4 + 1];
				blue = pBits[y*width*4 + x*4 + 0];
				pix[y*width + x] = (blue<<16) + (green<<8) + red;
			}
		}
		hash = hashword(&pix[0], width * height, HASH_SEED_0);
	}

	// selected pixel hash
	else if (hash_type>=1 && hash_type<=3) 
	{
		num_precs = (int) map.p$.GetSize();
		pixcount = 0;
		for (j=0; j<num_precs; j++) 
		{
			if (map.p$[j].number == hash_type) 
			{
				x = map.p$[j].x;
				y = map.p$[j].y;
				red = pBits[y*width*4 + x*4 + 2];
				green = pBits[y*width*4 + x*4 + 1];
				blue = pBits[y*width*4 + x*4 + 0];
				pix[pixcount++] = (blue<<16) + (green<<8) + red;
			}
		}
		if (hash_type==1)  hash = hashword(&pix[0], pixcount, HASH_SEED_1);
		if (hash_type==2)  hash = hashword(&pix[0], pixcount, HASH_SEED_2);
		if (hash_type==3)  hash = hashword(&pix[0], pixcount, HASH_SEED_3);
	}

	// bsearch h$ for hash
	for (j=0; j<(int) map.h$.GetSize(); j++) 
		hashes[j] = map.h$[j].hash;

	uresult = (uint32_t *) bsearch(&hash, &hashes, map.h$.GetSize(), 
								  sizeof(uint32_t), 
								  (int (*)(const void*, const void*)) compare_hash);

	// no hash match
	if (uresult == NULL) 
	{ 
		retval = ERR_NO_HASH_MATCH; 
		*text = "";
	}
	// hash match found
	else 
	{ 
		retval = ERR_GOOD_SCRAPE_GENERAL; 
		index = ((long) uresult - (long) hashes)/sizeof(uint32_t);
		*text = map.h$[index].name;
	}

	// Clean up
	HeapFree(GetProcessHeap(), NULL, bmi);
	delete []pBits;

	return retval;

    __SEH_LOGFATAL("CTransform::h_transform :\n");
}


int CTransform::t_transform(Stablemap_region *region, HDC hdc, CString *text, CString *separation, bool background[], bool character[][MAX_CHAR_HEIGHT])
{
    __SEH_HEADER

	int					x, y;
	int					i;
	int					width, height;
	CString				s$tXtype;
	HBITMAP				hbm;
	BYTE				*pBits, alpha, red, green, blue;

	// Allocate heap space for BITMAPINFO
	BITMAPINFO	*bmi;
	int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
	bmi = (BITMAPINFO *) ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

	// Initialize
	*text = "";
	for (x=0; x<MAX_CHAR_WIDTH; x++) 
	{
		background[x] = true;
		for (y=0; y<MAX_CHAR_HEIGHT; y++) 
		{
			character[x][y] = false;
		}
	}

	// Check max size
	width = region->right - region->left;
	height = region->bottom - region->top;
	if (width >= MAX_CHAR_WIDTH) 
	{
		text->Append("Field too wide");
		HeapFree(GetProcessHeap(), NULL, bmi);
		return ERR_FIELD_TOO_LARGE;
	}
	if (height >= MAX_CHAR_HEIGHT) 
	{
		text->Append("Field too tall");
		HeapFree(GetProcessHeap(), NULL, bmi);
		return ERR_FIELD_TOO_LARGE;
	}

	// Get associated s$record
	s$tXtype = "plain";
	for (i=0; i<map.s$.GetSize(); i++)
	{
		if (map.s$[i].name.Left(1) == "t" &&
			map.s$[i].name.Mid(1,1) == region->transform.Mid(1,1) &&
			map.s$[i].name.Mid(2,4) == "type")
		{
			s$tXtype = map.s$[i].text;
			i = map.s$.GetSize() + 1;
		}
	}

	// Load entire foreground pixel array into memory
	// Populate BITMAPINFOHEADER
	hbm = (HBITMAP) GetCurrentObject(hdc, OBJ_BITMAP);
	bmi->bmiHeader.biSize = sizeof(bmi->bmiHeader);
	bmi->bmiHeader.biBitCount = 0;
	::GetDIBits(hdc, hbm, 0, 0, NULL, bmi, DIB_RGB_COLORS);

	// Get the actual argb bit information
	bmi->bmiHeader.biHeight = -bmi->bmiHeader.biHeight;
	pBits = new BYTE[bmi->bmiHeader.biSizeImage];
	::GetDIBits(hdc, hbm, 0, height, pBits, bmi, DIB_RGB_COLORS);

	for (x = 0; x < width; x++) 
	{
		for (y = 0; y < height; y++) 
		{
			alpha = pBits[y*width*4 + x*4 + 3];
			red = pBits[y*width*4 + x*4 + 2];
			green = pBits[y*width*4 + x*4 + 1];
			blue = pBits[y*width*4 + x*4 + 0];

			// See if our pixel is in the defined color cube
			if (is_in_ARGB_color_cube((region->color>>24)&0xff,
								 GetRValue(region->color), 
								 GetGValue(region->color), 
								 GetBValue(region->color), 
								 region->radius, 
								 alpha, 
								 red, 
								 green,
								 blue))
			{
				character[x][y] = true;
			}

			// Set flag if this is a not a completely background band
			if (character[x][y] != false) 
				background[x] = false;

		}
	}

	// Clean up
	HeapFree(GetProcessHeap(), NULL, bmi);
	delete []pBits;

	// Save separation information
	if (separation != NULL)
	{
		*separation = "";
		for (y=0; y<height; y++) 
		{
			for (x=0; x<width; x++) 
				separation->Append(character[x][y] ? "X" : " ");

			separation->Append("\r\n");
		}
	}

	// scan through OCR region - plain
	if (s$tXtype.MakeLower() != "fuzzy" && atof(s$tXtype.GetString()) == 0)
		return do_plain_font_scan(region, width, height, background, character, text);

	// scan through OCR region - fuzzy
	else 
		return do_fuzzy_font_scan(region, width, height, background, character, 
								  s$tXtype.MakeLower()=="fuzzy" ? DEFAULT_FUZZY_TOLERANCE : atof(s$tXtype.GetString()) ,
								  text);

    __SEH_LOGFATAL("CTransform::t_transform :\n");
}

int CTransform::do_plain_font_scan(Stablemap_region *region, int width, int height, 
								   bool bg[], bool (*ch)[MAX_CHAR_HEIGHT], CString *text)
{
    __SEH_HEADER

	int					i, y, retval=ERR_TEXT_SCRAPE_NOMATCH;
	int					x_begin, x_end, y_begin, y_end;
	int					temp_right, vert_band_left = 0;
	bool				backg_band;
	CString				newchar;
	char				hexmash_array[512][MAX_SINGLE_CHAR_WIDTH*8 + 1];
	char				*cresult, hexmash[MAX_CHAR_WIDTH*16];
	char				t$chars[512];

	// populate hexmash array
	int hexmash_array_size = 0;
	int sel_region_text_group = atoi(region->transform.Right(1));
	for (i=0; i<(int) map.t$.GetSize(); i++)
	{
		if (map.t$[i].group == sel_region_text_group)
		{
			strcpy(hexmash_array[hexmash_array_size], map.t$[i].hexmash);
			t$chars[hexmash_array_size] = map.t$[i].ch;
			hexmash_array_size++;
		}
	}
	
	while (vert_band_left<width) 
	{
		// Skip any background bands
		backg_band = true;
		while (vert_band_left<width && backg_band==true) 
		{
			for (y = 0; y<=height; y++) 
			{
				if (ch[vert_band_left][y]) 
				{
						backg_band = false;
						y = height + 1;
				}
			}
			if (backg_band==true) 
			{
				vert_band_left++;
			}
		}

		// Shift left and down to eliminate white space on left and bottom, starting at 
		// vert_band_left and extending MAX_SINGLE_CHAR_WIDTH pixels to the right
		get_shift_left_down_indexes(vert_band_left, MAX_SINGLE_CHAR_WIDTH, height, bg, ch, &x_begin, &x_end, &y_begin, &y_end);

		if (y_end-y_begin > MAX_SINGLE_CHAR_HEIGHT)
			y_begin = y_end - MAX_SINGLE_CHAR_HEIGHT;


		// scanning right to left to find the largest match
		// start at vert_band_left+MAX_SINGLE_CHAR_WIDTH
		temp_right = vert_band_left + MAX_SINGLE_CHAR_WIDTH;
		do 
		{
			// calculate hexmash
			calc_hexmash(vert_band_left, temp_right, y_begin, y_end, ch, hexmash);

			// Check for match in t$
			cresult = (char *) bsearch( hexmash, hexmash_array, hexmash_array_size, sizeof(hexmash_array[0]),
										(int (*)(const void*, const void*)) compare_font);

			// Found match, save char and move on
			if (cresult!=NULL) 
			{
				retval = ERR_GOOD_SCRAPE_GENERAL;

				int array_index = ((long)cresult - (long)hexmash_array[0]) / sizeof(hexmash_array[0]);

				newchar.Format("%c", t$chars[array_index]);
				text->Append(newchar);

				vert_band_left = temp_right + 1;
			}

			// No match:  reduce right hand size, or report '?' of we are out of space
			else 
			{

				if (temp_right>vert_band_left) 
				{
					temp_right--;
				}
				else 
				{
					// we've scraped something that can't be recognized
					vert_band_left++;
					cresult = (char *) 1;
				}
			}
		} while (cresult==NULL);
	} // while (vert_band_left<width)

	return retval;

    __SEH_LOGFATAL("CTransform::do_plain_font_scan :\n");
}

int CTransform::do_fuzzy_font_scan(Stablemap_region *region, int width, int height, 
								   bool bg[], bool (*ch)[MAX_CHAR_HEIGHT], double tolerance, CString *text)
{

    __SEH_HEADER

	int					y, t_index, retval = ERR_TEXT_SCRAPE_NOMATCH;
	int					vert_band_left = 0;
	bool				backg_band;
	CString				newchar;

	while (vert_band_left<width) 
	{

		// Skip any background bands
		backg_band = true;
		while (vert_band_left<width && backg_band==true) 
		{
			for (y = 0; y<=height; y++) 
			{
				if (ch[vert_band_left][y]) 
				{
						backg_band = false;
						y = height + 1;
				}
			}
			if (backg_band==true) 
			{
				vert_band_left++;
			}
		}

		// Find best hamming distance match within this MAX_SINGLE_CHAR_WIDTH group of bands 
		t_index = get_best_hd(region, width, height, bg, ch, vert_band_left, tolerance);

		if (t_index != -1)
		{
			retval = ERR_GOOD_SCRAPE_GENERAL;
			
			newchar.Format("%c", map.t$[t_index].ch);
			text->Append(newchar);

			vert_band_left = vert_band_left + map.t$[t_index].x_count;
		}
		else
		{
			vert_band_left += 1;
		}
	}

	return retval;

    __SEH_LOGFATAL("CTransform::do_fuzzy_font_scan :\n");
}
		
int CTransform::get_best_hd(Stablemap_region *region, int width, int height, bool bg[], bool (*ch)[MAX_CHAR_HEIGHT], 
							int left, double tolerance)
{
    __SEH_HEADER

	int					i, j, x, y;
	int					x_begin, x_end, y_begin, y_end;
	CString				hex_bits;
	int					best_hd_width_index, best_hd_t_index;
	unsigned int		hexval_array[MAX_SINGLE_CHAR_WIDTH];
	int					lit_pixels, tot_hd[MAX_SINGLE_CHAR_WIDTH][512];
	double				weighted_hd[MAX_SINGLE_CHAR_WIDTH][512];

	best_hd_width_index = best_hd_t_index = -1;


	for (x=0; x<MAX_SINGLE_CHAR_WIDTH && left+x<width; x++)
	{
		// Shift left and down to eliminate white space on left and bottom
		get_shift_left_down_indexes(left, x+1, height, bg, ch, &x_begin, &x_end, &y_begin, &y_end);

		if (y_end-y_begin > MAX_SINGLE_CHAR_HEIGHT)
			y_begin = y_end - MAX_SINGLE_CHAR_HEIGHT;

		// Get array of hexvals
		for (i=0; i<MAX_SINGLE_CHAR_WIDTH; i++)
			hexval_array[i] = 0;

		for (i=x_begin; i<=x_end; i++)
		{
			for (y=y_end; y>=y_begin; y--)
				if (ch[i][y])
					hexval_array[i-x_begin] += (1 << (y_end-y));
		}

		// Scan through each font record for this region's group and identify the one with the 
		// best hamming distance match
		for (i=0; i<(int) map.t$.GetSize() && i<512; i++)
		{
			lit_pixels = 0;

			// If the size of this character is <= the number of columns we are looking at and
			// if this t$ record is in our font group then consider it
			if (map.t$[i].x_count<=x+1 && atoi(region->transform.Mid(1, 1).GetString()) == map.t$[i].group)
			{
				// Get the hamming distance and lit pix counts "by column" left to right
				tot_hd[x][i] = 0;
				for (j=0; j<map.t$[i].x_count && j<x+1; j++)
				{
					tot_hd[x][i] += hamdist(map.t$[i].x[j], hexval_array[j]);
					lit_pixels += bitcount(map.t$[i].x[j]);
				}

				weighted_hd[x][i] = (double) tot_hd[x][i] / lit_pixels;
			}
			// othewise set this character as a very bad match
			else
			{
				tot_hd[x][i] = 999999;
				weighted_hd[x][i] = 999999;
			}

			// Keep track of our best match - skip this if we have a "very bad match"
			if (tot_hd[x][i] != 999999 && weighted_hd[x][i] != 999999)
			{
				if (best_hd_width_index==-1 || best_hd_t_index==-1)
				{
					best_hd_width_index = x;
					best_hd_t_index = i;
				}
				else
				{
					if (weighted_hd[x][i] < weighted_hd[best_hd_width_index][best_hd_t_index] )
					{
						best_hd_width_index = x;
						best_hd_t_index = i;
					}
					else if (weighted_hd[x][i] == weighted_hd[best_hd_width_index][best_hd_t_index] )
					{
						if (i != best_hd_t_index)
						{
							best_hd_width_index = x;
							best_hd_t_index = i;
						}
					}
				}
			}
		}
	}
	
	if (best_hd_width_index==-1 || best_hd_t_index==-1)
		return -1;

	if (weighted_hd[best_hd_width_index][best_hd_t_index] < tolerance)
	{
		return best_hd_t_index;
	}
	else
	{
		return -1;
	}

    __SEH_LOGFATAL("CTransform::get_best_hd :\n");
}

int CTransform::hamdist(unsigned int x, unsigned int y)
{
    __SEH_HEADER

	int dist = 0, val = x ^ y;

	while(val)
	{
		++dist; 
		val &= val - 1;
	}

	return dist;

    __SEH_LOGFATAL("CTransform::hamdist :\n");
}


void CTransform::get_shift_left_down_indexes(int x_start, int width, int height, bool background[], bool character[][MAX_CHAR_HEIGHT], 
										  int *x_begin, int *x_end, int *y_begin, int *y_end)
{
    __SEH_HEADER

	int	x, y;

	// Find the real beginning and end on the x dimension
	*x_begin = x_start+width-1;
	*x_end = x_start;
	for (x=x_start; x<x_start+width; x++) 
	{
		if (!background[x]) 
		{
			*x_begin = x;
			x = x_start + width + 1;
		}
	}

	for (x=x_start+width-1; x>=x_start; x--) 
	{
		if (!background[x]) 
		{
			*x_end = x;
			x = -1;
		}
	}

	// Find the real beginning and end on the y dimension
	*y_begin = height-1;
	*y_end = 0;
	for (y=0; y<height; y++) 
	{
		for (x=*x_begin; x<=*x_end; x++) 
		{
			if (character[x][y]) 
			{
				*y_begin = y;
				x = *x_end + 1;
				y = height + 1;
			}
		}
	}

	for (y=height-1; y>=0; y--) 
	{
		for (x=*x_begin; x<=*x_end; x++) 
		{
			if (character[x][y]) 
			{
				*y_end = y;
				x = *x_end + 1;
				y = -1;
			}
		}
	}

    __SEH_LOGFATAL("CTransform::get_shift_left_down_indexes :\n");
}

bool CTransform::is_in_ARGB_color_cube(int center_a, int center_r, int center_g, int center_b, 
									   int radius, int pix_a, int pix_r, int pix_g, int pix_b) 
{
    __SEH_HEADER

	int		a_diff, r_diff, g_diff, b_diff, tot_diff;

	// zero radius
	if (radius == 0) 
	{
		if (pix_a == center_a && pix_r == center_r && pix_g == center_g && pix_b == center_b)
			return true;
	}

	// non-zero radius
	else 
	{
		a_diff = center_a - pix_a;
		r_diff = center_r - pix_r; 
		g_diff = center_g - pix_g; 
		b_diff = center_b - pix_b; 
		tot_diff = (int) pow((double) (a_diff*a_diff + r_diff*r_diff + g_diff*g_diff + b_diff*b_diff), 0.5);

		// positive radius
		if (radius >= 0)
		{
			if (tot_diff <= radius) 
				return true;
		}

		// negative radius (logical not)
		else 
		{
			if (tot_diff > -radius)
				 return true;
		}
	}	

	return false;

    __SEH_LOGFATAL("CTransform::is_in_ARGB_color_cube :\n");
}

bool CTransform::is_in_RGB_color_cube(int center_r, int center_g, int center_b, int radius, int pix_r, int pix_g, int pix_b)
{
    __SEH_HEADER

	int		r_diff, g_diff, b_diff, tot_diff;

	// zero radius
	if (radius == 0) 
	{
		if (pix_r == center_r && pix_g == center_g && pix_b == center_b)
			return true;
	}

	// non-zero radius
	else 
	{
		r_diff = center_r - pix_r; 
		g_diff = center_g - pix_g; 
		b_diff = center_b - pix_b; 
		tot_diff = (int) pow((double) (r_diff*r_diff + g_diff*g_diff + b_diff*b_diff), 0.5);

		// positive radius
		if (radius >= 0) 
		{
			if (tot_diff <= radius)
				return true;
		}

		// negative radius (logical not)
		else 
		{
			if (tot_diff > -radius)
				 return true;
		}
	}	

	return false;

    __SEH_LOGFATAL("CTransform::is_in_RGB_color_cube :\n");
}

void CTransform::calc_hexmash(int left, int right, int top, int bottom, bool (*ch)[MAX_CHAR_HEIGHT], char *hexmash, bool withspace)
{
    __SEH_HEADER

	int				x, y, last_fg_row;
	unsigned int	hexval;
	char			t[20];

	// find last horizontal row with foreground pixels
	last_fg_row = -1;
	for (y = bottom; y>=top; y--) 
	{
		for (x = left; x<= right; x++) 
		{
			if (ch[x][y]) 
			{
				last_fg_row = y;
				x = right + 1;
				y = -1;
			}
		}
	}

	// Calculate hexmash
	strcpy(hexmash, "");
	for (x = left; x <= right; x++) 
	{
		hexval = 0;
		for (y = last_fg_row; y>=0; y--)
			if (ch[x][y])
				hexval += (1 << (last_fg_row - y));

		sprintf(t, "%x", hexval);
		strcat(hexmash, t);
		if (withspace)  strcat(hexmash, " ");
	}

    __SEH_LOGFATAL("CTransform::calc_hexmash :\n");
}

void CTransform::parse_string_bsl(CString text, CString format, CString *results,
								  double *handnumber, double *sblind, double *bblind, double *bbet, 
								  double *ante, int *limit, double *sb_bb, double *bb_BB, bool *istournament,
								  bool *found_handnumber, bool *found_sblind, bool *found_bblind, bool *found_bbet,
								  bool *found_ante, bool *found_limit, bool *found_sb_bb, bool *found_bb_BB) 
{
    __SEH_HEADER

	int			place_in_format = 0, place_in_text = 0;
	CString		token, skip_str, number_type;
	double		number;
	CString		temp;

	if (results!=NULL)
		*results == "";

	while (place_in_format < format.GetLength() && place_in_text < text.GetLength()) 
	{
		// handnumber
		if (format.Mid(place_in_format,2) == "^h") 
		{
			place_in_format+=2;
			token = "";
			while (text.Mid(place_in_text,1).FindOneOf("0123456789-")!=-1 &&
				   place_in_text < text.GetLength()) 
			{
				if (text.Mid(place_in_text,1) != "-")
					token.Append(text.Mid(place_in_text,1));

				place_in_text+=1;
			}
			if (results!=NULL)
				results->Append("^h (handnumber)\t= " + token + "\r\n");

			if (handnumber!=NULL && found_handnumber!=NULL)
			{
				if (!*found_handnumber)
				{
					*handnumber = atof(token.GetString()); 
					*found_handnumber = true; 
				}
			}
		}

		// dummy int
		else if (format.Mid(place_in_format,2) == "^d") 
		{
			place_in_format+=2;
			token = "";

			while (text.Mid(place_in_text,1).FindOneOf("$0123456789,¢km")!=-1 && place_in_text<text.GetLength())
			{
				token.Append(text.Mid(place_in_text,1));
				place_in_text++;
			}

			if (results!=NULL)
				results->Append("^d (dummy int)\t= " + token + "\r\n");
		}

		// dummy float
		else if (format.Mid(place_in_format,2) == "^f") 
		{
			place_in_format+=2;
			token = "";

			while (text.Mid(place_in_text,1).FindOneOf("$0123456789,.¢ckm")!=-1 && place_in_text<text.GetLength())
			{
				token.Append(text.Mid(place_in_text,1));
				place_in_text++;
			}

			if (results!=NULL)
				results->Append("^f (dummy float)\t= " + token + "\r\n");
		}

		// numbers
		else if (format.Mid(place_in_format,2) == "^s" ||
				 format.Mid(place_in_format,2) == "^b" ||
				 format.Mid(place_in_format,2) == "^B" ||
				 format.Mid(place_in_format,2) == "^a" ||
				 format.Mid(place_in_format,2) == "^v" ||
				 format.Mid(place_in_format,2) == "^V") 
		{
			
			number_type = format.Mid(place_in_format,2);
			place_in_format+=2;
			token = "";

			while (text.Mid(place_in_text,1).FindOneOf("$0123456789,.¢ckm")!=-1 && place_in_text<text.GetLength())
			{
				token.Append(text.Mid(place_in_text,1));
				place_in_text++;
			}

			number = string_to_money(token);

			if (number == (int) number)
				temp.Format("%.0f", number);
			else
				temp.Format("%.2f", number);

			if (number_type == "^s")
			{
				if (results!=NULL)  
					results->Append("^s (sblind)\t= " + temp + "\r\n");

				if (found_sblind!=NULL && sblind!=NULL) 
				{
					if(!*found_sblind)
					{
						*sblind = number; 
						*found_sblind=true; 
					}
				}
			}

			else if (number_type == "^b")  
			{
				if (results!=NULL)	
					results->Append("^b (bblind)\t= " + temp + "\r\n");

				if (found_bblind!=NULL && bblind!=NULL) 
				{
					if(!*found_bblind)
					{
						*bblind = number; 
						*found_bblind=true; 
					}
				}
			}
			
			else if (number_type == "^B")
			{
				if (results!=NULL)  
					results->Append("^B (bbet)\t= " + temp + "\r\n");

				if (found_bbet!=NULL && bbet!=NULL) 
				{
					if(!*found_bbet)
					{
						*bbet = number; 
						*found_bbet=true; 
					}
				}
			}
			
			else if (number_type == "^a")
			{
				if (results!=NULL)  
					results->Append("^a (ante)\t= " + temp + "\r\n");

				if (found_ante!=NULL && ante!=NULL) 
				{
					if(!*found_ante)
					{
						*ante = number; 
						*found_ante=true; 
					}
				}
			}
			
			else if (number_type == "^v")
			{
				if (results!=NULL)  
					results->Append("^v (sb_bb)\t= " + temp + "\r\n");

				if (found_sb_bb!=NULL && sb_bb!=NULL) 
				{
					if(!*found_sb_bb)
					{
						*sb_bb = number; 
						*found_sb_bb=true; 
					}
				}
			}
			
			else if (number_type == "^V")
			{
				if (results!=NULL)  
					results->Append("^V (bb_BB)\t= " + temp + "\r\n");

				if (found_bb_BB!=NULL && bb_BB!=NULL) 
				{
					if(!*found_bb_BB)
					{
						*bb_BB = number; 
						*found_bb_BB=true; 
					}
				}
			}
		}

		// Skip all chars up to string following the ^*
		else if (format.Mid(place_in_format,2) == "^*") 
		{
			place_in_format+=2;
			skip_str = "";
			token = "";

			// Get the string that we want to match up to
			while (format.Mid(place_in_format,1) != "^" && place_in_format < format.GetLength()) 
			{
				skip_str.Append(format.Mid(place_in_format,1));
				place_in_format+=1;
			}
			while (text.Mid(place_in_text, skip_str.GetLength()) != skip_str &&
				   place_in_text < text.GetLength()) 
			{
   				token.Append(text.Mid(place_in_text,1));
				place_in_text+=1;
			}
			if (text.Mid(place_in_text, skip_str.GetLength()) == skip_str) 
			{
   				token.Append(text.Mid(place_in_text, skip_str.GetLength()));
				place_in_text += skip_str.GetLength();
			}
			if (results!=NULL)
				results->Append("^* (skip string)\t= '" + token + "'\r\n");
		}

		// Roman numerals
		else if (format.Mid(place_in_format,2) == "^R") 
		{
			place_in_format+=2;
			token = "";

			// Get roman numeral
			while ( ( text.Mid(place_in_text,1).MakeLower() == "i" ||
					  text.Mid(place_in_text,1).MakeLower() == "v" ||
					  text.Mid(place_in_text,1).MakeLower() == "x" ||
					  text.Mid(place_in_text,1).MakeLower() == "l" ||
					  text.Mid(place_in_text,1).MakeLower() == "c") &&
				   place_in_text < text.GetLength()) 
			{
				token.Append(text.Mid(place_in_text,1).MakeLower());
				place_in_text+=1;
			}

			// This is commented out for now, because, strangely, WH does not provide a 
			// place to report this value to the user
			// convert roman numeral to number
			//if (token == "i") { 
			//	level = 1;
			//}
			//if (token == "ii") { 
			//	level = 2;
			//}

			if (results!=NULL)
				results->Append("^R (roman numeral)\t= " + token + "\r\n");

		}

		// Limit, no-limit, pot limit identifier
		else if (format.Mid(place_in_format,2) == "^L") 
		{
			place_in_format+=2;
			if (text.Mid(place_in_text,8).MakeLower() == "no limit" ||
				text.Mid(place_in_text,8).MakeLower() == "no-limit") 
			{
				if (results!=NULL)  
					results->Append("^L (limit type)\t= " + text.Mid(place_in_text,8) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = LIMIT_NL; 
						*found_limit=true; 
					}
				}

				place_in_text += 8;
			}

			else if (text.Mid(place_in_text,7).MakeLower() == "nolimit") 
			{
				if (results!=NULL)  
					results->Append("^L (limit type)\t= " + text.Mid(place_in_text,7) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = LIMIT_NL; 
						*found_limit=true; 
					}
				}

				place_in_text += 7;
			}

			else if (text.Mid(place_in_text,2).MakeLower() == "nl") 
			{
				if (results!=NULL)  
					results->Append("^L (limit type)\t= " + text.Mid(place_in_text,2) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = LIMIT_NL; 
						*found_limit=true; 
					}
				}

				place_in_text += 2;
			}

			else if (text.Mid(place_in_text,9).MakeLower() == "pot limit" ||
				text.Mid(place_in_text,9).MakeLower() == "pot-limit") 
			{
				if (results!=NULL)  
					results->Append("^L (limit type)\t= " + text.Mid(place_in_text,9) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = LIMIT_PL; 
						*found_limit=true; 
					}
				}

				place_in_text += 9;
			}

			else if (text.Mid(place_in_text,8).MakeLower() == "potlimit") 
			{
				if (results!=NULL)  
					results->Append("^L (limit type)\t= " + text.Mid(place_in_text,8) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = LIMIT_PL; 
						*found_limit=true; 
					}
				}

				place_in_text += 8;
			}

			else if (text.Mid(place_in_text,2).MakeLower() == "pl") 
			{
				if (results!=NULL)  
					results->Append("^L (limit type)\t= " + text.Mid(place_in_text,2) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = LIMIT_PL; 
						*found_limit=true; 
					}
				}

				place_in_text += 2;
			}

			else if (text.Mid(place_in_text,11).MakeLower() == "fixed limit" ||
				text.Mid(place_in_text,11).MakeLower() == "fixed-limit") 
			{
				if (results!=NULL)  
					results->Append("^L (limit type)\t= " + text.Mid(place_in_text,11) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = LIMIT_FL; 
						*found_limit=true; 
					}
				}

				place_in_text += 11;
			}

			else if (text.Mid(place_in_text,10).MakeLower() == "fixedlimit") 
			{
				if (results!=NULL)  
					results->Append("^L (limit type)\t= " + text.Mid(place_in_text,10) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = LIMIT_FL; 
						*found_limit=true; 
					}
				}

				place_in_text += 10;
			}

			else if (text.Mid(place_in_text,2).MakeLower() == "fl") 
			{
				if (results!=NULL)  
					results->Append("^L (limit type)\t= " + text.Mid(place_in_text,2) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = LIMIT_FL; 
						*found_limit=true; 
					}
				}

				place_in_text += 2;
			}

			else if (text.Mid(place_in_text,5).MakeLower() == "limit") 
			{
				if (results!=NULL)  
					results->Append("^L (limit type)\t= " + text.Mid(place_in_text,5) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = LIMIT_FL; 
						*found_limit=true; 
					}
				}

				place_in_text += 5;
			}
		}
		
		// All other exact matches
		else if (text.Mid(place_in_text,1) == format.Mid(place_in_format,1))
		{
			if (results!=NULL)  
				results->Append("exact match\t= '");

			while (text.Mid(place_in_text,1) == format.Mid(place_in_format,1) && place_in_text<text.GetLength() && place_in_format<format.GetLength()) 
			{
				if (results!=NULL)
					results->Append(text.Mid(place_in_text,1));

				place_in_text+=1;
				place_in_format+=1;
			}

			if (results!=NULL)  
				results->Append("'\r\n");
		}

		// Match failed, exit loop
		else
		{
			place_in_format = format.GetLength()+1;
			place_in_text = text.GetLength()+1;
		}
	}

    __SEH_LOGFATAL("CTransform::parse_string_BSL :\n");
}

double CTransform::string_to_money(CString inStr)
{
    __SEH_HEADER

	const char *str = inStr.GetString();
	CStringArray possibleValues;
	CArray<int, int> possibleValuesMultiplier;
	CString activeValue;
	int iValueWithCurrencySymbol = -1;
	bool currencySymbol = false;
	while (*str) 
	{
		switch (*str) 
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '.':
				activeValue += *str;
				break;
			case ',':
				break;
			case '$':
			case '€':
			case '£':
				if (activeValue.GetLength() > 0) 
				{
					possibleValues.Add(activeValue);
					possibleValuesMultiplier.Add(1);
					activeValue.Empty();
				}
				currencySymbol = true;
				break;
			default:
				if (activeValue.GetLength() > 0) 
				{
					int index = possibleValues.Add(activeValue);
					if (currencySymbol)
						iValueWithCurrencySymbol = index;
					if (*str == '¢' || *str == 'c')
						possibleValuesMultiplier.Add(-100);
					else if (*str == 'k')
						possibleValuesMultiplier.Add(1000);
					else if (*str == 'm')
						possibleValuesMultiplier.Add(1000000);
					else
						possibleValuesMultiplier.Add(1);
					activeValue.Empty();
				}
				break;
		}
		str++;
	}

	if (activeValue.GetLength() > 0) 
	{
		int index = possibleValues.Add(activeValue);
		possibleValuesMultiplier.Add(1);
		if (currencySymbol)
			iValueWithCurrencySymbol = index;
	}

	double number = 0.0;

	int iValueToUse = -1;
	if (possibleValues.GetSize() == 1)
		iValueToUse = 0;
	else if (iValueWithCurrencySymbol >= 0)
		iValueToUse = iValueWithCurrencySymbol;
	else if (possibleValues.GetSize() > 1)
		iValueToUse = 0;
	if (iValueToUse >= 0) {
		number = atof(possibleValues[iValueToUse].GetString());
		if (possibleValuesMultiplier[iValueToUse] < 0)
			number /= -possibleValuesMultiplier[iValueToUse];
		else
			number *= possibleValuesMultiplier[iValueToUse];
	}

	return number;

    __SEH_LOGFATAL("CTransform::string_to_money :\n");
}


char * CTransform::get_time(char * timebuf) 
{
    __SEH_HEADER

	// returns current system time in WH format
	time_t	ltime;
	char tmptime[30];
	
	time( &ltime );
	ctime_s(tmptime, 26, &ltime);
	tmptime[24]='\0';

	memcpy(timebuf, tmptime+20, 4); //yyyy
	*(timebuf+4) = '-';

	// mm
	if (memcmp(tmptime+4, "Jan", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '1'; }
	else if (memcmp(tmptime+4, "Feb", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '2'; }
	else if (memcmp(tmptime+4, "Mar", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '3'; }
	else if (memcmp(tmptime+4, "Apr", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '4'; }
	else if (memcmp(tmptime+4, "May", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '5'; }
	else if (memcmp(tmptime+4, "Jun", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '6'; }
	else if (memcmp(tmptime+4, "Jul", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '7'; }
	else if (memcmp(tmptime+4, "Aug", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '8'; }
	else if (memcmp(tmptime+4, "Sep", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '9'; }
	else if (memcmp(tmptime+4, "Oct", 3)==0)  { *(timebuf+5) = '1'; *(timebuf+6) = '0'; }
	else if (memcmp(tmptime+4, "Nov", 3)==0)  { *(timebuf+5) = '1'; *(timebuf+6) = '1'; }
	else if (memcmp(tmptime+4, "Dec", 3)==0)  { *(timebuf+5) = '1'; *(timebuf+6) = '2'; }

	*(timebuf+7) = '-';
	memcpy(timebuf+8, tmptime+8, 2); //dd
	*(timebuf+10) = ' ';
	memcpy(timebuf+11, tmptime+11, 8); //HH:mm:ss
	*(timebuf+19) = '\0';

	return timebuf;

    __SEH_LOGFATAL("CTransform::get_time :\n");
}

char * CTransform::get_now_time(char * timebuf) 
{
    __SEH_HEADER

	// returns current system time as a UNIX style string
	time_t	ltime;

	time( &ltime );
	ctime_s(timebuf, 26, &ltime);
	timebuf[24]='\0';

	return timebuf;

    __SEH_LOGFATAL("CTransform::get_now_time :\n");
}


// supporting functions for bsearch's
int compare_font( char *hexmash1, char *hexmash2) 
{
	return _strcmpi( hexmash1, hexmash2 );
}

int compare_hash( uint32_t *hash1, uint32_t *hash2) 
{
	if (*hash1 < *hash2) return -1;
	else if (*hash1 > *hash2) return 1;
	else return 0;
}

int	bitcount(unsigned int u) 
{ 
	// returns count of number of bits set in the given value
	unsigned int uCount;

	 uCount = u 
			  - ((u >> 1) & 033333333333) 
			  - ((u >> 2) & 011111111111);
	 return 
	   ((uCount + (uCount >> 3)) 
		& 030707070707) % 63;
}
