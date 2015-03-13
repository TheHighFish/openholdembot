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
#include "CParseTreeNode.h"

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

CParseTreeNode::CParseTreeNode(int relative_line_number) {
  _relative_line_number = relative_line_number;
  _first_sibbling  = NULL;
  _second_sibbling = NULL;
  _third_sibbling  = NULL;
  _terminal_name   = "";
  _constant_value  = 0.0;
}

CParseTreeNode::~CParseTreeNode() {
  delete _first_sibbling;
  _first_sibbling = NULL;
  delete _second_sibbling;
  _second_sibbling = NULL;
  if (!IsOpenEndedWhenCondition()) {
    // Be carefull with open-ended when-conditions.
    // They create graphs, no longer pure trees/
    // The 3rd node is reachable on two paths.
    delete _third_sibbling;
    _third_sibbling = NULL;
  }

}

void CParseTreeNode::MakeConstant(double value)
{
	_node_type = kTokenNumber;
	_constant_value = value;
}

void CParseTreeNode::MakeIdentifier(CString name)
{
	_node_type = kTokenIdentifier;
  assert(name != "");
	_terminal_name = name;
  assert(p_parser_symbol_table != NULL);
  p_parser_symbol_table->VerifySymbol(name);
}

void CParseTreeNode::MakeUnaryOperator(int node_type, TPParseTreeNode first_sibbling)
{
	_node_type = node_type;
	_first_sibbling = first_sibbling;
}

void CParseTreeNode::MakeBinaryOperator(int node_type, TPParseTreeNode first_sibbling,
	TPParseTreeNode second_sibbling)
{
	_node_type = node_type;
	_first_sibbling = first_sibbling;
	_second_sibbling = second_sibbling;
}

void CParseTreeNode::MakeTernaryOperator(int node_type, TPParseTreeNode first_sibbling,
	TPParseTreeNode second_sibbling, TPParseTreeNode third_sibbling)
{
	_node_type = node_type;
	_first_sibbling = first_sibbling;
	_second_sibbling = second_sibbling;
	_third_sibbling = third_sibbling;

}

void CParseTreeNode::MakeAction(int action_constant)
{
	assert(TokenIsOpenPPLAction(action_constant));
  CString action_name = TokenString(action_constant);
  assert(action_name != "");
  MakeIdentifier(action_name);
}

void CParseTreeNode::MakeRaiseToAction(TPParseTreeNode raise_by_amount_in_big_blinds) {
	_node_type = kTokenActionRaiseToBigBlinds;
	_first_sibbling = raise_by_amount_in_big_blinds;
}

void CParseTreeNode::MakeRaiseByAction(TPParseTreeNode raise_by_amount_in_big_blinds) {
	_node_type = kTokenActionRaiseByBigBlinds;
	_first_sibbling = raise_by_amount_in_big_blinds;
}

// Values to be expected in the range (0..100] or more, not (0..1]
void CParseTreeNode::MakeRaiseByPercentagedPotsizeAction(
  	TPParseTreeNode raise_by_amount_percentage) {
	_node_type = kTokenActionRaiseByPercentagedPotsize;
	_first_sibbling = raise_by_amount_percentage;
}

void CParseTreeNode::MakeWhenCondition(TPParseTreeNode condition) {
  _node_type = kTokenOperatorConditionalWhen;
  _first_sibbling = condition;
  // Action and next when-condition or next when-condition
  // and next open-ended when-condition are currently undefined
  // and need to get set later
}

void CParseTreeNode::MakeUserVariableDefinition(CString uservariable)
{
  assert((uservariable.Left(4).MakeLower() == "user")
    || (uservariable.Left(3) == "me_"));
	_node_type = kTokenActionUserVariableToBeSet;
	_terminal_name = uservariable;
}

