//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose:
//
//*******************************************************************************

#ifndef INC_CTABLEMAPCOMPLETENESSCHECKER_H 
#define INC_CTABLEMAPCOMPLETENESSCHECKER_H 

class CTablemapCompletenessChecker {
 public:
  CTablemapCompletenessChecker();
  ~CTablemapCompletenessChecker();
 public:
  void VerifyMap();
 private:
  void CheckItem(CString item);
  void CheckItem(CString prefix, int infix, CString postfix);
  void CheckSetOfItems(CString prefix, int last_index, CString postfix, bool mandatory);
  void CheckCardFaces(CString prefix, int infix, CString postfix);
  void CheckBetsOrChips();
  void CheckMainPot();
 private:
  void CheckForDeprecatedItems();
  void VerifyTablePoints();
  void VerifyC0limits();
  void VerifySingleC0limitsItem(CString name);
 private:
  void ErrorMissingItem(CString item);
  void ErrorDeprecatedItem(CString item);
  void ErrorSwagRenamed();
  bool IsNoLimitMap();
 private:
  bool _error_flag;
};

extern CTablemapCompletenessChecker *p_tablemap_versionchecker;

#endif // INC_CTABLEMAPCOMPLETENESSCHECKER_H 