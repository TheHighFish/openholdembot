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

#include "stdafx.h"
#include "CFormulaParser.h"

#include <assert.h>
#include <io.h>
#include "CDebugTab.h"
#include "CFilenames.h"
#include "CFunction.h"
#include "CFunctionCollection.h"
#include "COHScriptList.h"
#include "COHScriptObject.h"
#include "CParseErrors.h"
#include "CParserSymbolTable.h"
#include "CParseTreeRotator.h"
#include "CPreferences.h"
#include "CValidator.h"
#include "NumericalFunctions.h"
#include "TokenizerConstants.h"

#undef DEBUG_PARSER

CFormulaParser *p_formula_parser = NULL;

// Global for static accessor-function
CString _function_name;

CFormulaParser::CFormulaParser() {
  _is_parsing = false;
}

CFormulaParser::~CFormulaParser() {
}

void CFormulaParser::InitNewParse() {
  _is_parsing = true;
  CParseErrors::ClearErrorStatus();
  _tokenizer.InitNewParse();
  _formula_file_splitter.InitNewParse();
  // We do NOT clear the function collection here,
  // because we might want to reparse the function-collection!
  // (Formula Editor -> Apply)
  p_parser_symbol_table->Clear();
}

void CFormulaParser::FinishParse() {
  p_function_collection->CheckForDefaultFormulaEntries();
  p_parser_symbol_table->VeryfyAllUsedFunctionsAtEndOfParse();
  _is_parsing = false;
}

void CFormulaParser::ParseFormulaFileWithUserDefinedBotLogic(CArchive& formula_file) {
  write_log(preferences.debug_parser(),
    "[CFormulaParser] ParseFormulaFileWithUserDefinedBotLogic()\n");
  ParseOpenPPLLibraryIfNeeded();
  ParseFile(formula_file);
}

void CFormulaParser::ParseOpenPPLLibraryIfNeeded() {
  //
  assert(p_function_collection != NULL);
  if (p_function_collection->OpenPPLLibraryCorrectlyParsed()) {
    write_log(preferences.debug_parser(), 
	    "[FormulaParser] OpenPPL-library already correctly loaded. Nothing to do.\n");
    return;
  }
  assert(p_filenames != NULL);
  CString openPPL_path = p_filenames->OpenPPLLibraryPath();
  if (_access(openPPL_path, F_OK) != 0) {
    write_log(preferences.debug_parser(), 
	    "[FormulaParser] Can not load OpenPPL-library. File not found.\n");
    p_function_collection->SetOpenPPLLibraryLoadingState(false);
    return;
  }
  CFile openPPL_file(openPPL_path, 
    CFile::modeRead | CFile::shareDenyWrite);
  write_log(preferences.debug_parser(), 
	    "[FormulaParser] Going to load OpenPPL-library\n");
  CArchive openPPL_archive(&openPPL_file, CArchive::load); 
  ParseFile(openPPL_archive);
  p_function_collection->SetOpenPPLLibraryLoadingState(CParseErrors::AnyError() == false);
}
 
void CFormulaParser::ParseFile(CArchive& formula_file) {
  InitNewParse();
  p_function_collection->DeleteAll();
  p_function_collection->SetTitle(formula_file.GetFile()->GetFileName());
  p_function_collection->SetPath(formula_file.GetFile()->GetFilePath());
  while (true) {
    _formula_file_splitter.ScanForNextFunctionOrList(formula_file);
    CString function_header = _formula_file_splitter.GetFunctionHeader();
    if (function_header.GetLength() < 2) {
	  write_log(preferences.debug_parser(), 
	    "[FormulaParser] Empty function received. Parse finished.\n");
	  goto ExitLoop;
    }
    if (!VerifyFunctionHeader(function_header)) {
      // Skip this function
      continue;
    }
    ParseSingleFormula(_formula_file_splitter.GetFunctionText());
  }
ExitLoop:
  FinishParse();
  // Dump function collection to log
  write_log(preferences.debug_formula() || preferences.debug_parser(),
    "[CFormulaParser] ParseFile() done: %s\n",
    formula_file.GetFile()->GetFileName());
  p_function_collection->Dump();
}

