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
#include "TokenizerConstants.h"

#include <assert.h>

const bool kTokenIsUnary[kNumberOfTokens] =
{
	// kTokenEndOfFile = 0,
	false,
	// kTokenEndOfFunction,
	false,
	// kTokenOperatorPlus,               
	false,
	// kTokenOperatorMinus,              
	false,
	// kTokenOperatorMultiplication,     
	false,
	// kTokenOperatorDivision,           
	false,
	// kTokenOperatorModulo,             
	false,
	// kTokenOperatorExponentiation,     
	false,
	// kTokenOperatorLog,                
	false,
	// kTokenOperatorEquality,           
	false,
	// kTokenOperatorApproximatellyEqual,
	false,
	// kTokenOperatorSmaller,            
	false,
	// kTokenOperatorSmallerOrEqual,     
	false,
	// kTokenOperatorGreater,           
	false,
	// kTokenOperatorGreaterOrEqual,     
	false,
	// kTokenOperatorNotEqual,           
	false,
	// kTokenOperatorLogicalAnd,        
	false,
	// kTokenOperatorLogicalOr,	    
	false,
	// kTokenOperatorLogicalNot,        
	true,
	// kTokenOperatorLogicalXOr,       
	false,
	// kTokenOperatorBinaryAnd,          
	false,
	// kTokenOperatorBinaryOr,           
	false,
	// kTokenOperatorBinaryNot,          
	true,
	// kTokenOperatorBinaryXOr,          
	false,
	// kTokenOperatorBitShiftLeft,      
	false,
	// kTokenOperatorBitShiftRight,     
	false,
	// kTokenOperatorBitCount,
	true,
	// kTokenOperatorPercentage,         
	false,
	// kTokenOperatorConditionalIf,      
	false,
	// kTokenOperatorConditionalThen,    
	false,
	// kTokenOperatorConditionalWhen,    
	false,
	// kTokenBracketOpen_1,              			
	false,
	// kTokenBracketOpen_2,              
	false,
	// kTokenBracketOpen_3,              
	false,
	// kTokenBracketClose_1,            
	false,
	// kTokenBracketClose_2,            
	false,
	// kTokenBracketClose_3,           
	false,
	// kTokenIdentifier,
	false,
	// kTokenNumber,
	false,
	// kTokenPocketCards,
	false,
	// kTokenDoubleShebang,
	false,
};

const bool kTokenIsBinary[kNumberOfTokens] =
{
	// kTokenEndOfFile = 0,
	false,
	// kTokenEndOfFunction,
	false,
	// kTokenOperatorPlus,               
	true,
	// kTokenOperatorMinus, 
	true,
	// kTokenOperatorMultiplication,     
	true,
	// kTokenOperatorDivision,           
	true,
	// kTokenOperatorModulo,             
	true,
	// kTokenOperatorExponentiation,     
	true,
	// kTokenOperatorLog,                
	true,
	// kTokenOperatorEquality,           
	true,
	// kTokenOperatorApproximatellyEqual,
	true,
	// kTokenOperatorSmaller,            
	true,
	// kTokenOperatorSmallerOrEqual,     
	true,
	// kTokenOperatorGreater,           
	true,
	// kTokenOperatorGreaterOrEqual,     
	true,
	// kTokenOperatorNotEqual,           
	true,
	// kTokenOperatorLogicalAnd,        
	true,
	// kTokenOperatorLogicalOr,	    
	true,
	// kTokenOperatorLogicalNot,        
	false,
	// kTokenOperatorLogicalXOr,       
	true,
	// kTokenOperatorBinaryAnd,          
	true,
	// kTokenOperatorBinaryOr,           
	true,
	// kTokenOperatorBinaryNot,          
	false,
	// kTokenOperatorBinaryXOr,          
	true,
	// kTokenOperatorBitShiftLeft,      
	true,
	// kTokenOperatorBitShiftRight,     
	true,
	// kTokenOperatorBitCount,
	false,
	// kTokenOperatorPercentage,         
	true,
	// kTokenOperatorConditionalIf,      
	false,
	// kTokenOperatorConditionalThen,    
	false,
	// kTokenOperatorConditionalWhen,    
	true, //??
	// kTokenBracketOpen_1,              			
	false,
	// kTokenBracketOpen_2,              
	false,
	// kTokenBracketOpen_3,              
	false,
	// kTokenBracketClose_1,            
	false,
	// kTokenBracketClose_2,            
	false,
	// kTokenBracketClose_3,           
	false,
	// kTokenIdentifier,
	false,
	// kTokenNumber,
	false,
	// kTokenPocketCards,
	false,
	// kTokenDoubleShebang,
	false,
};

