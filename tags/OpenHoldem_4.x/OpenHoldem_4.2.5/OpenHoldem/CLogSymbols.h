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

#ifndef INC_CLOGSYMBOLS_H
#define INC_CLOGSYMBOLS_H
	
class CLogSymbols
{
public:
	CLogSymbols();
	~CLogSymbols();
	double	ProcessQuery(const char * pquery, int *e);
};


#endif //INC_CLOGSYMBOLS_H