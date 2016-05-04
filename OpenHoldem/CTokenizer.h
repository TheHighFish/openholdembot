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

#ifndef INC_CTOKENIZER_H
#define INC_CTOKENIZER_H

#include "atlstr.h"

class CTokenizer {	
  friend class CFormulaParser;
 public:
	CTokenizer();
	~CTokenizer();
 public:
	void SetInput(const char* next_formula_to_be_parsed);
	int GetToken();
	int LookAhead(bool expect_action = false);
	char* GetTokenString();
 public:
	void PushBack()	{ _last_token_pushed_back = true; }
  // Very, very ugly way to handle OpenPPL-potsized-actions.
  // They look like modulo or percentage expressions,
  // but lack a 2nd operand and have "Force" instead.
  // When ... RaiseBy 60% Force
  // These statements can't get handled with a one-token-lookahead
  // (good parse-technology), but require 2 tokens lookahead
  // and potentially 2 tokens push-back.
  // Cross your fingers that it works. ;-)
  void PushBackAdditionalPercentageOperator() { _additional_percentage_operator_pushed_back = true; }
 public:
	static int LineAbsolute();
	static int LineRelative();
	static char* RemainingInput();
 public:
  void InitNewParse();
 private:
	int  ScanForNextToken();
	void SkipToEndOfLine();
	void SkipToEndOfMultiLineComment();
 private:
	bool IsBinaryMinus();
	bool IsTokenOpenPPLKeyword();
 private:
  void CheckTokenForOpenPPLAction(int *token);
private:
	void InitVars();
	char CURRENT_CHARACTER();
 private:
	int  _token_end_pointer;
	bool _last_token_pushed_back;
	int  _last_token;
  bool _additional_percentage_operator_pushed_back;
 private:
	// To distinguish modulo % and percentage % operators
	bool _inside_OpenPPL_function;
	// Gets set as side-effect by IsTokenOpenPPLKeyword()
	int  _OpenPPL_token_ID;
};

#endif INC_CTOKENIZER_H