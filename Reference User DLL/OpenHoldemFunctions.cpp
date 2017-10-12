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
// DO NOT DELETE THE  ERROR-HANDLING
// Using non-initialized pointers will crash OpenHoldem.
// If you get an error-message at runtime
// then change the timing of your initializations.
// E.g. use DLLOnLoad to initialize global variables.
//
//******************************************************************************

#include "OpenHoldemFunctions.h"
#include "Windows.h"

//******************************************************************************
//
// Function signatures and pointers
//
//******************************************************************************

typedef double(*t_GetSymbol)(const char* name_of_single_symbol__not_expression);
typedef void*(*t_GetPrw1326)();
typedef char*(*t_GetHandnumber)();
typedef void(*t_ParseHandList)(const char* name_of_list, const char* list_body);
typedef char*(*t_ScrapeTableMapRegion)(char* p_region, int& p_returned_lengh);
typedef void(*t_SendChatMessage)(const char *message);
typedef void(*t_WriteLog)(char* format, ...);

t_GetSymbol p_GetSymbol = nullptr;
t_GetPrw1326 p_GetPrw1326 = nullptr;
t_GetHandnumber p_GetHandnumber = nullptr;
t_ParseHandList p_ParseHandList = nullptr;
t_ScrapeTableMapRegion p_ScrapeTableMapRegion = nullptr;
t_SendChatMessage p_SendChatMessage = nullptr;
t_WriteLog p_WriteLog = nullptr;

//******************************************************************************
//
// OpenHoldem functions, to be used by the DLL
//
//******************************************************************************

double __stdcall GetSymbol(const char* name_of_single_symbol__not_expression) {
  if (p_GetSymbol == nullptr) {
    ErrorPointerNotInitialized();
	  return 0.0;
  }
  return p_GetSymbol(name_of_single_symbol__not_expression);
}

void* __stdcall GetPrw1326() {
  if (p_GetPrw1326 == nullptr) {
    ErrorPointerNotInitialized();
    return nullptr;
  }
  return p_GetPrw1326();
}

char* __stdcall GetHandnumber() {
  if (p_GetHandnumber == nullptr) {
    ErrorPointerNotInitialized();
    return "";
  }
  return p_GetHandnumber();
}

void __stdcall ParseHandList(const char* name_of_list, const char* list_body) {
  if (p_ParseHandList == nullptr) {
    ErrorPointerNotInitialized();
    return;
  }
  p_ParseHandList(name_of_list, list_body);
}

char* __stdcall ScrapeTableMapRegion(char* p_region, int& p_returned_lengh) {
  if (p_ScrapeTableMapRegion == nullptr) {
    ErrorPointerNotInitialized();
    return "";
  }
  return p_ScrapeTableMapRegion(p_region, p_returned_lengh);
}

void __stdcall SendChatMessage(const char *message) {
  if (p_SendChatMessage == nullptr) {
    ErrorPointerNotInitialized();
    return;
  }
  p_SendChatMessage(message);
}

void __stdcall WriteLog(char* format, ...) {
  if (p_WriteLog == nullptr) {
    ErrorPointerNotInitialized();
    return;
  }
  p_WriteLog(format); //!!!!!
}

//******************************************************************************
//
// Initialization of the interface
//
//******************************************************************************

void* LookupOpenHoldemFunction(char* name) {
	return nullptr;
}

void InitializeOpenHoldemFunctionInterface() {
  p_GetSymbol = (t_GetSymbol)LookupOpenHoldemFunction("GetSymbol");
  p_GetPrw1326 = (t_GetPrw1326)LookupOpenHoldemFunction("GetPrw1326");
  p_GetHandnumber = (t_GetHandnumber)LookupOpenHoldemFunction("GetHandnumber");
  p_ParseHandList = (t_ParseHandList)LookupOpenHoldemFunction("ParseHandList");
  p_ScrapeTableMapRegion = (t_ScrapeTableMapRegion)LookupOpenHoldemFunction("ScrapeTableMapRegion");
  p_SendChatMessage = (t_SendChatMessage)LookupOpenHoldemFunction("SendChatMessage");
  p_WriteLog = (t_WriteLog)LookupOpenHoldemFunction("WriteLog");
}

void ErrorPointerNotInitialized() {
  MessageBox(0,
    "OpenHoldem functions not yet initialized.\n"
    "Can't use function XYZ.\n", //!!!!!
    "Error",
    MB_ICONERROR);
}