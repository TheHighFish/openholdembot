#ifndef INC_KEYBOARDDLL_H
#define INC_KEYBOARDDLL_H

#ifdef KEYBOARDDLL_EXPORTS
#define KEYBOARDDLL_API __declspec(dllexport)
#else
#define KEYBOARDDLL_API __declspec(dllimport)
#endif

typedef int (*keyboard_sendstring_t)(const CString s);

KEYBOARDDLL_API int SendString(const CString s);


#endif //INC_KEYBOARDDLL_H