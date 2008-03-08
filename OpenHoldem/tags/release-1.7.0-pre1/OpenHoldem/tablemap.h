#ifndef INC_PROFILE_H
#define INC_PROFILE_H

#include "structs_defines.h"

void clear_tablemap(STableMap *map);
int load_tablemap(char *filename, STableMap *map, char *version, bool check_ws_date, int *linenum);

BOOL CALLBACK EnumProcTopLevelWindowList(HWND hwnd, LPARAM lparam);
bool check_window_match(STableMap *map, HWND h, RECT r, CString title);

#endif /* INC_PROFILE_H */