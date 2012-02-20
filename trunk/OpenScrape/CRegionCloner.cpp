#include "stdafx.h"
#include "CRegionCloner.h"

#include <assert.h>
#include <math.h>
#include "..\CTablemap\CTablemap.h"
#include "..\CTablemap\CTablemapAccess.h"
#include "..\OpenHoldem\MagicNumbers.h"
#include "OpenScrape.h"


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

void CRegionCloner::CalculateCircularRegions(STablemapRegion first_region, int number_of_regions)
{
	// Calculates the position for N regions,
	// about circular aligned and around the center of the table.
	assert(number_of_regions >= 1);
	assert(number_of_regions <= k_max_number_of_regions_to_clone); 

	int center_x_of_region = 0.5 * (first_region.left + first_region.right);
	int center_y_of_region = 0.5 * (first_region.top + first_region.bottom);

	MessageBox(0, CreateName("", center_x_of_region, ""),
			"center_x_of_region", 0);

	int center_x_of_table  = 0.5 * p_tablemap_access->GetClientSizeX();
	int center_y_of_table  = 0.5 * p_tablemap_access->GetClientSizeY();

	MessageBox(0, CreateName("", center_x_of_table, ""),
			"center_x_of_table", 0);

	int delta_x_between_centers = center_x_of_table - center_x_of_region;
	int delta_y_between_centers = center_y_of_table - center_y_of_region;

	MessageBox(0, CreateName("", delta_x_between_centers, ""),
			"delta_x_between_centers", 0);

	int distance_to_center = sqrt(float(
		  delta_x_between_centers * delta_x_between_centers
		+ delta_y_between_centers * delta_y_between_centers));

	MessageBox(0, CreateName("", distance_to_center, ""),
			"Distance", 0);

	float angle_between_regions = (2 * M_PI) / number_of_regions;

	// starting angle for region 0.
	// 0 means: 12 o'clock position
	int angle_for_region_0 = 0;
	if (delta_x_between_centers == 0)
	{
		if (center_y_of_region > center_y_of_table)
		{
			angle_for_region_0 = 0;
		}
		else
		{
			angle_for_region_0 = M_PI;
		}
	}
	else
	{
		angle_for_region_0 = sin(float(delta_y_between_centers / delta_x_between_centers));
	}
	// Don't do any calculations for region 0.
	// We keep it anyway and don't want to ruin it even by one pixel.
	for (int i=1; i<number_of_regions; i++)
	{
		float angle_for_region_i = angle_for_region_0 + (i * angle_between_regions);
		if (angle_for_region_i > 2 * M_PI)
		{
			// normalize angle, in case we didn't start right of 12 o'clock.
			angle_for_region_i -= 2 * M_PI;
		}
		assert(angle_for_region_i >= 0);
		assert(angle_for_region_i <= 2 * M_PI);

		MessageBox(0, CreateName("", angle_for_region_i, ""),
			"Angle", 0);

		// distance from center of table to center of region
		// distance may be negative, so we can simply add it alter;
		int delta_x_to_region_i = sin(angle_for_region_i) * distance_to_center;
		int delta_y_to_region_i = cos(angle_for_region_i) * distance_to_center;		
		
		// absolue position of center of new region
		int center_x_of_region_1 = center_x_of_table + delta_x_to_region_i;
		int center_y_of_region_1 = center_y_of_table + delta_y_to_region_i;

		int width_of_region = first_region.right - first_region.left;
		int height_of_region = first_region.top - first_region.bottom;

		// finally calculating the corners of the new region
		circular_region_positions[i].left   = center_x_of_region_1 - (1/2 * width_of_region);
		circular_region_positions[i].bottom = center_y_of_region_1 - (1/2 * height_of_region);
		circular_region_positions[i].right  = circular_region_positions[i].left   + width_of_region;
		circular_region_positions[i].top    = circular_region_positions[i].bottom + height_of_region;
		MessageBox(0, CreateName("", circular_region_positions[i].left, ""),
			"Left", 0);
	}	
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

void CRegionCloner::ApplyNextCircularRegionPosition(STablemapRegion *new_region, int index)
{
	// We didn't calculate position 0,
	// because we want to keep it exactly as is.
	assert(index > 0);
	assert(index < k_max_number_of_regions_to_clone);

	new_region->bottom = circular_region_positions[index].bottom;
	new_region->top    = circular_region_positions[index].top;
	new_region->left   = circular_region_positions[index].left;
	new_region->right  = circular_region_positions[index].right;
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

		theApp.m_TableMapDlg->UpdateDisplayOfAllRegions();
	}
}

const int k_number_of_circular_cloneable_regions = 9;

// 1st string: name of 0-region
// 2nd string: prefix before number
// 3rd string: postfix after number
const CString circular_cloneable_regions[k_number_of_circular_cloneable_regions][3] =
{{"p0active",    "p", "active"},  
 {"p0balance",   "p", "balance"},
 {"p0bet",       "p", "bet"},
 {"p0cardback",  "p", "cardback"},
 {"p0cardface0", "p", "cardface0"},
 {"p0cardface1", "p", "cardface1"},
 {"p0dealer",    "p", "dealer",},
 {"p0name",      "p", "name"},
 {"p0seated",    "p", "seated"}};

void CRegionCloner::CloneBalances()
{
	STablemapRegion c0cardface0;
	if (!p_tablemap_access->GetTableMapRegion("p0balance", &c0cardface0))
	{
		return;
	}
	CalculateCircularRegions(c0cardface0, k_max_number_of_players);
	for (int i=0; i<k_max_number_of_players; i++)
	{	
		STablemapRegion new_region;
		ApplyNextCircularRegionPosition(&new_region, i);

		new_region.name = CreateName("p", i, "balance");
		new_region.color = c0cardface0.color;
		new_region.radius = c0cardface0.radius;
		new_region.transform = c0cardface0.transform;

		p_tablemap->r$_insert(new_region);

		theApp.m_TableMapDlg->UpdateDisplayOfAllRegions();
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
	CloneBalances();
}