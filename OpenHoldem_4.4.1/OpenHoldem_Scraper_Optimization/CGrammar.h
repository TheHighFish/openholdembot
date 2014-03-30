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

#ifndef INC_CGRAMMAR_H
#define INC_CGRAMMAR_H

#include <boost/spirit/tree/ast.hpp>
#include "CFormula.h"
#include "CEvalInfo.h"
#include "..\CCritSec\CCritSec.h"

using namespace std;
using namespace boost::spirit;

typedef node_val_data_factory<const char *>				int_factory_t;
typedef char const*										iterator_t;
typedef tree_match<iterator_t, int_factory_t>			parse_tree_match_t;
typedef parse_tree_match_t::tree_iterator				iter_t;
typedef	tree_parse_info<const char *, int_factory_t>	tpi_type;

class CGrammar 
{

public:
	// public functions
	CGrammar();
	~CGrammar();
	bool ParseString(const CString *s, const SFormula *f, tpi_type *i, int *stopchar); 
	double EvaluateTree(CFormula * const f, tpi_type info, CEvalInfoFunction **logCallingFunction, int *e);
	double CalcF$symbol(CFormula * const f, const char *symbol, bool log, int *e);
	double CalcF$symbol(CFormula * const f, const char *symbol, int *e);
	double DoCalcF$symbol(CFormula * const f, const char *symbol, CEvalInfoFunction **logCallingFunction, bool skipCache, int *e);
	static void SetPosition(parse_tree_match_t::node_t &node, const char *begin, const char *end) { node.value.value(begin); } 
	static void ValidateSymbol(const char *begin, const char *end);

public:
	// public accessors
	const CArray<std::string, std::string> * parse_symbol_stop_strs() { return &_parse_symbol_stop_strs; }

private:
	// private functions and variables - not available via accessors or mutators
	double EvaluateExpression(CFormula * const f, iter_t const& i, CEvalInfoFunction **logCallingFunction, int *e);
	double DoEvaluateExpression(CFormula * const f, iter_t const& i, CEvalInfoFunction **logCallingFunction, int *e);
	double TryToEvaluateSymbolAsOpenPPLSymbol(CFormula * const f, string sym, CEvalInfoFunction **logCallingFunction, int *e);
	double EvaluateSymbol(CFormula * const f, string sym, CEvalInfoFunction **logCallingFunction, int *e);
	static void SetOffsets(iter_t &i, const char *start);

	unsigned int				_RecursionDepth;	// To detect endless recursion in DoCalcF$symbol(...);
	static const unsigned int	_MAX_RECURSION_DEPTH = 1000;

	// Result of formula set currently being parsed (this is for symbol validation)
	CArray<std::string, std::string>  _parse_symbol_stop_strs;
};

// This object ensures access to Spirit's evaluate by one thread at a time
extern CCritSec		m_critsec_evaluate;

// This object ensures access to Spirit's parse by one thread at a time, and ensures that "_parse_symbol_stop_strs"
// is only in use by one thread at a time
extern CCritSec		m_critsec_parse;

// Formula set currently being parsed (this is for symbol validation)
extern SFormula		*g_parse_symbol_formula;

// Result of formula set currently being parsed (this is for symbol validation)
// Upon completion of ast_parse, these results are copied back into the class
extern CArray<std::string, std::string>  g_parse_symbol_stop_strs;


// skip grammar
struct skip_grammar : public grammar<skip_grammar>
{
	template <typename ScannerT>
	struct definition
	{
		definition(skip_grammar const& /*self*/)
		{
			skip
				=   space_p
				|   "//" >> *(anychar_p - eol_p - end_p) >> (eol_p | end_p)	 // C++ comment
				|   "/*" >> *(anychar_p - "*/") >> "*/"	 // C comment
				;
		}

		rule<ScannerT> skip;

		rule<ScannerT> const&

		start() const { return skip; }
	};
};

