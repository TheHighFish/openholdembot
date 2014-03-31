#include "stdafx.h"
#include "NumericalFunctions.h"

int	bitcount(int bit_vector) 
{ 
	// returns count of number of bits set in the given value
	unsigned int uCount = 0;

	uCount = bit_vector 
		- ((bit_vector >> 1) & 033333333333) 
		- ((bit_vector >> 2) & 011111111111);

	return ((uCount + (uCount >> 3)) & 030707070707) % 63;
}