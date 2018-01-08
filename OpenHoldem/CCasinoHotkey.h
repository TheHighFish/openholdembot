//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Hotkeys as alternative for autoplayer-buttons
//
//******************************************************************************

#ifndef INC_CCasinoHotkey_H
#define INC_CCasinoHotkey_H



class CCasinoHotkey {
 public:
	CCasinoHotkey();
	~CCasinoHotkey();
 public:
  bool PressHotkey();
  void SetName(CString name);
 private:
  char LookupHotkey();
 private:
  CString _name;
}; 

#endif // INC_CCasinoHotkey_H