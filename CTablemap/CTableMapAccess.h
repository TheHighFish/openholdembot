#ifndef INC_CTABLEMAPACCESS_H
#define INC_CTABLEMAPACCESS_H

#include <windows.h>
#include "CTablemap.h"

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
	bool GetTableMapRect(CString rect_name, RECT *_rect);
	bool GetTableMapRegion(CString region_name, STablemapRegion *_region);
	// Usable for clientsize, clientsizemin and clientsizemax
	bool GetClientSize(CString size_name, clientsize *z_size);
	// functions below only for clientsize
	int GetClientSizeX();
	int GetClientSizeY();
	
} *p_tablemap_access;

#endif // INC_CTABLEMAPACCESS_H