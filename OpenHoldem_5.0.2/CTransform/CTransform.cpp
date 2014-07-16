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


#include "StdAfx.h"
#include "CTransform.h"

#include <Math.h>
#include "..\OpenHoldem\CPreferences.h"
#include "..\OpenHoldem\MagicNumbers.h"
#include "..\OpenHoldem\NumericalFunctions.h"
#include "..\OpenHoldem\StringFunctions.h"

#ifdef OPENHOLDEM_PROGRAM
#include "../OpenHoldem/debug.h"
#endif

#ifdef OPENSCRAPE_PROGRAM
#include "../OpenScrape/debug.h"
#endif

CTransform::CTransform(void)
{
}

CTransform::~CTransform(void)
{
}

const int CTransform::DoTransform(RMapCI region, const HDC hdc, CString *text, CString *separation, COLORREF *cr_avg) 
{
	bool				character[MAX_CHAR_WIDTH][MAX_CHAR_HEIGHT] = {false};	
	bool				background[MAX_CHAR_WIDTH] = {false};

	// Initialize arrays
	for (int i=0; i<MAX_CHAR_WIDTH; i++)
		background[i] = true;


	switch (region->second.transform[0]) 
	{
		case 'C':
			return CTypeTransform(region, hdc, text, cr_avg);
			break;
		
		case 'I':
			return ITypeTransform(region, hdc, text);
			break;
		
		case 'H':
			return HTypeTransform(region, hdc, text);
			break;

		case 'T':
			return TTypeTransform(region, hdc, text, separation, background, character);
			break;

		case 'N':
			*text = "";
			return ERR_GOOD_SCRAPE_GENERAL;
			break;
	}

	return ERR_INVALID_TRANSFORM_TYPE;
}

