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

//////////////////////////////////////////////////////////////////////////
//
//  Author: TheHighFish
//
//  Created: 2007.12.11
//
//  Last change: 2008.07.06
//
//  Description: An interface to the Perl programing
//	language for OpenHoldem
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <io.h>
#include <process.h>
#include <sstream>

#include "CPerl.hpp"

#include "CScraper.h"
#include "CIteratorThread.h"
#include "CPreferences.h"
#include "CDllExtension.h"
#include "OH_MessageBox.h"

using namespace std;

//  A single interpreter object for OH
//
CPerl	*p_perl = NULL;

//  Function types in PerlEz.DLL (implemented in PerlEz.h)
//
extern "C"
{
	typedef int (APIENTRY *T_PerlEzCall)(PERLEZHANDLE hHandle, LPCSTR lpFunction, LPSTR lpBuffer, DWORD dwBufSize, LPCSTR lpFormat, ...);
	typedef PERLEZHANDLE (APIENTRY *T_PerlEzCreate)(LPCSTR lpFileName, LPCSTR lpOptions);
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
//	as an empty string when expecting command line options.
//
#define EMPTY_STRING ""

//  gws (Get_WinHoldem_Symbol)
//
//	This function must be static, as we pass a pointer to it to Perl
//	and then to the callback DLL.
//	The result will be passed via a pointer as we had problems the normal way.
//
static void gws(const char *the_Symbol, double* the_Result)
{
	bool is_Error;
	double my_Chair = GetSymbolFromDll(0, "userchair", is_Error);
	*the_Result = GetSymbolFromDll(int(my_Chair), the_Symbol, is_Error);
}


//  gwt (Get_WinHoldem_TableName)
//
//	This function must be static, as we pass a pointer to it to Perl
//	and then to the callback DLL.
//	
static void gwt(char* the_ResultString)
{
	//	We assume a buffer of 180 chars on Perl side.
	sprintf_s(the_ResultString, 180, "%s", p_scraper->title());
}


//  gwp (Get_WinHoldem_PlayerName)
//
//	This function must be static, as we pass a pointer to it to Perl
//	and then to the callback DLL.
//	
static void gwp(int the_PlayerName, char* the_ResultString)
{
	//	We assume a buffer of 80 chars on Perl side.
	sprintf_s(the_ResultString, 80, "%s", p_scraper->player_name(the_PlayerName % 10));
}


CPerl::CPerl()
{
	_formula_loaded = false;
	_interpreter_not_loaded = true;

	if (preferences.perl_default_formula() == "")
	{
		// For people who don't need perl.
		return;
	}
	if (!LoadDLL())
	{
		//  DLL not found or invalid
		return;
	}
	//  Load file and create new instance of the interpreter.
	LoadFormulaFile(string(preferences.perl_default_formula()));
	//  _interpreter_not_loaded and _formula_loaded set automatically.

	if ((_interpreter == NULL) || _interpreter_not_loaded)
	{
		_formula_loaded = false;
		_interpreter_not_loaded = true;
		OH_MessageBox_Error_Warning("Could not create Perl interpreter.", "Perl Error");
	}
	SendCallbackPointers();
}

CPerl::~CPerl()
{
	//  Unload file and destroy interpreter.
	UnloadFormulaFile();
}

bool CPerl::IsPerlSymbol(const char *symbol)
{
	//  All Perl symbols for OpenHoldem have
	//	to begin with "pl_" (not "pl$", as
	//	perl does not allow this for identifiers
	//	and we don't want to create a new string
	//	for each symbol lookup).
	//
	if (strlen(symbol) <= 3)
		return false;

	return ((symbol[0] == 'p') && (symbol[1] == 'l') &&
			(symbol[2] == '_'));
}

//  double GetPerlSymbol(char *symbol);
//
//  Requesting a "symbol" from Perl, similar to
//	"gws" for OpenHoldems DLL-extensions.
//
//  It's assumed, the requested "symbol" is a function (to be evaluated)
//	without parameters (e.g. "pl$call" or "pl$ev_raise").
//	This isn't really a limitation, as OpenHoldem's
//	scripting language doesn't support parameters for functions.
//
//  If we need a constant symbol (I think, we don't),
//	we have to create a constant function.
//
double CPerl::GetPerlSymbol(const char *symbol)
{
	if (!IsPerlSymbol(symbol))
	{
		//  This should not happen!
		//	Getting symbol from OpenHoldem ...
		double the_Result;
		gws (symbol, &the_Result);
		return the_Result;
	}

	//  If no interpreter is loaded, return 0.0
	//	for a robust behaviour.
	if (_interpreter_not_loaded)
	{
		return 0.0;
	}

	int error_code = (*P_PerlEzCall)(
							_interpreter,
							symbol,
							_result_buffer,
							_size_of_result_buffer,
							EMPTY_STRING 				//  Format string for additional parameters
						);								//  No additional parameters

	DoErrorCheck(error_code);
	//  No further error check here.
	//	We assume, the result is of type double.
	//	If not, 0.0 is returned automatically.
	//

	return atof(_result_buffer);
}

void CPerl::EditMainFormulaFile()
{
	CString my_favourite_Editor = preferences.perl_editor();

	if (_access(my_favourite_Editor, F_OK) != 0)
	{
		OH_MessageBox_Error_Warning("Could not start editor.\nExecutable not found or not accessible.", "Perl Error");
		return;
	}

	if (_actual_formula_file == "")
	{
		OH_MessageBox_Error_Warning("Formula undefined.", "Perl Error");
		return;
	}

	//  Enclose path in quotation marks, as some editors
	//	(e.g emacs from the Unix world) have problems
	//	with spaces in (Windows-) path names.
	//
	string the_File_to_edit = '"' + _actual_formula_file + '"';

	int the_Process_ID = _spawnl(
							 P_NOWAIT,					   //  asynchronous execution
							 my_favourite_Editor,			//  path to executable (editor)
							 my_favourite_Editor,			//  arg0: name of the program
							 the_File_to_edit.c_str(),	   //  arg1: path to source file
							 NULL);						  //  end of parameter list
	if (errno == k_undefined)
	{
		OH_MessageBox_Error_Warning("Editor terminated with runtime error.",
					"Perl Error");
	}
}

void CPerl::LoadFormulaFile(string new_formula_file)
{
	//  Just a security measure
	if (P_PerlEzCreate == NULL)
		return;

	if (_access(new_formula_file.c_str(), F_OK) != 0)
	{
		//  Script to load not accessible
		OH_MessageBox_Error_Warning("Could not load Perl Formula.\nFile not found or not accessible.", "Perl Error");

		//  Prepare continuation without default file
		_interpreter = (*P_PerlEzCreate)
						  (EMPTY_STRING,				 //  Script to load (not accessable)
						   EMPTY_STRING);				//  Command line options
		_formula_loaded = false;
	}
	else
	{
		//  Script to load accessible
		UnloadFormulaFile();
		_interpreter = (*P_PerlEzCreate)
						  (new_formula_file.c_str(),  //  Script to load
						   EMPTY_STRING);				 //  Command line options
		_actual_formula_file = string(new_formula_file);
		_formula_loaded = true;
	}

	SendCallbackPointers();

	if (_interpreter != NULL)
	{
		_interpreter_not_loaded = false;
	}
	else
	{
		OH_MessageBox_Error_Warning("Unable to load formula.\n"
			"Could not create Perl interpreter object.\n\n"
			"Probable reasons:\n"
			"* Syntax error in your Perl file.\n"
			"* Failure at initialization.",
			"Perl Error");
		_interpreter_not_loaded = true;
		_formula_loaded = false;
	}
}

void CPerl::ReloadFormulaFile()
{
	UnloadFormulaFile();
	LoadFormulaFile(_actual_formula_file);
}

void CPerl::UnloadFormulaFile()
{
	if (_interpreter_not_loaded)
		return;

	(*P_PerlEzDelete)(_interpreter);
	_interpreter = NULL;
	_interpreter_not_loaded = true;
	_formula_loaded = false;
}

void CPerl::CheckSyntax()
{
	//  Check the syntax of "the actual Formula".
	//	We can't check "IsAFormulaLoaded()",
	//	as this implies, the formula got loaded with success.
	if (_actual_formula_file == "")
	{
		OH_MessageBox_Error_Warning("Formula undefined.", "Perl Error");
		return;
	}

	if (_access(_actual_formula_file.c_str(), F_OK) != 0)
	{
		OH_MessageBox_Error_Warning("Could not load Perl Formula.\nFile not found or not accessible.", "Perl Error");
		return;
	}

	//  Enclose path in quotation marks, as some editors
	//	(e.g emacs from the Unix world) have problems
	//	with spaces in (Windows-) path names.
	string the_File_to_compile = '"' + _actual_formula_file + '"';
	//  Build command for the DOS console to execute Perl
	string the_Console_Command =
		string("/K ") +					 //  Take rest of "commandline" as command to execute,
		//	when creating a new DOS shell.
		string("perl ") +					//  Perl :)
		string("-w ") +						//  Enable warnings
		string("-c ") +					 //  Compile only, no execution of the script
		the_File_to_compile;
	ShellExecute(
		NULL,								//  Pointer to parent window; not needed
		"open",								//  "open" == "execute" for an executable
		"cmd.exe",							//  DOS window
		the_Console_Command.c_str(),		//  Command to be executed by the console
		"",									//  Default directory
		SW_SHOWNORMAL);						//  Active window, default size
	//  Ignore return codes (errno) here.
	//	User can see compilation status in the DOS window.
}

bool CPerl::IsAFormulaLoaded()
{
	return (!_interpreter_not_loaded && _formula_loaded);
}

//  Loading the DLL at runtime (instead of loadtime of OH)
//	to avoid problems for people without a need for Perl.
//  Returns false, if DLL couldn't get loaded or
//	DLL interface is invalid.
bool CPerl::LoadDLL(void)
{
	//  Load DLL
	HINSTANCE Perl_Lib = LoadLibrary("PerlEz.dll");
	if (Perl_Lib == NULL)
	{
		DWORD ErrorCode = GetLastError();
		CString ErrorMessage;
		ErrorMessage.Format("%s%d%s",
			"PerlEz.DLL not found. \n"
			"Please check your installation of ActiveState-Perl\n"
			"and your systems PATH variable.\n"
			"\n"
			"ErrorCode: [", ErrorCode, "]");
		OH_MessageBox_Error_Warning(ErrorMessage, "Perl Error");
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
		OH_MessageBox_Error_Warning("PerlEz.DLL seems to be invalid. \nPlease check your installation.", "Perl Error");
		return false;
	}
	return true;
}

void CPerl::SendCallbackPointers()
{
	if (_interpreter == NULL)
		return;

	//  Send the pointer to the gws function to Perl,
	//	typecasted to an integer, as Perl doesn't support
	//	C function pointers.
	int pointer_to_gws = (INT_PTR)&gws;
	int error_code = (*P_PerlEzCall)(
							_interpreter,
							"set_CallbackPointer_to_gws",   //  Function on Perl side
							_result_buffer,
							_size_of_result_buffer,
							"i",							//  Format string: integer
							pointer_to_gws);			//  Callback pointer as integer
	DoErrorCheck(error_code);

	//  Send the pointer to the gwt function to Perl,
	//	typecasted to an integer, as Perl doesn't support
	//	C function pointers.
	int pointer_to_gwt = (INT_PTR)&gwt;
	error_code = (*P_PerlEzCall)(
						_interpreter,
						"set_CallbackPointer_to_gwt",   //  Function on Perl side
						_result_buffer,
						_size_of_result_buffer,
						"i",							//  Format string: integer
						pointer_to_gwt);			//  Callback pointer as integer
	DoErrorCheck(error_code);

	//  Send the pointer to the gwp function to Perl,
	//	typecasted to an integer, as Perl doesn't support
	//	C function pointers.
	int pointer_to_gwp = (INT_PTR)&gwp;
	error_code = (*P_PerlEzCall)(
						_interpreter,
						"set_CallbackPointer_to_gwp",   //  Function on Perl side
						_result_buffer,
						_size_of_result_buffer,
						"i",							//  Format string: integer
						pointer_to_gwp);			//  Callback pointer as integer
	DoErrorCheck(error_code);	
}


//  Error checking,
//	Error codes according to the documentation of ActivePerl / PerlEz.
//
void CPerl::DoErrorCheck(const int error_code)
{
	if (error_code <= 0) 
		return;

	if (error_code > 9)  
		return;

	string error_message;
	switch (error_code)
	{
	case 1:
		error_message = "Perl: More space is needed to return a result.";
		break;
	case 2: //  Error string returned in the buffer
		error_message = "Perl: " + string(_result_buffer);
		break;
	case 3:
		error_message = "Perl: More space needed to return the error message.";
		break;
	case 4:
		error_message = "Perl: Format string is invalid.";
		break;
	case 5:
		error_message = "Perl: Function call caused an exception.";
		break;
	case 6:
		error_message = "Perl: Handle is invalid.";
		break;
	case 7:
		error_message = "Perl: The second call to PerlEzSetMagicFunction failed.";
		break;
	case 8:
		error_message = "Perl: Invalid parameter was passed to the routine.";
		break;
	case 9:
		error_message = "Perl: Cannot allocate more memory.";
		break;
	}
	OH_MessageBox_Error_Warning(error_message.c_str(), "Perl Error");

}