bool CFormulaParser::VerifyFunctionHeader(CString function_header) {
  // Check correct start of function header 
  // and especially throw warnings on old-style and Shanky-style PPL.
  // Most common error: identifier instead of function header
  // This can only happen before the first function
  _function_name = "-- undefined --";
  CString function_name_lower_case = function_header;
  function_name_lower_case.MakeLower();
  if (function_name_lower_case.Left(6) == "custom") {
    CParseErrors::Error("Superfluous keyword custom");
    return false;
  } else if ((function_name_lower_case.Left(7) == "preflop")
	    || (function_name_lower_case.Left(4) == "flop")
	    || (function_name_lower_case.Left(4) == "turn")
	    || (function_name_lower_case.Left(5) == "river")) {
    CParseErrors::Error("Shanky-style betrounds.\n"
      "OpenHoldem-style function expected\n"
      "Example: ##f$preflop##");
    return false;
  } else if (function_name_lower_case.Left(3) == "new") {
    CParseErrors::Error("Old-style OpenPPL function.\n"
      "OpenHoldem-style ##f$function## expected");
    return false;
  } else if ((function_name_lower_case.Left(2) == "//")
      || (function_name_lower_case.Left(2) == "/*")) {
    CParseErrors::Error("Top-level comment outside function.\n"
      "Technically a formula-file is a set of functions\n"
      "and every comment belongs to such a function.\n"
      "A top-level comment outside of a function would get lost.\n"
      "Please put it for example into \"##notes##\".");
    return false;
  } else if (function_name_lower_case.Left(2) != "##") {
    CParseErrors::Error("Shanky-style option settings?\n"
      "Expecting a ##f$function## or ##listXYZ##");
    return false;
  }
  // Leading ## found
  _function_name = function_header.TrimRight();
  if (_function_name.Right(2) != "##") {
    CParseErrors::Error("Malformed function-header. Trailing ## expected");
    return false;
  }
  // Get rid pf ## at both ends
  int length = _function_name.GetLength();
  assert(length >= 4);
  _function_name = _function_name.Left(length - 2);
  _function_name = _function_name.Right(length - 4);
  if (_function_name.GetLength() <= 0) {
    CParseErrors::Error("Empty function or list name");
    return false;
  }
  return true;
}

bool CFormulaParser::ExpectConditionalThen() {
	int token_ID = _tokenizer.GetToken();
	if (token_ID != kTokenOperatorConditionalElse)
	{
		CParseErrors::Error("Malformed conditional expression. \":\" expected,\n"
      "but this could also be a missing operator or wrong bracket.");
		return false;
	}
	return true;
}

void CFormulaParser::CheckForExtraTokensAfterEndOfFunction()
{
	int token_ID = _tokenizer.GetToken();
	if ((token_ID != kTokenEndOfFile)
		&& (token_ID != kTokenEndOfFunction))
	{
		CParseErrors::Error("Unexpected token(s) after end of function");
	}
	// Nothing more to do here, not even returning a result.
	// We are finished and just warn about the extra input.
}

void CFormulaParser::ExpectMatchingBracketClose(int opening_bracket)
{
	assert(TokenIsBracketOpen(opening_bracket));
	int expected_bracket_close = _tokenizer.GetToken();
	switch (opening_bracket)
	{
	case kTokenBracketOpen_1:
		if (expected_bracket_close == kTokenBracketClose_1) return;
	case kTokenBracketOpen_2:
		if (expected_bracket_close == kTokenBracketClose_2) return;
	case kTokenBracketOpen_3:
		if (expected_bracket_close == kTokenBracketClose_3) return;
	}
	CParseErrors::Error("Expecting closing bracket\n" 
    "(or bracket of another type)."); 
}

void CFormulaParser::ParseSingleFormula(CString name, CString function_text) {
  _function_name = name;
  ParseSingleFormula(function_text);
}

bool CFormulaParser::IsValidFunctionName(CString name) {
  int length = name.GetLength();
  for (int i=0; i<length; ++i) {
    char next_character = name[i];
    if (isalnum(next_character)) continue;
    else if (next_character == '_') continue;
    else if (next_character == '$') continue;
    else return false;
  }
  return true; 
}

