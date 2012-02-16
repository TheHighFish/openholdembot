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

bool CTablemapAccess::GetClientSize(CString size_name, clientsize *z_size)
{
	ZMapCI z_iter = p_tablemap->z$()->find(size_name);

	if (z_iter != p_tablemap->z$()->end())
	{
		z_size->width = z_iter->second.width;
		z_size->height = z_iter->second.height;

		return true;
	}

	return false;
}

int CTablemapAccess::GetClientSizeX()
{
	clientsize z_size;
	if (GetClientSize("clientsize", &z_size))
	{
		return z_size.clientsize_x;
	}
	else
	{
		// 0 is the default formerly used by the auto-connector
		return 0;
	}
}

int CTablemapAccess::GetClientSizeY()
{
	clientsize z_size;
	if (GetClientSize("clientsize", &z_size))
	{
		return z_size.clientsize_y;
	}
	else
	{
		return 0;
	}
}