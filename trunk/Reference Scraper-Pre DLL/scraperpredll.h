#ifndef INC_SCRAPERPREDLL_H
#define INC_SCRAPERPREDLL_H

#include <atlstr.h>

#ifdef SCRAPERPREDLL_EXPORTS
#define SCRAPERPREDLL_API __declspec(dllexport)
#else
#define SCRAPERPREDLL_API __declspec(dllimport)
#endif

typedef void (*scraperpre_process_message_t)(const char *message, const void *param);
SCRAPERPREDLL_API void ProcessMessage(const char *message, const void *param);

#endif //INC_SCRAPERPREDLL_H