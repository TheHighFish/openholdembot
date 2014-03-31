#include "stdafx.h"
#include "CGrammar.h"

#include <strstream>
#include "..\CCritSec\CCritSec.h"
#include "..\CTransform\CTransform.h"
#include "CAutoplayer.h"
#include "CDllExtension.h"
#include "CLogSymbols.h"
#include "CEvalInfo.h"
#include "CFormula.h"
#include "CICMCalculator.h"
#include "CMemory.h"
#include "CPerl.hpp"
#include "CPokerAction.h"
#include "CPreferences.h"
#include "CSymbolEngineCards.h"
#include "CSymbols.h"
#include "CVersus.h"
#include "FloatingPoint_Comparisions.h"
#include "Numericalfunctions.h"
#include "OH_MessageBox.h"
#include "UnknownSymbols.h"

CGrammar::CGrammar(void)
{
	_RecursionDepth = 0;
}

CGrammar::~CGrammar(void)
{
}

CCritSec		m_critsec_evaluate;

double CGrammar::EvaluateTree(CFormula * const f, tpi_type info, CEvalInfoFunction **logCallingFunction, int *e)
{
	CSLock lock(m_critsec_evaluate);

	return EvaluateExpression(f, info.trees.begin(), logCallingFunction, e);
}

double CGrammar::CalcF$symbol(CFormula * const f, const char *symbol, int *e)
{ 
	return CalcF$symbol(f, symbol, false, e); 
}

// !!! e is an in-out-parameter and needs to be initialized to SUCCESS
// otherwise these functions don't work.
// Candidate for refactoring
double CGrammar::CalcF$symbol(CFormula * const f, const char *symbol, bool log, int *e)
{
	CEvalInfoFunction *logCallingFunction = NULL;
	double ret = DoCalcF$symbol(f, symbol, (log ? &logCallingFunction : NULL), log, e);

	if (logCallingFunction) 
	{
		logCallingFunction->DumpFunction(0);
		delete logCallingFunction;
	}

	return ret;
}

double CGrammar::EvaluateExpression(CFormula * const f, iter_t const& i, CEvalInfoFunction **logCallingFunction, int *e)
{
	string sym(i->value.begin(), i->value.end());
	parser_id tp = i->value.id();

	double ret = DoEvaluateExpression(f, i, logCallingFunction, e);

	if (logCallingFunction && *logCallingFunction && tp == exec_grammar::SYMBOL_ID) 
	{
			if (!(*logCallingFunction)->m_SymbolsUsed.FindSymbol(sym.c_str()) && !(*logCallingFunction)->m_CalledFunctions.FindFunction(sym.c_str()))
					(*logCallingFunction)->m_SymbolsUsed.Add(new CEvalInfoSymbol(sym.c_str(), ret));
	}

	return ret;
}

