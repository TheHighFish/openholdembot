#define BOOST_SPIRIT_DEBUG  ///$$$ DEFINE THIS BEFORE ANYTHING ELSE $$$///

//#include <afxwin.h>
#include <afx.h>
#include <boost/spirit.hpp> 
#include <boost/spirit/iterator/position_iterator.hpp>

#include <assert.h>
#include <fstream> 
#include <iostream> 
#include <sstream> 
#include <stdio.h>
#include "Board.h"
#include "CodeSnippets.h"
#include "CSymbolTable.h"
#include "ErrorMessages.h"
#include "ListOfOpenHoldemSymbolPrefixes.h"

using namespace boost::spirit;
using namespace std;

const bool k_assert_this_must_not_happen = false;

bool when_others_fold_force_detected = false;
bool when_others_when_others_fold_force_detected = false;
bool open_ended_when_condition_detected = false;

int when_conditions_since_last_action = 0;

// Stringstreams for the generated output
// We have to use two of them and can't output directly,
// as we have to reuse the code for open-ended when-conditions.
// Example:
//
// When A
//   When a1...
//   When a1...
// When B
//
// has to be translated to
//
// (A && a1) ? ... :
// (A && a2) ? ... :
// (B && ..)
stringstream current_open_ended_when_condition;
stringstream current_when_condition;
stringstream current_output;

// Remembering the original source to generate meaningful comments.
string original_source_of_current_when_condition;
string original_source_of_current_open_ended_when_condition;

// http://www.highscore.de/cpp/boost/parser.html
// http://www.boost.org/doc/libs/1_35_0/libs/spirit/doc/operators.html
// http://www.boost.org/doc/libs/1_34_1/libs/spirit/doc/primitives.html
// http://beta.boost.org/doc/libs/1_41_0/libs/spirit/doc/html/spirit/qi/reference/char/char.html
// http://www.johannes-asal.de/?p=107 (danish)
// http://www.ibm.com/developerworks/aix/library/au-boost_parser/index.html

int bracket_counter = 0;

boost::spirit::parse_info<> pi;

void AddSwagFunctionsForOH_2_1_x_IfExistent()
{
	const CString swag_file_name = "OpenPPL_SwagFunctions_For_OpenHoldem_2.1.x_Only_Please_Delete_This_File_Once_You_Use_OpenHoldem_2.2.0_And_Later.ohf";
	fstream swag_file(swag_file_name);
	// We wrote the swag-functions and know, that 1000 is way too much.
	// This file will never be changed, so we should be on the safe side.
	const int max_line_length = 1000;
	char next_line[max_line_length];
	if (swag_file.is_open())
	{
		while (!swag_file.eof())
		{
			swag_file.getline(next_line, max_line_length);
			cout << next_line << endl;
		}
	}
}

CString ErroneousCodeSnippet(const char *begin_of_erroneous_code_snippet)
{
	CString rest_of_input = begin_of_erroneous_code_snippet;
	CString erroneous_code_snippet = rest_of_input.Left(100);
	return erroneous_code_snippet;
}

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
				|   "/*" >> *(anychar_p - "*/") >> "*/"	// C comment
				|	as_lower_d["in "]					// Useless fill-word "In " like "In SmallBlind"
														// Space is necessary here, otherwise we damage
														// valid words starting with "in".
				;
		}

		rule<ScannerT> skip;

		rule<ScannerT> const&

		start() const { return skip; }
	};
};


struct json_grammar: public boost::spirit::grammar<json_grammar> 
{ 
// SemanticActions_Inc.cpp
//
// This file gets included as pure CPP as part of the parser definition,
// otherwise the parser would become way too large to keep it understandable
// and AFAIK we can't distribute it over multiple modules.
#include "SemanticActions_Inc.cpp"

	template <typename Scanner>

	struct definition 
	{ 
		boost::spirit::rule<Scanner> 

// ListOfSyntaxElements_Inc.cpp
//
// This file gets included as pure CPP as part of the parser definition,
// otherwise the parser would become way too large to keep it understandable
// and AFAIK we can't distribute it over multiple modules.
#include "ListOfSyntaxElements_Inc.cpp"

