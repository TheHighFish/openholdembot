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

#ifndef INC_CDLLEXTENSION_H
#define INC_CDLLEXTENSION_H

// All functions and data-types for communication
// are defined in the user.DLL with proper
// import/export-directives that work automatically
// for DLL and OpenHoldem.
#include "..\Reference User DLL\user.h"

class CDllExtension {
 public:
	// public functions
	CDllExtension();
	~CDllExtension();
 public:
	void Load(const char * path);
	void Unload();
	bool IsLoaded();
 private:
	// private variables - use public accessors and public mutators to address these
	HMODULE	_hmod_dll;
};

extern CDllExtension *p_dll_extension;

#endif INC_CDLLEXTENSION_H 
