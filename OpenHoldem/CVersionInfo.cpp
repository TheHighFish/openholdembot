//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Provides version info for the debug-log
//   (version and filesize) to distinguish various versions of OH
//   and self-compiled derivates.
//   We have seen way too much "bug"-reports for older versions or modified code,
//   sometimes wasting a couple of nights with unneccessary debug-sessions.
//   Enough!
//
//******************************************************************************

#include "stdafx.h"
#include "CVersionInfo.h"

#include <assert.h>
#include "CFilenames.h"
#include "CFunctionCollection.h"
#include "..\CTablemap\CTablemap.h"
#include "MD5_Checksum.h"

CVersionInfo *p_version_info = NULL;

CVersionInfo::CVersionInfo()
{
	// Getting the check-sum only once on start-up
	_openholdem_MD5 = CalculateMD5(p_filenames->PathOfExecutable());
}

CVersionInfo::~CVersionInfo()
{}

CString CVersionInfo::GetVersionInfo()
{
	CString version_info;

	assert(p_function_collection != NULL);
	assert(p_tablemap != NULL);

	version_info.Format("OpenHoldem\n" 
		"  Version  [%s, %s]\n" 
		"  Formula  [%s]\n" 
		"  Tablemap [%s]\n" 
		"  DLL      [%s]\n",
		VERSION_TEXT,
		_openholdem_MD5,
		p_function_collection->Title(),
		p_tablemap->filename(),
		p_function_collection->DLLPath());
	return version_info;
}
