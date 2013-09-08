#include "StdAfx.h"
#include "CFormula.h"

#include "..\CCritSec\CCritSec.h"
#include "..\CTransform\CTransform.h"
#include "CGrammar.h"
#include "CPreferences.h"
#include "CSymbols.h"
#include "OH_MessageBox.h"
#include "OpenHoldemDoc.h"
#include "UPDialog.h"

CFormula			*p_formula = NULL;

CFormula::CFormula(void)
{
	ClearFormula();
}

CFormula::~CFormula(void)
{
}

void CFormula::ClearFormula()
{
	CSLock lock(m_critsec);

	_formula.mHandList.RemoveAll();
	_formula.mFunction.RemoveAll();
	_formula_name = "";
	_is_parsing = false;
}

void CFormula::SetEmptyDefaultBot()
{
	SFunction		func;

	ClearFormula();

	CSLock lock(m_critsec);
	func.dirty = true;
	_formula_name = "NoName";
	// Adding empty standard-functions
	// http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=16230
	CheckForDefaultFormulaEntries();
}

const char *CFormula::GetFunctionText(const char *name)
{
	for (int i=0; i<formula()->mFunction.GetSize(); i++)
	{
		if (!formula()->mFunction[i].func.Compare(name))
			return formula()->mFunction[i].func_text.GetString();
	}
	return "";
}

// Reading a formula
void CFormula::ReadFormulaFile(CArchive& ar, bool ignoreFirstLine)
{
	CString		strOneLine = ""; 
	int			content = 0;
	char		funcname[k_max_size_of_function_name] = {0};
	int			start = 0, end = 0;
		
	SFunction	func;	
	SHandList	list;		

	CSLock lock(m_critsec);

	// Ignore first line (date/time)
	if (ignoreFirstLine)
		ar.ReadString(strOneLine);

	// read data in, one line at a time
	strOneLine = "";
	content = FTnone;
	func.func = "";
	while(ar.ReadString(strOneLine)) 
	{ 
		// If this line marks the beginning of a function, then save the previously
		// collected function, and start a new one
		if (strOneLine.Mid(0,2)=="##") 
		{
			// Save the previously collected function
			if (content == FTlist) 
			{
				// Strip the LFCR off the last line (we need to add CRLF for all but the last line)
				list.list_text.TrimRight("\r\n");
				if (DoesFormulaAlreadyExist(list.list))
				{
					CString ErrorMessage = "Handlist does already exist: " + list.list;
					OH_MessageBox_Error_Warning(ErrorMessage, "Error");
				}
				else
				{
					_formula.mHandList.Add(list); 
				}
			}
			else if (content == FTfunc) 
			{
				func.func_text.TrimRight("\r\n");
				if (DoesFormulaAlreadyExist(func.func))
				{
					CString ErrorMessage = "Function does already exist: " + func.func;
					OH_MessageBox_Error_Warning(ErrorMessage, "Error");
				}
				else
				{
					_formula.mFunction.Add(func);
				}
			}
			// Get the function name				
			start = strOneLine.Find("##",0);

			// No need to check the result of start,
			// as this code gets executed only,
			// if a line starts with "##"	
			end = strOneLine.Find("##", start+2);

			// Checking for malformed function header
			// without trailing "##"
			if (end == k_undefined) 
			{
				// Trying to continue gracefully.				
				// Skipping is not possible,
				// as this crashes the formula editor.											
				int number_of_chars_to_copy = (strOneLine.GetLength() < k_max_size_of_function_name) ?
					strOneLine.GetLength() : k_max_size_of_function_name;
				strncpy_s(funcname, 
					k_max_size_of_function_name, 
					strOneLine.GetString()+start+2,
					number_of_chars_to_copy);
				funcname[number_of_chars_to_copy]='\0';
				
				CString the_ErrorMessage = "Malformed function header!\nMissing trailing '##'.\n" 
					+ strOneLine + "\n"
					+ "Trying to continue...";
				OH_MessageBox_Error_Warning(the_ErrorMessage, "Syntax Error");
			}

			else 
			{
				int size_of_function_name = end - start + 1;    
				assert(size_of_function_name < k_max_size_of_function_name);
				// strncpy_s: http://msdn.microsoft.com/de-de/library/5dae5d43(v=vs.80).aspx
				strncpy_s(funcname,                              // Destination
					k_max_size_of_function_name * sizeof(char),  // Size of destination
					strOneLine.GetString() + start + 2,          // Start of source (without leading "##")
					size_of_function_name);						 // Elements to copy	
				funcname[end-2] = '\0';							 // Remove trailing "##"	
			}

			if (memcmp(funcname, "list", 4) == 0) 
			{ 
				content = FTlist;
				list.list = funcname;
				list.list_text = "";
			}

			else 
			{
				content = FTfunc;
				func.func = funcname;
				func.func_text = "";
				func.dirty = true;
			}
		}

		// Get the function content
		else 
		{
			switch (content) 
			{
				 case FTlist:
					 list.list_text.Append(strOneLine); list.list_text.Append("\r\n");
					 break;
				 case FTfunc:
					 func.func_text.Append(strOneLine); func.func_text.Append("\r\n");
					 break;
			}
		}	
	}  

	// Add the last list/function to the CArray on EOF, if it was a list being processed
	if (content == FTlist) 
	{
		list.list_text.TrimRight("\r\n");
		if (DoesFormulaAlreadyExist(list.list))
		{
			CString ErrorMessage = "Handlist does already exist: " + list.list;
			OH_MessageBox_Error_Warning(ErrorMessage, "Error");
		}
		else
		{
			_formula.mHandList.Add(list); 
		}
	}
	else if (content == FTfunc) 
	{
		func.func_text.TrimRight("\r\n");
		if (DoesFormulaAlreadyExist(func.func))
		{
			CString ErrorMessage = "Function does already exist: " + func.func;
			OH_MessageBox_Error_Warning(ErrorMessage, "Error");
		}
		else
		{
			_formula.mFunction.Add(func);
		}
	}
}