double CGrammar::DoEvaluateExpression(CFormula * const f, iter_t const& i, CEvalInfoFunction **logCallingFunction, int *e)
{
	double			result = 0.;
	boost::spirit::parser_id		id = i->value.id();

	// Bounce errors up the stack
	if (*e != SUCCESS)  return 0;

	if (logCallingFunction && *logCallingFunction && (*logCallingFunction)->m_Offset < (INT_PTR)i->value.value())
			(*logCallingFunction)->m_Offset = (INT_PTR)i->value.value();

	// Symbols
	if (id == exec_grammar::SYMBOL_ID)
	{
		string sym(i->value.begin(), i->value.end());
				return EvaluateSymbol(f, sym, logCallingFunction, e);
	}

	// Hex constant
	else if (id == exec_grammar::INT_CONSTANT_HEX_ID)
	{
		string number(i->value.begin()+2, i->value.end());
		return (double) strtoul(number.c_str(), 0, 16);
	}

	// Dec constant
	else if (id == exec_grammar::INT_CONSTANT_DEC_ID)
	{
		string number(i->value.begin(), i->value.end());
		return (double) strtoul(number.c_str(), 0, 10);
	}

	// Oct constant
	else if (id == exec_grammar::INT_CONSTANT_OCT_ID)
	{
		string number(i->value.begin()+2, i->value.end());
		return (double) strtoul(number.c_str(), 0, 8);
	}

	// Quad constant
	else if (id == exec_grammar::INT_CONSTANT_QUA_ID)
	{
		string number(i->value.begin()+2, i->value.end());
		return (double) strtoul(number.c_str(), 0, 4);
	}


	// Bin constant
	else if (id == exec_grammar::INT_CONSTANT_BIN_ID)
	{
		string number(i->value.begin()+2, i->value.end());
		return (double) strtoul(number.c_str(), 0, 2);
	}

	// Float constants
	else if (  id == exec_grammar::FLOAT_CONSTANT1_ID
						   | id == exec_grammar::FLOAT_CONSTANT2_ID
						   | id == exec_grammar::FLOAT_CONSTANT3_ID )
	{
		string number(i->value.begin(), i->value.end());
		return (double) strtod(number.c_str(), 0);
	}

	// Exponentiation
	else if (id == exec_grammar::EXP_EXPR_ID)
	{
		string type(i->value.begin(), i->value.end());
		if (memcmp(type.c_str(), "**", 2) == 0)
			return pow(EvaluateExpression(f, i->children.begin(), logCallingFunction, e), EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e));

		else if (memcmp(type.c_str(), "ln", 2) == 0)
			return log(EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e));

	}

	// Unary
	else if (id == exec_grammar::UNARY_EXPR_ID)
	{
		if (*i->value.begin() == '!')
			return ! EvaluateExpression(f, i->children.begin(), logCallingFunction, e);

		else if (*i->value.begin() == '~')
			return ~ ((unsigned long) EvaluateExpression(f, i->children.begin(), logCallingFunction, e));

		else if (*i->value.begin() == '-')
			return - EvaluateExpression(f, i->children.begin(), logCallingFunction, e);

		else if (*i->value.begin() == '`')
			return bitcount((unsigned long) EvaluateExpression(f, i->children.begin(), logCallingFunction, e));
	}

	// Multiplicative
	else if (id == exec_grammar::MULT_EXPR_ID)
	{
		if (*i->value.begin() == '*')
		{
			return EvaluateExpression(f, i->children.begin(), logCallingFunction, e) * EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e);
		}
		else if (*i->value.begin() == '/')
		{
			double div = EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e);
			if (div != 0)
			{
				result = EvaluateExpression(f, i->children.begin(), logCallingFunction, e) / div;
			}
			else
			{
				result = 0;
				*e = ERR_DIV_ZERO;
			}

			return result;
		}
		else if (*i->value.begin() == '%')
		{
			unsigned long mod = (unsigned long) EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e);
			if (mod != 0)
			{
				result = (double) ((unsigned long) EvaluateExpression(f, i->children.begin(), logCallingFunction, e) % mod);
			}
			else
			{
				result = 0;
				*e = ERR_DIV_ZERO;
			}

			return result;
		}
	}

	// Additive
	else if (id == exec_grammar::ADD_EXPR_ID)
	{
		if (*i->value.begin() == '+')
			return EvaluateExpression(f, i->children.begin(), logCallingFunction, e) + EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e);

		else if (*i->value.begin() == '-')
			return EvaluateExpression(f, i->children.begin(), logCallingFunction, e) - EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e);
	}

	// Shift
	else if (id == exec_grammar::SHIFT_EXPR_ID)
	{
		string type(i->value.begin(), i->value.end());
		if (memcmp(type.c_str(), "<<", 2) == 0)
			return (double) ((unsigned long) EvaluateExpression(f, i->children.begin(), logCallingFunction, e) << (unsigned long) EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e));

		else if (memcmp(type.c_str(), ">>", 2) == 0)
			return (double) ((unsigned long) EvaluateExpression(f, i->children.begin(), logCallingFunction, e) >> (unsigned long) EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e));
	}

	// Relational
	else if (id == exec_grammar::RELATIONAL_EXPR_ID)
	{
		string type(i->value.begin(), i->value.end());
		if (memcmp(type.c_str(), "<=", 2) == 0)
			return (IsSmallerOrEqual(EvaluateExpression(f, i->children.begin(), logCallingFunction, e), 
				EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e)));

		else if (memcmp(type.c_str(), ">=", 2) == 0)
			return (IsGreaterOrEqual(EvaluateExpression(f, i->children.begin(), logCallingFunction, e), 
				EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e)));			

		else if (memcmp(type.c_str(), "<", 1) == 0)
			return (IsSmaller(EvaluateExpression(f, i->children.begin(), logCallingFunction, e), 
				EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e)));
			
		else if (memcmp(type.c_str(), ">", 1) == 0)
			return (IsGreater(EvaluateExpression(f, i->children.begin(), logCallingFunction, e), 
				EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e)));
	}

	// Equality
	else if (id == exec_grammar::EQUALITY_EXPR_ID)
	{
		string type(i->value.begin(), i->value.end());
		if (memcmp(type.c_str(), "==", 2) == 0)
			return (IsEqual(EvaluateExpression(f, i->children.begin(), logCallingFunction, e), 
				EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e)));

		else if (memcmp(type.c_str(), "!=", 2) == 0)
			return (!IsEqual(EvaluateExpression(f, i->children.begin(), logCallingFunction, e), 
				EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e)));
	}

	// Binary AND
	else if (id == exec_grammar::BINARY_AND_EXPR_ID)
	{
		return (double) ((unsigned long) EvaluateExpression(f, i->children.begin(), logCallingFunction, e) & (unsigned long) EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e));
	}

	// Binary XOR
	else if (id == exec_grammar::BINARY_XOR_EXPR_ID)
	{
		return (double) ((unsigned long) EvaluateExpression(f, i->children.begin(), logCallingFunction, e) ^ (unsigned long) EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e));
	}

	// Binary OR
	else if (id == exec_grammar::BINARY_OR_EXPR_ID)
	{
		return (double) ((unsigned long) EvaluateExpression(f, i->children.begin(), logCallingFunction, e) | (unsigned long) EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e));
	}

	// Logical AND
	else if (id == exec_grammar::LOGICAL_AND_EXPR_ID)
	{
		return (double) ( EvaluateExpression(f, i->children.begin(), logCallingFunction, e)>0 && EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e)>0 );
	}

	// Logical XOR
	else if (id == exec_grammar::LOGICAL_XOR_EXPR_ID)
	{
		return (double) ( EvaluateExpression(f, i->children.begin(), logCallingFunction, e) != EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e) );
	}

	// Logical OR
	else if (id == exec_grammar::LOGICAL_OR_EXPR_ID)
	{
		return (double) ( EvaluateExpression(f, i->children.begin(), logCallingFunction, e)>0 || EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e)>0 );
	}

	// Conditional
	else if (id == exec_grammar::COND_EXPR_ID)
	{
		if (EvaluateExpression(f, i->children.begin(), logCallingFunction, e))
			return EvaluateExpression(f, i->children.begin()+1, logCallingFunction, e);
		else
			return EvaluateExpression(f, i->children.begin()+3, logCallingFunction, e);
	}

	// Error
	*e = ERR_INVALID_EXPR;

	return 0;
}

