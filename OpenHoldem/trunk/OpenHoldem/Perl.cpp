//////////////////////////////////////////////////////////////////////////
//
//  Author: TheHighFish
//
//  Created: 2007.12.11
//
//  Last change: 2008.01.29
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
#include <PerlEz.h>
#include "dll_extension.h"
#include "global.h"
#include "Perl.hpp"

using namespace std;


//  File accessable?
//  (<unistd.h> is not contained in MSCVPP)
//
#define F_OK 0


string toString(double d)
{
#ifdef SEH_ENABLE
	try {
#endif  
    ostringstream the_Result;
    the_Result << d;    
    return the_Result.str();                       
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Perl::toString : \n"); 
		throw;
	}
#endif
}


//  gws (Get_WinHoldem_Symbol)
// 
//    This function must be static, as we pass a pointer to it to Perl 
//    and then to the callback DLL.  
//    The result will be passed via a pointer as we had problems the normal way.
//
static void gws(const char *the_Symbol, double* the_Result)
{
#ifdef SEH_ENABLE
	try {
#endif 	 
	bool is_Error;
	double my_Chair = GetSymbolFromDll(0, "userchair", is_Error);
	//MessageBox(NULL, toString(my_Chair).c_str(), "userchair", 0);
	*the_Result = GetSymbolFromDll(int(my_Chair), the_Symbol, is_Error);
	//MessageBox(NULL, toString(*the_Result).c_str(), "gws", 0);
    #ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Perl::gws : \n"); 
		throw;
	}
#endif
}


void Perl::send_Callback_Pointer_to_gws()
{
#ifdef SEH_ENABLE
	try {
#endif  
	//  MessageBox(NULL, "Sending pointer", "Perl", 0);
	int the_Pointer_to_gws = (int)&gws;	
    int the_ErrorCode = PerlEzCall(
		the_Interpreter, 
		"set_CallbackPointer_to_gws",   //  Function on Perl side
		the_ResultBuffer,               
		the_Size_of_ResultBuffer,
		"i",                            //  Format string: integer       
		the_Pointer_to_gws);            //  Callback pointer as integer
	do_ErrorCheck(the_ErrorCode);	
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Perl::send_Callback_Pointer_to_gws : \n"); 
		throw;
	}
#endif
}


Perl::Perl()
{
#ifdef SEH_ENABLE
	try {
#endif		
	if (!global.preferences.Perl_load_Interpreter)
	{
		//  For the people who don't need Perl...
		Interpreter_not_loaded = true;
		return;
	}
	Interpreter_not_loaded = false;
	if (global.preferences.Perl_load_default_Formula)
	{
		//  Load file and create new instance of the interpreter.
		load_FormulaFile(string(global.preferences.Perl_default_Formula));
	}
	else
	{
		//  No script to load
		the_Interpreter = PerlEzCreate
		    (NULL,                          //  No script to load
			NULL);                          //  Command line options
		//  MessageBox(NULL, "No Script to load", "Perl", 0);
	}
	if (the_Interpreter == NULL)
	{
		MessageBox(NULL, "Could not create Perl interpreter.", "Perl Error", MB_OK);
	}
	send_Callback_Pointer_to_gws();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Perl::Perl : \n"); 
		throw;
	}
#endif 	 
}


Perl::~Perl()
{
#ifdef SEH_ENABLE
	try {
#endif 
    //  Unload file and destroy interpreter.
	unload_FormulaFile();
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Perl::Perl : \n"); 
		throw;
	}
#endif 	
}


bool Perl::is_Perl_Symbol(const char *the_Symbol)
{
#ifdef SEH_ENABLE
	try {
#endif 	 
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
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Perl::is_Perl_Symbol : \n"); 
		throw;
	}
#endif
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
#ifdef SEH_ENABLE
	try {	
#endif 	    
	if (!is_Perl_Symbol(the_Symbol))   
	{
		//  Get symbol from OpenHoldem 	
		double the_Result;
		gws (the_Symbol, &the_Result);
		return the_Result;
	}	
	//  If no interpreter is loaded, return 0.0 
    //    for a robust behaviour. 
	if (Interpreter_not_loaded) 
	{ return 0.0; }
	int the_ErrorCode = PerlEzCall(
		the_Interpreter, 
		the_Symbol,
		the_ResultBuffer, 
		the_Size_of_ResultBuffer,
		""							//  Format string for additional parameters
		);		                    //  No additional parameters  
	do_ErrorCheck(the_ErrorCode);
	//  No further error check here.
	//  We assume, the result is of type double.
	//  If not, 0.0 is returned. 
	//
	//  MessageBox(NULL, the_ResultBuffer, the_Symbol, 0);
	return atof(the_ResultBuffer);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Perl::get_Perl_Symbol : \n"); 
		throw;
	}
#endif
}

