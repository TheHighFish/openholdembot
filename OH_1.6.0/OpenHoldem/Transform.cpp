#include "StdAfx.h"
#include <Math.h>
#include "Transform.h"
#include "hash/lookup3.h"

CTransform::CTransform(void)
{
}

CTransform::~CTransform(void)
{
}

int CTransform::do_transform(STableMap *map, Stablemap_region *region, HDC hdc, CString *text, CString *separation, COLORREF *cr_avg) 
{
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
			return h_transform(map, region, hdc, text);
			break;

		case 'T':
			return t_transform(map, region, hdc, text, separation, background, character);
			break;

		case 'N':
			*text = "";
			return ERR_GOOD_SCRAPE_GENERAL;
			break;
	}

	return ERR_INVALID_TRANSFORM_TYPE;
}

int CTransform::c_transform(Stablemap_region *region, HDC hdc, CString *text, COLORREF *cr_avg) 
{
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
}

int CTransform::i_transform(Stablemap_region *region, HDC hdc, CString *text) 
{
	*text = "";
	return ERR_NO_IMAGE_MATCH;
}

int CTransform::h_transform(STableMap *map, Stablemap_region *region, HDC hdc, CString *text) 
{
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
	if (width >= MAX_HASH_WIDTH || height>=MAX_HASH_HEIGHT) {
		HeapFree(GetProcessHeap(), NULL, bmi);
		return ERR_FIELD_TOO_LARGE;
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
		num_precs = (int) map->p$.GetSize();
		pixcount = 0;
		for (j=0; j<num_precs; j++) 
		{
			if (map->p$[j].number == hash_type) 
			{
				x = map->p$[j].x;
				y = map->p$[j].y;
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
	for (j=0; j<(int) map->h$.GetSize(); j++) 
		hashes[j] = map->h$[j].hash;

	uresult = (uint32_t *) bsearch(&hash, &hashes, map->h$.GetSize(), 
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
		*text = map->h$[index].name;
	}

	// Clean up
	HeapFree(GetProcessHeap(), NULL, bmi);
	delete []pBits;

	return retval;
}


int CTransform::t_transform(STableMap *map, Stablemap_region *region, HDC hdc, CString *text, CString *separation, bool background[], bool character[][MAX_CHAR_HEIGHT])
{
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
	for (i=0; i<map->s$.GetSize(); i++)
	{
		if (map->s$[i].name.Left(1) == "t" &&
			map->s$[i].name.Mid(1,1) == region->transform.Mid(1,1) &&
			map->s$[i].name.Mid(2,4) == "type")
		{
			s$tXtype = map->s$[i].text;
			i = map->s$.GetSize() + 1;
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
		return do_plain_font_scan(map, width, height, background, character, text);

	// scan through OCR region - fuzzy
	else 
		return do_fuzzy_font_scan(map, region, width, height, background, character, 
								  s$tXtype.MakeLower()=="fuzzy" ? DEFAULT_FUZZY_TOLERANCE : atof(s$tXtype.GetString()) ,
								  text);
}

int CTransform::do_plain_font_scan(STableMap *map, int width, int height, bool bg[], bool (*ch)[MAX_CHAR_HEIGHT], CString *text)
{
	int					i, y, retval=ERR_TEXT_SCRAPE_NOMATCH;
	int					x_begin, x_end, y_begin, y_end;
	int					temp_right, vert_band_left = 0;
	bool				backg_band;
	CString				newchar;
	char				hexmash_array[512][MAX_SINGLE_CHAR_WIDTH*8 + 1];
	char				*cresult, hexmash[MAX_CHAR_WIDTH*16];

	// populate hexmash array
	for (i=0; i<(int) map->t$.GetSize(); i++)
		strcpy(hexmash_array[i], map->t$[i].hexmash);

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
			cresult = (char *) bsearch( hexmash, hexmash_array, map->t$.GetSize(), sizeof(hexmash_array[0]),
										(int (*)(const void*, const void*)) compare_font);

			// Found match, save char and move on
			if (cresult!=NULL) 
			{
				retval = ERR_GOOD_SCRAPE_GENERAL;

				newchar.Format("%c", map->t$[((long)cresult - (long)hexmash_array[0]) / sizeof(hexmash_array[0])].ch);
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
}
int CTransform::do_fuzzy_font_scan(STableMap *map, Stablemap_region *region, int width, int height, 
								   bool bg[], bool (*ch)[MAX_CHAR_HEIGHT], double tolerance, CString *text)
{

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
		t_index = get_best_hd(map, region, width, height, bg, ch, vert_band_left, tolerance);

		if (t_index != -1)
		{
			retval = ERR_GOOD_SCRAPE_GENERAL;
			
			newchar.Format("%c", map->t$[t_index].ch);
			text->Append(newchar);

			vert_band_left = vert_band_left + map->t$[t_index].x_count;
		}
		else
		{

			vert_band_left += 1;
		}
	}

	return retval;
}
		
int CTransform::get_best_hd(STableMap *map, Stablemap_region *region, int width, int height, bool bg[], bool (*ch)[MAX_CHAR_HEIGHT], 
							int left, double tolerance)
{
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
		for (i=0; i<(int) map->t$.GetSize() && i<512; i++)
		{
			lit_pixels = 0;

			if (atoi(region->transform.Mid(1, 1).GetString()) == map->t$[i].group && 
				map->t$[i].x_count<=x+1)
			{
				tot_hd[x][i] = 0;
				for (j=0; j<map->t$[i].x_count && j<x+1; j++)
				{
					tot_hd[x][i] += hamdist(map->t$[i].x[j], hexval_array[j]);
					lit_pixels += bitcount(map->t$[i].x[j]);
				}

				weighted_hd[x][i] = (double) tot_hd[x][i] / lit_pixels;
			}

			else
			{
				tot_hd[x][i] = 999999;
				weighted_hd[x][i] = 999999;
			}


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
}

int CTransform::hamdist(unsigned int x, unsigned int y)
{
	int dist = 0, val = x ^ y;

	while(val)
	{
		++dist; 
		val &= val - 1;
	}

	return dist;
}


void CTransform::get_shift_left_down_indexes(int x_start, int width, int height, bool background[], bool character[][MAX_CHAR_HEIGHT], 
										  int *x_begin, int *x_end, int *y_begin, int *y_end)
{
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
}

bool CTransform::is_in_ARGB_color_cube(int center_a, int center_r, int center_g, int center_b, 
									   int radius, int pix_a, int pix_r, int pix_g, int pix_b) 
{
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
}

bool CTransform::is_in_RGB_color_cube(int center_r, int center_g, int center_b, int radius, int pix_r, int pix_g, int pix_b)
{
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
}

void CTransform::calc_hexmash(int left, int right, int top, int bottom, bool (*ch)[MAX_CHAR_HEIGHT], char *hexmash, bool withspace)
{
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
}

void CTransform::parse_string_bsl(CString text, CString format, CString *results,
								  double *handnumber, double *sblind, double *bblind, double *bbet, 
								  double *ante, int *limit, double *sb_bb, double *bb_BB, bool *istournament,
								  bool *found_handnumber, bool *found_sblind, bool *found_bblind, bool *found_bbet,
								  bool *found_ante, bool *found_limit, bool *found_sb_bb, bool *found_bb_BB) 
{
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
}

double CTransform::string_to_money(CString str)
{
	double		number=0;
	CString		token;
	int			place_in_text;

	str.Remove('$');
	str.Remove(',');

	place_in_text = 0;
	token = "";

	// handle numerics and decimals
	while ( str.Mid(place_in_text,1).FindOneOf("0123456789.") != -1 && place_in_text<str.GetLength()) 
	{
		token.Append(str.Mid(place_in_text,1));
		place_in_text+=1;
	}

	// convert string to number
	number = atof(token.GetString());

	// Handle trailers
	if (place_in_text < str.GetLength())
	{
		if (str.Mid(place_in_text,1).FindOneOf("¢c") != -1) 
			number /= 100;

		else if (str.Mid(place_in_text,1) == "k")
			number *= 1000;

		else if (str.Mid(place_in_text,1) == "m")
			number *= 1000000;
	}

	return number;

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

int	bitcount(unsigned int u) { 
	// returns count of number of bits set in the given value
	unsigned int uCount;

	 uCount = u 
			  - ((u >> 1) & 033333333333) 
			  - ((u >> 2) & 011111111111);
	 return 
	   ((uCount + (uCount >> 3)) 
		& 030707070707) % 63;
}
