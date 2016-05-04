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

#include "stdafx.h"
#include "CTablemapCompletenessChecker.h"

#include "CSessionCounter.h"
#include "../CTablemap/CTablemap.h"
#include "OH_MessageBox.h"

CTablemapCompletenessChecker::CTablemapCompletenessChecker() {
}

CTablemapCompletenessChecker::~CTablemapCompletenessChecker() {
}

void CTablemapCompletenessChecker::ErrorMissingItem(CString item) {
  CString message;
  message.Format("Missing item in tablemap: %s.\n"
    "This item is absolutely necessary for correct execution.\n"
    "%s", 
    item, p_tablemap->filepath());
  OH_MessageBox_Interactive(message, "Error", 0);
}

void CTablemapCompletenessChecker::ErrorDeprecatedItem(CString item) {
  CString message;
  message.Format("Deprecated item in tablemap: %s.\n"
    "You can safely delete this item from the tablemap.\n"
    "Due to a simplification of OpenHoldem it is no longer required.\n"
    "The reasons and potential consequences are documented in the release-notes.\n"
    "\n"
    "%s", 
    item, p_tablemap->filepath());
  OH_MessageBox_Interactive(message, "Error", 0);
}

void CTablemapCompletenessChecker::ErrorSwagRenamed() {
  CString message;
  message.Format("\"Swagging\" has been removed from OpenHoldem\n"
    "The following tablemap-symbols got renamed:\n"
    "\n"
    "  * swagselectionmethod -> betsizeselectionmethod\n"
    "  * swagdeletionmethodmethod -> betsizedeletionmethodmethod\n"
    "  * swagconfirmationmethod -> betsizeconfirmationmethod\n"
    "  * swagtextmethod -> betsizeinterpretationmethod\n"
    "\n"
    "%s", 
    p_tablemap->filepath());
  OH_MessageBox_Interactive(message, "Error", 0);
}

void CTablemapCompletenessChecker::ErrorClientsizeReplaced() {
  CString message = "z$clientsize got replaced in favour\n"
    "of a more flexible and fail-safe approach.\n"
    "It needs to be replaced by\n"
    "  * z$clientsizemin\n"
    "  * z$clientsizemax\n"
    "  * z$targetsize\n";
  OH_MessageBox_Interactive(message, "Error", 0);
}

void CTablemapCompletenessChecker::CheckItem(CString item) {
  if (!p_tablemap->ItemExists(item)) {
    ErrorMissingItem(item);
  }
}

void CTablemapCompletenessChecker::CheckSetOfItems(CString prefix, 
                                                   int last_index, 
                                                   CString postfix, 
                                                   bool mandatory) {
  // All other items are also mandatory
  // if the first optional item exists                                                   
  CString first_optional_item;
  first_optional_item.Format("%s%i%s", prefix, 0, postfix);
  if (p_tablemap->ItemExists(first_optional_item)) {
    mandatory = true;
  }
  if (!mandatory) return;
  for (int i=0; i<=last_index; ++i) {
    CheckItem(prefix, i, postfix);
  }
}

void CTablemapCompletenessChecker::CheckItem(CString prefix, int infix, CString postfix) {
  CString name;
  name.Format("%s%d%s", prefix, infix, postfix);
  CheckItem(name);
}

bool CTablemapCompletenessChecker::IsNoLimitMap() {
  return (p_tablemap->ItemExists("i3edit")
    || p_tablemap->ItemExists("i3slider")
    || p_tablemap->ItemExists("betpotmethod")
    || p_tablemap->ItemExists("betsizedeletionmethod")
    || p_tablemap->ItemExists("betsizeinterpretationmethod")
    || p_tablemap->ItemExists("betsizeconfirmationmethod"));
}

void CTablemapCompletenessChecker::CheckBetsOrChips() {
  bool tm_uses_stacks = p_tablemap->ItemExists("p0chip00");
  int nchairs = p_tablemap->nchairs();
  for (int i=0; i<nchairs; ++i) {
    if (tm_uses_stacks) {
      CheckItem("p", i, "chip00");
      CheckItem("p", i, "chip01");
      CheckItem("p", i, "chip10");
    } else {
      CheckItem("p", i, "bet");
    }
  }
}

