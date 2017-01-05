//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Watching for crashed processes and cleaning up shared memory
//   * so that another instance can connect to this table
//   * because the OpenHoldem starter needs the number of running processes
//
//******************************************************************************

#include "stdafx.h"
#include "CWatchdog.h"
#include "CSharedMem.h"

CWatchdog::CWatchdog() {
}

CWatchdog::~CWatchdog() {
}

void CWatchdog::WatchForDeadProcessed() {
  for (int i = 0; i < MAX_SESSION_IDS; ++i) {
    if (p_sharedmem->IsDeadOpenHoldemProcess(i)) {
      p_sharedmem->CleanUpProcessMemory(i);
    }
  }
}




