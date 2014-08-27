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
#include "CTokenizer.h"

#include "assert.h"
#include "CardFunctions.h"
#include "CParseErrors.h"
#include "TokenizerConstants.h"

// Global vars to be used by static accessors
int line_absolute = 1;
int line_relative = 1;
const int kMaxSizeOfToken = 256;
char last_token_string[kMaxSizeOfToken];
char* input_buffer;
int  _token_start_pointer;

#define CURRENT_CHARACTER     input_buffer[_token_end_pointer]
#define NEXT_CHARACTER        input_buffer[_token_end_pointer+1]
#define SECOND_NEXT_CHARACTER input_buffer[_token_end_pointer+2]
#define SIZE_OF_TOKEN         (_token_end_pointer - _token_start_pointer)
#define TOKEN_ADDRESS         &input_buffer[_token_start_pointer]
#define SKIP_NEXT_CHARACTER   _token_end_pointer++;

CTokenizer::CTokenizer() {
  InitNewParse();
}

CTokenizer::~CTokenizer() {
}

void CTokenizer::InitNewParse() {
  line_absolute = 1;
	InitVars();
}

void CTokenizer::InitVars() {
  // Gets called once for every function
  // Therefore we don't reset line_absolute here
	line_relative = 1;
	_token_start_pointer = 0;
	_token_end_pointer = 0;
	_last_token_pushed_back = false;
  _additional_percentage_operator_pushed_back = false;
	_last_token = k_undefined;
	_inside_OpenPPL_function = false;
}

void CTokenizer::SetInput(const char* next_formula_to_be_parsed)
{
	input_buffer = (char*) next_formula_to_be_parsed;
	InitVars();
}

int CTokenizer::GetToken()
{
	// Like lookahead, but accepting the token
	int next_token = LookAhead();
  if (_additional_percentage_operator_pushed_back) {
    _additional_percentage_operator_pushed_back = false;
  } else {
	  _last_token_pushed_back = false;
  } 
	return next_token;
}

int CTokenizer::LookAhead(bool expect_action /*= false */)
{
  if (_additional_percentage_operator_pushed_back) {
    return kTokenOperatorPercentage;
  }
	if (!_last_token_pushed_back)
	{
		_last_token = ScanForNextToken();
	}
	// Not yet accepted: treat it like "pushed back"
	_last_token_pushed_back = true;
  // In case we expect an action watch out for action-like identifiers
  if (expect_action) {
    CheckTokenForOpenPPLAction(&_last_token);
  }
	return _last_token;
}

inline bool IsOperatorCharacter(char c)
{
	switch (c)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '<':
	case '>':
	case '`':
	case '=':
	case '!':
	case '?':
	case ':':
	case '%':
	case '&':
	case '|':
	case '~':
	case '^': return true;
	default: return false;
	}
}

inline bool IsBracket(char c)
{
	switch (c)
	{
	case '(':
	case '[':
	case '{':
	case ')':
	case ']':
	case '}': return true;
	default: return false;
	}
}

bool CTokenizer::IsBinaryMinus()
{
	// We need to distinguishing binary and unary minusses (negative numbers)
	// Binary ones can only happen in certain circumstances
	return ((_last_token == kTokenIdentifier)
		|| (_last_token == kTokenNumber)
		|| (_last_token == kTokenBracketClose_1)
		|| (_last_token == kTokenBracketClose_2)
		|| (_last_token == kTokenBracketClose_3));
}

