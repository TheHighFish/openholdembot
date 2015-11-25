//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Symbol lookup for various symbols 
//   that are not part of a regular symbol-engine
//
//*******************************************************************************

#include "stdafx.h"
#include "CSymbolEngineVariousDataLookup.h"

#include <assert.h>
#include <process.h>
#include <float.h>

#include "CAutoconnector.h"
#include "CAutoplayerTrace.h"
#include "CBetroundCalculator.h"
#include "CDllExtension.h"
#include "CFlagsToolbar.h"
#include "CFormulaParser.h"
#include "inlines/eval.h"
#include "Chair$Symbols.h"
#include "CHandresetDetector.h"
#include "CIteratorThread.h"
#include "CPokerTrackerThread.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CScraperAccess.h"
#include "CSessionCounter.h"
#include "CStringMatch.h"
#include "CSymbolEngineUserchair.h"
#include "..\CTablemap\CTablemap.h"
#include "..\CTransform\CTransform.h"
#include "CWhiteInfoBox.h"
#include "MagicNumbers.h"
#include "OpenHoldem.h"
#include "OH_MessageBox.h"

CSymbolEngineVariousDataLookup			*p_symbol_engine_various_data_lookup = NULL;


CSymbolEngineVariousDataLookup::CSymbolEngineVariousDataLookup() {
}

CSymbolEngineVariousDataLookup::~CSymbolEngineVariousDataLookup() {
}

void CSymbolEngineVariousDataLookup::InitOnStartup() {
}

void CSymbolEngineVariousDataLookup::ResetOnConnection() {
}

void CSymbolEngineVariousDataLookup::ResetOnHandreset() {
  // Reset display
  InvalidateRect(theApp.m_pMainWnd->GetSafeHwnd(), NULL, true);
}

void CSymbolEngineVariousDataLookup::ResetOnNewRound() {
}

void CSymbolEngineVariousDataLookup::ResetOnMyTurn() {
}

void CSymbolEngineVariousDataLookup::ResetOnHeartbeat() {
}

bool CSymbolEngineVariousDataLookup::EvaluateSymbol(const char *name, double *result, bool log /* = false */) {
  FAST_EXIT_ON_OPENPPL_SYMBOLS(name);
  // DLL
  if (memcmp(name, "dll$", 4) == 0) {
    assert(p_dll_extension != NULL);
    if (p_dll_extension->IsLoaded()) {
	    *result = ProcessQuery(name);
    } else {
	    *result = kUndefinedZero;
    }
  }
  // Various symbols below
  // without any optimized lookup.
  //ROUND&POSITIONS
  else if (memcmp(name, "betround", 8)==0 && strlen(name)==8)	*result = p_betround_calculator->betround();
  //FLAGS
  else if (memcmp(name, "fmax", 4)==0 && strlen(name)==4)			*result = p_flags_toolbar->GetFlagMax();
  // flags f0..f9
  else if (memcmp(name, "f", 1)==0 && strlen(name)==2)				*result = p_flags_toolbar->GetFlag(RightDigitCharacterToNumber(name));
  // flags f10..f19
  else if (memcmp(name, "f", 1)==0 && strlen(name)==3)				*result = p_flags_toolbar->GetFlag(10 * RightDigitCharacterToNumber(name, 1) + RightDigitCharacterToNumber(name, 0));
  else if (memcmp(name, "flagbits", 8)==0 && strlen(name)==8)	*result = p_flags_toolbar->GetFlagBits();
  // GENERAL
  else if (memcmp(name, "session", 7)==0 && strlen(name)==7)	*result = p_sessioncounter->session_id();
  else if (memcmp(name, "version", 7)==0 && strlen(name)==7)	*result = VERSION_NUMBER;
  // Handreset
  else if (memcmp(name, "handsplayed", 11)==0 && strlen(name)==11) *result = p_handreset_detector->hands_played();
  else if (memcmp(name, "handsplayed_headsup", 19)==0 && strlen(name)==19)  *result = p_handreset_detector->hands_played_headsup();
  // OH-script-messagebox
  else if (memcmp(name, "msgbox$", 7)==0 && strlen(name)>7) {
    // Don't show name messagebox if in parsing-mode
    if (p_formula_parser->IsParsing()
        || !p_autoconnector->IsConnected()
	      || !p_symbol_engine_userchair->userchair_confirmed()) {
	    *result = 0;
    } else {
	    OH_MessageBox_OH_Script_Messages(name);
	    *result = 0;
    }
  }
  else if ((memcmp(name, "log$", 4)==0) && (strlen(name)>4)) {
    if (!p_formula_parser->IsParsing()) {
      write_log(preferences.debug_auto_trace(), 
        "[CSymbolEngineVariousDataLookup] %s -> 0.000 [just logged]\n", name);
      p_autoplayer_trace->Add(name, 0);
      p_white_info_box->SetCustomLogMessage(name);
    }
    *result = 0;
  } else if ((memcmp(name, "attached_hwnd", 13)==0) && (strlen(name)==13)) {
    *result = int(p_autoconnector->attached_hwnd());
  } else if ((memcmp(name, "islobby", 7)==0) && (strlen(name)==7)) {
    *result = p_tablemap->islobby();
  } else if ((memcmp(name, kEmptyExpression_False_Zero_WhenOthersFoldForce, strlen(kEmptyExpression_False_Zero_WhenOthersFoldForce))==0) 
      && (strlen(name)==strlen(kEmptyExpression_False_Zero_WhenOthersFoldForce))) {
    *result = kUndefinedZero;
  } else {
    // Special symbol for empty expressions. Its evaluation adds something 
    // meaningful to the log when the end of an open-ended when-condition 
    // gets reached during evaluation.
    *result = kUndefined;
    return false;
  }
  return true;
}

CString CSymbolEngineVariousDataLookup::SymbolsProvided() {
  // This list includes some prefixes of symbols that can't be verified,
  // e.g. "dll$, pl_chair$, ....
  CString list = "dll$ pl_ vs$ msgbox$ log$ "
    "betround fmax f flagbits "
    "session version islobby "
    "handsplayed handsplayed_headsup ";
  list += RangeOfSymbols("f%i", 0, 19);
  return list;
}



