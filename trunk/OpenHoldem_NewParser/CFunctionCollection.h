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
#include <map>

class CFunctionCollection{
  friend class CFormulaParser;
 public:
  CFunctionCollection();
  ~CFunctionCollection();
 public:
  double Evaluate(CString function_name);
 public:
  // To be called by
  //   * ResetOnHeartbeat() !!
  //   * the formula editor for the working copy (f$debug and f$test)
  void ClearCache();
  void DeleteAll();
  void SetEmptyDefaultBot();
 public:
  void Add(COHScriptObject *new_function);
  bool Exists(CString name);
  COHScriptObject *LookUp(CString name);
 public:
  void Save(CArchive &ar);
  void SaveObject(CArchive &ar, COHScriptObject *function_or_list);
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
  void AddEmptyFunctionIfFunctionDoesNotExist(CString &function_name);
 private:
  std::map<CString, COHScriptObject*> _function_map;
  CString _title;
  CString _path;
 private:
  std::map<CString, COHScriptObject*>::iterator enumerator_it;
};

extern CFunctionCollection *p_function_collection;

#endif INC_CFUNCTIONCOLLECTION_H