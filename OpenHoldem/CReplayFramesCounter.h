//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose: Encapsuling the counter in an extra class
//   and searching the directory only once on startup,
//   as the old solution enumerated all files in the replay-directory
//   for every single replay-frame, which lead to performance issues
//   for some people: http://www.maxinmontreal.com/forums/viewtopic.php?f=110&t=16135
//
//***************************************************************************** 

#ifndef INC_CREPLAYFRAMESCOUNTER_H
#define INC_CREPLAYFRAMESCOUNTER_H

#include "..\CCritSec\CCritSec.h"

class CReplayFramesCounter
{
public:
	CReplayFramesCounter();
	~CReplayFramesCounter();
public:
	// Has to be called *EXACTLY ONCE* per heartbeat,
	// as it increments the number
	int GetNumberOfNextReplayFrame();
private:
	int GetNumberOfLastReplayFrameInDirectory();
private:
	// _current_last_replay_frame holds the value of the last replay-frame in use.
	int _current_last_replay_frame;
	CCritSec m_critsec;
};

extern CReplayFramesCounter *p_replayframes_counter;

#endif INC_CREPLAYFRAMESCOUNTER_H