const int CTransform::CTypeTransform(RMapCI region, const HDC hdc, CString *text, COLORREF *cr_avg) 
{
	int					x = 0, y = 0;
	int					width = 0, height = 0;
	int					rr_sum = 0, gg_sum = 0, bb_sum = 0, pix_cnt = 0;
	double				rr_avg = 0., gg_avg = 0., bb_avg = 0.;
	HBITMAP				hbm = NULL;
	BYTE				*pBits = NULL, red = 0, green = 0, blue = 0;

	// Allocate heap space for BITMAPINFO
	BITMAPINFO	*bmi = NULL;
	int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
	bmi = (BITMAPINFO *) ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

	// Get bitmap size
	width = region->second.right - region->second.left + 1;
	height = region->second.bottom - region->second.top + 1;

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

	if (IsInRGBColorCube(GetRValue(region->second.color), 
						 GetGValue(region->second.color), 
						 GetBValue(region->second.color), 
						 region->second.radius,
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
}

const int CTransform::ITypeTransform(RMapCI region, const HDC hdc, CString *text) 
{
	int					width = 0, height = 0, x = 0, y = 0, i = 0, result = 0;
	IMapCI				best_match = p_tablemap->i$()->end();
	unsigned int		smallest_pix_diff = 0;
	int					retval=ERR_NOTHING_TO_SCRAPE;
	HBITMAP				hbm = NULL;
	BYTE				*pBits = NULL, alpha = 0, red = 0, green = 0, blue = 0;
	CompareArgs			args;
	CString				s = "";


	// Allocate heap space for BITMAPINFO
	BITMAPINFO	*bmi = NULL;
	int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
	bmi = (BITMAPINFO *) ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

	width = region->second.right - region->second.left + 1;
	height = region->second.bottom - region->second.top + 1;
	
	// See if region size is too large
	if (width > MAX_IMAGE_WIDTH || height > MAX_IMAGE_HEIGHT) 
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
	args.ThresholdPixels = (unsigned int) ((width * height * 0.65) + 0.5);

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
	smallest_pix_diff = 0xffffffff;
	for (IMapCI i_iter=p_tablemap->i$()->begin(); i_iter!=p_tablemap->i$()->end(); i_iter++)
	{	
		if (i_iter->second.width == width && i_iter->second.height == height)
		{
			// point ImgB to i$ record that was populated on table map load
			args.ImgB = i_iter->second.image;

			// Compare the two images
			result = Yee_Compare(args);
			if (args.PixelsFailed==0)
			{
				best_match = i_iter;
				smallest_pix_diff = 0;
				break;
			}

			if (args.PixelsFailed < smallest_pix_diff)
			{
				best_match = i_iter;
				smallest_pix_diff = args.PixelsFailed;
			}
		}
	}

	// Null ImgB so it is not deleted when args calls its destructor
	args.ImgB = NULL;

	// return the i$ record text, if the smallest pixel difference is less than the threshold
	if (smallest_pix_diff<args.ThresholdPixels)
	{
		*text = best_match->second.name.GetString();
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
}

const int CTransform::HTypeTransform(RMapCI region, const HDC hdc, CString *text) 
{
	int					x = 0, y = 0, j = 0;
	int					width = 0, height = 0;
	int					hash_type = 0, pixcount = 0;
	uint32_t			hash = 0, pix[MAX_HASH_WIDTH*MAX_HASH_HEIGHT] = {0};
	int					retval = ERR_NOTHING_TO_SCRAPE;
	BYTE				*pBits = NULL, red = 0, green = 0, blue = 0;

	width = region->second.right - region->second.left + 1;
	height = region->second.bottom - region->second.top + 1;
	hash_type = region->second.transform[1] - '0';
	
	if (p_tablemap->h$(hash_type)->empty())
		return ERR_NO_HASH_MATCH;

	// See if region size is too large
	if (width > MAX_HASH_WIDTH || height > MAX_HASH_HEIGHT) 
		return ERR_FIELD_TOO_LARGE;

	// If width or height is negative (left>right or bottom>top), then return
	if (width<0 || height<0)
		return ERR_NOTHING_TO_SCRAPE;

	// Allocate heap space for BITMAPINFO
	BITMAPINFO	*bmi;
	int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
	bmi = (BITMAPINFO *) ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

	// Get pixels
	// Populate BITMAPINFOHEADER
	HBITMAP hbm = (HBITMAP) GetCurrentObject(hdc, OBJ_BITMAP);
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
	else if (hash_type>=1 && hash_type<k_max_number_of_hash_groups_in_tablemap) 
	{
		pixcount = 0;
		for (PMapCI p_iter=p_tablemap->p$(hash_type)->begin(); p_iter!=p_tablemap->p$(hash_type)->end(); p_iter++) 
		{
			x = p_iter->second.x;
			y = p_iter->second.y;

			if (x<width && y<height)
			{
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

	// lookup hash in h$ records
	HMapCI h_iter = p_tablemap->h$(hash_type)->find(hash);

	// no hash match
	if (h_iter == p_tablemap->h$(hash_type)->end()) 
	{ 
		retval = ERR_NO_HASH_MATCH; 
		*text = "";
	}
	// hash match found
	else 
	{ 
		retval = ERR_GOOD_SCRAPE_GENERAL; 
		*text = h_iter->second.name;
	}

	// Clean up
	HeapFree(GetProcessHeap(), NULL, bmi);
	delete []pBits;

	return retval;
}


const int CTransform::TTypeTransform(RMapCI region, const HDC hdc, CString *text, CString *separation, 
									 bool background[], bool (*character)[MAX_CHAR_HEIGHT])
{
	int					x = 0, y = 0;
	int					i = 0;
	int					width = region->second.right - region->second.left + 1;
	int					height = region->second.bottom - region->second.top + 1;
	CString				s = "", s$tXtype = "";
	BYTE				*pBits = NULL, alpha = 0, red = 0, green = 0, blue = 0;

	// Initialize
	*text = "";

	// Check max size
	if (width > MAX_CHAR_WIDTH) 
	{
		text->Append("Field too wide");
		return ERR_FIELD_TOO_LARGE;
	}
	if (height > MAX_CHAR_HEIGHT) 
	{
		text->Append("Field too tall");
		return ERR_FIELD_TOO_LARGE;
	}

	// Get associated s$record
	s$tXtype = "plain";

	s.Format("t%ctype", region->second.transform[1]);

	SMapCI s_iter = p_tablemap->s$()->find(s.GetString());
	if (s_iter != p_tablemap->s$()->end())
		s$tXtype = s_iter->second.text;

	// Allocate heap space for BITMAPINFO
	BITMAPINFO	*bmi = NULL;
	int			info_len = sizeof(BITMAPINFOHEADER) + 1024;
	bmi = (BITMAPINFO *) ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

	// Load entire foreground pixel array into memory
	// Populate BITMAPINFOHEADER
	HBITMAP hbm = (HBITMAP) GetCurrentObject(hdc, OBJ_BITMAP);
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
			if (IsInARGBColorCube((region->second.color>>24)&0xff,
								 GetRValue(region->second.color), 
								 GetGValue(region->second.color), 
								 GetBValue(region->second.color), 
								 region->second.radius, 
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
		return DoPlainFontScan(region, width, height, background, character, text);

	// scan through OCR region - fuzzy
	else 
		return DoFuzzyFontScan(region, width, height, background, character, 
								  s$tXtype.MakeLower()=="fuzzy" ? DEFAULT_FUZZY_TOLERANCE : atof(s$tXtype.GetString()) ,
								  text);
}

const int CTransform::DoPlainFontScan(RMapCI region, const int width, const int height, 
									  const bool bg[], const bool (*ch)[MAX_CHAR_HEIGHT], CString *text)
{
	int					y = 0, retval=ERR_TEXT_SCRAPE_NOMATCH;
	int					x_begin = 0, x_end = 0, y_begin = 0, y_end = 0;
	int					temp_right = 0, vert_band_left = 0;
	bool				backg_band = false;
	CString				newchar = "", hexmash = "";
	int					text_group = atoi(region->second.transform.Right(1));
	std::map<CString, int>::const_iterator	fontindex;

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

	while (vert_band_left<width) 
	{


		// Shift left and down to eliminate white space on left and bottom, starting at 
		// vert_band_left and extending MAX_SINGLE_CHAR_WIDTH pixels to the right
		GetShiftLeftDownIndexes(vert_band_left, MAX_SINGLE_CHAR_WIDTH, height, bg, ch, &x_begin, &x_end, &y_begin, &y_end);

		if (y_end-y_begin > MAX_SINGLE_CHAR_HEIGHT)
			y_begin = y_end - MAX_SINGLE_CHAR_HEIGHT;

		// scanning right to left to find the largest match
		// start at vert_band_left+MAX_SINGLE_CHAR_WIDTH
		temp_right = vert_band_left + MAX_SINGLE_CHAR_WIDTH;

		if (temp_right>width)
			temp_right = width;

		bool continue_looping = true;
		do 
		{
			// calculate hexmash
			CalcHexmash(vert_band_left, temp_right, y_begin, y_end, ch, &hexmash);

			// lookup font in t$ records
			TMapCI t_iter = p_tablemap->t$(text_group)->find(hexmash.GetString());

			// Found match, save char and move on
			if (t_iter != p_tablemap->t$(text_group)->end()) 
			{
				retval = ERR_GOOD_SCRAPE_GENERAL;

				newchar.Format("%c", t_iter->second.ch);
				text->Append(newchar);

				vert_band_left = temp_right + 1;

				continue_looping = false;
			}

			// No match:  reduce right hand size, or report '?' or we are out of space
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

					continue_looping = false;
				}
			}
		} while (continue_looping);
	} // while (vert_band_left<width)

	return retval;
}

const int CTransform::DoFuzzyFontScan(RMapCI region, const int width, const int height, 
									  const bool bg[], const bool (*ch)[MAX_CHAR_HEIGHT], const double tolerance, 
									  CString *text)
{

	int					y = 0, t_index = 0, retval = ERR_TEXT_SCRAPE_NOMATCH;
	int					vert_band_left = 0;
	bool				backg_band = false;
	CString				newchar = "";
	int					font_group = strtoul(CString(region->second.transform[1]), NULL, 10);

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
		TMapCI t_iter = GetBestHammingDistance(region, width, height, bg, ch, vert_band_left, tolerance);

		if (t_iter != p_tablemap->t$(font_group)->end())
		{
			retval = ERR_GOOD_SCRAPE_GENERAL;
			
			newchar.Format("%c", t_iter->second.ch);
			text->Append(newchar);

			vert_band_left = vert_band_left + t_iter->second.x_count;
		}
		else
		{
			vert_band_left += 1;
		}
	}

	return retval;
}
		
TMapCI CTransform::GetBestHammingDistance(RMapCI region, const int width, const int height, 
											 const bool bg[], const bool (*ch)[MAX_CHAR_HEIGHT], 
											 const int left, const double tolerance)
{
	CString				hex_bits = "";
	double				best_weighted_hd = 999999.;
	unsigned int		hexval_array[MAX_SINGLE_CHAR_WIDTH] = {0};
	int					font_group = strtoul(CString(region->second.transform[1]), NULL, 10);
	TMapCI				best_hd_t_iter = p_tablemap->t$(font_group)->end();

	for (int x=0; x<MAX_SINGLE_CHAR_WIDTH && left+x<width; x++)
	{
		// Shift left and down to eliminate white space on left and bottom
		int x_begin = 0, x_end = 0, y_begin = 0, y_end = 0;
		GetShiftLeftDownIndexes(left, x+1, height, bg, ch, &x_begin, &x_end, &y_begin, &y_end);

		if (y_end-y_begin > MAX_SINGLE_CHAR_HEIGHT)
			y_begin = y_end - MAX_SINGLE_CHAR_HEIGHT;

		// Get array of hexvals
		for (int i=0; i<MAX_SINGLE_CHAR_WIDTH; i++)
			hexval_array[i] = 0;

		for (int i=x_begin; i<=x_end; i++)
		{
			for (int y=y_end; y>=y_begin; y--)
				if (ch[i][y])
					hexval_array[i-x_begin] += (1 << (y_end-y));
		}

		// Scan through each font record for this region's group and identify the one with the 
		// best hamming distance match
		for (TMapCI t_iter=p_tablemap->t$(font_group)->begin(); t_iter!=p_tablemap->t$(font_group)->end(); t_iter++)
		{
			double lit_pixels = 0.000001;
			double tot_hd = 0.000001;
			double weighted_hd = 999999.;

			// If the size of this character is <= the number of columns we are looking at then consider it
			if (t_iter->second.x_count<=x+1)
			{
				// Get the hamming distance and lit pix counts "by column" left to right
				for (int j=0; j<t_iter->second.x_count && j<x+1; j++)
				{
					tot_hd += CalcHammingDistance(t_iter->second.x[j], hexval_array[j]);
					lit_pixels += bitcount(t_iter->second.x[j]);
				}

				weighted_hd = (double) tot_hd / (double) lit_pixels;
			}

			// Keep track of our best match - skip this if weighted_hd exceeds the tolerance
			if (weighted_hd < tolerance && weighted_hd < best_weighted_hd)
			{
				best_hd_t_iter = t_iter;
				best_weighted_hd = weighted_hd;
				if (tot_hd > lit_pixels) break;
			}
		}
	}

	return best_hd_t_iter;
}

const int CTransform::CalcHammingDistance(const unsigned int x, const unsigned int y)
{
	int dist = 0, val = x ^ y;

	while(val)
	{
		++dist; 
		val &= val - 1;
	}

	return dist;
}


const void CTransform::GetShiftLeftDownIndexes(const int x_start, const int width, const int height, 
											   const bool background[], const bool character[][MAX_CHAR_HEIGHT], 
											   int *x_begin, int *x_end, int *y_begin, int *y_end)
{
	int	x = 0, y = 0;

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
}

const bool CTransform::IsInARGBColorCube(const int center_a, const int center_r, const int center_g, const int center_b, 
									     const int radius, const int pix_a, const int pix_r, const int pix_g, 
										 const int pix_b) 
{
	int		a_diff = 0, r_diff = 0, g_diff = 0, b_diff = 0, tot_diff = 0;

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
}

const bool CTransform::IsInRGBColorCube(const int center_r, const int center_g, const int center_b, 
										const int radius, const int pix_r, const int pix_g, const int pix_b)
{
	int		r_diff = 0, g_diff = 0, b_diff = 0, tot_diff = 0;

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
}

const void CTransform::CalcHexmash(const int left, const int right, const int top, const int bottom, 
								   const bool (*ch)[MAX_CHAR_HEIGHT], CString *hexmash, const bool withspace)
{
	int				x = 0, y = 0, last_fg_row = 0;
	unsigned int	hexval = 0;
	char			t[20] = {0};

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
	*hexmash = "";
	for (x = left; x <= right; x++) 
	{
		hexval = 0;
		for (y = last_fg_row; y>=top; y--)
			if (ch[x][y])
				hexval += (1 << (last_fg_row - y));

		sprintf_s(t, 20, "%x", hexval);

		*hexmash += t;
		if (withspace)  
			*hexmash += " ";
	}
}

const void CTransform::ParseStringBSL(const CString text, const CString format, 
									  CString *results, CString *handnumber, double *sblind, double *bblind, double *bbet, 
									  double *ante, int *limit, double *sb_bb, double *bb_BB, bool *isfinaltable,
									  bool *found_handnumber, bool *found_sblind, bool *found_bblind, bool *found_bbet,
									  bool *found_ante, bool *found_limit, bool *found_sb_bb, bool *found_bb_BB) 
{
	int			place_in_format = 0, place_in_text = 0;
	CString		token = "", skip_str = "", number_type = "";
	double		number = 0.;
	CString		temp = "";

  // Skip on bogus out-pointer
	if (results == NULL) return;
  *results = "";

	while (place_in_format < format.GetLength() && place_in_text < text.GetLength()) 
	{
		// handnumber
		if (format.Mid(place_in_format,2) == "^h") 
		{
			place_in_format+=2;
			token = "";
			while (place_in_text < text.GetLength()
				&& (isdigit(text[place_in_text]) || (text[place_in_text] == '-')))
			{
				if (text[place_in_text] != '-')
					token += text[place_in_text];

				place_in_text+=1;
			}
			assert (results != NULL);
			results->Append("^h (handnumber)\t= " + token + "\r\n");

			if (handnumber!=NULL && found_handnumber!=NULL)
			{
				if (!*found_handnumber)
				{
					*handnumber = token.GetString(); 
					*found_handnumber = true; 
				}
			}
		}

		// dummy int
		else if (format.Mid(place_in_format,2) == "^d") 
		{
			place_in_format+=2;
			token = "";

			while (place_in_text<text.GetLength()
				&& (CString(text[place_in_text]).FindOneOf("$0123456789,¢km")))
			{
				token += (text[place_in_text]);
				place_in_text++;
			}

			assert (results != NULL);
			results->Append("^d (dummy int)\t= " + token + "\r\n");
		}

		// dummy float
		else if (format.Mid(place_in_format,2) == "^f") 
		{
			place_in_format+=2;
			token = "";

			while (CString(text[place_in_text]).FindOneOf("$0123456789,.¢ckm")!=-1 && place_in_text<text.GetLength())
			{
				token += text[place_in_text];
				place_in_text++;
			}

			assert (results != NULL);
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

			while (CString(text[place_in_text]).FindOneOf("$0123456789,.¢ckm")!=-1 && place_in_text<text.GetLength())
			{
				token += text[place_in_text];
				place_in_text++;
			}

			number = StringToMoney(token);
			temp = Number2CString(number);

			if (number_type == "^s")
			{
				assert (results != NULL);
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
				assert (results != NULL);	
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
				assert (results != NULL);  
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
				assert (results != NULL); 
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
				assert (results != NULL);
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
				assert (results != NULL); 
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
			while (format[place_in_format] != '^' && place_in_format < format.GetLength()) 
			{
				skip_str += format[place_in_format];
				place_in_format+=1;
			}
			while (text.Mid(place_in_text, skip_str.GetLength()) != skip_str &&
				   place_in_text < text.GetLength()) 
			{
   				token += text[place_in_text];
				place_in_text+=1;
			}
			if (text.Mid(place_in_text, skip_str.GetLength()) == skip_str) 
			{
   				token.Append(text.Mid(place_in_text, skip_str.GetLength()));
				place_in_text += skip_str.GetLength();
			}
			assert (results != NULL);
			results->Append("^* (skip string)\t= '" + token + "'\r\n");
		}

		// Roman numerals
		else if (format.Mid(place_in_format,2) == "^R") 
		{
			place_in_format+=2;
			token = "";

			// Get roman numeral
			while ((tolower(text[place_in_text]) == 'i' 
				|| tolower(text[place_in_text]) == 'v' 
				|| tolower(text[place_in_text]) == 'x' 
				|| tolower(text[place_in_text]) == 'l' 
				|| tolower(text[place_in_text]) == 'c') 
				&& place_in_text < text.GetLength()) 
			{
				token += (unsigned char)tolower(text[place_in_text]);
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

			assert (results != NULL);
			results->Append("^R (roman numeral)\t= " + token + "\r\n");

		}

		// Limit, no-limit, pot limit identifier
		else if (format.Mid(place_in_format,2) == "^L") 
		{
			place_in_format+=2;
			if (text.Mid(place_in_text,8).MakeLower() == "no limit" ||
				text.Mid(place_in_text,8).MakeLower() == "no-limit") 
			{
				assert (results != NULL);
				results->Append("^L (limit type)\t= " + text.Mid(place_in_text,8) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = k_gametype_NL; 
						*found_limit=true; 
					}
				}

				place_in_text += 8;
			}

			else if (text.Mid(place_in_text,7).MakeLower() == "nolimit") 
			{
				assert (results != NULL); 
				results->Append("^L (limit type)\t= " + text.Mid(place_in_text,7) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = k_gametype_NL; 
						*found_limit=true; 
					}
				}

				place_in_text += 7;
			}

			else if (text.Mid(place_in_text,2).MakeLower() == "nl") 
			{
				assert (results != NULL); 
				results->Append("^L (limit type)\t= " + text.Mid(place_in_text,2) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = k_gametype_NL; 
						*found_limit=true; 
					}
				}

				place_in_text += 2;
			}

			else if (text.Mid(place_in_text,9).MakeLower() == "pot limit" ||
				text.Mid(place_in_text,9).MakeLower() == "pot-limit") 
			{
				assert (results != NULL);  
				results->Append("^L (limit type)\t= " + text.Mid(place_in_text,9) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = k_gametype_PL; 
						*found_limit=true; 
					}
				}

				place_in_text += 9;
			}

			else if (text.Mid(place_in_text,8).MakeLower() == "potlimit") 
			{
				assert (results != NULL);  
				results->Append("^L (limit type)\t= " + text.Mid(place_in_text,8) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = k_gametype_PL; 
						*found_limit=true; 
					}
				}

				place_in_text += 8;
			}

			else if (text.Mid(place_in_text,2).MakeLower() == "pl") 
			{
				assert (results != NULL); 
				results->Append("^L (limit type)\t= " + text.Mid(place_in_text,2) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = k_gametype_PL; 
						*found_limit=true; 
					}
				}

				place_in_text += 2;
			}

			else if (text.Mid(place_in_text,11).MakeLower() == "fixed limit" ||
				text.Mid(place_in_text,11).MakeLower() == "fixed-limit") 
			{
				assert (results != NULL);  
				results->Append("^L (limit type)\t= " + text.Mid(place_in_text,11) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = k_gametype_FL; 
						*found_limit=true; 
					}
				}

				place_in_text += 11;
			}

			else if (text.Mid(place_in_text,10).MakeLower() == "fixedlimit") 
			{
				assert (results != NULL); 
				results->Append("^L (limit type)\t= " + text.Mid(place_in_text,10) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = k_gametype_FL; 
						*found_limit=true; 
					}
				}

				place_in_text += 10;
			}

			else if (text.Mid(place_in_text,2).MakeLower() == "fl") 
			{
				assert (results != NULL);  
				results->Append("^L (limit type)\t= " + text.Mid(place_in_text,2) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = k_gametype_FL; 
						*found_limit=true; 
					}
				}

				place_in_text += 2;
			}

			else if (text.Mid(place_in_text,5).MakeLower() == "limit") 
			{
				assert (results != NULL); 
				results->Append("^L (limit type)\t= " + text.Mid(place_in_text,5) + "\r\n");

				if (found_limit!=NULL && limit!=NULL) 
				{
					if(!*found_limit)
					{
						*limit = k_gametype_FL; 
						*found_limit=true; 
					}
				}

				place_in_text += 5;
			}
		}
		
		// All other exact matches
		else if (text[place_in_text] == format[place_in_format])
		{
			assert (results != NULL); 
			results->Append("exact match\t= '");

			while (text[place_in_text] == format[place_in_format] && place_in_text<text.GetLength() && place_in_format<format.GetLength()) 
			{
				assert (results != NULL);
				results += (text[place_in_text]);

				place_in_text+=1;
				place_in_format+=1;
			}

			assert (results != NULL); 
			results->Append("'\r\n");
		}

		// Match failed, exit loop
		else
		{
			place_in_format = format.GetLength()+1;
			place_in_text = text.GetLength()+1;
		}
	}
