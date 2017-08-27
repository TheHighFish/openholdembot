//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_MEMORYLOGGING_H
#define INC_MEMORYLOGGING_H

#include "debug.h"
#include "CPreferences.h"

// Logs memory-usage if enabled at debugging-preferences
// Message should not contain a new-line (otherwise log-file messed up)
void LogMemoryUsage(char *message);

// Macro to log size of allocated objects
// especially menat for the parse-tree which uses way too much space.
#define LOG_SIZE_OF_OBJECT(object, message) \
  write_log(preferences.debug_memory_usage(), "[MemoryLogging] %s: %i\n", \
    message, sizeof(object));

#endif INC_MEMORYLOGGING_H