#ifndef INC_CTABLEMAPACCESS_H
#define INC_CTABLEMAPACCESS_H

#include <windows.h>

typedef struct
{
	int width;
	int height;

} clientsize;

extern class CTablemapAccess
{

public:
	CTablemapAccess();
	~CTablemapAccess();
	
public:
	// public
	bool GetButtonRect(CString button_name, RECT *_rect);
	bool GetTableMapRect(CString region_name, RECT *_rect);
	bool GetClientSize(CString size_name, clientsize *z_size);
	static int GetClientSizeX();
	static int GetClientSizeY();
	
} *p_tablemap_access;

#endif // INC_CTABLEMAPACCESS_H