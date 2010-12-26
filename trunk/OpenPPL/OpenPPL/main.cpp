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
				|	as_lower_d["in"]					// Useless fill-word "In SmallBlind"
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


// stream handling
struct reset_output_streams
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		current_open_ended_when_condition.str("");
		current_open_ended_when_condition.clear();
		current_when_condition.str("");
		current_when_condition.clear();
		current_output.str("");
		current_output.clear();
		//MessageBox(0, "Reset", "Debug", 0);
	} 
};

struct flush_current_output_streams
{ 
	void operator()(const char *begin, const char *end) const 
	{
		if (current_open_ended_when_condition.str() != "")
		{
			cout << current_open_ended_when_condition.str() << " && ";
		}
		cout << current_output.str() << endl;;
	} 
};



struct handle_action
{
	void operator()(const char *begin, const char *end) const 
	{
		when_conditions_since_last_action = 0;
		if (open_ended_when_condition_detected)
		{
			cout << current_open_ended_when_condition.str() << "  &&  "
				 << current_when_condition.str() << " ? " 
				 << current_output.str() << " :" << endl;
		}
		else
		{
			cout << current_when_condition.str() << " ? "
				 << current_output.str() << " :" << endl;
		}
		//cout << "ACTION: " << current_output.str() << endl;
		current_output.str("");
		current_output.clear();
	}
};

struct handle_when_condition
{
	void operator()(const char *begin, const char *end) const 
	{
		assert(when_conditions_since_last_action >= 0);
		when_conditions_since_last_action++;
		if (when_conditions_since_last_action == 1)
		{
			original_source_of_current_when_condition = string(begin, end);
			current_when_condition.str("");
			current_when_condition.clear();
			current_when_condition << current_output.str();
			//cout << "WHEN CONDITION: " << current_when_condition.str() << endl;
			current_output.str("");
			current_output.clear();
		}
		else if (when_conditions_since_last_action == 2)
		{
			open_ended_when_condition_detected = true;
			original_source_of_current_open_ended_when_condition =
				original_source_of_current_when_condition;
			current_open_ended_when_condition.str("");
			current_open_ended_when_condition.clear();
			current_open_ended_when_condition << current_when_condition.str();
			current_when_condition.str("");
			current_when_condition.clear();
			current_when_condition << current_output.str();
			//cout << "WHEN CONDITION (OPEN ENDED): " << current_open_ended_when_condition.str() << endl;
			//cout << "WHEN CONDITION: " << current_when_condition.str() << endl;
			cout << "//" << endl;
			cout << "// Starting open-ended when-condition" << endl;
			cout << "// " << original_source_of_current_open_ended_when_condition << endl;
			cout << "//" << endl;
			current_output.str("");
			current_output.clear();
		}
		else
		{
			//MessageBox(0, "Error: wc > 2", "Debug", 0);
		}
	}
};

struct extra_brackets_for_condition
	{ 
		// We need extra brackets for conditions, otherwise we might get problems,
		// if we combine open-ended when-conditions and normal when conditions
		// with and-operators, as the operators in conditions might be 
		// of lower priority.
		void operator()(const char *begin, const char *end) const 
		{ 
			current_output << "[ "; 
		} 
	};

