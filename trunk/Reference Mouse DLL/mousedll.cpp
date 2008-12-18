// mousedll.cpp : Defines the entry point for the DLL application.
//

#include <windows.h>
#include "mousedll.h"

MOUSEDLL_API int MouseClick(const RECT rect, const MouseButton button, const int clicks)
{


	return 42;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return true;
}

