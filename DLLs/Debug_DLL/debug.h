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
// Purpose: Debug-DLL for OpenHoldem
//
//******************************************************************************

#ifdef DEBUG_DLL_EXPORTS
#define DEBUG_DLL_API __declspec(dllexport)
#else
#define DEBUG_DLL_API __declspec(dllimport)
#endif

#include <stdarg.h>

DEBUG_DLL_API void start_log(int current_session_iD, bool delete_old_log);
DEBUG_DLL_API void stop_log();
DEBUG_DLL_API void clear_log();
DEBUG_DLL_API void write_log_vl(bool debug_settings_for_this_message, const char* fmt, va_list vl);
DEBUG_DLL_API void write_log(bool debug_settings_for_this_message, const char* fmt, ...);
DEBUG_DLL_API void write_log_nostamp(bool debug_settings_for_this_message, const char* fmt, ...);
DEBUG_DLL_API void write_log_separator(bool debug_settings_for_this_message, const char* header_message);