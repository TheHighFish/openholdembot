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

#ifndef INC_STDAFX_H
#define INC_STDAFX_H

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// Turn some warnings off, because we consider them harmless
// and don't see an easy way to fix or avoid them.
#define _CRT_SECURE_NO_DEPRECATE 1  // secure functions with checks for buffer size 
#pragma warning(disable:4229)		// anachronism used : modifiers on data are ignored
#pragma warning(disable:4312)		// conversion from 'type1' to 'type2' of greater size
#pragma warning(disable:4319)		// zero extending 'unsigned long' to 'double' of greater size
#pragma warning(disable:4805)		// unsafe mix of type X and type 'bool' in operation

// Turn some warnings on, because we consider them helpful
// http://msdn.microsoft.com/en-us/library/2c8f766e(v=vs.80).aspx
// http://msdn.microsoft.com/en-US/library/23k5d385%28v=VS.80%29.aspx
#pragma warning(error:6246)		// warning C6246: Local declaration of <variable> hides declaration of same name in outer scope.
#pragma warning(error:4229)		// warning C6244: local declaration of <variable> hides previous declaration at <line> of <file>

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// ======================================================================================

#include <afxtempl.h>
#include <afxpriv.h>
#include <afxdlgs.h>

#include <afxadv.h>			// MFC support for MRU

#include <afxmt.h>			// CMutex

#include <windows.h>
#include <stdlib.h>
#include <string.h>

// Version
#define VERSION_NUMBER				4.41
#define VERSION_TEXT				"4.4.1"  // change version number in OpenHoldem.rc also, when needed

// PokerEval
#include "poker_defs.h"
#include "pokereval_export.h"
#include "evx_defs.h"
//#include "inlines/evx_inlines.h"

// Important project headers
#include "debug.h"
#include "..\StructsDefines\structs_defines.h"
#include "MagicNumbers.h"


#endif //INC_STDAFX_H