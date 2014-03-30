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

void CDllExtension::LoadDll(const char * path)
{
	CString		t = "", formula_dll = "";
	int			N = 0, i = 0;
	DWORD		err1 = 0, err2 = 0;

	if (_hmod_dll!=NULL)
		return;

	// try to load specific patch if passed in as a parameter
	if (strlen(path))
	{
		_hmod_dll = LoadLibrary(path);
		err1 = GetLastError();

		// If DLL is not loaded, pop an error
		if (_hmod_dll==NULL)
		{
			t.Format("Unable to load DLL from:%s, error=%d\n", path, err1);
			OH_MessageBox(t, "DLL Load Error", MB_OK | MB_TOPMOST);
			return;
		}
	}

	else
	{

		// Find DLL name to load from the formula file
		N = p_formula->formula()->mFunction.GetSize();
		formula_dll = "";
		for (i=0; i<N; i++)
		{
			if (p_formula->formula()->mFunction[i].func == "dll")
			{
				formula_dll = p_formula->formula()->mFunction[i].func_text;
				i = N + 1;
			}
		}
		formula_dll.Trim();

		// Try to load dll from the ##dll## section, if it is specified
		if (formula_dll != "")
		{
			t.Format("%s\\%s", _startup_path, formula_dll.GetString());
			SetCurrentDirectory(_startup_path);
			_hmod_dll = LoadLibrary(t.GetString());
			err1 = GetLastError();
		}

		// If dll is still not loaded, load from name in Edit/Preferences
		if (_hmod_dll==NULL)
		{
			t.Format("%s\\%s", _startup_path, prefs.dll_name().GetString());
			SetCurrentDirectory(_startup_path);
			_hmod_dll = LoadLibrary(t.GetString());
			err2 = GetLastError();
		}

		// If it is still not loaded, pop an error
		if (_hmod_dll==NULL)
		{
			t.Format("Unable to load DLL from:\n%s, error=%d\n-or-\n%s, error=%d",
				 prefs.dll_name().GetString(), err1,
				 formula_dll.GetString(), err2);
			OH_MessageBox(t, "DLL Load Error", MB_OK | MB_TOPMOST);
			return;
		}
	}

	if (_hmod_dll)
	{
		// Get address of process_message from dll
		// user.dll, as defined in WinHoldem, does not ship with a .def file by default - we must use the ordinal method to get the address
		//global.process_message = (process_message_t) GetProcAddress(global._hmod_dll, "process_message");
		_process_message = (process_message_t) ::GetProcAddress(_hmod_dll, (LPCSTR) 1);

		if (_process_message==NULL)
		{
			t.Format("Unable to find process_message in dll");
			OH_MessageBox(t, "DLL Load Error", MB_OK | MB_TOPMOST);

			FreeLibrary(_hmod_dll);
			_hmod_dll = NULL;
		}
		else
		{
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
	}
}

void CDllExtension::UnloadDll(void)
{
	if (_hmod_dll==NULL)
		return;

	p_symbols->set_prw1326_useme(0);

	(_process_message) ("event", "unload");

	if (FreeLibrary(_hmod_dll))
		_hmod_dll = NULL;
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
