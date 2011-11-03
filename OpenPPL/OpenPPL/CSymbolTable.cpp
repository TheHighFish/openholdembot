#include <afxwin.h>
#include "CSymbolTable.h"
#include <fstream> 
#include <iostream> 


CSymbolTable *p_symbol_table = 0; //NULL;


CSymbolTable::CSymbolTable()
{
	ClearSymbolTable();
	_generation_of_symboltable_in_progress = true;
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
	_generation_of_symboltable_in_progress = true;
	std::ifstream input_file(filename); 
	if (!input_file.is_open())
	{
		CString error_message = "Can't open file \"" + filename + "\".";
		MessageBox(0, error_message, "Error", 0);
		exit(-1);
	}
	while (!input_file.eof())
	{
		// According to JConner some insane Shanky-profiles
		// use more than 5000 chare per line, so we set our limit a bit higher
		// and also check for errors.
		// http://www.maxinmontreal.com/forums/viewtopic.php?f=216&t=11767&start=155
		const int max_length = 10000;
		char buffer[max_length];
		input_file.getline(buffer, max_length, '\n');
		if ((input_file.badbit ||input_file.failbit) && !input_file.eofbit)
		{
			perror(("error while reading file " + filename));
			MessageBox(0, "Error: Can't read input file.\n"
				"Probably line too long.\n"
				"10.000 characters is the maximum.\n"
				"Going to terminate.", "Error", 0);
			exit(-1);
		}
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
			// Add symbol as is
			//MessageBox(0, new_symbol, "Adding as is...", 0);
			AddSymbol(new_symbol);
		}
		else if (next_line.Left(11).MakeLower() == "new symbol ")
		{
			// User defined function from the OpenPPL-file
			CString new_symbol = next_line.Mid(11, next_line.GetLength()-11);
			// Add symbol with standardized name ("f$OpenPPL_")
			//MessageBox(0, new_symbol, "Trying to add...", 0);
			AddSymbol(new_symbol);
		}
	}
	input_file.close();
	_generation_of_symboltable_in_progress = false;
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
		// Use the lowercase name as lookup-key 
		// and store the correct name.
		CString new_symbol_lowercase = new_symbol;
		new_symbol_lowercase.MakeLower();
		//MessageBox(0, new_symbol, "Inserting", 0);
		known_symbols[new_symbol_lowercase] = new_symbol;
	}
}

bool CSymbolTable::IsOpenPPLSymbol(CString symbol)
{
	// OpenPPL symbols are either UDFs on OpenPPL-level
	// or UDFs from the OpenPPL-library.
	// All other symbols will be treated as standard OpenHoldem symbols.
	//
	// Docu on find(): http://www.cplusplus.com/reference/stl/map/find/
	CString standardized_symbol_lowercase = GetStandardizedSymbolName(symbol).MakeLower();
	CSMap::const_iterator find_result = known_symbols.find(standardized_symbol_lowercase);
	if (find_result == known_symbols.end())
	{
		//if (!GenerationOfSymboltableInProgress())
		//	MessageBox(0, standardized_symbol_lowercase, "not found", 0);
		return false;
	}

	// Check for exact match
	// if (find_result->second == GetStandardizedSymbolName(symbol))
	{
		return true;
	}
	//else
	{
		//if (!GenerationOfSymboltableInProgress())
		//	MessageBox(0, find_result->second, GetStandardizedSymbolName(symbol), 0);
		return (false);
	}
}

CString CSymbolTable::GetStandardizedSymbolName(CString symbol)
{
	// Standardized OpenPPL-symbols should start with "OpenPPL_",
	// be it a name from the library or a UDF ("New Symbol ...").
	// Only OpenHoldem symbols don't have this prefix.
	if (symbol.Left(10) == "f$OpenPPL_")
	{
		return symbol;
	}
	else
	{
		CString standardized_symbol_name = "f$OpenPPL_" + symbol;
		return standardized_symbol_name;
	}
}

bool CSymbolTable::GenerationOfSymboltableInProgress()
{
	return _generation_of_symboltable_in_progress;
}