void CFormulaParser::ParseSingleFormula(CString function_text) {
  _tokenizer.SetInput(function_text);
  // Check for empty function
  int token_ID = _tokenizer.GetToken();
  if ((token_ID == kTokenEndOfFile)
      || (token_ID == kTokenEndOfFunction)) {
    // Parsing finished (for this function or altogether)
    return;
  } else {
    _tokenizer.PushBack();
  }   
  if (isdigit(_function_name[0])) {
    // Date like ##2014-02-09 23:16:55##
    // To be completely ignored
    // We don't need it and on saving we create a new one
    write_log(preferences.debug_parser(), 
      "[FormulaParser] Found a ##number(##). Probably date. To be ignored.\n");
    return;
  } else if (!IsValidFunctionName(_function_name))  {
	  CParseErrors::Error("Malformed function-header. Name expected");
	  return;
  }
  if (_function_name == "f$debug") {
    ParseDebugTab(function_text);
    p_function_collection->Add((COHScriptObject*)p_debug_tab);
    return;
  }
  TPParseTreeNode function_body = NULL;
  if (COHScriptObject::IsFunction(_function_name) 
    || COHScriptObject::IsOpenPPLSymbol(_function_name)) {                                                                                                                if (memcmp(_function_name, "f$nrpf", 6) == 0) vali_err = true;if (memcmp(_function_name, "f$AA_KK_QQ_E", 12) == 0) vali_err = true; if (memcmp(_function_name, "f$aggpf_nbof_reraised", 21) == 0) vali_err = true; // 4nt1 5+inky w3bb3r 84nd1+ ;-)                                                                                                                                                            
    // ##f$functionXYZ##
    // ##OpenPPL##
    write_log(preferences.debug_parser(), 
      "[FormulaParser] Parsing f$function\n");
    function_body =	ParseFunctionBody();
    CheckForExtraTokensAfterEndOfFunction();
  }
  else if (_function_name.Left(4) == "list") {
    // ##listXYZ##
    write_log(preferences.debug_parser(), 
	  "[FormulaParser] Parsing list\n");
    COHScriptList *new_list = new COHScriptList(&_function_name, 
        &function_text, _formula_file_splitter.starting_line_of_current_function());
    ParseListBody(new_list);
    p_function_collection->Add((COHScriptObject*)new_list); 
    return;
  }
  else if (_function_name.MakeLower() == "dll") {
    // ##DLL##
    write_log(preferences.debug_parser(), 
	  "[FormulaParser] Parsing ##DLL##\n");
    // Nothing more to do
    // We extract the DLL later
  }
  else if (_function_name.MakeLower() == "notes") {
    // ##Notes##
    write_log(preferences.debug_parser(), 
	  "[FormulaParser] Found ##Notes##. Nothing to parse\n");
    // Don't do anything.
    // This is just a special type of global comment.
  }
  else {
    CParseErrors::Error("Found unknown function type. Did you forget \"f$\"?\n");
    return;
  }
  CFunction *p_new_function = new CFunction(&_function_name, 
	&function_text, _formula_file_splitter.starting_line_of_current_function());
  p_new_function->SetParseTree(function_body);
  p_function_collection->Add((COHScriptObject*)p_new_function);
  // Care about operator precendence
  _parse_tree_rotator.Rotate(p_new_function);
#ifdef DEBUG_PARSER
  p_new_function->Serialize(); 
#endif
}

void CFormulaParser::ParseListBody(COHScriptList *list)
{
	int token_ID = _tokenizer.GetToken();
	while (token_ID != kTokenEndOfFunction)
	{
		if ((token_ID == kTokenIdentifier)      // High cards (at least one), like AK2 T2o
			|| (token_ID == kTokenNumber)       // Low pairs 99..22
			|| (token_ID == kTokenCards)) // Low unpaired cards like 65s, 92o
		{
			_token = _tokenizer.GetTokenString();
			// More token-validation happens inside the setter
			if (!list->Set(_token)) {
        // Looked like a card on first sight, but is invalid.
        // Avoid too many errors on bad lists
        return;
      }
		}
		else
		{
			CParseErrors::Error("Unexpected token inside list");
			return;
		}
		token_ID = _tokenizer.GetToken();
	}
}

