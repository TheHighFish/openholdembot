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


#include "LineCounter.h"
#include "ListOfOpenHoldemSymbolPrefixes.h"

// SemanticActions_Inc.cpp
//
// This file gets included as pure CPP as part of the parser definition,
// otherwise the parser would become way too large to keep it understandable
// and AFAIK we can't distribute it over multiple modules.

#undef DEBUG_WHEN_CONDITIONS
#undef DEBUG_SYMBOLS
#undef DEBUG_USER_VARIABLES

//
// Larger code-snippets
//

struct init_variables
{
	// Initialized at run-time, because...
	// only static const integral data members can be initialized within a class
	void operator()(const char *begin, const char *end) const 
	{ 
		bool prefold_section_found = false;
		bool preflop_section_found = false;
		bool flop_section_found = false;
		bool turn_section_found = false;
		bool river_section_found = false;
	} 
};

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
#ifdef DEBUG_SYMBOLS
			current_output << "\n// OpenPPL-symbol\n";
#endif DEBUG_SYMBOLS
			current_output << p_symbol_table->GetSymbolNameWithcorrectCases(symbol.c_str());
		}
		else if (p_list_of_openholdem_symbol_prefixes->LooksLikeOpenHoldemSymbol(CString(symbol.c_str())))
		{
#ifdef DEBUG_SYMBOLS
			current_output << "\n// OpenHoldem-symbol\n";
#endif DEBUG_SYMBOLS
			current_output << symbol;
		}
		// Ubknown symbol
		else if (!p_symbol_table->GenerationOfSymboltableInProgress())
		{
			CString c_symbol = symbol.c_str();
			if (c_symbol.MakeLower().Left(4) == "user")
			{
				// User-defined variable
				//
				// Removing underscores, as they are used as separators
				// and would mix things up. 
				c_symbol.Remove('_');
#ifdef DEBUG_SYMBOLS
			current_output << "\n// User-defined variable (me_re_)\n";
#endif DEBUG_SYMBOLS
				current_output << "me_re_" << c_symbol.MakeLower();
			}
			else if (c_symbol.MakeLower().Right(6) == "suited")
			{
				ErrorMessage(k_error_unknown_symbol_ending_with_suited, 
					ErroneousCodeSnippet(begin));
			}
			else
			{
				ErrorMessage(k_error_unknown_symbol, ErroneousCodeSnippet(begin));
				// Output anyway, as 
				//   * some symbols are missing, that will be removed soon
				//   * some new symbols might be missing, but the translation
				//     should work anyway.
				current_output << symbol;
			}
		}
		else
		{
#ifdef DEBUG_SYMBOLS
			current_output << "\n// Nothing to do (generation of symbol table)\n";
#endif DEBUG_SYMBOLS
		}
	} 
}; 

struct print_function_header_for_betting_round
{
	void operator()(const char *begin, const char *end) const 
	{ 
		std::string text_as_std_string = std::string(begin, end);
		CString text = text_as_std_string.c_str();
		text = text.MakeLower();
		if (text == "prefold")
		{ 
			// Prefold needs 2 functions:
			// * one f$prefold that returns true / false
			// * one f$OpenPPL_prefold, that returns Fold / NoFold
			cout << "##f$OpenPPL_prefold##" << endl;
		}
		else if (text == "preflop")
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
			// Must be already in the symbol table,
			// as we did a first parse for symbol definitions.
			cout << "##" << p_symbol_table->GetSymbolNameWithcorrectCases(text) 
				<< "##" << endl;
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

struct print_fixed_betsize_action
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		current_output << " * f$RaiseByBigBlinds";
	} 
};

struct print_relative_potsize_action
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		// f$RaiseByPercentagedPotsize evaluates to -0.01
		// A negative value means potsize.
		current_output << " * f$RaiseByPercentagedPotsize";
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
			current_output << " ^^ ";
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
		else if (text == "%")
		{
			// Percentage-operator
			// Don't use " / 100 *" because that can lead to an integer-division,
			// and 70/100 will unexpectedly get evaluated as 0.
			current_output << " * 0.01 * ";
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

static const int k_number_of_different_brackets = 2;

struct print_opening_bracket 
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		// OpenHoldem supports 3 kinds of brackets: (), [], {}
		// We only use the first 2 pairs, because the 3rd one
		// does IMO look too similar to the first one.
		char open_brackets[k_number_of_different_brackets + 1] = "([";
		current_output << open_brackets[bracket_counter % k_number_of_different_brackets];
		bracket_counter++;
	}		
};

struct print_closing_bracket 
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		bracket_counter--;
		char close_brackets[k_number_of_different_brackets + 1] = ")]";
		current_output << close_brackets[bracket_counter % k_number_of_different_brackets];
	} 
};

