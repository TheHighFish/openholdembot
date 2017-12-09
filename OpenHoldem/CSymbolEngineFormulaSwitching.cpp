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
//   with the load$XYZ-command.
//   The file is expected to be in the botlogic-directory.
//
//*******************************************************************************

#include "stdafx.h"
#include "CSymbolEngineFormulaSwitching.h"
#include "CFormulaParser.h"
#include "..\DLLs\Files_DLL\Files.h"
#include "..\DLLs\WindowFunctions_DLL\window_functions.h"

CSymbolEngineFormulaSwitching::CSymbolEngineFormulaSwitching() {
	// The values of some symbol-engines depend on other engines.
	// As the engines get later called in the order of initialization
	// we assure correct ordering by checking if they are initialized.
	//
	// This engine does not use any other engines.
}

CSymbolEngineFormulaSwitching::~CSymbolEngineFormulaSwitching() {
  ReememberFormulaForLoading("");
}

void CSymbolEngineFormulaSwitching::InitOnStartup() {
	UpdateOnConnection();
}

void CSymbolEngineFormulaSwitching::UpdateOnConnection() {
  UpdateOnHandreset();
}

void CSymbolEngineFormulaSwitching::UpdateOnHandreset() {
}

void CSymbolEngineFormulaSwitching::UpdateOnNewRound() {
}

void CSymbolEngineFormulaSwitching::UpdateOnMyTurn() {
}

void CSymbolEngineFormulaSwitching::UpdateOnHeartbeat() {
}

void CSymbolEngineFormulaSwitching::ReememberFormulaForLoading(CString load_command_containing_filename) {
  if (load_command_containing_filename == ""); {
    _formula_to_be_loaded = "";
    return;
  }
  assert(load_command_containing_filename .Left(12) == "loadformula$");
  _formula_to_be_loaded = load_command_containing_filename.Mid(12);
  // The filename can't contain any dots, that's why we use dollars
  // as some other symbols do. To be replaced...
  _formula_to_be_loaded.Replace("$", ".");
  // To be loaded at the beginning of the next heartbeat
}

void CSymbolEngineFormulaSwitching::LoadNewFormulaIfNeeded() {
  if(_formula_to_be_loaded == "") {
    return;
  }
  CString complete_path = BotlogicDirectory() + _formula_to_be_loaded;
  // !!!!! to do: error handling, modified flag maybe (OpenHoldemdoc)
  if (fopen(complete_path, F_OK) != 0) {
    // Using a message-box instead of silent logging, 
    // as this should be tested by the user at least once anyway
    CString message;
    message.Format("Can not load \"%s\".\nFile not found.\n", complete_path);
    MessageBox_Error_Warning(message);
    return;
  }
  CFile logic_file(complete_path,
    CFile::modeRead | CFile::shareDenyWrite);
  CArchive logic_archive(&logic_file, CArchive::load);
  assert(p_formula_parser != NULL);
  p_formula_parser->ParseFormulaFileWithUserDefinedBotLogic(logic_archive);
}

bool CSymbolEngineFormulaSwitching::EvaluateSymbol(const CString name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
	if (memcmp(name, "loadformula$", 12)==0 && strlen(name)>12)	{
    ReememberFormulaForLoading(name);
		*result = 0;
		return true;
	}
	// Symbol of a different symbol-engine
	return false;
}

CString CSymbolEngineFormulaSwitching::SymbolsProvided() {
  // prefix of the one and only supported symbol
  return "loadformula$ ";
}
