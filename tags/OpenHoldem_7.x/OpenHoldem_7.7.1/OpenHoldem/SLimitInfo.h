//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_SLIMITINFO_H
#define INC_SLIMITINFO_H

class CLimitInfo {
  friend class CScraper;
 public:
  CLimitInfo();
  ~CLimitInfo();
 public:
  void Reset();
 public:
  // The functions below reurn values depending on game-state,
  // based on the info of
  //   * _sblind
  //   * _bblind
  //   * _bbet
  //   * _sb_bb
  //   * _bb_BB 
  double	sblind(bool is_fixed_limit);
	double	bblind(bool is_fixed_limit);
	double	bbet(bool is_fixed_limit);
 public:
  double	ante()           { return _ante; }
	int		  limit()          { return _limit; }
  CString	handnumber()     { return _handnumber; }
  double  buyin ()         { return _buyin; }
  bool    is_final_table() { return _is_final_table; }
 protected:
	double	_sblind;
	double	_bblind;
	double	_bbet;
	double	_ante;
	int		  _limit;
	// Handnumber should be a string, as
	//   * it may contain characters
	//   * its lengths my exceed the precision of double
	CString	_handnumber;
	double	_sb_bb;
	double	_bb_BB;
  double  _buyin;
  bool    _is_final_table;
};

#endif // INC_SLIMITINFO_H