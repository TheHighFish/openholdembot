#ifndef INC_CFORMULAPARSER_H
#define INC_CFORMULAPARSER_H

#define _AFXDLL 

#include "afx.h"
#include "atlstr.h"
#include "CFormulaFileSplitter.h"
#include "CTokenizer.h"

class CFormulaParser
{
public:
	CFormulaParser();
	~CFormulaParser();
public:
	void ParseFile(CArchive & formula_file);
	void ParseSingleFormula(CString function_content);
public:
	static CString CurrentFunctionName();
private:
	void ExpectDoubleShebangAsStartOfFunctionHeader();
	void ExpectDoubleShebangAsEndOfFunctionHeader();
	void ExpectMatchingBracketClose(int opening_bracket);
	void ExpectConditionalThen();
	void ExpectKeywordForce();
	void CheckForExtraTokensAfterEndOfFunction();
private:
	// OH-script and OpenPPL
	void ParseListBody();
	void ParseFunctionBody();
private:
	// OH-script
	void ParseExpression();
	void ParseBracketExpression();
	void ParseUnaryExpression();
	void ParseSimpleExpression();
	void ParseRightPartOfBinaryOrTernaryExpression();
	void ParseConditionalThenElseExpressions();
private:
	// OpenPPL
	void ParseOpenEndedWhenConditionSequence();
	void ParseOpenPPLAction();
	void ParseOpenPPLRaiseExpression();
private:
	CFormulaFileSplitter _formula_file_splitter;
	CTokenizer _tokenizer;
private:
	int _token_ID;
	CString _token;
};

#endif INC_CFORMULAPARSER_H