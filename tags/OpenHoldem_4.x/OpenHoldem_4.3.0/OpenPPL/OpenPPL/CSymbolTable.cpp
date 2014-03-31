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


#include <afxwin.h>
#include "CSymbolTable.h"
#include <fstream> 
#include <iostream> 


CSymbolTable *p_symbol_table = 0; //NULL;

#undef DEBUG_SYMBOLTABLE_GENERAL
#undef DEBUG_SYMBOLTABLE_CREATION
#undef DEBUG_SYMBOLTABLE_QUERY

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
#ifdef DEBUG_SYMBOLTABLE_GENERAL
	MessageBox(0, "Adding symbols from file: " + filename, "Debug", 0);
#endif
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
#ifdef DEBUG_SYMBOLTABLE_CREATION
			MessageBox(0, new_symbol, "Adding as is...", 0);
#endif
			// Add symbol as is
			AddSymbol(new_symbol);
		}
		else if (next_line.Left(11).MakeLower() == "new symbol ")
		{
			// User defined function from the OpenPPL-file
			CString new_symbol = next_line.Mid(11, next_line.GetLength()-11);
			// Cut off white spaces to avoid problems:
			// http://www.maxinmontreal.com/forums/viewtopic.php?f=214&t=14773
			new_symbol.TrimLeft(" \t");
			new_symbol.TrimRight(" \t");
#ifdef DEBUG_SYMBOLTABLE_CREATION
			MessageBox(0, new_symbol, "Adding with prefix...", 0);
#endif
			//Add symbol with standardized name ("f$OpenPPL_")
			AddSymbol(new_symbol);
		}
		else
		{
			// Nothing to do here.
			// This code-line simply does not start a symbol definition.
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
		CString new_symbol_with_correct_prefix = GetSymbolNameWithCorrectPrefix(new_symbol);
		// Use the lowercase name as lookup-key 
		// and store the correct name.
		CString new_symbol_lowercase_with_correct_prefix 
			= new_symbol_with_correct_prefix;
		new_symbol_lowercase_with_correct_prefix.MakeLower();
#ifdef DEBUG_SYMBOLTABLE_CREATION
		MessageBox(0, "Inserting " + new_symbol_with_correct_prefix 
			+ " as " + new_symbol_lowercase_with_correct_prefix, "Debug", 0);
#endif
		known_symbols[new_symbol_lowercase_with_correct_prefix] 
			= new_symbol_with_correct_prefix;
	}
}

bool CSymbolTable::IsOpenPPLSymbol(CString symbol)
{
	// OpenPPL symbols are either UDFs on OpenPPL-level
	// or UDFs from the OpenPPL-library.
	// All other symbols will be treated as standard OpenHoldem symbols.
	//
	// Docu on find(): http://www.cplusplus.com/reference/stl/map/find/
	CString standardized_symbol_lowercase = GetLookupKey(symbol);
	CSMap::const_iterator find_result = known_symbols.find(standardized_symbol_lowercase);
	if (find_result == known_symbols.end())
	{
#ifdef DEBUG_SYMBOLTABLE_CREATION
		if (!GenerationOfSymboltableInProgress())
		{
			MessageBox(0, standardized_symbol_lowercase, "not found", 0);
		}
#endif
		return false;
	}

	// Check for exact match
	// if (find_result->second == GetSymbolNameWithCorrectPrefix(symbol))
	{
		return true;
	}
	//else
	{
#ifdef DEBUG_SYMBOLTABLE_CREATION
		if (!GenerationOfSymboltableInProgress())
		{
			MessageBox(0, find_result->second, GetSymbolNameWithCorrectPrefix(symbol), 0);
		}
#endif
		return (false);
	}
}


CString CSymbolTable::GetSymbolNameWithCorrectPrefix(CString symbol)
{
	// Standardized OpenPPL-symbols should start with "OpenPPL_",
	// be it a name from the library or a UDF ("New Symbol ...").
	// Only OpenHoldem symbols don't have this prefix.
	//
	// This function should only be used for symbol lookup internally
	if (symbol.Left(10) == "f$OpenPPL_")
	{
		return symbol;
	}
	else
	{
		CString standardized_symbol_name = "f$OpenPPL_" + symbol;
#ifdef DEBUG_SYMBOLTABLE_QUERY
		MessageBox(0, "Symbol: [" + symbol + "]\n" 
			+ "Standardized name: [" + standardized_symbol_name + "]", 
			"Debug", 0);
#endif
		return standardized_symbol_name;
	}
}


CString CSymbolTable::GetLookupKey(CString symbol)
{
	CString symbol_with_prefix = GetSymbolNameWithCorrectPrefix(symbol);
	CString lookup_key = symbol_with_prefix.MakeLower();
	return lookup_key;
}

CString CSymbolTable::GetSymbolNameWithcorrectCases(CString symbol)
{
	// This function should be used for generation of correct code
	CString standardized_name_as_lookup_key = GetLookupKey(symbol);
	CString symbol_name_with_correct_cases = known_symbols[standardized_name_as_lookup_key];
#ifdef DEBUG_SYMBOLTABLE_QUERY
	MessageBox(0, "Correct cases: " + symbol_name_with_correct_cases, "Debug", 0);
#endif
	return symbol_name_with_correct_cases;
}


bool CSymbolTable::GenerationOfSymboltableInProgress()
{
	return _generation_of_symboltable_in_progress;
}
