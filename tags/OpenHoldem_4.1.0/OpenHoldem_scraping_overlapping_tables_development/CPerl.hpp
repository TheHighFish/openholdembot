//////////////////////////////////////////////////////////////////////////
//
//  Author: TheHighFish
//
//  Created: 2007.12.11
//
//  Last change: 2008.07.06
//
//  Description: An interface to the Perl programing language 
//	for OpenHoldem
//
//////////////////////////////////////////////////////////////////////////

#ifndef INC_CPEARL_H
#define INC_CPEARL_H

#include "stdafx.h"
#include <String>
#include "PerlEz.h"

using namespace std;


//  "PerlEzEvalString" returns all results as strings.
//	All OpenHoldem's calls to perl functions should by 
//	design return only doubles, but the buffer is needed
//	for error messages sometimes. 
//	255 should be enough.
//
static const unsigned int _size_of_result_buffer = 255;
	
extern class CPerl
{
public:
	// public functions
	CPerl();
	~CPerl();
	bool IsPerlSymbol(const char *symbol);
	double GetPerlSymbol(const char *symbol);
	void EditMainFormulaFile();
	void LoadFormulaFile(string new_formula_file);
	void ReloadFormulaFile();
	void UnloadFormulaFile();
	void CheckSyntax();
	bool IsAFormulaLoaded();

private:
	// private variables - use public accessors and public mutators to address these
	PERLEZHANDLE	_interpreter;
	string			_actual_formula_file;
	char			_result_buffer[_size_of_result_buffer];
	bool			_interpreter_not_loaded; 
	bool			_formula_loaded;

private:
	// private functions and variables - not available via accessors or mutators
	void SendCallbackPointers();
	void DoErrorCheck(const int error_code);
	bool LoadDLL(void);

} *p_perl;

#endif /* INC_CPEARL_H */


