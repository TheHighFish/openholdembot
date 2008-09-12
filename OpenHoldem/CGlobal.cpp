#include "StdAfx.h"
#include <io.h>
#include <fcntl.h>

#include "CGlobal.h"
#include "CPreferences.h"
#include "OpenHoldem.h"

#include "CScraper.h"
#include "CSymbols.h"
#include "CHeartbeatThread.h"
#include "..\CTablemap\CTablemap.h"
#include "CGrammar.h"

#include "resource.h"
#include "DialogFormulaScintilla.h"

CGlobal				*p_global = NULL;
CRITICAL_SECTION	CGlobal::cs_global;

CGlobal::CGlobal(void)
{
	__SEH_SET_EXCEPTION_HANDLER

	int			i = 0;

	InitializeCriticalSectionAndSpinCount(&cs_global, 4000);
	
	EnterCriticalSection(&cs_global);
		_attached_hwnd = NULL;

		for (i=0; i<10; i++)
			_flags[i] = false;
	LeaveCriticalSection(&cs_global);

	mm_network = "";

	EnterCriticalSection(&cs_global);
		_m_wait_cursor = false;
	LeaveCriticalSection(&cs_global);
}

CGlobal::~CGlobal(void)
{
	DeleteCriticalSection(&cs_global);
}
