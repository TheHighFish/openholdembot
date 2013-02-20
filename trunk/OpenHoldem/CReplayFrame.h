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
//   for some people: http://!!!
//
//*****************************************************************************

#ifndef INC_CREPLAYFRAME_H
#define INC_CREPLAYFRAME_H

class CReplayFrame 
{
public:
	// public functions
	CReplayFrame(void);
	~CReplayFrame(void);
	void CreateReplayFrame(void);
private:
	// private functions and variables - not available via accessors or mutators
	void CreateReplaySessionDirectoryIfNecessary();
	CString GetCardHtml(unsigned int card);
	CString GetPlayerInfoAsHTML();
	CString GetButtonStatesAsHTML();
	CString GetBlindInfoAsHTML();
	CString GetCommonCardsAsHTML();
	CString GetPotsAsHTML();
	CString GetLinksToPrevAndNextFile();
	void CreateBitMapFile();
private:
	int			_next_replay_frame;
};

#endif /* INC_CREPLAYFRAME_H */