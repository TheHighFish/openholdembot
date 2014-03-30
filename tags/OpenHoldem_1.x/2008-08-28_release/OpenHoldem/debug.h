#ifndef INC_DEBUG_H
#define INC_DEBUG_H

// To enable automatic minidumps:
// "#define SEH_ENABLE_STACK 1" must be commented out
// "#define	SEH_ENABLE_HANDLER 1" must *not* be commented out
//
// If using SEH_ENABLE_STACK, then the /EHa flag must be set in the compiler options
//#define	SEH_ENABLE_STACK	1
#define	SEH_ENABLE_HANDLER	1

// This lets transform.cpp know which source tree is #include'ing it
#ifndef OPENHOLDEM_PROGRAM
#define OPENHOLDEM_PROGRAM
#endif

//  Three macros for exception handling
#ifdef SEH_ENABLE_STACK
#define __SEH_HEADER try {
#else
#define __SEH_HEADER
#endif

//  SEH_LOGFATAL
#ifdef SEH_ENABLE_STACK
#define __SEH_LOGFATAL(...) } \
	catch (...) \
	{ \
	    logfatal(__VA_ARGS__); \
		throw; \
	}
#else
#define __SEH_LOGFATAL(...)
#endif

//  SEH_SET_EXCEPTION_HANDLER
#ifdef SEH_ENABLE_HANDLER
#define __SEH_SET_EXCEPTION_HANDLER SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#else
#define __SEH_SET_EXCEPTION_HANDLER
#endif


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
