#ifndef INC_CPARSEERRORS_H
#define INC_CPARSEERRORS_H

#include "stdafx.h"

class CParseErrors
{
public:
	CParseErrors();
	~CParseErrors();
public:
	// These functions are static,
	// as we want to encapsulate everything in a class,
	// but have no state and don't want/need the instantiation of an object.
	static void ErrorUnknownIdentifier();
	static void Error(CString short_message);
private:
	static CString ErroneousCodeSnippet();
};

#endif INC_CPARSEERRORS_H
