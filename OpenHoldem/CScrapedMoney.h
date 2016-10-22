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

#ifndef INC_CSCRAPEDMONEY_H
#define INC_CSCRAPEDMONEY_H

#include "afxwin.h"

class CScrapedMoney {
 public:
  CScrapedMoney();
  ~CScrapedMoney();
 public:
  // returns true if the scraped value is a number and non-negative
  bool SetValue(CString scraped_value);
  void Reset();
  double GetValue() { return _value; }
 private:
  double _value;
};

#endif INC_CSCRAPEDMONEY_H


