#include <afxwin.h>
#include "CSymbolTable.h"
#include <fstream> 
#include <iostream> 


CSymbolTable *p_symbol_table = 0; //NULL;

CSymbolTable::CSymbolTable()
{
	ClearSymbolTable();
}

CSymbolTable::~CSymbolTable()
{
	ClearSymbolTable();
}

void CSymbolTable::ClearSymbolTable()
{
	known_symbols.clear();
}

void CSymbolTable::AddSymbolsFromFile(CString filename)
{
	std::ifstream input_file(filename); 
	if (!input_file.is_open())
	{
		CString error_message = "Can't open file \"" + filename + "\".";
		MessageBox(0, error_message, "Error", 0);
		exit(-1);
	}
	while (!input_file.eof())
	{
		const int max_length = 1000;
		char buffer[max_length];
		input_file.getline(buffer, max_length, '\n');
		CString next_line(buffer);
		if (next_line.Left(2) == "##")
		{
			if (next_line.Right(2) != "##")
			{
				CString error_message = CString("Invalid function syntax.\n"
					"Expecting trailing \"##\".\n"
					"\n"
					"Symbol: \"") + next_line + "\"";
				MessageBox(0, error_message, "Error", 0);
			}
			// User defined function from the OpenPPL-library
			CString new_symbol = next_line.Mid(2, next_line.GetLength()-4);
			AddSymbol(new_symbol);
		}
		else if (next_line.Left(11).MakeLower() == "new symbol ")
		{
			// User defined function from the OpenPPL-file
			CString new_symbol = next_line.Mid(11, next_line.GetLength()-11);
			AddSymbol(new_symbol);
		}
	}
	input_file.close();
}

void CSymbolTable::AddSymbol(CString new_symbol)
{
	// Already known?
	if (IsOpenPPLSymbol(new_symbol))
	{
		CString error_message =
			CString("Symbol \"") + new_symbol + "\" already defined.";
		MessageBox(0, error_message, "Error", 0);
	}
	else
	{
		//MessageBox(0, new_symbol, "Adding symbol", 0);
		known_symbols[new_symbol] = true;
	}
}

bool CSymbolTable::IsOpenPPLSymbol(CString symbol)
{
	// OpenPPL symbols are either UDFs on OpenPPL-level
	// or UDFs from the OpenPPL-library.
	// All other symbols will be treated as standard OpenHoldem symbols.
	//
	// Docu on find(): http://www.cplusplus.com/reference/stl/map/find/
	return (known_symbols.find(symbol) != known_symbols.end());
}

CString CSymbolTable::GetStandardizedSymbolName(CString symbol)
{
	// Standardized OpenPPL-symbols should start with "OpenPPL_",
	// be it a name from the library or a UDF ("New Symbol ...").
	// Only OpenHoldem symbols don't have this prefix.
	if (symbol.Left(8) == "OpenPPL_")
	{
		return symbol;
	}
	else
	{
		CString standardized_symbol_name = "OpenPPL_" + symbol;
		return standardized_symbol_name;
	}
}
