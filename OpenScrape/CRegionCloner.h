#ifndef INC_C_REGIONCLONER_H
#define INC_C_REGIONCLONER_H

#include "..\OpenHoldem\MagicNumbers.h"

class CRegionCloner
{
public:
	CRegionCloner();
	~CRegionCloner();
	void CloneRegions();
private:
	bool TableSizeUndefined();
	void CloneCommonCards();
	void CalculateLinearRegions(RECT first_region, int number_of_regions);
	void CalculateCircularRegions(RECT first_region, int unmber_of_regions);
private:
	RECT linear_region_positions[k_max_number_of_regions_to_clone];
	RECT circular_region_positions[k_max_number_of_regions_to_clone];
};

#endif // INC_C_REGIONCLONER_H