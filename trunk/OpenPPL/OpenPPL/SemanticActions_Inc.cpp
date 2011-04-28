#include "ListOfOpenHoldemSymbolPrefixes.h"

// SemanticActions_Inc.cpp
//
// This file gets included as pure CPP as part of the parser definition,
// otherwise the parser would become way too large to keep it understandable
// and AFAIK we can't distribute it over multiple modules.

//
// Larger code-snippets
//

struct print_license
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::cout << k_code_snippet_license; 
	} 
};

struct print_options
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::cout << k_code_snippet_options; 
	} 
};

struct print_comment_for_list_section
{
	void operator()(const char *begin, const char *end) const 
	{ 
		std::cout << k_code_snippet_list_section; 
	}
};

struct print_user_defined_functions
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::cout << k_code_snippet_user_defined_functions; 
	} 
};

struct print_main_code_sections
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::cout << k_code_snippet_start_of_code_sections; 
	} 
};

struct print_prime_coded_board_ranks
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::cout << k_code_snipped_prime_coded_board_ranks; 
	} 
};

struct print_technical_functions
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::cout << k_code_snippet_technical_functions; 
	} 
};

struct print_OpenPPL_Library
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		string next_line;
		ifstream OpenPPL_Library("OpenPPL_Library.ohf");
		if (OpenPPL_Library.is_open())
		{
			while (OpenPPL_Library.good())
			{
				getline (OpenPPL_Library, next_line);
				cout << next_line << endl;
			}
			OpenPPL_Library.close();
		}
		else 
		{
			MessageBox(0, "Unable to open \"OpenPPL_Library.ohf\"", "Error", 0);
		}
	}
};

// 
// Easy to translate tokens
//

struct print_symbol
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::string symbol = std::string(begin, end);
		if (p_symbol_table->IsOpenPPLSymbol(symbol.c_str()))
		{
			// OpenPPL symbols and user-defined symbols 
			// get translated to f$...OH-script-symbols.
			// So we prepend "f$"
			current_output << p_symbol_table->GetStandardizedSymbolName(symbol.c_str());
		}
		else if (p_list_of_openholdem_symbol_prefixes->LooksLikeOpenHoldemSymbol(CString(symbol.c_str())))
		{
			current_output << symbol;
		}
		else if (!p_symbol_table->GenerationOfSymboltableInProgress())
		{
			ErrorMessage(k_error_unknown_symbol, ErroneousCodeSnippet(begin));
			// Output anyway, as 
			//   * some symbols are missing, that will be removed soon
			//   * some new symbols might be missing, but the translation
			//     should work anyway.
			current_output << symbol;
		}
	} 
}; 

struct print_function_header_for_betting_round
{
	void operator()(const char *begin, const char *end) const 
	{ 
		std::string text = std::string(begin, end);
		if (text == "preflop")
		{ 
			cout << "##f$preflop##" << endl;
		}
		else if (text == "flop")
		{
			cout << "##f$flop##" << endl; 
		}
		else if (text == "turn")
		{
			cout << "##f$turn##" << endl; 
		}
		else if (text == "river")
		{
			cout << "##f$river##" << endl;
		}
		else
		{
			// User defined function ("New Symbol ...")
			cout << "##f$OpenPPL_" << text << "##" << endl;
		}
		current_output.str("");
		current_output.clear();
	} 
};

struct print_list_header
{
	void operator()(const char *begin, const char *end) const 
	{ 
		std::string text = std::string(begin, end);
		cout << "##list" << text << "##" << endl;
		current_output.str("");
		current_output.clear();;
	} 
};

struct print_number 
{ 
	void operator()(const char *begin, const char *end) const 
	{  
		current_output << std::string(begin, end);
	} 
};

struct print_comment_for_fixed_betsize
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		current_output << " /*** big blinds ***/";
	} 
};

struct print_relative_potsize_action
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		current_output << "f$OpenPPL_PotSize";
	} 
};

