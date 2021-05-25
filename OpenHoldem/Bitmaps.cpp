//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Scraping the poker-table and providing access to the scraped data.
//  As the CScraper is low-level and quite large we created 
//  a interface SCraperAccess that provides higher-level accessors
//  like "UserHasCards()".
//  Better use that interface to access scraper-data whenever possible.
//
//******************************************************************************

#include "StdAfx.h"
#include "Bitmaps.h"

bool BitmapsAreEqual(HBITMAP HBitmapLeft, HBITMAP HBitmapRight) 
{
	if (HBitmapLeft == HBitmapRight)
		return true;

	if ((HBitmapLeft == NULL) || (HBitmapRight == NULL))
		return false;

	bool bSame = false;

	HDC hdc = GetDC(NULL);
	BITMAPINFO BitmapInfoLeft = {0};
	BITMAPINFO BitmapInfoRight = {0};

	BitmapInfoLeft.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfoRight.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	if (0 != GetDIBits(hdc, HBitmapLeft, 0, 0, NULL, &BitmapInfoLeft, DIB_RGB_COLORS) &&
			0 != GetDIBits(hdc, HBitmapRight, 0, 0, NULL, &BitmapInfoRight, DIB_RGB_COLORS))
	{
		// Compare the BITMAPINFOHEADERs of the two bitmaps
		if (0 == memcmp(&BitmapInfoLeft.bmiHeader, &BitmapInfoRight.bmiHeader, sizeof(BITMAPINFOHEADER)))
		{
			// The BITMAPINFOHEADERs are the same so now compare the actual bitmap bits
			BYTE *pLeftBits = new BYTE[BitmapInfoLeft.bmiHeader.biSizeImage];
			BYTE *pRightBits = new BYTE[BitmapInfoRight.bmiHeader.biSizeImage];
			BYTE *pByteLeft = NULL;
			BYTE *pByteRight = NULL;

			PBITMAPINFO pBitmapInfoLeft = &BitmapInfoLeft;
			PBITMAPINFO pBitmapInfoRight = &BitmapInfoRight;

			// calculate the size in BYTEs of the additional
			// memory needed for the bmiColor table
			int AdditionalMemory = 0;
			switch (BitmapInfoLeft.bmiHeader.biBitCount)
			{
			case 1:
				AdditionalMemory = 1 * sizeof(RGBQUAD);
				break;
			case 4:
				AdditionalMemory = 15 * sizeof(RGBQUAD);
				break;
			case 8:
				AdditionalMemory = 255 * sizeof(RGBQUAD);
				break;
			case 16:
			case 32:
				AdditionalMemory = 2 * sizeof(RGBQUAD);
			}

			if (AdditionalMemory)
			{
				// we have to allocate room for the bmiColor table that will be
				// attached to our BITMAPINFO variables
				pByteLeft = new BYTE[sizeof(BITMAPINFO) + AdditionalMemory];
				if (pByteLeft)
				{
					memset(pByteLeft, 0, sizeof(BITMAPINFO) + AdditionalMemory);
					memcpy(pByteLeft, pBitmapInfoLeft, sizeof(BITMAPINFO));
					pBitmapInfoLeft = (PBITMAPINFO)pByteLeft;
				}

				pByteRight = new BYTE[sizeof(BITMAPINFO) + AdditionalMemory];
				if (pByteRight)
				{
					memset(pByteRight, 0, sizeof(BITMAPINFO) + AdditionalMemory);
					memcpy(pByteRight, pBitmapInfoRight, sizeof(BITMAPINFO));
					pBitmapInfoRight = (PBITMAPINFO)pByteRight;
				}
			}

			if (pLeftBits && pRightBits && pBitmapInfoLeft && pBitmapInfoRight)
			{
				// zero out the bitmap bit buffers
				memset(pLeftBits, 0, BitmapInfoLeft.bmiHeader.biSizeImage);
				memset(pRightBits, 0, BitmapInfoRight.bmiHeader.biSizeImage);

				// fill the bit buffers with the actual bitmap bits
				if (0 != GetDIBits(hdc, HBitmapLeft, 0, pBitmapInfoLeft->bmiHeader.biHeight, pLeftBits, pBitmapInfoLeft, DIB_RGB_COLORS) &&
						0 != GetDIBits(hdc, HBitmapRight, 0, pBitmapInfoRight->bmiHeader.biHeight, pRightBits, pBitmapInfoRight, DIB_RGB_COLORS))
				{
					// compare the actual bitmap bits of the two bitmaps
					bSame = (0 == memcmp(pLeftBits, pRightBits, pBitmapInfoLeft->bmiHeader.biSizeImage));
				}
			}

			// clean up
			delete[] pLeftBits;
			delete[] pRightBits;
			delete[] pByteLeft;
			delete[] pByteRight;
		}
	}
	ReleaseDC(NULL, hdc);

	return bSame;
}

void ClearAlphaChannel(HBITMAP hBitmap, bool fast_exit_assuming_all_alphas_equal) {
    MessageBox(0, "ClearAlphaChannel 0", "ClearAlphaChannel", 0);
    if (hBitmap == NULL) {
        return;
    }
    MessageBox(0, "1", "ClearAlphaChannel", 0);
    BITMAP bitmapInfo;
    GetObject(hBitmap, sizeof(BITMAP), &bitmapInfo);
    MessageBox(0, "2", "ClearAlphaChannel", 0);
    INT32 *pixels = (INT32*)bitmapInfo.bmBits;
    MessageBox(0, "3", "ClearAlphaChannel", 0);
    if (pixels == NULL) {
        MessageBox(0, "3a", "ClearAlphaChannel", 0);
        return;
    }
    INT32 first_pixel = pixels[0];
    MessageBox(0, "4", "ClearAlphaChannel", 0);
    INT32 first_alpha = first_pixel & 0x000000FF;
    MessageBox(0, "5", "ClearAlphaChannel", 0);
    if (fast_exit_assuming_all_alphas_equal && !first_alpha) {
        // alpha already is zero
        return;
    }
    MessageBox(0, "6", "ClearAlphaChannel", 0);
    int n_pixels = bitmapInfo.bmWidth *  bitmapInfo.bmHeight;
    MessageBox(0, "7", "ClearAlphaChannel", 0);
    for (int i = 0; i < n_pixels; ++i) {
        // Clear alpha, keep all the rest, format is RGBA
        MessageBox(0, "8", "ClearAlphaChannel", 0);
        pixels[i] &= 0xFFFFFF00;
        MessageBox(0, "9", "ClearAlphaChannel", 0);
    }
}
