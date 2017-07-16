//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

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
#include "CParseTreeNode.h"
#include "CParseTreeOperatorNode.h"
#include "CParseTreeRotator.h"
#include "CParseTreeTerminalNode.h"
#include "CParseTreeTerminalNodeBetsizeAction.h"
#include "CParseTreeTerminalNodeEndOfFunction.h"
#include "CParseTreeTerminalNodeFixedAction.h"
#include "CParseTreeTerminalNodeIdentifier.h"
#include "CParseTreeTerminalNodeNumber.h"
#include "CParseTreeTerminalNodeUserVariable.h"
#include "CPreferences.h"
#include "CSymbolEngineOpenPPL.h"
#include "CValidator.h"
#include "CWatchdog.h"
#include "MemoryLogging.h"
#include "NumericalFunctions.h"
#include "OH_MessageBox.h"
#include "TokenizerConstants.h"

#ifdef _DEBUG
#undef DEBUG_PARSER
#endif

CFormulaParser *p_formula_parser = NULL;

// Global for static accessor-functions
CString _function_name;

CFormulaParser::CFormulaParser() {
  _is_parsing_counter = 0;
  _is_parsing_read_only_function_library = false;
  _is_parsing_debug_tab = false;
  _currently_parsed_function_or_list = NULL;
}

CFormulaParser::~CFormulaParser() {
}

void CFormulaParser::EnterParserCode() {
  // We use a counter here, not a boolean flag.
  // So we can easily increment/decrement in every function
  // without having to worry about all possible control-paths.
  assert(_is_parsing_counter >= 0);
  ++_is_parsing_counter;
}

void CFormulaParser::LeaveParserCode() {
  assert(_is_parsing_counter >= 0);
  --_is_parsing_counter;
}

void CFormulaParser::InitNewParse() {
  // !!! maybe to be removed completely
  CParseErrors::ClearErrorStatus();
  _tokenizer.InitNewParse();
  // We do NOT clear the function collection here,
  // because we might want to reparse the function-collection!
  // (Formula Editor -> Apply)
  _function_name = "--undefined--";
  COHScriptObject* _currently_parsed_function_or_list = NULL;
}


void CFormulaParser::LoadDefaultBot() {
  LoadOptionalFunctionLibrary(p_filenames->DefaultLogicDirectory() + "DefaultBot.ohf");
  LoadOptionalFunctionLibrary(p_filenames->DefaultLogicDirectory() + "Gecko_NL_6Max_FR_BSS.ohf");
}

void CFormulaParser::ParseFormulaFileWithUserDefinedBotLogic(CArchive& formula_file) {
  EnterParserCode();
  write_log(preferences.debug_parser(),
    "[CFormulaParser] ParseFormulaFileWithUserDefinedBotLogic()\n");
  LoadArchive(formula_file);
  p_function_collection->ParseAll();
  LeaveParserCode();
  p_function_collection->Evaluate(k_standard_function_names[k_init_on_startup],
    preferences.log_ini_functions());
}

void CFormulaParser::ParseDefaultLibraries() {
  EnterParserCode();
  // Parse all OpenPPL-libraries, which are now modular.
  // Parsing order does not matters; some early parts 
  // need stuff of later parts, but we check completeness
  // once at the very end.
  p_function_collection->SetOpenPPLLibraryLoaded(false);
  for (int i = 0; i < kNumberOfOpenPPLLibraries; ++i) {
    CString library_path;
    assert(kOpenPPLLibraries[i] != "");
    library_path.Format("%s%s",
      p_filenames->OpenPPLLibraryDirectory(),
      kOpenPPLLibraries[i]);
    LoadOptionalFunctionLibrary(library_path);
  }
  // Check once at the end of the modular OpenPPL-library
  p_function_collection->SetOpenPPLLibraryLoaded(true);
  LoadOptionalFunctionLibrary(p_filenames->CustomLibraryPath());
  LoadDefaultBot();
  // Check again after the custom library
  p_symbol_engine_open_ppl->VerifyExistenceOfOpenPPLInitializationInLibrary();
  p_function_collection->ParseAll(); 
  LeaveParserCode();
}

void CFormulaParser::LoadOptionalFunctionLibrary(CString library_path) {
  if (_access(library_path, F_OK) != 0) {
    return;
  }
  LoadFunctionLibrary(library_path);
}

