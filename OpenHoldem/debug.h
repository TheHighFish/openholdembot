#ifndef INC_DEBUG_H
#define INC_DEBUG_H

// This lets transform.cpp know which source tree is #include'ing it
#ifndef OPENHOLDEM_PROGRAM
#define OPENHOLDEM_PROGRAM
#endif

LONG WINAPI MyUnHandledExceptionFilter(EXCEPTION_POINTERS *pExceptionPointers);
char * get_time(char * timebuf);
char * get_now_time(char * timebuf);
void logfatal (char* fmt, ...);
BOOL CreateBMPFile(const char *szFile, HBITMAP hBMP);

void start_log(void);
void write_log(int level, char* fmt, ...);
void write_log_nostamp(int level, char* fmt, ...);
void write_logautoplay(int level, const char * action);
void stop_log(void);
void write_log_pokertracker(int level, char* fmt, ...);
int GenerateDump(EXCEPTION_POINTERS *pExceptionPointers);

extern FILE *log_fp;

#endif /* INC_DEBUG_H */
