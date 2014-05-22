//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose: Hash table for OH-script objects (functions and lists)
//
//***************************************************************************** 

#ifndef INC_CFUNCTIONCOLLECTION_H
#define INC_CFUNCTIONCOLLECTION_H

#include "COHScriptObject.h"
#include "CVirtualSymbolEngine.h"
#include <map>

// We make the collection a symbol-engine
// because of reset-functions and Evaluate()-function.
class CFunctionCollection: public CVirtualSymbolEngine {
  friend class CFormulaParser;
 public:
  CFunctionCollection();
  ~CFunctionCollection();
 public:
  bool EvaluateSymbol(const char *name, double *result, bool log = false);
  double Evaluate(CString function_name);
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
  void DeleteAll();
  void SetEmptyDefaultBot();
 public:
  void Add(COHScriptObject *new_function);
  bool Exists(CString name);
  COHScriptObject *LookUp(CString name);
  bool Rename(CString from_name, CString to_name);
  void Delete(CString name);
 public:
  void Save(CArchive &ar);
  void SaveObject(CArchive &ar, COHScriptObject *function_or_list);
 public:
  bool ParseAll();
  bool CorrectlyParsed();
 public:
  // Simply call GetFirst() first, then GetNext()
  // Once you receive NULL the end has been reached
  COHScriptObject *GetFirst();
  COHScriptObject *GetNext();
 public:
  CString Title()				{ return _title; }
  CString FormulaPath()			{ return _path; }
  CString DLLPath();
 protected:
  void SetTitle(CString title)	{ _title = title; }
  void SetPath(CString path)	{ _path = path; }
 private:
  void CheckForDefaultFormulaEntries();
  void CreateEmptyDefaultFunctionIfFunctionDoesNotExist(CString &function_name);
 private:
  std::map<CString, COHScriptObject*> _function_map;
  CString _title;
  CString _path;
 private:
  std::map<CString, COHScriptObject*>::iterator enumerator_it;
};

extern CFunctionCollection *p_function_collection;

#endif INC_CFUNCTIONCOLLECTION_H