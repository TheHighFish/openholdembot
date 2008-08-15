#include "StdAfx.h"

#include "debug.h"
#include "global.h"

class CGlobal	global;

CGlobal::CGlobal(void)
{
	__SEH_SET_EXCEPTION_HANDLER

	__SEH_HEADER

	__SEH_LOGFATAL("CGlobal::Constructor : \n");
}

CGlobal::~CGlobal(void)
{
	__SEH_HEADER

	__SEH_LOGFATAL("CGlobal::Destructor : \n");
}

