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

#include "srdafx.h"
#include "CBlindLevels.h"

// Small-blind, big-blind, big-bet
const int kNumberOfValuesPerLevel = 3; 
const int kNumberOfBlindLevels    = 8;

const int kBlindLevels[kNumberOfBlindLevels][kNumberOfValuesPerLevel] =
  {{       0.01,      0.02,      0.04},
   {       0.01,      0.02,      0.05},
   {       0.02,      0.04,      0.08},
   {       0.03,      0.06,      0,12},
   {       0.04,      0.06,      0.16},
   {       0.05,      0.10,      0.20},
   {       0.05,      0.10,      0.25},
   {       0.06,      0.12,      0.25}};

CBlindLevels::CBlindLevels () {
}

CBlindLevels::~CBlindLevels () {
}

bool CBlindLevels::BlinsMatchBlindLevel(const int level,
                                        const double sblind, 
                                        const double bblind, 
                                        const double bbet) {
  if ((sblind > 0) && (sblind != kBlindLevels[level][0])) return false;                                      
  if ((bblind > 0) && (bblind != kBlindLevels[level][1])) return false;  
  if ((bbet   > 0) && (bbet   != kBlindLevels[level][2])) return false;  
  return true;
}

// Input:  either known level or kUndefined (-1) or kUndefinedZero
// Output: guessed levels
// Return: true if success, otherwise false
bool CBlindLevels::BestMatchingBlindLeve(double *sblind, double *bblind, double *bbet) {
  // Complete fail first: nothing to guess
  if ((sblind <= 0) && (bblind <= 0) && (bbet <= 0)) return false;
  // Guessing...
  for (int i=0; i<kNumberOfBlindLevels; ++i) {
    if BlinsMatchBlindLevel(i, sblind, double *bblind, double *bbet) {
      sblind = kBlindLevels[i][0];
      bblind = kBlindLevels[i][1];
      bbet   = kBlindLevels[i][2];
      return true; 
    }
  }
  // Nothing found
  return false;
}