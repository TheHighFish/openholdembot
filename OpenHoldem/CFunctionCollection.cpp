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

#include "stdafx.h"
#include "CFunctionCollection.h"

#include "CAutoplayerTrace.h"
#include "CFormulaParser.h"
#include "CFunction.h"
#include "CParseErrors.h"
#include "CPreferences.h"
#include "CSelftestParserEvaluator.h"
#include "MagicNumbers.h"
#include "OH_MessageBox.h"

CFunctionCollection *p_function_collection = NULL;

CFunctionCollection::CFunctionCollection(){
  _title = "";
  _path = "";
  _openPPL_library_correctly_loaded = false;
  DeleteAll(true, true);
}

CFunctionCollection::~CFunctionCollection() {
  DeleteAll(true, true);
  assert(_function_map.size() == 0);
}

void CFunctionCollection::DeleteAll(bool open_ppl, bool user_defined) {
  write_log(preferences.debug_formula(), 
    "[CFunctionCollection] DeleteAll()\n");
  if (!_openPPL_library_correctly_loaded) {
    open_ppl = true;
  }
  COHScriptObject *p_nextObject = GetFirst();
  while (p_nextObject != NULL) {
    bool needs_deletion = false;
    if (open_ppl && p_nextObject->IsOpenPPLSymbol()) {
      needs_deletion = true;
    } else if (user_defined && !p_nextObject->IsOpenPPLSymbol()) {
      needs_deletion = true;
    }
    if (needs_deletion) {
      write_log(preferences.debug_formula(), 
        "[CFunctionCollection] Deleting %s\n", p_nextObject->name());
      Delete(p_nextObject->name());
    }
    p_nextObject = GetNext();
  }
}

bool CFunctionCollection::CheckForOutdatedFunction(CString name) {
  if (name  == "f$play") {
    CParseErrors::Error("f$play got replaced\n"
      "by f$sitin, f$sitout, f$leave.\n");
    return true;
  }
  if (name  == "f$P") {
    CParseErrors::Error("f$P got replaced\n"
      "by f$prwin_number_of_opponents.\n");
    return true;
  }
  if (name  == "f$swag") {
    CParseErrors::Error("f$swag and its supporting functions got replaced\n"
      "by a single function f$betsize (raise-to-semantics)\n"
      "and OpenHoldem automatically cares about casino-specific adjustments\n"
      "if you provide the casinos swagtextmethod in the tablemap.\n");
    return true;
  } 
  if (name.Left(9) == "f$OpenPPL") {
    CParseErrors::Error("This is old-style translated OpenPPL.\n"
      "OpenHoldem supports now plain-text OpenPPL.\n"
      "The old technology is no longer compatible.\n"
      "Please have a look at the latest OpenPPL-manual.\n"
      "Things became far more easy.\n");
	  return true;
  }
  return false;
}

bool CFunctionCollection::CheckForMisspelledOpenPPLMainFunction(CString name) {
  // OpenPPL main-functions are the only ones 
  // that have to be created by the user and get only called by OH
  // and nowhere got checked.
  // Looks like potential for PEBKAC, therefore we check that here.
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=297&t=17891
  CString name_lowercases = name;
  name_lowercases.MakeLower();
  if (name == name_lowercases) {
    // Can't be misspelled
    return false;
  }
  for (int i=kBetroundPreflop; i<kBetroundRiver; ++i) {
    if (name_lowercases == k_OpenPPL_function_names[i]) {
      CParseErrors::Error("Misspelled OpenPPL main-function?\n"
        "f$preflop, f$flop, f$turn and f$river are expected to be all lower-cases.\n");
      return true;
    }
  }
  return false;
}

void CFunctionCollection::Add(COHScriptObject *new_function) {
  CSLock lock(m_critsec);
  CString name = new_function->name();
  if (name == "") {
    write_log(preferences.debug_formula(), 
	    "[CFunctionCollection] Invalid empty name\n");
    return;
  }
  if (Exists(name)) {
    write_log(preferences.debug_formula(), 
	    "[CFunctionCollection] Name %s already exists. Deleting it\n", name);
    _function_map.erase(name);
  }
  if (CheckForOutdatedFunction(name) || CheckForMisspelledOpenPPLMainFunction(name)) {
    // Ignore it
    write_log(preferences.debug_formula(), 
      "[CFunctionCollection] Ignoring bad function %s\n", name);
    return;
  }

  write_log(preferences.debug_formula(), 
	  "[CFunctionCollection] Adding %s -> %i\n", name, new_function);
  _function_map[name] = new_function;
}

