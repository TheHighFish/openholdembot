// SemanticActions_Inc.cpp
//
// This file gets included as pure CPP as part of the parser definition,
// otherwise the parser would become way too large to keep it understandable
// and AFAIK we can't distribute it over multiple modules.

//
// Debugging functios
//

struct debug_1
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
//		MessageBox(0, "1", "Debug", 0);
	} 
};

struct debug_2
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
//		MessageBox(0, "2", "Debug", 0);
	} 
};

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

struct print_prime_coded_board_ranks
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::cout << k_code_snipped_prime_coded_board_ranks; 
	} 
};

struct print_predefined_action_constants
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::cout << k_code_snippet_predefined_constants; 
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

struct print_number 
	{ 
		void operator()(const char *begin, const char *end) const 
		{  
			current_output << std::string(begin, end);
		} 
	};

struct print_operator 
{ 
	void operator()(const char *begin, const char *end) const 
	{ 
		std::string text = std::string(begin, end);
		current_output << " " << text << " ";
		return;
		// Some operators have to be translated,
		// as they are named differently in OpenPPL and OH-script.
		if (text == "and")
		{
			current_output << "&&";
		}
		else if (text == "or")
		{
			current_output << "||";
		}
		else if (text == "xor")
		{
			current_output << "^^";
		}
		else if (text == "not")
		{
			current_output << "!";
		}
		else if (text == "bitand")
		{
			current_output << "&";
		}
		else if (text == "bitor")
		{
			current_output << "|";
		}
		else if (text == "bitxor")
		{
			current_output << "^";
		}
		else if (text == "bitnot")
		{
			current_output << "~";
		}
		else if (text == "=")
		{
			current_output << "==";
		}
		else if (text == "mod")
		{
			// Modulo needs special treatment,
			// as "%" gets used as the percentage-operator in OpenPPL.
			current_output << "%";
		}
		else
		{
			// No translation necessary
			// The operators are names the same way in both languages.
			current_output << text;
		}
		current_output << " ";
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
			current_output << "f$Action_Call";
		}
		else if ((text == "raisemin") || (text == "betmin"))
		{
			current_output << "f$Action_RaiseMin";
		}
		else if ((text == "raisehalfpot") || (text == "bethalfpot"))
		{
			current_output << "f$Action_RaiseHalfPot";
		}
		else if ((text == "raisepot") || (text == "betpot"))
		{
			current_output << "f$Action_RaisePot";
		}
		else if ((text == "raisemax") || (text == "betmax"))
		{
			current_output << "f$Action_RaiseMax";
		}		
		else if ((text == "raise") || (text == "bet"))
		{
			current_output << "f$Action_Raise";
		}
		else if (text == "fold")
		{
			current_output <<  "f$Action_Fold";
		}
		else if (text == "sitout")
		{
			current_output << "f$Action_SitOut";
		}
		// Beep not supported and handled otherwhere.
	} 
};



// Error messages

