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

#ifndef INC_DEBUG_H
#define INC_DEBUG_H

#include "../CCritSec/CCritSec.h"

// This lets transform.cpp know which source tree is #include'ing it
#ifndef OPENHOLDEM_PROGRAM
#define OPENHOLDEM_PROGRAM
#endif

LONG WINAPI MyUnHandledExceptionFilter(EXCEPTION_POINTERS *pExceptionPointers);
char * get_time(char * timebuf);
char * get_now_time(char * timebuf);
BOOL CreateBMPFile(const char *szFile, HBITMAP hBMP);

void start_log();
void stop_log();
void write_log_vl(bool debug_settings_for_this_message, char* fmt, va_list vl);
void write_log(bool debug_settings_for_this_message, char* fmt, ...);
void write_log_nostamp(bool debug_settings_for_this_message, char* fmt, ...);
void write_logautoplay(const char * action);
int GenerateDump(EXCEPTION_POINTERS *pExceptionPointers);

extern FILE *log_fp;
extern CCritSec	log_critsec;  // Used to ensure only one thread at a time writes to log file

#define __TRACE write_log(true, "[Trace] %s line %i\n", __FILE__, __LINE__);
#define __MTRACE msgbox(0, CString(__LINE__), "Debug-Info", 0);

#endif /* INC_DEBUG_H */
