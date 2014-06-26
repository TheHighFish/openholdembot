//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_COHSCRIPTLIST_H
#define INC_COHSCRIPTLIST_H

#include "COHScriptObject.h"

typedef bool THandListMatrix[k_number_of_ranks_per_deck][k_number_of_ranks_per_deck];
//   2 3 4 5 6 7 8 9 T J Q K A
// 2 P
// 3   A
// 4     I     SUITED  
// 5       R
// 6         S
// 7           P
// 8             A
// 9               I
// T                 R
// J                   S 
// Q    OFFSUITED        P
// K                       A   
// A                         I

class COHScriptList: public COHScriptObject {
 public:
  COHScriptList( 
    CString *new_name, 
    CString *new_function_text,
    int absolute_line);
  ~COHScriptList();
 public:
  void Clear();
  bool Set(CString list_member);
  // Public for CSymbolEngineVersus
  bool IsOnList(int first_rank, int second_rank, bool suited);
  // Actually Evaluate() returns true/false if our hand is in the list or not
  // but the function is inheritzed from the base-class.
  double Evaluate(bool log = false);
 private:
  void ErrorInvalidMember(CString list_member); 
  void ErrorOldStyleFormat(CString list_member);
 private:
  void Set(int first_rank, int second_rank, bool suited);
 private:
  THandListMatrix _handlist_matrix;
};

#endif INC_COHSCRIPTLIST_H