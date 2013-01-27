#ifndef INC_CSESSIONCOUNTER_H
#define INC_CSESSIONCOUNTER_H

#include "MagicNumbers.h"

extern class CSessionCounter
{
public:
	// public functions
	CSessionCounter();
	~CSessionCounter();

public:
	// public accessors
	// session_id() returns a value in the range 0..(MAX_SESSION_IDS - 1)
	const unsigned int session_id() { return _session_id; }

private:
	// private variables - use public accessors and public mutators to address these
	unsigned int _session_id;

private:
	// private functions and variables - not available via accessors or mutators		
	HANDLE hMutex;

} *p_sessioncounter;

#endif //INC_CSESSIONCOUNTER_H