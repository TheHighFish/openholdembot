#include "stdafx.h"
#include <strstream>

#include "grammar.h"
#include "symbols.h"
#include "debug.h"
#include "global.h"
#include "dll_extension.h"
#include "versus.h"
#include "threads.h"
#include "ICMCalculator.h"
#include "Transform.h"

using namespace std;
using namespace boost::spirit;
//#include <boost/spirit/tree/tree_to_xml.hpp>

////////////////////////////////////////////////////////////////////////////

bool parse (CString *s, tree_parse_info<> *i, int *stopchar) 
{
#ifdef SEH_ENABLE
	try {
#endif
		exec_grammar	gram;
		string			str;
		skip_grammar	skip;

		str = s->GetString();

		*i = ast_parse(str.c_str(), gram, skip);

		if (i->full) 
		{
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
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("grammar - parse\n"); 
		throw;
	}
#endif
}

double calc_f$symbol(SFormula *f, char *symbol, int *e)
{
#ifdef SEH_ENABLE
	try {
#endif
		int		i;
		double	ret;

		EnterCriticalSection(&cs_calc_f$symbol);

		if (strcmp(symbol, "f$debug") != 0 &&
			strcmp(symbol, "notes") != 0 &&
			strcmp(symbol, "dll") != 0)
		{
			for (i=0; i<(int) f->mFunction.GetSize(); i++) 
			{
				if (strcmp(f->mFunction[i].func, symbol)==0)
				{
					if (f->mFunction[i].fresh == true && !global.preferences.disable_caching)
					{
						ret = f->mFunction[i].cache;
						LeaveCriticalSection(&cs_calc_f$symbol);
						return ret;
					}
					else 
					{
						ret = evaluate(f, f->mFunction[i].tpi, e);
						f->mFunction[i].cache = ret;
						if (*e == SUCCESS)
							f->mFunction[i].fresh = true;
						LeaveCriticalSection(&cs_calc_f$symbol);
						return ret;
					}
				}
			}
		}

		*e = ERR_INVALID_FUNC_SYM;
		LeaveCriticalSection(&cs_calc_f$symbol);

		return 0;
		
#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("grammar - calc_f$symbol\n"); 
		throw;
	}
#endif
}


double evaluate(SFormula *f, tree_parse_info<> info, int *e) 
{
#ifdef SEH_ENABLE
	try {
#endif

		return eval_expression(f, info.trees.begin(), e);

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("grammar - evaluate :\n"); 
		throw;
	}
#endif
}

double eval_expression(SFormula *f, iter_t const& i, int *e) 
{
#ifdef SEH_ENABLE
	try {
#endif
		double	result;

		// Bounce errors up the stack
		if (*e != SUCCESS)  return 0;

		// Symbols
		if (i->value.id()==exec_grammar::SYMBOL_ID) 
		{
			string sym(i->value.begin(), i->value.end());

			// "e" literal
			if (strcmp(sym.c_str(), "e")==0) 
			{
				return (double) M_E;
			}

			// f$ symbols
			else if (memcmp(sym.c_str(), "f$", 2)==0 && strcmp(sym.c_str(), "f$debug") != 0) 
			{
				return calc_f$symbol(f, (char *) sym.c_str(), e);
			}

			// dll$ symbols
			else if (memcmp(sym.c_str(), "dll$", 4)==0) 
			{
				// MECHANISM FOR DETECTING INVALID DLL SYMBOLS DOES NOT YET EXIST
				if (1) 
				{
					if (cdll.hMod_dll!=NULL) 
					{
						return (cdll.process_message) ("query", sym.c_str());
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

			// vs$ symbols
			else if (memcmp(sym.c_str(), "vs$", 3)==0) 
			{
				return versus.get_symbol(sym.c_str(), e);
			}

			// icm_ symbols
			else if (memcmp(sym.c_str(), "icm", 3)==0) 
			{
				ICM		icm;
				return icm.ProcessQueryICM(sym.c_str()+3, e);
			}

			// all other symbols
			else 
			{
				return symbols.GetSymbolVal(sym.c_str(), e);
			}
		}

		// Hex constant
		else if (i->value.id() == exec_grammar::INT_CONSTANT_HEX_ID) 
		{
			string number(i->value.begin()+2, i->value.end());
			return (double) strtoul(number.c_str(), 0, 16);
		}

		// Dec constant
		else if (i->value.id() == exec_grammar::INT_CONSTANT_DEC_ID) 
		{
			string number(i->value.begin(), i->value.end());
			return (double) strtoul(number.c_str(), 0, 10);
		}

		// Oct constant
		else if (i->value.id() == exec_grammar::INT_CONSTANT_OCT_ID) 
		{
			string number(i->value.begin()+2, i->value.end());
			return (double) strtoul(number.c_str(), 0, 8);
		}

		// Quad constant
		else if (i->value.id() == exec_grammar::INT_CONSTANT_QUA_ID) 
		{
			string number(i->value.begin()+2, i->value.end());
			return (double) strtoul(number.c_str(), 0, 4);
		}

		// Bin constant
		else if (i->value.id() == exec_grammar::INT_CONSTANT_BIN_ID) 
		{
			string number(i->value.begin()+2, i->value.end());
			return (double) strtoul(number.c_str(), 0, 2);
		}

		// Float constants
		else if (  i->value.id() == exec_grammar::FLOAT_CONSTANT1_ID
				 | i->value.id() == exec_grammar::FLOAT_CONSTANT2_ID
				 | i->value.id() == exec_grammar::FLOAT_CONSTANT3_ID ) 
		{
			string number(i->value.begin(), i->value.end());
			return (double) strtod(number.c_str(), 0);
		}

		// Exponentiation
		else if (i->value.id() == exec_grammar::EXP_EXPR_ID) 
		{
			string type(i->value.begin(), i->value.end());
			if (memcmp(type.c_str(), "**", 2) == 0) 
				return pow(eval_expression(f, i->children.begin(), e), eval_expression(f, i->children.begin()+1, e));

			else if (memcmp(type.c_str(), "ln", 2) == 0) 
				return log(eval_expression(f, i->children.begin(), e));

		}

		// Unary
		else if (i->value.id() == exec_grammar::UNARY_EXPR_ID) 
		{
			if (*i->value.begin() == '!')
				return ! eval_expression(f, i->children.begin(), e);

			else if (*i->value.begin() == '~')
				return ~ ((unsigned long) eval_expression(f, i->children.begin(), e));

			else if (*i->value.begin() == '-')
				return - eval_expression(f, i->children.begin(), e);

			else if (*i->value.begin() == '`')
				return bitcount((unsigned long) eval_expression(f, i->children.begin(), e));
		}

		// Multiplicative
		else if (i->value.id() == exec_grammar::MULT_EXPR_ID) 
		{
			if (*i->value.begin() == '*') 
			{
				return eval_expression(f, i->children.begin(), e) * eval_expression(f, i->children.begin()+1, e);
			}
			else if (*i->value.begin() == '/') 
			{
				if (eval_expression(f, i->children.begin()+1, e) != 0)
				{
					result = eval_expression(f, i->children.begin(), e) / eval_expression(f, i->children.begin()+1, e);
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
				if (eval_expression(f, i->children.begin()+1, e) != 0)
				{
					result = (double) ((unsigned long) eval_expression(f, i->children.begin(), e) % (unsigned long) eval_expression(f, i->children.begin()+1, e));
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
		else if (i->value.id() == exec_grammar::ADD_EXPR_ID) 
		{
			if (*i->value.begin() == '+')
				return eval_expression(f, i->children.begin(), e) + eval_expression(f, i->children.begin()+1, e);

			else if (*i->value.begin() == '-')
				return eval_expression(f, i->children.begin(), e) - eval_expression(f, i->children.begin()+1, e);
		}

		// Shift
		else if (i->value.id() == exec_grammar::SHIFT_EXPR_ID) 
		{
			string type(i->value.begin(), i->value.end());
			if (memcmp(type.c_str(), "<<", 2) == 0)
				return (double) ((unsigned long) eval_expression(f, i->children.begin(), e) << (unsigned long) eval_expression(f, i->children.begin()+1, e));

			else if (memcmp(type.c_str(), ">>", 2) == 0)
				return (double) ((unsigned long) eval_expression(f, i->children.begin(), e) >> (unsigned long) eval_expression(f, i->children.begin()+1, e));
		}
		
		// Relational
		else if (i->value.id() == exec_grammar::RELATIONAL_EXPR_ID) 
		{
			string type(i->value.begin(), i->value.end());
			if (memcmp(type.c_str(), "<=", 2) == 0)
				return eval_expression(f, i->children.begin(), e) <= eval_expression(f, i->children.begin()+1, e);

			else if (memcmp(type.c_str(), ">=", 2) == 0)
				return eval_expression(f, i->children.begin(), e) >= eval_expression(f, i->children.begin()+1, e);

			else if (memcmp(type.c_str(), "<", 1) == 0)
				return eval_expression(f, i->children.begin(), e) < eval_expression(f, i->children.begin()+1, e);

			else if (memcmp(type.c_str(), ">", 1) == 0)
				return eval_expression(f, i->children.begin(), e) > eval_expression(f, i->children.begin()+1, e);
		}
		
		// Equality
		else if (i->value.id() == exec_grammar::EQUALITY_EXPR_ID) 
		{
			string type(i->value.begin(), i->value.end());
			if (memcmp(type.c_str(), "==", 2) == 0)
				return eval_expression(f, i->children.begin(), e) == eval_expression(f, i->children.begin()+1, e);

			else if (memcmp(type.c_str(), "!=", 2) == 0)
				return eval_expression(f, i->children.begin(), e) != eval_expression(f, i->children.begin()+1, e);
		}

		// Binary AND
		else if (i->value.id() == exec_grammar::BINARY_AND_EXPR_ID) 
		{
			return (double) ((unsigned long) eval_expression(f, i->children.begin(), e) & (unsigned long) eval_expression(f, i->children.begin()+1, e));
		}

		// Binary XOR
		else if (i->value.id() == exec_grammar::BINARY_XOR_EXPR_ID) 
		{
			return (double) ((unsigned long) eval_expression(f, i->children.begin(), e) ^ (unsigned long) eval_expression(f, i->children.begin()+1, e));
		}

		// Binary OR
		else if (i->value.id() == exec_grammar::BINARY_OR_EXPR_ID) 
		{
			return (double) ((unsigned long) eval_expression(f, i->children.begin(), e) | (unsigned long) eval_expression(f, i->children.begin()+1, e));
		}

		// Logical AND
		else if (i->value.id() == exec_grammar::LOGICAL_AND_EXPR_ID) 
		{
			return (double) ((unsigned long) eval_expression(f, i->children.begin(), e) && (unsigned long) eval_expression(f, i->children.begin()+1, e));
		}

		// Logical XOR
		else if (i->value.id() == exec_grammar::LOGICAL_XOR_EXPR_ID) 
		{
			return (double) ( eval_expression(f, i->children.begin(), e) != eval_expression(f, i->children.begin()+1, e));
		}

		// Logical OR
		else if (i->value.id() == exec_grammar::LOGICAL_OR_EXPR_ID) 
		{
			return (double) ((unsigned long) eval_expression(f, i->children.begin(), e) || (unsigned long) eval_expression(f, i->children.begin()+1, e));
		}

		// Conditional
		else if (i->value.id() == exec_grammar::COND_EXPR_ID) 
		{
			if (eval_expression(f, i->children.begin(), e))
				return eval_expression(f, i->children.begin()+1, e);

			else
				return eval_expression(f, i->children.begin()+3, e);
		}
		
		// Error
		*e = ERR_INVALID_EXPR;
		return 0;

#ifdef SEH_ENABLE
	}
	catch (...)	 { 
		logfatal("grammar - eval_expression :\n"); 
		throw;
	}
#endif
}

