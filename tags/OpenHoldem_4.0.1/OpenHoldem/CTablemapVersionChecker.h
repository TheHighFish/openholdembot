#ifndef INC_CTABLEMAPVERSIONCHECKER_H 
#define INC_CTABLEMAPVERSIONCHECKER_H 

extern class CTablemapVersionChecker
{
public:
	CTablemapVersionChecker();
public:
	bool IsOutdatedString(CString s);
	void ResetErrorFlag();
	bool IsOutDatedMap();
private:
	bool _error_flag;
} *p_tablemap_versionchecker;

#endif // INC_CTABLEMAPVERSIONCHECKER_H 