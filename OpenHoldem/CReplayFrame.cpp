//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "StdAfx.h"
#include "CReplayFrame.h"

#include "Bitmaps.h"
#include "CEngineContainer.h"
#include "CHeartbeatThread.h"

#include "CScraper.h"
#include "CSessionCounter.h"
#include "CSymbolEngineActiveDealtPlaying.h"
#include "CSymbolEngineAutoplayer.h"
#include "CSymbolEngineDealerchair.h"
#include "CSymbolEngineUserchair.h"
#include "CSymbolEngineTableLimits.h"
#include "CTableState.h"
#include "..\CTablemap\CTablemap.h"
#include "..\DLLs\WindowFunctions_DLL\window_functions.h"
#include "OpenHoldem.h"
#include <io.h>

// GPÜI+ 0x0110 is needed for some parameters of Bitmep::CobertFormat
// https://docs.microsoft.com/en-us/answers/questions/72595/gdi-versions.html
#define GDIPVER 0x0110
#include <gdiplus.h>
#include <gdiplusheaders.h>
#include <Gdipluspixelformats.h>

using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

//  Sanity check: enough disk-space for a replay frame?
//	We assume, 10 MB are enough
//	  - a large table of 1000x1000 pixels a 4 byte
//	  - some KB for the HTML file.
//	  - some space for other processes	
const unsigned int FREE_SPACE_NEEDED_FOR_REPLAYFRAME = 10000000;  

// We always start with 0 amd overwrite existing frames.
// That's more simple and we usually are not interested
// in old frames of bygone sessions.
int CReplayFrame::_next_replay_frame = 0;

CReplayFrame::CReplayFrame(void) {
	// if replay frame current nb is 0 (it will most often happen at start)
	// then check first available file and use that one first
	if (0 == _next_replay_frame) {
		for (int i = 0 ; i < Preferences()->replay_max_frames() ; i++) {
			CString path = ReplayHTMLFilename(p_sessioncounter->session_id(), i);
			if (ENOENT == _access_s(path.GetString(), 0)) {
				_next_replay_frame = i;
				break;
			}
		}
	}
}

CReplayFrame::~CReplayFrame(void) {
  // As we create a new CReplayFrame for each frame
  // we simply have to increment the global counter once we are finished.
	++_next_replay_frame;
  if (_next_replay_frame >= Preferences()->replay_max_frames()) {
    // Once we reach the maximum overwrite old frames
    // in cyclic order, starting again from 0.
    _next_replay_frame = 0;
  }
}

void CReplayFrame::CreateReplayFrame(void){
	ULARGE_INTEGER	free_bytes_for_user_on_disk = {0}, 
	total_bytes_on_disk = {0}, 
	free_bytes_total_on_disk = {0};
	//  Sanity check: Enough disk-space for replay frame?	
	GetDiskFreeSpaceEx(
		OpenHoldemDirectory(),  //  Directory on disk of interest
		&free_bytes_for_user_on_disk,  
		&total_bytes_on_disk,	
		&free_bytes_total_on_disk);
	if (free_bytes_for_user_on_disk.QuadPart < FREE_SPACE_NEEDED_FOR_REPLAYFRAME) {
		write_log(Preferences()->debug_replayframes(), "[CReplayFrame] Not enough disk-space\n");
		MessageBox_Error_Warning("Not enough disk space to create replay-frame.");
		return;
	}
	// Get exclusive access to CScraper and CSymbols variables
	// (Wait for scrape/symbol cycle to finish before saving frame)
	EnterCriticalSection(&p_heartbeat_thread->cs_update_in_progress);
	CString next_frame;
	next_frame.Format("[%06d]", _next_replay_frame);
	// Replay-frame should always be mentioned in the log for easy reference
	write_log(k_always_log_basic_information, "[CReplayFrame] Shooting frame %s\n", next_frame);
	CreateBitMapFile();
    CreateHTMLFile();
	LeaveCriticalSection(&p_heartbeat_thread->cs_update_in_progress);
}

void CReplayFrame::CreateHTMLFile() {
    FILE	*fp = NULL; 
    assert(p_sessioncounter != nullptr);
    CString path = ReplayHTMLFilename(p_sessioncounter->session_id(), _next_replay_frame);
    if (fopen_s(&fp, path.GetString(), "w") == 0) {
        write_log(Preferences()->debug_replayframes(), "[CReplayFrame] Creating HTML file: %s\n", path);
        // First line has to be the "title" of the table.
        // This is no longer valid HTML, but the way Ray.E.Bornert did it
        // for WinHoldem and WinScrape.
        fprintf(fp, "%s\n", LPCSTR(p_table_state->TableTitle()->Title()));
        fprintf(fp, "<br>\n");
        // HTML header
        fprintf(fp, "<html>\n");
        fprintf(fp, "  <head>\n");
        fprintf(fp, "    <title>%s</title>\n", LPCSTR(p_table_state->TableTitle()->Title()));
        fprintf(fp, "  </head>");
        fprintf(fp, "<body>\n");
        // Bitmap image
        fprintf(fp, "<img src=\"frame%06d.bmp\">\n", _next_replay_frame);
        fprintf(fp, "<br>\n");
        // End of HTML
        fprintf(fp, "</body>\n</html>\n");
        fclose(fp);
    }
}

