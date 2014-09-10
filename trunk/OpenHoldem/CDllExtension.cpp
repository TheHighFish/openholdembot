//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#include "stdafx.h"
#include "CDllExtension.h"

#include "CFilenames.h"
#include "CEngineContainer.h"
#include "CFunctionCollection.h"
#include "CHandresetDetector.h"
#include "CHandHistory.h"
#include "CIteratorThread.h"
#include "CPreferences.h"
#include "CSymbolEnginePrWin.h"
#include "CSymbolEngineVersus.h"
#include "debug.h"
#include "MagicNumbers.h"
#include "OH_MessageBox.h"
#include "OpenHoldem.h"
#include "PokerChat.hpp"

CDllExtension		*p_dll_extension = NULL;

CDllExtension::CDllExtension()
{
	_hmod_dll = NULL;
}

CDllExtension::~CDllExtension()
{
}

void CDllExtension::PassStateToDll(const SHoldemState *pstate)
{
	if (_hmod_dll==NULL)
		return;

	(_process_message) ("state", pstate);
}

void CDllExtension::Load(const char * path){
  CString		dll_path;
	if (IsLoaded()) {
     Unload(); 
  }
	// try to load specific path if passed in as a parameter
	if (strlen(path) > 0) {
		dll_path = path;
		write_log(preferences.debug_dll_extension(),
			"[CDLLExtension] setting path (1) to %s\n", dll_path);
	}	else {
		// Nothing passed in, so we try the DLL of the formula
		// and the DLL from preferences.
		dll_path = p_function_collection->DLLPath();
		write_log(preferences.debug_dll_extension(),
			"[CDLLExtension] setting path (2) to %s\n", dll_path);
		if (dll_path == "") {
			dll_path = preferences.dll_name().GetString();
			write_log(preferences.debug_dll_extension(),
				"[CDLLExtension] setting path (3) to %s\n", dll_path);
		}
	}
	if (dll_path == "")	{
		// Nothing to do
		return;
	}
	p_filenames->SwitchToOpenHoldemDirectory();
	_hmod_dll = LoadLibrary(dll_path);
	DWORD dll_error = GetLastError();

	// If the DLL didn't get loaded
	if (_hmod_dll == NULL) {
		CString error_message;
		error_message.Format("Unable to load DLL from:\n"
			"%s\n"
			"error-code: %d\n",
			 dll_path.GetString(), dll_error);
		OH_MessageBox_Error_Warning(error_message, "DLL Load Error");
		return;
	}
	// Get address of process_message from dll
	// user.dll, as defined in WinHoldem, does not ship with a .def file by default - we must use the ordinal method to get the address
	//global.process_message = (process_message_t) GetProcAddress(global._hmod_dll, "process_message");
	_process_message = (process_message_t) ::GetProcAddress(_hmod_dll, (LPCSTR) 1);

	if (_process_message==NULL)
	{
		CString error_message;
		error_message.Format("Unable to find process_message in dll");
		OH_MessageBox_Error_Warning(error_message, "DLL Load Error");

		FreeLibrary(_hmod_dll);
		_hmod_dll = NULL;
		return;
	}
	// pass "load" message
	(_process_message) ("event", "load");

	// No longer passing any pointers to the DLL.
	// We do no export functions an link them implicitly:
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=112&t=15470
}

void CDllExtension::Unload(void)
{
	if (_hmod_dll==NULL)
	{
		return;
	}
	(_process_message) ("event", "unload");
  assert(p_iterator_thread != NULL);
	p_iterator_thread->set_prw1326_useme(0);
	if (FreeLibrary(_hmod_dll))
	{
		_hmod_dll = NULL;
	}
}

const bool CDllExtension::IsLoaded()
{
	return _hmod_dll != NULL;
}

extern "C" __declspec(dllexport) double __stdcall GetSymbolFromDll(const int chair, const char* name, bool& iserr)
{
	int			stopchar = 0;
	double		res = 0.;
	CString		str = "";

	str.Format("%s", name);
	if (strcmp (str, "cmd$recalc") == 0)
	{
		// restart iterator thread
    p_iterator_thread->RestartPrWinComputations();
		
		// Recompute versus tables
		p_symbol_engine_versus->GetCounts ();
		iserr = false;
		return 0;
	}
  double result = k_undefined;
	p_engine_container->EvaluateSymbol(name, 
    &result, 
    preferences.dll_logging_enabled());
	iserr = false;
	return result;
}



extern "C" __declspec(dllexport) void __stdcall SendChatMessageFomDll(const char *msg)
{
	SendChatMessage((char *)msg);
}

extern "C" __declspec(dllexport) void* __stdcall GetPhl1kFromDll()
{
	// !!broken in OH 4.6.0 / 5.0.0 because hand list array removed
	return (void *)NULL; //(p_formula->formula()->inlist);
}

extern "C" __declspec(dllexport) void* __stdcall GetPrw1326FromDll()
{
  assert(p_iterator_thread != NULL);
	return (void *)(p_iterator_thread->prw1326());
}

extern "C" __declspec(dllexport) char* __stdcall GetHandnumberFromDll()
{
	assert(p_handreset_detector->GetHandNumber().GetLength() < k_max_length_of_handnumber);
	static char handnumber_as_char_array[k_max_length_of_handnumber];
	strcpy_s(handnumber_as_char_array, 
		k_max_length_of_handnumber, 
		p_handreset_detector->GetHandNumber());
	return handnumber_as_char_array;
}

extern "C" __declspec(dllexport) void __stdcall WriteLogFromDll(char* fmt, ...)
{
	// Docu about ellipsis and variadic macro:
	// http://msdn.microsoft.com/en-us/library/ms177415(v=vs.80).aspx
	// http://stackoverflow.com/questions/1327854/how-to-convert-a-variable-argument-function-into-a-macro
	va_list args;

	va_start(args, fmt);
	write_log_vl(preferences.dll_logging_enabled(), fmt, args);
	va_end(args);
}

