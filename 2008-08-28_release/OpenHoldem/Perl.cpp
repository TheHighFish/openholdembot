//////////////////////////////////////////////////////////////////////////
//
//  Author: TheHighFish
//
//  Created: 2007.12.11
//
//  Last change: 2008.07.06
//
//  Description: An interface to the Perl programing
//    language for OpenHoldem
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <io.h>
#include <process.h>
#include <sstream>
#include <String>
#include <windows.h>
#include "dll_extension.h"
#include "CScraper.h"
#include "Perl.hpp"

#include "CIteratorThread.h"
#include "CGlobal.h"


using namespace std;

//  A single interpreter object for OH
//
Perl	*the_Perl_Interpreter = NULL;

//  Function types in PerlEz.DLL (implemented in PerlEz.h)
//
extern "C"
{
    typedef int (APIENTRY *T_PerlEzCall)(PERLEZHANDLE hHandle, LPCSTR lpFunction, LPSTR lpBuffer, DWORD dwBufSize, LPCSTR lpFormat, ...);
    typedef PERLEZHANDLE (APIENTRY *APIENTRY T_PerlEzCreate)(LPCSTR lpFileName, LPCSTR lpOptions);
    typedef BOOL (APIENTRY *T_PerlEzDelete)(PERLEZHANDLE);
}


//  Function pointers to 3 functions of the embeddable Perl interpreter
//
T_PerlEzCall P_PerlEzCall;
T_PerlEzCreate P_PerlEzCreate;
T_PerlEzDelete P_PerlEzDelete;


//  File accessable?
//  (<unistd.h> is not contained in MSCVPP)
//
#define F_OK 0


//  The Perl interpreter doesn't accept a NULL pointer
//    as an empty string when expecting command line options.
//
#define EMPTY_STRING ""


string toString(double d)
{
    __SEH_HEADER

    ostringstream the_Result;
    the_Result << d;
    return the_Result.str();

    __SEH_LOGFATAL("Perl::toString : \n");
}


//  gws (Get_WinHoldem_Symbol)
//
//    This function must be static, as we pass a pointer to it to Perl
//    and then to the callback DLL.
//    The result will be passed via a pointer as we had problems the normal way.
//
static void gws(const char *the_Symbol, double* the_Result)
{
    __SEH_HEADER

    bool is_Error;
    double my_Chair = GetSymbolFromDll(0, "userchair", is_Error);
    *the_Result = GetSymbolFromDll(int(my_Chair), the_Symbol, is_Error);

    __SEH_LOGFATAL("Perl::gws : \n");
}


//  gwt (Get_WinHoldem_TableName)
//
//    This function must be static, as we pass a pointer to it to Perl
//    and then to the callback DLL.
//    
static void gwt(char* the_ResultString)
{
    __SEH_HEADER

	//    We assume a buffer of 80 chars on Perl side.
	sprintf_s(the_ResultString, 80, "%s", p_scraper->title());

    __SEH_LOGFATAL("Perl::gwt : \n");
}


//  gwp (Get_WinHoldem_PlayerName)
//
//    This function must be static, as we pass a pointer to it to Perl
//    and then to the callback DLL.
//    
static void gwp(int the_PlayerName, char* the_ResultString)
{
    __SEH_HEADER

	//    We assume a buffer of 80 chars on Perl side.
	sprintf_s(the_ResultString, 80, "%s", p_scraper->player_name(the_PlayerName % 10));

    __SEH_LOGFATAL("Perl::gwp : \n");
}


//  Loading the DLL at runtime (instead of loadtime of OH)
//    to avoid problems for people without a need for Perl.
//  Returns false, if DLL couldn't get loaded or
//    DLL interface is invalid.
//
bool Perl::load_DLL(void)
{
    __SEH_HEADER

    //  Load DLL
    HINSTANCE Perl_Lib = LoadLibrary("PerlEz.dll");
    if (Perl_Lib == NULL)
    {
        MessageBox(NULL, "PerlEz.DLL not found. \nPlease check your installation and your systems PATH variable",
                   "Perl Error", MB_OK | MB_TOPMOST);
        return false;
    }
    //  Load the 3 functions we need.
    P_PerlEzCall = (T_PerlEzCall) GetProcAddress(Perl_Lib, "PerlEzCall");
    P_PerlEzCreate = (T_PerlEzCreate) GetProcAddress(Perl_Lib, "PerlEzCreate");
    P_PerlEzDelete = (T_PerlEzDelete) GetProcAddress(Perl_Lib, "PerlEzDelete");
    //  Functions loaded correctly?
    if ((P_PerlEzCall == NULL) || (P_PerlEzCreate == NULL) ||
            (P_PerlEzDelete == NULL))
    {
        MessageBox(NULL, "PerlEz.DLL seems to be invalid. \nPlease check your installation.",
                   "Perl Error", MB_OK | MB_TOPMOST);
        return false;
    }
    return true;

    __SEH_LOGFATAL("Perl::load_DLL : \n");
}