bool CFunctionCollection::Exists(CString name) {
  CSLock lock(m_critsec);
  std::map<CString, COHScriptObject*>::iterator it; 
  it = _function_map.find(name); 
  return (it != _function_map.end());
}

// Generates smart error-messages on failure
// To be used by the parser
void CFunctionCollection::VerifyExistence(CString name) {
  if (Exists(name)) return;
  // Error: function does not exist
  CString message;
  message.Format("Function used but never defined> %s\n\n", name);
  CString similar_name = GetSimilarNameWithDifferentCases(name);
  name = name.MakeLower();
  if (similar_name != "") {
     message += "Did you mean ";
     message += similar_name;
     message += " instead?";
  } else if (name.Left(5) == "hand$" || name.Left(6) == "board$" || name.Left(4) == "user") {
    message += "hand$ and board$ expressions and user-variables\n";
    message += "are built-in and must be lower-cases.";
  } else {
    // Else: general error-message
  }
  OH_MessageBox_Interactive(message, "Error", 0);
}

CString CFunctionCollection::GetSimilarNameWithDifferentCases(CString function_name) {
  function_name.MakeLower();
  COHScriptObject *p_nextObject = GetFirst();
  while (p_nextObject != NULL) {
    if (function_name == p_nextObject->name().MakeLower()) {
      // Found similar name, just different cases
      return p_nextObject->name();
    }
    p_nextObject = GetNext();
  }
  // Nothing found
  return NULL;
}

COHScriptObject *CFunctionCollection::LookUp(CString name) {
  CSLock lock(m_critsec);
  write_log(preferences.debug_formula(), "[CFunctionCollection] Lookup %s\n", name); 
  std::map<CString, COHScriptObject*>::iterator it; 
  it = _function_map.find(name); 
  if (it == _function_map.end()) {
    // Function or list does not exist
    write_log(preferences.debug_formula(), "[CFunctionCollection] Function does not exist\n");
    return NULL;
  }
  return it->second;
}

double CFunctionCollection::Evaluate(CString function_name, bool log /* = false */) {
  CSLock lock(m_critsec);
  double result = kUndefined;
  EvaluateSymbol(function_name, &result, log);
  return result;
}

double CFunctionCollection::EvaluateAutoplayerFunction(int function_code) {
  CString function_name = k_standard_function_names[function_code];
  assert(function_name != "");
  return Evaluate(function_name, true);
}

bool CFunctionCollection::EvaluatesToBinaryNumber(CString function_name) {
  COHScriptObject *p_function = LookUp(function_name);
  if (p_function == NULL) return false;
  return p_function->EvaluatesToBinaryNumber();
}

CString CFunctionCollection::DLLPath() {
  CSLock lock(m_critsec);
  // First try upper-cases
  COHScriptObject *dll_node = LookUp("DLL");
  if (dll_node == NULL) {
    // If not found try lower-cases
    dll_node = LookUp("dll");
  }
  if (dll_node == NULL) {
	  return "";
  }
  CString dll_path = dll_node->function_text();
  dll_path.Trim();
  return dll_path;
}

void CFunctionCollection::SetEmptyDefaultBot() {
  write_log(preferences.debug_formula(), 
    "[CFunctionCollection] SetEmptyDefaultBot()\n");
  CSLock lock(m_critsec);
  DeleteAll(false, true);
  _title = "NoName";
  // Adding empty standard-functions
  // http://www.maxinmontreal.com/forums/viewtopic.php?f=156&t=16230
  CheckForDefaultFormulaEntries();
  // After setting the bot we should parse it so that OH can evaluate and act
  ParseAll();
  // Parser, function-collection and library initialized 
  // and a ParseAll to come:
  // this is the perfect place to execute the selftest. 
  ExecuteSelftest();
}

void CFunctionCollection::ExecuteSelftest() {
  write_log(preferences.debug_formula(), 
    "[CFunctionCollection] Executing self-test\n");
  CString name = kSelftestName;
  CString function_text = kSelftestFunction;
  CFunction *p_function = new CFunction(&name, 
    &function_text, kNoAbsoluteLineNumberExists); 
  Add((COHScriptObject *)p_function);
  p_function->Parse();
  CSelftestParserEvaluator selftest;
  selftest.Test();
}

