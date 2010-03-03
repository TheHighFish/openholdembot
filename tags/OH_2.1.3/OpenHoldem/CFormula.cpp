#include "StdAfx.h"

#include "..\CCritSec\CCritSec.h"
#include "CFormula.h"
#include "CPreferences.h"
#include "CSymbols.h"
#include "..\CTransform\CTransform.h"
#include "CGrammar.h"
#include "OpenHoldemDoc.h"
#include "UPDialog.h"


CFormula			*p_formula = NULL;

CFormula::CFormula(void)
{
	ClearFormula();
	_formula_name = "";
}

CFormula::~CFormula(void)
{
}

void CFormula::ClearFormula()
{
	CSLock lock(m_critsec);

	_formula.dBankroll = _formula.dDefcon = _formula.dRake = _formula.dNit = 0.0;
	_formula.mHandList.RemoveAll();
	_formula.mFunction.RemoveAll();

	_formula_name = "";
}

void CFormula::SetDefaultBot()
{
	SFunction		func;

	ClearFormula();

	CSLock lock(m_critsec);

	func.dirty = true;

	func.func = "notes"; func.func_text = defaultCSnotes; _formula.mFunction.Add(func);
	func.func = "dll"; func.func_text = ""; _formula.mFunction.Add(func);
	func.func = "f$alli"; func.func_text = defaultCSalli; _formula.mFunction.Add(func);
	func.func = "f$swag"; func.func_text = defaultCSswag; _formula.mFunction.Add(func);
	func.func = "f$srai"; func.func_text = defaultCSsrai; _formula.mFunction.Add(func);
	func.func = "f$rais"; func.func_text = defaultCSrais; _formula.mFunction.Add(func);
	func.func = "f$call"; func.func_text = defaultCScall; _formula.mFunction.Add(func);
	func.func = "f$prefold"; func.func_text = defaultCSprefold; _formula.mFunction.Add(func);
	func.func = "f$rebuy"; func.func_text = defaultCSrebuy; _formula.mFunction.Add(func);
	func.func = "f$delay"; func.func_text = defaultCSdelay; _formula.mFunction.Add(func);
	func.func = "f$chat"; func.func_text = defaultCSchat; _formula.mFunction.Add(func);
	func.func = "f$P"; func.func_text = defaultCSP; _formula.mFunction.Add(func);
	func.func = "f$play"; func.func_text = defaultCSplay; _formula.mFunction.Add(func);
	func.func = "f$test"; func.func_text = defaultCStest; _formula.mFunction.Add(func);
	func.func = "f$debug"; func.func_text = defaultCSdebug; _formula.mFunction.Add(func);
	_formula.dBankroll = defaultdBankroll;
	_formula.dDefcon = defaultdDefcon;
	_formula.dRake = defaultdRake;
	_formula.dNit = defaultdNit;

	// Create UDFs
	func.func = "f$evrais"; func.func_text = defaultCSevrais; _formula.mFunction.Add(func);
	func.func = "f$evcall"; func.func_text = defaultCSevcall; _formula.mFunction.Add(func);	

	_formula_name = "Default";
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

// Reading a part of a formula, which may be spread
// between two files in case of an old style whf / whx formula.
void CFormula::ReadFormulaFile(CArchive& ar, bool ignoreFirstLine, bool disable_msgbox)
{
	CString		strOneLine = ""; 
	int			content = 0;
	char		funcname[256] = {0};
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
					MessageBox(0, ErrorMessage, "Error", 0);
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
					MessageBox(0, ErrorMessage, "Error", 0);
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
			if (end == -1) 
			{
				// Trying to continue gracefully.				
				// Skipping is not possible,
				// as this crashes the formula editor.											
				strcpy_s(funcname, 256, strOneLine.GetString()+start+2);
				funcname[strOneLine.GetLength()]='\0';
				
				if (!disable_msgbox)
				{
					CString the_ErrorMessage = "Malformed function header!\nMissing trailing '##'.\n" 
						+ strOneLine + "\n"
						+ "Trying to continue...";
					MessageBox(0, the_ErrorMessage, "Syntax Error", MB_OK | MB_ICONEXCLAMATION);
				}
			}

			else 
			{
				strcpy_s(funcname, 256, strOneLine.GetString()+start+2);
				funcname[end-2]='\0';
			}

			if (strcmp(funcname, "bankroll") == 0) { _formula.dBankroll = 0.0; content = FTbankroll; }
			
			else if (strcmp(funcname, "defcon") == 0) { _formula.dDefcon = 0.0; content = FTdefcon; }
			
			else if (strcmp(funcname, "rake") == 0) { _formula.dRake = 0.0; content = FTrake; }
			
			else if (strcmp(funcname, "nit") == 0) { _formula.dNit = 0.0; content = FTnit; }
			
			else if (memcmp(funcname, "list", 4) == 0) 
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
				 case FTbankroll:
					 if (strOneLine.GetLength())
						 _formula.dBankroll = atof(strOneLine.GetString());
					 break;
				 case FTdefcon:
					 if (strOneLine.GetLength())
						 _formula.dDefcon = atof(strOneLine.GetString());
					 break;
				 case FTrake:
					 if (strOneLine.GetLength())
						 _formula.dRake = atof(strOneLine.GetString());
					 break;
				 case FTnit:
					 if (strOneLine.GetLength())
						 _formula.dNit = atof(strOneLine.GetString());
					 break;
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
			MessageBox(0, ErrorMessage, "Error", 0);
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
			MessageBox(0, ErrorMessage, "Error", 0);
		}
		else
		{
			_formula.mFunction.Add(func);
		}
	}
}