// execution grammar
struct exec_grammar : public grammar<exec_grammar> 
{
	enum { SYMBOL_ID=1, FLOAT_CONSTANT1_ID, FLOAT_CONSTANT2_ID, FLOAT_CONSTANT3_ID, INT_CONSTANT_HEX_ID, 
		INT_CONSTANT_DEC_ID, INT_CONSTANT_OCT_ID, INT_CONSTANT_QUA_ID, INT_CONSTANT_BIN_ID,	EXP_EXPR_ID, 
		UNARY_EXPR_ID, MULT_EXPR_ID, ADD_EXPR_ID, SHIFT_EXPR_ID, RELATIONAL_EXPR_ID,
		EQUALITY_EXPR_ID, BINARY_AND_EXPR_ID, BINARY_XOR_EXPR_ID, BINARY_OR_EXPR_ID, LOGICAL_AND_EXPR_ID, 
		LOGICAL_XOR_EXPR_ID, LOGICAL_OR_EXPR_ID, COND_EXPR_ID, NON_EMPTY_EXPRESSION_ID, EXPRESSION_ID, 
		PRIMARY_EXPR_ID };

	template <typename ScannerT>
	struct definition 
	{
		definition(exec_grammar const& /*self*/) :
			LOG_OR_OP("||"), LOG_XOR_OP("^^"), LOG_AND_OP("&&"),
			EQ_OP("=="), NE_OP("!="),
			RSHIFT_OP(">>"), LSHIFT_OP("<<"),
			LE_OP("<="), GE_OP(">="),
			EXP_OP("**"), LN_OP("ln"),
			QUEST_OP('?'), COLON_OP(':'),
			BIN_OR_OP('|'), BIN_XOR_OP('^'), BIN_AND_OP('&'),
			LT_OP('<'), GT_OP('>'), 
			ADD_OP('+'), SUB_OP('-'), MULT_OP('*'), DIV_OP('/'), MOD_OP('%'),
			LOG_NOT_OP('!'), BIN_NOT_OP('~'), BITCOUNT_OP('`'),
			LPAREN('('), RPAREN(')'), LBRACK('['), RBRACK(']'), LBRACE('{'), RBRACE('}')
		{

			// keywords
			SYMBOL			 =	access_node_d[
									leaf_node_d[ 
										lexeme_d[
											((alpha_p | '_' | '$') >> *(alnum_p | '_' | '$' | '.')) 
										][&CGrammar::ValidateSymbol]
									]
								][&CGrammar::SetPosition];

			// constants
			// float constant: 12345[eE][+-]123[lLfF]?
			FLOAT_CONSTANT1  = access_node_d
								  [leaf_node_d[ lexeme_d[ 
										+digit_p
										>> (ch_p('e') | ch_p('E'))
										>> !(ch_p('+') | ch_p('-'))
										>> +digit_p
								   ] ]
								][&CGrammar::SetPosition];
			
			// float constant: .123([[eE][+-]123)?[lLfF]?
			FLOAT_CONSTANT2  = access_node_d[
								  leaf_node_d[ lexeme_d[ 
										*digit_p
										>> ch_p('.')
										>> +digit_p
										>> !(   (ch_p('e') | ch_p('E'))
												>> !(ch_p('+') | ch_p('-'))
												>> +digit_p
											)
								  ] ]
							   ][&CGrammar::SetPosition];

			// float constant: 12345.([[eE][+-]123)?[lLfF]?
			FLOAT_CONSTANT3  = leaf_node_d[ lexeme_d[ access_node_d[
										+digit_p
										>> ch_p('.')
										>> *digit_p
										>> !(   (ch_p('e') | ch_p('E'))
												>> !(ch_p('+') | ch_p('-'))
												>> +digit_p
											)
									  ][&CGrammar::SetPosition]
									  ] ];

			INT_CONSTANT_HEX =	leaf_node_d[ lexeme_d[ access_node_d[
									ch_p('0')
									>> as_lower_d[ch_p('x')]
									>> +xdigit_p
								  ][&CGrammar::SetPosition]
								] ];

			INT_CONSTANT_DEC =	leaf_node_d[ lexeme_d[ access_node_d[
									+digit_p
								  ][&CGrammar::SetPosition]
								] ];

			INT_CONSTANT_OCT =	leaf_node_d[ lexeme_d[ access_node_d[
									ch_p('0')
									>> as_lower_d[ch_p('o')]
									>> +range<>('0', '7')
								  ][&CGrammar::SetPosition]
								] ];

			INT_CONSTANT_QUA =	leaf_node_d[ lexeme_d[ access_node_d[
									ch_p('0')
									>> as_lower_d[ch_p('q')]
									>> +range<>('0', '3')
								  ][&CGrammar::SetPosition]
								] ];

			INT_CONSTANT_BIN =	leaf_node_d[ lexeme_d[ access_node_d[
									ch_p('0')
									>> as_lower_d[ch_p('b')]
									>> +range<>('0', '1')
								  ][&CGrammar::SetPosition]
								] ];

		// Rules
			primary_expr = 
				  FLOAT_CONSTANT1 
				| FLOAT_CONSTANT2
				| FLOAT_CONSTANT3
				| INT_CONSTANT_HEX 
				| INT_CONSTANT_OCT 
				| INT_CONSTANT_QUA
				| INT_CONSTANT_BIN
				| INT_CONSTANT_DEC
				| inner_node_d[LPAREN >> non_empty_expression >> RPAREN]
				| inner_node_d[LBRACK >> non_empty_expression >> RBRACK]
				| inner_node_d[LBRACE >> non_empty_expression >> RBRACE]
				| SYMBOL
				;

			exp_expr = (root_node_d[LN_OP] >> LPAREN >> cond_expr >> RPAREN)
							  | (primary_expr >> *(root_node_d[EXP_OP] >> primary_expr))
							;

			unary_expr = exp_expr
							  | (root_node_d[LOG_NOT_OP] >> unary_expr)
							  | (root_node_d[BIN_NOT_OP] >> unary_expr)
							  | (root_node_d[SUB_OP] >> unary_expr)
							  | (root_node_d[BITCOUNT_OP] >> unary_expr)
							;

			mult_expr = unary_expr >>
							*(  (root_node_d[MULT_OP] >> unary_expr)
							  | (root_node_d[DIV_OP] >> unary_expr)
							  | (root_node_d[MOD_OP] >> unary_expr)
							);

			add_expr = mult_expr >> 
							*(  (root_node_d[ADD_OP] >> mult_expr)
							  | (root_node_d[SUB_OP] >> mult_expr)
							);

			shift_expr = add_expr >>
							*(  (root_node_d[LSHIFT_OP] >> add_expr)
							  | (root_node_d[RSHIFT_OP] >> add_expr)
							);

			relational_expr = shift_expr >> 
							*(  (root_node_d[LT_OP] >> shift_expr)
							  | (root_node_d[GT_OP] >> shift_expr)
							  | (root_node_d[LE_OP] >> shift_expr)
							  | (root_node_d[GE_OP] >> shift_expr)
							);

			equality_expr = relational_expr >>
							*(  (root_node_d[EQ_OP] >> relational_expr)
							  | (root_node_d[NE_OP] >> relational_expr)
							);

			binary_and_expr = equality_expr >> *(root_node_d[BIN_AND_OP] >> equality_expr);

			binary_xor_expr = binary_and_expr >> *(root_node_d[BIN_XOR_OP] >> binary_and_expr);

			binary_or_expr = binary_xor_expr >> *(root_node_d[BIN_OR_OP] >> binary_xor_expr);

			logical_and_expr = binary_or_expr >> *(root_node_d[LOG_AND_OP] >> binary_or_expr);

			logical_xor_expr = logical_and_expr >> *(root_node_d[LOG_XOR_OP] >> logical_and_expr);

			logical_or_expr = logical_xor_expr >> *(root_node_d[LOG_OR_OP] >> logical_xor_expr);

			cond_expr = logical_or_expr >> *(root_node_d[QUEST_OP] >> cond_expr >> COLON_OP >> cond_expr);

			// Epsilon-expressions are no longer allowed to avoid crashed,
			// when the expsilon expression gets evaluated e.g. inside empty brackets.
			// http://www.maxinmontreal.com/forums/viewtopic.php?f=111&t=12232&start=0
			// they should however be allowed as top-level expressions,
			// e.g. for f$srai, etc if a formula is not used at all
			non_empty_expression = cond_expr;
			expression = non_empty_expression | epsilon_p;

			//  End grammar definition
		}