void CFunctionCollection::CheckForDefaultFormulaEntries() {
  write_log(preferences.debug_formula(), 
    "[CFunctionCollection] CheckForDefaultFormulaEntries(\n");
  CSLock lock(m_critsec);
  // Header comment
  CreateEmptyDefaultFunctionIfFunctionDoesNotExist(CString("notes"));
  // DLL to be loaded
  CreateEmptyDefaultFunctionIfFunctionDoesNotExist(CString("dll"));
  // OpenPPL-functions
  if (IsOpenPPLProfile()) {
    for (int i=kBetroundPreflop; i<=kBetroundRiver; ++i) {
      CString function_name = k_OpenPPL_function_names[i];
      CreateEmptyDefaultFunctionIfFunctionDoesNotExist(function_name);
    }
  } 
  // Autoplayer-functions
  // We need these functions always,
  // for OH-script naturally and for OpenPPL technically,
  // when we translate the decision to actions
  for (int i=k_autoplayer_function_beep; i<=k_autoplayer_function_fold; ++i) {
	  CreateEmptyDefaultFunctionIfFunctionDoesNotExist(CString(k_standard_function_names[i]));
  }
  // hopper,standard, ini and PrWin functions
  for (int i=k_hopper_function_sitin; i<=k_icm_prize9; ++i) {
	  CreateEmptyDefaultFunctionIfFunctionDoesNotExist(CString(k_standard_function_names[i]));
  }
  // Debug functions	
  CreateEmptyDefaultFunctionIfFunctionDoesNotExist(CString("f$test"));
  CreateEmptyDefaultFunctionIfFunctionDoesNotExist(CString("f$debug"));
  // PrWin-functions, used by the GUI
  CreateEmptyDefaultFunctionIfFunctionDoesNotExist(
    CString(k_standard_function_names[k_prwin_number_of_iterations]));
}

void CFunctionCollection::SetAutoplayerFunctionValue(int function_code, double value) {
  CString function_name = k_standard_function_names[function_code];
  assert(function_name != "");
  CFunction *p_function = (CFunction *)LookUp(function_name);
  if (p_function == NULL) return;
  p_function->SetValue(value);
}

void CFunctionCollection::CreateEmptyDefaultFunctionIfFunctionDoesNotExist(CString &function_name) {
  if (Exists(function_name)) {
    write_log(preferences.debug_formula(), 
      "[CFunctionCollection] Function already exists : %s\n", function_name);
	  return;
  }
  // Formula not found.
  // Add the standard one.
  CString function_text;
  if (function_name == k_standard_function_names[k_autoplayer_function_betsize]) {
    function_text = 
      "// Betsize in dollars, raise-to semantics.\n"
      "// OpenHoldem will auto-adapt the betsize to the casinos input-requirements.\n"
      "// Please define your tablemaps \"betsizeinterpretationmethod\"\n"
      "// to configure this feature.\n";
  } else if (function_name == k_standard_function_names[k_autoplayer_function_check]) {    
    function_text = 
      "// Check whenever it is free to call\n"
      "(call == 0) "; 
  } else if (function_name == k_standard_function_names[k_autoplayer_function_fold]) {
    function_text = 
      "// f$fold should always evaluate to true per default\n"
      "// for auto-check-folding instead of time-outs.\n"
      "1 ";
  } else if (function_name == k_standard_function_names[k_hopper_function_rebuy]) {
    function_text = 
      "// f$rebuy should evaluate to the target-amount in dollars.\n"
      "// This value will be passed to the rebuy-script as a parameter.\n"
      "0\n";
  } else if (function_name == k_standard_function_names[k_standard_function_delay]) {
    function_text = 
      "// Autoplayer-delay in milli-seconds\n"
      "0\n";
  } else if (function_name == k_standard_function_names[k_standard_function_allin_on_betsize_balance_ratio]) {
    function_text = 
      "// Allin if betsize is more than X% of our total balance (including currentbet).\n"
      "// This affects f$betsize and all f$betpot_X_Y_ functions.\n" 
      "//  0.00 =  0% = disabled\n"
      "//  0.50 = 50%\n"
      "0.00\n";
  } else if (function_name == k_standard_function_names[k_init_on_startup]) {
    function_text = 
      "// Ini-functions are meant for\n"    
      "//   * Initialization of memory-symbols at well-defined times\n"
      "//   * Triggering calculations, e.g. in a DLL\n";
  } else if (function_name == k_standard_function_names[k_prwin_number_of_opponents]) {
    function_text = 
      "// \"Reasonable\" default to get standard PrWin running for beginners.\n"
      "// Adding one extra opponent has two advantages:\n"
      "//   * works even with mis-scraped \"no opponents\"\n"
      "//   * adjusts over-optimistic standard PrWin (random ranges).\n"
      "nopponentsplaying + 1 \n";
  } else if (function_name == k_standard_function_names[k_prwin_number_of_iterations]) {
    function_text = 
      "// \"Reasonable\" default to get PrWin running for beginners.\n"
      "// Large enough to get usable results,\n"
      "// small enough to save CPU-time.\n"
      "1000 ";
  } else if (function_name == k_standard_function_names[k_icm_prize1]) {
    function_text = 
      "// f$icm_prizeX functions are used to configure the payout-structure in tournaments.\n"
      "// The sum of all f$icm_prizeX functions should be 1.00 (= 100%).\n"
      "// Default to get things going: the winner takes it all.\n"
      "1.00\n";
  } else {
    // Add an empty function.
    // The function-text should contain at least one space.
    // The editor does somehow not work for completely empty formulas.
    function_text = " "; 
  }
  write_log(preferences.debug_formula(), 
      "[CFunctionCollection] Adding default function: %s\n", function_name);
  CFunction *p_function = new CFunction(&function_name, 
    &function_text, kNoAbsoluteLineNumberExists); 
  Add((COHScriptObject *)p_function);
  p_function->Parse();
}

