#include "stdafx.h"

#include "dll_extension.h"
#include "symbols.h"
#include "global.h"
#include "scraper.h"
#include "poker_defs.h"
#include "debug.h"
#include "grammar.h"
#include "versus.h"
#include "MainFrm.h"

CDll	cdll;

CDll::CDll() {
#ifdef SEH_ENABLE
	// Set exception handler
	SetUnhandledExceptionFilter(MyUnHandledExceptionFilter);
#endif

#ifdef SEH_ENABLE
	try {
#endif
		hMod_dll = NULL;
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDll::Constructor :\n"); 
		throw;
	}
#endif
}


void CDll::pass_state_to_dll(holdem_state *pstate) {
#ifdef SEH_ENABLE
	try {
#endif

		if (hMod_dll==NULL) {
			return;
		}

		(process_message) ("state", pstate);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDll::pass_state_to_dll :\n"); 
		throw;
	}
#endif
}

void CDll::load_dll(char * path) {
#ifdef SEH_ENABLE
	try {
#endif
		CString t, formula_dll;
		int N, i;
		DWORD err1=0, err2=0;

		if (hMod_dll!=NULL) {
			return;
		}

		// try to load specific patch if passed in as a parameter
		if (strlen(path))
		{
			hMod_dll = LoadLibrary(path);
			err1 = GetLastError();

			// If DLL is not loaded, pop an error
			if (hMod_dll==NULL) 
			{
				t.Format("Unable to load DLL from:%s, error=%d\n", path, err1);
				MessageBox(NULL, t, "DLL Load Error", MB_OK);
				return;
			}
		}

		else
		{

			// Find DLL name to load from the formula file
			N = global.formula.mFunction.GetSize();
			formula_dll = "";
			for (i=0; i<N; i++) {
				if (global.formula.mFunction[i].func == "dll") {
					formula_dll = global.formula.mFunction[i].func_text;
					i = N + 1;
				}
			}
			formula_dll.Trim();

			// Try to load dll from the ##dll## section, if it is specified
			if (formula_dll != "") {
				t.Format("%s\\%s", startup_path, formula_dll.GetString());
				SetCurrentDirectory(startup_path);
				hMod_dll = LoadLibrary(t.GetString());
				err1 = GetLastError();
			}

			// If dll is still not loaded, load from name in Edit/Preferences
			if (hMod_dll==NULL) {
				t.Format("%s\\%s", startup_path, global.preferences.dll_name.GetString());
				SetCurrentDirectory(startup_path);
				hMod_dll = LoadLibrary(t.GetString());
				err2 = GetLastError();
			}

			// If it is still not loaded, pop an error
			if (hMod_dll==NULL) 
			{
				t.Format("Unable to load DLL from:\n%s, error=%d\n-or-\n%s, error=%d", 
					global.preferences.dll_name.GetString(), err1,
					formula_dll.GetString(), err2);
				MessageBox(NULL, t, "DLL Load Error", MB_OK);
				return;
			}
		}

		if (hMod_dll)
		{
			// Get address of process_message from dll
			// whuser.dll does not ship with a .def file by default - we must use the ordinal method to get the address
			//global.process_message = (process_message_t) GetProcAddress(global.hMod_dll, "process_message");
			process_message = (process_message_t) ::GetProcAddress(hMod_dll, (LPCSTR) 1);

			if (process_message==NULL) {
				t.Format("Unable to find process_message in dll");
				MessageBox(NULL, t, "DLL Load Error", MB_OK);
				FreeLibrary(hMod_dll);
				hMod_dll = NULL;
			}
			else {
				// pass "load" message
				(process_message) ("event", "load");

				// pass "pfgws" message
				(process_message) ("pfgws", GetSymbolFromDll);
			}
		}

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDll::load_dll :\n"); 
		throw;
	}
#endif
}

void CDll::unload_dll(void) {
#ifdef SEH_ENABLE
	try {
#endif
		if (hMod_dll==NULL) {
			return;
		}

		(process_message) ("event", "unload");

		if (FreeLibrary(hMod_dll)) {
			hMod_dll = NULL;
		}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("CDll::unload_dll :\n"); 
		throw;
	}
#endif
}

double GetSymbolFromDll(int chair, const char* name, bool& iserr) {
#ifdef SEH_ENABLE
	try {
#endif
		int			e, stopchar;
		double		res;
		CString		str;
		boost::spirit::tree_parse_info<>	tpi;
		bool		result;

		str.Format("%s", name);

		result = parse(&str, &tpi, &stopchar); 
		if (result) {
			e = SUCCESS;
			res = evaluate(&global.formula, tpi, &e);
		}
		else {
			res = 0;
			e = ERR_INVALID_FUNC_SYM;
		}

		if (e == SUCCESS) {
			iserr = false;
		}
		else {
			iserr = true;
		}

		return res;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("::GetSymbolFromDll :\n"); 
		throw;
	}
#endif
}
