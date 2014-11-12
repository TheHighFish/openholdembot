//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: To be used with CMap-hashtables and CStrings as keys
//
//******************************************************************************

#ifndef INC_CSTRINGHASHTABLETEMPLATE_H
#define INC_CSTRINGHASHTABLETEMPLATE_H

// CString hashtable below by Sam NG
// http://www.codeproject.com/Articles/13458/CMap-How-to
template<> 
UINT AFXAPI HashKey<CString*> (CString* key);

// I don't know why, but CompareElements can't work with CString*
// have to define this
typedef CString* LPCString;

template<>
BOOL AFXAPI CompareElements<LPCString, LPCString> 
     (const LPCString* pElement1, const LPCString* pElement2);

// End of CString hashtable by Sam NG

#endif INC_CSTRINGHASHTABLETEMPLATE_H