#ifndef INC_CSESSIONCOUNTER_H
#define INC_CSESSIONCOUNTER_H

extern class CSessionCounter
{
public:
	// public functions
	CSessionCounter();
	~CSessionCounter();

public:
	// public accessors
	const unsigned int session_id() { return _session_id; }

private:
	// private variables - use public accessors and public mutators to address these
	unsigned int _session_id;

private:
	// private functions and variables - not available via accessors or mutators	
	static const int MAX_SESSION_IDS = 25;
	HANDLE hMutex;

} SessionCounter;

#endif //INC_CSESSIONCOUNTER_H