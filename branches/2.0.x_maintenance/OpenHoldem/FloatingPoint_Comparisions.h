#ifndef INC_FLOATINGPOINT_COMPARISIONS_H
#define INC_FLOATINGPOINT_COMPARISIONS_H

#include "stdafx.h"

// Floating-points shouldn't get compared directly,
// as rounding-errors can change the result.
// It is better to compare against a numerical range.
//
// http://www.maxinmontreal.com/forums/viewtopic.php?f=189&t=6465&start=30
// http://en.wikipedia.org/wiki/Binary_numeral_system
//
// Necessary for symbol-evaluation-engine and the validator.

static const double epsilon = 1E-06;

inline bool IsSmaller(double a, double b)
{
	return (a <= (b - epsilon));
}

inline bool IsGreater(double a, double b)
{
	return (a >= (b + epsilon));
}

inline bool IsEqual(double a, double b)
{
	return ((a > (b - epsilon)) && (a < (b + epsilon)));
}

inline bool IsSmallerOrEqual(double a, double b)
{
	return (a < (b + epsilon));
}

inline bool IsGreaterOrEqual(double a, double b)
{
	return (a > (b - epsilon));
}

#endif // #ifndef INC_FLOATINGPOINT_COMPARISIONS_H