bool CFormula::IsStandardFormula(CString formula_name)
{
	return ((formula_name == "notes")	    || (formula_name == "dll")
		|| (formula_name == "f$alli")	    || (formula_name == "f$betsize")
		|| (formula_name == "f$betpot_2_1") || (formula_name == "f$betpot_1_1")
		|| (formula_name == "f$betpot_3_4") || (formula_name == "f$betpot_2_3")
		|| (formula_name == "f$betpot_1_2") || (formula_name == "f$betpot_1_3")
		|| (formula_name == "f$betpot_1_4")
		|| (formula_name == "f$rais")	    || (formula_name == "f$call")  
		|| (formula_name == "f$prefold")    || (formula_name == "f$rebuy") 
		|| (formula_name == "f$delay")	    || (formula_name == "f$chat")	
		|| (formula_name == "f$prwin_number_of_opponents")		
		|| (formula_name == "f$prwin_number_of_iterations")		
		|| (formula_name == "f$sitin")	    || (formula_name == "f$sitout")	
		|| (formula_name == "f$leave")	    || (formula_name == "f$close")	
		|| (formula_name == "f$test")	    || (formula_name == "f$debug"));
}

void CFormula::WriteStandardFunction(CArchive& ar, CString name)
{
	int	number_of_functions = (int) _formula.mFunction.GetSize();

	for (int i=0; i<number_of_functions; i++) 
		if (_formula.mFunction[i].func == name) 
			ar.WriteString("##" + name + "##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
}

void CFormula::WriteFormula(CArchive& ar) 
{
	CString		s = "";
	char		nowtime[26] = {0};

	//  First write the standard formula functions...
	//  These are functions and symbols, that
	//	* are essential to control the behaviour 
	//	  of (nearly) every poker bot.
	//	* configure some very important constants.
	//  
	s.Format("##%s##\r\n\r\n", get_time(nowtime)); ar.WriteString(s);

	// !!! missed f$init and needs refactoring
	WriteStandardFunction(ar, "notes");
	WriteStandardFunction(ar, "dll");
	// ToDo: Check, if that can be done the normal way too?
	WriteStandardFunction(ar, "f$prwin_number_of_iterations");
	WriteStandardFunction(ar, "f$alli");
	WriteStandardFunction(ar, "f$betsize");
	WriteStandardFunction(ar, "f$betpot_2_1");
	WriteStandardFunction(ar, "f$betpot_1_1");
	WriteStandardFunction(ar, "f$betpot_3_4");
	WriteStandardFunction(ar, "f$betpot_2_3");
	WriteStandardFunction(ar, "f$betpot_1_2");
	WriteStandardFunction(ar, "f$betpot_1_3");
	WriteStandardFunction(ar, "f$betpot_1_4");
	WriteStandardFunction(ar, "f$rais");
	WriteStandardFunction(ar, "f$call");
	WriteStandardFunction(ar, "f$prefold");
	WriteStandardFunction(ar, "f$rebuy");
	WriteStandardFunction(ar, "f$delay");
	WriteStandardFunction(ar, "f$chat");
	WriteStandardFunction(ar, "f$prwin_number_of_opponents"); 
	WriteStandardFunction(ar, "f$sitin");
	WriteStandardFunction(ar, "f$sitout");
	WriteStandardFunction(ar, "f$leave");
	WriteStandardFunction(ar, "f$close");
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
	}
}

