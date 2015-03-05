//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: calculating the closest web/safe colour (216 values)
//   for a RGB-colour (16777216 values) to avoid problems with
//   fuzzyness when scraping colour-codes.
//   Also 216 colours are enough and makes it possible to introduce 
//   named colour constants.
//
//*******************************************************************************

#ifndef INC_WEBSAFECOLOURS_H
#define INC_WEBSAFECOLOURS_H

int ClosestWebSafeColour(const int argb_colour);

#endif // INC_WEBSAFECOLOURS_H