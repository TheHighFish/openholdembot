//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Checking tablepoints on connection,
//   deciding if OH should reconnect if table-theme changes
//
//******************************************************************************

#include "stdafx.h"
#include "CTablepointChecker.h"

#include "CAutoConnector.h"
#include "CPreferences.h"
#include "CTableMapLoader.h"
#include "..\CTransform\CTransform.h"

CTablepointChecker::CTablepointChecker() {
}

CTablepointChecker::~CTablepointChecker() {
}

bool CTablepointChecker::CheckTablepoints(HWND window_candidate, int tablemap_index, RECT r) {
  BYTE *pBits = NULL;
  BYTE alpha = 0, red = 0, green = 0, blue = 0;
  int	width = 0, height = 0;
  int x = 0, y = 0;
  HDC	    hdcScreen = NULL, hdcCompatible = NULL;
  HBITMAP	hbmScreen = NULL, hOldScreenBitmap = NULL;
  CTransform	trans;

  if (tablemap_connection_data[tablemap_index].TablePointCount > 0) {
    for (int i = 0; i<tablemap_connection_data[tablemap_index].TablePointCount; i++) {
      // Allocate heap space for BITMAPINFO
      BITMAPINFO  *bmi;
      int         info_len = sizeof(BITMAPINFOHEADER) + 1024;
      bmi = (BITMAPINFO *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, info_len);

      // Check table points for match
      width = r.right - r.left;
      height = r.bottom - r.top;
      hdcScreen = GetDC(window_candidate);
      hdcCompatible = CreateCompatibleDC(hdcScreen);
      hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);
      hOldScreenBitmap = (HBITMAP)SelectObject(hdcCompatible, hbmScreen);
      BitBlt(hdcCompatible, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

      // Populate BITMAPINFOHEADER
      bmi->bmiHeader.biSize = sizeof(bmi->bmiHeader);
      bmi->bmiHeader.biBitCount = 0;
      GetDIBits(hdcCompatible, hbmScreen, 0, 0, NULL, bmi, DIB_RGB_COLORS);

      // Get the actual argb bit information
      bmi->bmiHeader.biHeight = -bmi->bmiHeader.biHeight;
      pBits = new BYTE[bmi->bmiHeader.biSizeImage];
      GetDIBits(hdcCompatible, hbmScreen, 0, height, pBits, bmi, DIB_RGB_COLORS);

      bool good_table_points = true;
      x = tablemap_connection_data[tablemap_index].TablePoint[i].left;
      y = tablemap_connection_data[tablemap_index].TablePoint[i].top;

      int pbits_loc = y*width * 4 + x * 4;
      alpha = pBits[pbits_loc + 3];
      red = pBits[pbits_loc + 2];
      green = pBits[pbits_loc + 1];
      blue = pBits[pbits_loc + 0];

      COLORREF Color = tablemap_connection_data[tablemap_index].TablePoint[i].color;
      // positive radius
      if (tablemap_connection_data[tablemap_index].TablePoint[i].radius >= 0)
      {
        if (!trans.IsInARGBColorCube((Color >> 24) & 0xff, // function GetAValue() does not exist
          GetRValue(Color),
          GetGValue(Color),
          GetBValue(Color),
          tablemap_connection_data[tablemap_index].TablePoint[i].radius,
          alpha, red, green, blue)) {
          good_table_points = false;
        }
      }
      // negative radius (logical not)
      else {
        if (trans.IsInARGBColorCube((Color >> 24) & 0xff, // function GetAValue() does not exist
          GetRValue(Color),
          GetGValue(Color),
          GetBValue(Color),
          -tablemap_connection_data[tablemap_index].TablePoint[i].radius,
          alpha, red, green, blue))
        {
          good_table_points = false;
        }
      }

      // Clean up
      HeapFree(GetProcessHeap(), NULL, bmi);
      delete[]pBits;
      SelectObject(hdcCompatible, hOldScreenBitmap);
      DeleteObject(hbmScreen);
      DeleteDC(hdcCompatible);
      ReleaseDC(window_candidate, hdcScreen);

      if (!good_table_points) {
        write_log(preferences.debug_tablemap_loader(), "[CTablemapLoader] Not all tablepoints match.\n");
        return false;
      }
    }
  }
  return true;
}

bool CTablepointChecker::CheckTablepointsOfCurrentTablemap() {
  assert(p_autoconnector != NULL);
  HWND current_table = p_autoconnector->attached_hwnd();
  CRect rect;
  GetClientRect(current_table, &rect);
  return true;
}