//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#pragma once

#include "CSpaceOptimizedGlobalObject.h"

class CWhiteInfoBox : public CSpaceOptimizedGlobalObject {
 public:
  CWhiteInfoBox();
  ~CWhiteInfoBox();
 public:
  void Draw(RECT client_rect, LOGFONT logfont, CDC *pDC,
    CPen *black_pen, CBrush *white_brush);
  // For log$-symbols
  void SetCustomLogMessage(CString message) { _custom_log_message = message; }
 private:
  CString InfoText();
 private:
  CString _custom_log_message;
};

extern CWhiteInfoBox *p_white_info_box;