#ifndef INC_GRAMMAR_H
#define INC_GRAMMAR_H

#include <boost/spirit/tree/ast.hpp>
#include "structs_defines.h"

typedef char const*							iterator_t;
typedef boost::spirit::tree_match<iterator_t>	parse_tree_match_t;
typedef parse_tree_match_t::tree_iterator	iter_t;

// forward declarations
bool parse (CString *s, boost::spirit::tree_parse_info<> *i, int *stopchar);
double calc_f$symbol(SFormula *f, char *symbol, int *e);
//double evaluate(parse_tree_match_t hit);
double evaluate(SFormula *f, boost::spirit::tree_parse_info<> info, int *e);
double eval_expression(SFormula *f, iter_t const& i, int *e);

//  Here's the comment rule
struct skip_grammar : public boost::spirit::grammar<skip_grammar>
{
	template <typename ScannerT>
	struct definition
	{
		definition(skip_grammar const& /*self*/)
		{
			skip
				=   space_p
				|   "//" >> *(anychar_p - eol_p - end_p) >> (eol_p | end_p)     // C++ comment
				|   "/*" >> *(anychar_p - "*/") >> "*/"     // C comment
				;
		}

		boost::spirit::rule<ScannerT> skip;

		boost::spirit::rule<ScannerT> const&
		start() const { return skip; }
	};
};

// execution grammar
struct exec_grammar : public boost::spirit::grammar<exec_grammar> {
	enum { SYMBOL_ID=1, FLOAT_CONSTANT1_ID, FLOAT_CONSTANT2_ID, FLOAT_CONSTANT3_ID, INT_CONSTANT_HEX_ID, 
		INT_CONSTANT_DEC_ID, INT_CONSTANT_OCT_ID, INT_CONSTANT_QUA_ID, INT_CONSTANT_BIN_ID,	EXP_EXPR_ID, 
		UNARY_EXPR_ID, MULT_EXPR_ID, ADD_EXPR_ID, SHIFT_EXPR_ID, RELATIONAL_EXPR_ID,
		EQUALITY_EXPR_ID, BINARY_AND_EXPR_ID, BINARY_XOR_EXPR_ID, BINARY_OR_EXPR_ID, LOGICAL_AND_EXPR_ID, 
		LOGICAL_XOR_EXPR_ID, LOGICAL_OR_EXPR_ID, COND_EXPR_ID, EXPRESSION_ID, PRIMARY_EXPR_ID };

	template <typename ScannerT>
	struct definition {
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
			SYMBOL			 = leaf_node_d[ lexeme_d[ ((alpha_p | '_' | '$') >> *(alnum_p | '_' | '$')) ] ];

			// constants
			// float constant: 12345[eE][+-]123[lLfF]?
			FLOAT_CONSTANT1  = leaf_node_d[ lexeme_d[
										+digit_p
										>> (ch_p('e') | ch_p('E'))
										>> !(ch_p('+') | ch_p('-'))
										>> +digit_p
									  ] ];
			
			// float constant: .123([[eE][+-]123)?[lLfF]?
			FLOAT_CONSTANT2  = leaf_node_d[ lexeme_d[
										*digit_p
										>> ch_p('.')
										>> +digit_p
										>> !(   (ch_p('e') | ch_p('E'))
												>> !(ch_p('+') | ch_p('-'))
												>> +digit_p
											)
									  ] ];

			// float constant: 12345.([[eE][+-]123)?[lLfF]?
			FLOAT_CONSTANT3  = leaf_node_d[ lexeme_d[
										+digit_p
										>> ch_p('.')
										>> *digit_p
										>> !(   (ch_p('e') | ch_p('E'))
												>> !(ch_p('+') | ch_p('-'))
												>> +digit_p
											)
									  ] ];

			INT_CONSTANT_HEX =	leaf_node_d[ lexeme_d[
									ch_p('0')
									>> as_lower_d[ch_p('x')]
									>> +xdigit_p
								] ];