bool CTokenizer::IsTokenOpenPPLKeyword() {
  // No longer any fast exit on lower-case symbols, because
  // * that's no big improvement
  // * we need to create good error-messages, even on code like "when..."
  //
	// Now we compare...
  // Case-insensitive, as we want to catch mis-spelled keywords like "When".
  // They would otherwice get accepted as OpenPPL-functions 
  // and cause an error: "function used but never defined".
  //
  // We do no longer treat actions as key-words,
  // as actions can also appear as part of expressions like
  //   WHEN BotsLastAction = Raise...
  // and some others like Bet and Call are also OH-script symbols (bet, call)
	switch (SIZE_OF_TOKEN)
	{
	case 2:
		if (_memicmp(TOKEN_ADDRESS, "OR", 2) == 0)     { _OpenPPL_token_ID = kTokenOperatorLogicalOr;  return true; }
		break;
	case 3:
		if (_memicmp(TOKEN_ADDRESS, "NOT", 3) == 0)    { _OpenPPL_token_ID = kTokenOperatorLogicalNot; return true; }
		if (_memicmp(TOKEN_ADDRESS, "MOD", 3) == 0)    { _OpenPPL_token_ID = kTokenOperatorModulo;     return true; }
		if (_memicmp(TOKEN_ADDRESS, "AND", 3) == 0)    { _OpenPPL_token_ID = kTokenOperatorLogicalAnd; return true; }
		if (_memicmp(TOKEN_ADDRESS, "XOR", 3) == 0)    { _OpenPPL_token_ID = kTokenOperatorLogicalXOr; return true; }
    if (_memicmp(TOKEN_ADDRESS, "SET", 3) == 0)    { _OpenPPL_token_ID = kTokenActionUserVariableToBeSet; return true; }
		break;
	case 4:
		if (_memicmp(TOKEN_ADDRESS, "WHEN", 4) == 0) { 
			// Trigger handling of modulo / percentage operator
			_inside_OpenPPL_function = true;
			_OpenPPL_token_ID = kTokenOperatorConditionalWhen; 
			return true; 
		}
		break;
	case 5:
		if (_memicmp(TOKEN_ADDRESS, "BITOR", 5) == 0)   { _OpenPPL_token_ID = kTokenOperatorBinaryOr; return true; }
		if (_memicmp(TOKEN_ADDRESS, "DELAY", 5) == 0)   { _OpenPPL_token_ID = kTokenUnsupportedDelay; return true; }
    if (_memicmp(TOKEN_ADDRESS, "FORCE", 5) == 0)   { _OpenPPL_token_ID = kTokenKeywordForce;     return true; }
		break;
	case 6:
		if (_memicmp(TOKEN_ADDRESS, "RETURN", 6) == 0)  { _OpenPPL_token_ID = kTokenActionReturn;      return true; }
		if (_memicmp(TOKEN_ADDRESS, "BITAND", 6) == 0)  { _OpenPPL_token_ID = kTokenOperatorBinaryAnd; return true; }
		if (_memicmp(TOKEN_ADDRESS, "BITXOR", 6) == 0)  { _OpenPPL_token_ID = kTokenOperatorBinaryXOr; return true; }
		if (_memicmp(TOKEN_ADDRESS, "BITNOT", 6) == 0)  { _OpenPPL_token_ID = kTokenOperatorBinaryNot; return true; }
		break;
	default: return false;
	}
	return false;
}

// Some macros for tokenizing operators
// These macros also advance the end-pointer
//
// Macro for potential operator of size 2, first character already checked
#define IF_NEXT_CHARACTER_RETURN_OPERATOR(character, operator_token) \
	{ \
		if (NEXT_CHARACTER == character) \
		{ \
			_token_end_pointer++; \
			_token_end_pointer++; \
			return operator_token; \
		} \
	}
// Macro for operator of size 1, first character already checked,
// operator of size 2 not possible (already checked)
#define RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(operator_token) \
	{ \
		_token_end_pointer++; \
		return operator_token; \
	}

