#include "stdafx.h"
#include "CAutoplayerFunctions.h"

#include <assert.h>
#include "CGrammar.h"
#include "CPreferences.h"

// !!! make it a symbol-engine?
// Call it!

CAutoplayerFunctions *p_autoplayer_functions = NULL;


CAutoplayerFunctions::CAutoplayerFunctions()
{
	Reset();
}

void CAutoplayerFunctions::Reset()
{
	for (int i=0; i<k_number_of_autoplayer_functions; i++)
	{
		SetAutoplayerFunction(i, 0);
	}
}

void CAutoplayerFunctions::SetAutoplayerFunction(const int function_to_bn_set, const double new_value)
{
	assert(function_to_bn_set >= 0);
	assert(function_to_bn_set < k_number_of_autoplayer_functions);
	_autoplayer_functionvalues[function_to_bn_set] = new_value;
};

double CAutoplayerFunctions::GetAutoplayerFunctionValue(const int function_to_bn_queried)
{
	assert(function_to_bn_queried >= 0);
	assert(function_to_bn_queried < k_number_of_autoplayer_functions);
	return _autoplayer_functionvalues[function_to_bn_queried];
}

void CAutoplayerFunctions::CalcPrimaryFormulas()
{
	int			e = SUCCESS;
	CGrammar	gram;

	write_log(prefs.debug_symbolengine(), "Trace enabled: %i\n", prefs.trace_enabled());
	bool trace_needed = prefs.trace_enabled();

	for (int i=first_primary_autoplayer_function; i<=last_primary_autoplayer_function ; i++)
	{
		e = SUCCESS;
		p_autoplayer_functions->SetAutoplayerFunction(i, // function to be set
			gram.CalcF$symbol(p_formula, k_autoplayer_functionname[i], trace_needed, &e));
		write_log(prefs.debug_symbolengine(), "Primary formulas; %s: %f\n", 
			k_autoplayer_functionname[i], p_autoplayer_functions->GetAutoplayerFunctionValue(i));
	}
	// [nik0] always set check/fold to 1
	p_autoplayer_functions->SetAutoplayerFunction(k_autoplayer_function_check, 1);
	p_autoplayer_functions->SetAutoplayerFunction(k_autoplayer_function_fold, 1);
	CalcAutoTrace();
}

void CAutoplayerFunctions::CalcSecondaryFormulas(void)
{
	int			e = SUCCESS;
	CGrammar	gram;

	bool trace_needed = prefs.trace_enabled();

	for (int i=first_secondary_autoplayer_function; i<=last_secondary_autoplayer_function ; i++)
	{
		e = SUCCESS;
		p_autoplayer_functions->SetAutoplayerFunction(i, // function to be set
			gram.CalcF$symbol(p_formula, k_autoplayer_functionname[i], trace_needed, &e));
		write_log(prefs.debug_symbolengine(), "Primary formulas; %s: %f\n", 
			k_autoplayer_functionname[i], p_autoplayer_functions->GetAutoplayerFunctionValue(i));
	}
	CalcAutoTrace();
}

void CAutoplayerFunctions::CalcAutoTrace()
{
	int			e = SUCCESS;
	CGrammar	gram;

	gram.CalcF$symbol(p_formula, "f$autotrace", true, &e);
}