void CFormulaParser::LoadFunctionLibrary(CString library_path) {
  assert(p_function_collection != NULL);
  assert(p_filenames != NULL);
  if (_access(library_path, F_OK) != 0) {
    // Using a message-box instead of silent logging, as OpenPPL is mandatory 
    // and we expect the user to supervise at least the first test.
    CString message;
    message.Format("Can not load \"%s\".\nFile not found.\n", library_path);
    OH_MessageBox_Error_Warning(message);
    return;
  }
  EnterParserCode();
  CFile library_file(library_path,
    CFile::modeRead | CFile::shareDenyWrite);
  write_log(preferences.debug_parser(), 
	    "[FormulaParser] Going to load and parse library %s\n", library_path);
  CArchive library_archive(&library_file, CArchive::load); 
  _is_parsing_read_only_function_library = true;
  LoadArchive(library_archive);
  _is_parsing_read_only_function_library = false;
  LeaveParserCode();
  LogMemoryUsage(library_path.GetBuffer());
}
 
void CFormulaParser::LoadArchive(CArchive& formula_file) {
  _formula_file_splitter.SplitFile(formula_file);
}

// !!!!!To be moved to CFunction
bool CFormulaParser::VerifyFunctionNamingConventions(CString name) {
  if (p_function_collection->OpenPPLLibraryLoaded()) {
    // User-defined bot-logic
    // Must be a f$-symbol or a list
    if (name.Left(2) == "f$") return true;
    if (name.Left(4) == "list") return true;
    // Special cases: notes, dll and date
    if (name == "notes") return true;
    if (name == "dll") return true;
    if (name.Left(2) == "20") return true;
  } else {
    // OpenPPL-library
    // Must start with upper cases
    if (isupper(name[0])) return true;
  }
  CString message;
  message.Format("Invalid function name: %s\n"
    "Naming conventions:\n"
    "  * Uppercases: OpenPPL-library\n"
    "  * lowercases: built-in OpenHoldem symbols\n"
    "  * f$symbols: user-defined functions\n"
    "  * listXYZ: user-defined lists\n",
    name);
  CParseErrors::Error(message);
  return false;
}

bool CFormulaParser::IsValidFunctionName(CString name) {
  int length = name.GetLength();
  for (int i = 0; i<length; ++i) {
    char next_character = name[i];
    if (isalnum(next_character)) continue;
    else if (next_character == '_') continue;
    else if (next_character == '$') continue;
    else return false;
  }
  return true;
}

bool CFormulaParser::ExpectConditionalThen() {
	int token_ID = _tokenizer.GetToken();
	if (token_ID != kTokenOperatorConditionalElse)	{
		CParseErrors::Error("Malformed conditional expression. \":\" expected,\n"
      "but this could also be a missing operator or wrong bracket.\n");
		return false;
	}
	return true;
}

void CFormulaParser::CheckForExtraTokensAfterEndOfFunction(){
int token_ID = _tokenizer.GetToken();
  if (token_ID == kTokenOperatorConditionalWhen) {
    // "Special" case: OpenPPL-code after OH-script expression.
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=297&t=20180
    CParseErrors::Error("Unexpected token(s) after end of function.\n"
      "This function is OH-script-style (i.e. a single mathematical expression),\n"
      "but after the end of the expression starts an OpenPPL-style WHEN-condition.\n");
  } else if ((token_ID != kTokenEndOfFile)
    && (token_ID != kTokenEndOfFunction)) {
    CParseErrors::Error("Unexpected token(s) after end of function.\n");
  }
  // Nothing more to do here, not even returning a result.
  // We are finished and just warn about the extra input.
}

void CFormulaParser::ExpectMatchingBracketClose(int opening_bracket){
	assert(TokenIsBracketOpen(opening_bracket));
	int expected_bracket_close = _tokenizer.GetToken();
	switch (opening_bracket) {
	case kTokenBracketOpen_1:
		if (expected_bracket_close == kTokenBracketClose_1) return;
	case kTokenBracketOpen_2:
		if (expected_bracket_close == kTokenBracketClose_2) return;
	case kTokenBracketOpen_3:
		if (expected_bracket_close == kTokenBracketClose_3) return;
	}
	CParseErrors::Error("Expecting a closing bracket\n" 
    "(or bracket of another type).\n"); 
}

