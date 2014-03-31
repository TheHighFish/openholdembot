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