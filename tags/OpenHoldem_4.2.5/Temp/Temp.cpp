// Temp.cpp : main project file.

#include "stdafx.h"

using namespace System;

String ^PreprocessString(String ^input)
{
	// Needs probably 4 sub-functions as discussed in the forum
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=114&t=15038
	return input;
}

int main(array<String ^> ^args)
{
	Console::WriteLine(L"Please enter any number or title-string for preprocessing");
	String ^input = Console::ReadLine();
	String ^preprocessed_input = PreprocessString(input);
	Console::WriteLine(preprocessed_input);
    return 0;
}
