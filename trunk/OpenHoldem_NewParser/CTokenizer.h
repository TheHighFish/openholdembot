#ifndef INC_CTOKENIZER_H
#define INC_CTOKENIZER_H

#include "atlstr.h"

class CTokenizer 
{	
friend class CFormulaParser;
public:
	CTokenizer();
	~CTokenizer();
public:
	void SetInput(const char* next_formula_to_be_parsed);
	int GetToken();
	int LookAhead();
	char* GetTokenString();
public:
	void PushBack()	{ _last_token_pushed_back = true; }
public:
	static int LineAbsolute();
	static int LineRelative();
	static char* RemainingInput();
private:
	int  ScanForNextToken();
	void SkipToEndOfLine();
	void SkipToEndOfMultiLineComment();
private:
	bool IsBinaryMinus();
	bool IsTokenOpenPPLKeyword();
private:
	void InitVars();
private:
	int  _token_end_pointer;
	bool _last_token_pushed_back;
	int  _last_token;
private:
	// To distinguish modulo % and percentage % operators
	bool _inside_OpenPPL_function;
	// Gets set as side-effect by IsTokenOpenPPLKeyword()
	int  _OpenPPL_token_ID;
};

#endif INC_CTOKENIZER_H