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
#include "CParseTreeNode.h"

#include <math.h>
#include "CAutoplayerTrace.h"
#include "CEngineContainer.h"
#include "CPreferences.h"
#include "CSymbolEngineOpenPPLUserVariables.h"
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

CParseTreeNode::~CParseTreeNode()
{}

void CParseTreeNode::MakeConstant(double value)
{
	_node_type = kTokenNumber;
	_constant_value = value;
}

void CParseTreeNode::MakeIdentifier(CString name)
{
	_node_type = kTokenIdentifier;
	_terminal_name = name;
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
	_node_type = action_constant;
}

void CParseTreeNode::MakeRaiseByAction(TPParseTreeNode raise_by_amount_in_big_blinds)
{
	_node_type = kTokenActionRaiseByBigBlinds;
	_first_sibbling = raise_by_amount_in_big_blinds;
}

// Values to be expected in the range (0..100] or more, not (0..1]
void CParseTreeNode::MakeRaiseByPercentagedPotsizeAction(
	TPParseTreeNode raise_by_amount_percentage)
{
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
	assert(uservariable.Left(4) == "user");
	_node_type = kTokenActionUserVariableToBeSet;
	_terminal_name = uservariable;
	// !!! also needs a continue on execution!
}

double CParseTreeNode::Evaluate()
{
    write_log(preferences.debug_formula(), 
        "[CParseTreeNode] Evaluating node type %i %s\n", 
		_node_type, TokenString(_node_type));
    p_autoplayer_trace->SetLastEvaluatedRelativeLineNumber(_relative_line_number);
	// Most common types fiorst: numbers and identifiers
	if (_node_type == kTokenNumber)
	{
		write_log(preferences.debug_formula(), 
            "[CParseTreeNode] Number evaluates to %6.3f\n",
			_constant_value);
		return _constant_value;
	}
	else if (_node_type == kTokenIdentifier)
	{
		assert(_terminal_name != "");
		double value = EvaluateIdentifier(_terminal_name);
		write_log(preferences.debug_formula(), 
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
	// * positive values mean: raise size (by big-blinds, raise-by-semantics)
	// * negative values mean: elementary actions
	else if (TokenIsElementaryAction(_node_type))
	{
		return (0 - _node_type);
	}
	else if (_node_type == kTokenActionRaiseByBigBlinds)
	{
		return EvaluateSibbling(_first_sibbling);
	}
	else if (_node_type == kTokenActionRaiseByPercentagedPotsize)
	{
		double raise_by_percentage = EvaluateSibbling(_first_sibbling);
		double pot_size_after_call_in_big_blinds = 0; // !! PotSize() + AmountToCall();
		double raise_by_amount = 0.01 * raise_by_percentage
			* pot_size_after_call_in_big_blinds;
		return raise_by_amount;
	}
	else if (_node_type == kTokenActionUserVariableToBeSet)
	{
		p_symbol_engine_openppl_user_variables->Set(_terminal_name);
		// Continue with next open-ended when-condition
		EvaluateSibbling(_second_sibbling);
	}
	// Finally operators
	else if (TokenIsUnary(_node_type))
	{
		return EvaluateUnaryExpression();
	}
	else if (TokenIsBinary(_node_type))
	{
		return EvaluateBinaryExpression();
	}
	else if (TokenIsTernary(_node_type))
	{
		return EvaluateTernaryExpression();
	}
	assert(false);
	return k_undefined;
}

double CParseTreeNode::EvaluateIdentifier(CString name)
{
	assert(name != "");
	// EvaluateSymbol cares about ALL symbols, 
	// including DLL, PokerTracker and Perl
	double result;
	p_engine_container->EvaluateSymbol(name, &result);
	return result;
}

double CParseTreeNode::EvaluateUnaryExpression()
{
	double value_of_first_sibbling = EvaluateSibbling(_first_sibbling);
	switch (_node_type)
	{
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

double CParseTreeNode::EvaluateBinaryExpression()
{
	double value_of_first_sibbling  = EvaluateSibbling(_first_sibbling);
	double value_of_second_sibbling = 0.0;
	// Short circuiting
	// Don't evaluate unnecessary parts of expressions
	if (_node_type == kTokenOperatorLogicalAnd)
	{
		if (value_of_first_sibbling == false)
		{
			return false;
		}
		value_of_second_sibbling = EvaluateSibbling(_second_sibbling);
		return value_of_second_sibbling;
	}
	else if (_node_type == kTokenOperatorLogicalOr)
	{
		if (value_of_first_sibbling == true)
		{
			return true;
		}
		value_of_second_sibbling = EvaluateSibbling(_second_sibbling);
		return value_of_second_sibbling;
	}
	// Short circuiting done
	// Now normal evaluation of operators that need both operands
	value_of_second_sibbling = EvaluateSibbling(_second_sibbling);
	switch (_node_type)
	{
	case kTokenOperatorPlus: 
		return value_of_first_sibbling + value_of_second_sibbling;
	case kTokenOperatorMinus: 
		return value_of_first_sibbling - value_of_second_sibbling;
	case kTokenOperatorMultiplication: 
		return value_of_first_sibbling * value_of_second_sibbling;
	case kTokenOperatorDivision: 
		if (value_of_second_sibbling == 0)
		{
			OH_MessageBox_Error_Warning("Division by zero.", "Error");
			return k_undefined;
		}
		else
		{
			return value_of_first_sibbling / value_of_second_sibbling;
		}
	case kTokenOperatorModulo: if (value_of_second_sibbling == 0)
		{
			OH_MessageBox_Error_Warning("Division by zero.", "Error");
			return k_undefined;
		}
		else
		{
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

double CParseTreeNode::EvaluateTernaryExpression()
{
	// This function covers both OH-style ternary expressions
	// and OpenPPL-style (open-ended) when-conditions.
	// In case of (OE)WCs the parse-tree-generation assures
	// that _third_sibbling points to the next (OE)WC.
	// Again we use short circuiting.
	assert((_node_type == kTokenOperatorConditionalIf)
		|| (_node_type == kTokenOperatorConditionalWhen));
	double value_of_first_sibbling  = EvaluateSibbling(_first_sibbling);
	if (value_of_first_sibbling)
	{
		double value_of_second_sibbling  = EvaluateSibbling(_second_sibbling);
		return value_of_second_sibbling;
	}
	else
	{
		double value_of_third_sibbling  = EvaluateSibbling(_third_sibbling);
		return value_of_third_sibbling;
	}
}

double CParseTreeNode::EvaluateSibbling(TPParseTreeNode first_second_or_third_sibbling) {
  // We allow NULL-nodes here, because that can happen 
  // after the end of a sequence of when-conditions
  if (first_second_or_third_sibbling == NULL) {
    return k_undefined_zero;
  }
  double result = first_second_or_third_sibbling->Evaluate();
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
    assert(false);
    return "ERROR";
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
  if ((_second_sibbling == NULL) && (_third_sibbling == NULL)) return true;
  if ((_second_sibbling != NULL) 
      && (_second_sibbling->_node_type == kTokenOperatorConditionalWhen)) {
    return true;
  }
  return false;
}
