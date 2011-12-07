// Grammar_Inc.cpp
//
// This file gets included as pure CPP as part of the parser definition,
// otherwise the parser would become way too large to keep it understandable
// and AFAIK we can't distribute it over multiple modules.


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
			//
			// Whole PPL-file
			//
			openPPL_code = 
				option_settings_to_be_ignored
				>> ((keyword_custom 
					[print_license()]
					[print_options()]
					[print_comment_for_list_section()]
				>> custom_sections));
					/*| missing_keyword_custom); !!!*/
			custom_sections = 
				!list_section
					[print_user_defined_functions()]
				>> !symbol_section
					[print_main_code_sections()]
				>> (code_sections)
				>> end_p
					[print_prime_coded_board_ranks()]
					[print_technical_functions()]
					[print_OpenPPL_Library()];

			//
			// Keywords like "When", "RaiseMax", etc.
			///
#include "ListOfKeywords_Inc.cpp"

			// !!!missing_keyword_custom = (str_p("") >> custom_sections)[error_missing_keyword_custom()];
			
			//
			// Option settings - to be ignored
			//
			option_settings_to_be_ignored = *single_option;
			single_option = option_name 
				>> '=' 
				>> option_value;
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
				>> code_block
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
				>> code_block)
					[print_when_others_fold_force()][reset_variables()];
			flop_section = (keyword_flop/*[register_code_section()]*/
					[print_function_header_for_betting_round()]
				>> code_block)
					[print_when_others_fold_force()][reset_variables()];
			turn_section = (keyword_turn/*[register_code_section()]*/
					[print_function_header_for_betting_round()]
				>> code_block)
					[print_when_others_fold_force()][reset_variables()];
			river_section = (keyword_river/*[register_code_section()]*/
					[print_function_header_for_betting_round()]
				>> code_block)
					[print_when_others_fold_force()][reset_variables()];
			
			// ToDo:
			//
			// Better way to check for missing code-sections.
			// * Register, what we have seen and then throw an error-message
			//   at the end of parsing.
			// * Change the grammar, so that it accepts e.g. a sequence 
			//   of preflop, turn and river 

			// When-condition sequences (with action)
			//
			// Our simplified grammar allows arbitrary sequences 
			// of (open-ended) when-conditions and actions.
			// But we can more easily generate OH-script-code and check 
			// for illegal Open-PPL-code in "handle_when_condition()"
			// this way.
			//
			// For technical reasons ("HALP ME!!!! IT DO NO WORKS!!!!!")
			// evary condition needs to be enclosed in brackets.
			// For details see http://www.maxinmontreal.com/forums/viewtopic.php?f=216&t=13934
			// The simple "When Others" as an exception.
			code_block = *(when_condition | action);
			when_condition = 
				keyword_when[extra_brackets_for_condition()] 
				>> ((bracket_expression | condition_others)
					[extra_closing_brackets_for_condition()]
					[handle_when_condition()]
					);
			
			// "Others" - e.g. in "when others fold force"
			condition_others = keyword_others[print_symbol()];

			// Operators
			unary_operator          = (keyword_not | "-")[print_operator()];
			percentage_operator     = str_p("%");
			multiplicative_operator = (str_p("*") | ("/")) 									| percentage_operator; 
			additive_operator       = (str_p("+") | "-");
			equality_operator       = str_p("=");
			relational_operator     = longest_d[(equality_operator | str_p("<=") | ">=" | "<" | ">")];
			binary_boolean_operator = keyword_and | keyword_or | keyword_xor;
			binary_operator         = (multiplicative_operator | additive_operator | relational_operator | binary_boolean_operator)[print_operator()];
			
			// Expressions
			expression = sequence_of_binary_expressions;
			sequence_of_binary_expressions = operand >> *(binary_operator >> operand);
			operand = unary_expression | terminal_expression | bracket_expression; //!!!
			unary_expression = (unary_operator[print_opening_bracket()] >> operand)[print_closing_bracket()];
			bracket_expression = str_p("(")[print_opening_bracket()] 
				>> (hand_expression | board_expression | expression)
				>> str_p(")")[print_closing_bracket()];
			// !!! TODO: missing_closing_bracket_expression = (str_p("(") >> expression) >> str_p("")[error_missing_closing_bracket()];
			
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
			hand_expression = keyword_hand 
					>> str_p("=") 
					>> card_expression[print_hand_expression()];

			// board expressions
			board_expression = suited_board_expression | non_suited_board_expression;
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
				| keyword_raisepot | keyword_raisemax |	keyword_allin
				| keyword_raise | keyword_fold
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
			// Unfortunatelly we can't use "number = real_p[print_number()]";
			// so we put all actions into "terminal_expression";
			terminal_expression = 
				number             [print_number()]
				| boolean_constant [print_symbol()] 
				| symbol           [print_symbol()];
			number = real_p;
			boolean_constant = (keyword_true | keyword_false);
			// "Symbol" is a lexeme - we have to be very careful here.
			// We have to use the lexeme_d directive to disable skipping of whitespace,
			// otherwise things like "When x Bet force" would treat "x Bet force"
			// as a single keyword and cause an error.
			// http://www.boost.org/doc/libs/1_40_0/libs/spirit/classic/doc/quickref.html
			symbol = (lexeme_d[alpha_p >> *alnum_p]);
				/*| invalid_symbol;*/
			// "Symbols" cintaining invalid cahracters
			invalid_character = str_p(";")  | "," | ":" | "|" | "@" | "€" | "!" | "\\"
				| "\""  | "§" | "$" | "&" | "?" | "´" | "´" | "[" | "]"
				| "^" | "°" | "{" | "}" | "#" | "³" | "²";
			invalid_symbol = (*alnum_p >> invalid_character
				>> *(alnum_p | invalid_character))[error_invalid_character()];

			/*
			// Debugging boost spirit
			//
			// For details see http://www.boost.org/doc/libs/1_35_0/libs/spirit/doc/debugging.html
			//
			BOOST_SPIRIT_DEBUG_RULE(expression);
			BOOST_SPIRIT_DEBUG_RULE(binary_expression);
			BOOST_SPIRIT_DEBUG_RULE(operand);
			BOOST_SPIRIT_DEBUG_RULE(terminal_expression);
			BOOST_SPIRIT_DEBUG_RULE(bracket_expression);
			*/
		} 

		const boost::spirit::rule<Scanner> &start() 
		{ 
			return openPPL_code; 
		} 
	};
}; 