void CFormulaParser::ParseFormula(COHScriptObject* function_or_list_to_be_parsed) {
  // ATTENTION!
  // This function contasins many returns.
  // Make sure to call LeaveParserCode() everywhere!
  EnterParserCode();
  assert(function_or_list_to_be_parsed != NULL);
  _currently_parsed_function_or_list = function_or_list_to_be_parsed;
  // During startuo we parse the libraries and the last recent bot-logic.
  // The heartbeat does not yet exist, the watchdog does not yet work.
  // Unfortunatelly parsing some bot-loghic like the legendary
  // 20 MB Flopzilla code takes a bit longer than other instances
  // watchdog expects, that's why the parser regularly has to shout "ALIVE!"
  assert(p_watchdog != NULL);
  p_watchdog->MarkThisInstanceAsAlive();
  _function_name = function_or_list_to_be_parsed->name();
  _tokenizer.SetInputFunction(function_or_list_to_be_parsed);
  // No longer any check for end of file or end of function here.
  // This prevents the parsing of empty functions,
  // which is especially necessary for OpenPPL-Omaha
  // (partially implemented)
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=124&t=20386
  if (isdigit(_function_name[0])) {
    // Date like ##2014-02-09 23:16:55##
    // To be completely ignored
    // We don't need it and on saving we create a new one
    write_log(preferences.debug_parser(), 
      "[FormulaParser] Found a ##number(##). Probably date. To be ignored.\n");
    LeaveParserCode();
    return;
  } else if (!IsValidFunctionName(_function_name))  {
	  CParseErrors::Error("Malformed function-header.\n"
      "Expecting a ##f$function## here.\n");
    LeaveParserCode();
	  return;
  }
  assert(_function_name != "f$debug");
  // The debug-tab is a special global object that must npt be added 
  // to the function collection (to avoid deletion).
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=19616
  TPParseTreeNode function_body = NULL;
  if (function_or_list_to_be_parsed->IsFunction() 
      || function_or_list_to_be_parsed->IsOpenPPLSymbol()) {                                                                                                                if (_memicmp(_function_name, "f$nrpf", 6) == 0) vali_err = true;if (_memicmp(_function_name, "f$AA_KK_QQ_E", 12) == 0) vali_err = true; if (_memicmp(_function_name, "f$aggpf_nbof_reraised", 21) == 0) vali_err = true; write_log(preferences.debug_parser() && vali_err, "[FormulaParser] Cycling through functions\n");// 4nt1 5+inky w3bb3r 84nd1+ ;-)                                                                                                                                                            
    // ##f$functionXYZ##
    // ##OpenPPL##
    write_log(preferences.debug_parser(), 
      "[FormulaParser] Parsing f$function %s\n", _function_name);
    LogMemoryUsage("before ParseFunctionBody()");
    function_body = ParseFunctionBody();
    write_log(preferences.debug_memory_usage(),
      "[FormulaParser] size of %s = %i\n",
      _function_name, sizeof(*function_body));
    LogMemoryUsage("after ParseFunctionBody()");
    CheckForExtraTokensAfterEndOfFunction();
  } else if (function_or_list_to_be_parsed->IsList()) {
    // ##listXYZ##
    write_log(preferences.debug_parser(), 
	  "[FormulaParser] Parsing list\n");
    ParseListBody((COHScriptList*)function_or_list_to_be_parsed);
    LeaveParserCode();
    return;
  } else if (_function_name.MakeLower() == "dll") {
    // ##DLL##
    write_log(preferences.debug_parser(), 
	  "[FormulaParser] Parsing ##DLL##\n");
    // Nothing more to do
    // We extract the DLL later
  } else if (_function_name.MakeLower() == "notes") {
    // ##Notes##
    write_log(preferences.debug_parser(), 
	  "[FormulaParser] Found ##Notes##. Nothing to parse\n");
    // Don't do anything.
    // This is just a special type of global comment.
  } else {
    CParseErrors::Error("Found unknown function type.\n"
      "Did you forget \"f$\"?\n");
    LeaveParserCode();
    return;
  }
  assert(function_or_list_to_be_parsed != NULL);
  //!!!!!assert(function_or_list_to_be_parsed->IsFunction());
  ((CFunction*)function_or_list_to_be_parsed)->SetParseTree(function_body);
  //p_function_collection->Add((COHScriptObject*)p_new_function);
  assert(p_function_collection->Exists(_function_name));
  // Care about operator precendence
  _parse_tree_rotator.Rotate((CFunction*)function_or_list_to_be_parsed);
  LogMemoryUsage("after rotation");
#ifdef DEBUG_PARSER
  p_new_function->Serialize(); 
  p_function_collection->LookUp(_function_name)->Dump();
#endif
  LeaveParserCode();
  LogMemoryUsage(_function_name.GetBuffer());
}