void CFormula::CreateHandListMatrices()
{
	CString		token = "";

	CSLock lock(m_critsec);

	for (int listnum=0; listnum<MAX_HAND_LISTS; listnum++)
		for (int i=0; i<=12; i++)
			for (int j=0; j<=12; j++)
				_formula.inlist[listnum][i][j] = false;

	for (int i=0; i<(int) _formula.mHandList.GetSize(); i++)
	{
		int listnum = atoi(_formula.mHandList[i].list.Mid(4).GetString());
		if (listnum>=MAX_HAND_LISTS)			
		{
			OH_MessageBox_Error_Warning("List number too high.", "Error");
			break;
		}
		ParseHandList(_formula.mHandList[i].list_text, _formula.inlist[listnum]);
	}
}

bool CFormula::ParseAllFormula(HWND hwnd)
{
	// returns true for successful parse of all trees, false otherwise
	sData			data;

	data.all_parsed = true;
	data.calling_hwnd = hwnd;
	data.pParent = this;

	_is_parsing = true;

	CUPDialog dlg_progress(hwnd, ParseLoop, &data, "Please wait", false);
	dlg_progress.DoModal();

	WarnAboutOutdatedConcepts();

	_is_parsing = false;

	return data.all_parsed;
}

void CFormula::AddEmptyFunctionIfFunctionDoesNotExist(const CString &FunctionName)
{
	int size_of_formula_set = _formula.mFunction.GetSize();
	for (int i=0; i<size_of_formula_set; i++)  
	{
		if (!_formula.mFunction[i].func.Compare(FunctionName)) 
		{
			// Formula found. No need to create it.
			return;
		}
	}
	// Formula not found.
	// Add the standard one.
	SFunction func;
	if ((FunctionName.Compare(k_standard_function_names[k_autoplayer_function_check]) == k_CString_identical)
		|| (FunctionName.Compare(k_standard_function_names[k_autoplayer_function_fold]) == k_CString_identical))
	{
		// f$check and f$fold should evaluate to true per default
		// for auto-check-folding instead of time-outs.
		func.func_text = "1 ";
	}
	else
	{
		// Add an empty function.
		// The function-text should contain at least one space.
		// The editor does somehow not work for completely empty formulas.
		func.func_text = " "; 
	}
	func.dirty = true; 
	func.func = FunctionName; 
	_formula.mFunction.Add(func); 
}

