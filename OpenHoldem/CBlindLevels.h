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

class CBlindLevels {
 public:
  CBlindLevels();
  ~CBlindLevels();
 public:
  bool BestMatchingBlindLeve(double *sblind, double *bblind, double *bbet);
  bool BlindsMatchBlindLevel(const int level,
                             const double sblind, 
                             const double bblind, 
                             const double bbet);
};
