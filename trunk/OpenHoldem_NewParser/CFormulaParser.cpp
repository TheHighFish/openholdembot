#include "stdafx.h"
#include "CFormulaParser.h"

#include <assert.h>
#include "CFunction.h"
#include "CFunctionCollection.h"
#include "CParseErrors.h"
#include "TokenizerConstants.h"

// Global for static accessor-function
CString current_function_name;

CFormulaParser::CFormulaParser()
{}

CFormulaParser::~CFormulaParser()
{}

void CFormulaParser::ParseFile(CArchive& formula_file)
{
	while (true)
	{
		CString function_content = _formula_file_splitter.GetNextFunctionOrList(formula_file);
		if (function_content.GetLength() < 2)
		{
#ifdef _DEBUG
			//printf("Empty function received. Terminating.\n");
#endif
			goto ExitLoop;
		}
		ParseSingleFormula(function_content);
	}
ExitLoop:;
}

void CFormulaParser::ExpectDoubleShebangAsStartOfFunctionHeader()
{
	int _token_ID = _tokenizer.GetToken();
	
	if (_token_ID == kTokenIdentifier)
	{
		// Most common error: identifier instead of function header
		// This can only happen before the first function
		_token = _tokenizer.GetTokenString();
		//printf("Found %s\n", _token);
		_token = _token.MakeLower();
		if (_token.Left(6) == "custom")
		{
			CParseErrors::Error("Superfluous keyword custom");
		}
		else if ((_token.Left(7) == "preflop")
			|| (_token.Left(4) == "flop")
			|| (_token.Left(4) == "turn")
			|| (_token.Left(5) == "river"))
		{
			CParseErrors::Error("Shanky-style betrounds. OpenHoldem-style ##f$function## expected");
		}
		else if (_token.Left(3) == "new")
		{
			CParseErrors::Error("Old-style OpenPPL function. OpenHoldem-style ##f$function## expected");
		}
		else
		{
			CParseErrors::Error("Shanky-style option settings?");
		}
	}
	else if (_token_ID != kTokenDoubleShebang)
	{
		// Other errors
		CParseErrors::Error("Expecting a ##f$function## or ##listNNN##");
	}
}

void CFormulaParser::ExpectDoubleShebangAsEndOfFunctionHeader()
{
	_token_ID = _tokenizer.GetToken();
	if (_token_ID != kTokenDoubleShebang)
	{
		CParseErrors::Error("Malformed function-header. ## expected");
	}
	// !! Terminate parsing
}

void CFormulaParser::ExpectConditionalThen()
{
	_token_ID = _tokenizer.GetToken();
	if (_token_ID != kTokenOperatorConditionalElse)
	{
		CParseErrors::Error("Malformed conditional expression. \":\" expected, but this could also be a missing operator or wrong bracket.");
	}
}

void CFormulaParser::CheckForExtraTokensAfterEndOfFunction()
{
	_token_ID = _tokenizer.GetToken();
	if ((_token_ID != kTokenEndOfFile)
		&& (_token_ID != kTokenEndOfFunction))
	{
		CParseErrors::Error("Unexpected token(s) after end of function");
	}
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
	CParseErrors::Error("Expecting closing bracket"); //!! of another type
}

void CFormulaParser::ParseSingleFormula(CString function_content)
{
	current_function_name = "-- undefined --";
	_tokenizer.SetInput(function_content);

	// Check for empty function
	_token_ID = _tokenizer.GetToken();
	if ((_token_ID == kTokenEndOfFile)
		|| (_token_ID == kTokenEndOfFunction))
	{
		// Parsing finished (for this function or altogether)
		return;
	}
	else
	{
		_tokenizer.PushBack();
	}
	// Check correct start of function header 
	// and especially throw warnings on old-style and Shanky-style PPL.
	ExpectDoubleShebangAsStartOfFunctionHeader();
	_token_ID = _tokenizer.GetToken();
	if (_token_ID == kTokenNumber)
	{
		// Date like ##2014-02-09 23:16:55##
		// To be completely ignored
		// We don't need it and on saving we create a new one
		//printf("Found a date. To be ignored.\n");
		return;
	}
	else if (_token_ID != kTokenIdentifier)
	{
		CParseErrors::Error("Malformed function-header. Name expected");
		return;
	}
	_token = _tokenizer.GetTokenString();
	current_function_name = _token;
	TPParseTreeNode function_body = NULL;
	if (_token.Left(2) == "f$")
	{
		// ##f$functionXYZ##
		//printf("Parsing f$function\n");
		ExpectDoubleShebangAsEndOfFunctionHeader();
		TPParseTreeNode function_body =	ParseFunctionBody();

	}
	else if (_token.Left(4) == "list")
	{
		// ##listNNN##
		//printf("Parsing list\n");
		ExpectDoubleShebangAsEndOfFunctionHeader();
		ParseListBody();
	}
	else if (_token.MakeLower() == "dll")
	{
		// ##DLL##
		//printf("Parsing DLL\n");
		ExpectDoubleShebangAsEndOfFunctionHeader();
		// !!! To do
	}
	else if (_token.MakeLower() == "notes")
	{
		// ##Notes##
		//printf("Parsing Notes\n");
		ExpectDoubleShebangAsEndOfFunctionHeader();
		// Don't do anything.
		// This is just a special type of global comment.
	}
	else
	{
		CParseErrors::Error("Found unknown function type. Did you forget f$?\n");
		return;
	}
	CheckForExtraTokensAfterEndOfFunction();
	CFunction *p_new_function = new CFunction(current_function_name, "function text!!!");
	p_new_function->SetParseTree(function_body);
	p_function_collection->Add(p_new_function);
}

