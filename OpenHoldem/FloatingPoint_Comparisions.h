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
// Purpose: Floating-points shouldn't get compared directly,
//   as rounding-errors can change the result.
//   It is better to compare against a numerical range.
//
//   http://www.maxinmontreal.com/forums/viewtopic.php?f=189&t=6465&start=30
//   http://en.wikipedia.org/wiki/Binary_numeral_system
//
//   Necessary for symbol-evaluation-engine and the validator.
//
//******************************************************************************

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

// Numbers very close to zero can cause problems,
// so we create functions for that.
inline bool IsTrue(double d)
{
	return ((d >= 0.5) || (d <= -0.5));
}

inline bool IsFalse(double d)
{
	return (!IsTrue(d));
}

inline int Round(double d)
{
	// Rounding to nearest integer
	return int(d + 0.5);
}

inline int IsApproximatellyEqual(double d1, double d2)
{
	// True, if after rounding both are equal
	return (Round(d1) == Round(d2));
}
