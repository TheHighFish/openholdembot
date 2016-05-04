//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Finding best matches to partially known blind-levels
//
//******************************************************************************

#ifndef INC_CBLINDLEVELS_H
#define INC_CBLINDLEVELS_H

class CBlindLevels {
 public:
  CBlindLevels();
  ~CBlindLevels();
 public:
  bool BestMatchingBlindLevel(double *sblind, double *bblind, double *bbet);
  // Especially used for guessing the small-blind,
  // which usually is 50% or sometimes 40% of the big-blind
  double GetNextSmallerOrEqualBlindOnList(double guessed_blind);
 private:
  bool BlindsMatchBlindLevelPerfectly(const int level,
                             const double sblind, 
                             const double bblind, 
                             const double bbet);
  bool BlindsMatchBlindLevelPartially(const int level,
                             const double sblind, 
                             const double bblind, 
                             const double bbet);
};

#endif INC_CBLINDLEVELS_H