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

#include "stdafx.h"
#include "MemoryLogging.h"

#include "CPreferences.h"
#include "debug.h"
#include <psapi.h>
#include "Windows.h"

int last_working_set_size = 0;

void LogMemoryUsage(char *message) {
  if (!preferences.debug_memory_usage()) return;
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
    int peal_working_set_size = pmc.PeakWorkingSetSize;
    int working_set_size = pmc.WorkingSetSize;
    int delta_working_set_size = working_set_size - last_working_set_size;
    last_working_set_size = working_set_size;
    write_log(true, "[MemoryLogging] WorkingSetSize: %9d    [%9d]\n", 
      working_set_size, delta_working_set_size);
  }
  CloseHandle(hProcess);
}