//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: class for bets, balances and pots,
//  which also handles proprocessing of special num,ber formats, ...
//
//******************************************************************************

#include "stdafx.h"
#include "CScrapedMoney.h"

#include "..\StringFunctionsDLL\string_functions.h"

CScrapedMoney::CScrapedMoney() {
  Reset();
}

CScrapedMoney::~CScrapedMoney() {
}

bool CScrapedMoney::SetValue(CString scraped_value) {
  RemoveLeftWhiteSpace(&scraped_value);
  RemoveRightWhiteSpace(&scraped_value);
  RemoveMultipleWhiteSpaces(&scraped_value);
  RemoveSpacesInsideNumbers(&scraped_value);
  ReplaceOutlandischCurrencyByDollarsandCents(&scraped_value);
  RemoveSpacesInFrontOfCentMultipliers(&scraped_value);
  ReplaceCommasInNumbersByDots(&scraped_value);
  RemoveExtraDotsInNumbers(&scraped_value);
  KeepBalanceNumbersOnly(&scraped_value);
  double result = atof(scraped_value); //!!!!!
  if (result >= 0.0) {
    _value = result;
    return true;
  }
  return false;
}

void CScrapedMoney::Reset() {
  _value = 0.0;
}