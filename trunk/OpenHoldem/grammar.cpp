#include "stdafx.h"
#include <strstream>

#include "grammar.h"
#include "symbols.h"
#include "debug.h"
#include "global.h"
#include "versus.h"
#include "threads.h"
#include "ICMCalculator.h"
//  2008.02.27 by THF
#include "Perl.hpp"
#include "action.h"
#include "myhand.h"
#include "memory.h"
#include "logsymbols.h"

//#include <boost/spirit/tree/tree_to_xml.hpp>

////////////////////////////////////////////////////////////////////////////


void SetPosition(parse_tree_match_t::node_t &node, 
						const char *begin, 
						const char *end)
{
    __SEH_HEADER

	node.value.value(begin);

	__SEH_LOGFATAL("grammar - SetPosition\n");
}

void SymbolValidation(const char *begin, const char *end)
{
    __SEH_HEADER

	int		i, e;
	string	sym(begin, end);
	bool	match;

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
				global.parse_symbol_stop_strs.Add(sym);
				return;
			}
		}
        else if (strlen(sym.c_str())==5)
		{
			if (tolower(sym.c_str()[3])!='x' || tolower(sym.c_str()[4])!='x')
			{
				global.parse_symbol_stop_strs.Add(sym);
				return;
			}
		}
        else if (strlen(sym.c_str())==6)
		{
			if (tolower(sym.c_str()[3])!='x' || tolower(sym.c_str()[4])!='x' || tolower(sym.c_str()[5])!='x')
			{
				global.parse_symbol_stop_strs.Add(sym);
				return;
			}
		}
        else
        {
			global.parse_symbol_stop_strs.Add(sym);
			return;
        }
    }

    // f$ symbols
    else if (memcmp(sym.c_str(), "f$", 2)==0)
    {
		match = false;
		for (i=0; i<global.parse_symbol_formula->mFunction.GetSize() && match==false; i++)
			if (strcmp(global.parse_symbol_formula->mFunction[i].func.GetString(), sym.c_str())==0)
				match = true;

		if (!match)
			global.parse_symbol_stop_strs.Add(sym);

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
		if (!the_Perl_Interpreter.is_Perl_Symbol(sym.c_str()))
			global.parse_symbol_stop_strs.Add(sym);

		return;
    }

    // vs$ symbols
    else if (memcmp(sym.c_str(), "vs$", 3)==0)
    {
		e = SUCCESS;
        versus.get_symbol(sym.c_str(), &e);

		if (e != SUCCESS)
			global.parse_symbol_stop_strs.Add(sym);

		return;
    }

    // log$ symbols
    else if (memcmp(sym.c_str(), "log$", 4)==0)
    {
        LogSymbols  logsymbols;
		e = SUCCESS;
        logsymbols.process_query(sym.c_str(), &e);

		if (e != SUCCESS)
			global.parse_symbol_stop_strs.Add(sym);

		return;
    }

    // icm_ symbols
    else if (memcmp(sym.c_str(), "icm", 3)==0)
    {
        ICM		icm;
		e = SUCCESS;
        icm.ProcessQueryICM(sym.c_str()+3, &e);

		if (e != SUCCESS)
			global.parse_symbol_stop_strs.Add(sym);

		return;
	}

    // Action symbols
    else if (memcmp(sym.c_str(), "ac_", 3)==0)
    {
        Action	action;
		e = SUCCESS;
        action.process_query(sym.c_str(), &e);

		if (e != SUCCESS)
			global.parse_symbol_stop_strs.Add(sym);

		return;
    }

    // MyHand symbols
    else if (memcmp(sym.c_str(), "mh_", 3)==0)
    {
        MyHand	myhand;
		e = SUCCESS;
        myhand.process_query(sym.c_str(), &e);

		if (e != SUCCESS)
			global.parse_symbol_stop_strs.Add(sym);

		return;
    }

    // Memory symbols
    else if (memcmp(sym.c_str(), "me_st_", 6)==0 ||
			 memcmp(sym.c_str(), "me_re_", 6)==0)
    {
		// Memory symbols may need valid parse trees for all formulas, an since we 
		// are just parsing here, we may not have those trees yet.

		//e = SUCCESS;
		//memory.process_query(sym.c_str(), &e);

		//if (e != SUCCESS)
		//	global.parse_symbol_stop_strs.Add(sym);

        return;
    }

    // all other symbols
    else
    {
		e = SUCCESS;
        symbols.GetSymbolVal(sym.c_str(), &e);

		if (e != SUCCESS)
			global.parse_symbol_stop_strs.Add(sym);

        return;
	}

	__SEH_LOGFATAL("grammar - SymbolValidation\n");
}

