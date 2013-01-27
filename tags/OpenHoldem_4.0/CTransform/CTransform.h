#ifndef INC_CTRANSFORM_H
#define INC_CTRANSFORM_H


#include "hash/stdint.h"
#include "hash/lookup3.h"
#include "pdiff/CompareArgs.h"
#include "pdiff/Metric.h"
#include "pdiff/RGBAImage.h"

#include "..\CTablemap\CTablemap.h"

///////////////////////////////
// defines
///////////////////////////////

#define DEFAULT_FUZZY_TOLERANCE	0.25

// Hash seeds
#define HASH_SEED_0		0xf0e1d2c3
#define HASH_SEED_1		0x13245768
#define HASH_SEED_2		0xfdecb9a8
#define HASH_SEED_3		0xaaeeff33


///////////////////////////////
// classes
///////////////////////////////
class CTransform
{
public:
	// public functions
	CTransform(void);
	~CTransform(void);
	const int DoTransform(RMapCI region, const HDC hdc, CString *text, 
						  CString *separation=NULL, COLORREF *cr_avg=0);
	const int TTypeTransform(RMapCI region, const HDC hdc, CString *text, 
							 CString *separation, bool background[], bool (*character)[MAX_CHAR_HEIGHT]);
	const void ParseStringBSL(const CString text, const CString format, CString *results=NULL, 	
							  CString *handnumber=NULL, double *sblind=NULL, double *bblind=NULL, double *bbet=NULL, 
							  double *ante=NULL, int *limit=NULL, double *sb_bb=NULL, double *bb_BB=NULL, bool *istournament=NULL,
							  bool *found_handnumber=NULL, bool *found_sblind=NULL, bool *found_bblind=NULL, bool *found_bbet=NULL,
							  bool *found_ante=NULL, bool *found_limit=NULL, bool *found_sb_bb=NULL, bool *found_bb_BB=NULL);
	const bool IsInARGBColorCube(const int center_a, const int center_r, const int center_g, const int center_b, 
								 const int radius, const int pix_a, const int pix_r, const int pix_g, const int pix_b);
	const bool IsInRGBColorCube(const int center_r, const int center_g, const int center_b, 
								const int radius, const int pix_r, const int pix_g, const int pix_b);
	const void GetShiftLeftDownIndexes(const int x_start, const int width, const int height, 
								 const bool background[], const bool character[][MAX_CHAR_HEIGHT], 
								 int *x_begin, int *x_end, int *y_begin, int *y_end);
	const void CalcHexmash(const int left, const int right, const int top, const int bottom, 
						   const bool (*ch)[MAX_CHAR_HEIGHT], CString *hexmash, const bool withspace=false);
	const double StringToMoney(const CString str);

private:
	// private functions and variables - not available via accessors or mutators
	const int CTypeTransform(RMapCI region, const HDC hdc, CString *text, COLORREF *cr_avg);
	const int ITypeTransform(RMapCI region, const HDC hdc, CString *text);
	const int HTypeTransform(RMapCI region, const HDC hdc, CString *text);
	const int DoPlainFontScan(RMapCI region, const int width, const int height, 
							  const bool bg[], const bool (*ch)[MAX_CHAR_HEIGHT], CString *text);
	const int DoFuzzyFontScan(RMapCI region, const int width, const int height, 
							  const bool bg[], const bool (*ch)[MAX_CHAR_HEIGHT], 
							  const double tolerance, CString *text);
	TMapCI GetBestHammingDistance(RMapCI region, const int width, const int height, 
								  const bool bg[], const bool (*ch)[MAX_CHAR_HEIGHT], const int left, 
								  const double tolerance);
	const int CalcHammingDistance(const unsigned int x, const unsigned int y);

};


#endif //INC_CTRANSFORM_H