COHScriptObject *CFunctionCollection::GetFirst() {
  CSLock lock(m_critsec);
  enumerator_it = _function_map.begin();
  return GetNext();
}

COHScriptObject *CFunctionCollection::GetNext() {
  CSLock lock(m_critsec);
  if (enumerator_it == _function_map.end()) {
    // "end" points behind the last element
    return NULL;
  }
  COHScriptObject *result = enumerator_it->second;
  enumerator_it++;
  return result;
}

void CFunctionCollection::ClearCache() {
  CSLock lock(m_critsec);
  COHScriptObject *p_oh_script_object = GetFirst();
  while (p_oh_script_object != NULL) {
    if (p_oh_script_object->IsFunction() || p_oh_script_object->IsOpenPPLSymbol()) {
      ((CFunction*)p_oh_script_object)->ClearCache();
    }
    p_oh_script_object = GetNext();
  }
}

void CFunctionCollection::Save(CArchive &ar) {
  CSLock lock(m_critsec);
  // First write the date
  char nowtime[26] = {0};
  CString s;
  s.Format("##%s##\r\n\r\n", get_time(nowtime)); 
  ar.WriteString(s);
  // DLL  and notes are a bit special "functions",
  // so they get extra treatment.
  SaveObject(ar, LookUp("notes"));
  SaveObject(ar, LookUp("dll"));
  // Then write the standard formula functions...
  // These are functions and symbols, that
  //   * are essential to control the behaviour 
  //	 of (nearly) every poker bot.
  //   * configure some very important constants.
  for (int i=k_autoplayer_function_allin; 
      i<k_number_of_standard_functions; 
      ++i) {
	  SaveObject(ar, LookUp(k_standard_function_names[i]));
  }
  // f$test and f$debug are again special
  SaveObject(ar, LookUp("f$test"));
  SaveObject(ar, LookUp("f$debug"));
  // Handlists
  COHScriptObject *next_object = GetFirst();
  while (next_object != NULL) {
    if (next_object->IsList()) {
      SaveObject(ar, next_object);
    }
    next_object = GetNext();
  }
  // OpenPPL-functions
  for (int i=kBetroundPreflop; i<=kBetroundRiver; ++i) {
    assert(k_OpenPPL_function_names[i] != "");
    SaveObject(ar, LookUp(k_OpenPPL_function_names[i]));
  }
  // User defined functions
  // We already saved the standard-functions
  next_object = GetFirst();
  while (next_object != NULL) {
    if (next_object->IsUserDefinedFunction()) {
      SaveObject(ar, next_object);
    }
    next_object = GetNext();
  }
}

void CFunctionCollection::SaveObject(
    CArchive &ar, 
    COHScriptObject *function_or_list) {
  CSLock lock(m_critsec);
  if (function_or_list == NULL) return;
  // Don't save OpenPPL-symbols from the OpenPPL-library
  // to user-defined bot-logic files
  if (function_or_list->IsOpenPPLSymbol()) return;
  ar.WriteString(function_or_list->Serialize());
}

void CFunctionCollection::Dump() {
  COHScriptObject *p_oh_script_object = GetFirst();
  while (p_oh_script_object != NULL) {
    p_oh_script_object->Dump();
    p_oh_script_object = GetNext();  
  }
}

bool CFunctionCollection::Rename(CString from_name, CString to_name) {
  CSLock lock(m_critsec);
  COHScriptObject *object_to_rename = LookUp(from_name);
  if (object_to_rename == NULL) return false;
  if (p_function_collection->LookUp(to_name) != NULL) {
    OH_MessageBox_Interactive("Cannot rename to a function/list that already exists", "Error", 0);
    return false;
  }
  // Delete old entry from the binary tree... 
  Delete(from_name);
  // ...then rename...
  object_to_rename->SetName(to_name);
  // ...and insert again.
  Add(object_to_rename);
  return true;
}

