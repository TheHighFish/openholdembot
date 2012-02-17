#ifndef INC_CTABLEMAPACCESS_H
#define INC_CTABLEMAPACCESS_H

#include <windows.h>
#include "CTablemap.h"

typedef struct
{
	int width;
	int height;

} clientsize;

enum dim {width, height};

extern class CTablemapAccess
{

public:
	CTablemapAccess();
	~CTablemapAccess();
	
public:
	// r$ public
	bool GetButtonRect(CString button_name, RECT *_rect);
	bool GetTableMapRect(CString rect_name, RECT *_rect);
	bool GetTableMapRegion(CString region_name, STablemapRegion *_region);

public:
	// s$ public
	bool SetString(CString search, CString &destination, SWholeMap *smap);

public:
	// z$ public
	// Usable for clientsize, clientsizemin and clientsizemax
	unsigned int GetSize(CString size_name, dim dimension, SWholeMap *smap);
	bool SetSize(CString size_name, unsigned int &width, unsigned int &height, SWholeMap *smap);
	bool GetClientSize(CString size_name, clientsize *z_size);

	// functions below only for clientsize
	int GetClientSizeX();
	int GetClientSizeY();
	
} *p_tablemap_access;

#endif // INC_CTABLEMAPACCESS_H