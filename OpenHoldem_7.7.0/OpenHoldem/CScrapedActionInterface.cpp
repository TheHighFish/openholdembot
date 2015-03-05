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

#include "stdafx.h"
#include "CScrapedActionInterface.h"

CScrapedActionInterface::CScrapedActionInterface() {
  Reset();
}

CScrapedActionInterface::~CScrapedActionInterface() {
}

void CScrapedActionInterface::Reset() {
  for (int i=0; i<k_max_number_of_buttons; i++)  {
    _button_state[i] = "false";
		_button_label[i] = "";
    _i86X_button_state[i] = "false";
  }
  _i86_button_state = "false";
  _button_label[0]  = "fold";
	_button_label[1]  = "call";
	_button_label[2]  = "raise";
	_button_label[3]  = "allin";
}
