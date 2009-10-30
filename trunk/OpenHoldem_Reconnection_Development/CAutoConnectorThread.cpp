#include "stdafx.h"
#include "CAutoConnector.h"
#include "CAutoConnectorThread.h"
#include "CPreferences.h"
#include "CSharedMem.h"

CAutoConnectorThread *p_autoconnectorthread = NULL;

CAutoConnectorThread::CAutoConnectorThread()
{
}

CAutoConnectorThread::~CAutoConnectorThread()
{
}

// This function should be called, when everything else 
// in the main application is initialized, but not earlier!
void CAutoConnectorThread::StartThread()
{
	AfxBeginThread(AutoConnectorThreadFunction, this);
}

UINT CAutoConnectorThread::AutoConnectorThreadFunction(LPVOID pParam)
{
	while (true)
	{
		if ((prefs.autoconnector_when_to_connect() == k_AutoConnector_Connect_Permanent) && !p_autoconnector->IsConnected())
		{
			if(p_autoconnector->TimeSinceLastFailedAttemptToConnect() > prefs.autoconnector_time_to_next_try())
			{
				MessageBox(0, "AutoConnectorThreadFunction()", "Debug", 0);
				p_autoconnector->Connect(0);
				Sleep(prefs.autoconnector_time_to_next_try() * 1000);
			}
		}
	}
	return 0;
}