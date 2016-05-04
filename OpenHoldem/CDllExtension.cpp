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

#include "stdafx.h"
#include "CDllExtension.h"

#include "CFilenames.h"
#include "CEngineContainer.h"
#include "CFormulaParser.h"
#include "CFunctionCollection.h"
#include "CHandresetDetector.h"
#include "CIteratorThread.h"
#include "CPreferences.h"
#include "CSymbolEnginePrWin.h"
#include "CSymbolEngineVersus.h"
#include "debug.h"
#include "MagicNumbers.h"
#include "OH_MessageBox.h"
#include "OpenHoldem.h"
#include "PokerChat.hpp"

CDllExtension		*p_dll_extension = NULL;

// Variables exported by OpenHoldem
// avoiding the message-mess of WinHoldem,
// no longer sending any state-messages
// http://www.maxinmontreal.com/forums/viewtopic.php?f=174&t=18642
holdem_state  state[kNumberOfHoldemStatesForDLL];
int state_index;

CDllExtension::CDllExtension() {
	_hmod_dll = NULL;
}

CDllExtension::~CDllExtension() {
}

void CDllExtension::Load(const char * path) {
  CString	dll_path;
	if (IsLoaded()) {
     Unload(); 
  }
	// try to load specific path if passed in as a parameter
	if (strlen(path) > 0) {
		dll_path = path;
		 write_log(preferences.debug_dll_extension(),
			"[CDLLExtension] setting path (1) to %s\n", dll_path);
	}	else {
		// Nothing passed in, so we try the DLL of the formula
		// and the DLL from preferences.
		dll_path = p_function_collection->DLLPath();
		 write_log(preferences.debug_dll_extension(),
			"[CDLLExtension] setting path (2) to %s\n", dll_path);
		if (dll_path == "") {
			dll_path = preferences.dll_name().GetString();
			 write_log(preferences.debug_dll_extension(),
				"[CDLLExtension] setting path (3) to %s\n", dll_path);
		}
	}
  p_filenames->SwitchToOpenHoldemDirectory();
	if (dll_path == "")	{
		// Nothing to do
		return;
	}
	_hmod_dll = LoadLibrary(dll_path);
	DWORD dll_error = GetLastError();
  // If the DLL didn't get loaded
	if (_hmod_dll == NULL) {
		CString error_message;
		error_message.Format("Unable to load DLL from:\n"
			"%s\n"
			"error-code: %d\n",
			 dll_path.GetString(), dll_error);
		OH_MessageBox_Error_Warning(error_message, "DLL Load Error");
		return;
	}
	// No longer passing any pointers to the DLL.
	// We do now export functions and link them implicitly:
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=112&t=15470
  DLLOnLoad();
}

void CDllExtension::Unload(void) {
	if (_hmod_dll==NULL) return;
  DLLOnUnLoad();
  assert(p_iterator_thread != NULL);
	p_iterator_thread->set_prw1326_useme(0);
	if (FreeLibrary(_hmod_dll)) {
		_hmod_dll = NULL;
	}
}

bool CDllExtension::IsLoaded() {
	return _hmod_dll != NULL;
}

/*EXE_IMPLEMENTS*/ double __stdcall GetSymbol(const char* name_of_single_symbol__not_expression) {
	CString	str = "";
	str.Format("%s", name_of_single_symbol__not_expression);
	if (strcmp (str, "cmd$recalc") == 0) {
		// restart iterator thread
    p_iterator_thread->RestartPrWinComputations();
		// Recompute versus tables
		p_symbol_engine_versus->GetCounts();
    // Busy waiting until recalculation got finished.
    // Nothing better to do, as we already evaluate bot-logic,
    // so we can't continue with another heartbeat or something else.
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=19012
    while (!p_iterator_thread->IteratorThreadComplete()) {
      Sleep(100);
    }
		return 0;
	}
  double result = kUndefined;
	p_engine_container->EvaluateSymbol(name_of_single_symbol__not_expression, 
    &result, 
    preferences.dll_logging_enabled());
	return result;
}

void __stdcall SendChatMessage(const char *msg) {
	SendChatMessage((char *)msg);
}

EXE_IMPLEMENTS void* __stdcall GetPrw1326() {
  assert(p_iterator_thread != NULL);
	return (void *)(p_iterator_thread->prw1326());
}

char* __stdcall GetHandnumber() {
	assert(p_handreset_detector->GetHandNumber().GetLength() < k_max_length_of_handnumber);
	static char handnumber_as_char_array[k_max_length_of_handnumber];
	strcpy_s(handnumber_as_char_array, 
		k_max_length_of_handnumber, 
		p_handreset_detector->GetHandNumber());
	return handnumber_as_char_array;
}

void __stdcall WriteLog(char* fmt, ...) {
	// Docu about ellipsis and variadic macro:
	// http://msdn.microsoft.com/en-us/library/ms177415(v=vs.80).aspx
	// http://stackoverflow.com/questions/1327854/how-to-convert-a-variable-argument-function-into-a-macro
	va_list args;
  va_start(args, fmt);
	write_log_vl(preferences.dll_logging_enabled(), fmt, args);
	va_end(args);
}

/*EXE_IMPLEMENTS*/ void __stdcall ParseHandList(const char* name_of_list, const char* list_body) {
  CString function_header;
  function_header.Format("%s%s%s", "##", name_of_list, "##");
  CString handlist;
  handlist.Format("%s\n%s", function_header, list_body);
  // "Formula" includes lists, too.
  p_formula_parser->ParseSingleFormula(handlist, kUndefinedZero);
}