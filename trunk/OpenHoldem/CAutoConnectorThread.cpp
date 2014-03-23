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

#include "stdafx.h"
#include "CAutoConnector.h"
#include "CAutoConnectorThread.h"
#include "CPreferences.h"
#include "CSharedMem.h"
#include "CTableMapLoader.h"

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
	write_log(preferences.debug_autoconnector(), "[CAutoConnectorThread] StartThread()\n");
	AfxBeginThread(AutoConnectorThreadFunction, this);
}

UINT CAutoConnectorThread::AutoConnectorThreadFunction(LPVOID pParam)
{
	write_log(preferences.debug_autoconnector(), "[CAutoConnectorThread] AutoConnectorThreadFunction(..)\n");
	while (true)
	{
		p_tablemap_loader->ReloadAllTablemapsIfChanged();
		if ((preferences.autoconnector_when_to_connect() == k_AutoConnector_Connect_Permanent) 
			&& !p_autoconnector->IsConnected())
		{
			if(p_autoconnector->TimeSincelast_failed_attempt_to_connect() > 1 /* seconds */)
			{
				write_log(preferences.debug_autoconnector(), "[CAutoConnectorThread] going to call Connect()");
				p_autoconnector->Connect(NULL);
			}
			else
			{
				write_log(preferences.debug_autoconnector(), "[CAutoConnectorThread] Reconnection blocked. Other instance failed previously.\n");
			}
		}
		Sleep(1000); // 1000 milli-seconds
	}
	return 0;
}