int CTokenizer::ScanForNextToken() {
  // "Ugly" goto to get rid of end-recursion
StartOfScanForNextToken:
  //printf("Analyzing: %s\n", &CURRENT_CHARACTER);

  // Useful functions:
  // http://www.cplusplus.com/reference/cctype/isspace
  // http://www.cplusplus.com/reference/cctype/isalnum
  // http://www.cplusplus.com/reference/cctype/isalpha
  // http://www.cplusplus.com/reference/cctype/isdigit
  // http://www.cplusplus.com/reference/cctype/isxdigit
  // http://www.cplusplus.com/reference/cctype/ispunct
  // http://www.cplusplus.com/reference/cctype/isgraph
  // http://www.cplusplus.com/reference/cctype/isblank
  //
  // White-space: skip
  while (isspace(CURRENT_CHARACTER)) {
    // [\r][\n]Line break
    if ((CURRENT_CHARACTER == '\n')
	    || (CURRENT_CHARACTER == '\r'))
    {
      line_absolute++;
      line_relative++;
    }
    SKIP_NEXT_CHARACTER
  }
  // Set start of next token at position after space
  _token_start_pointer = _token_end_pointer;
  // [a..zA..Z\$] Identifiers, 
  // including OpenPPL-style operators and keywords
  // including OpenHoldem-style $AA-symbols, etc.
  if (isalpha(CURRENT_CHARACTER) || (CURRENT_CHARACTER == '$')) {
    while(isalnum(CURRENT_CHARACTER)
        || (CURRENT_CHARACTER == '$')
	    || (CURRENT_CHARACTER == '_')) {
	  _token_end_pointer++;
	}
	if (IsTokenOpenPPLKeyword()) {
	  return _OpenPPL_token_ID;
	}
	return kTokenIdentifier;
  }
  // [0..9\.] Numbers
  // This includes 
  // * 12345
  // * .85
  // * 0xCE
  else if (isdigit(CURRENT_CHARACTER) || (CURRENT_CHARACTER == '.'))
  {
    // Special case suited and offsuited hands like 72o or 54s
    if (IsCardRankCharacter(NEXT_CHARACTER)) {
      char third_character_lowercase = tolower(SECOND_NEXT_CHARACTER);
	  if ((third_character_lowercase == 's')
          || (third_character_lowercase == 'o')) {
        _token_end_pointer += 3;
        return kTokenCards;
      }
    }
	// Entry-point for negative numbers after unary minus
NegativeNumber:
    while(isxdigit(CURRENT_CHARACTER)
	    || (CURRENT_CHARACTER == '.')
	    || (CURRENT_CHARACTER == 'x')) {
	  _token_end_pointer++;
	}
	return kTokenNumber;
  }
  // [/] Single-line-comments
  else if ((CURRENT_CHARACTER == '/')
      && (NEXT_CHARACTER == '/')) {
	SkipToEndOfLine();
	goto StartOfScanForNextToken;
  }
  // [/] Multi-line comments
  else if ((CURRENT_CHARACTER == '/')
      && (NEXT_CHARACTER == '*')) {
    SkipToEndOfMultiLineComment();
	goto StartOfScanForNextToken;	
  }
  // [+-*/<>=!?:%&|~^] OpenHoldem-style-Operators
	else if (IsOperatorCharacter(CURRENT_CHARACTER)) 
	{
		switch (CURRENT_CHARACTER)
		{
		case '+': 
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorPlus)
		case '-': 
			if (IsBinaryMinus())
			{
				RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorMinus)
			}
			else
			{
				// Unary minus
				_token_end_pointer++;
        if (isdigit(CURRENT_CHARACTER)) {
          // Start of negative number
				  goto NegativeNumber;
        }
        // Unary minus, starting an unary expression
        return kTokenOperatorUnaryMinus;
			}
		case '*': 
			IF_NEXT_CHARACTER_RETURN_OPERATOR('*', kTokenOperatorExponentiation)
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorMultiplication)
		case '/': // Special case comments already handled
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorDivision)
		case '<': 
			IF_NEXT_CHARACTER_RETURN_OPERATOR('=', kTokenOperatorSmallerOrEqual)
			IF_NEXT_CHARACTER_RETURN_OPERATOR('<', kTokenOperatorBitShiftLeft)
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorSmaller)
		case '>':
			IF_NEXT_CHARACTER_RETURN_OPERATOR('=', kTokenOperatorGreaterOrEqual)
			IF_NEXT_CHARACTER_RETURN_OPERATOR('>', kTokenOperatorBitShiftRight)
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorGreater)
		case '`':
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorBitCount)
		case '=': // Equality, either = or ==
			IF_NEXT_CHARACTER_RETURN_OPERATOR('=', kTokenOperatorEquality)
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorEquality)
		case '!': 
			IF_NEXT_CHARACTER_RETURN_OPERATOR('=', kTokenOperatorNotEqual)
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorLogicalNot)
		case '?': RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorConditionalIf)
		case ':': RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorConditionalElse)
		case '%': // Might be modulo (OpenHoldem) or percentage (OpenPPL)
			if (_inside_OpenPPL_function)
			{
				RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorPercentage);
			}
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorModulo);
		case '&': 
			IF_NEXT_CHARACTER_RETURN_OPERATOR('&', kTokenOperatorLogicalAnd)
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorBinaryAnd)
		case '|': 
			IF_NEXT_CHARACTER_RETURN_OPERATOR('|', kTokenOperatorLogicalOr)
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorBinaryOr)
		case '~': 
			IF_NEXT_CHARACTER_RETURN_OPERATOR('~', kTokenOperatorApproximatellyEqual)
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorBinaryNot)
		case '^': 
			IF_NEXT_CHARACTER_RETURN_OPERATOR('^', kTokenOperatorLogicalXOr)
			RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenOperatorBinaryXOr)
		default: 
			MessageBoxA(0, 
				"Unhandled operator in CTokenizer.\n"
				"This should not happen.\n"
				"Please get in contact with the development-team",
				"ERROR", 0);
		}
	}
	// [(){}[]] Brackets
	else if (IsBracket(CURRENT_CHARACTER))
	{
		switch (CURRENT_CHARACTER)
		{
		case '(': RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenBracketOpen_1)
		case '[': RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenBracketOpen_2)
		case '{': RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenBracketOpen_3)
		case ')': RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenBracketClose_1)
		case ']': RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenBracketClose_2)
		case '}': RETURN_DEFAULT_SINGLE_CHARACTER_OPERATOR(kTokenBracketClose_3)
		}
		// This must not happen
		assert(false);

	}
	// [#] List and function headers
	else if ((CURRENT_CHARACTER == '#')
		&& (NEXT_CHARACTER == '#')) 
	{
		// Double shebang as start/end of list and function headers
		_token_end_pointer++;
		_token_end_pointer++;
		// New function. Therefore assume it is OH-script
		// until we see a WHEN condition
		_inside_OpenPPL_function = false;
		return kTokenDoubleShebang;
	}
	// [\0] End of string = end of function
	else if (CURRENT_CHARACTER == '\0')
	{
		line_absolute++; 
		return kTokenEndOfFunction;
	}
	// Do not advance the input pointer,
	// as we don't accept anything
	CParseErrors::Error("Unexpected character.\n"
    "Maybe you are a little yellow chinese man,\n"
    "maybe you wrote old greek or hebrew,\n"
    "maybe you took MS-Word instead of a serious text-editor?");
  // Can't really continue parsing
  // Treat it as end of function
	return kTokenEndOfFunction;
}

