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
  Reset();
}

CLimitInfo::~CLimitInfo() {
}

void CLimitInfo::Reset() {
  _handnumber = "";
	_sblind = k_undefined;
	_bblind = k_undefined;
	_bbet   = k_undefined;
	_ante   = k_undefined;
	_sb_bb  = k_undefined;
	_bb_BB  = k_undefined;
	_limit  = k_undefined_zero;
  _buyin  = k_undefined;
  _is_final_table = false;
}

double CLimitInfo::sblind(bool is_fixed_limit) {
  if (_sblind > k_undefined_zero) {
    return _sblind;
  }
  if (!is_fixed_limit) {
    if (_sb_bb > k_undefined_zero) {
      return _sb_bb;
    }
  } else {
    // Info not available for FL games
  }
  return k_undefined_zero;
}

double CLimitInfo::bblind(bool is_fixed_limit) {
  if (_bblind > k_undefined_zero) {
    return _bblind;
  }
  if (is_fixed_limit) {
    if (_sb_bb > k_undefined_zero) {
      return _sb_bb;
    } else {
      return _bb_BB;
    }
  }
  return k_undefined_zero;
}

double CLimitInfo::bbet(bool is_fixed_limit) {
  if (_bbet > k_undefined_zero) {
    return _bbet;
  }
  if (is_fixed_limit) {
    if (_bb_BB > k_undefined_zero) {
      return _bb_BB;
    }
  } else {
    // Info not available for NL and PL games
  }
  return k_undefined_zero;
}