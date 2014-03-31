#ifndef INC_CFUNCTIONCOLLECTION_H
#define INC_CFUNCTIONCOLLECTION_H

#include "CFunction.h"

class CFunctionCollection
{
public:
	CFunctionCollection();
	~CFunctionCollection();
public:
	void Clear();
	void Add(CFunction *new_function);
	double Evaluate(CString function_name);
private:
	CMap< CString*, CString*, CFunction*, CFunction*>_function_hashtable;
};

extern CFunctionCollection *p_function_collection;

#endif INC_CFUNCTIONCOLLECTION_H