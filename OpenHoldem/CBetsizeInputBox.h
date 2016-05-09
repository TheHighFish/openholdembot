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

#ifndef INC_CBETSIZEINPUTBOX_H
#define INC_CBETSIZEINPUTBOX_H

class CBetsizeInputBox {
 public:
  CBetsizeInputBox();
  ~CBetsizeInputBox();
 public:
  bool EnterBetsize(double total_betsize_in_dollars);
  // Depends on complete tablemap
  // and maybe visible betsize-confirmation-button
  bool IsReadyToBeUsed();
 private:
  void SelectText();
  void Clear();
  void Confirm();
 private:
  // For future use
  bool VerifyEnteredBetsize();
 private:
  RECT _i3_edit_region;
  //!!!!!
  RECT r_null;
  POINT p_null;
};

#endif INC_CBETSIZEINPUTBOX_H