struct print_percentage_operator
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		// Don't use " / 100 *" because that can lead to an integer-division,
		// and 70/100 will unexpectedly get evaluated as 0.
		current_output << " * 0.01 * ";
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
			current_output << "f$OpenPPL_Call";
		}
		else if (text == "check")
		{
			current_output << "f$OpenPPL_Check";
		}
		else if ((text == "raisemin") || (text == "betmin"))
		{
			current_output << "f$OpenPPL_RaiseMin";
		}
		else if ((text == "raisehalfpot") || (text == "bethalfpot"))
		{
			current_output << "f$OpenPPL_RaiseHalfPot";
		}
		else if ((text == "raisepot") || (text == "betpot"))
		{
			current_output << "f$OpenPPL_RaisePot";
		}
		else if ((text == "raisemax") || (text == "betmax") || (text == "allin"))
		{
			current_output << "f$OpenPPL_RaiseMax";
		}		
		else if ((text == "raise") || (text == "bet"))
		{
			current_output << "f$OpenPPL_Raise";
		}
		else if (text == "fold")
		{
			current_output <<  "f$OpenPPL_Fold";
		}
		else if (text == "sitout")
		{
			current_output << "f$OpenPPL_SitOut";
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
		open_ended_when_condition_detected = false;
	}
};

struct print_when_others_fold_force
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		cout << "//" << endl;
		cout << "// When Others Fold Force" << endl;
		cout << "// Automatically added, just to be safe" << endl;
		cout << "//" << endl;
		cout << "f$OpenPPL_Fold" << endl << endl << endl; 
	} 
};

