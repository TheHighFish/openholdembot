#ifndef INC_CREPLAYFRAME_H
#define INC_CREPLAYFRAME_H

#include "..\CCritSec\CCritSec.h"

class CReplayFrame 
{
public:
	// public functions
	CReplayFrame(void);
	~CReplayFrame(void);
	void CreateReplayFrame(void);

private:
	// private functions and variables - not available via accessors or mutators
	CString GetCardHtml(unsigned int card);
	CString GetPlayerInfoAsHTML();
	CString GetButtonStatesAsHTML();
	CString GetBlindInfoAsHTML();
	CString GetCommonCardsAsHTML();
	CString GetPotsAsHTML();
	CString GetLinksToPrevAndNextFile();
	void CreateBitMapFile();

	int			_next_replay_frame;

	CCritSec	m_critsec;
};

#endif /* INC_CREPLAYFRAME_H */