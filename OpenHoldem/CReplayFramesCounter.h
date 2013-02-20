#ifndef INC_CREPLAYFRAMESCOUNTER_H
#define INC_CREPLAYFRAMESCOUNTER_H

#include "..\CCritSec\CCritSec.h"

extern class CReplayFramesCounter
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
} *p_replayframes_counter;

#endif INC_CREPLAYFRAMESCOUNTER_H