//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#ifndef INC_CSHAREDMEM_H
#define INC_CSHAREDMEM_H

// Shared memory to coordinate the auto-connectors
// of multiple instances of OpenHoldem.
//
// For an introduction to shared memory see
//   * http://www.programmersheaven.com/mb/Win32API/156366/156366/shared-memory-and-dll/
//   * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2044.html
//   * http://www.codeproject.com/KB/threads/SharedMemory_IPC_Threads.aspx
//   * http://www.fstsoft.de/shared.htm (german)
//
// We chose the first solution for its simplicity

class CSharedMem {
 public:
	CSharedMem();
	~CSharedMem();
 public:
	// public accessors and mutators
	bool PokerWindowAttached(HWND Window);
	bool AnyWindowAttached();
	void MarkPokerWindowAsAttached(HWND Window);
	void MarkPokerWindowAsUnAttached();
	void RememberTimeOfLastFailedAttemptToConnect();
	time_t GetTimeOfLastFailedAttemptToConnect();
	// Used for the table positioner
	HWND *GetDenseListOfConnectedPokerWindows();
	int  SizeOfDenseListOfAttachedPokerWindows();
	bool IsAnyOpenHoldemProcess(int PID);
 public:
  // For table positioner
  // It is most easz if everzbodz shares his position
  void StoreTablePosition(int left, int top, int right, int bottom);
  bool OverlapsAnyTable(int left, int top, int right, int bottom);
  // My position
  int TablePositionLeft();
  int TablePositionTop();
 private:
	void CreateDenseListOfConnectedPokerWindows();
	void VerifyMainMutexName();
 private:
	CCritSec	m_critsec;
};

extern CSharedMem *p_sharedmem;

#endif INC_CSHAREDMEM_H