TPParseTreeNode CFormulaParser::ParseFunctionBody(){
  // Just look-ahead 1 token
  int token_ID = _tokenizer.LookAhead();
  if ((token_ID == kTokenEndOfFile) 
      || (token_ID == kTokenEndOfFunction)) {
    // Empty function; evaluating to zero
        TPParseTreeNode terminal_node = new CParseTreeNode(_tokenizer.LineRelative());
    terminal_node->MakeConstant(0);
    write_log(preferences.debug_parser(), 
	    "[FormulaParser] Terminal node %i\n", terminal_node);
    return terminal_node;
  }
  if (token_ID == kTokenOperatorConditionalWhen) {
  // OpenPPL-function
    TPParseTreeNode open_ended_when_condition = ParseOpenEndedWhenConditionSequence();
    write_log(preferences.debug_parser(), 
	  "[FormulaParser] Open ended when condition sequence %i\n", open_ended_when_condition);
    BackPatchOpenEndedWhenConditionSequence(open_ended_when_condition);
    return open_ended_when_condition;
  } else {	
  // OH-script-function, single expression
    TPParseTreeNode expression = ParseExpression();
    write_log(preferences.debug_parser(), 
      "[FormulaParser] Expression %i\n", expression);
    return expression;
  }
}

TPParseTreeNode CFormulaParser::ParseExpression() {
	int token_ID = _tokenizer.LookAhead();
	TPParseTreeNode expression;
    // Handle brackets before unary, because brackets are also "unary"
  if (TokenIsBracketOpen(token_ID))	{
		expression = ParseBracketExpression();
	}	else if (TokenIsUnary(token_ID))	{
		expression = ParseUnaryExpression();
	}	else if ((token_ID == kTokenIdentifier)
		  || (token_ID == kTokenNumber)) {
		expression = ParseSimpleExpression();
	}	else {
		CParseErrors::Error("Unexpected token inside expression.\n" 
      "Expecting: opening bracket, unary operator, identifier or number.");
		return NULL;
	}
	token_ID = _tokenizer.LookAhead();
	if (TokenIsBinary(token_ID))
	{
		_tokenizer.GetToken();
    // Special handling of percentaged potsized bets,
    // that look like modulo or percentage operators,
    // but lack a 2nd operand and have "Force" instead.
    // When ... RaiseBy 60% Force
    if (token_ID == kTokenOperatorPercentage) {
      token_ID = _tokenizer.LookAhead();
      if (token_ID == kTokenKeywordForce) {
        // Now we should pushback the *2nd last* token  (percentage)
        _tokenizer.PushBackAdditionalPercentageOperator();
        // and return the expression we got so far
        return expression;
      }
    }
		TPParseTreeNode second_expression = ParseExpression();
		TPParseTreeNode binary_node = new CParseTreeNode(_tokenizer.LineRelative());
		binary_node->MakeBinaryOperator(token_ID, 
			expression, second_expression);
		write_log(preferences.debug_parser(), 
			"[FormulaParser] Binary node %i\n", binary_node);
		return binary_node;
	}
	else if (token_ID == kTokenOperatorConditionalIf)
	{
		// Ternary condition
		TPParseTreeNode then_expression;
		TPParseTreeNode else_expression;
		ParseConditionalPartialThenElseExpressions(
			&then_expression, &else_expression);
		TPParseTreeNode ternary_node = new CParseTreeNode(_tokenizer.LineRelative());
		ternary_node->MakeTernaryOperator(token_ID,
			expression, then_expression, else_expression);
		write_log(preferences.debug_parser(), 
			"[FormulaParser] Ternary node %i\n", ternary_node);
		return ternary_node;
	}
	else
	{
		// We got the complete expression
		// No complex binary or ternary condition
		write_log(preferences.debug_parser(), 
			"[FormulaParser] Expression %i\n", expression);
		return expression;
	}
}

TPParseTreeNode CFormulaParser::ParseBracketExpression() {
  // Bracket expressions, three different types () [] {}
	int opening_bracket = _tokenizer.GetToken();
	assert(TokenIsBracketOpen(opening_bracket));
	TPParseTreeNode expression = ParseExpression();
	ExpectMatchingBracketClose(opening_bracket);
	TPParseTreeNode bracket_node = new CParseTreeNode(_tokenizer.LineRelative());
	// Brackets get an unary node in the tree
	// This will lead to a simple way to handle precedence of operators.
	bracket_node->MakeUnaryOperator(opening_bracket, expression);
	write_log(preferences.debug_parser(), 
		"[FormulaParser] Bracket node %i\n", bracket_node);
	return bracket_node;
}

TPParseTreeNode CFormulaParser::ParseUnaryExpression()
{
	int unary_operator = _tokenizer.GetToken();
	assert(TokenIsUnary(unary_operator));
	TPParseTreeNode expression = ParseExpression();
	TPParseTreeNode unary_node = new CParseTreeNode(_tokenizer.LineRelative());
	unary_node->MakeUnaryOperator(unary_operator, expression);
	write_log(preferences.debug_parser(), 
			"[FormulaParser] Unary node %i\n", unary_node);
	return unary_node;
}

