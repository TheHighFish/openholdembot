//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Flexible delay for the heartbeat iteration
//
//******************************************************************************

#ifndef INC_CHEARTBEATDELAY_H
#define INC_CHEARTBEATDELAY_H

class CHeartbeatDelay { 
 public:
  CHeartbeatDelay();
  ~CHeartbeatDelay();
 public:
   // Should be executed exactly once per heartbeat
   // by CHeartbeat.cpp
  void FlexibleSleep();
 private:
  double SleepingFactor();
  double SleepingFactorNotSeated();
  double SleepingFactorPlayingNotMyTurn();
  double SleepingFactorActiveButFolded();
};

#endif //INC_CHEARTBEATDELAY_H