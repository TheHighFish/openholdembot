#pragma once

//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: base class for formula objects 
//   that use memory-pools
//
//******************************************************************************

class CSpaceOptimizedFormulaObject {
public:
  CSpaceOptimizedFormulaObject() {}
  ~CSpaceOptimizedFormulaObject() {}
public:
  void* operator new(size_t size);
public:
  void operator delete(void* ptr) {}
private:
};