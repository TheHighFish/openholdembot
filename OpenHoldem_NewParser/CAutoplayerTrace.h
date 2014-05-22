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

#ifndef INC_CAUTOPLAYER_TRACE_H
#define INC_CAUTOPLAYER_TRACE_H

class CAutoplayerTrace {
 public:
   CAutoplayerTrace();
   ~CAutoplayerTrace();
 public:
  void Clear();
  void Add(CString symbol, double value);
  // Logging function, value currently unknown
  // Returning index (line number)
  int  Add(CString symbol); 
  void BackPatchValue(int index, double value);  
  void Indent(bool more);
  void Print(const char *action_taken);
 public:
  CString LogSymbolsForGUI();
 public:
#define ENT CSLock lock(m_critsec);
  void logsymbols_collection_add(const char *s) 	{ ENT _logsymbols_collection.Add(s);}
  void logsymbols_collection_removeall() 			{ ENT _logsymbols_collection.RemoveAll();}
  void symboltrace_collection_add(const char *s)	{ ENT _logsymbols_collection.Add(s);}
  void symboltrace_collection_removeall();
#undef ENT
 private:
  void LogLogSymbols();
  void LogBasicInfo(const char *action_taken);
  void LogAutoPlayerTrace();	
 private:
  CString Indentation();
 private:
  //!!!const CArray <CString, CString> *logsymbols_collection() { return &_logsymbols_collection; }
  //const CArray <CString, CString> *symboltrace_collection() { return &_symboltrace_collection; }
  // private variables - use public accessors and public mutators to address these
  CArray <CString, CString>   _logsymbols_collection; // Used to track the log$ symbols
  CArray <CString, CString>   _symboltrace_collection;// Used to trace function execution
  int _indentation;
  int _number_of_log_lines;
  CCritSec m_critsec;
};

extern CAutoplayerTrace *p_autoplayer_trace;

#endif INC_CAUTOPLAYER_TRACE_H