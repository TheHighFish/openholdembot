#include "stdafx.h"
#include "CAutoplayerFunctions.h"
#include <assert.h>

CAutoplayerFunctions *p_autoplayer_functions = NULL;


CAutoplayerFunctions::CAutoplayerFunctions()
{
	Reset();
}

void CAutoplayerFunctions::Reset()
{
	for (int i=0; i<k_number_of_autoplayer_functions; i++)
	{
		SetAautoplayerFunction(i, 0);
	}
}

void CAutoplayerFunctions::SetAautoplayerFunction(const int function_to_bn_set, const double new_value)
{
	assert(function_to_bn_set >= 0);
	assert(function_to_bn_set < k_number_of_autoplayer_functions);
	_autoplayer_functionvalues[function_to_bn_set] = new_value;
};

double CAutoplayerFunctions::GetAautoplayerFunctionValue(const int function_to_bn_queried)
{
	assert(function_to_bn_queried >= 0);
	assert(function_to_bn_queried < k_number_of_autoplayer_functions);
	return _autoplayer_functionvalues[function_to_bn_queried];
}

