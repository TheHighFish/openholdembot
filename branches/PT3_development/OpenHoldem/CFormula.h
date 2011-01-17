#ifndef INC_CFORMULA_H
#define INC_CFORMULA_H

#include "UPDialog.h"
#include "..\CCritSec\CCritSec.h"

typedef	boost::spirit::tree_parse_info<const char *, int_factory_t>		tpi_type;

struct SHandList 
{
	CString		list;
	CString		list_text;
};

struct SFunction 
{
	CString		func;
	CString		func_text;
	bool		dirty;
	tpi_type	tpi;
	bool		fresh;
	double		cache;
};

struct SFormula 
{
	CArray <SFunction, SFunction> mFunction;

	double dBankroll, dDefcon, dRake, dNit;

	CArray <SHandList, SHandList> mHandList;

	bool	inlist[1000][13][13];  // list number, rank0/rank1 - rank0>=rank1 == suited, rank0<rank1 == unsuited
};

class CFormula;
struct	sData 
{
	// For parsing progress dialog
	bool		all_parsed;
	HWND		calling_hwnd;
	CFormula	*pParent;
};

extern class CFormula 
{
public:
	// public functions
	CFormula();
	~CFormula();
	void ClearFormula();
	void SetDefaultBot();
	void ReadFormulaFile(CArchive& ar, bool ignoreFirstLine);
	void WriteFormula(CArchive& ar);
	void CreateHandListMatrices();
	bool ParseAllFormula(HWND hwnd, bool disable_msgbox);
	void AddDefaultFunctionIfFunctionDoesNotExist(const CString &FunctionName);
	void CheckForDefaultFormulaEntries();
	void MarkCacheStale();
	void ParseHandList(const CString &list_text, bool inlist[13][13]);
	void CopyFormulaFrom(CFormula *f);
	const char *GetFunctionText(const char *name);

public:
	// public accessors
	const SFormula * formula() { return &_formula; }
	const CString formula_name() { return _formula_name; }

public:
#define ENT CSLock lock(m_critsec);
	// public mutators
	void set_formula_name(const CString s) { ENT _formula_name = s; }
	void set_func_func(const int i, const CString s) { ENT if (i>=0 && i<_formula.mFunction.GetSize()) _formula.mFunction[i].func = s; }
	void set_func_cache(const int i, const double d) { ENT if (i>=0 && i<_formula.mFunction.GetSize()) _formula.mFunction[i].cache = d; }
	void set_func_fresh(const int i, const bool b) { ENT if (i>=0 && i<_formula.mFunction.GetSize()) _formula.mFunction[i].fresh = b; }
	void set_func_dirty(const int i, const bool b) { ENT if (i>=0 && i<_formula.mFunction.GetSize()) _formula.mFunction[i].dirty = b; }
	void set_func_text(const int i, const CString s) { ENT if (i>=0 && i<_formula.mFunction.GetSize()) _formula.mFunction[i].func_text = s; }
	void set_func_add(const SFunction f) { ENT _formula.mFunction.Add(f); }
	void set_func_remove(const int i) { ENT if (i>=0 && i<_formula.mFunction.GetSize()) _formula.mFunction.RemoveAt(i,1); }
	void set_list_list(const int i, const CString s) { ENT if (i>=0 && i<_formula.mHandList.GetSize()) _formula.mHandList[i].list = s; }
	void set_list_text(const int i, const CString s) { ENT if (i>=0 && i<_formula.mHandList.GetSize()) _formula.mHandList[i].list_text = s; }
	void set_list_add(const SHandList h) { ENT _formula.mHandList.Add(h); }
	void set_list_remove(const int i) { ENT if (i>=0 && i<_formula.mHandList.GetSize()) _formula.mHandList.RemoveAt(i,1); }
	//void set_list_removeall() { ENT _formula.mHandList.RemoveAll(); }
	void set_bankroll(const double d) { ENT _formula.dBankroll = d; }
	void set_defcon(const double d) { ENT _formula.dDefcon = d; }
	void set_rake(const double d) { ENT _formula.dRake = d; }
	void set_nit(const double d) { ENT _formula.dNit = d; }
	tpi_type * set_tpi(const int i) { if (i>=0 && i<_formula.mFunction.GetSize()) return &_formula.mFunction[i].tpi; else return NULL; }
#undef ENT

private:
	bool IsStandardFormula(CString formula_name);
	void WriteStandardFunction(CArchive& ar, CString name);
	bool DoesFormulaAlreadyExist(const CString new_name);
	bool DoesHandlistAlreadyExist(const CString new_name);
	void WarnAboutOutdatedConcepts();

private:
	// private variables - use public accessors and public mutators to address these
	SFormula		_formula;
	CString			_formula_name;

private:
	// private functions and variables - not available via accessors or mutators
	const int CardIdentHelper(const char c);
	static bool ParseLoop(const CUPDUPDATA* pCUPDUPData);

	CCritSec		m_critsec;

} *p_formula;

#endif //INC_CFORMULA_H