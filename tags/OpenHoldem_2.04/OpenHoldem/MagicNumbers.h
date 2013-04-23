#ifndef _INC_MAGICNUMBERS_H

#define _INC_MAGICNUMBERS_H


// Used for bit-calculations: 2^N
const int k_exponents[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

// Betting rounds
const int k_number_of_betrounds = 4;
const int k_betround_preflop	= 1;
const int k_betround_flop		= 2;
const int k_betround_turn		= 3;
const int k_betround_river		= 4;


#endif // _INC_MAGICNUMBERS_H