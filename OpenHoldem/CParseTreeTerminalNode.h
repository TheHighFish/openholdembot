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

#ifndef INC_CPARSETREETERMINALNODE_H
#define INC_CPARSETREETERMINALNODE_H

#include "CParsetreeNode.h"

class CParseTreeTerminalNode: public CParseTreeNode {
  friend class CFormulaParser;
  friend class CParseTreeRotator;
 public:
  CParseTreeTerminalNode(int relative_line_number);
  virtual ~CParseTreeTerminalNode();
};

typedef CParseTreeTerminalNode *TPParseTreeTerminalNode;

#endif INC_CPARSETREETERMINALNODE_H