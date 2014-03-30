#ifndef INC_DEBUG_H
#define INC_DEBUG_H

// To enable seh:
// uncomment this 
// and add /EHa flag to the compiler options
//#define	SEH_ENABLE	1

LONG WINAPI MyUnHandledExceptionFilter(struct _EXCEPTION_POINTERS *lpExceptionInfo);
char * get_time(char * timebuf);
char * get_now_time(char * timebuf);
void logfatal (char* fmt, ...);
BOOL CreateBMPFile(const char *szFile, HBITMAP hBMP);

extern FILE *log_fp;

#endif /* INC_DEBUG_H */
