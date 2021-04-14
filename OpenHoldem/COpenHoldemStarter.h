#pragma once
//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: starting and terminating bots automatically
//
//******************************************************************************

// For time_t
#include "corecrt.h"

class COpenHoldemStarter {
public:
  COpenHoldemStarter();
  ~COpenHoldemStarter();
public:
  void StartNewInstanceIfNeeded();
  void CloseThisInstanceIfNoLongerNeeded();
private:
  time_t _starting_time_of_last_instance;
};

