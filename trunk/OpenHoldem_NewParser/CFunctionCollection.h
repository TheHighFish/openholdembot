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
	void Add(CFunction &new_function);
	double Evaluate(CString function_name);
};

extern CFunctionCollection *p_function_collection;

#endif INC_CFUNCTIONCOLLECTION_H