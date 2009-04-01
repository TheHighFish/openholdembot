#ifndef INC_CEVALINFO_H
#define INC_CEVALINFO_H

// Forward declaration
class CEvalInfoFunction;

class CEvalInfoFunctionArray : public CArray<CEvalInfoFunction *, CEvalInfoFunction *>
{
public:
	~CEvalInfoFunctionArray();

	void DumpFunctionArray(int indent);

	CEvalInfoFunction *FindFunction(const char *name);
};

class CEvalInfoSymbol
{
public:
	CEvalInfoSymbol(const char *symbol , double val) { m_Symbol = symbol; m_Value = val; }

	void DumpSymbol(int indent);

	CString m_Symbol;
	double m_Value;
};

class CEvalInfoSymbolArray : public CArray<CEvalInfoSymbol *, CEvalInfoSymbol *>
{
public:
	~CEvalInfoSymbolArray();

	CEvalInfoSymbol *FindSymbol(const char *name) {
		for (int i=0;i<GetSize();i++) {
			if (!GetAt(i)->m_Symbol.Compare(name))
				return GetAt(i);
		}
		return NULL;
	}

	void DumpSymbolArray(int indent);
};

class CEvalInfoFunction
{
public:
	CEvalInfoFunction(const char *name) { m_FunctionName = name; m_Cached = false; m_Result = 0.0; m_Offset = m_Line = m_Column = 0; }
	CEvalInfoFunction(const char *name, bool cached, double result) { m_FunctionName = name; m_Cached = cached; m_Result = result; m_Offset = m_Line = m_Column = 0; }

	CString m_FunctionName;
	bool	m_Cached;
	double  m_Result;
	int		m_Offset, m_Line, m_Column;

	void DumpFunction(int indent);

	CEvalInfoFunctionArray m_CalledFunctions;
	CEvalInfoSymbolArray m_SymbolsUsed;
};

#endif  //INC_CEVALINFO_H

