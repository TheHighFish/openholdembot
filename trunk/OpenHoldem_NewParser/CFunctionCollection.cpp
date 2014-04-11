//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#include "stdafx.h"
#include "CFunctionCollection.h"

#include "CFunction.h"
#include "CPreferences.h"

CFunctionCollection *p_function_collection = NULL;

CFunctionCollection::CFunctionCollection()
{
	_title = "";
	_path = "";
	DeleteAll();
}

CFunctionCollection::~CFunctionCollection()
{}

void CFunctionCollection::DeleteAll()
{
	write_log(preferences.debug_parser(), 
			"[CFunctionCollection] DeleteAll()\n");
	_function_map.clear();
}

void CFunctionCollection::Add(COHScriptObject *new_function)
{
	CString name = new_function->name();
	if (name == "") {
		write_log(preferences.debug_parser(), 
			"[CFunctionCollection] Invalid empty name\n");
		return;
	}
	if (Exists(name)) {
		write_log(preferences.debug_parser(), 
			"[CFunctionCollection] Name %s already exists. Deleting it\n", name);
		_function_map.erase(name);
	}
	write_log(preferences.debug_parser(), 
		"[CFunctionCollection] Adding %s -> %i\n", name, new_function);
	_function_map[name] = new_function;
}

bool CFunctionCollection::Exists(CString name)
{
	std::map<CString, COHScriptObject*>::iterator it; 
	it = _function_map.find(name); 
	return (it != _function_map.end());
}

COHScriptObject *CFunctionCollection::LookUp(CString name)
{
	write_log(true, "[CFunctionCollection] Lookup %s\n", name); //!!
	std::map<CString, COHScriptObject*>::iterator it; 
	it = _function_map.find(name); 
	if (it == _function_map.end()) {
		// Function or list does not exist
		write_log(true, "[CFunctionCollection] Function does not exist\n");
		return NULL;
	}
	return it->second;
}

double CFunctionCollection::Evaluate(CString function_name)
{
	COHScriptObject *p_function = LookUp(function_name);
	if (p_function == NULL)
	{
		// Function does not exist
		return k_undefined_zero;
	}
	return p_function->Evaluate();
}

bool CFunctionCollection::IsList(int list_ID)
{
	CString list_name;
	list_name.Format("list%0.3d", list_ID);
	return Evaluate(list_name);
}

CString CFunctionCollection::DLLPath()
{
	COHScriptObject *dll_node = LookUp("DLL");
	if (dll_node == NULL)
	{
		return "";
	}
	CString dll_path = dll_node->function_text();
	// Remove "##DLL##"
	dll_path.Delete(0, 7);
	dll_path.Trim();
	return dll_path;
}

void CFunctionCollection::SetEmptyDefaultBot()
{
	DeleteAll();
	//!!CSLock lock(m_critsec);
	_title = "NoName";
	// Adding empty standard-functions
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=16230
	CheckForDefaultFormulaEntries();
}

void CFunctionCollection::CheckForDefaultFormulaEntries()
{
	//!!CSLock lock(m_critsec);

	// Header comment
	AddEmptyFunctionIfFunctionDoesNotExist(CString("notes"));
	// DLL to be loaded
	AddEmptyFunctionIfFunctionDoesNotExist(CString("dll"));
	// Autoplayer, standard, ini and PrWin functions
	for (int i=0; i<k_number_of_standard_functions; ++i)
	{
		AddEmptyFunctionIfFunctionDoesNotExist(CString(k_standard_function_names[i]));
	}
	// Debug functions	
	AddEmptyFunctionIfFunctionDoesNotExist(CString("f$test"));
	AddEmptyFunctionIfFunctionDoesNotExist(CString("f$debug"));
}

void CFunctionCollection::AddEmptyFunctionIfFunctionDoesNotExist(CString &function_name)
{
	if (Exists(function_name))
	{
		return;
	}
	// Formula not found.
	// Add the standard one.
	CString function_text;
	if ((function_name.Compare(k_standard_function_names[k_autoplayer_function_check]) == k_CString_identical)
		|| (function_name.Compare(k_standard_function_names[k_autoplayer_function_fold]) == k_CString_identical))
	{
		// f$check and f$fold should evaluate to true per default
		// for auto-check-folding instead of time-outs.
		function_text = "1 "; //!! header?
	}
	else
	{
		// Add an empty function.
		// The function-text should contain at least one space.
		// The editor does somehow not work for completely empty formulas.
		function_text = " "; //!! header?
	}
	CFunction *p_function = new CFunction(&function_name, 
		&function_text); // ##name##?
	Add((COHScriptObject *)p_function); //!!
}

void CFunctionCollection::Save(CArchive &ar)
{
	//function_hashtable.
	//!!! old code
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
