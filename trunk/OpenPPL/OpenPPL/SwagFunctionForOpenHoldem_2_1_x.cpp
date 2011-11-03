#include "SwagFunctionForOpenHoldem_2_1_x.h"
#include <afx.h>
#include <fstream> 
#include <iostream> 

using namespace std;

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