#ifndef INC_MEMORY_H
#define INC_MEMORY_H

class CEvalInfoFunction;
	
extern class CMemory 
{
public:
	// public functions
	CMemory();
	~CMemory();
	const double ProcessQuery(const char * pquery, CEvalInfoFunction **logCallingFunction, int *e);

private:
	// private functions and variables - not available via accessors or mutators
	void StoreValue(const char * pquery, CEvalInfoFunction **logCallingFunction, int *e);
	double RetrieveValue(const char * pquery, int *e);

	int		_var_count;
	char	_var_name[512][64];
	double	_var_value[512];

} *p_memory;

#endif //INC_MEMORY_H