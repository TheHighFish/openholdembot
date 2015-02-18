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
 public:
  CLimitInfo();
  ~CLimitInfo();
 public:
  void Reset();
 public:
	double	sblind;
	double	bblind;
	double	bbet;
	double	ante;
	int		  limit;
	// Handnumber should be a string, as
	//   * it may contain characters
	//   * its lengths my exceed the precision of double
	CString	handnumber;
	double	sb_bb;
	double	bb_BB;
  double  buyin;
  bool    is_final_table;
};

#endif // INC_SLIMITINFO_H