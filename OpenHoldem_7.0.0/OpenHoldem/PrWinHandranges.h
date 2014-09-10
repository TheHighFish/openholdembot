//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Determining hand-ranges for weighted PrWin
//
//******************************************************************************

#ifndef INC_PRWINHANDRANGES_H
#define INC_PRWINHANDRANGES_H

bool IsHandInWeightedRange(const int card1, const int card2, 
							const int willplay, const int wontplay, 
							const int topclip, const int mustplay);
	/*
	p |---|	---------
	r |   |   /      \
	o |   |  /        \
	b |   | /          \
	a |   |/            \
	b |                  \
	l -----------------------------------------------------
	e	 1	2	   3	 4	 handrank169 ->
	1=mustplay
	2=topclip
	3=willplay
	4=wontplay
	(with the current statistics available in OH only willplay and wontplay should be used.
	 mustplay and topclip are debatable constructs at the best of times 2008-01-29)
	*/

#endif