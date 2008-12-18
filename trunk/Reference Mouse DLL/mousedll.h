#ifndef INC_MOUSEDLL_H
#define INC_MOUSEDLL_H

#ifdef MOUSEDLL_EXPORTS
#define MOUSEDLL_API __declspec(dllexport)
#else
#define MOUSEDLL_API __declspec(dllimport)
#endif

enum MouseButton { MouseLeft, MouseCenter, MouseRight };
typedef int (*mouse_click_t)(const RECT rect, const MouseButton button, const int clicks);

MOUSEDLL_API int MouseClick(const RECT rect, const MouseButton button, const int clicks);

#endif //INC_MOUSEDLL_H