void CFormula::CheckForDefaultFormulaEntries()
{
	CSLock lock(m_critsec);

	// Header comment
	AddEmptyFunctionIfFunctionDoesNotExist("notes");
	// DLL to be loaded
	AddEmptyFunctionIfFunctionDoesNotExist("dll");
	// Autoplayer, standard, ini and PrWin functions
	for (int i=0; i<k_number_of_standard_functions; ++i)
	{
		AddEmptyFunctionIfFunctionDoesNotExist(k_standard_function_names[i]);
	}
	// Debug functions	
	AddEmptyFunctionIfFunctionDoesNotExist("f$test");
	AddEmptyFunctionIfFunctionDoesNotExist("f$debug");
}

void CFormula::MarkCacheStale()
{
	CSLock lock(m_critsec);

	for (int i=0; i<_formula.mFunction.GetSize(); i++)
        _formula.mFunction[i].fresh = false;
}

void CFormula::ParseHandList(const CString &list_text, bool inlist[13][13])
{
	CSLock lock(m_critsec);

	for (int i=0; i<=12; i++)
		for (int j=0; j<=12; j++)
			inlist[i][j] = false;

	int	token_card0_rank = 0, token_card1_rank = 0, temp_rank = 0;

	CString list = list_text;
	list.MakeUpper();

	const char *pStr = list.GetString();

	while (*pStr)
	{
		if (pStr[0] == '/' && pStr[1] == '/')
		{
			int index = CString(pStr).FindOneOf("\r\n");
			if (index == k_undefined) break;
			pStr += index;
		}

		token_card0_rank = CardIdentHelper(*pStr++);

		if (token_card0_rank == k_undefined)
			continue;

		token_card1_rank = CardIdentHelper(*pStr++);

		if (token_card1_rank == k_undefined)
			continue;

		// make card0 have the higher rank
		if (token_card0_rank < token_card1_rank)
		{
			temp_rank = token_card0_rank;
			token_card0_rank = token_card1_rank;
			token_card1_rank = temp_rank;
		}

		if (*pStr == 'S') // suited
		{
			inlist[token_card0_rank][token_card1_rank] = true;
			pStr++;
		}
		else  // offsuit or pair
		{
			inlist[token_card1_rank][token_card0_rank] = true;
		}
	}
}

void CFormula::CopyFormulaFrom(CFormula *f)
{
	SHandList		list;
	SFunction		func;
	int				from_count = 0, to_count = 0;
	bool			addit = false, deleteit = false;

	// Init locals
	list.list = "";
	list.list_text = "";
	func.cache = 0.;
	func.dirty = false;
	func.fresh = false;
	func.func = "";
	func.func_text = "";

	CSLock lock(m_critsec);

	// handle deleted udfs
	to_count = (int) _formula.mFunction.GetSize();
	for (int to_iter=0; to_iter<to_count; to_iter++)
	{
		from_count = (int) f->formula()->mFunction.GetSize();
		deleteit = true;
		for (int from_iter=0; from_iter<from_count; from_iter++)
		{
			if (_formula.mFunction[to_iter].func == f->formula()->mFunction[from_iter].func)
			{
				deleteit = false;
				from_iter = from_count+1;
			}
		}
		if (deleteit)
		{
			_formula.mFunction.RemoveAt(to_iter, 1);
			to_count = (int) _formula.mFunction.GetSize();
			to_iter-=1;
		}
	}

	// handle new/changed udfs
	from_count = (int) f->formula()->mFunction.GetSize();
	for (int from_iter=0; from_iter<from_count; from_iter++)
	{
		to_count = (int) _formula.mFunction.GetSize();
		addit = true;
		for (int to_iter=0; to_iter<to_count; to_iter++)
		{
			if (_formula.mFunction[to_iter].func == f->formula()->mFunction[from_iter].func)
			{
				// changed?
				addit = false;
				if (_formula.mFunction[to_iter].func_text == f->formula()->mFunction[from_iter].func_text)
				{
					// no change
					_formula.mFunction[to_iter].dirty = false;
					_formula.mFunction[to_iter].fresh = false;
				}
				else
				{
					// yup, it changed
					_formula.mFunction[to_iter].func_text = f->formula()->mFunction[from_iter].func_text;
					_formula.mFunction[to_iter].dirty = true;
					_formula.mFunction[to_iter].fresh = false;
					_formula.mFunction[to_iter].cache = 0.0;
				}
				to_iter = to_count+1;
			}
		}

		// new
		if (addit)
		{
			func.func = f->formula()->mFunction[from_iter].func;
			func.func_text = f->formula()->mFunction[from_iter].func_text;
			func.dirty = true;
			func.fresh = false;
			func.cache = 0.0;
			_formula.mFunction.Add(func);
		}
	}

	// Copy hand lists
	_formula.mHandList.RemoveAll();
	from_count = (int) f->formula()->mHandList.GetSize();
	for (int from_iter=0; from_iter<from_count; from_iter++)
	{
		list.list = f->formula()->mHandList[from_iter].list;
		list.list_text = f->formula()->mHandList[from_iter].list_text;
		if (DoesFormulaAlreadyExist(list.list))
		{
			CString ErrorMessage = "Handlist does already exist: " + list.list;
			OH_MessageBox_Error_Warning(ErrorMessage, "Error");
		}
		else
		{
			_formula.mHandList.Add(list); 
		}
	}

	// Copy name
	_formula_name = f->formula_name();
}

