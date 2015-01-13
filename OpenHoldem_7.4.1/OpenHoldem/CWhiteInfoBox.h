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

class CWhiteInfoBox {
 public:
  CWhiteInfoBox();
  ~CWhiteInfoBox();
 public:
  void Draw(RECT client_rect, LOGFONT logfont, CDC *pDC,
    CPen *black_pen, CBrush *white_brush);
 private:
  CString InfoText();
};

extern CWhiteInfoBox *p_white_info_box;