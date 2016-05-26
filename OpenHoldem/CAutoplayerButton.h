//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Clean interface for scraper, casino-interface amd autoplayer
//
//******************************************************************************

#ifndef INC_CAUTOPLAYERBUTTON_H
#define INC_CAUTOPLAYERBUTTON_H

#include "CCasinoHotkey.h"
#include "MagicNumbers.h"

class CAutoplayerButton {
  friend class CScraper;
 public:
  CAutoplayerButton();
  ~CAutoplayerButton();
 public:
  void SetTechnicalName(const CString name);
  void Reset();
  bool Click();
  bool IsClickable() { 
    return _clickable; 
  }
 public:
  // The button-type of iX-buttons depends on the scraped label.
  // BetPot-buttons don't have a label, they get identified by ID
  // (position in button-array).
  // There might be multiple (moving) buttons for one action,
  // named e.g allin1 and allin2, this works as we check
  // only the first N characters.
  bool IsAllin();
  bool IsRaise();
  bool IsCall();
  bool IsCheck();
  bool IsFold();
  bool IsAutopost();
  bool IsSitin();
  bool IsSitout();
  bool IsLeave();
  bool IsRematch();
  bool IsPrefold();
 protected:
  // To be used by the scraper
  void SetClickable(bool clickable);
  // Same as above, but to be used with input like "true", "yes", "on", ...
  void SetState(const CString state);
  void SetLabel(const CString label);
 protected:
  CCasinoHotkey _hotkey;
 private:
  bool _clickable;
  // label = logical name, e.g. "raise"
  CString _label;
  // technical name = tablemap-object, e.g. "i3button"
  CString _technical_name;
};

#endif // INC_CAUTOPLAYERBUTTON_H