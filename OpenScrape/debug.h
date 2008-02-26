#ifndef INC_DEBUG_H
#define INC_DEBUG_H

char * get_time(char * timebuf);
char * get_now_time(char * timebuf);
BOOL CreateBMPFile(const char *szFile, HBITMAP hBMP);

void start_log(void);
void write_log(char* fmt, ...);
void stop_log(void);

extern FILE *log_fp;

#endif /* INC_DEBUG_H */
