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


#ifndef INC_NUMERICALFUNCTIONS_H
#define INC_NUMERICALFUNCTIONS_H

#include <assert.h>

inline void SwapDoubles(double *first, double *second)
{
	double temp = *first;
	*first = *second;
	*second = temp;
}

inline void SwapInts(int *first, int *second)
{
	int temp = *first;
	*first = *second;
	*second = temp;
}

inline bool IsInteger(double k) 
{ 
	return (double(int(k)) == k); 
}

inline int RankOfCard(int card)
{
	// Suit is stored in bits 0..3
	// Rank is stored in bits 4..7
	return ((card >>4) & 0x0F);
}

inline void AssertRange(double value, double lower_bound, double upper_bound)
{
	assert(value >= lower_bound);
	assert(value <= upper_bound);
}

inline void SetBit(int &bit_vector, int n_th_bit)
{
	bit_vector |= (1 << n_th_bit);
}

inline bool IsBitSet(int bit_vector, int n_th_bit)
{
	return ((bit_vector >> n_th_bit) & 1);
}

int	bitcount(unsigned bit_vector);

inline void AdaptValueToMinMaxRange(double *value, double min, double max)
{
	if (*value < min)
	{
		*value = min;
	}
	else if (*value > max)
	{
		*value = max;
	}
}

// returns a random number in the range 0..n-1
int Random(int n);

#endif INC_NUMERICALFUNCTIONS_H