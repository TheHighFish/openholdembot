#ifndef INC_CAUTOCONNECTOR_H
#define INC_CAUTOCONNECTOR_H

#include "..\CCritSec\CCritSec.h"
#include "..\CTablemap\CTablemap.h"

extern class CAutoConnector
{
public:
	CAutoConnector();
	~CAutoConnector();
	bool Connect(HWND targetHWnd);
	void Disconnect();
public:
	// public mutators
	#define ENT CSLock lock(m_critsec);
	void set_attached_hwnd(const HWND h) { ENT _attached_hwnd = h; }
#undef ENT
	const HWND attached_hwnd() { return _attached_hwnd; }
private:
	int SelectTableMapAndWindow(int Choices);
	int SelectTableMapAndWindowManually(int Choices);
	int SelectTableMapAndWindowAutomatically(int Choices);
	void ShutdownOH();
private:
	void Check_TM_Against_All_Windows(SWholeMap smap, HWND targetHWnd);
private:
	CCritSec		m_critsec;
private:
	// private variables - use public accessors and public mutators to address these
	HWND			_attached_hwnd;	 // Table that we are attached to
} *p_autoconnector;

#endif INC_CAUTOCONNECTOR_H