void CTablemapCompletenessChecker::CheckCardFaces(CString prefix, int infix, CString postfix) {
  CString name;
  name.Format("%s%d%s", prefix, infix, postfix);
  CString rank_name = name + "rank";
  if (p_tablemap->ItemExists(rank_name)) {
    // If a rank exists then the suit also needs to get scraped
    CString suit_name = name + "suit";
    CheckItem(suit_name);
  } else {
    // Default: cardface must be there
    CheckItem(name);
  }
}

void CTablemapCompletenessChecker::CheckMainPot() {
  if (p_tablemap->ItemExists("c0pot0chip00")) {
     CheckItem("c0pot0chip01");
     CheckItem("c0pot0chip10");
  } else {
    CheckItem("c0pot0");
  }
}

void CTablemapCompletenessChecker::VerifySingleC0limitsItem(CString name) {
  // Each optional r$c0limitsX-region must have 
  // a corresponding s£c0limitsX-symbol,
  // otherwise OpenHoldem can crash on evaluation
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=110&t=18865
  if (!p_tablemap->ItemExists(name)) return;
  SMapCI sit = p_tablemap->s$()->find(name); 
  if (sit == p_tablemap->s$()->end()) {
    CString missing_item = "s$" + name;
    ErrorMissingItem(missing_item);
  }
}

void CTablemapCompletenessChecker::VerifyC0limits() {
  VerifySingleC0limitsItem("c0limits");
  for (int i=0; i<9; ++i) {
    CString c0limitsX;
    c0limitsX.Format("c0limits%i", i);
    VerifySingleC0limitsItem(c0limitsX);
  }
}

