#include "stdafx.h"

#include "CDllExtension.h"

#include "OpenHoldem.h"

#include "CSymbols.h"
#include "CPreferences.h"
#include "CFormula.h"
#include "CGrammar.h"
#include "CHandHistory.h"

#include "PokerChat.hpp"
#include "Cversus.h"
#include "CIteratorThread.h"
#include "OH_MessageBox.h"

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
	}
	else
	{
		// Nothing passed in, so we try the DLL of the formula
		// and the DLL from preferences.
		dll_path = GetDLLSpecifiedInFormula();
		if (dll_path == "")
		{
			dll_path = prefs.dll_name().GetString();
		}
	}

	if (dll_path == "")
	{
		// Nothing to do
		return;
	}
	SetCurrentDirectory(_startup_path);
	_hmod_dll = LoadLibrary(dll_path);
	DWORD dll_error = GetLastError();

	// If the DLL didn't get loaded
	if (_hmod_dll == NULL)
	{
		CString error_message;
		error_message.Format("Unable to load DLL from:\n%s, error=%d\n",
			 dll_path.GetString(), dll_error);
		OH_MessageBox(error_message, "DLL Load Error", MB_OK | MB_TOPMOST);
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
		OH_MessageBox(error_message, "DLL Load Error", MB_OK | MB_TOPMOST);

		FreeLibrary(_hmod_dll);
		_hmod_dll = NULL;
		return;
	}
	// pass "load" message
	(_process_message) ("event", "load");

	// pass "pfgws" message
	(_process_message) ("pfgws", GetSymbolFromDll);

	//pass "phl1k" message (address of handlist arrays)
	//  2008-03-22 Matrix
	(_process_message) ("phl1k", p_formula->formula()->inlist);

	//pass "prw1326" message (address of prw1326 structure)
	//  2008-05-08 Matrix
	(_process_message) ("prw1326", p_symbols->prw1326());
	
	//pass "history" message (address of history structure)
	//  2010-01-23 Demonthus
	(_process_message) ("history", p_handhistory->history());

	//  2008.02.27 by THF
	//
	//  pass "p_send_chat_message" message
	//
	//  Providing a pointer to the chat function,
	//	which can be used inside the dll,
	//	similar to "pfgws".
	//
	(_process_message)(pointer_for_send_chat_message,
					  GetPointerToSendChatMessage());
}

void CDllExtension::UnloadDll(void)
{
	if (_hmod_dll==NULL)
		return;

	p_symbols->set_prw1326_useme(0);
	(_process_message) ("event", "unload");
	if (FreeLibrary(_hmod_dll))
	{
		_hmod_dll = NULL;
	}
}

const bool CDllExtension::IsDllLoaded()
{
	return _hmod_dll != NULL;
}

double GetSymbolFromDll(const int chair, const char* name, bool& iserr)
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
		if (p_symbols->sym()->nit==0)
		{
			iter_vars.set_iterator_thread_complete(true);
		}
		else
		{
			if (p_iterator_thread)
				delete p_iterator_thread;

			p_iterator_thread = new CIteratorThread;
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
