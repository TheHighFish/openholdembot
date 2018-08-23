//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Debug-DLL for OpenHoldem
//
//******************************************************************************

#define DEBUG_DLL_EXPORTS

#include "debug.h"
#include "afxwin.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include <time.h>
#include "..\..\Shared\CCritSec\CCritSec.h"
#include "..\..\Shared\MagicNumbers\MagicNumbers.h"

#ifdef _DEBUG
// visual leak detector in debug-mode
// https://vld.codeplex.com/
#include <vld.h>			
#endif _DEBUG

FILE *log_fp = NULL;
int session_ID = kUndefined;
CCritSec log_critsec;  // Used to ensure only one thread at a time writes to log file
bool footer_needs_to_be_written = false;
const char *footer = "********************************************************************************\n";

void write_footer_if_necessary() {
  if (footer_needs_to_be_written == false) {
    return;
  }
  // First set footer_needs_to_be_written = false
  // otherwise we get an endless recursion
  footer_needs_to_be_written = false;
  write_log_nostamp(true, footer);
}

char *get_time(char * timebuf) {
  // returns current system time in WH format
  time_t	ltime;
  char tmptime[30];
  time(&ltime);
  ctime_s(tmptime, 26, &ltime);
  tmptime[24] = '\0';
  memcpy(timebuf, tmptime + 20, 4); //yyyy
  *(timebuf + 4) = '-';
  // mm
  if (memcmp(tmptime + 4, "Jan", 3) == 0) {
    *(timebuf + 5) = '0';
    *(timebuf + 6) = '1';
  }
  else if (memcmp(tmptime + 4, "Feb", 3) == 0) {
    *(timebuf + 5) = '0';
    *(timebuf + 6) = '2';
  }
  else if (memcmp(tmptime + 4, "Mar", 3) == 0) {
    *(timebuf + 5) = '0';
    *(timebuf + 6) = '3';
  }
  else if (memcmp(tmptime + 4, "Apr", 3) == 0) {
    *(timebuf + 5) = '0';
    *(timebuf + 6) = '4';
  }
  else if (memcmp(tmptime + 4, "May", 3) == 0) {
    *(timebuf + 5) = '0';
    *(timebuf + 6) = '5';
  }
  else if (memcmp(tmptime + 4, "Jun", 3) == 0) {
    *(timebuf + 5) = '0';
    *(timebuf + 6) = '6';
  }
  else if (memcmp(tmptime + 4, "Jul", 3) == 0) {
    *(timebuf + 5) = '0';
    *(timebuf + 6) = '7';
  }
  else if (memcmp(tmptime + 4, "Aug", 3) == 0) {
    *(timebuf + 5) = '0';
    *(timebuf + 6) = '8';
  }
  else if (memcmp(tmptime + 4, "Sep", 3) == 0) {
    *(timebuf + 5) = '0';
    *(timebuf + 6) = '9';
  }
  else if (memcmp(tmptime + 4, "Oct", 3) == 0) {
    *(timebuf + 5) = '1';
    *(timebuf + 6) = '0';
  }
  else if (memcmp(tmptime + 4, "Nov", 3) == 0) {
    *(timebuf + 5) = '1';
    *(timebuf + 6) = '1';
  }
  else if (memcmp(tmptime + 4, "Dec", 3) == 0) {
    *(timebuf + 5) = '1';
    *(timebuf + 6) = '2';
  }
  *(timebuf + 7) = '-';
  memcpy(timebuf + 8, tmptime + 8, 2); //dd
  *(timebuf + 10) = ' ';
  memcpy(timebuf + 11, tmptime + 11, 8); //HH:mm:ss
  *(timebuf + 19) = '\0';
  return timebuf;
}

char *get_now_time(char * timebuf) {
  // returns current system time as a UNIX style string
  time_t	ltime;
  time(&ltime);
  ctime_s(timebuf, 26, &ltime);
  timebuf[24] = '\0';
  return timebuf;
}

