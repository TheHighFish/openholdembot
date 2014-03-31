#ifndef INC_KEYBOARDDLL_H
#define INC_KEYBOARDDLL_H

#ifdef KEYBOARDDLL_EXPORTS
#define KEYBOARDDLL_API __declspec(dllexport)
#else
#define KEYBOARDDLL_API __declspec(dllimport)
#endif

typedef int (*keyboard_sendstring_t)(const HWND hwnd, const RECT rect, const CString s, const bool use_comma,
									 const HWND restore_focus, const POINT restore_cursor);
KEYBOARDDLL_API int SendString(const HWND hwnd, const RECT rect, const CString s, const bool use_comma,
							   const HWND restore_focus, const POINT restore_cursor);

typedef int (*keyboard_sendkey_t)(const HWND hwnd, const RECT rect, UINT vkey,
								  const HWND restore_focus, const POINT restore_cursor);
KEYBOARDDLL_API int SendKey(const HWND hwnd, const RECT rect, UINT vkey,
							const HWND restore_focus, const POINT restore_cursor);

typedef void (*keyboard_process_message_t)(const char *message, const void *param);
KEYBOARDDLL_API void ProcessMessage(const char *message, const void *param);

const POINT RandomizeClickLocation(const RECT rect);
const void GetClickPoint(const int x, const int y, const int rx, const int ry, POINT *p);
const double RandomNormalScaled(const double scale, const double m, const double s);
const double RandomNormal(const double m, const double s);

#endif //INC_KEYBOARDDLL_H