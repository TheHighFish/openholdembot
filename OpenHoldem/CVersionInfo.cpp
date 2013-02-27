//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Provides version info for the debug-log
//   (version and filesize) to distinguish various versions of OH
//   and self-compiled derivates.
//   We have seen way too much "bug"-reports for older versions or modified code,
//   sometimes wasting a couple of nights with unneccessary debug-sessions.
//   Enough!
//
//*****************************************************************************

#include "stdafx.h"
#include "CVersionInfo.h"

#include <assert.h>
#include "CDllExtension.h"
#include "CFormula.h"
#include "..\CTablemap\CTablemap.h"

CVersionInfo *p_version_info = NULL;

CVersionInfo::CVersionInfo()
{}

CVersionInfo::~CVersionInfo()
{
	// Getting the file-size only once on start-up
	_openholdem_file_size = GetFileSize();
}

CString CVersionInfo::GetVersionInfo()
{
	CString version_info;

	assert(p_formula != NULL);
	assert(p_tablemap != NULL);
	assert(p_dll_extension != NULL);

	version_info.Format("OpenHoldem version [%s, %li], formula [%s], tablemap [%s], DLL [%s]",
		VERSION_TEXT,
		GetFileSize(),
		p_formula->formula_name(),
		p_tablemap->filename(),
		p_dll_extension->GetDLLSpecifiedInFormula());
	return version_info;
}

long int CVersionInfo::GetFileSize()
{
	//!!!
	return 0;
}