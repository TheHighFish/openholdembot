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

#ifndef INC_CLINECOUNTER_H
#define INC_CLINECOUNTER_H

class CLineCounter {
 public:
  CLineCounter();
  ~CLineCounter();
 public:
  int AbsoluteLine()                              { return _absolute_line; }
  int StartingLineOfCurrentlyParsedFunction()     { return _starting_line_of_function; }
  int RelativeLineInsideCurrentlyParsedFunction() { return _relative_line; }
 public:
  void ResetOnNewSourceFile();
  void OnLineBreak();
  void OnNextFunction();
 private:
  int _absolute_line;
  int _starting_line_of_function;
  int _relative_line;
};

extern CLineCounter line_counter;

#endif INC_CLINECOUNTER_H