struct print_operator 
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::string original_text = std::string(begin, end);
		CString text = original_text.c_str();
		text.MakeLower();
		// Some operators have to be translated,
		// as they are named differently in OpenPPL and OH-script.
		if (text == "and")
		{
			current_output << " && ";
		}
		else if (text == "or")
		{
			current_output << " || ";
		}
		else if (text == "xor")
		{
			current_output << "^^";
		}
		else if (text == "not")
		{
			// No space after unary operator
			current_output << " !";
		}
		else if (text == "bitand")
		{
			current_output << " & ";
		}
		else if (text == "bitor")
		{
			current_output << " | ";
		}
		else if (text == "bitxor")
		{
			current_output << " ^ ";
		}
		else if (text == "bitnot")
		{
			// No space after unary operator
			current_output << " ~";
		}
		else if (text == "=")
		{
			current_output << " == ";
		}
		else if (text == "mod")
		{
			// Modulo needs special treatment,
			// as "%" gets used as the percentage-operator in OpenPPL.
			current_output << " % ";
		}
		else
		{
			// No translation necessary
			// The operators are names the same way in both languages.
			current_output << " " << text << " ";
		};
	} 
};

struct print_bracket 
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::string text = std::string(begin, end);
		if (text == "(")
		{
			char open_brackets[4] = "([{";
			current_output << open_brackets[bracket_counter%3];
			bracket_counter++;
		}
		else
		{
			bracket_counter--;
			char close_brackets[4] = ")]}";
			current_output << close_brackets[bracket_counter%3];
			bracket_counter++;
		}
	} 
};

struct print_percentage_operator
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		current_output << "/100 * ";
	} 
};

struct print_predefined_action
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::string text = std::string(begin, end);
		std::transform(begin, end, text.begin(), std::tolower);
		if ((text == "call") || (text == "play"))
		{
			current_output << "f$OpenPPL_Action_Call";
		}
		else if ((text == "raisemin") || (text == "betmin"))
		{
			current_output << "f$OpenPPL_Action_RaiseMin";
		}
		else if ((text == "raisehalfpot") || (text == "bethalfpot"))
		{
			current_output << "f$OpenPPL_Action_RaiseHalfPot";
		}
		else if ((text == "raisepot") || (text == "betpot"))
		{
			current_output << "f$OpenPPL_Action_RaisePot";
		}
		else if ((text == "raisemax") || (text == "betmax") || (text == "allin"))
		{
			current_output << "f$OpenPPL_Action_RaiseMax";
		}		
		else if ((text == "raise") || (text == "bet"))
		{
			current_output << "f$OpenPPL_Action_Raise";
		}
		else if (text == "fold")
		{
			current_output <<  "f$OpenPPL_Action_Fold";
		}
		else if (text == "sitout")
		{
			current_output << "f$OpenPPL_Action_SitOut";
		}
		// Beep not supported and handled otherwhere.
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

//
// When Others Fold Force
//

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

struct check_for_correct_when_others_fold_force
{
	void operator()(const char *begin, const char *end) const
	{ 
		if (open_ended_when_condition_detected && !when_others_when_others_fold_force_detected)
		{
			ErrorMessage(k_error_missing_when_others_fold_force, ErroneousCodeSnippet(begin));
		}
		else if (!open_ended_when_condition_detected && !when_others_fold_force_detected)
		{
			ErrorMessage(k_error_missing_when_others_fold_force, ErroneousCodeSnippet(begin));
		}
	}
};

struct print_when_others_fold_force
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		cout << "//" << endl;
		cout << "// When Others Fold Force" << endl;
		cout << "//" << endl;
		cout << "f$OpenPPL_Action_Fold" << endl << endl << endl; 
	} 
};

struct print_default_return_value_for_user_defined_symbol
{ 
	void operator()(const char *begin, const char *end) const 
	{
		cout << "//" << endl;
		cout << "// To ensure syntactical completeness" << endl;
		cout << "// and to take care about undefined cases..." << endl;
		cout << "//" << endl;
		cout << "f$OpenPPL_UndefinedReturnValue" << endl << endl << endl; 
	}
};

//
// (Open-ended) when-conditions
//

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
			ErrorMessage(k_error_too_many_open_ended_when_conditions,
				ErroneousCodeSnippet(begin));
		}
	}
};

//
// Hand and board expressions
//

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

//
// Actions
//

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

//
// Error messages
//

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