void CFunctionCollection::Delete(CString name) {
  CSLock lock(m_critsec);
  COHScriptObject *object_to_delete = LookUp(name);
  if (object_to_delete != NULL) {
    std::map<CString, COHScriptObject*>::iterator it; 
    it = _function_map.find(name);
    if (it != _function_map.end()) {
      write_log(preferences.debug_formula(), 
        "[CFunctionCollection] Deleting %s\n", name);
      _function_map.erase(it);
    }
  }
}

void CFunctionCollection::SetFunctionText(CString name, CString content) {
  CSLock lock(m_critsec);
  COHScriptObject *function = LookUp(name);
  if (function == NULL) {
    // Function does not yet exist; new one
    // We need to create name and text on the heap, can't point to the stack
    CString *my_text = new CString(content);
    CString *my_name = new CString(name);
    function = new CFunction(my_name, my_text, kUndefinedZero);
    Add(function);
  } else {
    write_log(preferences.debug_formula(), 
      "[CFunctionCollection] Setting function text for %s\n", name);
    function->SetText(content);
  }
}

bool CFunctionCollection::BotLogicCorrectlyParsed() {
  return (!CParseErrors::AnyError());
}

bool CFunctionCollection::ParseAll() {
  write_log(preferences.debug_formula(), 
    "[CFunctionCollection] ParseAll()\n");
  CSLock lock(m_critsec);
  CheckForDefaultFormulaEntries();
  p_formula_parser->InitNewParse();
  COHScriptObject *p_oh_script_object = GetFirst();
  while (p_oh_script_object != NULL) {
    if (p_oh_script_object->IsFunction() || p_oh_script_object->IsList()) {
      p_oh_script_object->Parse();
    }
    p_oh_script_object = GetNext();  
  }
  p_formula_parser->FinishParse();
  return true;
}

bool CFunctionCollection::IsOpenPPLProfile() {
  // A profile is OpenPPL if at least f$preflop exists
  // and f$preflop is not empty
  if (!Exists(k_OpenPPL_function_names[kBetroundPreflop])) return false;
  COHScriptObject *p_preflop = LookUp(k_OpenPPL_function_names[kBetroundPreflop]);
  assert(p_preflop != NULL);
  CString function_text = p_preflop->function_text();
  // Counting nearly empty functions as empty (default: 1 space)
  return (function_text.GetLength() > 1);
}

void CFunctionCollection::InitOnStartup() {
}

void CFunctionCollection::ResetOnConnection() {
  ClearCache();
}

void CFunctionCollection::ResetOnHandreset() {
}

void CFunctionCollection::ResetOnNewRound() {
}

void CFunctionCollection::ResetOnMyTurn() {
}

void CFunctionCollection::ResetOnHeartbeat() {
  ClearCache();
}

bool CFunctionCollection::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  CSLock lock(m_critsec);
  if (COHScriptObject::IsFunction(name)
      || COHScriptObject::IsList(name)
      || COHScriptObject::IsOpenPPLSymbol(name)) {
#ifdef _DEBUG
    if (COHScriptObject::IsOpenPPLSymbol(name)) {
      // Just to be able to set a break-point,
      // that doesn't get triggered every heartbeat by an ini-function
      int i = 0;
    }
#endif
    COHScriptObject *p_function = LookUp(name);
    if (p_function == NULL) {
      // Function does not exist
      *result = kUndefinedZero;
      if (log) {
        write_log(preferences.debug_auto_trace(),
          "[CFunctionCollection] %s -> 0.000 [does not exist]\n", name);
        p_autoplayer_trace->Add(name, *result);
      }
      // This symbol is something that HAS TO be evaluated here,
      // We didn't find it, so we treat it as zero/false/whatever.
      // Symbol-verification happens at parse-time,
      // so this kind of error can only happen for DLL-guys
      // who call OH-script-functions.
      return true;
    }
    // Function/list found
    *result = p_function->Evaluate(log);
    return true;
  }
  // Not a function and not a list
  return false;
}

CString CFunctionCollection::SymbolsProvided() {
  CString result;
  COHScriptObject *p_oh_script_object = GetFirst();
  while (p_oh_script_object != NULL) {
    result += p_oh_script_object->name(); 
    result += " ";
    p_oh_script_object = GetNext();  
  }
  return result;
}