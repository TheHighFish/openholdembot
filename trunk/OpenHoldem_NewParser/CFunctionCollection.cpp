#include "stdafx.h"
#include "CFunctionCollection.h"

// CString hashtable below by Sam NG
// http://www.codeproject.com/Articles/13458/CMap-How-to
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

// End of CString hashtable below by Sam NG

CFunctionCollection *p_function_collection = NULL;

CFunctionCollection::CFunctionCollection()
{
	// Hash-table-size must be 
	// * a prime number
	// * if possible at least 20% larger than the number of entries
	//   (OpenPPL-library 4.4.0 contains 710 functions)
	_function_hashtable.InitHashTable(1031);	
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

void CFunctionCollection::Save(CArchive &ar)
{
	//!! old code
	/*CString		s = "";
	char		nowtime[26] = {0};

	//  First write the standard formula functions...
	//  These are functions and symbols, that
	//	* are essential to control the behaviour 
	//	  of (nearly) every poker bot.
	//	* configure some very important constants.
	//  
	s.Format("##%s##\r\n\r\n", get_time(nowtime)); ar.WriteString(s);

	// DLL  and notes are a bit special "functions",
	// so they get extra treatment.
	WriteStandardFunction(ar, "notes");
	WriteStandardFunction(ar, "dll");
	for (int i=k_autoplayer_function_allin; i<k_number_of_standard_functions; i++)
	{
		WriteStandardFunction(ar, k_standard_function_names[i]);
	}
	// f$test and f$debug are again special
	WriteStandardFunction(ar, "f$test");
	WriteStandardFunction(ar, "f$debug");

	// handlists for both ohf and old whf style
	for (int i=0; i<(int) _formula.mHandList.GetSize(); i++) 
		ar.WriteString("##" + _formula.mHandList[i].list + "##\r\n" + _formula.mHandList[i].list_text + "\r\n\r\n");

	// User defined functions for new ohf style only.
	// We don't ever have to save them, as for a conversion
	// we only have to generate an ohf file and (for technical reasons)
	// recreate the old whf (which is already open for storing).
	//
	for (int i=0; i<(int) _formula.mFunction.GetSize(); i++) 
	{
		if (!IsStandardFormula(_formula.mFunction[i].func))
		{
			ar.WriteString("##" + _formula.mFunction[i].func + "##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
		}
	}*/
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


