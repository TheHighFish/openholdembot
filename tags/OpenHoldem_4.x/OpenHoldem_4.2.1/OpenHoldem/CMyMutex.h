#ifndef INC_MYMUTEX_H
#define INC_MYMUTEX_H

// Wrapper around CMutex objects to provide for automatic
// acquire and release of the mutex.


class CMyMutex : public CMutex
{
public:
	CMyMutex();
	~CMyMutex();
	bool IsLocked();

private:
	CMutex _mutex;
	bool _locked;
};

#endif INC_MYMUTEX_H
