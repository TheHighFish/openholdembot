//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Data container for scraped buttons etc.
//
//******************************************************************************

#ifndef INC_CSCRAPEDACTIONINTERFACE_H
#define INC_CSCRAPEDACTIONINTERFACE_H

#include "afxstr.h"

class CScrapedActionInterface {
  friend class CScraper;
 public:
  CScrapedActionInterface();
  ~CScrapedActionInterface();
 public:
   /*
  CString	i86_button_state()         { return _i86_button_state; }
	CString	i86X_button_state(int n)   { RETURN_DEFAULT_IF_OUT_OF_RANGE(n, (k_max_number_of_i86X_buttons-1), "") return _i86X_button_state[n]; }
	CString	betpot_button_state(int n) { RETURN_DEFAULT_IF_OUT_OF_RANGE(n, (k_max_betpot_buttons-1), "")         return _betpot_button_state[n]; }
  const CString		button_state(int n)        { RETURN_DEFAULT_IF_OUT_OF_RANGE(n, k_last_chair, "")      return _button_state[n]; }
  const CString		button_label(int n) { if (n>=0 && n<=9) return _button_label[n]; else return ""; }
	const bool			handle_found_at_xy() { return _handle_found_at_xy; }
	const POINT			handle_xy() { return _handle_xy; }
  */
 protected:
  CString _i86_button_state;
	CString	_i86X_button_state[k_max_number_of_i86X_buttons];
	CString	_button_state[k_max_number_of_buttons];
	CString _button_label[k_max_number_of_buttons];
	CString	_betpot_button_state[k_max_betpot_buttons];
  bool		_handle_found_at_xy;
	POINT		_handle_xy;
 private:
};

#endif INC_CSCRAPEDACTIONINTERFACE_H