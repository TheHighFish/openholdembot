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
  SelfTest();
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

const char kUnprintableBeepChar = 0x07;
const char kCharToBeRemoved     = kUnprintableBeepChar;

void RemoveLeftWhiteSpace(CString *s) {
  s->TrimLeft();
}

void RemoveRightWhiteSpace(CString *s) {
  s->TrimRight();
}

void RemoveMultipleWhiteSpaces(CString *s) {
  // Remove all sequences or more white spaces,
  // keep only the first white space.
  int length = s->GetLength();
  for (int i = 0; i < (length - 1); ++i) {
    switch (s->GetAt(i)) {
    case ' ':
    case '\t':
    case '\n':
    case kCharToBeRemoved:
      char next_char = s->GetAt(i + 1);
      switch (next_char) {
      case ' ':
      case '\t':
      case '\n':
        s->SetAt(i + 1, kCharToBeRemoved);
        break;
      }
      break;
    }
  }
  // Finally remove all superfluous characters
  s->Remove(kCharToBeRemoved);
}

void RemoveSpacesInsideNumbers(CString *s) {
  int length = s->GetLength();
  // Looking for patters like "7 500"
  // last_index is the last position where the first digit might occur.
  int last_index = length - 4;
  for (int i = 0; i < last_index; ++i) {
    if (isdigit(s->GetAt(i))
      && isspace(s->GetAt(i + 1))
      && isdigit(s->GetAt(i + 2))
      && isdigit(s->GetAt(i + 3))
      && isdigit(s->GetAt(i + 4))
      // Accessing +5 is save;
      // If things go bad it is \0
      && !isdigit(s->GetAt(i + 5))) {
      s->SetAt(i + 1, kCharToBeRemoved);
    }
  }
  // Finally remove all superfluous characters
  s->Remove(kCharToBeRemoved);
}

void ReplaceCommasInNumbersByDots(CString *s) {
  int length = s->GetLength();
  int last_index = length - 1;
  for (int i = 0; i < last_index; ++i) {
    if ((s->GetAt(i) == ',')
      && isdigit(s->GetAt(i - 1))
      && isdigit(s->GetAt(i + 1))) {
      s->SetAt(i, '.');
    }
  }
}

void ReplaceOutlandischCurrencyByDollarsandCents(CString *s) {
  int length = s->GetLength();
  for (int i = 0; i < length; ++i) {
    char current_char = s->GetAt(i);
    switch (current_char) {
    case '€':
    case '£': s->SetAt(i, '$');
      break;
    case '¢': s->SetAt(i, 'c');
      break;
    }
  }
}

void RemoveSpacesInFrontOfCentMultipliers(CString *s) {
  int length = s->GetLength();
  int last_index = length - 2;
  for (int i = 0; i < last_index; ++i) {
    if (isdigit(s->GetAt(i))
      && (isspace(s->GetAt(i + 1)))
      && (s->GetAt(i + 2) == 'c')) {
      s->SetAt(i + 1, kCharToBeRemoved);
    }
  }
  // Finally remove all superfluous characters
  s->Remove(kCharToBeRemoved);
}

void RemoveExtraDotsInNumbers(CString *s) {
  bool inside_number = false;
  bool dot_inside_number_seen = false;
  int length = s->GetLength();
  int last_index = length - 1;
  // Searching backwards for dots in numbers
  for (int i = last_index; i >= 0; --i) {
    char current_char = s->GetAt(i);
    if (isdigit(current_char)) {
      inside_number = true;
      continue;
    } else if (current_char == '.') {
      if (dot_inside_number_seen) {
        s->SetAt(i, kCharToBeRemoved);
      }
      dot_inside_number_seen = true;
      continue;
    } else {
      inside_number = false;
      dot_inside_number_seen = false;
    }
  }
  // Finally remove all superfluous characters
  s->Remove(kCharToBeRemoved);
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

void CTableTitle::SelfTest() {
#ifdef _DEBUG
  CString crappy_title = "  Robostars  Buyin €5,666.777,8     Ante 250 000      Rake 25 ¢     ";
  ReplaceOutlandischCurrencyByDollarsandCents(&crappy_title);
  crappy_title = "  Robostars  Buyin $5,666.777,8     Ante 250 000      Rake 25 c     ";
  RemoveLeftWhiteSpace(&crappy_title);
  assert(crappy_title == "Robostars  Buyin $5,666.777,8     Ante 250 000      Rake 25 c     ");
  RemoveRightWhiteSpace(&crappy_title);
  assert(crappy_title == "Robostars  Buyin $5,666.777,8     Ante 250 000      Rake 25 c");
  RemoveMultipleWhiteSpaces(&crappy_title);
  assert(crappy_title == "Robostars Buyin $5,666.777,8 Ante 250 000 Rake 25 c");
  RemoveSpacesInsideNumbers(&crappy_title);
  assert(crappy_title == "Robostars Buyin $5,666.777,8 Ante 250000 Rake 25 c");
  RemoveSpacesInFrontOfCentMultipliers(&crappy_title);
  assert(crappy_title == "Robostars Buyin $5,666.777,8 Ante 250000 Rake 25c");
  ReplaceCommasInNumbersByDots(&crappy_title);
  assert(crappy_title == "Robostars Buyin $5.666.777.8 Ante 250000 Rake 25c");
  RemoveExtraDotsInNumbers(&crappy_title);
  assert(crappy_title == "Robostars Buyin $5666777.8 Ante 250000 Rake 25c");
#endif
}