char* CTokenizer::GetTokenString() {
	assert(SIZE_OF_TOKEN >= 0);
	// >= because we need one additional char for \0.
	if (SIZE_OF_TOKEN >= kMaxSizeOfToken)	{
		CParseErrors::Error("Identifier exceeds maximum of 256 chars");
	}
	memcpy(last_token_string, TOKEN_ADDRESS, SIZE_OF_TOKEN);
	last_token_string[SIZE_OF_TOKEN] = '\0';
	return last_token_string;
}

char* CTokenizer::RemainingInput()
{
	return TOKEN_ADDRESS;
}

void CTokenizer::SkipToEndOfLine()
{
	while (CURRENT_CHARACTER != '\n')
	{
		SKIP_NEXT_CHARACTER
	}
	// And skip the end of line too
	SKIP_NEXT_CHARACTER
  line_absolute++;
  line_relative++;
}

void CTokenizer::SkipToEndOfMultiLineComment()
{
	while (((CURRENT_CHARACTER != '*') || (NEXT_CHARACTER != '/'))
		&& (CURRENT_CHARACTER != '\0'))
	{
    if (CURRENT_CHARACTER == '\n') {
      line_absolute++;
      line_relative++;
    }
		SKIP_NEXT_CHARACTER
	}
	if (CURRENT_CHARACTER == '\0')
	{
		CParseErrors::Error("End of function reached while looking for end of comment.");
		SKIP_NEXT_CHARACTER
	}
	else
	{
		// Skip the remaining "*/"
		SKIP_NEXT_CHARACTER
		SKIP_NEXT_CHARACTER
	}
}

int CTokenizer::LineAbsolute()
{
	return line_absolute;
}

int CTokenizer::LineRelative()
{
	return line_relative;
}

void CTokenizer::CheckTokenForOpenPPLAction(int *token) {
  // Actions now treated as identifiers
  if (*token != kTokenIdentifier) return;
  CString token_string = GetTokenString();
  for (int i=0; i<kNumberOfOpenPPLActions; ++i) {
    int length_to_compare = kOpenPPLActionStrings[i].GetLength() + kOneCharacterExtraForTerminatingNull;
    if (_memicmp(token_string, kOpenPPLActionStrings[i], length_to_compare) == 0) {
      // Action expected and something action-like found
      // Now check for exact match, because especially
      // "bet" and "call" mean something different for OH-script
      if (token_string != kOpenPPLActionStrings[i]) {
        CString error_message;
        error_message.Format(
          "Found identifier \"%s\"\n"
          "Did you mean \"%s\"?",
          token_string,
          kOpenPPLActionStrings[i]);
        CParseErrors::Error(error_message);
      }
      // Replace kTokenIdentifier by something more appropriate
      *token = kOpenPPLActionConstants[i];
      return;
    }
  }
}