void Perl::send_Callback_Pointers()
{
    __SEH_HEADER

    if (the_Interpreter == NULL)
    {
        return;
    }

    //  Send the pointer to the gws function to Perl,
    //    typecasted to an integer, as Perl doesn't support
    //    C function pointers.
    int the_Pointer_to_gws = (int)&gws;
    int the_ErrorCode = (*P_PerlEzCall)(
                            the_Interpreter,
                            "set_CallbackPointer_to_gws",   //  Function on Perl side
                            the_ResultBuffer,
                            the_Size_of_ResultBuffer,
                            "i",                            //  Format string: integer
                            the_Pointer_to_gws);            //  Callback pointer as integer
	do_ErrorCheck(the_ErrorCode);

	//  Send the pointer to the gwt function to Perl,
    //    typecasted to an integer, as Perl doesn't support
    //    C function pointers.
    int the_Pointer_to_gwt = (int)&gwt;
    the_ErrorCode = (*P_PerlEzCall)(
						the_Interpreter,
                        "set_CallbackPointer_to_gwt",   //  Function on Perl side
                        the_ResultBuffer,
                        the_Size_of_ResultBuffer,
                        "i",                            //  Format string: integer
                        the_Pointer_to_gwt);            //  Callback pointer as integer
    do_ErrorCheck(the_ErrorCode);

	//  Send the pointer to the gwp function to Perl,
    //    typecasted to an integer, as Perl doesn't support
    //    C function pointers.
    int the_Pointer_to_gwp = (int)&gwp;
    the_ErrorCode = (*P_PerlEzCall)(
                        the_Interpreter,
                        "set_CallbackPointer_to_gwp",   //  Function on Perl side
                        the_ResultBuffer,
                        the_Size_of_ResultBuffer,
                        "i",                            //  Format string: integer
                        the_Pointer_to_gwp);            //  Callback pointer as integer
    do_ErrorCheck(the_ErrorCode);    

    __SEH_LOGFATAL("Perl::send_Callback_Pointers : \n");
}


Perl::Perl()
{
    __SEH_SET_EXCEPTION_HANDLER

    __SEH_HEADER

    Formula_loaded = false;
    Interpreter_not_loaded = true;
    if (!p_global->preferences.Perl_load_Interpreter)
    {
        //  For the people who don't need Perl...
        return;
    }
    if (!load_DLL())
    {
        //  DLL not found or invalid
        return;
    }
    if (p_global->preferences.Perl_load_default_Formula)
    {
        //  Load file and create new instance of the interpreter.
        load_FormulaFile(string(p_global->preferences.Perl_default_Formula));
        //  Interpreter_not_loaded and Formula_loaded set automatically.
    }
    else
    {
        //  No script to load
        the_Interpreter = (*P_PerlEzCreate)
                          (EMPTY_STRING,                  //  No script to load
                           EMPTY_STRING);                 //  Command line options
        Formula_loaded = false;
        Interpreter_not_loaded = false;
    }
    if ((the_Interpreter == NULL) || Interpreter_not_loaded)
    {
        Formula_loaded = false;
        Interpreter_not_loaded = true;
        MessageBox(NULL, "Could not create Perl interpreter.",
                   "Perl Error", MB_OK | MB_TOPMOST);
    }
    send_Callback_Pointers();

    __SEH_LOGFATAL("Perl::Constructor : \n");

}

Perl::~Perl()
{
    __SEH_HEADER

    //  Unload file and destroy interpreter.
    unload_FormulaFile();

    __SEH_LOGFATAL("Perl::Destructor : \n");
}

bool Perl::is_Perl_Symbol(const char *the_Symbol)
{
    __SEH_HEADER

    //  All Perl symbols for OpenHoldem have
    //    to begin with "pl_" (not "pl$", as
    //    perl does not allow this for identifiers
    //    and we don't want to create a new string
    //    for each symbol lookup).
    //
    if (strlen(the_Symbol) <= 3)
    {
        return false;
    }
    return ((the_Symbol[0] == 'p') && (the_Symbol[1] == 'l') &&
            (the_Symbol[2] == '_'));

    __SEH_LOGFATAL("Perl::is_Perl_Symbol : \n");
}


