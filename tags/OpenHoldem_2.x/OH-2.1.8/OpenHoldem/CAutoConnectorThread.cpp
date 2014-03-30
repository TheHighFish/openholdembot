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
	write_log(3, "CAutoConnectorThread::StartThread()\n");
	AfxBeginThread(AutoConnectorThreadFunction, this);
}

UINT CAutoConnectorThread::AutoConnectorThreadFunction(LPVOID pParam)
{
	write_log(3, "CAutoConnectorThread::AutoConnectorThreadFunction(..)\n");
	while (true)
	{
		if ((prefs.autoconnector_when_to_connect() == k_AutoConnector_Connect_Permanent) && !p_autoconnector->IsConnected())
		{
			if(p_autoconnector->TimeSinceLastFailedAttemptToConnect() > 1 /* seconds */)
			{
				write_log(3, "CAutoConnectorThread: going to call Connect()");
				p_autoconnector->Connect(NULL);
			}
			else
			{
				write_log(3, "CAutoConnectorThread: Reconnection blocked. Other instance failed previously.\n");
			}
		}
		Sleep(1000); // 1000 milli-seconds
	}
	return 0;
}