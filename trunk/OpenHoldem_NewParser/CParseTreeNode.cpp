#include "stdafx.h"
#include "CParseTreeNode.h"

#include "CEngineContainer.h"
#include "FloatingPoint_Comparisions.h"
#include "NumericalFunctions.h"
#include "OH_MessageBox.h"
#include "TokenizerConstants.h"

CParseTreeNode::CParseTreeNode()
{}

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

void CParseTreeNode::MakeUserVariableDefinition(CString uservariable)
{
	assert(uservariable.Left(4) == "user");
	_node_type = kTokenActionUserVariableToBeSet;
	_terminal_name = uservariable;
	// !!! also needs a continue on execution!
}

double CParseTreeNode::Evaluate()
{
	// Most common types fiorst: numbers and identifiers
	if (_node_type == kTokenNumber)
	{
		return _constant_value;
	}
	else if (_node_type == kTokenIdentifier)
	{
		assert(_terminal_name != "");
		return EvaluateIdentifier(_terminal_name);
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
		// !!! set user variable
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
	// !!! dll$ and more
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

double CParseTreeNode::EvaluateSibbling(TPParseTreeNode first_second_or_third_sibbling)
{
	assert(first_second_or_third_sibbling != NULL);
	double result = first_second_or_third_sibbling->Evaluate();
	return result;
}