//  double get_Perl_Symbol(char *the_Symbol);
//
//  Requesting a "symbol" from Perl, similar to
//    "gws" for OpenHoldems DLL-extensions.
//
//  It's assumed, the requested "symbol" is a function (to be evaluated)
//    without parameters (e.g. "pl$call" or "pl$ev_raise").
//    This isn't really a limitation, as WinHoldems
//    scripting language doesn't support parameters for functions.
//
//  If we need a constant symbol (I think, we don't),
//    we have to create a constant function.
//
double Perl::get_Perl_Symbol(const char *the_Symbol)
{
    __SEH_HEADER

    if (!is_Perl_Symbol(the_Symbol))
    {
        //  This should not happen!
        //    Getting symbol from OpenHoldem ...
        double the_Result;
        gws (the_Symbol, &the_Result);
        return the_Result;
    }
    //  If no interpreter is loaded, return 0.0
    //    for a robust behaviour.
    if (Interpreter_not_loaded)
    {
        return 0.0;
    }
    int the_ErrorCode = (*P_PerlEzCall)(
                            the_Interpreter,
                            the_Symbol,
                            the_ResultBuffer,
                            the_Size_of_ResultBuffer,
                            EMPTY_STRING 				//  Format string for additional parameters
                        );		                    //  No additional parameters
    //MessageBox(NULL, (string(the_Symbol) + string(": ") + string(the_ResultBuffer)).c_str(),
    //	"Result" , 0);
    do_ErrorCheck(the_ErrorCode);
    //  No further error check here.
    //    We assume, the result is of type double.
    //    If not, 0.0 is returned automatically.
    //
    return atof(the_ResultBuffer);

    __SEH_LOGFATAL("Perl::get_Perl_Symbol : \n");
}

//  Error checking,
//    Error codes according to the documentation of ActivePerl / PerlEz.
//
void Perl::do_ErrorCheck(int the_ErrorCode)
{
    __SEH_HEADER

    if (the_ErrorCode <= 0) 
        return;

    if (the_ErrorCode > 9)  
        return;

    string the_ErrorMessage;
    switch (the_ErrorCode)
    {
    case 1:
        the_ErrorMessage = "Perl: More space is needed to return a result.";
        break;
    case 2: //  Error string returned in the buffer
        the_ErrorMessage = "Perl: " + string(the_ResultBuffer);
        break;
    case 3:
        the_ErrorMessage = "Perl: More space needed to return the error message.";
        break;
    case 4:
        the_ErrorMessage = "Perl: Format string is invalid.";
        break;
    case 5:
        the_ErrorMessage = "Perl: Function call caused an exception.";
        break;
    case 6:
        the_ErrorMessage = "Perl: Handle is invalid.";
        break;
    case 7:
        the_ErrorMessage = "Perl: The second call to PerlEzSetMagicFunction failed.";
        break;
    case 8:
        the_ErrorMessage = "Perl: Invalid parameter was passed to the routine.";
        break;
    case 9:
        the_ErrorMessage = "Perl: Cannot allocate more memory.";
        break;
    }
    MessageBox(NULL, the_ErrorMessage.c_str(), "Perl Error", MB_OK | MB_TOPMOST);

    __SEH_LOGFATAL("Perl::do_ErrorCheck : \n");
}


void Perl::unload_FormulaFile()
{
    __SEH_HEADER

    if (Interpreter_not_loaded)
        return;

    (*P_PerlEzDelete)(the_Interpreter);
    the_Interpreter = NULL;
    Interpreter_not_loaded = true;
    Formula_loaded = false;

    __SEH_LOGFATAL("Perl::unload_FormulaFile : \n");
}