		definition(const json_grammar &self) 
		{ 
			using namespace boost::spirit; 

			// Whole PPL-file
			openPPL_code = option_settings_to_be_ignored
				>> ((keyword_custom [print_license()]
					[print_options()][print_comment_for_list_section()]
				>> custom_sections)
					| missing_keyword_custom);
			custom_sections = !list_section[print_user_defined_functions()]
				>> !symbol_section[print_main_code_sections()]
				>> (code_sections /*| missing_code_section*/)
				>> end_p
					[print_prime_coded_board_ranks()]
					[print_technical_functions()]
					[print_OpenPPL_Library()];

#include "ListOfKeywords_Inc.cpp"

			missing_keyword_custom = (str_p("") >> custom_sections)[error_missing_keyword_custom()];
			  
			// Option settings - to be ignored
			option_settings_to_be_ignored = *single_option;
			single_option = option_name >> '=' >> option_value;
			option_name = alpha_p >> *(alnum_p | "-");
			
			option_value = keyword_on | keyword_off | number;

			// Start of custom code

			/***
			// user defined lists
			list_section = keyword_lists >> *list_definition;
			list_definition = keyword_new >> keyword_list
				>> number[print_list_header()] >> list_content;
			list_content = end_of_list || (next_line_of_list >> end_of_list);
			next_line_of_list = (anychar_p - eol_p - end_p) >> (eol_p | end_p);
			end_of_list = keyword_end >> keyword_list;
			***/

			/***
			// User defined symbols
			symbol_section = keyword_symbols >> *symbol_definition;
			symbol_definition = keyword_new >> keyword_symbol[reset_variables()]
				>> symbol[print_function_header_for_betting_round()] 
				>> when_section
				>> keyword_end >> keyword_symbol[reset_variables()]
					[print_default_return_value_for_user_defined_symbol()]
					[reset_variables()];
			***/

			// Preflop, flop, turn, river
			code_sections = preflop_section
				>> flop_section 
				>> turn_section 
				>> river_section;
			preflop_section = (keyword_preflop/*[register_code_section()]*/
					[print_function_header_for_betting_round()]
				>> when_section)[check_for_correct_when_others_fold_force()]
					[print_when_others_fold_force()][reset_variables()];
			flop_section = (keyword_flop/*[register_code_section()]*/
					[print_function_header_for_betting_round()]
				>> when_section)[check_for_correct_when_others_fold_force()]
					[print_when_others_fold_force()][reset_variables()];
			turn_section = (keyword_turn/*[register_code_section()]*/
					[print_function_header_for_betting_round()]
				>> when_section)[check_for_correct_when_others_fold_force()]
					[print_when_others_fold_force()][reset_variables()];
			river_section = (keyword_river/*[register_code_section()]*/
					[print_function_header_for_betting_round()]
				>> when_section)[check_for_correct_when_others_fold_force()]
					[print_when_others_fold_force()][reset_variables()];
			when_section = when_condition_sequence;
			
			missing_code_section =
				// missing river
				((preflop_section >> flop_section >> turn_section >> str_p("")[error_missing_code_section()])
				// missing turn
				|((preflop_section >> flop_section) >> str_p("")[error_missing_code_section()]) 
				// missing flop
				|(preflop_section >> str_p("")[error_missing_code_section()]) 
				// missing preflop
				|(str_p("")[error_missing_code_section()]));
			

			// When-condition sequences (with action)
			when_condition = (keyword_when >> condition)[handle_when_condition()];
			when_condition_sequence = *(when_others_when_others_fold_force
				| when_others_fold_force
				| when_condition
				| action);

			// When Others Fold Force
			when_others_fold_force = (keyword_when >> keyword_others >> keyword_fold 
				>> keyword_force)[set_when_others_fold_force_detected()];
			when_others_when_others_fold_force = (keyword_when >> keyword_others 
				>> when_others_fold_force)[set_when_others_when_others_fold_force_detected()];
			
			// Conditions
			condition = str_p("")[extra_brackets_for_condition()] >> expression
				[extra_closing_brackets_for_condition()];

			// Operators
			unary_operator = (keyword_not | "-")[print_operator()];
			percentage_operator = str_p("%")[print_percentage_operator()];
			multiplicative_operator = (str_p("*") | ("/"))[print_operator()] | percentage_operator; 
			additive_operator = (str_p("+") | "-")[print_operator()];
			equality_operator = str_p("=")[print_operator()];
			relational_operator = longest_d[(equality_operator | str_p("<=") | ">=" | "<" | ">")][print_operator()];
			
			// Expressions
			expression = or_expression;
			bracket_expression = str_p("(")[print_opening_bracket()] >> expression >> str_p(")")[print_closing_bracket()];
			missing_closing_bracket_expression = (str_p("(") >> expression) >> str_p("")[error_missing_closing_bracket()];
			operand_expression = terminal_expression | bracket_expression 
				| missing_closing_bracket_expression;
			unary_expression = !unary_operator[print_opening_bracket()] >> operand_expression[print_opening_bracket()];
			multiplicative_expression = unary_expression >> *(multiplicative_operator >> unary_expression);
			additive_expression = multiplicative_expression >> *(additive_operator >> multiplicative_expression);
			relational_expression = additive_expression >> *(relational_operator >> additive_expression);
			
			equality_expression = hand_expression_with_brackets 
				| board_expression_with_brackets 
                | hand_expression_without_brackets 
				| board_expression_without_brackets
				| (relational_expression /*>> *(str_p("=")[print_operator()] >> relational_expression)*/);
			and_expression = equality_expression >> *(keyword_and >> equality_expression);
			xor_expression = and_expression >> *(keyword_xor >> and_expression);
			or_expression = xor_expression >> *(keyword_or >> xor_expression);

			// Hand and board expressions
			card_constant = lexeme_d[ch_p("A") | "a" | "K" | "k" | "Q" | "q" | "J" | "j" |
				"T" | "t" | "9" | "8" | "7" | "6" | "5" | "4" | "3" | "2"];
			suit_constant = lexeme_d[ch_p("C") | "c" | "D"| "d" | "H" | "h" | "S" | "s"];
			
			// card_expression = longest_d[
			card_expression = suited_card_expression
				| card_expression_with_specific_suits
				| non_suited_card_expression
				| invalid_card_expression;
			suited_card_expression = (non_suited_card_expression >> keyword_suited);
			non_suited_card_expression = (lexeme_d[+card_constant]);
			invalid_card_expression = lexeme_d[+alnum_p][error_invalid_card_expression()];
			card_expression_with_specific_suits = card_constant >> suit_constant >> *(card_constant >> !suit_constant)
				[error_specific_suits_not_supported()];

			// hand expression
			hand_expression_with_brackets = str_p("(") >> keyword_hand 
                    >> str_p("=") >> card_expression[print_hand_expression()] >> ")";            
            hand_expression_without_brackets = (keyword_hand 
                    >> str_p("=") >> card_expression)[error_missing_brackets_for_card_expression()];

			// board expressions
			board_expression_with_brackets = str_p("(") 
                    >> (suited_board_expression | non_suited_board_expression) >> ")";
			board_expression_without_brackets = (keyword_board 
                    >> str_p("=")[print_operator()] >> card_expression)[error_missing_brackets_for_card_expression()];
			suited_board_expression = (keyword_board >> str_p("=") 
					>> suited_card_expression)[print_suited_board_expression()];
			non_suited_board_expression = (keyword_board >> str_p("=") 
					>> non_suited_card_expression)[print_non_suited_board_expression()];	                        
                                 
			// Actions (and return statements)
			// We handle both in the same way, as it simplifies things a lot.
			action = (action_without_force >> keyword_force)
					[handle_action()]
				| erroneous_action_without_force;
			// No longest_d[] for action_without_force,
            // otherwise Boost Spirit tries to evaluate everything,
            // Causing superfluos output. Therefore we simply start
            // with the longest expression first:
            // 1. Raise X % 
            // 2. Raise X
			// 3. Raise	 
			action_without_force = relative_betsize_action
				| fixed_betsize_action 
				| predefined_action
				| return_statement;
			predefined_action = keyword_predefined_action[print_predefined_action()];
			
			keyword_predefined_action = longest_d[keyword_beep | keyword_call	| keyword_play 
				| keyword_raisemin | keyword_raisehalfpot
				| keyword_raisepot | keyword_raisemax |	keyword_raise | keyword_fold
				| keyword_betmin | keyword_bethalfpot
				| keyword_betpot | keyword_betmax | keyword_bet | keyword_sitout];
			fixed_betsize_action = (keyword_bet | keyword_raise) >> number[print_comment_for_fixed_betsize()];
			relative_betsize_action = (keyword_bet | keyword_raise) >> number[print_number()] 
				>> percentage_operator[print_percentage_operator()][print_relative_potsize_action()] ;
			erroneous_action_without_force = action_without_force[error_action_without_force()];

			// UserDefined Variables
			/*
			user_defined_variable = lexeme_d[user_prefix >> *alnum_p];
			user_prefix = str_p("user") | "User" | "USER";
			*/

			// Return statement (without force, as this is part of "action")
			return_statement = keyword_return >> expression;

			// Terminal expressions
			terminal_expression = number[print_number()] 
				| boolean_constant 
				| symbol[print_symbol()];
			number = real_p;
			boolean_constant = (keyword_true | keyword_false)[print_symbol()];
			// "Symbol" is a lexeme - we have to be very careful here.
			// We have to use the lexeme_d directive to disable skipping of whitespace,
			// otherwise things like "When x Bet force" would treat "x Bet force"
			// as a single keyword and cause an error.
			// http://www.boost.org/doc/libs/1_40_0/libs/spirit/classic/doc/quickref.html
			symbol = (lexeme_d[alpha_p >> *alnum_p]) | invalid_symbol;
			// "Symbols" cintaining invalid cahracters
			invalid_character = str_p(";")  | "," | ":" | "|" | "@" | "€" | "!" | "\\"
				| "\""  | "§" | "$" | "&" | "?" | "´" | "´" | "[" | "]"
				| "^" | "°" | "{" | "}" | "#" | "³" | "²";
			invalid_symbol = (*alnum_p >> invalid_character 
				>> *(alnum_p | invalid_character))[error_invalid_character()];

			BOOST_SPIRIT_DEBUG_NODE(equality_expression);
			BOOST_SPIRIT_DEBUG_NODE(symbol);
			BOOST_SPIRIT_DEBUG_NODE(number);
			BOOST_SPIRIT_DEBUG_NODE(relational_operator);
			BOOST_SPIRIT_DEBUG_NODE(relational_expression);
		} 