CString log_filename() {
  assert(session_ID >= 0);
  CString log_filename;
  log_filename.Format("log\OH_%2d.log");
  return log_filename;
}

void delete_log() {
  // Log file must not be open
  remove(log_filename().GetString());
}

void clear_log() {
  stop_log();
  assert(session_ID > 0);
  start_log(session_ID, true);
}

void start_log(int current_session_iD, bool delete_old_log) {
  if (log_fp != NULL) {
    return;
  }
  assert(current_session_iD >= 0);
  session_ID = current_session_iD;
  CSLock lock(log_critsec);
  if (delete_old_log) {
    delete_log();
  }
  // Append (or create) log
  if ((log_fp = _fsopen(log_filename().GetString(), "a", _SH_DENYWR)) != 0) {
    write_log_separator(k_always_log_basic_information, "LOG FILE OPEN");
    fflush(log_fp);
  }
}

void write_log_vl(bool debug_settings_for_this_message, const char* fmt, va_list vl) {
  char		buff[10000];
  char		nowtime[26];
  write_footer_if_necessary();
  if (debug_settings_for_this_message == false) {
    return;
  }
  if (log_fp != NULL) {
    CSLock lock(log_critsec);
    vsprintf_s(buff, 10000, fmt, vl);
    get_time(nowtime);
    fprintf(log_fp, "%s > %s", nowtime, buff);
    fflush(log_fp);
  }
}

void write_log(bool debug_settings_for_this_message, const char* fmt, ...) {
  char		buff[10000];
  va_list		ap;
  char		nowtime[26];
  if (debug_settings_for_this_message == false) {
    return;
  }
  if (log_fp == NULL) {
    return;
  }
  write_footer_if_necessary();
  CSLock lock(log_critsec);
  va_start(ap, fmt);
  vsprintf_s(buff, 10000, fmt, ap);
  get_time(nowtime);
  fprintf(log_fp, "%s - %s", nowtime, buff);
  va_end(ap);
  fflush(log_fp);
}

void write_log_nostamp(bool debug_settings_for_this_message, const char* fmt, ...) {
  char		buff[10000];
  va_list		ap;
  write_footer_if_necessary();
  if (debug_settings_for_this_message == false) {
    return;
  }
  if (log_fp == NULL) {
    return;
  }
  CSLock lock(log_critsec);
  va_start(ap, fmt);
  vsprintf_s(buff, 10000, fmt, ap);
  fprintf(log_fp, "%s", buff);
  va_end(ap);
  fflush(log_fp);
}

void stop_log(void) {
  write_footer_if_necessary();
  if (log_fp == NULL) return;
  write_log_separator(k_always_log_basic_information, "LOG FILE CLOSED");
  fclose(log_fp);
  log_fp = NULL;
}

void write_log_separator(bool debug_settings_for_this_message, const char* header_message) {
  if ((header_message == NULL) || (strcmp(header_message, "") == 0)) {
    // Empty header, i.e. footer
    // Don't write it immediatelly to avoid multiple consecutive headers
    footer_needs_to_be_written = true;
    return;
  }
  // Write separator with header, skip potential footers
  footer_needs_to_be_written = false;
  char header[90];
  assert(strlen(footer) < 90);
  assert(strlen(header_message) < 60);
  // Copy the footer and \0 into the header
  memcpy(header, footer, strlen(footer) + 1);
  // Now copz the header-message into the header (without \0)
  memcpy((header + 10), header_message, strlen(header_message));
  header[9] = ' ';
  header[10 + strlen(header_message)] = ' ';
  write_log_nostamp(true, header);
}


