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

#ifndef INC_CFORMULAPARSER_H
#define INC_CFORMULAPARSER_H

#define _AFXDLL 

#include "afx.h"
#include "atlstr.h"
#include "CFormulaFileSplitter.h"
#include "CParseTreeNode.h"
#include "CParseTreeRotator.h"
#include "CTokenizer.h"

class COHScriptList;

class CFormulaParser {
 public:
  CFormulaParser();
  ~CFormulaParser();
 public:
  void ParseFile(CArchive & formula_file);
  void ParseSingleFormula(CString function_text);
 public:
  static CString CurrentFunctionName();
  bool IsParsing()	{ return _is_parsing; }
 private:
  bool ExpectDoubleShebangAsStartOfFunctionHeader();
  bool ExpectDoubleShebangAsEndOfFunctionHeader();
  void ExpectMatchingBracketClose(int opening_bracket);
  bool ExpectConditionalThen();
  bool ExpectKeywordForce();
  void CheckForExtraTokensAfterEndOfFunction();
 private:
  // OH-script and OpenPPL
  void ParseListBody(COHScriptList *list);
  TPParseTreeNode ParseFunctionBody();
 private:
  // OH-script
  TPParseTreeNode ParseExpression();
  TPParseTreeNode ParseBracketExpression();
  TPParseTreeNode ParseUnaryExpression();
  TPParseTreeNode ParseSimpleExpression();
  void ParseConditionalPartialThenElseExpressions(
	  TPParseTreeNode *then_expression, TPParseTreeNode *else_expression);
 private:
  // OpenPPL
  TPParseTreeNode ParseOpenEndedWhenConditionSequence();
  TPParseTreeNode ParseOpenPPLAction();
  TPParseTreeNode ParseOpenPPLRaiseExpression();
 private:
  void BackPatchOpenEndedWhenCondition(
      TPParseTreeNode open_ended_when_condition);
  void BackPatchWhenCondition(TPParseTreeNode when_condition);
 private:
  CFormulaFileSplitter _formula_file_splitter;
  CTokenizer _tokenizer;
  CParseTreeRotator parse_tree_rotator;
 private:
  CString _token;
  bool _is_parsing;
private:
  // For backpatching pointers to next (open ended)when-condition
  TPParseTreeNode _last_open_ended_when_condition;
  TPParseTreeNode _last_when_condition;
  TPParseTreeNode _current__open_ended_when_condition;
};

extern CFormulaParser *p_formula_parser;

#endif INC_CFORMULAPARSER_H