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

#ifndef INC_CVIRTUALSYMBOLENGINE_H
#define INC_CVIRTUALSYMBOLENGINE_H

// OH-symbols are lower-case
// OpenPPL-symbols are upper-case
// To be used in EvaluateSymbol()
#define FAST_EXIT_ON_OPENPPL_SYMBOLS(name) if (isupper(name[0])) return false;

class CVirtualSymbolEngine {
 public:
	CVirtualSymbolEngine();
	virtual ~CVirtualSymbolEngine();
 public:
	virtual void ResetOnConnection();
	virtual void ResetOnHandreset();
	virtual void ResetOnNewRound();
	virtual void ResetOnMyTurn();
	virtual void ResetOnHeartbeat();
 public:
	virtual bool EvaluateSymbol(const char *name, double *result, bool log = false);
	// To build a list of identifiers for the editor
	virtual CString SymbolsProvided();
  // For the format-string to add symbols like.currentbet0..currentbet9
  CString RangeOfSymbols(CString format_string, int first, int last);
};

#endif INC_CVIRTUALSYMBOLENGINE_H


