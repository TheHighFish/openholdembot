#include "stdafx.h"
#include "CTableMapAccess.h"

#include "..\CTablemap\CTablemap.h"


CTablemapAccess *p_tablemap_access = NULL;


CTablemapAccess::CTablemapAccess()
{
}

CTablemapAccess::~CTablemapAccess()
{
}

bool CTablemapAccess::GetButtonRect(CString button_name, RECT *_rect)
{
	/*
		Inserts button details into RECT parameter
	*/

	return GetTableMapRect(button_name, _rect);
}

bool CTablemapAccess::GetTableMapRect(CString region_name, RECT *_rect)
{
	/*
		Finds tablemap regions (i3_edit, i3_slider, i3_handle, iXbutton, i86button, i86Xbutton)
		and inserts details into RECT parameter
	*/

	RMapCI wanted_region = p_tablemap->r$()->find(region_name);

	if (wanted_region != p_tablemap->r$()->end())
	{
		_rect->left   = wanted_region->second.left;
		_rect->top    = wanted_region->second.top;
		_rect->right  = wanted_region->second.right;
		_rect->bottom = wanted_region->second.bottom;

		return true;
	}

	return false;
}

bool CTablemapAccess::IsTableMapRectDefined(CString region_name)
{
	/*
		Checks if a Tablemap region is defined
	*/

	RMapCI wanted_region = p_tablemap->r$()->find(region_name);

	if (wanted_region != p_tablemap->r$()->end())
		return true;

	return false;
}