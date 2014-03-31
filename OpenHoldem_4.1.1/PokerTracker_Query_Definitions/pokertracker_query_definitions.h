#ifndef INC_MOUSEDLL_H
#define INC_MOUSEDLL_H

#ifdef MOUSEDLL_EXPORTS
#define MOUSEDLL_API __declspec(dllexport)
#else
#define MOUSEDLL_API __declspec(dllimport)
#endif

/*
typedef int (*mouse_click_t)(const HWND hwnd, const RECT rect, const MouseButton button, const int clicks, 
							 const HWND restore_focus, const POINT restore_cursor);
MOUSEDLL_API int MouseClick(const HWND hwnd, const RECT rect, const MouseButton button, const int clicks, 
							const HWND restore_focus, const POINT restore_cursor);
*/

int GetNumberOfStats();
CString GetQueryDefinition(int index, bool isomaha, bool istournament);
CString GetDescription(int index);
bool IsBasicStat(int index);
bool IsPositionalPreflopStat(int index);
bool isAdvancedStat(int index);

//IsValidSymbol?, GetPrefix?

#endif //INC_MOUSEDLL_H