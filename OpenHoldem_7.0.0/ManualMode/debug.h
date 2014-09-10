//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************


#ifndef INC_DEBUG_H
#define INC_DEBUG_H

LONG WINAPI MyUnHandledExceptionFilter(EXCEPTION_POINTERS *pExceptionPointers);
char * get_time(char * timebuf);
char * get_now_time(char * timebuf);
void logfatal (char* fmt, ...);
BOOL CreateBMPFile(const char *szFile, HBITMAP hBMP);
int GenerateDump(EXCEPTION_POINTERS *pExceptionPointers);

extern FILE *log_fp;

#endif /* INC_DEBUG_H */