double CGrammar::TryToEvaluateSymbolAsOpenPPLSymbol(CFormula * const f, string sym, CEvalInfoFunction **logCallingFunction, int *e)
{
	CString symbol_name = sym.c_str();
	if (symbol_name.Left(k_lenth_of_open_ppl_symbol_prefix) == k_open_ppl_symbol_prefix) 
	{
		// OpenPPL-prefix already present.
		// This shouldn't happen, as it should be caught 
		// by the check for f$ symbols.
		// We check it anyway and throw an error.
		*e = ERR_INVALID_SYM;
		return 0.0;
	}
	else
	{
		// Add OpenPPL-prefix and try to evaluate again recursively.
		symbol_name = CString(k_open_ppl_symbol_prefix) + sym.c_str();
		double symbol_value = EvaluateSymbol(f, std::string(symbol_name), logCallingFunction, e);
		if (e == SUCCESS)
		{
			return symbol_value;
		}
		else
		{
			// Symbol still unknown, even with OpenPPL_prefix -> error
			*e = ERR_INVALID_SYM;
			return 0.0;
		}
	}
}

double CGrammar::EvaluateSymbol(CFormula * const f, string sym, CEvalInfoFunction **logCallingFunction, int *e)
{
	double			result = 0.0;
	char			f$func[k_max_number_of_players] = {0}; 
	const char		*ranks = "  23456789TJQKA";
	int				rank_temp = 0;
	
	int				$$pc0 = p_symbol_engine_cards->$$pc(0);
	int				$$pc1 = p_symbol_engine_cards->$$pc(1);
	int				rank0 = RankOfCard($$pc0);
	int				rank1 = RankOfCard($$pc1);
	bool			sym_issuited = p_symbol_engine_cards->issuited();

	// "e" literal
	if (strcmp(sym.c_str(), "e")==0)
	{
		return (double) M_E;
	}

	// f$$ symbols (hand multiplexor)
	else if (memcmp(sym.c_str(), "f$$", 3)==0)
	{
		if (rank1>rank0)
		{
			SwapInts(&rank0, &rank1);
		}

		// Which form of f$$ is being called determines nature of resultant udf call
		strcpy_s(f$func, 10, "f$");

		if (tolower(sym.c_str()[3])=='x' &&
				strlen(sym.c_str())==4)
		{
			f$func[2] = ranks[rank0];
			f$func[3] = '\0';
		}

		else if (tolower(sym.c_str()[3])=='x' &&
				 tolower(sym.c_str()[4])=='x' &&
				 strlen(sym.c_str())==5)
		{
			f$func[2] = ranks[rank0];
			f$func[3] = ranks[rank1];
			f$func[4] = '\0';
		}

		else if (tolower(sym.c_str()[3])=='x' &&
				 tolower(sym.c_str()[4])=='x' &&
				 tolower(sym.c_str()[5])=='x' &&
				 strlen(sym.c_str())==6)
		{
			f$func[2] = ranks[rank0];
			f$func[3] = ranks[rank1];
			f$func[4] = sym_issuited ? 's' : 'o';
			f$func[5] = '\0';
		}

		else
		{
			*e = ERR_INVALID_F$$_REF;
			return 0;
		}

		// Calculate resultant udf
		result = DoCalcF$symbol(f, f$func, logCallingFunction, false, e);
		if (*e == SUCCESS)
		{
			return result;
		}
		else
		{
			*e = SUCCESS;
			return 0;
		}
	}

	// f$ symbols
	else if (memcmp(sym.c_str(), "f$", 2)==0 && strcmp(sym.c_str(), "f$debug") != 0)
	{
		return DoCalcF$symbol(f, (char *) sym.c_str(), logCallingFunction, false, e);
	}

	// dll$ symbols
	else if (memcmp(sym.c_str(), "dll$", 4)==0)
	{
		// MECHANISM FOR DETECTING INVALID DLL SYMBOLS DOES NOT YET EXIST
		if (1)
		{
			if (p_dll_extension->IsDllLoaded())
			{
				return (p_dll_extension->process_message()) ("query", sym.c_str());
			}
			else
			{
				return 0;
			}
		}
		else
		{
			*e = ERR_INVALID_DLL_SYM;
			return 0;
		}
	}

	//  Perl symbols (starting with "pl_")
	else if (p_perl->IsPerlSymbol(sym.c_str()))
	{
		//  Error checking is done inside the Perl class
		//  At the moment, creating a messagebox on serious errors.
		return p_perl->GetPerlSymbol(sym.c_str());
	}

	// vs$ symbols
	else if (memcmp(sym.c_str(), "vs$", 3)==0)
	{
		return p_versus->GetSymbol(sym.c_str(), e);
	}

	// log$ symbols
	else if (memcmp(sym.c_str(), "log$", 4)==0)
	{
		CLogSymbols  logsymbols;
		return logsymbols.ProcessQuery(sym.c_str(), e);
	}

	// icm_ symbols
	else if (memcmp(sym.c_str(), "icm", 3)==0)
	{
		CICMCalculator		  icm;
		return icm.ProcessQueryICM(sym.c_str()+3, e);
	}

	// Action symbols
	else if (memcmp(sym.c_str(), "ac_", 3)==0)
	{
		CPokerAction	action;
		return action.ProcessQuery(sym.c_str(), e);
	}

	// Memory symbols
	else if (memcmp(sym.c_str(), "me_", 3)==0)
	{
				return p_memory->ProcessQuery(sym.c_str(), logCallingFunction, e);
	}
	   
	// all other symbols
	else
	{
		// Don't return, but first check for success.
		// If there was no success, it might be, 
		// that we try to evaluate an OpenPPL-symbol
		// without prefix (user-input from debug-tab).
		// Then add this prefix and try to evaluate again.
		double symbol_value = p_symbols->GetSymbolVal(sym.c_str(), e);
		if (*e == SUCCESS)
		{
			return symbol_value;
		}
		else
		{
			symbol_value = TryToEvaluateSymbolAsOpenPPLSymbol(f, sym, logCallingFunction, e);
			if (*e == SUCCESS)
			{
				return symbol_value;
			}
			else
			{
				// Unknown symbol.
				// Though we check the syntax, this can still happen
				// by gws-calls from Perl or a DLL, etc.
				WarnAboutUnknownOrOutdatedSymbol(sym.c_str());
				return 0.0;
			}
		}
	}
}

