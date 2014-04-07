#include "stdafx.h"
#include "COHScriptList.h"

COHScriptList::COHScriptList(CString *new_name, CString *new_function_text)
{
	COHScriptObject(new_name, new_function_text);
}

COHScriptList::~COHScriptList()
{}

void COHScriptList::Set(CString list_member)
{

}

double COHScriptList::Evaluate()
{
	return false;
}