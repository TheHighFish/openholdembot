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

bool CTablemapAccess::GetTableMapRect(CString rect_name, RECT *_rect)
{
	/*
		Finds tablemap regions (i3_edit, i3_slider, i3_handle, iXbutton, i86button, i86Xbutton)
		and inserts details into RECT parameter
	*/

	RMapCI wanted_region = p_tablemap->r$()->find(rect_name);

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

bool GetTableMapRegion(CString region_name, STablemapRegion *_region)
{
	/*
		Finds tablemap regions (i3_edit, i3_slider, i3_handle, iXbutton, i86button, i86Xbutton)
		and inserts details into region parameter
	*/

	RMapCI wanted_region = p_tablemap->r$()->find(region_name);

	if (wanted_region != p_tablemap->r$()->end())
	{
		_region->left      = wanted_region->second.left;
		_region->top       = wanted_region->second.top;
		_region->right     = wanted_region->second.right;
		_region->bottom    = wanted_region->second.bottom;
		_region->color     = wanted_region->second.color;
		_region->radius    = wanted_region->second.radius;
		_region->transform = wanted_region->second.transform;
		_region->name      = wanted_region->second.name;
		_region->cur_bmp   = wanted_region->second.cur_bmp;
		_region->last_bmp  = wanted_region->second.last_bmp;
	


		return true;
	}

	return false;
}

unsigned int CTablemapAccess::GetSize(CString size_name, dim dimension, SWholeMap *smap)
{
	// Extract client size information
	ZMapCI z_iter = smap->z$->find(size_name);

	if (z_iter != smap->z$->end())
	{
		if (dimension == width)
			return z_iter->second.width;
		if (dimension == height)
			return z_iter->second.height;
	}

	// 0 is the default formerly used by the auto-connector
	return 0;
}

bool CTablemapAccess::SetSize(CString size_name, unsigned int &width, unsigned int &height, SWholeMap *smap)
{
	// Extract client size information
	ZMapCI z_iter = smap->z$->find(size_name);

	if (z_iter != smap->z$->end())
	{
		width = z_iter->second.width;
		height = z_iter->second.height;

		return true;
	}

	// 0 is the default formerly used by the auto-connector
	width = 0;
	height = 0;
	
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
		return z_size.width;
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
		return z_size.height;
	}
	else
	{
		// 0 is the default formerly used by the auto-connector
		return 0;
	}
}