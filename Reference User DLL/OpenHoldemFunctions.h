#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Functions that get exported by OpenHoldem.
//   They are available to the DLL once the interface has been 
//   properly initialized (a warning will be shown if not)
//
//******************************************************************************
//
// DO NOT CHANGE ANYTHING IN THIS FILE!
//   
// This Header defines an interface
// Functions and data-types must exactly match.
//
//******************************************************************************

#include "user.h"

EXE_IMPLEMENTS double __stdcall GetSymbol(const char* name_of_single_symbol__not_expression);
EXE_IMPLEMENTS void*  __stdcall GetPrw1326();
EXE_IMPLEMENTS char*  __stdcall GetHandnumber();
EXE_IMPLEMENTS void   __stdcall ParseHandList(const char* name_of_list, const char* list_body);
EXE_IMPLEMENTS char*  __stdcall ScrapeTableMapRegion(char* p_region, int& p_returned_lengh);
EXE_IMPLEMENTS void   __stdcall SendChatMessage(const char *message);
EXE_IMPLEMENTS void   __stdcall WriteLog(char* format, ...);

void InitializeOpenHoldemFunctionInterface();