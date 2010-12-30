#include <afxwin.h>
#include "CSymbolTable.h"
#include <fstream> 
#include <iostream> 

CSymbolTable *p_symbol_table = 0; //NULL;


CSymbolTable::CSymbolTable()
{}

CSymbolTable::~CSymbolTable()
{}

void CSymbolTable::ClearSymbolTable()
{}

void CSymbolTable::AddSymbolsFromFile(CString filename)
{
	std::ifstream input_file(filename); 
	if (!input_file.is_open())
	{
		CString error_message = "Can't open file \"" + filename + "\".";
		MessageBox(0, error_message, "Error", 0);
	}
	while (input_file.eof())
	{
		CString next_line;
		//input_file.getline(next_line);
		if (next_line.Left(2) == "##")
		{
			// User defined function from the OpenPPL-library
			CString new_symbol = "";
			AddSymbol(new_symbol);
		}
		else if (next_line.Left(11).MakeLower() == "new symbol ")
		{
			// User defined function from the OpenPPL-file
			CString new_symbol = "";
			AddSymbol(new_symbol);
		}
	}
	input_file.close();
}

bool CSymbolTable::IsOpenPPLSymbol(CString symbol)
{
	// OpenPPL symbols are either UDFs on OpenPPL-level
	// or UDFs from the OpenPPL-library.
	// All other symbols will be treated as standard OpenHoldem symbols.
	return true;
}
