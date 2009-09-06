#ifndef INC_CSHAREDMEM_H
#define INC_CSHAREDMEM_H

// Shared memory to coordinate the auto-connectors
// of multiple instances of OpenHoldem.
//
// For an introduction to shared memory see
// !!!

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
private:
	#define ENT CSLock lock(m_critsec);
private:
	CCritSec	m_critsec;
} *p_sharedmem;

#endif INC_CSHAREDMEM_H