double CGrammar::DoCalcF$symbol(CFormula * const f, const char *symbol, CEvalInfoFunction **logCallingFunction, bool skipCache, int *e)
{
	double	ret = 0.0;

	// Check recursion depth of DoCalcF$symbol 
	// to detect a recursive formula.
	// Decrease _RecursionDepth on every function exit!
	_RecursionDepth++;
	if (_RecursionDepth > _MAX_RECURSION_DEPTH)
	{
		CString error_message = CString(
			"Recursion to deep.\n"
			"Probably endless.\n"
			"Stopping autoplayer.\n"
			"\n"
			"Last function: ") + symbol;
		OH_MessageBox_Error_Warning(error_message, "ERROR");
		p_autoplayer->set_autoplayer_engaged(false);
		_RecursionDepth--;
		return 0.0;
	}

	if (strcmp(symbol, "f$debug") != 0 &&
			strcmp(symbol, "notes") != 0 &&
			strcmp(symbol, "dll") != 0)
	{
		for (int i=0; i<(int) f->formula()->mFunction.GetSize(); i++)
		{
			if (strcmp(f->formula()->mFunction[i].func, symbol)==0)
			{
				if (f->formula()->mFunction[i].fresh == true && !skipCache) // !! skipcache?
				{
					ret = f->formula()->mFunction[i].cache;

					if (logCallingFunction && *logCallingFunction) 
					{
						if (!(*logCallingFunction)->m_CalledFunctions.FindFunction(symbol))
							(*logCallingFunction)->m_CalledFunctions.Add(new CEvalInfoFunction(symbol, true, ret));
					} 
					
					else if (logCallingFunction && !*logCallingFunction)
						*logCallingFunction = new CEvalInfoFunction(symbol, true, ret);
					
					_RecursionDepth--;
					return ret;
				}
				else
				{
					if (!logCallingFunction)
						ret = EvaluateTree(f, f->formula()->mFunction[i].tpi, NULL, e);
					else 
					{
						CEvalInfoFunction *newFunc = new CEvalInfoFunction(symbol);

						ret = EvaluateTree(f, f->formula()->mFunction[i].tpi, &newFunc, e);

						newFunc->m_Line = newFunc->m_Column = 1;

						for (int c=0; c<newFunc->m_Offset; c++)
						{
							if (f->formula()->mFunction[i].func_text.Mid(c, 1)=="\n") 
							{
								newFunc->m_Line++;
								newFunc->m_Column = 1;
							} 
							else
								newFunc->m_Column++;
						}
						newFunc->m_Result = ret;

						if (*logCallingFunction)
							(*logCallingFunction)->m_CalledFunctions.Add(newFunc);
						else
							(*logCallingFunction) = newFunc;
					}

					f->set_func_cache(i, ret);

					if (*e == SUCCESS)
						f->set_func_fresh(i, true);

					_RecursionDepth--;
					return ret;
				}
			}
		}
	}

	*e = ERR_INVALID_FUNC_SYM;

	_RecursionDepth--;
	return 0;
}