void CFormulaParser::ParseListBody()
{
	_token_ID = _tokenizer.GetToken();
	while (_token_ID != kTokenEndOfFunction)
	{
		if ((_token_ID == kTokenIdentifier)      // High cards (at least one), like AK2 T2o
			|| (_token_ID == kTokenNumber)       // Low pairs 99..22
			|| (_token_ID == kTokenPocketCards)) // Low unpaired cards like 65s, 92o
		{
			_token = _tokenizer.GetTokenString();
			//printf("Adding to list: %s\n", _token);
		}
		else
		{
			CParseErrors::Error("Unexpected token inside list");
		}
		_token_ID = _tokenizer.GetToken();
	}
}

TPParseTreeNode CFormulaParser::ParseFunctionBody()
{
	// Just look-ahead 1 token
	_token_ID = _tokenizer.LookAhead();
	if ((_token_ID == kTokenEndOfFile) 
		|| (_token_ID == kTokenEndOfFunction))
	{
		CParseErrors::Error("Function is empty");
		return NULL;
	}
	if (_token_ID == kTokenOperatorConditionalWhen)
	{
		// OpenPPL-function
		/// !!!ParseOpenEndedWhenConditionSequence();
	}
	else
	{	
		// OH-script-function, single expression
		return ParseExpression();
	}

}

TPParseTreeNode CFormulaParser::ParseExpression()
{
	_token_ID = _tokenizer.LookAhead();
	if (TokenIsUnary(_token_ID))
	{
		ParseUnaryExpression();
	}
	else if (TokenIsBracketOpen(_token_ID))
	{
		ParseBracketExpression();
	}
	else if ((_token_ID == kTokenIdentifier)
		|| (_token_ID == kTokenNumber))
	{
		ParseSimpleExpression();
	}
	else
	{
		CParseErrors::Error("Unexpected token inside expression. Expect: Opening Bracket, Unary, Identifier or number");
	}
	_token_ID = _tokenizer.LookAhead();
	if (TokenIsBinary(_token_ID) 
		|| (_token_ID == kTokenOperatorConditionalIf))
	{
		ParseRightPartOfBinaryOrTernaryExpression();
	}
}

void CFormulaParser::ParseBracketExpression()
{
	//MessageBox(0, "Bracket_Open", "Info", 0);
	int opening_bracket = _tokenizer.GetToken();
	assert(TokenIsBracketOpen(opening_bracket));
	TPParseTreeNode expression = ParseExpression();
	ExpectMatchingBracketClose(opening_bracket);
	TPParseTreeNode bracket_node = new CParseTreeNode();
	// Brackets get an unary node in the tree
	// This will lead to a simple way to handle precedence of operators.
	bracket_node->MakeUnaryOperator(opening_bracket, expression);
	return bracket_node;
}

TPParseTreeNode CFormulaParser::ParseUnaryExpression()
{
	//MessageBox(0, "Unary", "Info", 0);
	int unary_operator = _tokenizer.GetToken();
	assert(TokenIsUnary(unary_operator));
	TPParseTreeNode expression = ParseExpression();
	TPParseTreeNode unary_node = new CParseTreeNode();
	unary_node->MakeUnaryOperator(unary_operator, expression);
	return unary_node;
}

