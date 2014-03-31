#include "stdafx.h"
#include "CDllExtension.h"

#include "CFilenames.h"
#include "CFormula.h"
#include "CGrammar.h"
#include "CHandresetDetector.h"
#include "CHandHistory.h"
#include "CIteratorThread.h"
#include "CIteratorVars.h"
#include "CPreferences.h"
#include "CSymbolEnginePrWin.h"
#include "Cversus.h"
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


CString CDllExtension::GetDLLSpecifiedInFormula()
{
	CString formula_dll;
	int N = p_formula->formula()->mFunction.GetSize();
	formula_dll = "";
	for (int i=0; i<N; i++)
	{
		if (p_formula->formula()->mFunction[i].func == "dll")
		{
			formula_dll = p_formula->formula()->mFunction[i].func_text;
			break;
		}
	}
	formula_dll.Trim();
	return formula_dll;
}


void CDllExtension::LoadDll(const char * path)
{
	CString		dll_path;

	if (_hmod_dll != NULL)
		return;

	// try to load specific path if passed in as a parameter
	if (strlen(path) > 0)
	{
		dll_path = path;
		write_log(true/*preferences.debug_dll_interface()*/,
			"[CDLLExtension] setting path (1) to %s\n", dll_path);
	}
	else
	{
		// Nothing passed in, so we try the DLL of the formula
		// and the DLL from preferences.
		dll_path = GetDLLSpecifiedInFormula();
		write_log(true/*preferences.debug_dll_interface()*/,
			"[CDLLExtension] setting path (2) to %s\n", dll_path);
		if (dll_path == "")
		{
			dll_path = preferences.dll_name().GetString();
			write_log(true/*preferences.debug_dll_interface()*/,
				"[CDLLExtension] setting path (3) to %s\n", dll_path);
		}
	}

	if (dll_path == "")
	{
		// Nothing to do
		return;
	}
	p_filenames->SwitchToOpenHoldemDirectory();
	_hmod_dll = LoadLibrary(dll_path);
	DWORD dll_error = GetLastError();

	// If the DLL didn't get loaded
	if (_hmod_dll == NULL)
	{
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

	// To do !! Create a function if necessary
	//pass "history" message (address of history structure)
	//  2010-01-23 Demonthus
	//(_process_message) ("history", p_handhistory->history());
}

void CDllExtension::UnloadDll(void)
{
	if (_hmod_dll==NULL)
	{
		return;
	}
	(_process_message) ("event", "unload");
	p_iterator_thread->set_prw1326_useme(0);
	if (FreeLibrary(_hmod_dll))
	{
		_hmod_dll = NULL;
	}
}

const bool CDllExtension::IsDllLoaded()
{
	return _hmod_dll != NULL;
}

extern "C" __declspec(dllexport) double __stdcall GetSymbolFromDll(const int chair, const char* name, bool& iserr)
{
	int			e = SUCCESS, stopchar = 0;
	double		res = 0.;
	CString		str = "";
	tpi_type	tpi;
	bool		result = false;
	CGrammar	gram;

	str.Format("%s", name);

	if (strcmp (str, "cmd$recalc") == 0)
	{
		// restart iterator thread
		if (p_symbol_engine_prwin->nopponents_for_prwin() == 0)
		{
			iter_vars.set_iterator_thread_complete(true);
		}
		else
		{
			p_iterator_thread->RestartIteratorThread();
		}

		// Recompute versus tables
		p_versus->GetCounts ();
		iserr = false;
		return 0;
	}

	result = gram.ParseString(&str, (const SFormula *) p_formula, &tpi, &stopchar);

	if (result)
	{
		e = SUCCESS;
		res = gram.EvaluateTree(p_formula, tpi, NULL, &e);
	}
	else
	{
		res = 0;
		e = ERR_INVALID_FUNC_SYM;
	}

	if (e == SUCCESS)
		iserr = false;

	else
		iserr = true;

	return res;
}



extern "C" __declspec(dllexport) void __stdcall SendChatMessageFomDll(const char *msg)
{
	SendChatMessage((char *)msg);
}

extern "C" __declspec(dllexport) void* __stdcall GetPhl1kFromDll()
{
	return (void *)(p_formula->formula()->inlist);
}

extern "C" __declspec(dllexport) void* __stdcall GetPrw1326FromDll()
{
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
	// !! should true be replaced by an option?
	write_log_vl(true, fmt, args);
	va_end(args);
}

