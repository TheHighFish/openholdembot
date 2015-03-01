//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Reading the poker-table.
//  State-less class for future multi-table support.
//  All data is now in the CTable'state container.
//
//******************************************************************************

#ifndef INC_CSCRAPER_H
#define INC_CSCRAPER_H

#include "../../CTransform/hash/stdint.h"
#include "../CTablemap/CTablemap.h"
#include "MagicNumbers.h"
#include "NumericalFunctions.h"

class CScraper {
  friend class CLazyScraper;
  friend class CAutoConnector;
 public:
	// public functions and accessors
	CScraper(void);
	~CScraper(void);
 public:
  // For replay-frames
	const HBITMAP		entire_window_cur() { return _entire_window_cur; }
 public:
  bool GetButtonState(const int button_index);
  bool GetButtonState(CString button_state_as_string);
  bool IsCommonAnimation();
 protected:
	void CreateBitmaps(void);
	void DeleteBitmaps(void);
	bool IsIdenticalScrape();
 protected:
	void ScrapeDealer();
	void ScrapeActionButtons();
	void ScrapeActionButtonLabels();
	void ScrapeInterfaceButtons();
	void ScrapeBetpotButtons();
	void ClearAllPlayerNames();
	void ScrapeName(const int chair);
	void ScrapePlayerCards(int chair);
	void ScrapeSlider();
	void ScrapeCommonCards();
	void ScrapeSeatedActive();
	void ScrapeBetsAndBalances();
	void ScrapeAllPlayerCards();
  void ScrapeColourCodes();
  void ScrapeMTTRegions();
 private:
	void ScrapeSeated(int chair);
	void ScrapeActive(int chair);
 private:
	int ScrapeCard(CString name);
  int ScrapeCardback(CString base_name);
  int ScrapeCardByRankAndSuit(CString base_name);
  int ScrapeCardface(CString base_name);
  int ScrapeNoCard(CString base_name);
 private:
	int CardString2CardNumber(CString card);
 private:
	// private functions and variables - not available via accessors or mutators
  double ScrapeUPBalance(int chair, char scrape_u_else_p);
	void ScrapeBalance(const int chair);
	void ScrapeBet(const int chair);
	void ScrapePots();
	void ScrapeLimits();
	const CString extractHandnumFromString(const CString t);
	const double DoChipScrape(RMapCI r_iter);
 private:
	bool ProcessRegion(RMapCI r_iter);
	bool EvaluateRegion(CString name, CString *result);
  void EvaluateTrueFalseRegion(bool *result, const CString name);
  bool EvaluateNumericalRegion(double *result, const CString name);
	bool IsExtendedNumberic(CString text);
	CString ProcessBalanceNumbersOnly(CString balance_and_or_potential_text);
 private:
	void SetButtonState(CString *button_state, CString text);
  void ResetLimitInfo();
 private:
#define ENT CSLock lock(m_critsec);
  void delete_entire_window_cur() { ENT DeleteObject(_entire_window_cur);}
#undef ENT
 private:
	// private variables - use public accessors and public mutators to address these
  CCritSec		m_critsec;
  // Counter of GDI objects (potential memorz leak)
  // Should be 0 at end of program -- will be checked.
  int         _leaking_GDI_objects;
  // Used for potential optimizations
  int total_region_counter;
  int identical_region_counter;
 private:
	HBITMAP			_entire_window_last;
  HBITMAP			_entire_window_cur;
};

extern CScraper *p_scraper;

#endif // INC_CSCRAPER_H


