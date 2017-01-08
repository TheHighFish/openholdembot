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
#include "CPreferences.h"
#include "CSessionCounter.h"
#include "CSharedMem.h"

// CWatchdog uses the same shared memory segment like CSharedMem
#pragma data_seg(kOpenHoldemSharedmemorySegment) // names are limited to 8 chars, including the dot.
__declspec(allocate(kOpenHoldemSharedmemorySegment)) static	time_t timestamps_openholdem_alive[MAX_SESSION_IDS] = { NULL };
#pragma data_seg()
#pragma comment(linker, "/SECTION:.ohshmem,RWS")		// RWS: read, write, shared

const int kSecondsToconsiderAProcessAsFrozen = 15;

CWatchdog::CWatchdog() {
  MarkThisInstanceAsAlive();
}

CWatchdog::~CWatchdog() {
}

void CWatchdog::HandleCrashedAndFrozenProcesses() {
  MarkThisInstanceAsAlive();
  WatchForCrashedProcesses();
  WatchForFrozenProcesses();
}

void CWatchdog::MarkInstanceAsAlive(int session_ID) {
  assert(session_ID >=  0);
  assert(session_ID < MAX_SESSION_IDS);
  time_t current_time;
  time(&current_time);
  timestamps_openholdem_alive[session_ID] = current_time;
}

void CWatchdog::MarkThisInstanceAsAlive() {
  assert(p_sessioncounter != NULL);
  MarkInstanceAsAlive(p_sessioncounter->session_id());
}

void CWatchdog::WatchForCrashedProcesses() {
  for (int i = 0; i < MAX_SESSION_IDS; ++i) {
    if (p_sharedmem->IsDeadOpenHoldemProcess(i)) {
      p_sharedmem->CleanUpProcessMemory(i);
    }
  }
}

BOOL KillProcess(DWORD dwProcessId) {
  // http://stackoverflow.com/questions/2443738/c-terminateprocess-function !!!
  DWORD dwDesiredAccess = PROCESS_TERMINATE;
  BOOL  bInheritHandle = FALSE;
  INT   uExitCode = -1;
  HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
  if (hProcess == NULL) {
    return FALSE;
  }
  BOOL result = TerminateProcess(hProcess, uExitCode);
  CloseHandle(hProcess);
  return result;
}

void CWatchdog::WatchForFrozenProcesses() {
  time_t current_time;
  time(&current_time);
  for (int i = 0; i < MAX_SESSION_IDS; ++i) {
    if (p_sharedmem->OpenHoldemProcessID(i) == 0) {
      // Not a process
      continue;
    }
    time_t last_process_timestamp = timestamps_openholdem_alive[i];
    int seconds_elapsed = current_time - last_process_timestamp;
    assert(seconds_elapsed >= 0);
    if (seconds_elapsed > kSecondsToconsiderAProcessAsFrozen) {
      if (seconds_elapsed > 2 * kSecondsToconsiderAProcessAsFrozen) {
        // Differences between time-stamp and currernt time way too large,
        // we should already have killed it.
        // Probably a new process which does not yet proper heartbeating,
        // fix its time-stamp and grant it some time to continue.
        MarkInstanceAsAlive(i);
        continue;
      }
      KillProcess(p_sharedmem->OpenHoldemProcessID(i));
    }
  }
}

