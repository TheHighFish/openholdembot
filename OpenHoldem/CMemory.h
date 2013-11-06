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

#ifndef INC_MEMORY_H
#define INC_MEMORY_H

#include "..\CCritSec\CCritSec.h"
#include "MagicNumbers.h"

// Forward declaration
class CEvalInfoFunction;
	
extern class CMemory 
{
public:
	// public functions
	CMemory();
	~CMemory();
	const double ProcessQuery(const char * pquery, CEvalInfoFunction **logCallingFunction, int *e);

private:
#define ENT CSLock lock(m_critsec);
	// private mutators
	void	set_var_count(const int i) { ENT _var_count = i; }
	void	set_var_name(const int i, const char *name)    { ENT strcpy_s(_var_name[i], 64, name); }
	void	set_var_value(const int i, const double value) { ENT _var_value[i] = value; }
#undef ENT

private:
	// private functions and variables - not available via accessors or mutators
	bool RightValueIsNumber(char first_character_of_right_value);
	bool RightValueIsMemorySymbol(const char *value);
	bool RightValueIsFunction(const char *value);
	void StoreValue(const char * pquery, CEvalInfoFunction **logCallingFunction, int *e);
	double RetrieveValue(const char * pquery, int *e);

private:
	int		_var_count;
	char	_var_name[k_max_number_of_memory_symbols][k_max_length_of_memory_symbols];
	double	_var_value[k_max_number_of_memory_symbols];

	CCritSec			m_critsec;

} *p_memory;

#endif //INC_MEMORY_H