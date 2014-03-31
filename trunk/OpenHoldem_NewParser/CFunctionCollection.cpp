#include "stdafx.h"
#include "CFunctionCollection.h"

CFunctionCollection *p_function_collection = NULL;
/*
CMap< CSAPrefsSubDlg *, CSAPrefsSubDlg *, UINT, UINT&  > m_dlgMap;
  E:\OH_SVN\trunk\OpenHoldem_NewParser\SAPrefsDialog.cpp(205):				m_dlgMap.SetAt(pPS->pDlg, uipTree);
  E:\OH_SVN\trunk\OpenHoldem_NewParser\SAPrefsDialog.cpp(255):		if (m_dlgMap.Lookup(pParent, dwHTree))
*/

CFunctionCollection::CFunctionCollection()
{
	_function_hashtable.InitHashTable(1027); //!!!
	Clear();
}

CFunctionCollection::~CFunctionCollection()
{}

void CFunctionCollection::Clear()
{
	_function_hashtable.RemoveAll();
}

void CFunctionCollection::Add(CFunction *new_function)
{
	CString name = new_function->name();
	_function_hashtable.SetAt(&name, new_function);
}

double CFunctionCollection::Evaluate(CString function_name)
{
	CFunction *p_function;
	_function_hashtable.Lookup(&function_name, p_function);
	return p_function->Evaluate();
}


