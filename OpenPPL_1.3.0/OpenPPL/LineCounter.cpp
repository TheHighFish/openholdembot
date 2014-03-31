#include "Linecounter.h"

long int line_counter = 1;

void IncLineCounter()
{
	line_counter++;
}

int GetLineCount()
{
	return line_counter;
}