double CParseTreeNode::Evaluate(bool log /* = false */){
  write_log(MAIN->p_preferences()->debug_formula(), 
    "[CParseTreeNode] Evaluating node type %i %s\n", 
		_node_type, TokenString(_node_type));
  p_autoplayer_trace->SetLastEvaluatedRelativeLineNumber(_relative_line_number);
	// Most common types first: numbers and identifiers
	if (_node_type == kTokenNumber)	{
		write_log(MAIN->p_preferences()->debug_formula(), 
      "[CParseTreeNode] Number evaluates to %6.3f\n",
			_constant_value);
		return _constant_value;
	}	else if (_node_type == kTokenIdentifier) {
    assert(_first_sibbling  == NULL);
    assert(_second_sibbling == NULL);
    assert(_third_sibbling  == NULL);
		assert(_terminal_name != "");
		double value = EvaluateIdentifier(_terminal_name, log);
		write_log(MAIN->p_preferences()->debug_formula(), 
      "[CParseTreeNode] Identifier evaluates to %6.3f\n", value);
    // In case of f$-functions the line changed inbetween,
    // so we have to set it to the current location (again)
    // for the next log.
    p_autoplayer_trace->SetLastEvaluatedRelativeLineNumber(_relative_line_number);
		return value;
	}
	// Actions second, which are also "unary".
	// We have to encode all possible outcomes in a single floating-point,
	// therefore:
	// * positive values mean: raise size (by big-blinds, raise-to-semantics) 
	// * negative values mean: elementary actions
	else if (_node_type == kTokenActionRaiseToBigBlinds)	{
    // RaiseTo N Force
		return EvaluateSibbling(_first_sibbling, log);
	}	else if (_node_type == kTokenActionRaiseByBigBlinds)	{
    // RaiseBy N Force
    double raise_by_amount_in_bblinds = EvaluateSibbling(_first_sibbling, log);
    double final_betsize_in_bblinds = SYM->p_symbol_engine_chip_amounts()->ncallbets()
      + raise_by_amount_in_bblinds;
    write_log(MAIN->p_preferences()->debug_formula(), 
      "[CParseTreeNode] raiseby = %.2f ncallbets = %.2f final = %.2f\n",
      raise_by_amount_in_bblinds,
      SYM->p_symbol_engine_chip_amounts()->ncallbets(),
      final_betsize_in_bblinds);
		return final_betsize_in_bblinds;
	}	else if (_node_type == kTokenActionRaiseByPercentagedPotsize)	{
    // RaiseBy X% Force
		double raise_by_percentage = EvaluateSibbling(_first_sibbling, log);
    assert(SYM->p_symbol_engine_tablelimits()->bet() > 0);
		double pot_size_after_call_in_big_blinds = 
      (SYM->p_symbol_engine_chip_amounts()->pot() / SYM->p_symbol_engine_tablelimits()->bet()) 
      + SYM->p_symbol_engine_chip_amounts()->nbetstocall();
    assert(pot_size_after_call_in_big_blinds >= 0);
		double raise_by_amount_in_bblinds = 0.01 * raise_by_percentage
			* pot_size_after_call_in_big_blinds;
    double final_betsize_in_bblinds = SYM->p_symbol_engine_chip_amounts()->ncallbets()
      + raise_by_amount_in_bblinds;
    write_log(MAIN->p_preferences()->debug_formula(), 
      "[CParseTreeNode] raiseby percentage = %.2f pot after call = %.2f raiseby = %.2f final = %.2f\n",
      raise_by_percentage,
      pot_size_after_call_in_big_blinds,
      raise_by_amount_in_bblinds,
      final_betsize_in_bblinds);
    return final_betsize_in_bblinds;
  }	else if (_node_type == kTokenActionUserVariableToBeSet) {
    // User-variables are a special case of elementary actions
    // Therefore need to be handled first.
    SetUserVariable(_terminal_name);
		return k_undefined_zero;
  } else if (TokenIsElementaryAction(_node_type)) {
		return (0 - _node_type);
  }
	// Finally operators
	else if (TokenIsUnary(_node_type)) {
		return EvaluateUnaryExpression(log);
	}	else if (TokenIsBinary(_node_type))	{
		return EvaluateBinaryExpression(log);
	}	else if (TokenIsTernary(_node_type)) {
		return EvaluateTernaryExpression(log);
	}
	assert(false);
	return k_undefined;
}

