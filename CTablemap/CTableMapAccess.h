//********************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//********************************************************************************
//
// Purpose:
//
//********************************************************************************


#ifndef INC_CTABLEMAPACCESS_H
#define INC_CTABLEMAPACCESS_H

#include <windows.h>
#include "CTablemap.h"

enum dim {width, height};

class CTablemapAccess
{

public:
	CTablemapAccess();
	~CTablemapAccess();
	
public:
	// r$
	bool GetButtonRect(CString button_name, RECT *_rect);
	bool GetTableMapRect(CString rect_name, RECT *_rect);
	bool GetTableMapRegion(CString region_name, STablemapRegion *_region);

public:
	// s$
	bool SetTitleText(CString title_name, CString &destination);

public:
	// z$
	// Usable for clientsize, clientsizemin, clientsizemax and targetsize
  bool SetClientSize(CString size_name, int *width, int *height);
	unsigned int GetClientSize(CString size_name, dim dimension);

	// functions below only for clientsize	
	unsigned int GetClientSizeX();
	unsigned int GetClientSizeY();
	
};

extern CTablemapAccess *p_tablemap_access;

#endif // INC_CTABLEMAPACCESS_H