void CReplayFrame::CreateReplaySessionDirectoryIfNecessary() {
	assert(p_sessioncounter != nullptr);
	CString path = ReplaySessionDirectory(p_sessioncounter->session_id());
	if (GetFileAttributes(path.GetString()) == INVALID_FILE_ATTRIBUTES)	{
    write_log(Preferences()->debug_replayframes(), "[CReplayFrame] Creating replay directory %s\n", path);
		SHCreateDirectoryEx(NULL, path.GetString(), NULL);
	}
}

void CReplayFrame::CreateBitMapFile() {
	CreateReplaySessionDirectoryIfNecessary();
	CStringW path = CA2W(ReplayBitmapFilename(p_sessioncounter->session_id(), _next_replay_frame));
	// There is a BITMAP-structure (wingdi)
	// https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmap
	// and a Bitmap class (gdiplus)
	// https://docs.microsoft.com/en-us/windows/win32/api/gdiplusheaders/nl-gdiplusheaders-bitmap
	// First create a BITMAP structure, just for debuggng output
	HBITMAP hBMP = p_scraper->entire_window_cur();
	BITMAP s_bitmap;
	GetObject(hBMP, sizeof(BITMAP), &s_bitmap);
	write_log(Preferences()->debug_replayframes(), "[CReplayFrame] width_s: %i\n", s_bitmap.bmWidth);
	// Initialiize GDIplus
	// Do NOT move this code to DLL-main,
	// when replay-frames get put into a DLL
	// https://docs.microsoft.com/en-us/windows/win32/api/gdiplusinit/nf-gdiplusinit-gdiplusstartup
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	write_log(Preferences()->debug_replayframes(), "GDIplis iniialized\n");
	// Create palette for bitmap conversion
	HDC hDDC = GetDC(GetDesktopWindow());
	HPALETTE hPalette = (HPALETTE)GetCurrentObject(hDDC, OBJ_PAL);
	LOGPALETTE s_palette;
	GetObject(hPalette, sizeof(LOGPALETTE), &s_palette);;
	write_log(Preferences()->debug_replayframes(), "Palette version %i\n", s_palette.palVersion);
	write_log(Preferences()->debug_replayframes(), "Palette entries %i\n", s_palette.palNumEntries);
	// Create a Bitmap class
	Bitmap c_bitmap(hBMP, hPalette);
	write_log(Preferences()->debug_replayframes(), "[CReplayFrame] width c: %i\n", c_bitmap.GetWidth());
	//  Get the cokour palete
	//get palette from GDI+
    int palsize = c_bitmap.GetPaletteSize();
    ColorPalette *palette = (Gdiplus::ColorPalette*)new BYTE[palsize];
	//ColorPalette s_color_palette
    c_bitmap.GetPalette(palette, palsize);
	//c_bitmap.GetPalette(&s_color_palette,);
	//CreatePalette(&c_bitmap, &s_color_palette);
	write_log(Preferences()->debug_replayframes(), "[CReplayFrame] Palette created\n");
	
	
	// Removing the alpha-chammel from (A)RGB
	/*c_bitmap.ConvertFormat(PixelFormat32bppRGB, 
        DitherTypeNone, 
        PaletteTypeCustom,
        NULL,
        0);
		*/
    // Save bitmap
	CLSID bmpClsid = ImageFormatBMP;
    write_log(Preferences()->debug_replayframes(), "[CReplayFrame] CLSID created\n");
    c_bitmap.Save(path, &bmpClsid, NULL);
	// Ckean upi
    write_log(Preferences()->debug_replayframes(), "[CReplayFrame] saved\n");
	GdiplusShutdown(gdiplusToken);
    write_log(Preferences()->debug_replayframes(), "[CReplayFrame] shutdown\n");
}
/*)
	//Bitmap c_bitmap(); 
	/
		c_bitmap.FromBITMAPINFO(s_bitmap);
	write_log(Preferences()->debug_replayframes(), "[CReplayFrame] width_c: %i\n", c_bitmap.GetWidth());
	
	Bitmap c_bitmap(hBMP, hPalette);
	//write_log(Preferences()->debug_replayframes(), "[CReplayFrame] width 5: %i\n", c_bitmap.GetWidth());
	c_bitmap.FromHBITMAP(hBMP, hPalette);
	write_log(Preferences()->debug_replayframes(), "[CReplayFrame] width 6: %i\n", c_bitmap.GetWidth());
    
    //Bitmap  bitmap(1, 1, PixelFormat32bppARGB);
    //GetObject(hBMP, sizeof(Bitmap), &bitmap);
	//
    // https://docs.microsoft.com/en-us/windows/win32/api/gdiplusheaders/nf-gdiplusheaders-bitmap-convertformat
    write_log(Preferences()->debug_replayframes(), "[CReplayFrame] width: %i\n", c_bitmap.GetWidth());
    //write_log(Preferences()->debug_replayframes(), "[CReplayFrame] width2: %i\n", bitmap.bmWidth);
	//write_log(Preferences()->debug_replayframes(), "[CReplayFrame] width4: %i\n", bitmap.bmWidth);
    

    /*BITMAP  bn;
    GetObject(hBMP, sizeof(BITMAP), &bm);
    write_log(Preferences()->debug_replayframes(), "[CReplayFrame] width5: %i\n", bm.bmWidth);*

    );
}
*/
