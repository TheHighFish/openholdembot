//****************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//****************************************************************************** 
//
// Purpose: first guess of blind values for CTableLimits,
//   which might be overridden bz blind-locking.
//
//****************************************************************************** 

#ifndef INC_CBLINDGUESSER_H
#define INC_CBLINDGUESSER_H

class CBlindGuesser {
 public:
  CBlindGuesser();
  ~CBlindGuesser();
 public:
  void Guess(double *sblind, double *bblind, double *bbet);
};

#endif INC_CBLINDGUESSER_H