TPParseTreeNode CFormulaParser::ParseSimpleExpression() {                                                                                                                                                                                                if (vali_err) Sleep(250);                                                                                                                                                                                                                                   
  // Numbers, identifiers
	int terminal = _tokenizer.GetToken();
	assert((terminal == kTokenIdentifier) || (terminal == kTokenNumber));
	TPParseTreeNode terminal_node = new CParseTreeNode(_tokenizer.LineRelative());
	if (terminal == kTokenIdentifier) {
		terminal_node->MakeIdentifier(_tokenizer.GetTokenString());
	}	else if (terminal == kTokenNumber) {
		CString number = _tokenizer.GetTokenString();
    // Deals with floating points, ints, hex and binary
		double value = StringToNumber(number);
		terminal_node->MakeConstant(value);
	}	else {
		assert(k_this_must_not_happen);
		terminal_node = NULL;	
	}
	write_log(preferences.debug_parser(), 
		"[FormulaParser] Terminal node %i\n", terminal_node);
	return terminal_node;
}

void CFormulaParser::ParseConditionalPartialThenElseExpressions(
	TPParseTreeNode *then_expression, TPParseTreeNode *else_expression) {
	// <Condition> ? <Then-Expression> : <Else-Expression>
	// Condition up to question-mark already parsed
	int token_ID = _tokenizer.GetToken();
	assert(token_ID == kTokenOperatorConditionalIf);
	*then_expression = ParseExpression();
	if (!ExpectConditionalThen())	{
		*else_expression = NULL;
	}	else {
		*else_expression = ParseExpression();
	}
	// Results get returned via out-pointers here.
	// We have to put everything together on the call-site.
}

CString CFormulaParser::CurrentFunctionName() {
	return _function_name;
}

void CFormulaParser::ErrorMissingAction(int token_ID) {
  CString error_message = "Missing action after when-condition";
  if (token_ID == kTokenNumber) {
    error_message += "\nFound a number. Probably missing operator";
  } else if (token_ID == TokenIsBracketOpen(token_ID)) {
    error_message += "\nFound a bracket. Probably missing operator";
  } else if (token_ID == kTokenIdentifier) {
    CString name = _tokenizer.GetTokenString();
    if (name.Left(4) == "user") {
      error_message += "\nFound a user-variable.\n";
      error_message += "Correct syntax: When <condition> Set user_xyz";
    } else {
      error_message += "\nFound an identifier. Probably missing operator";
    }
  }
  CParseErrors::Error(error_message);
}

TPParseTreeNode CFormulaParser::ParseOpenEndedWhenConditionSequence() {
  TPParseTreeNode last_when_condition = NULL;
  bool last_when_condition_was_open_ended = false;
  TPParseTreeNode first_when_condition_of_sequence = NULL;
  int token_ID = _tokenizer.LookAhead();
  while (token_ID == kTokenOperatorConditionalWhen) {
    token_ID = _tokenizer.GetToken();
    TPParseTreeNode condition = ParseExpression();
    TPParseTreeNode when_condition = new CParseTreeNode(_tokenizer.LineRelative());
    when_condition->MakeWhenCondition(condition);
    // Remember first when-condition
    if (first_when_condition_of_sequence == NULL) {
      first_when_condition_of_sequence = when_condition;
    }
    // Concatenate conditions in sequence
    if (last_when_condition != NULL) {
      if (last_when_condition_was_open_ended) {
        // Open ended when-conditions:
        // Second sibbling points to next when-condition
        // Third siblling points to next open-ender
        last_when_condition->_second_sibbling = when_condition;
      } else {
        // When condition with action (2nd sibbling)
        // Third sibbling oints to next when-conditon
        last_when_condition->_third_sibbling = when_condition;
      }
    }
    // For future back-patching
    last_when_condition = when_condition;
    // Next either:
    // * action
    // * another when-condition
    // * user-variable to be set
    token_ID = _tokenizer.LookAhead(true);
    if (TokenIsOpenPPLAction(token_ID))  { 
      TPParseTreeNode action = ParseOpenPPLAction(); 
      when_condition->_second_sibbling = action;
      // For future backpatching
      last_when_condition_was_open_ended = false;
      token_ID = _tokenizer.LookAhead();
    } else if (token_ID == kTokenOperatorConditionalWhen) {
      // All work to do: in the next loop
      last_when_condition_was_open_ended = true;
      // LookAhead() already executed
      continue;
    } else if ((token_ID == kTokenEndOfFile) 
        || (token_ID == kTokenEndOfFunction)) {
      // Parsing successfully finished
      break;
    } else {
      ErrorMissingAction(token_ID);
      break;
    }
  }
  return first_when_condition_of_sequence;
}