#ifdef OPENHOLDEM_PROGRAM
    write_log(preferences.debug_scraper(), 
      "[CTransform] parsed title string\n");
    write_log(preferences.debug_scraper(), 
      "[CTransform] text = %s, format = %s\n", 
      text, format);
    write_log(preferences.debug_scraper(), 
      "[CTransform] handnumber = %s, sblind = %f, bblind = %f\n",
      *handnumber,  *sblind, *bblind);
    write_log(preferences.debug_scraper(), 
      "[CTransform] bbet = %f, ante = %f, limit = %d, sb_bb = %f, bb_BB = %f\n",
      *bbet, *ante,  *limit, *sb_bb, *bb_BB);
    write_log(preferences.debug_scraper(), 
      "[CTransform] isfinaltable = %d, found_handnumber = %d, found_sblind = %d\n",
      *isfinaltable, *found_handnumber, *found_sblind);
    write_log(preferences.debug_scraper(), 
      "[CTransform] found_bblind = %d, found_bbet = %s, found_ante = %d\n",
      *found_bblind, *found_bbet, *found_ante);
    write_log(preferences.debug_scraper(), 
      "[CTransform] found_limit = %d, found_sb_bb = %d, found_bb_BB = %d\n",
      *found_limit, *found_sb_bb, *found_bb_BB);
#endif
}

const double CTransform::StringToMoney(const CString inStr)
{
	const char			*str = inStr.GetString();
	CStringArray		possibleValues;
	CArray<int, int>	possibleValuesMultiplier;
	CString				activeValue = "";
	int					iValueWithCurrencySymbol = -1;
	bool				currencySymbol = false;

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
					int index = (int) possibleValues.Add(activeValue);
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
		int index = (int) possibleValues.Add(activeValue);
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
}
