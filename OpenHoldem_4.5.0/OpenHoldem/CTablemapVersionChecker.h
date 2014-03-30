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

#ifndef INC_CTABLEMAPVERSIONCHECKER_H 
#define INC_CTABLEMAPVERSIONCHECKER_H 

class CTablemapVersionChecker
{
public:
	CTablemapVersionChecker();
public:
	bool IsOutdatedString(CString s);
	void ResetErrorFlag();
	bool IsOutDatedMap();
private:
	bool _error_flag;
};

extern CTablemapVersionChecker *p_tablemap_versionchecker;

#endif // INC_CTABLEMAPVERSIONCHECKER_H 