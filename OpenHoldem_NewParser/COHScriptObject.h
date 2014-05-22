//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose: virtual base-class for CFunction and COHList
//
//***************************************************************************** 

#ifndef INC_COHSCRIPTOBJECT_H
#define INC_COHSCRIPTOBJECT_H

class COHScriptObject {
 public:
  COHScriptObject(); 
  COHScriptObject(CString *new_name, CString *new_function_text);
  ~COHScriptObject();
 public:
  CString name()			      { return _name; }
  virtual CString function_text() { return _function_text; }
  void SetName(CString name)      { _name = name; }
 public:
  virtual double Evaluate(bool log = false);
 public:
  bool IsList()                 { return _name.Left(4) == "list"; }
  bool IsFunction()             { return _name.Left(2) == "f$"; }
  // Autoplayer, Secondary, Ini, PrWin
  bool IsStandardFunction();
  // f$debug, f$test, notes, DLL
  bool IsSpecialFunction();
  bool IsUserDefinedFunction()  { return (IsFunction() 
                                    && !IsStandardFunction()
                                    && !IsSpecialFunction()); }
 public:
  // For saving
  CString Serialize()         { return "##" + _name + "##\r\n" 
                                  + _function_text + "\r\n"; }
 public:
  void Parse();
 protected:
  CString _name;
  CString _function_text;
};

#endif INC_COHSCRIPTOBJECT_H