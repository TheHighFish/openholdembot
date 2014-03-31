// scraper_preprocessor_dll.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include "scraper_preprocessor_dll.h"
#include <conio.h>

HWND	*target_hwnd;

SCRAPER_PREPROCESSOR_DLL_API void ProcessMessage(const char *message, const void *param)
{
	if (message==NULL)  return;

	if (strncmp(message, "connect", 7)==0) 
		target_hwnd = (HWND *) param;

	if (strncmp(message, "disconnect", 10)==0) 
		target_hwnd = (HWND *) NULL;

	if (strncmp(message, "c0pot0", 6)==0)
	{
		CString *s = (CString *) param;
		_cprintf("c0pot0: %s\n", s);
	}
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		FreeConsole();
		break;
	}
    return true;
}