const int CFormula::CardIdentHelper(const char c)
{
	if (c>='2' && c<='9')
		return c - '0' - 2;

	else if (c=='T')
		return 8;

	else if (c=='J')
		return 9;

	else if (c=='Q')
		return 10;

	else if (c=='K')
		return 11;

	else if (c=='A')
		return 12;

	return -1;
}

bool CFormula::ParseLoop(const CUPDUPDATA* pCUPDUPData)
{
	int				i = 0, j = 0, N = 0;
	CString			s = "";
	bool			result = false;
	int				stopchar = 0;
	int				c = 0, linenum = 0, colnum = 0;
	LARGE_INTEGER	bcount = {0}, ecount = {0}, lFrequency = {0};
	double			time_elapsed = 0.;
	sData			*data = (sData*) (pCUPDUPData->GetAppData());
	CGrammar		gram;
	bool            gui_enable_progress_dialog = !preferences.gui_disable_progress_dialog();

	pCUPDUPData->SetProgress("", 0, false);

	// init timer
	QueryPerformanceCounter(&bcount);
	QueryPerformanceFrequency(&lFrequency);

	N = (int) data->pParent->formula()->mFunction.GetSize();
	for (int i=0; i<N; i++)
	{

		// Update progress dialog
		s.Format("Parsing formula set %s : %.0f%%", data->pParent->formula()->mFunction[i].func.GetString(), (double) i / (double) N * 100.0);
		QueryPerformanceCounter(&ecount);
		time_elapsed = ((double) (ecount.LowPart - bcount.LowPart))/((double) lFrequency.LowPart);
		pCUPDUPData->SetProgress(s.GetString(), (int) ((double) i / (double) N * 100.0), ((time_elapsed>=3.0) && gui_enable_progress_dialog));

		// Parse it if it is dirty, and not notes, dll or f$debug
		if (data->pParent->formula()->mFunction[i].dirty == true &&
				data->pParent->formula()->mFunction[i].func != "notes" &&
				data->pParent->formula()->mFunction[i].func != "dll" &&
				data->pParent->formula()->mFunction[i].func != "f$debug")
		{
			result = gram.ParseString(
							&data->pParent->formula()->mFunction[i].func_text, 
							data->pParent->formula(), 
							data->pParent->set_tpi(i), 
							&stopchar);

			if (!result)
			{
				linenum = colnum = 1;
				for (c=0; c<stopchar; c++)
				{
					if (data->pParent->formula()->mFunction[i].func_text.Mid(c, 1)=="\n")
					{
						linenum++;
						colnum = 1;
					}
					else
					{
						colnum++;
					}
				}
				s.Format("Error in parse of %s\nLine: %d  Character: %d\n\nNear:\n \"%s\"",
					 data->pParent->formula()->mFunction[i].func.GetString(),
					 linenum, colnum,
					 data->pParent->formula()->mFunction[i].func_text.Mid(stopchar, 40).GetString());
				OH_MessageBox_Error_Warning(s, "PARSE ERROR");

				data->all_parsed = false;
			}

			else if (gram.parse_symbol_stop_strs()->GetSize() != 0)
			{
				s.Format("Error in parse of %s\n\nInvalid symbols:\n",
					 data->pParent->formula()->mFunction[i].func.GetString());
				for (int j=0; j<gram.parse_symbol_stop_strs()->GetSize(); j++)
				{
					s.Append("   ");
					s.Append(gram.parse_symbol_stop_strs()->GetAt(j).c_str());
					s.Append("\n");
				}
				OH_MessageBox_Error_Warning(s, "PARSE ERROR");

				data->all_parsed = false;
			}

			else
			{
				data->pParent->set_func_dirty(i, false);
			}
		}
	}
	pCUPDUPData->SetProgress("", 100, gui_enable_progress_dialog);

	return true;
}

