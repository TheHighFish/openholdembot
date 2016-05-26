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

#ifndef INC_CALLINSLIDER_H
#define INC_CALLINSLIDER_H

class CAllinSlider {
 public:
  CAllinSlider();
  ~CAllinSlider();
 public:
  void SetHandlePosition(const POINT position);
  void ResetHandlePosition();
  bool SlideAllin();
  bool GetSliderRegions();
  bool SlideAllinPossible();
 private:
  POINT _position; //?????
  RECT _i3_slider;
  RECT _i3_handle;
};

#endif INC_CALLINSLIDER_H