TPParseTreeNode CFormulaParser::ParseOpenPPLUserVar() {
	// User-variable to be set
  int token_ID = _tokenizer.GetToken();
  if (token_ID != kTokenIdentifier) {
    CParseErrors::Error("Unexpected token.\n"
      "User-variable or memory-store-command expected.");
		return NULL;
  }
	CString identifier = _tokenizer.GetTokenString();
	if (identifier.Left(4).MakeLower() != "user") {
		CParseErrors::Error("Unexpected identifier.\n"
      "User-variable or memory-store-command expected.");
		return NULL;
	}
	TPParseTreeNode action = new CParseTreeNode(_tokenizer.LineRelative());
	action->MakeUserVariableDefinition(identifier);
  // Not expecting any Force here
  return action;
}

TPParseTreeNode CFormulaParser::ParseOpenPPLAction(){
	int token_ID = _tokenizer.GetToken();
	assert(TokenIsOpenPPLAction(token_ID));
	TPParseTreeNode action;
	if (token_ID == kTokenActionReturn) {
		// RETURN <Expression> FORCE
		action = ParseExpression();
    ExpectKeywordForce(token_ID);
	}
	else if (token_ID == kTokenActionRaiseTo) { 
    // NL-betsizing
    //   RaiseTo N Force
		action = ParseOpenPPLRaiseToExpression();
    ExpectKeywordForce(token_ID);
	}
  else if (token_ID == kTokenActionRaiseBy) {
    // NL-betsizing
		// There are 2 possibilities
		//   RaiseBy N Force
		//   RaiseBy X% Force
		action = ParseOpenPPLRaiseByExpression(); 
    ExpectKeywordForce(token_ID);
	}
  else if (token_ID == kTokenActionUserVariableToBeSet) { 
    action = ParseOpenPPLUserVar();
    // Not expecting keyword Force here
  } else {
		// Predefined action, like Check or Fold
    action = new CParseTreeNode(_tokenizer.LineRelative());
		action->MakeAction(token_ID);
    ExpectKeywordForce(token_ID);
	}
	return action;
}

bool CFormulaParser::ExpectKeywordForce(int last_important_roken_ID) {
	int _token_ID = _tokenizer.GetToken();
	if (_token_ID == kTokenKeywordForce) {
		// Check for unsupported Shanky-style delay
		// which can only happen after actions 
		// WHEN ... RAISEMAX FORCE DELAY 42
		_token_ID = _tokenizer.LookAhead();
		if (_token_ID == kTokenUnsupportedDelay) {
			CParseErrors::Error("Unsupported Shanky-style delay.\n"
        "OpenHoldem provides a far more simple\n"
        "and far more powerful f$delay-function for that");
			// And consume 2 tokens to avoid further messages;
			_token_ID = _tokenizer.GetToken();
			_token_ID = _tokenizer.GetToken();
		}
		// Both cases, with and without delay, are considered "good".
		return true;
	} else if (last_important_roken_ID == kTokenActionRaise) {
    // Last thing we saw was a Raise
    // Probably Shanky-style betsizing
    CParseErrors::Error("Missing keyword FORCE after action Raise.\n"
      "Did you attempt to specify a betsize the old Shanky way?\n"
      "Then either use RaiseTo or RaiseBy.");
    return false;

  }
  // General error message on missing keyword force
	CParseErrors::Error("Missing keyword FORCE");
	return false;
}

TPParseTreeNode CFormulaParser::ParseOpenPPLRaiseToExpression() { 
  // RaiseTo N Force
	// Keyword RaiseTo got already consumed
	TPParseTreeNode action = new CParseTreeNode(_tokenizer.LineRelative());
	TPParseTreeNode expression;
	int _token_ID = _tokenizer.LookAhead();
	if ((_token_ID == kTokenNumber)
		  || (_token_ID == kTokenIdentifier)
		  || TokenIsBracketOpen(_token_ID)) {
		expression = ParseExpression();
	} else {
    CParseErrors::Error("Missing expression after keyword RaiseTo.\n"
      "Expecting the betsize in big blinds.");
    return NULL;
  }
	action->MakeRaiseToAction(expression);
	return action;
}

