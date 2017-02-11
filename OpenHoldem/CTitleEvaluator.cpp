//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Evaluating titletext (ttlimits)
//   and the craped c0limitsX
//
//******************************************************************************

#include "stdafx.h"
#include "CTitleEvaluator.h"

#include "CPreferences.h"
#include "CScraper.h"
#include "CTableTitle.h"
#include "..\StringFunctionsDLL\string_functions.h"

//!!!!!
#include "..\CTransform\CTransform.h"

CTableEvaluator *p_title_evaluator = NULL;

CTableEvaluator::CTableEvaluator() {
}

CTableEvaluator::~CTableEvaluator() {
}

void CTableEvaluator::ClearAllDataOncePerHeartbeat() {
  _sblind = kUndefined;
  _bblind = kUndefined;
  _bbet = kUndefined;
  _ante = kUndefined;
  _sb_bb = kUndefined;
  _bb_BB = kUndefined;
  _buyin = kUndefined;
  _limit = kUndefined;
}

void CTableEvaluator::EvaluateTitleText() {
  CString titletext = p_table_title->PreprocessedTitle();
  CString title_format = p_tablemap->GetTMSymbol("ttlimits");
  ProcessTitle(titletext, title_format);
  // Now try alternative ttlimitsX
  for (int i = 0; i < k_max_number_of_titletexts; i++) {
    title_format.Format("ttlimits%d", i);
    ProcessTitle(titletext, title_format);
  }
}

void CTableEvaluator::EvaluateScrapedTitleTexts() {
  EvaluateC0LimitsX("c0limits");
  for (int i = 0; i < k_max_number_of_titletexts; i++) {
    CString c0limitsX;
    c0limitsX.Format("c0limits%d", i);
    EvaluateC0LimitsX(c0limitsX);
  }
}

void CTableEvaluator::EvaluateC0LimitsX(CString c0limitsX) {
  // Lookup s$c0limitsX
  CString title_format = p_tablemap->GetTMSymbol(c0limitsX);
  if (title_format == "") {
    // Not supported by tablemap
    return;
  }
  // Scrape the region r$c0limitsX from the table
  CString scraped_title;
  if (p_scraper->EvaluateRegion(c0limitsX, &scraped_title)) {
    ProcessTitle(scraped_title, title_format);
  }
}

