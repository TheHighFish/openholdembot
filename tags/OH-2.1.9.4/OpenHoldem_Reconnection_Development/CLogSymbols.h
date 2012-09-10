#ifndef INC_CLOGSYMBOLS_H
#define INC_CLOGSYMBOLS_H
	
class CLogSymbols
{
public:
	CLogSymbols();
	~CLogSymbols();
	double	ProcessQuery(const char * pquery, int *e);
};


#endif //INC_CLOGSYMBOLS_H