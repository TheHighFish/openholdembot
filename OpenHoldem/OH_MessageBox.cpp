//!!!!! to be refactored

#include "stdafx.h"
#include "OH_MessageBox.h"

#include <assert.h>
#include "CFormulaParser.h"
#include "CParseErrors.h"
#include "CPreferences.h"


#ifdef OPENHOLDEM_PROGRAM
// For OpenHoldem only, not for OpenScrape
void OH_MessageBox_Formula_Error(CString Message, CString Title) {
  // Make sure, that we have a line-break at the end.
  if (Message.Right(1) != "\n") {
    Message += "\n";
  }
  if (p_formula_parser->IsParsing()) {
    CParseErrors::Error(Message);
  } else {
    MessageBox_Error_Warning(Message, Title);
  }
}
#endif OPENHOLDEM_PROGRAM