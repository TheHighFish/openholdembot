#pragma once
//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: switching formulas at runtime
//   at the beginning of a heartbeat
//   with the load$XYZ-command-
//   The file is expected to be in the botlogic-directory.
//
//*******************************************************************************

#include "CVirtualSymbolEngine.h"

class CSymbolEngineFormulaSwitching : public CVirtualSymbolEngine {
 public:
	CSymbolEngineFormulaSwitching();
	~CSymbolEngineFormulaSwitching();
 public:
	// Mandatory reset-functions
	void InitOnStartup();
	void UpdateOnConnection();
	void UpdateOnHandreset();
	void UpdateOnNewRound();
	void UpdateOnMyTurn();
	void UpdateOnHeartbeat();
 public:
	// Public accessors
	bool EvaluateSymbol(const CString name, double *result, bool log = false);
	CString SymbolsProvided();;
 private:
  void ReememberFormulaForLoading(CString load_command_containing_filename);
  void LoadNewFormulaIfNeeded();
 private:
  CString _formula_to_be_loaded;
};