			INT_CONSTANT_DEC =	leaf_node_d[ lexeme_d[
									+digit_p
								] ];

			INT_CONSTANT_OCT =	leaf_node_d[ lexeme_d[
									ch_p('0')
									>> as_lower_d[ch_p('o')]
									>> +range<>('0', '7')
								] ];

			INT_CONSTANT_QUA =	leaf_node_d[ lexeme_d[
									ch_p('0')
									>> as_lower_d[ch_p('q')]
									>> +range<>('0', '3')
								] ];

			INT_CONSTANT_BIN =	leaf_node_d[ lexeme_d[
									ch_p('0')
									>> as_lower_d[ch_p('b')]
									>> +range<>('0', '1')
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
				| inner_node_d[LPAREN >> expression >> RPAREN]
				| inner_node_d[LBRACK >> expression >> RBRACK]
				| inner_node_d[LBRACE >> expression >> RBRACE]
				| SYMBOL
				;

			exp_expr = (root_node_d[LN_OP] >> exp_expr)
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

			cond_expr = logical_or_expr >> *(root_node_d[QUEST_OP] >> expression >> COLON_OP >> cond_expr);

			expression = *(cond_expr >> expression);

			//  End grammar definition
		}

		// operators
		boost::spirit::strlit<>
			LOG_OR_OP, LOG_XOR_OP, LOG_AND_OP,
			EQ_OP, NE_OP,
			RSHIFT_OP, LSHIFT_OP,
			LE_OP, GE_OP,
			EXP_OP, LN_OP;
		boost::spirit::chlit<>
			QUEST_OP, COLON_OP, 
			BIN_OR_OP, BIN_XOR_OP, BIN_AND_OP,
			LT_OP, GT_OP,
			ADD_OP, SUB_OP, MULT_OP, DIV_OP, MOD_OP,
			LOG_NOT_OP, BIN_NOT_OP, BITCOUNT_OP, 
			LPAREN, RPAREN, LBRACK, RBRACK, LBRACE, RBRACE;

		// terminals
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<SYMBOL_ID> >				SYMBOL;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<INT_CONSTANT_HEX_ID> >		INT_CONSTANT_HEX;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<INT_CONSTANT_DEC_ID> >		INT_CONSTANT_DEC;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<INT_CONSTANT_OCT_ID> >		INT_CONSTANT_OCT;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<INT_CONSTANT_QUA_ID> >		INT_CONSTANT_QUA;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<INT_CONSTANT_BIN_ID> >		INT_CONSTANT_BIN;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<FLOAT_CONSTANT1_ID> >		FLOAT_CONSTANT1;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<FLOAT_CONSTANT2_ID> >		FLOAT_CONSTANT2;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<FLOAT_CONSTANT3_ID> >		FLOAT_CONSTANT3;

		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<PRIMARY_EXPR_ID> >			primary_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<EXPRESSION_ID> >			expression;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<EXP_EXPR_ID> >				exp_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<UNARY_EXPR_ID> >			unary_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<MULT_EXPR_ID> >				mult_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<ADD_EXPR_ID> >				add_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<SHIFT_EXPR_ID> >			shift_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<RELATIONAL_EXPR_ID> >		relational_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<EQUALITY_EXPR_ID> >			equality_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<BINARY_AND_EXPR_ID> >		binary_and_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<BINARY_XOR_EXPR_ID> >		binary_xor_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<BINARY_OR_EXPR_ID> >		binary_or_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<LOGICAL_AND_EXPR_ID> >		logical_and_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<LOGICAL_XOR_EXPR_ID> >		logical_xor_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<LOGICAL_OR_EXPR_ID> >		logical_or_expr;
		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<COND_EXPR_ID> >				cond_expr;

		boost::spirit::rule<ScannerT, boost::spirit::parser_tag<EXPRESSION_ID> > const&
		start() const { return expression; }
	};
};


#endif  /* INC_GRAMMAR_H */

