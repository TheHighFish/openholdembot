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
#include "MemoryLogging.h"

#include "CPreferences.h"
#include "debug.h"
#include <psapi.h>
#include "Windows.h"

void LogMemoryUsage(char *message) {
  if (!MAIN->p_preferences()->debug_memory_usage()) return;
  write_log(true, "[MemoryLogging] %s\n", message);
  // http://msdn.microsoft.com/de-de/library/windows/desktop/ms683180%28v=vs.85%29.aspx
  DWORD process_ID = GetCurrentProcessId();
  // http://msdn.microsoft.com/de-de/library/windows/desktop/ms682050%28v=vs.85%29.aspx 
  // http://msdn.microsoft.com/en-us/library/windows/desktop/ms684877%28v=vs.85%29.aspx
  PROCESS_MEMORY_COUNTERS pmc;
  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                FALSE, process_ID);
  assert(NULL != hProcess);
  if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
    write_log(true, "[MemoryLogging] WorkingSetSize:     %09d\n", 
      pmc.WorkingSetSize);
    write_log(true, "[MemoryLogging] PeakWorkingSetSize: %09d\n", 
      pmc.PeakWorkingSetSize);
  }
  CloseHandle(hProcess);
}