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

#ifndef INC_MEMORYLOGGING_H
#define INC_MEMORYLOGGING_H

// Logs memory-usage if enabled at debugging-preferences
// Message should not contain a new-line (otherwise log-file messed up)
void LogMemoryUsage(char *message);

#endif INC_MEMORYLOGGING_H