void setOffsets(iter_t &i, const char *start)
{
    __SEH_HEADER

	if (i->value.value() != NULL)
		i->value.value((const char *)(i->value.value()-start));
	iter_t &cur = i->children.begin();
	iter_t &end = i->children.end();
	for (; cur != end; ++cur)
		setOffsets(cur, start);

	__SEH_LOGFATAL("grammar - setOffsets\n");
}

bool parse (CString *s, tree_parse_info<const char *, int_factory_t> *i, int *stopchar)
{
    __SEH_HEADER
    exec_grammar	gram;
    string			str;
    skip_grammar	skip;

	str = s->GetString();
	const char *last = str.c_str();
    while (*last) last++;
	
    *i = ast_parse<int_factory_t>(str.c_str(), last, gram, skip, int_factory_t());

	if (i->full)
    {
		iter_t &cur = i->trees.begin();
		iter_t &end = i->trees.end();
		for (; cur != end; ++cur)
			setOffsets(cur, str.c_str());

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

    __SEH_LOGFATAL("grammar - parse\n");
}
double do_calc_f$symbol(SFormula *f, char *symbol, CEvalInfoFunction **logCallingFunction, bool skipCache, int *e)
{
    __SEH_HEADER
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
                if (f->mFunction[i].fresh == true && !global.preferences.disable_caching && !skipCache)
                {
                    ret = f->mFunction[i].cache;
					if (logCallingFunction && *logCallingFunction) {
						if (!(*logCallingFunction)->m_CalledFunctions.FindFunction(symbol))
							(*logCallingFunction)->m_CalledFunctions.Add(new CEvalInfoFunction(symbol, true, ret));
					} else if (logCallingFunction && !*logCallingFunction)
						*logCallingFunction = new CEvalInfoFunction(symbol, true, ret);
                    LeaveCriticalSection(&cs_calc_f$symbol);
                    return ret;
                }
                else
                {
					if (!logCallingFunction)
	                    ret = evaluate(f, f->mFunction[i].tpi, NULL, e);
					else {
						CEvalInfoFunction *newFunc = new CEvalInfoFunction(symbol);
	                    ret = evaluate(f, f->mFunction[i].tpi, &newFunc, e);
						newFunc->m_Line = newFunc->m_Column = 1;
						for (int c=0; c<newFunc->m_Offset; c++)
						{
							if (f->mFunction[i].func_text.Mid(c, 1)=="\n") {
								newFunc->m_Line++;
								newFunc->m_Column = 1;
							} else
								newFunc->m_Column++;
						}
						newFunc->m_Result = ret;
						if (*logCallingFunction)
							(*logCallingFunction)->m_CalledFunctions.Add(newFunc);
						else
							(*logCallingFunction) = newFunc;
					}
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

	__SEH_LOGFATAL("grammar - do_calc_f$symbol\n");
}

double calc_f$symbol(SFormula *f, char *symbol, int *e)
{ 
	return calc_f$symbol(f, symbol, false, e); 
}

double calc_f$symbol(SFormula *f, char *symbol, bool log, int *e)
{
    __SEH_HEADER
	CEvalInfoFunction *logCallingFunction = NULL;
	double ret = do_calc_f$symbol(f, symbol, (log ? &logCallingFunction : NULL), log, e);

	if (logCallingFunction) {
		logCallingFunction->DumpFunction(0);
		delete logCallingFunction;
	}

	return ret;
    __SEH_LOGFATAL("grammar - calc_f$symbol\n");
}

double evaluate(SFormula *f, tree_parse_info<const char *, int_factory_t> info, CEvalInfoFunction **logCallingFunction, int *e)
{
    __SEH_HEADER
    return eval_expression(f, info.trees.begin(), logCallingFunction, e);
    __SEH_LOGFATAL("grammar - evaluate\n");
}

double do_eval_expression(SFormula *f, iter_t const& i, CEvalInfoFunction **logCallingFunction, int *e)
{
    __SEH_HEADER

    double						result;
	boost::spirit::parser_id	id = i->value.id();

    // Bounce errors up the stack
    if (*e != SUCCESS)  return 0;

	if (logCallingFunction && *logCallingFunction && (*logCallingFunction)->m_Offset < (int)i->value.value())
		(*logCallingFunction)->m_Offset = (int)i->value.value();

    // Symbols
    if (id == exec_grammar::SYMBOL_ID)
    {
        string sym(i->value.begin(), i->value.end());
		return eval_symbol(f, sym, logCallingFunction, e);
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
            return pow(eval_expression(f, i->children.begin(), logCallingFunction, e), eval_expression(f, i->children.begin()+1, logCallingFunction, e));

        else if (memcmp(type.c_str(), "ln", 2) == 0)
            return log(eval_expression(f, i->children.begin()+1, logCallingFunction, e));

    }

    // Unary
    else if (id == exec_grammar::UNARY_EXPR_ID)
    {
        if (*i->value.begin() == '!')
            return ! eval_expression(f, i->children.begin(), logCallingFunction, e);

        else if (*i->value.begin() == '~')
            return ~ ((unsigned long) eval_expression(f, i->children.begin(), logCallingFunction, e));

        else if (*i->value.begin() == '-')
            return - eval_expression(f, i->children.begin(), logCallingFunction, e);

        else if (*i->value.begin() == '`')
            return bitcount((unsigned long) eval_expression(f, i->children.begin(), logCallingFunction, e));
    }

    // Multiplicative
    else if (id == exec_grammar::MULT_EXPR_ID)
    {
        if (*i->value.begin() == '*')
        {
            return eval_expression(f, i->children.begin(), logCallingFunction, e) * eval_expression(f, i->children.begin()+1, logCallingFunction, e);
        }
        else if (*i->value.begin() == '/')
        {
            double div = eval_expression(f, i->children.begin()+1, logCallingFunction, e);
            if (div != 0)
            {
                result = eval_expression(f, i->children.begin(), logCallingFunction, e) / div;
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
            unsigned long mod = (unsigned long) eval_expression(f, i->children.begin()+1, logCallingFunction, e);
            if (mod != 0)
            {
                result = (double) ((unsigned long) eval_expression(f, i->children.begin(), logCallingFunction, e) % mod);
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
            return eval_expression(f, i->children.begin(), logCallingFunction, e) + eval_expression(f, i->children.begin()+1, logCallingFunction, e);

        else if (*i->value.begin() == '-')
            return eval_expression(f, i->children.begin(), logCallingFunction, e) - eval_expression(f, i->children.begin()+1, logCallingFunction, e);
    }

    // Shift
    else if (id == exec_grammar::SHIFT_EXPR_ID)
    {
        string type(i->value.begin(), i->value.end());
        if (memcmp(type.c_str(), "<<", 2) == 0)
            return (double) ((unsigned long) eval_expression(f, i->children.begin(), logCallingFunction, e) << (unsigned long) eval_expression(f, i->children.begin()+1, logCallingFunction, e));

        else if (memcmp(type.c_str(), ">>", 2) == 0)
            return (double) ((unsigned long) eval_expression(f, i->children.begin(), logCallingFunction, e) >> (unsigned long) eval_expression(f, i->children.begin()+1, logCallingFunction, e));
    }

    // Relational
    else if (id == exec_grammar::RELATIONAL_EXPR_ID)
    {
        string type(i->value.begin(), i->value.end());
        if (memcmp(type.c_str(), "<=", 2) == 0)
            return eval_expression(f, i->children.begin(), logCallingFunction, e) <= eval_expression(f, i->children.begin()+1, logCallingFunction, e);

        else if (memcmp(type.c_str(), ">=", 2) == 0)
            return eval_expression(f, i->children.begin(), logCallingFunction, e) >= eval_expression(f, i->children.begin()+1, logCallingFunction, e);

        else if (memcmp(type.c_str(), "<", 1) == 0)
            return eval_expression(f, i->children.begin(), logCallingFunction, e) < eval_expression(f, i->children.begin()+1, logCallingFunction, e);

        else if (memcmp(type.c_str(), ">", 1) == 0)
            return eval_expression(f, i->children.begin(), logCallingFunction, e) > eval_expression(f, i->children.begin()+1, logCallingFunction, e);
    }

    // Equality
    else if (id == exec_grammar::EQUALITY_EXPR_ID)
    {
        string type(i->value.begin(), i->value.end());
        if (memcmp(type.c_str(), "==", 2) == 0)
            return eval_expression(f, i->children.begin(), logCallingFunction, e) == eval_expression(f, i->children.begin()+1, logCallingFunction, e);

        else if (memcmp(type.c_str(), "!=", 2) == 0)
            return eval_expression(f, i->children.begin(), logCallingFunction, e) != eval_expression(f, i->children.begin()+1, logCallingFunction, e);
    }

    // Binary AND
    else if (id == exec_grammar::BINARY_AND_EXPR_ID)
    {
        return (double) ((unsigned long) eval_expression(f, i->children.begin(), logCallingFunction, e) & (unsigned long) eval_expression(f, i->children.begin()+1, logCallingFunction, e));
    }

    // Binary XOR
    else if (id == exec_grammar::BINARY_XOR_EXPR_ID)
    {
        return (double) ((unsigned long) eval_expression(f, i->children.begin(), logCallingFunction, e) ^ (unsigned long) eval_expression(f, i->children.begin()+1, logCallingFunction, e));
    }

    // Binary OR
    else if (id == exec_grammar::BINARY_OR_EXPR_ID)
    {
        return (double) ((unsigned long) eval_expression(f, i->children.begin(), logCallingFunction, e) | (unsigned long) eval_expression(f, i->children.begin()+1, logCallingFunction, e));
    }

    // Logical AND
    else if (id == exec_grammar::LOGICAL_AND_EXPR_ID)
    {
        return (double) ( eval_expression(f, i->children.begin(), logCallingFunction, e)>0 && eval_expression(f, i->children.begin()+1, logCallingFunction, e)>0 );
    }

    // Logical XOR
    else if (id == exec_grammar::LOGICAL_XOR_EXPR_ID)
    {
        return (double) ( eval_expression(f, i->children.begin(), logCallingFunction, e) != eval_expression(f, i->children.begin()+1, logCallingFunction, e) );
    }

    // Logical OR
    else if (id == exec_grammar::LOGICAL_OR_EXPR_ID)
    {
        return (double) ( eval_expression(f, i->children.begin(), logCallingFunction, e)>0 || eval_expression(f, i->children.begin()+1, logCallingFunction, e)>0 );
    }

    // Conditional
    else if (id == exec_grammar::COND_EXPR_ID)
    {
        if (eval_expression(f, i->children.begin(), logCallingFunction, e))
            return eval_expression(f, i->children.begin()+1, logCallingFunction, e);
        else
            return eval_expression(f, i->children.begin()+3, logCallingFunction, e);
    }

    // Error
    *e = ERR_INVALID_EXPR;
    return 0;

    __SEH_LOGFATAL("grammar - do_eval_expression :\n");
}

double eval_symbol(SFormula *f, string sym, CEvalInfoFunction **logCallingFunction, int *e)
{
    double		result;
    char		f$func[10];
    const char	*ranks = "  23456789TJQKA";
    int			rank0, rank1, rank_temp;

    // "e" literal
    if (strcmp(sym.c_str(), "e")==0)
    {
        return (double) M_E;
    }

    // f$$ symbols (hand multiplexor)
    else if (memcmp(sym.c_str(), "f$$", 3)==0)
    {
        // Get our card ranks
        rank0 = (((int)(symbols.sym.$$pc[0]))>>4)&0x0f;
        rank1 = (((int)(symbols.sym.$$pc[1]))>>4)&0x0f;

        if (rank1>rank0)
        {
            rank_temp=rank0;
            rank0=rank1;
            rank1=rank_temp;
        }

        // Which form of f$$ is being called determines nature of resultant udf call
        strcpy(f$func, "f$");

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
            f$func[4] = symbols.sym.issuited ? 's' : 'o';
            f$func[5] = '\0';
        }

        else
        {
            *e = ERR_INVALID_F$$_REF;
            return 0;
        }

        // Calculate resultant udf
        result = do_calc_f$symbol(f, f$func, logCallingFunction, false, e);
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
        return do_calc_f$symbol(f, (char *) sym.c_str(), logCallingFunction, false, e);
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

    //  2008.02.27 by THF
    //  Perl symbols (starting with "pl_")
    else if (the_Perl_Interpreter.is_Perl_Symbol(sym.c_str()))
    {
        //  Error checking is done inside the Perl class
        //  A.t.m. creating a messagebox on serious errors.
        return the_Perl_Interpreter.get_Perl_Symbol(sym.c_str());
    }

    // vs$ symbols
    else if (memcmp(sym.c_str(), "vs$", 3)==0)
    {
        return versus.get_symbol(sym.c_str(), e);
    }

    // log$ symbols
    else if (memcmp(sym.c_str(), "log$", 4)==0)
    {
        LogSymbols  logsymbols;
        return logsymbols.process_query(sym.c_str(), e);
    }

    // icm_ symbols
    else if (memcmp(sym.c_str(), "icm", 3)==0)
    {
        ICM		icm;
        return icm.ProcessQueryICM(sym.c_str()+3, e);
    }

    // Action symbols
    else if (memcmp(sym.c_str(), "ac_", 3)==0)
    {
        Action	action;
        return action.process_query(sym.c_str(), e);
    }

    // MyHand symbols
    else if (memcmp(sym.c_str(), "mh_", 3)==0)
    {
        MyHand	myhand;
        return myhand.process_query(sym.c_str(), e);
    }

    // Memory symbols
    else if (memcmp(sym.c_str(), "me_", 3)==0)
    {
        return memory.process_query(sym.c_str(), e);
    }

    // all other symbols
    else
    {
        return symbols.GetSymbolVal(sym.c_str(), e);
    }

	return 0;

	__SEH_LOGFATAL("grammar - eval_symbol\n");
}

double eval_expression(SFormula *f, iter_t const& i, CEvalInfoFunction **logCallingFunction, int *e)
{
    __SEH_HEADER

    string sym(i->value.begin(), i->value.end());
	parser_id tp = i->value.id();

	double ret = do_eval_expression(f, i, logCallingFunction, e);

	if (logCallingFunction && *logCallingFunction && tp == exec_grammar::SYMBOL_ID) {
		if (!(*logCallingFunction)->m_SymbolsUsed.FindSymbol(sym.c_str()) && !(*logCallingFunction)->m_CalledFunctions.FindFunction(sym.c_str()))
			(*logCallingFunction)->m_SymbolsUsed.Add(new CEvalInfoSymbol(sym.c_str(), ret));
	}

	return ret;

	__SEH_LOGFATAL("grammar - eval_expression\n");
}

CEvalInfoFunctionArray::~CEvalInfoFunctionArray()
{
	for (int i=0;i<GetSize();i++)
		delete GetAt(i);
	RemoveAll();
}

void CEvalInfoFunctionArray::DumpFunctionArray(int indent)
{
    __SEH_HEADER
	indent++;
	for (int i=0;i<GetSize();i++)
		GetAt(i)->DumpFunction(indent);
    __SEH_LOGFATAL("CEvalInfoFunctionArray::DumpFunctionArray\n");
}

void CEvalInfoSymbol::DumpSymbol(int indent)
{
    __SEH_HEADER
	CString message, format;
	if (indent > 0) {
		format.Format("%%%ds%%s=%%.2f", indent*3);
		message.Format(format, "", m_Symbol, m_Value);
	} else
		message.Format("%s=%.2f", m_Symbol, m_Value);
    symbols.symboltrace_collection.Add(message);
    __SEH_LOGFATAL("CEvalInfoSymbol::DumpSymbol\n");
};
void CEvalInfoSymbolArray::DumpSymbolArray(int indent)
{
    __SEH_HEADER
	indent++;
	for (int i=0;i<GetSize();i++)
		GetAt(i)->DumpSymbol(indent);
    __SEH_LOGFATAL("CEvalInfoSymbolArray::DumpSymbolArray\n");
}

void CEvalInfoFunction::DumpFunction(int indent)
{
    __SEH_HEADER
	CString message, format, space;
	if (indent > 0) {
		format.Format("%%%ds", indent*3);
		space.Format(format, "");
	}
	if (m_Cached)
		message.Format("%s%s=%.2f [Cached]", space, m_FunctionName, m_Result);
	else
		message.Format("%s%s=%.2f [Line: %d, Col: %d]", space, m_FunctionName, m_Result, m_Line, m_Column);
    symbols.symboltrace_collection.Add(message);

	m_CalledFunctions.DumpFunctionArray(indent+1);
	m_SymbolsUsed.DumpSymbolArray(indent+1);
    __SEH_LOGFATAL("CEvalInfoFunction::DumpFunction\n");
}

CEvalInfoSymbolArray::~CEvalInfoSymbolArray()
{
	for (int i=0;i<GetSize();i++)
		delete GetAt(i);
	RemoveAll();
}

CEvalInfoFunction *CEvalInfoFunctionArray::FindFunction(const char *name) 
{
    __SEH_HEADER
	for (int i=0;i<GetSize();i++) {
		if (!GetAt(i)->m_FunctionName.Compare(name))
			return GetAt(i);
	}
	return NULL;
    __SEH_LOGFATAL("CEvalInfoFunctionArray::FindFunction\n");
}
