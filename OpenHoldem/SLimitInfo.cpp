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
	_sblind = kUndefined;
	_bblind = kUndefined;
	_bbet   = kUndefined;
	_ante   = kUndefined;
	_sb_bb  = kUndefined;
	_bb_BB  = kUndefined;
	_limit  = kUndefinedZero;
  _buyin  = kUndefined;
  _is_final_table = false;
}

double CLimitInfo::sblind(bool is_fixed_limit) {
  if (_sblind > kUndefinedZero) {
    return _sblind;
  }
  if (!is_fixed_limit) {
    if (_sb_bb > kUndefinedZero) {
      return _sb_bb;
    }
  } else {
    // Info not available for FL games
  }
  return kUndefinedZero;
}

double CLimitInfo::bblind(bool is_fixed_limit) {
  if (_bblind > kUndefinedZero) {
    return _bblind;
  }
  if (is_fixed_limit) {
    if (_sb_bb > kUndefinedZero) {
      return _sb_bb;
    } else {
      return _bb_BB;
    }
  }
  return kUndefinedZero;
}

double CLimitInfo::bbet(bool is_fixed_limit) {
  if (_bbet > kUndefinedZero) {
    return _bbet;
  }
  if (is_fixed_limit) {
    if (_bb_BB > kUndefinedZero) {
      return _bb_BB;
    }
  } else {
    // Info not available for NL and PL games
  }
  return kUndefinedZero;
}