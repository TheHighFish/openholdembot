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


extern class CSharedMem
{
public:
	CSharedMem();
	~CSharedMem();
public:
	// public accessors and mutators
	bool PokerWindowAttached(HWND Window);
	void MarkPokerWindowAsAttached(HWND Window);
	void MarkPokerWindowAsUnAttached();
	void RememberTimeOfLastFailedAttemptToConnect();
	time_t GetTimeOfLastFailedAttemptToConnect();
	// Used for the table positioner
	HWND *GetDenseListOfConnectedPokerWindows();
	int  SizeOfDenseListOfAttachedPokerWindows();
private:
	void CreateDenseListOfConnectedPokerWindows();
	void VerifyMainMutexName();
	#define ENT CSLock lock(m_critsec);
private:
	CCritSec	m_critsec;
} *p_sharedmem;

#endif INC_CSHAREDMEM_H