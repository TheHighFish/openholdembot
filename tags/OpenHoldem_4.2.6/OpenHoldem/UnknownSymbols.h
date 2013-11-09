//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose: Warning about unknown (erroneaous) and outdated symbols
//   Is not able to care about wrong function names; this has to be handled
//   by CFormula::WarnAboutOutdatedConcepts().
//
//***************************************************************************** 

#ifndef INC_UNKNOWNSYMBOLS_H
#define INC_UNKNOWNSYMBOLS_H

void WarnAboutUnknownOrOutdatedSymbol(CString symbol);

#endif // NC_UNKNOWNSYMBOLS_H