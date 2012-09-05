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

void start_log(void);
void write_log_nostamp(bool debug_settings_for_this_message, char* fmt, ...);
void write_logautoplay(const char * action);
void stop_log(void);
void write_log_pokertracker(bool debug_settings_for_this_message, char* fmt, ...);
int GenerateDump(EXCEPTION_POINTERS *pExceptionPointers);

// Exported to the DLL
// http://www.maxinmontreal.com/forums/viewtopic.php?f=112&t=15470
// http://www.codeguru.com/cpp/w-p/dll/article.php/c3649/Calling-an-Exported-Function-in-an-EXE-from-Within-a-DLL.htm
extern "C" void __declspec(dllexport) void write_log(bool debug_settings_for_this_message, char* fmt, ...);

extern FILE *log_fp;
extern CCritSec	log_critsec;  // Used to ensure only one thread at a time writes to log file

#endif /* INC_DEBUG_H */