void CTablemapCompletenessChecker::VerifyMap() {
  // Only session 0 verifies the tablemaps
  // for better performance amd to avoid driving users crazy.
  if (p_sessioncounter->session_id() > 0) return;
  // First check for deprecated (and renamed( items.
  // This avoids confusion if we cmplain about missing renamed symbols later
  CheckForDeprecatedItems();
  // Absoluely mandatory for connection
  CheckItem("titletext");
  // Necessary for connection: clientsizemin/max and z$targetsize (since 9.0.1)
  CheckItem("clientsizemin");
  CheckItem("clientsizemax");
  CheckItem("targetsize");
  // All the rest is only needed for tables, but not for the lobby
  if (p_tablemap->islobby()) return;
  // Basic info, needed by every table
  CheckItem("nchairs");
  CheckItem("network");
  CheckItem("sitename"); 
  CheckItem("ttlimits");
  // Range-check nchairs
  int nchairs = p_tablemap->nchairs();
  int last_chair = nchairs - 1;
  if ((nchairs < 2) || (nchairs > kMaxNumberOfPlayers)) {
    CString message;
    message.Format("Tablemap item nchairs out of range\n"
      "Correct values: 2..10\n"
      "%s",
      p_tablemap->filepath());
    OH_MessageBox_Interactive(message, "Error", 0);
  }
  // Check mandatory items for every seat
  CheckSetOfItems("p", last_chair, "active",   true);  
  CheckSetOfItems("p", last_chair, "balance",  true);
  CheckSetOfItems("p", last_chair, "dealer",   true);
  CheckSetOfItems("p", last_chair, "name",     true);
  CheckSetOfItems("p", last_chair, "seated",   true);
  CheckSetOfItems("p", last_chair, "cardback", true);
  CheckSetOfItems("p", last_chair, "cardface0nocard", true);
  CheckSetOfItems("p", last_chair, "cardface1nocard", true);
  // Check mandatory cards faces (or rank + suit) for every seat
  for (int i=0; i<nchairs; ++i) {
    CheckCardFaces("p", i, "cardface0");
    CheckCardFaces("p", i, "cardface1");
  }
  CheckBetsOrChips();
  // Community cards
  for (int i=0; i<kNumberOfCommunityCards; ++i) {
    CheckCardFaces("c0cardface", i, "");
  }
  int last_communitz_card = kNumberOfCommunityCards - 1;
  CheckSetOfItems("c0cardface", last_communitz_card, "nocard", true);
  CheckMainPot();
  // Action buttons
  int number_of_buttons_seen = 0;
  for (int i=0; i<k_max_number_of_buttons; ++i) {
    CString button;
    button.Format("i%dbutton", i);
    if (p_tablemap->ItemExists(button)) {
      ++number_of_buttons_seen;
      CheckItem("i", i, "state");
      CheckItem("i", i, "label");
    }
  }
  if (number_of_buttons_seen < 3) {
    CString message;
    message.Format(
      "A tablemap needs at least 3 buttons:\n" 
      "  Fold, Check/Call, Bet/Raise\n"
      "At least one of them is missing.\n"
      "%s",
      p_tablemap->filepath());
    OH_MessageBox_Interactive(message, "Error", 0);
  }
  // No Limit only
  if (IsNoLimitMap()) {
    CheckItem("betsizedeletionmethod");
    CheckItem("betsizeinterpretationmethod");
    CheckItem("betsizeconfirmationmethod");
    CheckItem("i3edit");
    // i3edit ("betsizing") needs a betsizeconfirmation-button,
    // which might be something different than min-raise-button.
    CheckItem("i3button");
    CheckItem("i3state");
    CheckItem("i3label");
    // Slider now optional, as allinmethod (= 3) is gone.
    // CheckItem("i3slider");
    // CheckItem("i3handle");
  }
  // Font-types
  for (int i=0; i<k_max_number_of_font_groups_in_tablemap; ++i) {
    if (p_tablemap->FontGroupInUse(i)) {
      CheckItem("t", i, "type");
    }
  }
  VerifyC0limits();
  // Optional uX-regions
  CheckSetOfItems("u", last_chair, "name",    false);
  CheckSetOfItems("u", last_chair, "seated",  false);
  CheckSetOfItems("u", last_chair, "active",  false);
  CheckSetOfItems("u", last_chair, "balance", false);
  CheckSetOfItems("u", last_chair, "bet",     false);
  CheckSetOfItems("u", last_chair, "dealer",  false);
  // Not checked, despite nice to have
  //   * betpot-buttons
  //   * i86X-buttons
  //
  // Not necessary, as optional
  //   * r$c0isfinaltable
  //   * potmethod
  //   * buttonclickmethod
  //   * betpotmethod
  // and some others  
}

void CTablemapCompletenessChecker::CheckForDeprecatedItems() {
  // r$c0istournament no longer supported, works automatically
  if (p_tablemap->ItemExists("c0istournament")) {
    ErrorDeprecatedItem("c0istournament");
  }
  // handresetmethod no longer supported, works automatically
  if (p_tablemap->ItemExists("handresetmethod")) {
    ErrorDeprecatedItem("handresetmethod");
  }
  // swag cofiguration symbols got renamed
  if (p_tablemap->ItemExists("swagselectionmethod")
      || p_tablemap->ItemExists("swagdeletionmethodmethod")
      || p_tablemap->ItemExists("swagconfirmationmethod")
      || p_tablemap->ItemExists("swagtextmethod")) {
    ErrorSwagRenamed();
  }
  // activemethod no longer upported since OpenHoldem 7.3.1
  // since it was based on backward-compatibilitz for a bug
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=117&t=17818&start=30#p125307
  if (p_tablemap->ItemExists("activemethod")) {
    ErrorDeprecatedItem("activemethod");
  }
  // allinmethod no longer supported, works automatically
  if (p_tablemap->ItemExists("allinmethod")) {
    ErrorDeprecatedItem("allinmethod");
  }
  // clientsize removed in favour of clientsizemin/max and targetzize
  if (p_tablemap->ItemExists("clientsize")) {
    ErrorClientsizeReplaced();
  }
}