CString CParseTreeNode::EvaluateToString(bool log /* = false */) {
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

double CParseTreeNode::EvaluateIdentifier(CString name, bool log) {
	// EvaluateSymbol cares about ALL symbols, 
	// including DLL and PokerTracker.
	double result;
	p_engine_container->EvaluateSymbol(name, &result, log);
	return result;
}

double CParseTreeNode::EvaluateUnaryExpression(bool log_symbol) {
  // Paramater named "log_symbol" instead of "log"
  // due to naming conflict with mathematical function
  assert(_first_sibbling  != NULL);
  assert(_second_sibbling == NULL);
  assert(_third_sibbling  == NULL);
  assert(_terminal_name == "");
  double value_of_first_sibbling = EvaluateSibbling(_first_sibbling, log_symbol);
    switch (_node_type) {
    case kTokenOperatorUnaryMinus:     return (0 - value_of_first_sibbling);
    case kTokenOperatorLog:            return log(value_of_first_sibbling);		
    case kTokenOperatorLogicalNot:     return !value_of_first_sibbling;
    case kTokenOperatorBinaryNot:      return ~ ((unsigned long)value_of_first_sibbling);
    case kTokenOperatorBitCount:       return bitcount((unsigned long)value_of_first_sibbling);
    case kTokenBracketOpen_1: 
    case kTokenBracketOpen_2: 
    case kTokenBracketOpen_3:          return value_of_first_sibbling;
    default: 
	  assert(false);
	  return k_undefined;
  }
}

double CParseTreeNode::EvaluateBinaryExpression(bool log) {
  assert(_first_sibbling  != NULL);
  assert(_second_sibbling != NULL);
  assert(_third_sibbling  == NULL);
  assert(_terminal_name == "");
	double value_of_first_sibbling  = EvaluateSibbling(_first_sibbling, log);
	double value_of_second_sibbling = 0.0;
	// Short circuiting
	// Don't evaluate unnecessary parts of expressions
	if (_node_type == kTokenOperatorLogicalAnd)	{
		if (value_of_first_sibbling == false) {
			return false;
		}
		value_of_second_sibbling = EvaluateSibbling(_second_sibbling, log);
		return (value_of_second_sibbling ? true : false);
	}	else if (_node_type == kTokenOperatorLogicalOr)	{
    // Attention!
    // We can not look here for "value_of_first_sibbling == true"
    // because this way we would only accept true (==1)
    // but we want to accept any non-zero value.
    // http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=17899
		if (value_of_first_sibbling) {
			return true;
		}
		value_of_second_sibbling = EvaluateSibbling(_second_sibbling, log);
		return (value_of_second_sibbling ? true : false);
	}
	// Short circuiting done
	// Now normal evaluation of operators that need both operands
	value_of_second_sibbling = EvaluateSibbling(_second_sibbling, log);
	switch (_node_type) {
	  case kTokenOperatorPlus: 
		  return value_of_first_sibbling + value_of_second_sibbling;
	  case kTokenOperatorMinus: 
		  return value_of_first_sibbling - value_of_second_sibbling;
	  case kTokenOperatorMultiplication: 
		  return value_of_first_sibbling * value_of_second_sibbling;
	  case kTokenOperatorDivision: 
		  if (value_of_second_sibbling == 0) {
			  OH_MessageBox_Error_Warning("Division by zero.");
			  return k_undefined;
		  } else {
			  return value_of_first_sibbling / value_of_second_sibbling;
		  }
	  case kTokenOperatorModulo: 
      if (value_of_second_sibbling == 0) {
			  OH_MessageBox_Error_Warning("Division by zero.");
			  return k_undefined;
		  } else {
			  return (unsigned long)value_of_first_sibbling 
				  % (unsigned long)value_of_second_sibbling;
		  }
	  case kTokenOperatorExponentiation: 
		  return pow(value_of_first_sibbling, value_of_second_sibbling);
	  case kTokenOperatorEquality: 
		  return IsEqual(value_of_first_sibbling, value_of_second_sibbling);
	  case kTokenOperatorApproximatellyEqual: 
		  return IsApproximatellyEqual(value_of_first_sibbling, value_of_second_sibbling);
	  case kTokenOperatorSmaller: 
		  return IsSmaller(value_of_first_sibbling, value_of_second_sibbling);
	  case kTokenOperatorSmallerOrEqual: 
		  return IsSmallerOrEqual(value_of_first_sibbling, value_of_second_sibbling);
	  case kTokenOperatorGreater: 
		  return IsGreater(value_of_first_sibbling, value_of_second_sibbling);
	  case kTokenOperatorGreaterOrEqual: 
		  return IsGreaterOrEqual(value_of_first_sibbling, value_of_second_sibbling);
	  case kTokenOperatorNotEqual: 
	  case kTokenOperatorLogicalXOr: 
		  return value_of_first_sibbling != value_of_second_sibbling;
	  case kTokenOperatorBinaryAnd: 
		  return (unsigned long)value_of_first_sibbling 
			  & (unsigned long)value_of_second_sibbling;
	  case kTokenOperatorBinaryOr: 
		  return (unsigned long)value_of_first_sibbling 
			  | (unsigned long)value_of_second_sibbling;
	  case kTokenOperatorBinaryXOr: 
		  return (unsigned long)value_of_first_sibbling 
			  ^ (unsigned long)value_of_second_sibbling;
	  case kTokenOperatorBitShiftLeft: 
		  return (unsigned long)value_of_first_sibbling 
			  << (unsigned long)value_of_second_sibbling;
	  case kTokenOperatorBitShiftRight: 
		  return (unsigned long)value_of_first_sibbling 
			  >> (unsigned long)value_of_second_sibbling;
	  case kTokenOperatorPercentage: 
		  return value_of_first_sibbling * value_of_second_sibbling * 0.01;
	  default: assert(false);
	}
	return k_undefined;
}

double CParseTreeNode::EvaluateTernaryExpression(bool log) {
	// This function covers both OH-style ternary expressions
	// and OpenPPL-style (open-ended) when-conditions.
	// In case of (OE)WCs the parse-tree-generation assures
	// that _third_sibbling points to the next (OE)WC.
	// Again we use short circuiting.
  assert(_first_sibbling  != NULL);
  // Both second and thirs sibbling can be zero in case of an OEWC.
  // So no assertion here.
  // We handle this case gracefully in EvaluateSibbling().
  assert(_terminal_name == "");
	assert((_node_type == kTokenOperatorConditionalIf)
		  || (_node_type == kTokenOperatorConditionalWhen));
	double value_of_first_sibbling = EvaluateSibbling(_first_sibbling, log);
	if (value_of_first_sibbling) {
		double value_of_second_sibbling = EvaluateSibbling(_second_sibbling, log);
    // Special behaviour for user-variables:
    // we have to set them, but then continue with the next when-condition 
    // (third sibbling)
    if (!SecondSibblingIsUserVariableToBeSet()) {
      // Normal behaviour: return the evaluated result
      return value_of_second_sibbling;
    }
    // Uvervariables: will fall through to the evaluation 
    // of the third sibbling...
	}
	double value_of_third_sibbling = EvaluateSibbling(_third_sibbling, log);
	return value_of_third_sibbling;
}

double CParseTreeNode::EvaluateSibbling(
  TPParseTreeNode first_second_or_third_sibbling, bool log) {
  // We allow NULL-nodes here, because that can happen 
  // after the end of a sequence of when-conditions
  if (first_second_or_third_sibbling == NULL) {
    return k_undefined_zero;
  }
  double result = first_second_or_third_sibbling->Evaluate(log);
  return result;
}

TPParseTreeNode CParseTreeNode::GetRightMostSibbling() {
  if (TokenIsTernary(_node_type)) {
    return _third_sibbling;
  } else if (TokenIsBinary(_node_type)) {
    return _second_sibbling;
  } else if (TokenIsUnary(_node_type)) {
    return _first_sibbling;
  } else {
    // Not an operator
    return NULL;
  }
}

TPParseTreeNode CParseTreeNode::GetLeftMostSibbling() {
  if (TokenIsUnary(_node_type)
      || TokenIsBinary(_node_type)
      || TokenIsTernary(_node_type)) {
    return _first_sibbling;
  }
  // Not an operator
   return NULL;
}

void CParseTreeNode::SetRightMostSibbling(TPParseTreeNode sibbling){
  if (TokenIsTernary(_node_type)) {
   _third_sibbling = sibbling;
  } else if (TokenIsBinary(_node_type)) {
    _second_sibbling = sibbling;
  } else {
    // Default: first one is always present
    _first_sibbling = sibbling;
  }
}

void CParseTreeNode::SetLeftMostSibbling(TPParseTreeNode sibbling){
  _first_sibbling = sibbling;
}

CString CParseTreeNode::Serialize()
{
  if (_node_type == kTokenIdentifier) {
    return _terminal_name;
  } else if (_node_type == kTokenNumber) {
    return Number2CString(_constant_value);
  } else if (TokenIsBracketOpen(_node_type)) {
    return ("(" + _first_sibbling->Serialize() + ")");
  } else if (TokenIsUnary(_node_type)) {
    assert(_first_sibbling != NULL);
    return TokenString(_node_type) + "(" + _first_sibbling->Serialize() + ")";
  } else if (TokenIsBinary(_node_type)) {
    assert(_first_sibbling != NULL);
    assert(_second_sibbling != NULL);
    return "(" + _first_sibbling->Serialize() + " "
      + TokenString(_node_type) + " " + _second_sibbling->Serialize() + ")";
  } else if (_node_type == kTokenOperatorConditionalIf) {
    assert(_first_sibbling != NULL);
    assert(_second_sibbling != NULL);
    assert(_third_sibbling != NULL);  
    return "(" + _first_sibbling->Serialize() + " ? "
      + _second_sibbling->Serialize() + " : "
      + _third_sibbling->Serialize() + ")";
  } else if (IsOpenEndedWhenCondition()) {
    return "WHEN: " + _first_sibbling->Serialize() + "\n"
      + (_second_sibbling? _second_sibbling->Serialize(): "")
      // No third sibbling to serialize, because this is the next open-ender
      // and TWO pointers point to it (one from a normal "when")
      + "WEND";
  } else if (IsWhenConditionWithAction()) {
    return "    WHEN: " + _first_sibbling->Serialize() + "WRETURN: "
      + (_second_sibbling? _second_sibbling->Serialize(): "") + "\n"
      // Third sibbling: either next when-condition or next open-ended when-condition
      + (_third_sibbling? _third_sibbling->Serialize(): "");
  } else {
    // Unhandled note-type, probably new and therefore not yet handled
    write_log(k_always_log_errors, "[CParseTreeNode] ERROR: Unhandled node-tzpe %i in serialiyation of parse-tree\n",
      _node_type);
    return "";
  }
}

bool CParseTreeNode::IsAnyKindOfWhenCondition() {
  return (_node_type == kTokenOperatorConditionalWhen);
}

bool CParseTreeNode::IsWhenConditionWithAction() {
  return (IsAnyKindOfWhenCondition() && !IsOpenEndedWhenCondition());
}

bool CParseTreeNode::IsOpenEndedWhenCondition() {
  if (!IsAnyKindOfWhenCondition()) return false;
  if ((_second_sibbling == NULL) && (_third_sibbling == NULL)) return true; // ?????
  if ((_second_sibbling != NULL) 
      && (_second_sibbling->_node_type == kTokenOperatorConditionalWhen)) {
    return true;
  }
  return false;
}

bool CParseTreeNode::IsBinaryIdentifier() {
  const int kNumberOfElementaryBinaryIdentifiers = 21;
  static const char* kElementaryBinaryIdentifiers[kNumberOfElementaryBinaryIdentifiers] = {
    "pcbits",              "playersactivebits",  "playersdealtbits",
    "playersplayingbits",  "playersblindbits",   "opponentsseatedbits",
    "opponentsactivebits", "opponentsdealtbits", "opponentsplayingbits",
    "opponentsblindbits",  "flabitgs",           "rankbits",
    "rankbitscommon",      "rankbitsplayer",     "rankbitspoker",
    "srankbits",           "srankbitscommon",    "srankbitsplayer",
    "srankbitspoker",      "myturnbits",         "pcbits"};
  const int kNumberOfParameterizedBinaryIdentifiers = 4;
  static const char* kParameterizedBinaryIdentifiers[kNumberOfParameterizedBinaryIdentifiers] = {
    "chairbit$", "raisbits", "callbits", "foldbits"};

  if (_node_type != kTokenIdentifier) return false;
  assert(_terminal_name != "");
  // Check elementary binary identifiers first
  for (int i=0; i<kNumberOfElementaryBinaryIdentifiers; ++i) {
    if (_terminal_name == kElementaryBinaryIdentifiers[i]) return true;
  }
  // Then check parameterized binary symbols
  for (int i=0; i<kNumberOfParameterizedBinaryIdentifiers; ++i) {
    if (StringAIsPrefixOfStringB(kParameterizedBinaryIdentifiers[i],
        _terminal_name)) {
      return true;
    }                                 
  }
  // Not a binary identifier
  return false;
}

bool CParseTreeNode::SecondSibblingIsUserVariableToBeSet() {
  if (_second_sibbling == NULL) return false;
  return (_second_sibbling->_node_type == kTokenActionUserVariableToBeSet);
}

void CParseTreeNode::SetUserVariable(CString name) {
  if (name.Left(4).MakeLower() == "user") {   
    SYM->p_symbol_engine_openppl_user_variables()->Set(name);
  } else if (name.Left(3) == "me_") {
    double temp_result;
    SYM->p_symbol_engine_memory_symbols()->EvaluateSymbol(name, 
      &temp_result, true);
  }
  else {
    assert(k_this_must_not_happen);
  }
}

bool CParseTreeNode::EvaluatesToBinaryNumber() {
  if (TokenEvaluatesToBinaryNumber(_node_type)) {
    // Operation that evaluates to binary number,
    // e.g. bit-shift, logical and, etc.
    return true;
  }
  else if (TokenIsBracketOpen(_node_type)) {
    // Have a look at the sub-extreesopn
    TPParseTreeNode sub_expression = _first_sibbling;
    // There has to be an expresison inside the brackets
    assert (_first_sibbling != NULL);
    return _first_sibbling->EvaluatesToBinaryNumber();
  }
  else if (IsBinaryIdentifier()) return true;
  else if ((_node_type == kTokenIdentifier)
      && SYM->p_function_collection()->EvaluatesToBinaryNumber(_terminal_name)) {
    return true;
  }
  // Nothing binary
  return false;
}
