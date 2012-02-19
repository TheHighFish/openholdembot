#include "stdafx.h"
#include "CRegionCloner.h"

#include <assert.h>
#include "..\CTablemap\CTablemap.h"
#include "..\CTablemap\CTablemapAccess.h"
#include "..\OpenHoldem\MagicNumbers.h"


CRegionCloner::CRegionCloner()
{
	if (p_tablemap_access != NULL)
	{
		p_tablemap_access = new CTablemapAccess;
	}
}

CRegionCloner::~CRegionCloner()
{
	delete(p_tablemap_access); 
}

bool CRegionCloner::TableSizeUndefined()
{
	return ((p_tablemap_access->GetClientSizeX() <= 0)
		|| (p_tablemap_access->GetClientSizeY() <= 0));
}

CString CRegionCloner::CreateName(CString prefix, int number, CString postfix)
{
	const int k_max_length_of_name = 100;
	char new_symbol_name[k_max_length_of_name];
	assert(number >= 0);
	assert(number <= 9);
	assert(prefix.GetLength() + postfix.GetLength() < k_max_length_of_name); 
	sprintf(new_symbol_name, "%s%d%s", prefix, number, postfix);
	return CString(new_symbol_name);
}

void CRegionCloner::CalculateLinearRegions(STablemapRegion first_region, int number_of_regions)
{
	// Calculates the position for N regions,
	// horizontally aligned and symmetrically to the middle of the table,
	// with equal distance to each other.
	assert(number_of_regions >= 1);
	assert(number_of_regions <= k_max_number_of_regions_to_clone); 

	int width_of_region = first_region.right - first_region.left;
	int space_for_remaining_regions = p_tablemap_access->GetClientSizeX()
		- first_region.left // distance to the left of first region
		- first_region.left // distance to the right of last region
		- width_of_region;
	int delta_X_to_next_region = space_for_remaining_regions
		/ (number_of_regions - 1);
	for (int i=0; i<number_of_regions; i++)
	{
		// Align horizontally, don't change the first region
		linear_region_positions[i].left = first_region.left
			+ i * delta_X_to_next_region;
		linear_region_positions[i].right = first_region.right
			+ i * delta_X_to_next_region;
		// Keep all at the same Y-position
		linear_region_positions[i].top = first_region.top;
		linear_region_positions[i].bottom = first_region.bottom;
	}
}

void CalculateCircularRegions(STablemapRegion first_region, int number_of_regions)
{
	// Calculates the position for N regions,
	// about circular aligned and around the center of the table.
	assert(number_of_regions >= 1);
	assert(number_of_regions <= k_max_number_of_regions_to_clone); 
}

void CRegionCloner::ApplyNextLinearRegionPosition(STablemapRegion *new_region, int index)
{
	assert(index >= 0);
	assert(index < k_max_number_of_regions_to_clone);

	new_region->bottom = linear_region_positions[index].bottom;
	new_region->top    = linear_region_positions[index].top;
	new_region->left   = linear_region_positions[index].left;
	new_region->right  = linear_region_positions[index].right;
}

void CRegionCloner::CloneCommonCards()
{
	STablemapRegion c0cardface0;
	if (!p_tablemap_access->GetTableMapRegion("c0cardface0", &c0cardface0))
	{
		return;
	}
	CalculateLinearRegions(c0cardface0, k_number_of_community_cards);
	for (int i=0; i<k_number_of_community_cards; i++)
	{	
		STablemapRegion new_region;
		ApplyNextLinearRegionPosition(&new_region, i);

		new_region.name = CreateName("c0cardface", i, "");
		new_region.color = c0cardface0.color;
		new_region.radius = c0cardface0.radius;
		new_region.transform = c0cardface0.transform;

		p_tablemap->r$_insert(new_region);

		//pDoc->SetModifiedFlag(true);
		//			Invalidate(false);
	}
}

void CRegionCloner::CloneRegions()
{
	if (TableSizeUndefined())
	{
		MessageBox(0, "Can't clone regions.\n"
			"Table size undefined.", "Error", 0);
		return;
	}
	CloneCommonCards();

	RECT c0;
	if (p_tablemap_access->GetTableMapRect("c0pot0", &c0))
	{
		CString s = "Region found"; // + c0.left;
		MessageBox(0, s, "Info", 0);
	}
	else
	{
		MessageBox(0, "Failed", "Info", 0);
	}
}