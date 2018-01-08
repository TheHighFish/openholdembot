#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: checking the game-state for inconsistencies
//
//******************************************************************************

// !! ATTENTION
// !! CString not properly declared/included.
// !! atlstr / afxstr don't work.
// !! Any other includes cause lots of erros
// !! (escpecially CSpaceOptimiedObject.h).

class CValidator {
 public:
	// public functions
	CValidator();
	~CValidator();
 public:
  void Validate();
	void SetEnabledManually(bool Enabled);
 private:
  bool _enabled_manually;
};

extern CValidator *p_validator; 																																																					