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
	bool SetTitleText(CString title_name, CString &destination);

public:
	// z$ public
	// Usable for clientsize, clientsizemin and clientsizemax
	bool SetClientSize(CString size_name, unsigned int &width, unsigned int &height);
	// functions below only for clientsize
	unsigned int GetClientSize(CString size_name, dim dimension);
	unsigned int GetClientSizeX();
	unsigned int GetClientSizeY();
	
} *p_tablemap_access;

#endif // INC_CTABLEMAPACCESS_H