//  Error checking,
//    Error codes according to the documentation of ActivePerl / PerlEz.
//
void Perl::do_ErrorCheck(int the_ErrorCode)
{
#ifdef SEH_ENABLE
	try {
#endif  
	if (the_ErrorCode <= 0) { return; }
	if (the_ErrorCode > 9)  { return; }
	string the_ErrorMessage;
	switch(the_ErrorCode)
	{
	case 1: the_ErrorMessage = "Perl: More space is needed to return a result.";
		break;
	case 2: //  Error string returned in the buffer 
		the_ErrorMessage = "Perl: " + string(the_ResultBuffer);
		break;
	case 3: the_ErrorMessage = "Perl: More space needed to return the error message.";
		break;
	case 4: the_ErrorMessage = "Perl: Format string is invalid.";
		break;
	case 5: the_ErrorMessage = "Perl: Function call caused an exception.";
		break;
	case 6: the_ErrorMessage = "Perl: Handle is invalid.";
		break;
	case 7: the_ErrorMessage = "Perl: The second call to PerlEzSetMagicFunction failed.";
		break;
	case 8: the_ErrorMessage = "Perl: Invalid parameter was passed to the routine.";
		break;
	case 9: the_ErrorMessage = "Perl: Cannot allocate more memory.";
		break;
	}
	MessageBox(NULL, the_ErrorMessage.c_str(), "Perl Error", 0);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Perl::do_ErrorCheck : \n"); 
		throw;
	}
#endif
}



void Perl::unload_FormulaFile()
{
#ifdef SEH_ENABLE
	try {
#endif  
	if (Interpreter_not_loaded) { return; }
	PerlEzDelete(the_Interpreter);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Perl::unload_FormulaFile : \n"); 
		throw;
	}
#endif
}


void Perl::load_FormulaFile(string the_new_FormulaFile)
{
#ifdef SEH_ENABLE
	try {
#endif  
	if (_access(the_new_FormulaFile.c_str(), F_OK) != 0)
	{
		//  Script to load not accessible
		MessageBox(NULL, "Could not load formula file.", "Perl Error", MB_OK);
		//  Prepare continuation without default file
		the_Interpreter = PerlEzCreate
			(NULL,                      //  Script to load (not accessable)
			 NULL);                     //  Command line options
	}
	else
	{
		//  Script to load accessible
		the_Interpreter = PerlEzCreate
			(the_new_FormulaFile.c_str(),  //  Script to load
		    NULL);                         //  Command line options	
		//  MessageBox(NULL, the_default_Perl_Formula_File, "Perl", 0);
		the_actual_FormulaFile = string(the_new_FormulaFile);
	}	
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Perl::load_FormulaFile : \n"); 
		throw;
	}
#endif
}


void Perl::reload_FormulaFile()
{
#ifdef SEH_ENABLE
	try {
#endif  
	unload_FormulaFile();
	load_FormulaFile(the_actual_FormulaFile);
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Perl::reload_FormulaFile : \n"); 
		throw;
	}
#endif
}


void Perl::edit_main_FormulaFile()
{ 
#ifdef SEH_ENABLE
	try {
#endif  	 
	CString my_favourite_Editor = global.preferences.Perl_Editor;
	if (_access(my_favourite_Editor, F_OK) != 0) 
    {
	   	MessageBox(NULL, my_favourite_Editor, "Perl Error", MB_OK);
	}								    
	else
	{
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
		    MessageBox(NULL, "Executing editor failed.", "Perl Error", MB_OK);    		  
	    }
	}
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("Perl::edit_main_FormulaFile : \n"); 
		throw;
	}
#endif
}