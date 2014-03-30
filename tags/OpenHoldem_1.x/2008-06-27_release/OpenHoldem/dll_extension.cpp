#include "stdafx.h"

#include "dll_extension.h"
#include "global.h"
#include "debug.h"
#include "grammar.h"
//  2008.02.27 by THF
//  PokerChat
#include "PokerChat.hpp"
#include "symbols.h" //2008-05-08 Matrix (for prw1326 mesage)

CDll	cdll;

CDll::CDll()
{

    __SEH_SET_EXCEPTION_HANDLER(MyUnHandledExceptionFilter);


    __SEH_HEADER

    hMod_dll = NULL;


    __SEH_LOGFATAL("CDll::Constructor :\n");

}


void CDll::pass_state_to_dll(holdem_state *pstate)
{
    __SEH_HEADER

    if (hMod_dll==NULL)
        return;

    (process_message) ("state", pstate);


    __SEH_LOGFATAL("CDll::pass_state_to_dll :\n");

}

void CDll::load_dll(char * path)
{
    __SEH_HEADER
    CString t, formula_dll;
    int N, i;
    DWORD err1=0, err2=0;

    if (hMod_dll!=NULL)
        return;

    // try to load specific patch if passed in as a parameter
    if (strlen(path))
    {
        hMod_dll = LoadLibrary(path);
        err1 = GetLastError();

        // If DLL is not loaded, pop an error
        if (hMod_dll==NULL)
        {
            t.Format("Unable to load DLL from:%s, error=%d\n", path, err1);
            MessageBox(NULL, t, "DLL Load Error", MB_OK | MB_TOPMOST);
            return;
        }
    }

    else
    {

        // Find DLL name to load from the formula file
        N = global.formula.mFunction.GetSize();
        formula_dll = "";
        for (i=0; i<N; i++)
        {
            if (global.formula.mFunction[i].func == "dll")
            {
                formula_dll = global.formula.mFunction[i].func_text;
                i = N + 1;
            }
        }
        formula_dll.Trim();

        // Try to load dll from the ##dll## section, if it is specified
        if (formula_dll != "")
        {
            t.Format("%s\\%s", global.startup_path, formula_dll.GetString());
            SetCurrentDirectory(global.startup_path);
            hMod_dll = LoadLibrary(t.GetString());
            err1 = GetLastError();
        }

        // If dll is still not loaded, load from name in Edit/Preferences
        if (hMod_dll==NULL)
        {
            t.Format("%s\\%s", global.startup_path, global.preferences.dll_name.GetString());
            SetCurrentDirectory(global.startup_path);
            hMod_dll = LoadLibrary(t.GetString());
            err2 = GetLastError();
        }

        // If it is still not loaded, pop an error
        if (hMod_dll==NULL)
        {
            t.Format("Unable to load DLL from:\n%s, error=%d\n-or-\n%s, error=%d",
                     global.preferences.dll_name.GetString(), err1,
                     formula_dll.GetString(), err2);
            MessageBox(NULL, t, "DLL Load Error", MB_OK | MB_TOPMOST);
            return;
        }
    }

    if (hMod_dll)
    {
        // Get address of process_message from dll
        // whuser.dll does not ship with a .def file by default - we must use the ordinal method to get the address
        //global.process_message = (process_message_t) GetProcAddress(global.hMod_dll, "process_message");
        process_message = (process_message_t) ::GetProcAddress(hMod_dll, (LPCSTR) 1);

        if (process_message==NULL)
        {
            t.Format("Unable to find process_message in dll");
            MessageBox(NULL, t, "DLL Load Error", MB_OK | MB_TOPMOST);
            FreeLibrary(hMod_dll);
            hMod_dll = NULL;
        }
        else
        {
            // pass "load" message
            (process_message) ("event", "load");

            // pass "pfgws" message
            (process_message) ("pfgws", GetSymbolFromDll);

            //pass "phl1k" message (address of handlist arrays)
            //  2008-03-22 Matrix
            (process_message) ("phl1k",global.formula.inlist);

			//pass "prw1326" message (address of prw1326 structure)
			//  2008-05-08 Matrix
            (process_message) ("prw1326",&symbols.prw1326);

            //  2008.02.27 by THF
            //
            //  pass "p_send_chatMessage" message
            //
            //  Providing a pointer to the chat function,
            //    which can be used inside the dll,
            //    similar to "pfgws".
            //
            (process_message)(Pointer_for__send_ChatMessage,
                              get_Pointer_to__send_ChatMessage());
        }
    }


    __SEH_LOGFATAL("CDll::load_dll :\n");

}

void CDll::unload_dll(void)
{
    __SEH_HEADER
    if (hMod_dll==NULL)
        return;
	symbols.prw1326.useme=0;
    (process_message) ("event", "unload");

    if (FreeLibrary(hMod_dll))
        hMod_dll = NULL;


    __SEH_LOGFATAL("CDll::unload_dll :\n");

}

double GetSymbolFromDll(int chair, const char* name, bool& iserr)
{
    __SEH_HEADER
    int			e, stopchar;
    double		res;
    CString		str;
    boost::spirit::tree_parse_info<const char *, int_factory_t>	tpi;
    bool		result;

    str.Format("%s", name);

    result = parse(&str, &tpi, &stopchar);
    if (result)
    {
        e = SUCCESS;
		int offset;
        res = evaluate(&global.formula, tpi, NULL, &e);
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


    __SEH_LOGFATAL("::GetSymbolFromDll :\n");

}
