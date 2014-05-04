#include "stdafx.h"
#include "COHScriptList.h"

#include "CardFunctions.h"
#include "CParseErrors.h"
#include "CSymbolEngineCards.h"
#include "CSymbolEnginePokerval.h"
#include "NumericalFunctions.h"

COHScriptList::COHScriptList(CString *new_name, CString *new_function_text) {
  COHScriptObject(new_name, new_function_text);
}

COHScriptList::~COHScriptList() {
}

void COHScriptList::Clear() {
  for (int i=0; i<k_number_of_ranks_per_deck; ++i) {
    for (int j=0; j<k_number_of_ranks_per_deck; ++j) {
      _handlist_matrix[i][j] = false;
    }
  }
}

void COHScriptList::Set(int first_rank, int second_rank, bool suited) {
  AssertRange(first_rank, 2, k_rank_ace);
  AssertRange(second_rank, 2, k_rank_ace);
  // Higher rank first
  if (first_rank < second_rank) {
    SwapInts(&first_rank, &second_rank);
  }
  // Normating to [0..12]
  int x = first_rank - 2;
  int y = second_rank - 2;
  if (suited) {
    _handlist_matrix[x][y] = true;
  } else {
    _handlist_matrix[y][x] = true;
  }
}

bool COHScriptList::IsOnList(int first_rank, int second_rank, bool suited) {
  AssertRange(first_rank, 2, k_rank_ace);
  AssertRange(second_rank, 2, k_rank_ace);
  // Higher rank first
  if (first_rank < second_rank) {
    SwapInts(&first_rank, &second_rank);
  }
  // Normating to [0..12]
  int x = first_rank - 2;
  int y = second_rank - 2;
  if (suited) {
    return _handlist_matrix[x][y];
  } else {
    return _handlist_matrix[y][x];
  }
}

void COHScriptList::Set(CString list_member) {
  int length = list_member.GetLength();
  if ((length < 2) || (length > 3)) {
    ErrorInvalidMember(list_member);
    return;
  }
  char first_rank_string = tolower(list_member.GetAt(0));
  char second_rank_string = tolower(list_member.GetAt(1));
  int first_rank = CardRankCharacterToCardRank(first_rank_string);
  int second_rank = CardRankCharacterToCardRank(second_rank_string);
  if ((first_rank < 0) || (second_rank < 0)) {
    ErrorInvalidMember(list_member);
    return;
  }
  if (length == 2) {
    if (first_rank_string == second_rank_string) {
      Set(first_rank, second_rank, false);
      return;
    }
    ErrorOldStyleFormat(list_member);
    return;
  }
  assert(length == 3);
  char suit = tolower(list_member.GetAt(2));
  if (suit == 's') {
    Set(first_rank, second_rank, true);
  } else if (suit == 'o') {
    Set(first_rank, second_rank, false);
  } else {
    ErrorInvalidMember(list_member);
  }
}

void COHScriptList::ErrorInvalidMember(CString list_member) {
  CString error_message;
  error_message.Format("Invalid list member %s\n"
    "Valid are entries like \"AA AKs ATo 22",
    list_member);
  CParseErrors::Error(error_message);
}

void COHScriptList::ErrorOldStyleFormat(CString list_member) {
  CString error_message;
  error_message.Format("Old style list format detected: %s\n"
    "Use AKs for suited hands, AKo for off-suited, AKs AKo for both.",
    list_member);
  CParseErrors::Error(error_message);
}

double COHScriptList::Evaluate() {
  write_log(true, "[COHScriptList] Evaluating list %s\n", _name); 
  return IsOnList(p_symbol_engine_pokerval->rankhiplayer(),
    p_symbol_engine_pokerval->rankloplayer(),
    p_symbol_engine_cards->issuited());
}