		// operators
		strlit<>
			LOG_OR_OP, LOG_XOR_OP, LOG_AND_OP,
			EQ_OP, NE_OP,
			RSHIFT_OP, LSHIFT_OP,
			LE_OP, GE_OP,
			EXP_OP, LN_OP;
		chlit<>
			QUEST_OP, COLON_OP, 
			BIN_OR_OP, BIN_XOR_OP, BIN_AND_OP,
			LT_OP, GT_OP,
			ADD_OP, SUB_OP, MULT_OP, DIV_OP, MOD_OP,
			LOG_NOT_OP, BIN_NOT_OP, BITCOUNT_OP, 
			LPAREN, RPAREN, LBRACK, RBRACK, LBRACE, RBRACE;

		// terminals
		rule<ScannerT, parser_tag<SYMBOL_ID> >					SYMBOL;
		rule<ScannerT, parser_tag<INT_CONSTANT_HEX_ID> >		INT_CONSTANT_HEX;
		rule<ScannerT, parser_tag<INT_CONSTANT_DEC_ID> >		INT_CONSTANT_DEC;
		rule<ScannerT, parser_tag<INT_CONSTANT_OCT_ID> >		INT_CONSTANT_OCT;
		rule<ScannerT, parser_tag<INT_CONSTANT_QUA_ID> >		INT_CONSTANT_QUA;
		rule<ScannerT, parser_tag<INT_CONSTANT_BIN_ID> >		INT_CONSTANT_BIN;
		rule<ScannerT, parser_tag<FLOAT_CONSTANT1_ID> >			FLOAT_CONSTANT1;
		rule<ScannerT, parser_tag<FLOAT_CONSTANT2_ID> >			FLOAT_CONSTANT2;
		rule<ScannerT, parser_tag<FLOAT_CONSTANT3_ID> >			FLOAT_CONSTANT3;

