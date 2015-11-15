//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Very simple user-DLL as a starting-point
//
// DO NOT CHANGE ANYTHING IN THIS FILE!
//   
// This Header defines an interface
// Functions and data-types must exactly match.
//
//******************************************************************************

#ifndef _INC_USER_H
#define _INC_USER_H

// Import and export directives
// for use by this DLL and by OpenHoldem
#ifdef USER_DLL
#define DLL_IMPLEMENTS extern "C" __declspec(dllexport)
#define EXE_IMPLEMENTS extern "C" __declspec(dllimport)
#else
#define DLL_IMPLEMENTS extern "C" __declspec(dllimport)
#define EXE_IMPLEMENTS extern "C" __declspec(dllexport)
#endif

// Functions implemented and exported by the DLL,
// imported by OpenHoldem
DLL_IMPLEMENTS double __stdcall ProcessQuery(const char* pquery);
DLL_IMPLEMENTS void __stdcall DLLOnLoad();
DLL_IMPLEMENTS void __stdcall DLLOnUnLoad();

// Functions implemented and exported by OpenHoldem,
// imported by the DLL
EXE_IMPLEMENTS double __stdcall GetSymbol(const char* name_of_single_symbol__not_expression);
EXE_IMPLEMENTS void   __stdcall SendChatMessage(const char *message);
EXE_IMPLEMENTS void*  __stdcall GetPrw1326();
EXE_IMPLEMENTS void   __stdcall WriteLog(char* format, ...);
EXE_IMPLEMENTS char*  __stdcall GetHandnumber();
// !!!!! To do> table title and player names

#endif // _INC_USER_H
