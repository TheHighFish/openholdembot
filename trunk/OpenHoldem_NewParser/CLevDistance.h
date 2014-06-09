//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_CLEVDISTANCE_H
#define INC_CLEVDISTANCE_H
	
class CLevDistance
{
public:
	// public functions
	int LD (const char *s, const char *t);  // Calculates the Levenshtein Distance between the two input strings

private:
	// private functions and variables - not available via accessors or mutators
	int Minimum (int a, int b, int c);
	int *GetCellPointer (int *pOrigin, int col, int row, int nCols);
	int GetAt (int *pOrigin, int col, int row, int nCols);
	void PutAt (int *pOrigin, int col, int row, int nCols, int x);
}; 

#endif //INC_CLEVDISTANCE_H