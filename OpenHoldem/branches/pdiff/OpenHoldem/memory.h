#pragma once

extern class Memory 
{
public:
	Memory::Memory();
	double process_query(const char * pquery, int *e);

private:
	int		var_count;
	char	var_name[512][64];
	double	var_value[512];

	void store_value(const char * pquery, int *e);
	double retrieve_value(const char * pquery, int *e);

} memory;
