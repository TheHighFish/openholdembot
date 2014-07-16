//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose:
//
//*****************************************************************************

#ifndef INC_CAUTOCONNECTORTHREAD_H
#define INC_CAUTOCONNECTORTHREAD_H

class CAutoConnectorThread
{
public:
	CAutoConnectorThread();
	~CAutoConnectorThread();
public:
	// This function should be called, when everything else 
	// in the main application is initialized, but not earlier!
	void StartThread();
private:
	static UINT AutoConnectorThreadFunction(LPVOID pParam);
};

extern CAutoConnectorThread *p_autoconnectorthread;

#endif // INC_CAUTOCONNECTORTHREAD_H