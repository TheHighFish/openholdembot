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

#include "stdafx.h"
#include "SLimitInfo.h"

CLimitInfo::CLimitInfo() {
  handnumber = "";
	sblind = k_undefined;
	bblind = k_undefined;
	bbet   = k_undefined;
	ante   = k_undefined;
	sb_bb  = k_undefined;
	bb_BB  = k_undefined;
	limit  = k_undefined_zero;
  buyin  = k_undefined;
  is_final_table = false;
}

CLimitInfo::~CLimitInfo() {
}