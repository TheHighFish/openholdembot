//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: small data container for the table-title 
//
//******************************************************************************

class CTableTitle {
 public:
  CTableTitle();
  ~CTableTitle();
 public:
  // Must be executed exactly once per heartbeat by the scraper
  // as we also use this function to re,e,ber the last title.
  void UpdateTitle();
  void Clear();
  CString Title();
  CString PreprocessedTitle();
  bool TitleChangedSinceLastHeartbeat();
 private:
  void SelfTest();
  void SetTitle(CString new_title);
 private:
  CString _title;
  CString _previous_title;
};