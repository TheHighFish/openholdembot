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


#include "AddingOfIncludeFiles.h"
#include <afx.h>
#include <fstream> 
#include <iostream> 

using namespace std;

void AppendFileToOHFOutputIfFileExists(CString file_name, bool error_on_failure)
{
	fstream file_to_append(file_name);

	// 10000 should be really long enough,
	// as this function won't be used for auto-generated code
	// and even that should only exceed the limit for very crazy Shanky-botters.
	const int max_line_length = 10000;
	char next_line[max_line_length];
	if (file_to_append.is_open())
	{
		while (!file_to_append.eof())
		{
			file_to_append.getline(next_line, max_line_length);
			cout << next_line << endl;
		}
	}
	else if (error_on_failure)
	{
		CString error_message;
		error_message.Format("File not found: %s", file_name);
		MessageBox(0, error_message, "ERROR", 0);
		exit(-1);
	}
}