void CFormula::WriteFormula(CArchive& ar, bool use_new_OHF_style) 
{
	CString		s = "";
	int			i = 0, N = (int) _formula.mFunction.GetSize();
	char		nowtime[26] = {0};

	//  First write the standard formula functions...
	//  These are functions and symbols, that
	//	* are essential to control the behaviour 
	//	  of (nearly) every poker bot.
	//	* configure some very important constants.
	//  
	s.Format("##%s##\r\n\r\n", get_time(nowtime)); ar.WriteString(s);

	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "notes") 
			ar.WriteString("##notes##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n"); 

	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "dll")
			ar.WriteString("##dll##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");

	s.Format("##bankroll##\r\n%f\r\n\r\n", _formula.dBankroll); ar.WriteString(s);
	
	s.Format("##defcon##\r\n%f\r\n\r\n", _formula.dDefcon); ar.WriteString(s);
	
	s.Format("##rake##\r\n%f\r\n\r\n", _formula.dRake); ar.WriteString(s);
	
	s.Format("##nit##\r\n%d\r\n\r\n", (int) _formula.dNit); ar.WriteString(s);
	
	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "f$alli")
			ar.WriteString("##f$alli##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	
	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "f$swag") 
			ar.WriteString("##f$swag##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	
	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "f$srai") 
			ar.WriteString("##f$srai##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	
	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "f$rais") 
			ar.WriteString("##f$rais##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	
	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "f$call") 
			ar.WriteString("##f$call##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	
	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "f$prefold") 
			ar.WriteString("##f$prefold##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");

	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "f$rebuy") 
			ar.WriteString("##f$rebuy##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	
	// New standard formulas are
	//   * f$delay
	//   * f$chat
	//
	// Old standard formulas are:
	//   * f$evrais 
	//   * f§evcall
	//
	if (use_new_OHF_style)
	{
		for (i=0; i<N; i++) 
			if (_formula.mFunction[i].func == "f$delay") 
				ar.WriteString("##f$delay##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	
		for (i=0; i<N; i++) 
			if (_formula.mFunction[i].func == "f$chat") 
				ar.WriteString("##f$chat##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	}
	else
	{
		for (i=0; i<N; i++) 
			if (_formula.mFunction[i].func == "f$evrais") 
				ar.WriteString("##f$evrais##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	
		for (i=0; i<N; i++) 
			if (_formula.mFunction[i].func == "f$evcall") 
				ar.WriteString("##f$evcall##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	}

	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "f$P") 
			ar.WriteString("##f$P##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	
	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "f$play") 
			ar.WriteString("##f$play##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	
	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "f$test") 
			ar.WriteString("##f$test##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
	
	for (i=0; i<N; i++) 
		if (_formula.mFunction[i].func == "f$debug") 
			ar.WriteString("##f$debug##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");

	// handlists for both ohf and old whf style
	for (i=0; i<(int) _formula.mHandList.GetSize(); i++) 
		ar.WriteString("##" + _formula.mHandList[i].list + "##\r\n" + _formula.mHandList[i].list_text + "\r\n\r\n");

	// User defined functions for new ohf style only.
	// We don't ever have to save them, as for a conversion
	// we only have to generate an ohf file and (for technical reasons)
	// recreate the old whf (which is already open for storing).
	//
	if (use_new_OHF_style)
	{
		for (i=0; i<(int) _formula.mFunction.GetSize(); i++) 
		{
			if (_formula.mFunction[i].func != "notes" &&
				_formula.mFunction[i].func != "dll" &&
				_formula.mFunction[i].func != "f$alli" &&
				_formula.mFunction[i].func != "f$swag" &&
				_formula.mFunction[i].func != "f$srai" &&
				_formula.mFunction[i].func != "f$rais" &&
				_formula.mFunction[i].func != "f$call" &&
				_formula.mFunction[i].func != "f$prefold" &&
				_formula.mFunction[i].func != "f$rebuy" &&
				_formula.mFunction[i].func != "f$delay" &&
				_formula.mFunction[i].func != "f$chat" &&
				_formula.mFunction[i].func != "f$P" &&
				_formula.mFunction[i].func != "f$play" &&
				_formula.mFunction[i].func != "f$test" &&
				_formula.mFunction[i].func != "f$debug" ) 
			{
				ar.WriteString("##" + _formula.mFunction[i].func + "##\r\n" + _formula.mFunction[i].func_text + "\r\n\r\n");
			}
		}
	}
}

void CFormula::CreateHandListMatrices()
{
	int			listnum = 0, i = 0, j = 0;
	CString		token = "";

	CSLock lock(m_critsec);

	for (listnum=0; listnum<MAX_HAND_LISTS; listnum++)
		for (i=0; i<=12; i++)
			for (j=0; j<=12; j++)
				_formula.inlist[listnum][i][j] = false;

	for (i=0; i<(int) _formula.mHandList.GetSize(); i++)
	{
		listnum = atoi(_formula.mHandList[i].list.Mid(4).GetString());
		
		if(listnum>=MAX_HAND_LISTS)
			continue;
		
		ParseHandList(_formula.mHandList[i].list_text, _formula.inlist[listnum]);
	}
}

bool CFormula::ParseAllFormula(HWND hwnd, bool disable_msgbox)
{
	// returns true for successful parse of all trees, false otherwise
	sData			data;

	data.all_parsed = true;
	data.calling_hwnd = hwnd;
	data.pParent = this;
	data.disable_msgbox = disable_msgbox;

	CUPDialog dlg_progress(hwnd, ParseLoop, &data, "Please wait", false);
	dlg_progress.DoModal();

	return data.all_parsed;
}

void CFormula::CheckForDefaultFormulaEntries()
{
	int			N = 0, i = 0;
	bool		addit = false;
	SFunction	func;	
	
	N = (int) _formula.mFunction.GetSize();
	func.func_text = ""; 
	func.dirty = true; 

	CSLock lock(m_critsec);

	// notes
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="notes") addit = false;
	if (addit==true)  
	{ 
		func.func = "notes"; 
		_formula.mFunction.Add(func); 
	}

	// dll
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="dll") addit = false;
	if (addit==true)  
	{ 
		func.func = "dll"; 
		_formula.mFunction.Add(func); 
	}

	// f$alli
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$alli") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$alli"; 
		_formula.mFunction.Add(func); 
	}

	// f$swag
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$swag") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$swag"; 
		_formula.mFunction.Add(func); 
	}

	// f$srai
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$srai") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$srai"; 
		_formula.mFunction.Add(func); 
	}

	// f$rais
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$rais") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$rais"; 
		_formula.mFunction.Add(func); 
	}

	// f$call
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$call") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$call"; 
		_formula.mFunction.Add(func); 
	}

	// f$prefold
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$prefold") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$prefold"; 
		_formula.mFunction.Add(func); 
	}

	// f$rebuy
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$rebuy") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$rebuy"; 
		_formula.mFunction.Add(func); 
	}

	// f$delay
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$delay") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$delay"; 
		_formula.mFunction.Add(func); 
	}

	// f$chat
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$chat") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$chat"; 
		_formula.mFunction.Add(func); 
	}

	// f$P
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$P") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$P"; 
		_formula.mFunction.Add(func); 
	}

	// f$play
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$play") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$play"; 
		_formula.mFunction.Add(func); 
	}

	// f$test
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$test") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$test"; 
		_formula.mFunction.Add(func); 
	}

	// f$debug
	addit = true;
	for (i=0; i<N; i++)  
		if (_formula.mFunction[i].func=="f$debug") addit = false;
	if (addit==true)  
	{ 
		func.func = "f$debug"; 
		_formula.mFunction.Add(func); 
	}
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
			if (index == -1) break;
			pStr += index;
		}

		token_card0_rank = CardIdentHelper(*pStr++);

		if (token_card0_rank == -1)
			continue;

		token_card1_rank = CardIdentHelper(*pStr++);

		if (token_card0_rank == -1)
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
	int				from_count = 0, to_count = 0, from_iter = 0, to_iter = 0;
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
	for (to_iter=0; to_iter<to_count; to_iter++)
	{
		from_count = (int) f->formula()->mFunction.GetSize();
		deleteit = true;
		for (from_iter=0; from_iter<from_count; from_iter++)
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
	for (from_iter=0; from_iter<from_count; from_iter++)
	{
		to_count = (int) _formula.mFunction.GetSize();
		addit = true;
		for (to_iter=0; to_iter<to_count; to_iter++)
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

	// Copy numbers
	_formula.dBankroll = f->formula()->dBankroll;
	_formula.dDefcon = f->formula()->dDefcon;
	_formula.dRake = f->formula()->dRake;
	_formula.dNit = f->formula()->dNit;

	// Copy hand lists
	_formula.mHandList.RemoveAll();
	from_count = (int) f->formula()->mHandList.GetSize();
	for (from_iter=0; from_iter<from_count; from_iter++)
	{
		list.list = f->formula()->mHandList[from_iter].list;
		list.list_text = f->formula()->mHandList[from_iter].list_text;
		if (DoesFormulaAlreadyExist(list.list))
		{
			CString ErrorMessage = "Handlist does already exist: " + list.list;
			MessageBox(0, ErrorMessage, "Error", 0);
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
	bool			gui_enable_progress_dialog = !prefs.gui_disable_progress_dialog();

	pCUPDUPData->SetProgress("", 0, false);

	// init timer
	QueryPerformanceCounter(&bcount);
	QueryPerformanceFrequency(&lFrequency);

	N = (int) data->pParent->formula()->mFunction.GetSize();
	for (i=0; i<N; i++)
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

				if (!data->disable_msgbox)
				{
					s.Format("Error in parse of %s\nLine: %d  Character: %d\n\nNear:\n \"%s\"",
							 data->pParent->formula()->mFunction[i].func.GetString(),
							 linenum, colnum,
							 data->pParent->formula()->mFunction[i].func_text.Mid(stopchar, 40).GetString());
					MessageBox(data->calling_hwnd, s, "PARSE ERROR", MB_OK);
				}

				data->all_parsed = false;
			}

			else if (gram.parse_symbol_stop_strs()->GetSize() != 0)
			{
				if (!data->disable_msgbox)
				{
					s.Format("Error in parse of %s\n\nInvalid symbols:\n",
							 data->pParent->formula()->mFunction[i].func.GetString());
					for (j=0; j<gram.parse_symbol_stop_strs()->GetSize(); j++)
					{
						s.Append("   ");
						s.Append(gram.parse_symbol_stop_strs()->GetAt(j).c_str());
						s.Append("\n");
					}
					MessageBox(data->calling_hwnd, s, "PARSE ERROR", MB_OK);
				}

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
	write_log(3, "CFormula::DoesFormulaAlreadyExist(): number_of_formulae = [%d]\n", 
		number_of_formulae);
	for (int i=0; i<number_of_formulae; i++)
	{
		write_log(3, "CFormula::DoesFormulaAlreadyExist(): [%s] == [%s]\n",
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
		write_log(3, "CFormula::DoesHandlistAlreadyExist(): [%s] == [%s]\n",
			new_name, _formula.mHandList[i].list);
		if (new_name == _formula.mHandList[i].list)
		{
			return true;
		}
	}
	return false;
}