void Perl::load_FormulaFile(string the_new_FormulaFile)
{
    __SEH_HEADER
		
	//  Just a security measure
    if (P_PerlEzCreate == NULL)
		return;

    if (_access(the_new_FormulaFile.c_str(), F_OK) != 0)
    {
        //  Script to load not accessible
        MessageBox(NULL, "Could not load Perl Formula.\nFile not found or not accessible.",
                   "Perl Error", MB_OK | MB_TOPMOST);
        //  Prepare continuation without default file
        the_Interpreter = (*P_PerlEzCreate)
                          (EMPTY_STRING,                 //  Script to load (not accessable)
                           EMPTY_STRING);                //  Command line options
        Formula_loaded = false;
    }
    else
    {
        //  Script to load accessible
        unload_FormulaFile();
        the_Interpreter = (*P_PerlEzCreate)
                          (the_new_FormulaFile.c_str(),  //  Script to load
                           EMPTY_STRING);                 //  Command line options
        the_actual_FormulaFile = string(the_new_FormulaFile);
        Formula_loaded = true;
    }

    send_Callback_Pointers();

    if (the_Interpreter != NULL)
    {
        Interpreter_not_loaded = false;
    }
    else
    {
        MessageBox(NULL, "Unable to load formula.\nCould not create Perl interpreter object.\n\nProbable reasons:\n* Syntax error in your Perl file.\n* Failure at initialization.",
                   "Perl Error", MB_OK | MB_TOPMOST);
        Interpreter_not_loaded = true;
        Formula_loaded = false;
    }

    __SEH_LOGFATAL("Perl::load_FormulaFile : \n");
}


void Perl::reload_FormulaFile()
{
    __SEH_HEADER

    unload_FormulaFile();
    load_FormulaFile(the_actual_FormulaFile);

    __SEH_LOGFATAL("Perl::reload_FormulaFile : \n");
}


void Perl::edit_main_FormulaFile()
{
    __SEH_HEADER

    CString my_favourite_Editor = p_global->preferences.Perl_Editor;
    if (_access(my_favourite_Editor, F_OK) != 0)
    {
        MessageBox(NULL, "Could not start editor.\nExecutable not found or not accessible.",
                   "Perl Error", MB_OK | MB_TOPMOST);
        return;
    }
    if (the_actual_FormulaFile == "")
    {
        MessageBox(NULL, "Formula undefined.", "Perl Error", MB_OK | MB_TOPMOST);
        return;
    }
    //  Enclose path in quotation marks, as some editors
    //    (e.g emacs from the Unix world) have problems
    //    with spaces in (Windows-) path names.
    //
    string the_File_to_edit = '"' + the_actual_FormulaFile + '"';
    int the_Process_ID = _spawnl(
                             P_NOWAIT,                       //  asynchronous execution
                             my_favourite_Editor,            //  path to executable (editor)
                             my_favourite_Editor,            //  arg0: name of the program
                             the_File_to_edit.c_str(),       //  arg1: path to source file
                             NULL);                          //  end of parameter list
    if (errno == -1)
    {
        MessageBox(NULL, "Editor terminated with runtime error.",
                   "Perl Error", MB_OK | MB_TOPMOST);
    }

    __SEH_LOGFATAL("Perl::edit_main_FormulaFile : \n");
}

bool Perl::is_a_Formula_loaded()
{
    __SEH_HEADER

    return (!Interpreter_not_loaded && Formula_loaded);

    __SEH_LOGFATAL("Perl::is_a_Formula_loaded : \n");
}

void Perl::check_Syntax()
{
    __SEH_HEADER

    //  Check the syntax of "the actual Formula".
    //    We can't check "is_a_Formula_loaded()",
    //    as this implies, the formula got loaded with success.
    if (the_actual_FormulaFile == "")
    {
        MessageBox(NULL, "Formula undefined.", "Perl Error", MB_OK | MB_TOPMOST);
        return;
    }
    if (_access(the_actual_FormulaFile.c_str(), F_OK) != 0)
    {
        MessageBox(NULL, "Could not load Perl Formula.\nFile not found or not accessible.",
                   "Perl Error", MB_OK | MB_TOPMOST);
        return;
    }

    //  Enclose path in quotation marks, as some editors
    //    (e.g emacs from the Unix world) have problems
    //    with spaces in (Windows-) path names.
    string the_File_to_compile = '"' + the_actual_FormulaFile + '"';
    //  Build command for the DOS console to execute Perl
    string the_Console_Command =
        string("/K ") +                     //  Take rest of "commandline" as command to execute,
        //    when creating a new DOS shell.
        string("perl ") +					//  Perl :)
        string("-w ") +						//  Enable warnings
        string("-c ") +                     //  Compile only, no execution of the script
        the_File_to_compile;
    ShellExecute(
        NULL,								//  Pointer to parent window; not needed
        "open",								//  "open" == "execute" for an executable
        "cmd.exe",							//  DOS window
        the_Console_Command.c_str(),		//  Command to be executed by the console
        "",									//  Default directory
        SW_SHOWNORMAL);						//  Active window, default size
    //  Ignore return codes (errno) here.
    //    User can see compilation status in the DOS window.

    __SEH_LOGFATAL("Perl::is_a_Formula_loaded:\n");

}
