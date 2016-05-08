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
  void EnterBetsize();
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
};

#endif INC_CBETSIZEINPUTBOX_H