bool CFormula::DoesFormulaAlreadyExist(const CString new_name)
{
	int number_of_formulae = _formula.mFunction.GetCount(); 
	write_log(preferences.debug_alltherest(), "CFormula::DoesFormulaAlreadyExist(): number_of_formulae = [%d]\n", 
		number_of_formulae);
	for (int i=0; i<number_of_formulae; i++)
	{
		write_log(preferences.debug_alltherest(), "CFormula::DoesFormulaAlreadyExist(): [%s] == [%s]\n",
			new_name, _formula.mFunction[i].func);
		if (new_name == _formula.mFunction[i].func)
		{
			return true;
		}
	}
	return false;
}

bool CFormula::DoesHandlistAlreadyExist(const CString new_name)
{
	int number_of_handlists = _formula.mHandList.GetCount(); 
	for (int i=0; i<number_of_handlists; i++)
	{
		write_log(preferences.debug_alltherest(), "CFormula::DoesHandlistAlreadyExist(): [%s] == [%s]\n",
			new_name, _formula.mHandList[i].list);
		if (new_name == _formula.mHandList[i].list)
		{
			return true;
		}
	}
	return false;
}

void CFormula::WarnAboutOutdatedConcepts()
{
	if (DoesFormulaAlreadyExist("f$play"))
	{
		OH_MessageBox_Error_Warning("Your formula set contains a function f$play.\n"
			"The play-formula is an outdated messy concept,\n"
			"that got abolished in OpenHoldem 4.0.0.\n"
			"It got replaced by three separate functions:\n"
			"  * f$sitin\n"
			"  * f$sitout\n"
			"  * f$leave\n"
			"Please refer to the manual about how to adapt\n"
			"your formula set.",
			"Error");
	}
	if (DoesFormulaAlreadyExist("f$swag") 
		|| DoesFormulaAlreadyExist("f$srai")
		|| DoesFormulaAlreadyExist("f$swag_adjust"))
	{
		OH_MessageBox_Error_Warning("Your formula set contains old \"swag\"-functions.\n"
			"\"Swagging\" is an outdated messy concept,\n"
			"where the user had to care about everything.\n\n"
			"f$swag and its supporting functions and symbols\n"
			"got finally replaced by a single function f$betsize\n"
			"and OpenHoldem cares about all the rest automatically\n"
			"(as long as you define the s$betsizemethod correctly).\n\n"
			"Please refer to the manual about how to adapt\n"
			"your formula set.",
			"Error");
	}
	if (DoesFormulaAlreadyExist("f$P"))
	{
		OH_MessageBox_Error_Warning("Your formula set contains old \"prwin\"-functions.\n"
			"\"f$P\" got replaced by \"f$prwin_number_of_opponents\".",
			"Error");
	}

}