struct print_when_others_no_prefold_force
{
	void operator()(const char *begin, const char *end) const 
	{ 
		cout << "//" << endl;
		cout << "// When Others Do_Not_Prefold Force" << endl;
		cout << "// Automatically added for syntactical completeness." << endl;
		cout << "//" << endl;
		cout << "f$OpenPPL_Do_Not_PreFold" << endl << endl << endl; 
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
// Our simplified grammar allows arbitrary sequences 
// of (open-ended) when-conditions and actions.
// But we can more easily generate OH-script-code and check 
// for illegal Open-PPL-code in "handle_when_condition()"
// this way.
//
struct handle_when_condition
{
	void operator()(const char *begin, const char *end) const 
	{
		assert(when_conditions_since_last_action >= 0);
		when_conditions_since_last_action++;
		if (when_conditions_since_last_action == 1)
		{
			// 1st when-condition found
			// When-condition with action
			original_source_of_current_when_condition = string(begin, end);
			current_when_condition.str("");
			current_when_condition.clear();
			current_when_condition << current_output.str();
#ifdef DEBUG_WHEN_CONDITIONS
			cout << "WHEN CONDITION: " << current_when_condition.str() << endl;
#endif
			current_output.str("");
			current_output.clear();
		}
		else if (when_conditions_since_last_action == 2)
		{
			// 2nd when-condition found
			// Open-ended when condition
			open_ended_when_condition_detected = true;
			original_source_of_current_open_ended_when_condition =
				original_source_of_current_when_condition;
			current_open_ended_when_condition.str("");
			current_open_ended_when_condition.clear();
			current_open_ended_when_condition << current_when_condition.str();
			current_when_condition.str("");
			current_when_condition.clear();
			current_when_condition << current_output.str();
#ifdef DEBUG_WHEN_CONDITIONS
			cout << "WHEN CONDITION (OPEN ENDED): " << current_open_ended_when_condition.str() << endl;
			cout << "WHEN CONDITION: " << current_when_condition.str() << endl;
#endif
			cout << "//" << endl;
			cout << "// Starting open-ended when-condition" << endl;
			cout << "// " << original_source_of_current_open_ended_when_condition << endl;
			cout << "//" << endl;
			current_output.str("");
			current_output.clear();
		}
		else
		{
			// More than 2 when-conditions found: 
			// illegal Open-PPL-code
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
		CString text = std::string(begin, end).c_str();
		generate_code_for_hand_expression(text);
	}
};

struct print_hand_expression_with_specific_suits
{
	void operator()(const char *begin, const char *end) const 
	{
		CString text = std::string(begin, end).c_str();
		generate_code_for_hand_expression_with_specific_suits(text);
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
		current_output.str("");
		current_output.clear();
	}
};

struct print_setting_UDV
{
	void operator()(const char *begin, const char *end) const
 	{
 		std::string text = std::string(begin, end);
 		CString ctext = text.c_str();
#ifdef DEBUG_USER_VARIABLES
		cout << endl << "Uservariable to be set: " << ctext << endl;	
#endif 
		// Removing underscores, as they are used as separators
		// and would mix things up, e.g. me_st_Pi_3_141592653
		ctext.Remove('_');
 		//Add to list of user variables if not yet known
 		CSMap::const_iterator find_result = user_vars.find(ctext.MakeLower());
 		if (find_result == user_vars.end())
 		{
 			user_vars[ctext.MakeLower()] = ctext.MakeLower();
 		}
 		when_conditions_since_last_action = 0;
 		if (open_ended_when_condition_detected)
 		{
 			cout << current_open_ended_when_condition.str() << "  &&  ["
 				 << current_when_condition.str() << " ? "
 				 << "me_st_" << ctext.MakeLower() << "_1 : 0] && 0 ? 0 :" << endl;
 		}
 		else
 		{
 			cout << "[ " << current_when_condition.str() << " ? "
 				 << "me_st_" << ctext.MakeLower() << "_1 : 0] && 0 ? 0 :" << endl;
 		}
 		current_output.str("");
 		current_output.clear();
 	}
};

struct print_recalling_UDV
{
	void operator()(const char *begin, const char *end) const
 	{
		std::string text = std::string(begin, end);
 		CString ctext = text.c_str();
		cout << "me_re_" << ctext.MakeLower();
	}
};

struct print_prefold_function_if_necessary
{
 	void operator()(const char *begin, const char *end) const
 	{
		if (prefold_section_found)
		{
			cout << k_code_snippet_prefold_function;
		}
	}

};


struct print_user_reset_function
{
 	void operator()(const char *begin, const char *end) const
 	{
		map<CString, CString>::iterator p;
		cout << k_code_snippet_reset_user_variables;
		cout << "##f$OpenPPL_ResetUserVariables##" << endl;
		cout << "0" << endl;
   		for(p = user_vars.begin(); p != user_vars.end(); p++) 
		{
 			cout << "+  me_st_" << p->first << "_0" << endl;
 		}
		cout << endl << endl;
 	}
};


struct print_memory_store_command
{
	void operator()(const char *begin, const char *end) const 
	{
		std::string text = std::string(begin, end);
 		CString ctext = text.c_str();
		// The memory_store_command may contain a space after "me_st_",
		// despitee there was no in the source code. Heaven knows why.
		// We remove it.
		ctext.Remove(' ');
 		when_conditions_since_last_action = 0;
 		if (open_ended_when_condition_detected)
 		{
 			cout << current_open_ended_when_condition.str() << "  &&  ["
 				 << current_when_condition.str() << " ? "
 				 << ctext << " : 0] && 0 ? 0 :" << endl;
 		}
 		else
 		{
 			cout << "[ " << current_when_condition.str() << " ? "
 				 << ctext << " : 0] && 0 ? 0 :" << endl;
 		}
 		current_output.str("");
 		current_output.clear();
	}
};

struct print_hand_in_list
{
	void operator()(const char *begin, const char *end) const 
	{
		std::string text = std::string(begin, end);
		cout << text << " ";
	}
};

struct print_newline
{
	void operator()(const char *begin, const char *end) const 
	{
		cout << endl;
	};
};

struct increase_line_counter
{
	void operator()(const char *begin, const char *end) const 
	{
		IncLineCounter();
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

struct error_invalid_card_expression
{
	void operator()(const char *begin, const char *end) const 
	{
		ErrorMessage(k_error_invalid_card_expression, ErroneousCodeSnippet(begin));
	}
};

struct error_action_without_force
{
	void operator()(const char *begin, const char *end) const 
	{
		ErrorMessage(k_error_action_without_force, ErroneousCodeSnippet(begin));
	}
};

struct register_code_section
{
	void operator()(const char *begin, const char *end) const 
	{
		CString text = std::string(begin, end).c_str();
		CString text_lowercase = text.MakeLower();
		if (text == "prefold")
		{
			prefold_section_found = true;
		}
		else if (text == "preflop")
		{
			preflop_section_found = true;
		}
		else if (text == "flop")
		{
			flop_section_found = true;
		}
		else if (text == "turn")
		{
			turn_section_found = true;
		}
		else if (text == "river")
		{
			river_section_found = true;
		}
		else
		{
			MessageBox(0, "Internal Error: invalid name of code section detected.", "Error", 0);
		}
	}
};


struct check_for_missing_code_section
{
	void operator()(const char *begin, const char *end) const 
	{
		if ((preflop_section_found == false)
			|| (flop_section_found == false)
			|| (turn_section_found == false)
			|| (river_section_found == false))
		{
			ErrorMessage(k_error_missing_code_section, ErroneousCodeSnippet(begin));
		}
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

struct error_missing_brackets_for_card_expression
{
	void operator()(const char *begin, const char *end) const 
	{
		ErrorMessage(k_error_missing_brackets_for_card_expression, ErroneousCodeSnippet(begin));
	}
};

struct error_missing_brackets_for_when_condition
{
	void operator()(const char *begin, const char *end) const 
	{
		ErrorMessage(k_error_missing_brackets_for_when_condition, ErroneousCodeSnippet(begin));
	}
};

struct error_invalid_character
{
	void operator()(const char *begin, const char *end) const 
	{
		ErrorMessage(k_error_invalid_character, ErroneousCodeSnippet(begin));
	}
};

struct error_operator_instead_of_action
{
	void operator()(const char *begin, const char *end) const 
	{
		ErrorMessage(k_error_operator_instead_of_action, ErroneousCodeSnippet(begin));
	}
};

struct error_missing_list_number
{
	void operator()(const char *begin, const char *end) const 
	{
		ErrorMessage(k_error_missing_list_number, ErroneousCodeSnippet(begin));	
	}
};

struct error_invalid_list_content
{
	void operator()(const char *begin, const char *end) const 
	{
		ErrorMessage(k_error_invalid_list_content, ErroneousCodeSnippet(begin));	
	}
};