		rule<ScannerT, parser_tag<PRIMARY_EXPR_ID> >			primary_expr;
		rule<ScannerT, parser_tag<EXPRESSION_ID> >				expression;
		rule<ScannerT, parser_tag<NON_EMPTY_EXPRESSION_ID> >	non_empty_expression;
		rule<ScannerT, parser_tag<EXP_EXPR_ID> >				exp_expr;
		rule<ScannerT, parser_tag<UNARY_EXPR_ID> >				unary_expr;
		rule<ScannerT, parser_tag<MULT_EXPR_ID> >				mult_expr;
		rule<ScannerT, parser_tag<ADD_EXPR_ID> >				add_expr;
		rule<ScannerT, parser_tag<SHIFT_EXPR_ID> >				shift_expr;
		rule<ScannerT, parser_tag<RELATIONAL_EXPR_ID> >			relational_expr;
		rule<ScannerT, parser_tag<EQUALITY_EXPR_ID> >			equality_expr;
		rule<ScannerT, parser_tag<BINARY_AND_EXPR_ID> >			binary_and_expr;
		rule<ScannerT, parser_tag<BINARY_XOR_EXPR_ID> >			binary_xor_expr;
		rule<ScannerT, parser_tag<BINARY_OR_EXPR_ID> >			binary_or_expr;
		rule<ScannerT, parser_tag<LOGICAL_AND_EXPR_ID> >		logical_and_expr;
		rule<ScannerT, parser_tag<LOGICAL_XOR_EXPR_ID> >		logical_xor_expr;
		rule<ScannerT, parser_tag<LOGICAL_OR_EXPR_ID> >			logical_or_expr;
		rule<ScannerT, parser_tag<COND_EXPR_ID> >				cond_expr;

		rule<ScannerT, parser_tag<EXPRESSION_ID> > const&

		start() const { return expression; }
	};
};

#endif  /* INC_CGRAMMAR_H */

