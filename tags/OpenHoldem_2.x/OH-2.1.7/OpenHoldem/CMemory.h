#ifndef INC_MEMORY_H
#define INC_MEMORY_H

#include "..\CCritSec\CCritSec.h"

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
	void	set_var_name(const int i, const char *s) { ENT strcpy_s(_var_name[i], 64, s); }
	void	set_var_value(const int i, const double d) { ENT _var_value[i] = d; }
#undef ENT

private:
	// private functions and variables - not available via accessors or mutators
	void StoreValue(const char * pquery, CEvalInfoFunction **logCallingFunction, int *e);
	double RetrieveValue(const char * pquery, int *e);

	int		_var_count;
	char	_var_name[512][64];
	double	_var_value[512];

	CCritSec			m_critsec;

} *p_memory;

#endif //INC_MEMORY_H