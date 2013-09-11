//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose:
//
//*****************************************************************************

#ifndef C_SINGLETONS_H
#define C_SINGLETONS_H

// Creation and deletion of singletons (global objects)
// in well-defined order.
// Order is very important, as these objects might depend on each other;
// an undefined order will lead to very hard to trace crashes
// during startup and to 0-byte crash-dumps.

void StopThreads();
void InstantiateAllSingletons();
void InstantiateSomeSingletonsForVeryEarlyUseInInitInstance();
void DeleteAllSingletons();

#endif // C_SINGLETONS_H