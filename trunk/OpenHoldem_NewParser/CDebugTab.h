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

#ifndef INC_CDEBUGTAB_H
#define INC_CDEBUGTAB_H

#include "COHScriptObject.h"
#include "CParseTreeNode.h"

class CDebugTab: public COHScriptObject{
 public:
  CDebugTab();
  ~CDebugTab();
 public:
  // f$debug is not a normaö function
  // Always returning zero.
  double Evaluate()   { return k_undefined_zero; }
  // To be used by the formula-editor
  CString EvaluateAll(); 
};

#endif INC_CDEBUGTAB_H

