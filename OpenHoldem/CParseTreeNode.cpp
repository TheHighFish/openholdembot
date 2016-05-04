//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: Base class for Operators and terminal nodes 
//   (identifiers, numbers)
//
//******************************************************************************

#include "stdafx.h" 
#include "CParseTreeNode.h"

#include "TokenizerConstants.h"

CParseTreeNode::CParseTreeNode(int relative_line_number) {
  _relative_line_number = relative_line_number;
  _node_type = kUndefined;
  _first_sibbling  = NULL;
  _second_sibbling = NULL;
  _third_sibbling  = NULL;
}

CParseTreeNode::~CParseTreeNode() {
  if (_first_sibbling != NULL) {
    delete _first_sibbling;
    _first_sibbling = NULL;
  }
  if (_second_sibbling != NULL) {
    delete _second_sibbling;
    _second_sibbling = NULL;
  }
  if ((_third_sibbling != NULL) && (!IsOpenEndedWhenCondition())) {
    // Be carefull with open-ended when-conditions.
    // They create graphs, no longer pure trees/
    // The 3rd node is reachable on two paths.
    delete _third_sibbling;
    _third_sibbling = NULL;
  }
}

TPParseTreeNode CParseTreeNode::GetRightMostSibbling() {
  if (TokenIsTernary(_node_type)) {
    return _third_sibbling;
  } else if (TokenIsBinary(_node_type)) {
    return _second_sibbling;
  } else if (TokenIsUnary(_node_type)) {
    return _first_sibbling;
  } else {
    // Not an operator
    return NULL;
  }
}

TPParseTreeNode CParseTreeNode::GetLeftMostSibbling() {
  if (TokenIsUnary(_node_type)
      || TokenIsBinary(_node_type)
      || TokenIsTernary(_node_type)) {
    return _first_sibbling;
  }
  // Not an operator
   return NULL;
}

void CParseTreeNode::SetRightMostSibbling(TPParseTreeNode sibbling){
  if (TokenIsTernary(_node_type)) {
   _third_sibbling = sibbling;
  } else if (TokenIsBinary(_node_type)) {
    _second_sibbling = sibbling;
  } else {
    // Default: first one is always present
    _first_sibbling = sibbling;
  }
}

void CParseTreeNode::SetLeftMostSibbling(TPParseTreeNode sibbling){
  _first_sibbling = sibbling;
}

bool CParseTreeNode::IsAnyKindOfWhenCondition() {
  return (_node_type == kTokenOperatorConditionalWhen);
}

bool CParseTreeNode::IsWhenConditionWithAction() {
  return (IsAnyKindOfWhenCondition() && !IsOpenEndedWhenCondition());
}

bool CParseTreeNode::IsOpenEndedWhenCondition() {
  if (!IsAnyKindOfWhenCondition()) return false;
  if ((_second_sibbling == NULL) && (_third_sibbling == NULL)) return true; // ?????
  if ((_second_sibbling != NULL) 
      && (_second_sibbling->_node_type == kTokenOperatorConditionalWhen)) {
    return true;
  }
  return false;
}

// virtual 
double CParseTreeNode::Evaluate(bool log /* = false */) {
  return kUndefined;
}

// virtual 
CString CParseTreeNode::EvaluateToString(bool /* log = false */) {
  return "";
}

// virtual 
bool CParseTreeNode::EvaluatesToBinaryNumber() {
  return false;
}

// virtual 
CString CParseTreeNode::Serialize() {
  return "";
}
