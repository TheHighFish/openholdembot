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

// !! Bad code
// !! * Clone of OpenHoldem\debug.cpp 
// !! * Outdated
// !! But it works and there is nothing to touch ATM

#include "stdafx.h"

#include "..\dbghelp\dbghelp.h"

#include "MainFrm.h"
#include "OpenScrape.h"
#include "global.h"

//#include <vld.h>			// visual leak detector

FILE *log_fp = NULL;

char * get_time(char * timebuf) 
{
	// returns current system time in WH format
	time_t	ltime;
	char tmptime[30];
	
	time( &ltime );
	ctime_s(tmptime, 26, &ltime);
	tmptime[24]='\0';

	memcpy(timebuf, tmptime+20, 4); //yyyy
	*(timebuf+4) = '-';

	// mm
	if (memcmp(tmptime+4, "Jan", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '1'; }
	else if (memcmp(tmptime+4, "Feb", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '2'; }
	else if (memcmp(tmptime+4, "Mar", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '3'; }
	else if (memcmp(tmptime+4, "Apr", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '4'; }
	else if (memcmp(tmptime+4, "May", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '5'; }
	else if (memcmp(tmptime+4, "Jun", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '6'; }
	else if (memcmp(tmptime+4, "Jul", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '7'; }
	else if (memcmp(tmptime+4, "Aug", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '8'; }
	else if (memcmp(tmptime+4, "Sep", 3)==0)  { *(timebuf+5) = '0'; *(timebuf+6) = '9'; }
	else if (memcmp(tmptime+4, "Oct", 3)==0)  { *(timebuf+5) = '1'; *(timebuf+6) = '0'; }
	else if (memcmp(tmptime+4, "Nov", 3)==0)  { *(timebuf+5) = '1'; *(timebuf+6) = '1'; }
	else if (memcmp(tmptime+4, "Dec", 3)==0)  { *(timebuf+5) = '1'; *(timebuf+6) = '2'; }

	*(timebuf+7) = '-';
	memcpy(timebuf+8, tmptime+8, 2); //dd
	*(timebuf+10) = ' ';
	memcpy(timebuf+11, tmptime+11, 8); //HH:mm:ss
	*(timebuf+19) = '\0';

	return timebuf;
}

char * get_now_time(char * timebuf) 
{
	// returns current system time as a UNIX style string
	time_t	ltime;

	time( &ltime );
	ctime_s(timebuf, 26, &ltime);
	timebuf[24]='\0';
	return timebuf;
}

void logfatal (char* fmt, ...) 
{
    char		buff[10000] ;
    va_list		ap;
    char		fatallogpath[MAX_PATH];
    FILE		*fatallog;
    char		nowtime[26];

    sprintf_s(fatallogpath, MAX_PATH, "%s\\fatal error.log", _startup_path);
    
	if (fopen_s(&fatallog, fatallogpath, "a")==0)
	{
		va_start(ap, fmt);
		vsprintf_s(buff, 10000, fmt, ap);
		fprintf(fatallog, "%s> %s", get_now_time(nowtime), buff);

		va_end(ap);
		fclose(fatallog);
	}
}

void start_log(void) {
	if (log_fp==NULL) {
		CString fn;
		if (fopen_s(&log_fp, fn.GetString(), "a")==0)
		{
			write_log("! log file open\n");
			fprintf(log_fp, "----------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			fflush(log_fp);
		}
	}
}

void write_log(char* fmt, ...) {
	char		buff[10000] ;
	va_list		ap;
	char		nowtime[26];

	if (log_fp != NULL) {

		va_start(ap, fmt);
		vsprintf_s(buff, 10000, fmt, ap);
		fprintf(log_fp, "%s - %s", get_time(nowtime), buff);
		
		va_end(ap);

		fflush(log_fp);
	}
}

void stop_log(void) {
	if (log_fp != NULL) {
		write_log("! log file closed\n");
		fclose(log_fp);
		log_fp = NULL;
	}
}

