//*******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*******************************************************************************
//
// Purpose: Base class for Operators and terminal nodes 
//   (identifiers, numbers)
//
//*******************************************************************************

#include "stdafx.h" 
#include "CParseTreeNode.h"

CParseTreeNode::CParseTreeNode(int relative_line_number) {
  _relative_line_number = relative_line_number;
  _node_type = kUndefined;
}

CParseTreeNode::~CParseTreeNode() {
}

