#include "stdafx.h"
#include "CSessionCounter.h"

CSessionCounter::CSessionCounter()
{	
	session_ID = -1;
	// Try to get a session_ID that is not in use.
	for (int i=0; i<MAX_SESSION_IDS; i++)
	{
		// Name the mutexes A..Y
		CString mutex_name = "Session_ID_Mutex_" + CString(char('A' + i));
		hMutex = CreateMutex(0, FALSE, mutex_name);
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			session_ID = i;
			return;
		}
		else
		{
			CloseHandle(hMutex);
		}
	}
	// Otherwise: We failed. No ID available.
	MessageBox(
		0, 
		CString("Could not grab a session_ID.\n")
		+ CString("Too many instances of OpenHoldem\n"),
		"SessionCounter Error",
		0);	
}

CSessionCounter::~CSessionCounter()
{
	// Release the mutex for our session_ID
	CloseHandle(hMutex);
}

unsigned int CSessionCounter::get_Session_ID()
{	
	return session_ID;
}

CSessionCounter SessionCounter;