struct extra_closing_brackets_for_condition
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			current_output << " ]"; 
		} 
	};


	struct reset_variables
	{
		void operator()(const char *begin, const char *end) const
		{
			when_others_fold_force_detected = false;
			when_others_when_others_fold_force_detected = false;
			open_ended_when_condition_detected = false;
		}
	};

	struct set_when_others_fold_force_detected
	{
		void operator()(const char *begin, const char *end) const
		{
			//MessageBox(0, "set_when_others_fold_force_detected", "Debug", 0);
			when_others_fold_force_detected = true;
		}
	};

 	struct set_when_others_when_others_fold_force_detected
	{
		void operator()(const char *begin, const char *end) const
		{
			//MessageBox(0, "set_when_others_when_others_fold_force_detected", "Debug", 0);
			when_others_when_others_fold_force_detected = true;
		}
	};

	struct set_open_ended_when_condition_detected
	{
		void operator()(const char *begin, const char *end) const
		{
			//MessageBox(0, "set_open_ended_when_condition_detected", "Debug", 0);
			open_ended_when_condition_detected = true;
			debug_2();
		}
	};

	struct check_for_correct_when_others_fold_force
	{
		void operator()(const char *begin, const char *end) const
		{ 
			//MessageBox(0, "check_for_correct_when_others_fold_force", "Debug", 0);

			bool correct_when_others_fold_force = false;
			if (open_ended_when_condition_detected && !when_others_when_others_fold_force_detected)
			{
				//MessageBox(0, "woff", "Error", 0); //!!!
			}
			else if (!open_ended_when_condition_detected && !when_others_fold_force_detected)
			{
				//MessageBox(0, "wowoff", "Error", 0); //!!!
			}
		}
	};

	struct print_comment_for_open_ended_when_condition
	{
		void operator()(const char *begin, const char *end) const
		{
			std::string text = std::string(begin, end);
			cout << "// Starting open-ended when-condition" << endl;
			cout << "// " << text << endl;
		}
	};

	struct print_newline
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			current_output << std::endl; 
		} 
	};

	struct print_function_header_for_betting_round
	{
		void operator()(const char *begin, const char *end) const 
		{ 
			std::string text = std::string(begin, end);
			if (text == "preflop")
			{ 
				current_output << "##f$preflop##" << endl;
			}
			else if (text == "flop")
			{
				current_output << "##f$flop##" << endl; 
			}
			else if (text == "turn")
			{
				current_output << "##f$turn##" << endl; 
			}
			else if (text == "river")
			{
				current_output << "##f$river##" << endl;
			}
			else
			{
				assert(k_assert_this_must_not_happen);
			}
		} 
	};

	struct print_keyword
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			if (1/*p_symbol_table->IsOpenPPLSymbol()*/)
			{
				// OpenPPL symbols and user-defined symbols 
				// get translated to f$...OH-script-symbols.
				// So we prepend "f$"
				current_output << "f$";
			}
			current_output << std::string(begin, end);
		} 
	}; 

	struct print_when_others_fold_force
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			current_output << "// When Others Fold Force" << endl;
			current_output << "f$Action_Fold" << endl; 
		} 
	};

	struct print_relative_potsize_action
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			current_output << "pot";
		} 
	};

	struct print_non_suited_board_expression
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			CString text = std::string(begin, end).c_str();
			generate_code_for_non_suited_board(text);
		} 
	};

	struct print_suited_board_expression
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			CString text = std::string(begin, end).c_str();
			generate_code_for_suited_board(text);
		} 
	};

	struct error_beep_not_supported
	{
		void operator()(const char *begin, const char *end) const 
		{
			ErrorMessage(k_error_beep_not_supported, ErroneousCodeSnippet(begin));
		}
	};

	struct error_specific_suits_not_supported
	{
		void operator()(const char *begin, const char *end) const 
		{
			ErrorMessage(k_error_specific_suits_not_supported, ErroneousCodeSnippet(begin));
		}
	};

	struct error_action_without_force
	{
		void operator()(const char *begin, const char *end) const 
		{
			ErrorMessage(k_error_action_without_force, ErroneousCodeSnippet(begin));
		}
	};

	struct error_missing_code_section
	{
		void operator()(const char *begin, const char *end) const 
		{
			ErrorMessage(k_error_missing_code_section, ErroneousCodeSnippet(begin));
		}
	};

	struct error_missing_closing_bracket
	{
		void operator()(const char *begin, const char *end) const 
		{
			ErrorMessage(k_error_missing_closing_bracket, ErroneousCodeSnippet(begin));
		}
	};

	struct error_missing_keyword_custom
	{
		void operator()(const char *begin, const char *end) const 
		{
			ErrorMessage(k_error_missing_keyword_custom, ErroneousCodeSnippet(begin));
		}
	};

	struct error_missing_brackets_for_card_expression
	{
		void operator()(const char *begin, const char *end) const 
		{
			ErrorMessage(k_error_missing_brackets_for_card_expression, ErroneousCodeSnippet(begin));
		}
	};

	struct error_invalid_character
		{
		void operator()(const char *begin, const char *end) const 
		{
			ErrorMessage(k_error_invalid_character, ErroneousCodeSnippet(begin));
		}
	};

	struct print_fold_as_last_alternative_for_when_condition_sequence
	{ 
		void operator()(const char *begin, const char *end) const 
		{ 
			current_output << "//\n"
				"// Undefined action\n"
				"// No backup action available (no built-in default bot)\n"
				"// Going to fold\n"
				"//\n"
				"f$Action_Fold\n\n"; 
		} 
	};

	struct print_hand_expression
	{
		void operator()(const char *begin, const char *end) const 
		{
			std::string text = std::string(begin, end);
			current_output << "$";
			int length = text.length();
			for (int i=0; i<length; i++)
			{
				char next_char = toupper(text[i]);
				if (next_char >= '0' && next_char <= '9')
				{
					current_output << next_char;
				}
				else if ((next_char == 'T') || (next_char == 'J')
					|| (next_char == 'Q') || (next_char == 'K') || (next_char == 'A'))
				{
					current_output << next_char;
				}
				else if (next_char == 'S')
				{
					// Suited
					current_output << "s";
					// Stop processing this part of the input,
					// as the rest is not relevant.
					break;
				}
			}
		}
	};

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
				>> ((keyword_custom [print_license()][print_options()]
					[reset_output_streams()]
				>> custom_sections)
					| missing_keyword_custom);
			custom_sections = !symbol_section
				>> (code_sections/* | missing_code_section !!!*/)
				>> end_p[flush_current_output_streams()]
					[print_prime_coded_board_ranks()]
					[print_predefined_action_constants()]
					[print_technical_functions()]
					[print_OpenPPL_Library()];
			missing_keyword_custom = (str_p("") >> custom_sections)[error_missing_keyword_custom()];
			  
			// Option settings - to be ignored
			option_settings_to_be_ignored = *single_option;
			single_option = option_name >> '=' >> option_value;
			option_name = alpha_p >> *(alnum_p | "-");
			keyword_on = str_p("on") | "On" | "ON";
			keyword_off = str_p("off") | "Off" | "OFF";
			option_value = keyword_on | keyword_off | number;

			// Start of custom code
			keyword_custom = str_p("custom") | "Custom" | "CUSTOM";

			// User defined symbols
			symbol_section = keyword_symbols >> *symbol_definition;
			keyword_new = str_p("new") | "New";
			keyword_symbols = str_p("symbols") | "Symbols";
			keyword_symbol = str_p("symbol") | "Symbol";
			keyword_end = str_p("end") | "End";
			symbol_definition = keyword_new >> keyword_symbol[reset_variables()]
				>> symbol//[print_function_header_for_betting_round()] 
				>> when_section
				>> keyword_end >> keyword_symbol;

			// Preflop, flop, turn, river
			code_sections = preflop_section[print_newline()][print_newline()]
				>> flop_section [print_newline()][print_newline()] 
				>> turn_section [print_newline()][print_newline()] 
				>> river_section [print_newline()][print_newline()];
			preflop_section = (keyword_preflop[print_function_header_for_betting_round()]
				[flush_current_output_streams()][reset_variables()][reset_output_streams()]
				>> when_section)[check_for_correct_when_others_fold_force()]
					[print_when_others_fold_force()];
			keyword_preflop = str_p("preflop") | "Preflop" | "PREFLOP";
			flop_section = (keyword_flop[print_function_header_for_betting_round()]
				[flush_current_output_streams()][reset_variables()][reset_output_streams()]
				>> when_section)[check_for_correct_when_others_fold_force()]
					[print_when_others_fold_force()];
			keyword_flop = str_p("flop") | "Flop" | "FLOP";
			turn_section = (keyword_turn[print_function_header_for_betting_round()]
				[flush_current_output_streams()][reset_variables()][reset_output_streams()]
				>> when_section)[check_for_correct_when_others_fold_force()]
					[print_when_others_fold_force()];
			keyword_turn = str_p("turn") | "Turn" | "TURN";
			river_section = (keyword_river[print_function_header_for_betting_round()]
				[flush_current_output_streams()][reset_variables()][reset_output_streams()]
				>> when_section)[check_for_correct_when_others_fold_force()]
					[print_when_others_fold_force()];
			keyword_river = str_p("river") | "River" | "RIVER";
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
			keyword_when = str_p("when") | "When" | "WHEN";
			when_condition = (keyword_when >> condition)[handle_when_condition()];
			when_condition_sequence = *(when_others_when_others_fold_force
				| when_others_fold_force
				| when_condition
				| action);

			// When Others Fold Force
			keyword_others = str_p("others") | "Others" | "OTHERS";
			when_others_fold_force = (keyword_when >> keyword_others >> keyword_fold 
				>> keyword_force)[set_when_others_fold_force_detected()];
			when_others_when_others_fold_force = (keyword_when >> keyword_others 
				>> when_others_fold_force)[set_when_others_when_others_fold_force_detected()];
			
			// Conditions
			condition = str_p("")[extra_brackets_for_condition()] >> expression
				[extra_closing_brackets_for_condition()];

			// Expressions
			expression = or_expression;
			bracket_expression = str_p("(")[print_bracket()] >> expression >> str_p(")")[print_bracket()];
			missing_closing_bracket_expression = (str_p("(") >> expression) >> str_p("")[error_missing_closing_bracket()];
			operand_expression = terminal_expression | bracket_expression 
				| missing_closing_bracket_expression;
			keyword_not = (str_p("not") | "Not" | "NOT")[print_operator()];
			unary_operator = (keyword_not | "-")[print_operator()];
			unary_expression = !unary_operator >> operand_expression;
			percentage_operator = str_p("%");
			multiplicative_operator = (str_p("*") | ("/"))[print_operator()] | percentage_operator[print_percentage_operator()]; 
			multiplicative_expression = unary_expression >> *(multiplicative_operator >> unary_expression);
			additive_operator = (str_p("+") | "-")[print_operator()];
			additive_expression = multiplicative_expression >> *(additive_operator >> multiplicative_expression);
			relational_operator = (str_p("<=") | ">=" | "<" | ">")[print_operator()];
			relational_expression = additive_expression >> *(relational_operator >> additive_expression);
			equality_expression = /*longest_d[*/hand_expression_with_brackets | board_expression_with_brackets 
				| hand_expression_without_brackets | board_expression_without_brackets
				| (relational_expression >> *(str_p("=")[print_operator()] >> relational_expression))/*]*/;
			keyword_and = (str_p("and") | "And" | "AND")[print_operator()];
			and_expression = equality_expression >> *(keyword_and >> equality_expression);
			keyword_xor = (str_p("xor") | "Xor" | "XOr" | "XOR")[print_operator()];
			xor_expression = and_expression >> *(keyword_xor >> and_expression);
			keyword_or = (str_p("or") | "Or" | "OR")[print_operator()];
			or_expression = xor_expression >> *(keyword_or >> xor_expression);

			// Hand and board expressions
			card_constant = lexeme_d[ch_p("A") | "a" | "K" | "k" | "Q" | "q" | "J" | "j" |
				"T" | "t" | "9" | "8" | "7" | "6" | "5" | "4" | "3" | "2"];
			keyword_suited = str_p("suited") | "Suited" | "SUITED";
			keyword_board = str_p("board") | "Board" | "BOARD";
			keyword_hand = str_p("hand") | "Hand" | "HAND";
			suit_constant = ch_p("C") | "c" | "D"| "d" | "H" | "h" | "S" | "s";
			card_expression = /*longest_d[*/card_expression_with_specific_suits
				| suited_card_expression
				| non_suited_card_expression; 
			suited_card_expression = (lexeme_d[+card_constant] >> keyword_suited);
			non_suited_card_expression = (lexeme_d[+card_constant]);
			card_expression_with_specific_suits = card_constant >> suit_constant >> *(card_constant >> !suit_constant)
				[error_specific_suits_not_supported()];
			board_expression_with_brackets = str_p("(") 
				>> (suited_board_expression | non_suited_board_expression) >> ")";
			suited_board_expression = (keyword_board >> str_p("=") 
				>> suited_card_expression)[print_suited_board_expression()];
			non_suited_board_expression = (keyword_board >> str_p("=") 
				>> non_suited_card_expression)[print_non_suited_board_expression()];	
			hand_expression_with_brackets = str_p("(") >> keyword_hand 
				>> str_p("=") >> card_expression[print_hand_expression()] >> ")";
			board_expression_without_brackets = (keyword_board 
				>> str_p("=")[print_operator()] >> card_expression)[error_missing_brackets_for_card_expression()];
			hand_expression_without_brackets = (keyword_hand 
				>> str_p("=") >> card_expression)[error_missing_brackets_for_card_expression()];
				 
			// Actions (and return statements)
			// We handle boths in the same way, as it simplifies things a lot.
			action = (action_without_force >> keyword_force)
					[handle_action()]//[flush_current_output_streams()][reset_output_streams()]
				| erroneous_action_without_force;
			action_without_force = predefined_action 
				| fixed_betsize_action 
				| relative_betsize_action
				| return_statement;
			predefined_action = keyword_predefined_action[print_predefined_action()];
			keyword_force = (str_p("force") | "Force" | "FORCE");
			keyword_beep = (str_p("beep") | "Beep" | "BEEP")[error_beep_not_supported()];
			keyword_call = str_p("call") | "Call" | "CALL";
			keyword_play = str_p("play") | "Play" | "PLAY";
			keyword_raise = str_p("raise") | "Raise" | "RAISE";
			keyword_raisemin = str_p("raisemin") | "Raisemin" | "RaiseMin" | "RAISEMIN";
			keyword_raisehalfpot = str_p("raisehalfpot") | "Raisehalfpot" | "RaiseHalfPot" | "RAISEHALFPOT";
			keyword_raisepot = str_p("raisepot") | "Raisepot" | "RaisePot" | "RAISEPOT";
			keyword_raisemax = str_p("raisemax") | "Raisemax" | "RaiseMax" | "RAISEMAX";
			keyword_allin = str_p("allin") | "Allin" | "ALLIN";
			keyword_fold = str_p("fold") | "Fold" | "FOLD";
			keyword_bet = str_p("bet") | "Bet" | "BET";
			keyword_betmin = str_p("betmin") | "Betmin" | "BetMin" | "BETMIN";
			keyword_bethalfpot = str_p("bethalfpot") | "Bethalfpot" | "BetHalfPot" | "BETHALFPOT";
			keyword_betpot = str_p("betpot") | "Betpot" | "BetPot" | "BETPOT";
			keyword_betmax = str_p("betmax") | "Betmax" | "BetMax" | "BETMAX";
			keyword_sitout = str_p("sitout") | "Sitout" | "SitOut" | "SITOUT";
			keyword_predefined_action = longest_d[keyword_beep | keyword_call	| keyword_play 
				| keyword_raisemin | keyword_raisehalfpot
				| keyword_raisepot | keyword_raisemax |	keyword_raise | keyword_fold
				| keyword_betmin | keyword_bethalfpot
				| keyword_betpot | keyword_betmax | keyword_bet | keyword_sitout];
			fixed_betsize_action = (keyword_bet | keyword_raise) >> number;
			relative_betsize_action = (keyword_bet | keyword_raise) >> number[print_number()] 
				>> percentage_operator[print_percentage_operator()][print_relative_potsize_action()] ;
			keyword_others = str_p("others") | "Others" | "OTHERS";
			erroneous_action_without_force = action_without_force[error_action_without_force()];

			// Return statement
			keyword_return = str_p("return") | "Return";
			return_statement = keyword_return >> expression >> keyword_force;

			// Terminal expressions
			terminal_expression = number[print_number()] | boolean_constant | symbol;
			number = real_p;
			keyword_true = str_p("true") | "True" | "TRUE";
			keyword_false = (str_p("false") | "False" | "FALSE")[print_keyword()];
			boolean_constant = keyword_true | keyword_false;
			// "Symbol" is a lexeme - we have to be very careful here.
			// We have to use the lexeme_d directive to disable skipping of whitespace,
			// otherwise things like "When x Bet force" would treat "x Bet force"
			// as a single keyword and cause an error.
			// http://www.boost.org/doc/libs/1_40_0/libs/spirit/classic/doc/quickref.html
			symbol = (lexeme_d[alpha_p >> *alnum_p][print_keyword()]) | invalid_symbol;

			// "Symbols" cintaining invalid cahracters
			invalid_character = str_p(";")  | "," | ":" | "|" | "@" | "€" | "!" | "\\"
				| "\""  | "§" | "$" | "&" | "?" | "´" | "´" | "[" | "]"
				| "^" | "°" | "{" | "}" | "#" | "³" | "²";
			invalid_symbol = (*alnum_p >> invalid_character 
				>> *(alnum_p | invalid_character))[error_invalid_character()];
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
	//!!!p_symbol_table = new(CSymbolTable);

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
} 

//missing keyword \"custom\"\n"
//			  "  * missing \"when others fold force\" at the end of a block\n"