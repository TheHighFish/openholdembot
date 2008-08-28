#include "stdafx.h"
#include "CSessionCounter.h"

// Global instantiation of CSessionCounter class
CSessionCounter SessionCounter;

CSessionCounter::CSessionCounter()
{	
	_session_id = -1;

	// Try to get a _session_id that is not in use.
	for (int i=0; i<MAX_SESSION_IDS; i++)
	{
		// Name the mutexes A..Y
		CString mutex_name = "Session_ID_Mutex_" + CString(char('A' + i));
		hMutex = CreateMutex(0, FALSE, mutex_name);
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			_session_id = i;
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
		CString("Could not grab a session ID.\n")
		+ CString("Too many instances of OpenHoldem\n"),
		"SessionCounter Error",
		0);	
}

CSessionCounter::~CSessionCounter()
{
	// Release the mutex for our _session_id
	CloseHandle(hMutex);
}