TPParseTreeNode CFormulaParser::ParseSimpleExpression()
{
	MessageBox(0, "Terminal", "Info", 0); 
	int terminal = _tokenizer.GetToken();
	assert((terminal == kTokenIdentifier) || (terminal == kTokenNumber));
	TPParseTreeNode terminal_node = new CParseTreeNode();
	if (terminal == kTokenIdentifier)
	{
		terminal_node->MakeIdentifier(_tokenizer.GetTokenString());
	}
	else if (terminal == kTokenNumber)
	{
		CString number = _tokenizer.GetTokenString();
		double value = atof(number);
		terminal_node->MakeConstant(value);
	}
	else
	{
		assert(k_this_must_not_happen);
		terminal_node = NULL;	
	}
	return terminal_node;
}

void CFormulaParser::ParseRightPartOfBinaryOrTernaryExpression()
{
	int token_ID = _tokenizer.GetToken();
	if (TokenIsBinary(token_ID))
	{
		//MessageBox(0, "Binary", "Info", 0);
		ParseExpression();
	}
	else if (token_ID == kTokenOperatorConditionalIf)
	{
		//MessageBox(0, "Ternary", "Info", 0);
		ParseConditionalThenElseExpressions();
	}
	else
	{
		_tokenizer.PushBack();
	}
}

void CFormulaParser::ParseConditionalThenElseExpressions()
{
	// <Condition> ? <Then-Expression> : <Else-Expression>
	// Condition up to question-mark already parsed
	ParseExpression();
	ExpectConditionalThen();
	//MessageBox(0, "Ternary Then", "Info", 0);
	ParseExpression();
}

CString CFormulaParser::CurrentFunctionName()
{
	return current_function_name;
}

void CFormulaParser::ParseOpenEndedWhenConditionSequence()
{
	//MessageBox(0, "When", "Info", 0);
	_token_ID = _tokenizer.GetToken();
	assert(_token_ID == kTokenOperatorConditionalWhen);
	ParseExpression();
	// Next either:
	// * another when-condition
	// * action
	// * user-variable to be set
	_token_ID = _tokenizer.LookAhead();
	if (_token_ID == kTokenOperatorConditionalWhen)
	{
		ParseOpenEndedWhenConditionSequence();
	}
	else if (TokenIsOpenPPLAction(_token_ID))
	{
		ParseOpenPPLAction();
	}
	else if (false)
	{
		// 
	}
	else
	{
		CParseErrors::Error("Missing action after when condition");
	}
	_token_ID = _tokenizer.LookAhead();
	if (_token_ID == kTokenOperatorConditionalWhen)
	{
		ParseOpenEndedWhenConditionSequence();
	}
}

void CFormulaParser::ParseOpenPPLAction()
{
	//MessageBox(0, "Action", "Info", 0);
	_token_ID = _tokenizer.GetToken();
	assert(TokenIsOpenPPLAction(_token_ID));
	if (_token_ID == kTokenActionReturn)
	{
		// RETURN <Expression> FORCE
		ParseExpression();
	}
	else if (_token_ID == kTokenActionRaise)
	{
		// There are 3 possibilities
		//   RAISE FORCE
		//   RAISE <Amount> FORCE
		//   RAISE <PercentagedPot>% FORCE
		ParseOpenPPLRaiseExpression();
	}
	ExpectKeywordForce();
}

void CFormulaParser::ExpectKeywordForce()
{
	int _token_ID = _tokenizer.GetToken();
	if (_token_ID == kTokenKeywordForce)
	{
		// Check for unsupported Shanky-style delay
		// which can only happen after actions 
		// WHEN ... RAISEMAX FORCE DELAY 42
		_token_ID = _tokenizer.LookAhead();
		if (_token_ID == kTokenUnsupportedDelay)
		{
			CParseErrors::Error("Unsupported Shanky-style delay");
			// And consume 2 tokens to avoid further messages;
			_token_ID = _tokenizer.GetToken();
			_token_ID = _tokenizer.GetToken();
		}
	}
	else
	{
		CParseErrors::Error("Missing keyword force");
	}
}

void CFormulaParser::ParseOpenPPLRaiseExpression()
{
	// There are 3 possibilities
	//   RAISE FORCE
	//   RAISE <Amount> FORCE
	//   RAISE <PercentagedPot>% FORCE
	//
	// Keyword RAISE got already consumed
	int _token_ID = _tokenizer.LookAhead();
	if ((_token_ID == kTokenNumber)
		|| (_token_ID == kTokenIdentifier)
		|| TokenIsBracketOpen(_token_ID))
	{
		ParseExpression();
	}
	else
	{
		// Simple RAISE
		return;
	}
	_token_ID = _tokenizer.LookAhead();
	if (_token_ID == kTokenOperatorPercentage)
	{
		// Percentaged Potsize
		_tokenizer.GetToken();
	}
}

//ParseWhenCondition
