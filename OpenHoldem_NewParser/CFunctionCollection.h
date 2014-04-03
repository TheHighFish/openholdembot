#ifndef INC_CFUNCTIONCOLLECTION_H
#define INC_CFUNCTIONCOLLECTION_H

#include "CFunction.h"

class CFunctionCollection
{
	friend class CFormulaParser;
public:
	CFunctionCollection();
	~CFunctionCollection();
public:
	void Clear();
	void Add(CFunction *new_function);
	double Evaluate(CString function_name);
	void Save(CArchive &ar);
protected:
	void SetTitle(CString title)	{ _title = title; }
	void SetPath(CString path)		{ _path = path; }
public:
	CString Title()					{ return _title; }
	CString FormulaPath();
	CString DLLPath()				{ return _path; } 
private:
	CMap<CString*, CString*, CFunction*, CFunction*> _function_hashtable;
	CString _title;
	CString _path;
};

extern CFunctionCollection *p_function_collection;

#endif INC_CFUNCTIONCOLLECTION_H