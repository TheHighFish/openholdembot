//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose:
//
//*****************************************************************************


#include "stdafx.h"
#include "NumericalFunctions.h"

// Deals with floating points, ints, hex and binary
double StringToNumber(CString number) {
  // http://www.cplusplus.com/reference/cstdlib/strtol/
  // http://stackoverflow.com/questions/10156409/convert-hex-string-char-to-int
  assert(number[0] != NULL);
  if (memcmp(number, "0x", 2) == 0) {
    // Hex number
    return strtol(number, NULL, 16);
  }
  else if (memcmp(number, "0b", 2) == 0) {
    // Binary number
    return strtol(number, NULL, 2);
  }
  else {
    // Decimal number, either int or floating point
    return atof(number);
  }
}

#define TWO(c) (0x1u << (c))
#define MASK(c) (((unsigned int)(-1)) / (TWO(TWO(c)) + 1u))
#define COUNT(x,c) ((x) & MASK(c)) + (((x) >> (TWO(c))) & MASK(c))

int bitcount (unsigned int n)
{
    n = COUNT(n, 0) ;
    n = COUNT(n, 1) ;
    n = COUNT(n, 2) ;
    n = COUNT(n, 3) ;
    n = COUNT(n, 4) ;
    /* n = COUNT(n, 5) ;    for 64-bit integers */
    return n ;
}

int Random(int n)
{
	// rang 0..randmax
	int r = rand();
	int result = double(n) * (double(r)/RAND_MAX);
	return result;
}