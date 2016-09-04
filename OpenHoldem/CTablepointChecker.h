//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Checking tablepoints on connection,
//   deciding if OH should reconnect if table-theme changes
//
//******************************************************************************

#ifndef INC_TABLEPOINTCHECKER_H
#define INC_TABLEPOINTCHECKER_H

const int kMaxToleratedHeartbeatsWithMismatchingTablepointsBeforeDisconnect = 10;

class CTablepointChecker {
 public:
  CTablepointChecker();
  ~CTablepointChecker();
 public:
  bool CheckTablepoints(HWND window_candidate, int tablemap_index, RECT r);
  bool CheckTablepointsOfCurrentTablemap();
  bool TablepointsMismatchedTheLastNHeartbeats();
 private:
  static int _number_of_mismatches_the_last_N_heartbeats;
};

#endif // INC_TABLEPOINTCHECKER_H