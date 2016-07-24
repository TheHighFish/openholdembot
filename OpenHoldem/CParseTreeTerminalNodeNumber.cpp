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

#include "stdafx.h" 
#include "CParseTreeTerminalNodeNumber.h"

#include <math.h>
#include "CAutoplayerTrace.h"
#include "CEngineContainer.h"
#include "CFunctionCollection.h"
#include "CParserSymbolTable.h"
#include "CPreferences.h"
#include "CSymbolEngineChipAmounts.h"
#include "CSymbolEngineMemorySymbols.h"
#include "CSymbolEngineOpenPPLUserVariables.h"
#include "CSymbolEngineTableLimits.h"
#include "FloatingPoint_Comparisions.h"
#include "NumericalFunctions.h"
#include "OH_MessageBox.h"
#include "StringFunctions.h"
#include "TokenizerConstants.h"

CParseTreeTerminalNodeNumber::CParseTreeTerminalNodeNumber(int relative_line_number) : 
    CParseTreeTerminalNode(relative_line_number) {
  _constant_value  = 0.0;
  _node_type = kTokenNumber;
}

CParseTreeTerminalNodeNumber::~CParseTreeTerminalNodeNumber() {
}

void CParseTreeTerminalNodeNumber::MakeConstant(double value) {
	_node_type = kTokenNumber;
	_constant_value = value;
}

double CParseTreeTerminalNodeNumber::Evaluate(bool log /* = false */){
  write_log(preferences.debug_formula(), 
    "[CParseTreeTerminalNode] Evaluating node type %i %s\n", 
		_node_type, TokenString(_node_type));
  p_autoplayer_trace->SetLastEvaluatedRelativeLineNumber(_relative_line_number);
	if (_node_type == kTokenNumber)	{
		write_log(preferences.debug_formula(), 
      "[CParseTreeTerminalNode] Number evaluates to %6.3f\n",
			_constant_value);
		return _constant_value;
	}
	// This must not happen for a terminal node
	assert(false);
	return kUndefined;
}

CString CParseTreeTerminalNodeNumber::EvaluateToString(bool log /* = false */) {
  double numerical_result = Evaluate(log);
  CString result;
  if (IsInteger(numerical_result) && EvaluatesToBinaryNumber()) {
    // Generqate binary representation;
    result.Format("%s", IntToBinaryString(int(numerical_result)));
  } else {
    // Generate floating-point representation
    // with 3 digits precision
    result.Format("%.3f", numerical_result);
  }
  return result;
}

CString CParseTreeTerminalNodeNumber::Serialize() {
  if (_node_type == kTokenNumber) {
    return Number2CString(_constant_value);
  } else {
    // Unhandled note-type, probably new and therefore not yet handled
    write_log(k_always_log_errors, "[CParseTreeTerminalNode] ERROR: Unhandled node-type %i in serialization of parse-tree\n",
      _node_type);
    return "";
  }
}


