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

#ifndef INC_CVERSIONINFO_H
#define INC_CVERSIONINFO_H

extern class CVersionInfo
{
public:
	CVersionInfo();
	~CVersionInfo();
public:
	CString GetVersionInfo();
private:
	CString _openholdem_MD5;
} *p_version_info;

#endif INC_CVERSIONINFO_H