bool TokenIsUnary(int token)
{
	assert(token >=0);
	assert(token < kNumberOfTokens);
	return kTokenIsUnary[token];
}

bool TokenIsBinary(int token)
{
	assert(token >=0);
	assert(token < kNumberOfTokens);
	return kTokenIsBinary[token];
}

CString TokenString(int token)
{
	switch (token)
	{
	case kTokenEndOfFile: return "End of file";
	case kTokenEndOfFunction: return "End of function";
	case kTokenOperatorPlus: return "+";               
	case kTokenOperatorMinus: return "-";
	case kTokenOperatorMultiplication: return "*";    
	case kTokenOperatorDivision: return "/";
	case kTokenOperatorModulo: return "modulo";	// because % can also mean percentage
	case kTokenOperatorExponentiation: return "**";
	case kTokenOperatorLog: return "ln";
	case kTokenOperatorEquality: return "=="; 
	case kTokenOperatorApproximatellyEqual: return "~~";
	case kTokenOperatorSmaller: return "<";     
	case kTokenOperatorSmallerOrEqual: return "<=";
	case kTokenOperatorGreater: return ">";     
	case kTokenOperatorGreaterOrEqual: return ">=";
	case kTokenOperatorNotEqual: return "!=";           
	case kTokenOperatorLogicalAnd: return "&&";        
	case kTokenOperatorLogicalOr: return "||";	    
	case kTokenOperatorLogicalNot: return "!";        
	case kTokenOperatorLogicalXOr: return "^^"; 
	case kTokenOperatorBinaryAnd: return "&";
	case kTokenOperatorBinaryOr: return "|";           
	case kTokenOperatorBinaryNot: return "~";          
	case kTokenOperatorBinaryXOr: return "^";    
	case kTokenOperatorBitShiftLeft: return "<<";      
	case kTokenOperatorBitShiftRight: return ">>";
	case kTokenOperatorPercentage: return "percentage"; // because % can also mean modulo      
	case kTokenOperatorConditionalIf: return "?";     
	case kTokenOperatorConditionalElse: return "";    
	case kTokenOperatorConditionalWhen: return ":";
	case kTokenBracketOpen_1: return "(";              			
	case kTokenBracketOpen_2: return "[";              
	case kTokenBracketOpen_3: return "{";              
	case kTokenBracketClose_1: return ")";            
	case kTokenBracketClose_2: return "]";            
	case kTokenBracketClose_3: return "}";
	case kTokenIdentifier: return "identifier";
	case kTokenNumber: return "number";
	case kTokenPocketCards: return "cards";
	case kTokenDoubleShebang: return "##";
	default:
		assert(k_this_must_not_happen);
		return "Error: invalid token ID";
	}
}

/*
kTokenEndOfFile = 0,
	kTokenEndOfFunction,
	kTokenOperatorPlus,               
	kTokenOperatorMinus,              
	kTokenOperatorMultiplication,     
	kTokenOperatorDivision,           
	kTokenOperatorModulo,             
	kTokenOperatorExponentiation,     
	kTokenOperatorLog,                
	kTokenOperatorEquality,           
	kTokenOperatorApproximatellyEqual,
	kTokenOperatorSmaller,            
	kTokenOperatorSmallerOrEqual,     
	kTokenOperatorGreater,           
	kTokenOperatorGreaterOrEqual,     
	kTokenOperatorNotEqual,           
	kTokenOperatorLogicalAnd,        
	kTokenOperatorLogicalOr,	    
	kTokenOperatorLogicalNot,        
	kTokenOperatorLogicalXOr,       
	kTokenOperatorBinaryAnd,          
	kTokenOperatorBinaryOr,           
	kTokenOperatorBinaryNot,          
	kTokenOperatorBinaryXOr,          
	kTokenOperatorBitShiftLeft,      
	kTokenOperatorBitShiftRight,     
	kTokenOperatorPercentage,         
	kTokenOperatorConditionalIf,      
	kTokenOperatorConditionalThen,    
	kTokenOperatorConditionalWhen,    
	kTokenBracketOpen_1,              			
	kTokenBracketOpen_2,              
	kTokenBracketOpen_3,              
	kTokenBracketClose_1,            
	kTokenBracketClose_2,            
	kTokenBracketClose_3,           
	kTokenIdentifier,
	kTokenNumber,
	kTokenPocketCards,
	kTokenDoubleShebang,
	*/