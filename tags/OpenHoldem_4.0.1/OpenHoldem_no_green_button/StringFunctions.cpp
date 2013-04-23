#include <stdafx.h>
#include "StringFunctions.h"

bool StringIsExactMatch(const char *string_a, const char *string_b)
{
	return ((strlen(string_a) == strlen(string_b))
		&& StringAIsPrefixOfStringB(string_a, string_b));
}

bool StringAIsPrefixOfStringB(const char *string_a, const char *string_b)
{
	if (strlen(string_a) > strlen(string_b))
	{
		return false;
	}
	// Result of memcmp == 0 means: identical
	// See http://www.cplusplus.com/reference/clibrary/cstring/memcmp/
	return (memcmp(string_a, string_b, strlen(string_a)) == 0);
}