#include "stdafx.h"
#include "CRegionCloner.h"
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

void CRegionCloner::CalculateLinearRegions(RECT first_region, int number_of_regions)
{
	// Calculates the position for N regions,
	// horizontally aligned and symmetrically to the middle of the table,
	// with equal distance to each other.
	int width_of_region = first_region.right - first_region.left;
	int space_for_remaining_regions = p_tablemap_access->GetClientSizeX()
		- first_region.left // distance to the left of first region
		- first_region.left // distance to the right of last region
		- width_of_region;
	int delta_X_to_next_region = space_for_remaining_regions
		/ (number_of_regions - 1);
	for (int i=1; i<k_number_of_community_cards; i++)
	{
		linear_region_positions[i].left = first_region.left
			+ i * delta_X_to_next_region;
		linear_region_positions[i].right = first_region.right
			+ i * delta_X_to_next_region;
	}
}

void CRegionCloner::CloneCommonCards()
{
	RECT c0cardface0;
	if (!p_tablemap_access->GetTableMapRect("c0cardface0", &c0cardface0))
	{
		return;
	}
	int width_of_card = c0cardface0.right - c0cardface0.left;
	int space_for_remaining_cards = p_tablemap_access->GetClientSizeX()
		- c0cardface0.left // distance to the left
		- c0cardface0.left // distance to the left
		- width_of_card;
	int delta_X_to_next_card = space_for_remaining_cards 
		/ (k_number_of_community_cards - 1);
	for (int i=1; i<k_number_of_community_cards; i++)
	{
		int next_pos_left  = c0cardface0.left
			+ i * delta_X_to_next_card;
		int next_pos_right = c0cardface0.right
			+ i * delta_X_to_next_card;
		MessageBox(0, "next region", "Info", 0);

		STablemapRegion new_region;
		new_region.name = "c0cardface4";
				new_region.left = next_pos_left;
				new_region.top = c0cardface0.top;
				new_region.right = next_pos_right;
				new_region.bottom = c0cardface0.bottom;
				new_region.color = 0;
				new_region.radius = 0;
				new_region.transform = "N";
		p_tablemap->r$_insert(new_region);
		/*new_region.name = "r$c0cardface3";*/
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