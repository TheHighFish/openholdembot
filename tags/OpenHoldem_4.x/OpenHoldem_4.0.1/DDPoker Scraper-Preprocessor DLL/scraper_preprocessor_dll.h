#ifndef INC_SCRAPER_PREPROCESSOR_DLL_H
#define INC_SCRAPER_PREPROCESSOR_DLL_H

#include <atlstr.h>

#ifdef SCRAPER_PREPROCESSOR_DLL_EXPORTS
#define SCRAPER_PREPROCESSOR_DLL_API __declspec(dllexport)
#else
#define SCRAPER_PREPROCESSOR_DLL_API __declspec(dllimport)
#endif

typedef void (*scraperpreprocessor_process_message_t)(const char *message, const void *param);
SCRAPER_PREPROCESSOR_DLL_API void ProcessMessage(const char *message, const void *param);

#endif //INC_SCRAPER_PREPROCESSOR_DLL_H