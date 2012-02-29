#ifndef INC_CAUTOPLAYER_FUNCTIONS
#define INC_CAUTOPLAYER_FUNCTIONS

extern class CAutoplayerFunctions
{
public:
	CAutoplayerFunctions();
public:
	void Reset();
#define ENT CSLock lock(m_critsec);
	void SetAautoplayerFunction(const int function_to_bn_set, const double new_value);
	double GetAautoplayerFunctionValue(const int function_to_bn_set);
#undef ENT
public:
	double	f$alli()	{ return _autoplayer_functionvalues[k_autoplayer_function_allin]; }
	double	f$betsize()	{ return _autoplayer_functionvalues[k_autoplayer_function_betsize]; }
	double	f$rais()	{ return _autoplayer_functionvalues[k_autoplayer_function_raise]; }
	double	f$call()	{ return _autoplayer_functionvalues[k_autoplayer_function_call]; }
	double	f$sitin()	{ return _autoplayer_functionvalues[k_autoplayer_function_sitin]; }
	double	f$sitout()	{ return _autoplayer_functionvalues[k_autoplayer_function_sitout]; }
	double	f$leave()	{ return _autoplayer_functionvalues[k_autoplayer_function_leave]; }
	double	f$close()	{ return _autoplayer_functionvalues[k_autoplayer_function_close]; }
	double	f$prefold()	{ return _autoplayer_functionvalues[k_autoplayer_function_prefold]; }
	double	f$rebuy()	{ return _autoplayer_functionvalues[k_autoplayer_function_rebuy]; }
	double	f$delay()	{ return _autoplayer_functionvalues[k_autoplayer_function_delay]; }
	double	f$chat()	{ return _autoplayer_functionvalues[k_autoplayer_function_chat]; }
private:
	double _autoplayer_functionvalues[k_number_of_autoplayer_functions];
private:
	CCritSec	m_critsec;
} *p_autoplayer_functions;

#endif // INC_CAUTOPLAYER_FUNCTIONS