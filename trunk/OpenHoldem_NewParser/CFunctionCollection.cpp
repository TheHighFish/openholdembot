#include "stdafx.h"
#include "CFunctionCollection.h"

CFunctionCollection *p_function_collection = NULL;

CFunctionCollection::CFunctionCollection()
{
	Clear();
}

CFunctionCollection::~CFunctionCollection()
{}

void CFunctionCollection::Clear()
{}

void CFunctionCollection::Add(CFunction &new_function)
{}

double CFunctionCollection::Evaluate(CString function_name)
{
	return 0.0;
}


