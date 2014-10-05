//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Hash table for OH-script objects (functions and lists)
//
//******************************************************************************

#ifndef INC_CFUNCTIONCOLLECTION_H
#define INC_CFUNCTIONCOLLECTION_H

#include "..\CCritSec\CCritSec.h"
#include "COHScriptObject.h"
#include "CVirtualSymbolEngine.h"
#include <map>

// We make the collection a symbol-engine because of 
// * reset-functions 
// * Evaluate()-function,
// * SymbolsProvided() for syntax-highlighting
class CFunctionCollection: public CVirtualSymbolEngine {
  friend class CFormulaParser;
  friend class CAutoplayerFunctions;
 public:
  CFunctionCollection();
  ~CFunctionCollection();
 public:
  bool EvaluateSymbol(const char *name, double *result, bool log = false);
  double Evaluate(CString function_name, bool log = false);
  double EvaluateAutoplayerFunction(int function_code);
  bool EvaluatesToBinaryNumber(CString function_name);
 public: 
  CString SymbolsProvided();
 public:
  // Mandatory reset-functions
  void InitOnStartup();
  void ResetOnConnection();
  void ResetOnHandreset();
  void ResetOnNewRound();
  void ResetOnMyTurn();
  void ResetOnHeartbeat();
 public:
  // To be called by
  //   * ResetOnHeartbeat() 
  //   * the formula editor for the working copy (f$debug and f$test)
  void ClearCache();
 public:
  // Deletes all user-defined functions
  // Keeps OpenPPL-library if correctly parsed.
  void DeleteAll();
  void SetEmptyDefaultBot();
 public:
  void Add(COHScriptObject *new_function);
  COHScriptObject *LookUp(CString name);
  bool Rename(CString from_name, CString to_name);
  void Delete(CString name);
  void SetFunctionText(CString name, CString content);
 public:
  bool Exists(CString name);
  // Generates smart error-messages on failure
  // To be used by the parser
  void VerifyExistence(CString name);
 public:
  void Save(CArchive &ar);
  void SaveObject(CArchive &ar, COHScriptObject *function_or_list);
  // For debugging
  void Dump();
 public:
  bool ParseAll();
  void SetOpenPPLLibraryLoadingState(bool is_good) { _openPPL_library_correctly_loaded = is_good; }
  void CheckForDefaultFormulaEntries();
 public:
  bool BotLogicCorrectlyParsed();
  bool OpenPPLLibraryCorrectlyParsed()             { return _openPPL_library_correctly_loaded; }
  bool IsOpenPPLProfile();
 public:
  // Simply call GetFirst() first, then GetNext()
  // Once you receive NULL the end has been reached
  COHScriptObject *GetFirst();
  COHScriptObject *GetNext();
 public:
  CString Title()           { return _title; }
  CString FormulaPath()     { return _path; }
  CString DLLPath();
 protected:
  void SetTitle(CString title)	{ _title = title; }
  void SetPath(CString path)	  { _path = path; }
  // For OpenPPL, which evaluates f$preflop, ...
  // instead of f$beep, f$alli, ...
  void SetAutoplayerFunctionValue(int function_code, double value);
 private:
  void CreateEmptyDefaultFunctionIfFunctionDoesNotExist(CString &function_name);
  CString GetSimilarNameWithDifferentCases(CString function_name);
  bool IsOutdatedFunction(CString name);
 private:
  std::map<CString, COHScriptObject*> _function_map;
  CString _title;
  CString _path;
  bool _openPPL_library_correctly_loaded;
 private:
  std::map<CString, COHScriptObject*>::iterator enumerator_it;
  CCritSec m_critsec;
};

extern CFunctionCollection *p_function_collection;

#endif INC_CFUNCTIONCOLLECTION_H