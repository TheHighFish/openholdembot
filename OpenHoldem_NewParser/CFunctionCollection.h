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
	void SetTitle(CString title)	{ _title = title; }
public:
	CString Title()					{ return _title; }
	CString FormulaPath();
	CString DLLPath()				{ return ""; } //!!!
private:
	CMap<CString*, CString*, CFunction*, CFunction*> _function_hashtable;
	CString _title;
};

extern CFunctionCollection *p_function_collection;

#endif INC_CFUNCTIONCOLLECTION_H