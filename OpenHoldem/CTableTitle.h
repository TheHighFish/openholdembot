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

#ifndef INC_CTABLETITLE_H
#define INC_CTABLETITLE_H

class CTableTitle {
  friend class CDlgEditSymbols;
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
 public:
  // Public static for preprocessing of scraped ttlimitsX by CScraper.cpp
  static CString PreprocessTitle(CString title);
 protected:
  // Accessible by OpenScrape (CDlgEditSymbols)
  void SetTitle(CString new_title);
 private:
  void SelfTest();
 private:
  CString _title;
  CString _preprocessed_title;
  CString _previous_title;
};

extern CTableTitle *p_table_title;

#endif INC_CTABLETITLE_H