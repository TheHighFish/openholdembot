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

#include "CBlindLevels.h"

class CBlindGuesser {
 public:
  CBlindGuesser();
  ~CBlindGuesser();
 public:
  void Guess(double *sblind, double *bblind, double *bbet);
private:
  void GetFirstBlindDataFromScraper(double *sblind, double *bblind, double *bbet);
  void GetFirstBlindDataFromBetsAtTheTable(double *sblind, double *bblind, double *bbet);
 private:
  bool SBlindBBlindCombinationReasonable(double sblind, double bblind);
  bool SBlindBBetCombinationReasonable(double sblind, double bbet);
  bool BBlindBBetCombinationReasonable(double bblind, double bbet);
 private:
  // Some beautiful number in the range of 40..50%
  double ReasonableLookingHalfBlindValue(double known_value);
  bool CompletePartiallyKnownBlinds(double *sblind, double *bblind, double *bbet);
 private:
  CBlindLevels _blind_levels;
};

#endif INC_CBLINDGUESSER_H