////////////////////////
// Parsing functions
CArray<std::string, std::string>  g_parse_symbol_stop_strs;
SFormula		*g_parse_symbol_formula;
CCritSec		m_critsec_parse;

bool CGrammar::ParseString(const CString *s, const SFormula *f, tpi_type *i, int *stopchar)
{
	CSLock lock(m_critsec_parse);	// must come before instantiation of exec_grammar struct on stack

	exec_grammar	gram;
	string			str = "";
	skip_grammar	skip;
	int				c = 0;

	g_parse_symbol_formula = (SFormula *) f;
	g_parse_symbol_stop_strs.RemoveAll();

	str = s->GetString();
	const char *last = str.c_str();
	while (*last) last++;
	
	*i = ast_parse<int_factory_t>(str.c_str(), last, gram, skip, int_factory_t());

	// Copy stop_strs result from global holder back into the class
	_parse_symbol_stop_strs.RemoveAll();
	for (c=0; c<g_parse_symbol_stop_strs.GetCount(); c++)
		_parse_symbol_stop_strs.Add(g_parse_symbol_stop_strs[c]);

	// Figure out if we were successful
	if (i->full)
	{
		iter_t &cur = i->trees.begin();
		iter_t &end = i->trees.end();
		for (; cur != end; ++cur)
			SetOffsets(cur, str.c_str());

		// parsing succeeded
		*stopchar = -1;
		return true;
	}
	else
	{
		// parsing failed
		*stopchar = i->stop - str.c_str();
		return false;
	}
	return true;
}