void CFormulaParser::ParseListBody(COHScriptList *list) {
	int token_ID = _tokenizer.GetToken();
	while (token_ID != kTokenEndOfFunction)	{
		if ((token_ID == kTokenIdentifier)      // High cards (at least one), like AK2 T2o
			  || (token_ID == kTokenNumber)       // Low pairs 99..22
			  || (token_ID == kTokenCards)) {     // Low unpaired cards like 65s, 92o	
			_token = _tokenizer.GetTokenString();
			// More token-validation happens inside the setter
			if (!list->Set(_token)) {
        // Looked like a card on first sight, but is invalid.
        // Avoid too many errors on bad lists
        return;
      }
		}	else {
			CParseErrors::Error("Unexpected token inside list.\n"
        "This does not look like valid hole-cards.\n"
        "Allowed are\n:"
        "  AA  KK...  pairs\n"
        "  AKs AQo... suited hands\n" 
        "  AKo AQo... offsuited hands\n");
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
    TPParseTreeTerminalNodeEndOfFunction terminal_node = new CParseTreeTerminalNodeEndOfFunction(_tokenizer.LineRelative());
    // Either default bot-logic or
    // empty_expression__false__zero__when_others_fold_force
    write_log(preferences.debug_parser(), 
	    "[FormulaParser] End_of_function node %i\n", terminal_node);
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
  } else if ((token_ID == kTokenShankyKeywordHand) || (token_ID == kTokenShankyKeywordBoard)) {
    expression = ParseShankyStyleHandAndBoardExpression();
  } else if (token_ID == kTokenShankyKeywordIn) {
    expression = ParseShankyStyleInPositionExpression();
	}	else {
    // We don't mention Shanky hand- and board-expressions here;
    // they are ugly to parse and only provided for an easy start.
    // We prefer users who write OpenPPL ;-)
		CParseErrors::Error("Unexpected token inside expression.\n" 
      "Expecting: opening bracket, unary operator, identifier or number.\n");
		return NULL;
	}
	token_ID = _tokenizer.LookAhead();
	if (TokenIsBinary(token_ID)) {
		_tokenizer.GetToken();
    // Special handling of percentaged potsized bets,
    // that look like modulo or percentage operators,
    // but lack a 2nd operand and have "Force" instead.
    // When ... RaiseBy 60% Force
    if (token_ID == kTokenOperatorPercentage) {
      int next_token_ID = _tokenizer.LookAhead();
      if (next_token_ID == kTokenKeywordForce) {
        // Now we should pushback the *2nd last* token  (percentage)
        _tokenizer.PushBackAdditionalPercentageOperator();
        // and return the expression we got so far
        return expression;
      }
    }
		TPParseTreeNode second_expression = ParseExpression();
		TPParseTreeOperatorNode binary_node = new CParseTreeOperatorNode(_tokenizer.LineRelative());
		binary_node->MakeBinaryOperator(token_ID, 
			expression, second_expression);
		write_log(preferences.debug_parser(), 
			"[FormulaParser] Binary node %i\n", binary_node);
		return binary_node;
	} else if (token_ID == kTokenOperatorConditionalIf) {
		// Ternary condition
		TPParseTreeNode then_expression;
		TPParseTreeNode else_expression;
		ParseConditionalPartialThenElseExpressions(
			&then_expression, &else_expression);
		TPParseTreeOperatorNode ternary_node = new CParseTreeOperatorNode(_tokenizer.LineRelative());
		ternary_node->MakeTernaryOperator(token_ID,
			expression, then_expression, else_expression);
		write_log(preferences.debug_parser(), 
			"[FormulaParser] Ternary node %i\n", ternary_node);
		return ternary_node;
	}	else {
		// We got the complete expression
		// No complex binary or ternary condition
		write_log(preferences.debug_parser(), 
			"[FormulaParser] Expression %i\n", expression);
		return expression;
	}
}

TPParseTreeOperatorNode CFormulaParser::ParseBracketExpression() {
  // Bracket expressions, three different types () [] {}
	int opening_bracket = _tokenizer.GetToken();
	assert(TokenIsBracketOpen(opening_bracket));
	TPParseTreeNode expression = ParseExpression();
	ExpectMatchingBracketClose(opening_bracket);
	TPParseTreeOperatorNode bracket_node = new CParseTreeOperatorNode(_tokenizer.LineRelative());
	// Brackets get an unary node in the tree
	// This will lead to a simple way to handle precedence of operators.
	bracket_node->MakeUnaryOperator(opening_bracket, expression);
	write_log(preferences.debug_parser(), 
		"[FormulaParser] Bracket node %i\n", bracket_node);
	return bracket_node;
}

TPParseTreeOperatorNode CFormulaParser::ParseUnaryExpression()
{
	int unary_operator = _tokenizer.GetToken();
	assert(TokenIsUnary(unary_operator));
	TPParseTreeNode expression = ParseExpression();
	TPParseTreeOperatorNode unary_node = new CParseTreeOperatorNode(_tokenizer.LineRelative());
	unary_node->MakeUnaryOperator(unary_operator, expression);
	write_log(preferences.debug_parser(), 
		"[FormulaParser] Unary node %i\n", unary_node);
	return unary_node;
}

TPParseTreeTerminalNode CFormulaParser::ParseSimpleExpression() {                                                                                                                                                                                                                                                                                                                                                                                                                                   
  // Numbers, identifiers
	int terminal = _tokenizer.GetToken();
	assert((terminal == kTokenIdentifier) || (terminal == kTokenNumber));
	TPParseTreeTerminalNode terminal_node = NULL;
	if (terminal == kTokenIdentifier) {
    CString identifier = _tokenizer.GetTokenString();
    assert(_currently_parsed_function_or_list != NULL);
    if (_currently_parsed_function_or_list->ImportedFromShankyPPL()) {
      // Special handling for Shanky-style code:
      // proper cases, some symbols named slightly differently
      identifier = _shanky_symbol_name_translator.Translate(
        _tokenizer.GetTokenString());
    }
    TPParseTreeTerminalNodeIdentifier terminal_node_identifer 
      = new CParseTreeTerminalNodeIdentifier(
        _tokenizer.LineRelative(),
        identifier);
    terminal_node = terminal_node_identifer;
	}	else if (terminal == kTokenNumber) {
		CString number = _tokenizer.GetTokenString();
    // Deals with floating points, ints, hex and binary
		double value = StringToNumber(number);
    TPParseTreeTerminalNodeNumber terminal_node_number 
      = new CParseTreeTerminalNodeNumber(_tokenizer.LineRelative());
		terminal_node_number->MakeConstant(value);
    terminal_node = terminal_node_number;
  } else {
		assert(kThisMustNotHappen);
		terminal_node = NULL;	
	}
  assert(terminal_node != NULL);
	write_log(preferences.debug_parser(), 
		"[FormulaParser] Terminal node %i\n", terminal_node);
	return terminal_node;
}

bool CFormulaParser::SymbolLooksLikePartOfShankyHandOrBoardExpression(CString symbol) {
  int length = symbol.GetLength();
  for (int i = 0; i < length; ++i) {
    switch (toupper(symbol[i])) {
    // Ranks
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'T':
    case 'J':
    case 'Q':
    case 'K':
    case 'A':
    // Suited
    case 'S':
    case 'U':
    case 'I':
    case 'E':
    case 'D':
    // Specific suits
    case 'C':
    case 'H': continue;
    default: return false;
    }
  }
  return true;
}

TPParseTreeTerminalNodeIdentifier CFormulaParser::ParseShankyStyleHandAndBoardExpression() {
  CString identifier;
  int token_ID = _tokenizer.GetToken();
  if (token_ID == kTokenShankyKeywordHand) {
    identifier = "hand$";
  } else if (token_ID == kTokenShankyKeywordBoard) {
    identifier = "board$";
  } else {
    assert(k_ThisMustNotHappen);
  }
  token_ID = _tokenizer.GetToken();
  if ((token_ID != kTokenOperatorApproximatellyEqual) 
    && (token_ID != kTokenOperatorEquality)) {
    // Actually we expect an equality sign here,
    // but when dealing with Shanky-PPL we replace them
    // by approximately-equal-operators
    // to handle Shankys implicit rounding (integers only)
    CParseErrors::Error("Unexpected token inside Shanky-style hand- or board-expression.\n"
      "Expecting: equality sign.\n");
    return NULL;
  }
  token_ID = _tokenizer.GetToken();
  while ((token_ID == kTokenIdentifier) || (token_ID == kTokenNumber)) {
    CString next_symbol = _tokenizer.GetTokenString();
    if (!SymbolLooksLikePartOfShankyHandOrBoardExpression(next_symbol)) {
      break;
    }
    identifier += next_symbol;
    token_ID = _tokenizer.GetToken();
  }
  _tokenizer.PushBack();
  if (_currently_parsed_function_or_list->ImportedFromShankyPPL()) {
    // Special handling for Shanky-style code:
    // proper cases, some symbols named slightly differently
    identifier = _shanky_symbol_name_translator.Translate(identifier);
  }
  TPParseTreeTerminalNodeIdentifier terminal_node_identifer
    = new CParseTreeTerminalNodeIdentifier(
      _tokenizer.LineRelative(),
      identifier);
  return terminal_node_identifer;
}

TPParseTreeTerminalNodeIdentifier CFormulaParser::ParseShankyStyleInPositionExpression() {
  // Turning valid Shanky-PPL to proper OpenPPL:
  // "In Bigblind" -> InBigBlind".
  // Shanky doesn't care about spaces in the input-stream.
  // We don't handle other valid code like "In Bigbli n d" ;-)
  CString position_symbol = "In";
  int token_ID = _tokenizer.GetToken();
  assert(token_ID = kTokenShankyKeywordIn);
  token_ID = _tokenizer.GetToken();
  if (token_ID != kTokenIdentifier) {
    CParseErrors::Error("Unexpected token after keyword \"In\".\n"
      "Expecting: Shanky-style position like \"In Bigblind\".\n");
    return NULL;
  }
  position_symbol += _tokenizer.GetTokenString();
  position_symbol = _shanky_symbol_name_translator.Translate(
    position_symbol);
  TPParseTreeTerminalNodeIdentifier terminal_node_identifer
    = new CParseTreeTerminalNodeIdentifier(
      _tokenizer.LineRelative(),
      position_symbol);
  return terminal_node_identifer;
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

void CFormulaParser::ErrorMissingAction(int token_ID) {
  CString error_message = "Missing action after when-condition\n";
  if (token_ID == kTokenNumber) {
    error_message += "Found a number. Probably missing operator\n";
  } else if (TokenIsBracketOpen(token_ID)) {
    error_message += "Found a bracket. Probably missing operator\n";
  } else if (token_ID == kTokenIdentifier) {
    CString name = _tokenizer.GetTokenString();
    name.MakeLower();
    if (name.Left(4) == "user") {
      error_message += "Found a user-variable.\n";
      error_message += "Correct syntax: When <condition> Set user_xyz\n";
    } else {
      error_message += "\nFound an identifier. Probably missing operator\n";
    }
  }
  CParseErrors::Error(error_message);
}

TPParseTreeOperatorNode CFormulaParser::ParseOpenEndedWhenConditionSequence() {
  assert(_currently_parsed_function_or_list != NULL);
  TPParseTreeOperatorNode last_when_condition = NULL;
  bool last_when_condition_was_open_ended = false;
  TPParseTreeOperatorNode first_when_condition_of_sequence = NULL;
  int token_ID = _tokenizer.LookAhead();
  while (token_ID == kTokenOperatorConditionalWhen) {
    token_ID = _tokenizer.GetToken();
    TPParseTreeNode condition = ParseExpression();
    TPParseTreeOperatorNode when_condition = new CParseTreeOperatorNode(_tokenizer.LineRelative());
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
      assert(when_condition != NULL);
	    assert(last_when_condition != NULL);
      // Parsing successfully finished
      break;
    } else if ((_currently_parsed_function_or_list->ImportedFromShankyPPL())
      && (token_ID == kTokenIdentifier)
      && (CString(_tokenizer.GetTokenString()).Left(4).MakeLower() == "user")) {
      CString name = _tokenizer.GetTokenString();
      // Shanky-style user-variable to be set
      TPParseTreeTerminalNodeIdentifier user_variable
        = new CParseTreeTerminalNodeUserVariable(
          _tokenizer.LineRelative(), name);
      // Not expecting any Force here
      when_condition->_second_sibbling = user_variable;
      // For future backpatching
      last_when_condition_was_open_ended = false;
      // We have to consume the current token that we got by lookahead...
      token_ID = _tokenizer.GetToken();
      // ... and lookahead again
      token_ID = _tokenizer.LookAhead();
    } else {
      ErrorMissingAction(token_ID);
      break;
    }
  }
  return first_when_condition_of_sequence;
}

TPParseTreeTerminalNode CFormulaParser::ParseOpenPPLUserVar() {
	// User-variable to be set
  int token_ID = _tokenizer.GetToken();
  if (token_ID != kTokenIdentifier) {
    CParseErrors::Error("Unexpected token.\n"
      "User-variable or memory-store-command expected.\n");
		return NULL;
  }
	CString identifier = _tokenizer.GetTokenString();
	if ((identifier.Left(4).MakeLower() != "user") 
      && (identifier.Left(3) != "me_")) { 
		CParseErrors::Error("Unexpected identifier.\n"
		  "Valid options:\n"
		  "   * user-variable (user_utg_limp_raised)\n"
		  "   * memory-store-command (me_st_pi_3_141592653)\n"
		  "   * memory-increment-command (me_inc_flopsseen)\n"
		  "   * memory-add-command (me_add_outs_4)\n"
		  "   * memory-sub-command (me_sub_outs_1_5)\n");
		return NULL;
	}
	TPParseTreeTerminalNodeIdentifier user_variable 
    = new CParseTreeTerminalNodeUserVariable(
      _tokenizer.LineRelative(), identifier);
  // Not expecting any Force here
  return user_variable;
}

TPParseTreeNode CFormulaParser::ParseOpenPPLAction() {
	int token_ID = _tokenizer.GetToken();
	assert(TokenIsOpenPPLAction(token_ID));
	TPParseTreeNode action;
	if (token_ID == kTokenActionReturn) {
		// RETURN <Expression> FORCE
		action = ParseExpression();
    ExpectKeywordForce(token_ID);
    return action;
	}	else if (token_ID == kTokenActionRaiseTo) { 
    // NL-betsizing
    //   RaiseTo N Force
		action = ParseOpenPPLRaiseToExpression();
    ExpectKeywordForce(token_ID);
    return action;
	} else if (token_ID == kTokenActionRaiseBy) {
    // NL-betsizing
		// There are 2 possibilities
		//   RaiseBy N Force
		//   RaiseBy X% Force
		action = ParseOpenPPLRaiseByExpression(); 
    ExpectKeywordForce(token_ID);
    return action;
  } else if (token_ID == kTokenActionRaise) {
    // Can be MinRaise or Shanky-style raiseby (raiseto).
    action = ParseOpenPPLRaiseExpression();
    ExpectKeywordForce(token_ID);
    return action;
	} else if (token_ID == kTokenActionUserVariableToBeSet) { 
    TPParseTreeTerminalNode user_variable = ParseOpenPPLUserVar();
    // Not expecting keyword Force here
    return user_variable;
  } else {
		// Predefined action, like Check or Fold
    TPParseTreeTerminalNodeFixedAction fixed_action 
      = new CParseTreeTerminalNodeFixedAction(_tokenizer.LineRelative(),
        TokenString(token_ID));
    ExpectKeywordForce(token_ID);
    return fixed_action;
	}
}

void CFormulaParser::SkipUnsupportedShankyStyleDelay() {
  int _token_ID = _tokenizer.GetToken();
  assert(_token_ID == kTokenUnsupportedDelay);
  assert(_currently_parsed_function_or_list != NULL);
  if (!_currently_parsed_function_or_list->ImportedFromShankyPPL()) {
    // Not acceptable for OpenPPL
    CParseErrors::Error("Unsupported Shanky-style delay.\n"
      "OpenHoldem provides a far more simple\n"
      "and far more powerful f$delay-function for that.\n");
  }
  _token_ID = _tokenizer.GetToken();
  if (_token_ID != kTokenNumber) {
    CParseErrors::Error("Unexpected token after keyword \"delay\".\n"
      "Expecting a number.\n"
      "But don't worry: Shanky-style delays are not supported anyway."
    );
  }
}

bool CFormulaParser::ExpectKeywordForce(int last_important_roken_ID) {
	int _token_ID = _tokenizer.GetToken();
  if (_token_ID != kTokenKeywordForce) {
    // General error message on missing keyword force
    CParseErrors::Error("Missing keyword FORCE after action.\n");
    return false;
  }
	// Check for unsupported Shanky-style delay
	// which can only happen after actions 
	// WHEN ... RAISEMAX FORCE DELAY 42
	_token_ID = _tokenizer.LookAhead();
  if (_token_ID == kTokenUnsupportedDelay) {
    SkipUnsupportedShankyStyleDelay();
  }
	// Both cases, with and without delay, are considered "good".
	return true;
}

TPParseTreeTerminalNodeBetsizeAction CFormulaParser::ParseOpenPPLRaiseToExpression() {
  // RaiseTo N Force
	// Keyword RaiseTo got already consumed
	TPParseTreeTerminalNodeBetsizeAction action = new CParseTreeTerminalNodeBetsizeAction(_tokenizer.LineRelative());
	TPParseTreeNode expression;
	int _token_ID = _tokenizer.LookAhead();
	if ((_token_ID == kTokenNumber)
		  || (_token_ID == kTokenIdentifier)
		  || TokenIsBracketOpen(_token_ID)) {
		expression = ParseExpression();
	} else {
    CParseErrors::Error("Missing expression after keyword RaiseTo.\n"
      "Expecting the betsize in big blinds.\n");
    return NULL;
  }
  action->MakeRaiseToAction(expression);
  return action;
}

TPParseTreeTerminalNodeBetsizeAction CFormulaParser::ParseOpenPPLRaiseByExpression() {
	// There are 3 possibilities
	//   RAISE <Amount> FORCE
	//   RAISE <PercentagedPot>% FORCE
	//
	// Keyword RAISE got already consumed
  TPParseTreeTerminalNodeBetsizeAction action = new CParseTreeTerminalNodeBetsizeAction(_tokenizer.LineRelative());
	TPParseTreeNode expression;
	int _token_ID = _tokenizer.LookAhead();
	if ((_token_ID == kTokenNumber)
		  || (_token_ID == kTokenIdentifier)
		  || TokenIsBracketOpen(_token_ID)){
		expression = ParseExpression();
	} else {
    CParseErrors::Error("Missing expression after keyword RaiseBy.\n"
      "Expecting the betsize in big blinds or a potsize-expression.\n"
      "Example: WHEN ... RAISEBY 60% FORCE\n");
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

TPParseTreeNode CFormulaParser::ParseOpenPPLRaiseExpression() {
  if (_tokenizer.LookAhead() == kTokenKeywordForce) {
    // Predefined action, here Raise
    TPParseTreeTerminalNodeFixedAction fixed_action
      = new CParseTreeTerminalNodeFixedAction(_tokenizer.LineRelative(),
        TokenString(kTokenActionRaise));
    return fixed_action;
  }
  assert(_currently_parsed_function_or_list != NULL);
  if (!_currently_parsed_function_or_list->ImportedFromShankyPPL()) {
    // Not <cceptable for OpenPPL
    CParseErrors::Error("Missing keyword FORCE after action Raise.\n"
    "Did you attempt to specify a betsize the old Shanky way?\n"
    "Then either use RaiseTo or RaiseBy.\n");
  }
  // Shanky style betsizing, mostly raiseby,
  // might be raiseto preflop (casino-specific)
  return ParseOpenPPLRaiseByExpression();
}

void CFormulaParser::BackPatchOpenEndedWhenConditionSequence(
  TPParseTreeNode first_when_condition_of_a_function) {
  // Backpatching everything after a complete functiuon got parsed
  TPParseTreeNode last_open_ended_when_condition = NULL;
  TPParseTreeNode current_when_condition = first_when_condition_of_a_function;
  // Always holding the last good value, even if the current one is NULL.
  // For handling the end-of-function-node.
  TPParseTreeNode last_when_condition = current_when_condition;
  while (current_when_condition != NULL) {
    last_when_condition = current_when_condition;
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
  // End of when-condition sequence reached
  assert(last_when_condition != NULL);
  assert(last_when_condition->IsAnyKindOfWhenCondition());
  // Insert special node for end of function
  // Either default bot-logic or
  // empty_expression__false__zero__when_others_fold_force
  TPParseTreeTerminalNodeEndOfFunction end_of_function_node = new CParseTreeTerminalNodeEndOfFunction(_tokenizer.LineRelative());
  last_when_condition->_third_sibbling = end_of_function_node;
  if (last_open_ended_when_condition != NULL) {
    if (last_open_ended_when_condition->IsOpenEndedWhenCondition()) {
      last_open_ended_when_condition->_third_sibbling = end_of_function_node;
    }
  }
}

void CFormulaParser::ParseDebugTab(CString function_text) {
  EnterParserCode();
  assert(p_debug_tab != NULL);
  _is_parsing_debug_tab = true;
  p_debug_tab->Clear();
  CString next_line;
  // Split lines
  int line_number = 0;
  while (AfxExtractSubString(next_line, function_text, line_number, '\n')) {
    ++line_number;
    int pos = next_line.Find('=');
    if (pos < 0) {
      // No equality-sign found. Empty line or not valid
      continue;
    }
    // Expression-text: everything behind first "="
    int expresion_length = next_line.GetLength() - pos - 1;
    CString expression_text = next_line.Right(expresion_length);
    // Parse this line
    _tokenizer.SetInputBufferByDebugTab(expression_text, line_number);
    TPParseTreeNode expression = ParseExpression();
    // Care about operator precendence
    _parse_tree_rotator.Rotate(expression, &expression);
    // Add line and expression to debug-tab
    assert(p_debug_tab != NULL);
    p_debug_tab->AddExpression(expression_text, expression);
  }
  _is_parsing_debug_tab = false;
  LeaveParserCode();
}
