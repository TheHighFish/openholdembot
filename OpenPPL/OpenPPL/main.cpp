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
				>> custom_sections)
					| missing_keyword_custom);
			custom_sections = !symbol_section
				>> (code_sections/* | missing_code_section !!!*/)
				>> end_p
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
			keyword_new = str_p("new") | "New" | "NEW";
			keyword_symbols = str_p("symbols") | "Symbols" | "SYMBOLS";
			keyword_symbol = str_p("symbol") | "Symbol" | "SYMBOL";
			keyword_end = str_p("end") | "End" | "END";
			symbol_definition = keyword_new >> keyword_symbol[reset_variables()]
				>> symbol//[print_function_header_for_betting_round()] 
				>> when_section
				>> keyword_end >> keyword_symbol;

			// Preflop, flop, turn, river
			code_sections = preflop_section
				>> flop_section 
				>> turn_section 
				>> river_section;
			preflop_section = (keyword_preflop[print_function_header_for_betting_round()]
				>> when_section)[check_for_correct_when_others_fold_force()]
					[print_when_others_fold_force()][reset_variables()];
			keyword_preflop = str_p("preflop") | "Preflop" | "PREFLOP";
			flop_section = (keyword_flop[print_function_header_for_betting_round()]
				>> when_section)[check_for_correct_when_others_fold_force()]
					[print_when_others_fold_force()][reset_variables()];
			keyword_flop = str_p("flop") | "Flop" | "FLOP";
			turn_section = (keyword_turn[print_function_header_for_betting_round()]
				>> when_section)[check_for_correct_when_others_fold_force()]
					[print_when_others_fold_force()][reset_variables()];
			keyword_turn = str_p("turn") | "Turn" | "TURN";
			river_section = (keyword_river[print_function_header_for_betting_round()]
				>> when_section)[check_for_correct_when_others_fold_force()]
					[print_when_others_fold_force()][reset_variables()];
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
			// We handle both in the same way, as it simplifies things a lot.
			action = (action_without_force >> keyword_force)
					[handle_action()]
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
	p_symbol_table = new(CSymbolTable);
	p_symbol_table->AddSymbolsFromFile("OpenPPL_Library.ohf");
	p_symbol_table->AddSymbolsFromFile(InputFile);

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