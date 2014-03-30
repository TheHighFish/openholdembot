//////////////////////////////////////////////////////////////////////////
//
//  Author: TheHighFish
//
//  Created: 2008.01.10
//
//  Last change: 2008.07.06
//
//  Description: A very simple DLL for Openoldem,
//    enabling Perl to call the function get_OpenHoldem_Symbol()
//    via a C function pointer.
//  This DLL shall only be used by Perl using the module Win32::API.
//
//////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <io.h>
#include <stdio.h>
#include <sstream>
#include <String>
#include <windows.h> 

using namespace std;

//  Define this to enable logging.
//
#undef LOG_CALLBACK


#define APPEND_MODE "a"


//  Your log file.
//    You should have write access to the file.
//    Note: Backslash is an escape char.
//    Use two to get a single one.
//
#define my_LogFile "C:\\Documents\\Jeff\\Desktop\\Perl_Callback.log"


string toString(double d)
{
    ostringstream the_Result;
    the_Result << d;    
    return the_Result.str();                       
}


//  Pointer type to get_OpenHoldem_Symbol (alias gws)
//    It shall point to the function defined in OHs "Perl.cpp".
//
typedef void (*T_get_OpenHoldem_Symbol_Pointer) (const char* the_Symbol,
						   						 double* the_Result);

//  
//  Parameters:
//    1. Pointer to OHs function "gws", defined in "Perl.cpp".
//       We pass it as an integer, as Perl doesn't support pointers
//       to C functions. 
//    2. Pointer to string (symbol)
//    3. Pointer to string (result); we use a string, as 
//       Perls Win32::API seems to have problems returning 
//       values > 4 bytes (e.g. doubles); via the reference 
//       we write directly into the buffer on Perl side.
//       If we use the callback funtion wisely (i.e. buffer symbols
//       in case of complex formulas) this type conversion shouldn`t
//       become a bottleneck.
//
extern "C" void _declspec(dllexport) _stdcall get_OpenHoldem_Symbol_indirect(
	int Pointer_to_Function_get_OpenHoldem_Symbol,
	char *the_Symbol,
	char *the_ResultString)
{	
#ifdef LOG_CALLBACK
	FILE *DLL_Log = fopen(my_LogFile, APPEND_MODE);
	//  This is for debugging only; we check the file the rude way.
	assert(DLL_Log != NULL);
	fprintf(DLL_Log, "Pointer: %i Symbol: %s\n", 
		Pointer_to_Function_get_OpenHoldem_Symbol, 
		the_Symbol);
	fclose(DLL_Log);	
#endif
	double the_Result; 
	((T_get_OpenHoldem_Symbol_Pointer)(Pointer_to_Function_get_OpenHoldem_Symbol))(the_Symbol, &the_Result);
	//  Convert result to string
	//    Be cautious with the format string!
	//    "%d" doesn't mean double.
	//    Have a look at the documentation,
	//    e.g. "http://www.cplusplus.com/reference/clibrary/cstdio/sprintf.html"
	//    We assume a buffer of 80 chars on Perl side.
	sprintf_s(the_ResultString, 80, "%f", the_Result);	
#ifdef LOG_CALLBACK
	DLL_Log = fopen(my_LogFile, APPEND_MODE);
	//  This is for debugging only; we check the file the rude way.
	assert(DLL_Log != NULL);
	fprintf(DLL_Log, "Result: %s\n", the_ResultString);
	fclose(DLL_Log);	
#endif	
}


//  Pointer type to get_OpenHoldem_TableName 
//    It shall point to the function defined in OHs "Perl.cpp".
//
typedef void (*T_get_OpenHoldem_TableName_Pointer) (char* the_ResultString);


extern "C" void _declspec(dllexport) _stdcall get_OpenHoldem_TableName_indirect(
	int Pointer_to_Function_get_OpenHoldem_TableName,	
	char *the_ResultString)
{
#ifdef LOG_CALLBACK
	FILE *DLL_Log = fopen(my_LogFile, APPEND_MODE);
	//  This is for debugging only; we check the file the rude way.
	assert(DLL_Log != NULL);
	fprintf(DLL_Log, "Pointer: %i\n", 
		Pointer_to_Function_get_OpenHoldem_TableName );
	fclose(DLL_Log);	
#endif
	((T_get_OpenHoldem_TableName_Pointer)(Pointer_to_Function_get_OpenHoldem_TableName))(the_ResultString);	
#ifdef LOG_CALLBACK
	DLL_Log = fopen(my_LogFile, APPEND_MODE);
	//  This is for debugging only; we check the file the rude way.
	assert(DLL_Log != NULL);
	fprintf(DLL_Log, "Result: %s\n", the_ResultString);
	fclose(DLL_Log);	
#endif	
}

//  Pointer type to get_OpenHoldem_PlayerName 
//    It shall point to the function defined in OHs "Perl.cpp".
//
typedef void (*T_get_OpenHoldem_PlayerName_Pointer) (int the_Player_Number, char* the_ResultString);


extern "C" void _declspec(dllexport) _stdcall get_OpenHoldem_PlayerName_indirect(
	int Pointer_to_Function_get_OpenHoldem_PlayerName,
	int the_Player_Number,
	char *the_ResultString)
{
#ifdef LOG_CALLBACK
	FILE *DLL_Log = fopen(my_LogFile, APPEND_MODE);
	//  This is for debugging only; we check the file the rude way.
	assert(DLL_Log != NULL);
	fprintf(DLL_Log, "Pointer: %i Player Number: %i\n", 
		Pointer_to_Function_get_OpenHoldem_PlayerName, the_Player_Number );
	fclose(DLL_Log);	
#endif
	((T_get_OpenHoldem_PlayerName_Pointer)(Pointer_to_Function_get_OpenHoldem_PlayerName))(the_Player_Number, 
		the_ResultString);	
#ifdef LOG_CALLBACK
	DLL_Log = fopen(my_LogFile, APPEND_MODE);
	//  This is for debugging only; we check the file the rude way.
	assert(DLL_Log != NULL);
	fprintf(DLL_Log, "Result: %s\n", the_ResultString);
	fclose(DLL_Log);	
#endif	
}
