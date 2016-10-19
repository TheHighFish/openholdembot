//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: small data container for the table-title 
//
//******************************************************************************

#include "stdafx.h"
#include "CTableTitle.h"

#include "CAutoconnector.h"
#include "..\StringFunctionsDLL\string_functions.h"

CTableTitle::CTableTitle() {
  Clear();
}

CTableTitle::~CTableTitle() {
}

void CTableTitle::UpdateTitle() {
	char title[MAX_WINDOW_TITLE];
	GetWindowText(p_autoconnector->attached_hwnd(), title, MAX_WINDOW_TITLE-1);
  SetTitle(title);
}

void CTableTitle::Clear() {
  _title = "";
  CString _previous_title = "";
}

void CTableTitle::SetTitle(CString new_title) {
  _previous_title = _title;
  _title = new_title;
}

CString CTableTitle::Title() {
  return _title;
}

CString CTableTitle::PreprocessedTitle() {
  // !!!!!
  // There are some annoying title formats that cause problems
  // for nirmal title processing. We try to transform these
  // mal-formed title-strings to good ones:
  // * spaces between numbers and cent multipliers at 888
  //   http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=19216
  // * spaces inside large numbers at WPN
  //   http://www.maxinmontreal.com/forums/viewtopic.php?f=120&t=20096&hilit=space%2A
  // * spaces at the beginning of the title
  //   http://www.maxinmontreal.com/forums/viewtopic.php?f=117&t=20085
  // One more problem:
  // * extra dots in mubers at Microgaming
  //   http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=19658
  // but the latter one mainly affects balances. The function
  // to handle this case should be in a library that gets shared
  // with the scraper.
  return _title;
}

bool CTableTitle::TitleChangedSinceLastHeartbeat() {
  return (_title != _previous_title);
}