// Former ParseStringBSL()
// For OpenHoldem 7.3.1 we removed the superfluous parameters
// bool *found_handnumber=NULL, bool *found_sblind=NULL, bool *found_bblind=NULL,
// bool *found_bbet=NULL, bool *found_ante=NULL, bool *found_limit=NULL,
// bool *found_sb_bb=NULL, bool *found_bb_BB=NULL
// to get a cleanner interface and cleaner code.
// The other in-out-parameters must be initialized with kUndefined now
// (or with the empty string). This value represents the same information.
bool CTableEvaluator::ProcessTitle(CString title, CString ttlimits_format) {
  CString text = title;
  CString format = ttlimits_format;
  CString *results_for_openscrape;
  CString *handnumber;
  double *sblind;
  double *bblind;
  double *bbet;
  double *ante;
  int    *limit;
  double *sb_bb;
  double *bb_BB;
  double *buyin;

    int     place_in_format = 0, place_in_text = 0;
    CString token = "", skip_str = "", number_type = "";
    double  number = 0.;
    CString temp = "";
    CString throw_away_result_for_OH;
    // Skip on bogus out-pointer
    if (results_for_openscrape == NULL) {
      results_for_openscrape = &throw_away_result_for_OH;
    }
    *results_for_openscrape = "";
    while (place_in_format < format.GetLength() && place_in_text < text.GetLength()) {
      // handnumber
      if (format.Mid(place_in_format, 2) == "^h") {
        place_in_format += 2;
        token = "";
        while (place_in_text < text.GetLength()
          && (isdigit(text[place_in_text]) || (text[place_in_text] == '-'))) {
          if (text[place_in_text] != '-') {
            token += text[place_in_text];
          }
          place_in_text += 1;
        }
        assert(results_for_openscrape != NULL);
        results_for_openscrape->Append("^h (handnumber)\t= " + token + "\r\n");
        if (handnumber != NULL && *handnumber != "") {
          *handnumber = token.GetString();
        }
      }
      // dummy int
      else if (format.Mid(place_in_format, 2) == "^d") {
        place_in_format += 2;
        token = "";
        while (place_in_text<text.GetLength()
          && (CString(text[place_in_text]).FindOneOf("$0123456789,¢km") != -1)) {
          token += (text[place_in_text]);
          place_in_text++;
        }
        assert(results_for_openscrape != NULL);
        results_for_openscrape->Append("^d (dummy int)\t= " + token + "\r\n");
      }
      // dummy float
      else if (format.Mid(place_in_format, 2) == "^f") {
        place_in_format += 2;
        token = "";
        while (CString(text[place_in_text]).FindOneOf("$0123456789,.¢ckm") != -1 && place_in_text<text.GetLength()) {
          token += text[place_in_text];
          place_in_text++;
        }
        assert(results_for_openscrape != NULL);
        results_for_openscrape->Append("^f (dummy float)\t= " + token + "\r\n");
      }
      // numbers
      else if (format.Mid(place_in_format, 2) == "^s" ||
        format.Mid(place_in_format, 2) == "^b" ||
        format.Mid(place_in_format, 2) == "^B" ||
        format.Mid(place_in_format, 2) == "^a" ||
        format.Mid(place_in_format, 2) == "^y" ||
        format.Mid(place_in_format, 2) == "^v" ||
        format.Mid(place_in_format, 2) == "^V") {
        number_type = format.Mid(place_in_format, 2);
        place_in_format += 2;
        token = "";
        while (CString(text[place_in_text]).FindOneOf("$0123456789,.¢ckm") != -1 && place_in_text<text.GetLength()) {
          token += text[place_in_text];
          place_in_text++;
        }
        CTransform t;
        number = t.StringToMoney(token);
        temp = Number2CString(number);
        if (number_type == "^s") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^s (sblind)\t= " + temp + "\r\n");
          if (sblind != NULL && *sblind == kUndefined) {
            *sblind = number;
          }
        }
        else if (number_type == "^b") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^b (bblind)\t= " + temp + "\r\n");
          if (bblind != NULL && *bblind == kUndefined) {
            *bblind = number;
          }
        }
        else if (number_type == "^B") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^B (bbet)\t= " + temp + "\r\n");
          if (bbet != NULL && *bbet == kUndefined) {
            *bbet = number;
          }
        }
        else if (number_type == "^a") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^a (ante)\t= " + temp + "\r\n");
          if (ante != NULL && *ante == kUndefined) {
            *ante = number;
          }
        }
        else if (number_type == "^v") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^v (sb_bb)\t= " + temp + "\r\n");
          if (sb_bb != NULL && *sb_bb == kUndefined) {
            *sb_bb = number;
          }
        }
        else if (number_type == "^V") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^V (bb_BB)\t= " + temp + "\r\n");
          if (bb_BB != NULL && *bb_BB == kUndefined) {
            *bb_BB = number;
          }
        }
        else if (number_type == "^y") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^y (buyin)\t= " + temp + "\r\n");
          if (buyin != NULL && *buyin == kUndefined) {
            *buyin = number;
          }
        }
      }
      // Skip all chars up to string following the ^*
      else if (format.Mid(place_in_format, 2) == "^*") {
        place_in_format += 2;
        skip_str = "";
        token = "";
        // Get the string that we want to match up to
        while (format[place_in_format] != '^' && place_in_format < format.GetLength()) {
          skip_str += format[place_in_format];
          place_in_format += 1;
        }
        while (text.Mid(place_in_text, skip_str.GetLength()) != skip_str &&
          place_in_text < text.GetLength()) {
          token += text[place_in_text];
          place_in_text += 1;
        }
        if (text.Mid(place_in_text, skip_str.GetLength()) == skip_str) {
          token.Append(text.Mid(place_in_text, skip_str.GetLength()));
          place_in_text += skip_str.GetLength();
        }
        assert(results_for_openscrape != NULL);
        results_for_openscrape->Append("^* (skip string)\t= '" + token + "'\r\n");
      }
      // Roman numerals
      else if (format.Mid(place_in_format, 2) == "^R") {
        place_in_format += 2;
        token = "";
        // Get roman numeral
        while ((tolower(text[place_in_text]) == 'i'
          || tolower(text[place_in_text]) == 'v'
          || tolower(text[place_in_text]) == 'x'
          || tolower(text[place_in_text]) == 'l'
          || tolower(text[place_in_text]) == 'c')
          && place_in_text < text.GetLength()) {
          token += (unsigned char)tolower(text[place_in_text]);
          place_in_text += 1;
        }
        // This is commented out for now, because, strangely, WH does not provide a
        // place to report this value to the user
        // convert roman numeral to number
        //if (token == "i") {
        //      level = 1;
        //}
        //if (token == "ii") {
        //      level = 2;
        //}
        assert(results_for_openscrape != NULL);
        results_for_openscrape->Append("^R (roman numeral)\t= " + token + "\r\n");
      }
      // Limit, no-limit, pot limit identifier
      else if (format.Mid(place_in_format, 2) == "^L") {
        place_in_format += 2;
        if (text.Mid(place_in_text, 8).MakeLower() == "no limit"
          || text.Mid(place_in_text, 8).MakeLower() == "no-limit") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^L (limit type)\t= " + text.Mid(place_in_text, 8) + "\r\n");
          if (limit != NULL && *limit == kUndefined) {
            *limit = kGametypeNL;
          }
          place_in_text += 8;
        }
        else if (text.Mid(place_in_text, 7).MakeLower() == "nolimit") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^L (limit type)\t= " + text.Mid(place_in_text, 7) + "\r\n");
          if (limit != NULL && *limit == kUndefined) {
            *limit = kGametypeNL;
          }
          place_in_text += 7;
        }
        else if (text.Mid(place_in_text, 2).MakeLower() == "nl") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^L (limit type)\t= " + text.Mid(place_in_text, 2) + "\r\n");
          if (limit != NULL && *limit == kUndefined) {
            *limit = kGametypeNL;
          }
          place_in_text += 2;
        }
        else if (text.Mid(place_in_text, 9).MakeLower() == "pot limit"
          || text.Mid(place_in_text, 9).MakeLower() == "pot-limit") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^L (limit type)\t= " + text.Mid(place_in_text, 9) + "\r\n");
          if (limit != NULL && *limit == kUndefined) {
            *limit = kGametypePL;
          }
          place_in_text += 9;
        }
        else if (text.Mid(place_in_text, 8).MakeLower() == "potlimit") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^L (limit type)\t= " + text.Mid(place_in_text, 8) + "\r\n");
          if (limit != NULL && *limit == kUndefined) {
            *limit = kGametypePL;
          }
          place_in_text += 8;
        }
        else if (text.Mid(place_in_text, 2).MakeLower() == "pl") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^L (limit type)\t= " + text.Mid(place_in_text, 2) + "\r\n");
          if (limit != NULL && *limit == kUndefined) {
            *limit = kGametypePL;
          }
          place_in_text += 2;
        }
        else if (text.Mid(place_in_text, 11).MakeLower() == "fixed limit"
          || text.Mid(place_in_text, 11).MakeLower() == "fixed-limit") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^L (limit type)\t= " + text.Mid(place_in_text, 11) + "\r\n");
          if (limit != NULL && *limit == kUndefined) {
            *limit = kGametypeFL;
          }
          place_in_text += 11;
        }
        else if (text.Mid(place_in_text, 10).MakeLower() == "fixedlimit") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^L (limit type)\t= " + text.Mid(place_in_text, 10) + "\r\n");
          if (limit != NULL && *limit == kUndefined) {
            *limit = kGametypeFL;
          }
          place_in_text += 10;
        }
        else if (text.Mid(place_in_text, 2).MakeLower() == "fl") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^L (limit type)\t= " + text.Mid(place_in_text, 2) + "\r\n");
          if (limit != NULL && *limit == kUndefined) {
            *limit = kGametypeFL;
          }
          place_in_text += 2;
        }
        else if (text.Mid(place_in_text, 5).MakeLower() == "limit") {
          assert(results_for_openscrape != NULL);
          results_for_openscrape->Append("^L (limit type)\t= " + text.Mid(place_in_text, 5) + "\r\n");
          if (limit != NULL && *limit == kUndefined) {
            *limit = kGametypeFL;
          }
          place_in_text += 5;
        }
      }
      // All other exact matches
      else if (text[place_in_text] == format[place_in_format]) {
        assert(results_for_openscrape != NULL);
        results_for_openscrape->Append("exact match\t= '");
        while (text[place_in_text] == format[place_in_format] && place_in_text<text.GetLength() && place_in_format<format.GetLength()) {
          assert(results_for_openscrape != NULL);
          *results_for_openscrape += (text[place_in_text]);
          place_in_text += 1;
          place_in_format += 1;
        }
        assert(results_for_openscrape != NULL);
        results_for_openscrape->Append("'\r\n");
      }
      // Match failed, exit loop
      else {
        place_in_format = format.GetLength() + 1;
        place_in_text = text.GetLength() + 1;
      }
    }
#ifdef OPENHOLDEM_PROGRAM
    write_log(preferences.debug_scraper(),
      "[CTransform] parsed title string\n");
    write_log(preferences.debug_scraper(),
      "[CTransform] text = %s, format = %s\n",
      text, format);
    write_log(preferences.debug_scraper(),
      "[CTransform] handnumber = %s, sblind = %f, bblind = %f\n",
      *handnumber, *sblind, *bblind);
    write_log(preferences.debug_scraper(),
      "[CTransform] bbet = %f, ante = %f, limit = %d, sb_bb = %f, bb_BB = %f\n",
      *bbet, *ante, *limit, *sb_bb, *bb_BB);
    write_log(preferences.debug_scraper(),
      "[CTransform] buyin = %d\n",
      *buyin);
#endif
  }
