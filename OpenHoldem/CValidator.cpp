#include "StdAfx.h"

#include "CAutoPlayer.h"
#include "CDllExtension.h"
#include "CPreferences.h"
#include "CSymbols.h"
#include "CValidator.h"


CValidator::CValidator()
{
}


CValidator::~CValidator()
{
}


void CValidator::ValidateSingleRule()
{
	if (_precondition) 
    { 
	    if (!_postcondition ) 
        { 
			if (prefs.validator_stop_on_error()) 
			{ 
				p_autoplayer->set_autoplayer_engaged(false); 
			} 
			CString the_ErrorMessage = "Testcase ID: " 
				+ CString(_testcase_id) 
				+ "\n\n" 
			    + "Symbols affected: " 
				+ CString(_symbols_possibly_affected); 
			MessageBox(NULL, the_ErrorMessage, "Validator Error", 
				MB_OK | MB_TOPMOST | MB_ICONEXCLAMATION); 
		} 
    } 
}


// gws function to access the symbols by name
//
double CValidator::gws(const char *the_Symbol)
{
	bool is_Error;
	double my_Chair = GetSymbolFromDll(0, "userchair", is_Error);
	return GetSymbolFromDll(int(my_Chair), the_Symbol, is_Error);
}

/////////////////////////////////////////////////////////////////////////////
//
// Macros for the validator,
// turning rules in "pseudo-code"
// into executable C-code.


// BEGIN_TESTCASE
//   
// Action: Ignore this by turning it into an empty string.
//
#define BEGIN_TESTCASE


// REASONING
//
// Action: Ignore this by turning it into an empty string.
//
#define REASONING(R)


// TESTCASE_ID
//
// Action: Assign the testcase-ID to a private variable.
//
#define TESTCASE_ID(N) { _testcase_id = (N); }

// PRECONDITION
//
// Action: Assign the precondition to a private variable.
//
#define PRECONDITION(C) { _precondition = (C); }


// POSTCONDITION
//
// Action: Assign the postcondition to a private variable.
//
#define POSTCONDITION(C) { _postcondition = (C); }


// SYMBOLS_POSSIBLY_AFFECTED
//
// Action: Defines a message about the symbols, that
//   could be affected by this rule. 
//
#define SYMBOLS_POSSIBLY_AFFECTED(S) { _symbols_possibly_affected = (S); }


// END_TESTCASE
//
// Actions: ValidateSingleRule(), i.e.: 
//   * execution of the testcase
//     (if precondition for the rule applies).
//   * message on error (postcondition not met).
//   * stop autoplayer on error (optional)
//
#define END_TESTCASE ValidateSingleRule();
	

//
// End of macro code
//
/////////////////////////////////////////////////////////////////////////////

void CValidator::ValidateGameState()
{
	// Do not validate, if not enabled.
	// Validate never? Then leave.
	if (prefs.validator_enabled() == 0) 
	{ 
		return; 
	}
	// Validate only, when it's my turn, but it is not my turn? Then leave.
	if ((prefs.validator_enabled() == 1) && !(p_symbols->sym()->ismyturn)) 
	{ 
		return; 
	}
	// Otherwise validate.
	//
	//
	// Validator-rules are defined in "pseudo-code",
	// that is easily understandable for non-programmers,
	// but can be turned into C-code using macro-techniques.
	//
	// Due to the large number of rules, 
	// we just put them in external files
	// and include them here as is.
	//
#include "Validator_Rules\range_checks_inline.cpp"
}