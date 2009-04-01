#ifndef INC_MOUSEDLL_H
#define INC_MOUSEDLL_H

#ifdef MOUSEDLL_EXPORTS
#define MOUSEDLL_API __declspec(dllexport)
#else
#define MOUSEDLL_API __declspec(dllimport)
#endif

enum MouseButton { MouseLeft, MouseMiddle, MouseRight };

typedef int (*mouse_click_t)(const HWND hwnd, const RECT rect, const MouseButton button, const int clicks, 
							 const HWND restore_focus, const POINT restore_cursor);
MOUSEDLL_API int MouseClick(const HWND hwnd, const RECT rect, const MouseButton button, const int clicks, 
							const HWND restore_focus, const POINT restore_cursor);

typedef int (*mouse_clickdrag_t)(const HWND hwnd, const RECT rect, const HWND restore_focus, const POINT restore_cursor);
MOUSEDLL_API int MouseClickDrag(const HWND hwnd, const RECT rect, const HWND restore_focus, const POINT restore_cursor);

typedef void (*mouse_process_message_t)(const char *message, const void *param);
MOUSEDLL_API void ProcessMessage(const char *message, const void *param);

const POINT RandomizeClickLocation(const RECT rect);
const void GetClickPoint(const int x, const int y, const int rx, const int ry, POINT *p);
const double RandomNormalScaled(const double scale, const double m, const double s);
const double RandomNormal(const double m, const double s);

#endif //INC_MOUSEDLL_H