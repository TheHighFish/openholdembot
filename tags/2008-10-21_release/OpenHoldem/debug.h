#ifndef INC_DEBUG_H
#define INC_DEBUG_H

// This lets transform.cpp know which source tree is #include'ing it
#ifndef OPENHOLDEM_PROGRAM
#define OPENHOLDEM_PROGRAM
#endif

//  SEH_SET_EXCEPTION_HANDLER
#define __SEH_SET_EXCEPTION_HANDLER SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);

LONG WINAPI MyUnHandledExceptionFilter(EXCEPTION_POINTERS *pExceptionPointers);
char * get_time(char * timebuf);
char * get_now_time(char * timebuf);
void logfatal (char* fmt, ...);
BOOL CreateBMPFile(const char *szFile, HBITMAP hBMP);

void start_log(void);
void write_log(char* fmt, ...);
void write_log_nostamp(char* fmt, ...);
void write_logautoplay(const char * action);
void stop_log(void);
int GenerateDump(EXCEPTION_POINTERS *pExceptionPointers);

extern FILE *log_fp;

#endif /* INC_DEBUG_H */
