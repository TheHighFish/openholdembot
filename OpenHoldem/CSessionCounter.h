#include "stdafx.h"
#include <windows.h>

class CSessionCounter
{
public:
	CSessionCounter();
	~CSessionCounter();
public:
	unsigned int get_Session_ID();
private:
	static const int MAX_SESSION_IDS = 25;
private:
	HANDLE hMutex;
	unsigned int session_ID;
};

extern CSessionCounter SessionCounter;