		const boost::spirit::rule<Scanner> &start() 
		{ 
			return openPPL_code; 
		} 
	};
}; 

int main(int argc, char *argv[]) 
{ 
	CString InputFile;
	if (argc != 2)
	{
		// One argument is always present
		// It is the name of the executable in argv[0]
		// The other one is the desired input file
		CString error_message = 
			"No input file specified.\n"
			"Syntax to be used at the command line:\n"
			"\n"
			"OpenPPL INPUTFILE.txt > OUTPUTFILE.ohf";

		//MessageBox(0, error_message, "Error", 0);
		exit(-1);
	}
	else
	{
		// Using the first real argument
		InputFile = argv[1];
	}
	p_symbol_table = new(CSymbolTable);
	p_symbol_table->AddSymbolsFromFile("OpenPPL_Library.ohf");
	p_symbol_table->AddSymbolsFromFile(InputFile);

	p_list_of_openholdem_symbol_prefixes = new(ListOfOpenHoldemSymbolPrefixes);

	std::ifstream fs(InputFile); 
	std::ostringstream ss; 
	ss << fs.rdbuf(); 
	std::string data = ss.str(); 

	json_grammar g; 
	skip_grammar skip;
	// Case insensitive parsing: 
	// http://www.ibm.com/developerworks/aix/library/au-boost_parser/index.html

	pi = boost::spirit::parse(data.c_str(), /*as_lower_d*/g, skip); 
	if (!pi.hit) 
	{
		//CString rest_of_input = pi.stop;
		CString erroneous_input = ErroneousCodeSnippet(pi.stop);//rest_of_input.Left(100);
		if (erroneous_input.GetLength() > 0)
		{
		  // Examples about how to use the position operator:
		  // http://live.boost.org/doc/libs/1_34_0/libs/spirit/doc/position_iterator.html
		  // http://live.boost.org/doc/libs/1_34_0/libs/spirit/example/fundamental/position_iterator/position_iterator.cpp
		  file_position parse_error_position;
		  //parse_error_position = pi.first.get_position(); 
		  ErrorMessage(k_error_general, ErroneousCodeSnippet(pi.stop));
		}
	}
	else
	{
		// Translation sucessfully finished.
		// Last step: add swag-functions for OH 2.1.x if necessary.
		AddSwagFunctionsForOH_2_1_x_IfExistent();
	}
} 