/*!!!!


//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: String functions library
//
//******************************************************************************

#define _AFXDLL

// Needs to be defined here, before #include "window_functions.h"
// to generate proper export- and inport-definitions
#define STRING_FUNCTIONS_EXPORTS

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#include <afxwin.h>
#include <assert.h>
#include <math.h>
#include <windows.h>
#include "assert.h"
#include "string_functions.h"
#include "..\OpenHoldem\MagicNumbers.h"
#include "Psapi.h"

#ifdef OPENHOLDEM_PROGRAM
#include "..\OpenHoldem\OH_MessageBox.h"
#include "..\OpenHoldem\CSymbolEngineReplayFrameController.h"
#endif

const char kUnprintableBeepChar = 0x07;
const char kCharToBeRemoved = kUnprintableBeepChar;

CString Bool2CString(bool b) {
  return (b ? "true" : "false");
}

BOOL CreateBMPFile(const char *szFile, HBITMAP hBMP) {
// Saves the hBitmap as a bitmap.
HDC					hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
HDC					hdcCompatible = CreateCompatibleDC(hdcScreen);
PBITMAPINFO			pbmi = NULL;
BOOL				bret = FALSE;
HANDLE				hf = NULL; // file handle
BITMAPFILEHEADER	hdr; // bitmap file-header
PBITMAPINFOHEADER	pbih = NULL; // bitmap info-header
LPBYTE				lpBits = NULL; // memory pointer
DWORD				dwTotal = 0; // total count of bytes
DWORD				cb = 0; // incremental count of bytes
BYTE				*hp = NULL; // byte pointer
DWORD				dwTmp = 0;
BITMAP				bmp;

memset(&bmp, 0, sizeof(BITMAP));
GetObject(hBMP, sizeof(BITMAP), &bmp);
memset(&hdr, 0, sizeof(hdr));

int cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
if (cClrBits>8) {
// No Palette (normally)
pbmi = (PBITMAPINFO)calloc(1, sizeof(BITMAPINFOHEADER));
}
else {
pbmi = (PBITMAPINFO)calloc(1, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << (min(8, cClrBits))));
pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
}

// Initialize the fields in the BITMAPINFO structure.
pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

// Retrieve the color table (RGBQUAD array) and the bits
// (array of palette indices) from the DIB.
if (!GetDIBits(hdcCompatible, hBMP, 0, bmp.bmHeight, NULL, pbmi, DIB_RGB_COLORS)) {
goto to_return;
}
pbih = &(pbmi->bmiHeader);
pbmi->bmiHeader.biCompression = BI_RGB;
lpBits = (LPBYTE)calloc(1, pbih->biSizeImage);

if (!lpBits) {
goto to_return;
}

if (!GetDIBits(hdcCompatible, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS)) {
goto to_return;
}

// Create the .BMP file.
hf = CreateFile(szFile, GENERIC_READ | GENERIC_WRITE, (DWORD)0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
if (hf == INVALID_HANDLE_VALUE) {
goto to_return;
}
hdr.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M"
// Compute the size of the entire file.
hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
hdr.bfReserved1 = 0;
hdr.bfReserved2 = 0;

// Compute the offset to the array of color indices.
hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);

// Copy the BITMAPFILEHEADER into the .BMP file.
if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, NULL)) {
goto to_return;
}

// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD), (LPDWORD)&dwTmp, (NULL))) {
goto to_return;
}

// Copy the array of color indices into the .BMP file.
dwTotal = cb = pbih->biSizeImage;
hp = lpBits;
if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL)) {
goto to_return;
}

// Close the .BMP file.
if (!CloseHandle(hf)) {
goto to_return;
}
bret = TRUE;

to_return:
;
// Free memory.
if (pbmi)free(pbmi);
if (lpBits) {
free(lpBits);
}
DeleteDC(hdcCompatible);
DeleteDC(hdcScreen);
return bret;
}


struct stat file_stats = { 0 };
if (stat(fn.GetString(), &file_stats) == 0) {
unsigned long int max_file_size = 1E06 * preferences.log_max_logsize();
size_t file_size = file_stats.st_size;
if (file_size > max_file_size) {
delete_log();
}
}

*/