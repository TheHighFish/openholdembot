//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

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
	
	return 0;
}
