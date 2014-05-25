//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

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
  void CheckCardFaces(CString prefix, int infix, CString postfix);
  void CheckBetsOrStacks();
  void CheckMainPot();
 private:
   void ErrorMissingItem(CString item);
  bool IsNoLimitMap();
 private:
  bool _error_flag;
};

extern CTablemapCompletenessChecker *p_tablemap_versionchecker;

#endif // INC_CTABLEMAPCOMPLETENESSCHECKER_H 