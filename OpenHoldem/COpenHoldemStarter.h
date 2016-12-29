#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: starting and terminating bots automatically
//
//******************************************************************************

class COpenHoldemStarter {
public:
  COpenHoldemStarter();
  ~COpenHoldemStarter();
public:
  void StartNewInstanceIfNeeded();
  void CloseThisInstanceIfNoLongerNeeded();
private:
};

extern COpenHoldemStarter *p_openholdem_starter;