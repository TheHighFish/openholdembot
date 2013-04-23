#include "stdafx.h"
#include "CSharedMem.h"

#include "CPreferences.h"
#include "CSessionCounter.h"

// For an introduction about shared memory see:
//   * http://www.programmersheaven.com/mb/Win32API/156366/156366/shared-memory-and-dll/
//   * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2044.html
//   * http://www.codeproject.com/KB/threads/SharedMemory_IPC_Threads.aspx
//   * http://www.fstsoft.de/shared.htm (german)
//
// We chose the first solution for its simplicity


///////////////////////////////////////////////////////////////////////////////////
//
// SHARED MEMORY for communication and synchronisation of OH-instances.
//
///////////////////////////////////////////////////////////////////////////////////

#pragma data_seg(".ohshmem") // names are limited to 8 chars, including the dot.

__declspec(allocate(".ohshmem"))	static	HWND	attached_poker_windows[MAX_SESSION_IDS] = { NULL };	// for the auto-connector
__declspec(allocate(".ohshmem"))	static	time_t	last_failed_attempt_to_connect;	// last time any instance failed; to avoid superflous attempts by other instances of OH
__declspec(allocate(".ohshmem"))	static	int		session_ID_of_last_instance_that_failed_to_connect; 
__declspec(allocate(".ohshmem"))	static	HWND	dense_list_of_attached_poker_windows[MAX_SESSION_IDS] = { NULL }; // for the table positioner
__declspec(allocate(".ohshmem"))	static	int		size_of_dense_list_of_attached_poker_windows;


#pragma data_seg()
#pragma comment(linker, "/SECTION:.ohshmem,RWS")		// RWS: read, write, shared

///////////////////////////////////////////////////////////////////////////////////
//
// CLASS CSharedMem to access the shared memory
//
///////////////////////////////////////////////////////////////////////////////////

CSharedMem	*p_sharedmem = NULL;

CSharedMem::CSharedMem()
{}


CSharedMem::~CSharedMem()
{}


bool CSharedMem::PokerWindowAttached(HWND Window)
{
	ENT;
	for (int i=0; i<MAX_SESSION_IDS; i++)
	{
		if (attached_poker_windows[i] == Window)
		{
			// Window found. Already attached.
			return true;
		}
	}
	// Window not found. Not attached.
	return false;
}


void CSharedMem::MarkPokerWindowAsAttached(HWND Window)
{
	ENT;
	attached_poker_windows[p_sessioncounter->session_id()] = Window;	
}


void CSharedMem::MarkPokerWindowAsUnAttached()
{
	ENT;
	attached_poker_windows[p_sessioncounter->session_id()] = NULL;
}


void CSharedMem::RememberTimeOfLastFailedAttemptToConnect()
{
	ENT;
	time(&last_failed_attempt_to_connect);
	write_log(prefs.debug_autoconnector(), "Set last_failed_attempt_to_connect %d\n", last_failed_attempt_to_connect);
	session_ID_of_last_instance_that_failed_to_connect = p_sessioncounter->session_id();
	write_log(prefs.debug_autoconnector(), "Failed session ID: %d\n", session_ID_of_last_instance_that_failed_to_connect);
}


time_t CSharedMem::GetTimeOfLastFailedAttemptToConnect()
{
	ENT;
	write_log(prefs.debug_autoconnector(), "Get last_failed_attempt_to_connect %d\n", last_failed_attempt_to_connect);
	write_log(prefs.debug_autoconnector(), "Stored by failed session ID: %d\n", session_ID_of_last_instance_that_failed_to_connect);
	return last_failed_attempt_to_connect;
}

HWND *CSharedMem::GetDenseListOfConnectedPokerWindows()
{
	ENT;
	// Some functions like TileWindows() and CascadeWindows()
	// need a dense list of poker-tables without NULLs inbetween,
	// otherwise non-tables would be affected, too.
	//
	// See e.g. TileWindows():
	//   http://msdn.microsoft.com/en-us/library/windows/desktop/ms633554(v=vs.85).aspx
	//   lpKids [in, optional]
	//   Type: const HWND*
    //   An array of handles to the child windows to arrange. 
	//   If a specified child window is a top-level window 
	//   with the style WS_EX_TOPMOST or WS_EX_TOOLWINDOW, 
	//   the child window is not arranged. If this parameter is NULL, 
	//   all child windows of the specified parent window 
	//   (or of the desktop window) are arranged.
	CreateDenseListOfConnectedPokerWindows();
	return dense_list_of_attached_poker_windows;
}

void CSharedMem::CreateDenseListOfConnectedPokerWindows()
{
	int size_of_list = 0;
	for (int i=0; i<MAX_SESSION_IDS; i++)
	{
		if (attached_poker_windows[i] != NULL)
		{
			dense_list_of_attached_poker_windows[size_of_list] = attached_poker_windows[i];
			size_of_list++;
		}
	}
	// Only at the very end update the size of the list.
	// This helps to avoid potential race-conditions,
	// if another application uses the list at the same time,
	// although at least the creation of the list is protected.
	// Updating the content of the list doesn't harm much,
	// but working with an incorrect size would.
	size_of_dense_list_of_attached_poker_windows = size_of_list;
}

int CSharedMem::SizeOfDenseListOfAttachedPokerWindows()
{
	return size_of_dense_list_of_attached_poker_windows;
}
