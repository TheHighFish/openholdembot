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
#include "CAutoplayerFunctions.h"

#include <assert.h>
#include "CGrammar.h"
#include "MagicNumbers.h"
#include "CPreferences.h"
#include "StringFunctions.h"

// TODO make it a symbol-engine?
// Call it!

CAutoplayerFunctions *p_autoplayer_functions = NULL;


CAutoplayerFunctions::CAutoplayerFunctions()
{
	Reset();
}

void CAutoplayerFunctions::Reset()
{
	for (int i=0; i<k_number_of_standard_functions; i++)
	{
		SetAutoplayerFunction(i, 0);
	}
}

void CAutoplayerFunctions::SetAutoplayerFunction(const int function_to_bn_set, const double new_value)
{
	assert(function_to_bn_set >= 0);
	assert(function_to_bn_set < k_number_of_standard_functions);
	_autoplayer_functionvalues[function_to_bn_set] = new_value;
};

double CAutoplayerFunctions::GetAutoplayerFunctionValue(const int function_to_bn_queried)
{
	assert(function_to_bn_queried >= 0);
	assert(function_to_bn_queried < k_number_of_standard_functions);
	return _autoplayer_functionvalues[function_to_bn_queried];
}

void CAutoplayerFunctions::CalcPrimaryFormulas()
{
	int			e = SUCCESS;
	CGrammar	gram;

	write_log(preferences.debug_symbolengine(), "[CAutoplayerFunctions] Trace enabled: %i\n", Bool2CString(preferences.trace_enabled()));
	bool trace_needed = preferences.trace_enabled();

	for (int i=k_autoplayer_function_allin; i<=k_autoplayer_function_fold; i++)
	{
		e = SUCCESS;
		p_autoplayer_functions->SetAutoplayerFunction(i, // function to be set
			gram.CalcF$symbol(p_formula, k_standard_function_names[i], trace_needed, &e));
		write_log(preferences.debug_symbolengine(), "[CAutoplayerFunctions] Primary formulas; %s: %f\n", 
			k_standard_function_names[i], p_autoplayer_functions->GetAutoplayerFunctionValue(i));
	}
	CalcAutoTrace();
}

void CAutoplayerFunctions::CalcSecondaryFormulas(void)
{
	int			e = SUCCESS;
	CGrammar	gram;

	bool trace_needed = preferences.trace_enabled();

	for (int i=k_standard_function_prefold; i<=k_standard_function_delay; i++)
	{
		e = SUCCESS;
		p_autoplayer_functions->SetAutoplayerFunction(i, // function to be set
			gram.CalcF$symbol(p_formula, k_standard_function_names[i], trace_needed, &e));
		write_log(preferences.debug_symbolengine(), "[CAutoplayerFunctions] Secondary formulas; %s: %f\n", 
			k_standard_function_names[i], p_autoplayer_functions->GetAutoplayerFunctionValue(i));
	}
	CalcAutoTrace();
}

void CAutoplayerFunctions::CalcAutoTrace()
{
	int			e = SUCCESS;
	CGrammar	gram;

	gram.CalcF$symbol(p_formula, "f$autotrace", true, &e);
}