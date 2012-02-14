#ifndef INC_CTABLEMAPACCESS_H
#define INC_CTABLEMAPACCESS_H

#include <windows.h>

extern class CTablemapAccess
{

public:
	CTablemapAccess();
	~CTablemapAccess();
	
public:
	// public
	bool GetButtonRect(CString button_name, RECT *_rect);
	bool GetTableMapRect(CString region_name, RECT *_rect);
	bool IsTableMapRectDefined(CString region_name);
	
} *p_tablemap_access;

#endif // INC_CTABLEMAPACCESS_H