void CGrammar::SetOffsets(iter_t &i, const char *start)
{
	if (i->value.value() != NULL)
		i->value.value((const char *)(i->value.value()-start));

	iter_t &cur = i->children.begin();

	iter_t &end = i->children.end();

	for (; cur != end; ++cur)
		SetOffsets(cur, start);
}

void CGrammar::ValidateSymbol(const char *begin, const char *end)
{
	int		 e = 0;
	string	sym(begin, end);
	bool	match = false;

	// "e" literal
	if (strcmp(sym.c_str(), "e")==0)
		return;

	// f$$ symbols (hand multiplexor)
	else if (memcmp(sym.c_str(), "f$$", 3)==0)
	{
		if (strlen(sym.c_str())==4)
		{
			if (tolower(sym.c_str()[3])!='x')
			{
				g_parse_symbol_stop_strs.Add(sym);
				return;
			}
		}
		else if (strlen(sym.c_str())==5)
		{
			if (tolower(sym.c_str()[3])!='x' || tolower(sym.c_str()[4])!='x')
			{
				g_parse_symbol_stop_strs.Add(sym);
				return;
			}
		}
		else if (strlen(sym.c_str())==6)
		{
			if (tolower(sym.c_str()[3])!='x' || tolower(sym.c_str()[4])!='x' || tolower(sym.c_str()[5])!='x')
			{
				g_parse_symbol_stop_strs.Add(sym);
				return;
			}
		}
		else
		{
			g_parse_symbol_stop_strs.Add(sym);
			return;
		}
	}

	// f$ symbols
	else if (memcmp(sym.c_str(), "f$", 2)==0)
	{
		match = false;
		for (int i=0; i<g_parse_symbol_formula->mFunction.GetSize() && match==false; i++)
			if (strcmp(g_parse_symbol_formula->mFunction[i].func.GetString(), sym.c_str())==0)
				match = true;

		if (!match)
			g_parse_symbol_stop_strs.Add(sym);

		return;
	}

	// dll$ symbols
	else if (memcmp(sym.c_str(), "dll$", 4)==0)
	{
		// MECHANISM FOR DETECTING INVALID DLL SYMBOLS DOES NOT YET EXIST
		return;
	}

	//  Perl symbols (starting with "pl_")
	else if (memcmp(sym.c_str(), "pl_", 3)==0)
	{
		if (!p_perl->IsPerlSymbol(sym.c_str()))
			g_parse_symbol_stop_strs.Add(sym);

		return;
	}

	// vs$ symbols
	else if (memcmp(sym.c_str(), "vs$", 3)==0)
	{
		e = SUCCESS;
		p_versus->GetSymbol(sym.c_str(), &e);

		if (e != SUCCESS)
			g_parse_symbol_stop_strs.Add(sym);

		return;
	}

	// log$ symbols
	else if (memcmp(sym.c_str(), "log$", 4)==0)
	{
		CLogSymbols  logsymbols;
		e = SUCCESS;
		logsymbols.ProcessQuery(sym.c_str(), &e);

		if (e != SUCCESS)
			g_parse_symbol_stop_strs.Add(sym);

		return;
	}

	// icm_ symbols
	else if (memcmp(sym.c_str(), "icm", 3)==0)
	{
		CICMCalculator		icm;
		e = SUCCESS;
		icm.ProcessQueryICM(sym.c_str()+3, &e);

		if (e != SUCCESS)
			g_parse_symbol_stop_strs.Add(sym);

		return;
	}

	// Action symbols
	else if (memcmp(sym.c_str(), "ac_", 3)==0)
	{
		CPokerAction	action;
		e = SUCCESS;
		action.ProcessQuery(sym.c_str(), &e);

		if (e != SUCCESS)
			g_parse_symbol_stop_strs.Add(sym);

		return;
	}

	// Memory symbols
	else if (memcmp(sym.c_str(), "me_st_", 6)==0 ||
			 memcmp(sym.c_str(), "me_re_", 6)==0)
	{
		// Memory symbols may need valid parse trees for all formulas, an since we 
		// are just parsing here, we may not have those trees yet.

		//e = SUCCESS;
		//p_memory->ProcessQuery(sym.c_str(), &e);

		//if (e != SUCCESS)
		//	g_parse_symbol_stop_strs.Add(sym);

		return;
	}

	// all other symbols
	else
	{
		e = SUCCESS;
		p_symbols->GetSymbolVal(sym.c_str(), &e);

		if (e != SUCCESS)
			g_parse_symbol_stop_strs.Add(sym);

		return;
	}
}
