//#define BOOST_SPIRIT_DEBUG  ///$$$ DEFINE THIS BEFORE ANYTHING ELSE $$$///
//
// For details see http://www.boost.org/doc/libs/1_35_0/libs/spirit/doc/debugging.html

#include <afx.h>
#include <boost/spirit.hpp> 
#include <boost/spirit/iterator/position_iterator.hpp>

#include <assert.h>
#include <fstream> 
#include <iostream> 
#include <sstream> 
#include <stdio.h>
#include "AddingOfIncludeFiles.h"
#include "Board.h"
#include "CodeSnippets.h"
#include "CSymbolTable.h"
#include "ErrorMessages.h"
#include "ListOfOpenHoldemSymbolPrefixes.h"

using namespace boost::spirit;
using namespace std;

const bool k_assert_this_must_not_happen = false;

bool open_ended_when_condition_detected = false;
int when_conditions_since_last_action = 0;

typedef map<CString, CString> CSMap;
CSMap user_vars;

static bool prefold_section_found; 
static bool preflop_section_found; 
static bool flop_section_found;   
static bool turn_section_found;   
static bool river_section_found;  

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
	CString erroneous_code_snippet = rest_of_input.Left(500);
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
				=   (space_p)
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


// Grammar_Inc.cpp
//
// This file gets included as pure CPP as part of the parser definition,
// otherwise the parser would become way too large to keep it understandable
// and AFAIK we can't distribute it over multiple modules.
#include "Grammar_Inc.cpp"


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

		MessageBox(0, error_message, "Error", 0);
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
		// Last step: add include files
		// Swag-functions are depricated now, as OH 4.0.0s f$betsize-function
		// cares about that

		/*
		AppendFileToOHFOutputIfFileExists(CString k_swag_file_name, true);
		*/
		AppendFileToOHFOutputIfFileExists(k_hand_strength_library_file_name, true);
		AppendFileToOHFOutputIfFileExists(k_custom_OHF_file_name, false);
	}
} 