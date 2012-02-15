#ifndef INC_C_REGIONCLONER_H
#define INC_C_REGIONCLONER_H

class CRegionCloner
{
public:
	CRegionCloner();
	~CRegionCloner();
	void CloneRegions();
private:
	bool TableSizeUndefined();
	void CloneCommonCards();
};

#endif // INC_C_REGIONCLONER_H