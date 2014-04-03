#include "stdafx.h"
#include "CFunctionCollection.h"


template<> 
UINT AFXAPI HashKey<CString*> (CString* key)
{
    return (NULL == key) ? 0 : HashKey((LPCTSTR)(*key));
}

// I don't know why, but CompareElements can't work with CString*
// have to define this
typedef CString* LPCString;

template<>
BOOL AFXAPI CompareElements<LPCString, LPCString> 
     (const LPCString* pElement1, const LPCString* pElement2)
{
    if ( *pElement1 == *pElement2 ) {
        // true even if pE1==pE2==NULL
        return true;
    } else if ( NULL != *pElement1 && NULL != *pElement2 ) {
        // both are not NULL
        return **pElement1 == **pElement2;
    } else {
        // either one is NULL
        return false;
    }
}



CFunctionCollection *p_function_collection = NULL;
/*
CMap< CSAPrefsSubDlg *, CSAPrefsSubDlg *, UINT, UINT&  > m_dlgMap;
  E:\OH_SVN\trunk\OpenHoldem_NewParser\SAPrefsDialog.cpp(205):				m_dlgMap.SetAt(pPS->pDlg, uipTree);
  E:\OH_SVN\trunk\OpenHoldem_NewParser\SAPrefsDialog.cpp(255):		if (m_dlgMap.Lookup(pParent, dwHTree))
*/

CFunctionCollection::CFunctionCollection()
{
	_function_hashtable.InitHashTable(1031); //!!!
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
	CFunction function("f$alli", "1");
	CFunction *p_function = &function;
	_function_hashtable.Lookup(&function_name, p_function);
	return function.Evaluate();
}

/*!!!
CString CDllExtension::GetDLLSpecifiedInFormula()
{
	CString formula_dll;
	int N = p_formula->formula()->mFunction.GetSize();
	formula_dll = "";
	for (int i=0; i<N; i++)
	{
		if (p_formula->formula()->mFunction[i].func == "dll")
		{
			formula_dll = p_formula->formula()->mFunction[i].func_text;
			break;
		}
	}
	formula_dll.Trim();
	return formula_dll;
}
*/


