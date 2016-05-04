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

bool CTableTitle::TitleChangedSinceLastHeartbeat() {
  return (_title != _previous_title);
}