TPParseTreeNode CFormulaParser::ParseOpenPPLRaiseByExpression() { 
	// There are 3 possibilities
	//   RAISE <Amount> FORCE
	//   RAISE <PercentagedPot>% FORCE
	//
	// Keyword RAISE got already consumed
	TPParseTreeNode action = new CParseTreeNode(_tokenizer.LineRelative());
	TPParseTreeNode expression;
	int _token_ID = _tokenizer.LookAhead();
	if ((_token_ID == kTokenNumber)
		  || (_token_ID == kTokenIdentifier)
		  || TokenIsBracketOpen(_token_ID)){
		expression = ParseExpression();
	} else {
    CParseErrors::Error("Missing expression after keyword RaiseTo.\n"
      "Expecting the betsize in big blinds.");
    return NULL;
  }
	_token_ID = _tokenizer.LookAhead();
	if (_token_ID == kTokenOperatorPercentage) {
		// Percentaged Potsize
		_tokenizer.GetToken();
		action->MakeRaiseByPercentagedPotsizeAction(expression);
		return action;
	}	else {
		// Raise by N big blinds
		action->MakeRaiseByAction(expression);
		return action;
	}
}

void CFormulaParser::BackPatchOpenEndedWhenConditionSequence(
    TPParseTreeNode first_when_condition_of_a_function) {
  // Backpatching everything after a complete functiuon got parsed
  TPParseTreeNode last_open_ended_when_condition = NULL;
  TPParseTreeNode current_when_condition = first_when_condition_of_a_function;
  while (current_when_condition != NULL) {
    if (current_when_condition->IsOpenEndedWhenCondition()) {
      // Setting the "Else"-part of the last open-ended when-condition
      // to the next open ended when condition
      if (last_open_ended_when_condition != NULL) {
        assert(last_open_ended_when_condition != current_when_condition);
        last_open_ended_when_condition->SetRightMostSibbling(
          current_when_condition);
      }
      last_open_ended_when_condition = current_when_condition;
      // The "Then"-part (2nd sibbling) of an open-ended when-condition
      // points to the next (maybe open-ended) when-condition.
      // The 3rd sibbling is ATM undefined and gets back-patched later.
      assert(current_when_condition != current_when_condition->_second_sibbling);
      current_when_condition = current_when_condition->_second_sibbling;
    } else if (current_when_condition->IsWhenConditionWithAction()) {
      // Normal when-condition with action (2nd sibbling).
      // The 3rd sibbling contains the "Else"-part,
      // i.e. next (maybe open-ended) when-condition
      TPParseTreeNode right_most_sibbling = current_when_condition->GetRightMostSibbling();
      assert(current_when_condition != right_most_sibbling);
      if ((right_most_sibbling == NULL)
          || !right_most_sibbling->IsAnyKindOfWhenCondition()) {
        // End of when-condition-sequence reached
        break;
      }
      current_when_condition = right_most_sibbling;
    } else {
      // End of when-condition sequence reached
      assert(current_when_condition != NULL);
      assert(!current_when_condition->IsAnyKindOfWhenCondition());
      break;
    }   
  }
}

void CFormulaParser::ParseDebugTab(CString function_text) {
  p_debug_tab->Clear();
  CString next_line;
  int separator_index = 0;
  // Split lines
  while (AfxExtractSubString(next_line, function_text, separator_index, '\n')) {
    ++separator_index;
    int pos = next_line.Find('=');
    if (pos < 0) {
      // No equality-sign found. Empty line or not valid
      continue;
    }
    // Expression-text: everything behind first "="
    int expresion_length = next_line.GetLength() - pos - 1;
    CString expression_text = next_line.Right(expresion_length);
    // Parse this line
    _tokenizer.SetInput(expression_text);
    TPParseTreeNode expression = ParseExpression();
    // Care about operator precendence
    _parse_tree_rotator.Rotate(expression, &expression);
    // Add line and expression to debug-tab
    p_debug_tab->AddExpression(expression_text, expression);
  }
}