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

int CTablemapAccess::GetClientSizeX()
{
	ZMapCI z_iter = p_tablemap->z$()->find("clientsize");

	if (z_iter != p_tablemap->z$()->end())
	{
		return z_iter->second.width;
	}
	else
	{
		// 0 is the default formerly used by the auto-connector
		return 0;
	}
}

int CTablemapAccess::GetClientSizeY()
{
	ZMapCI z_iter = p_tablemap->z$()->find("clientsize");

	if (z_iter != p_tablemap->z$()->end())
	{
		return z_iter->second.height;
	}
	else
	{
		return 0;
	}
}