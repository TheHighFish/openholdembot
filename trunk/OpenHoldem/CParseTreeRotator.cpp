//*****************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//*****************************************************************************
//
// Purpose: Priority-ordering of operator-nodes in the parse-tree.
//   OpenHoldem creates parse-trees in a very straight-forward way:
//
// The (naive) parse-tree for "2*3 + 4*5"
//
//     *
//    / \
//   2   +
//      / \
//     3   *
//        / \
//       4   5
//
// However if we evaluate depth-first this results in (2 * (3 + ( 4 * 5))) = 46
// Now we need to modifiy the parse-tree, low-priority operators to the top:
//  
//         +
//       /   \
//     *      *
//    / \    / \
//   2   3  4   5
//
//
// Evaluating this tree depth-first will result in (2 * 3) + (4 * 5),
// first the sibblings, then the parent node (plus-operator).
//
//*****************************************************************************

#include "stdafx.h"
#include "CParseTreeRotator.h"

#include "CFunction.h"
#include "CParseTreeNode.h"
#include "TokenizerConstants.h"

CParseTreeRotator::CParseTreeRotator()
{}

CParseTreeRotator::~CParseTreeRotator()
{}

void CParseTreeRotator::Rotate(CFunction *function)
{
  if (function == NULL) {
    return;
  }
  Rotate(function->_parse_tree_node, &function->_parse_tree_node);
}

void CParseTreeRotator::Rotate(TPParseTreeNode parse_tree_node,
                               TPParseTreeNode *pointer_to_parent_pointer_for_back_patching)
{
  if (parse_tree_node == NULL) {
    return;
  }
  // We must rotate the sibblings first, bottom-up
  Rotate(parse_tree_node->_first_sibbling,  &parse_tree_node->_first_sibbling);
  Rotate(parse_tree_node->_second_sibbling, &parse_tree_node->_second_sibbling);
  Rotate(parse_tree_node->_third_sibbling,  &parse_tree_node->_third_sibbling);
  // Then we can rotate our node
  if (NeedsLeftRotation(parse_tree_node)) {
    RotateLeft(parse_tree_node, pointer_to_parent_pointer_for_back_patching);
  }
  if (NeedsRightRotation(parse_tree_node)) {
    RotateRight(parse_tree_node, pointer_to_parent_pointer_for_back_patching);
  }
}

bool CParseTreeRotator::NeedsLeftRotation(TPParseTreeNode parse_tree_node) {
  if (parse_tree_node == NULL) {
    return false;
  }
  TPParseTreeNode right_most_sibbling = parse_tree_node->GetRightMostSibbling();
  if (right_most_sibbling == NULL) {
    return false;
  }
  int sibblings_type = right_most_sibbling->_node_type; 
  if (TokenIsBracketOpen(sibblings_type)) {
    // Nothing to do for bracket-expressions as operands.
    // They are "unary" and the parser took care about correct recognition
    return false;
  }
  if (TokenIsUnary(sibblings_type)) {
    // Nothing to do for unary expressions as right-hand-operands
    // e.g. "2 + !false": we clearly can't move the negation to the top.
    return false;
  }
  if (sibblings_type == kTokenOperatorConditionalWhen) {
    // Nothing to do for when-conditions
    // We already got them right when constructing the parse-tree
    return false;
  }
  int sibblings_priority = GetOperatorPriority(sibblings_type);
  if (sibblings_priority == k_undefined) {
    // Something that is not an operator
    return false;
  }
  int my_priority = GetOperatorPriority(parse_tree_node->_node_type);
  if (sibblings_priority < my_priority) {
    return true;
  }
  if ((sibblings_priority == my_priority) 
      && !IsOperatorRightAssociativ(parse_tree_node->_node_type)
      && !TokenIsUnary(right_most_sibbling->_node_type)) {
    // No rotation for unary sibbling of equal priority
    // Otherwise we get an endless recusion for expressions like "NOT NOT NOT true".
    return true;
  }
  return false;
}

void CParseTreeRotator::RotateLeft(TPParseTreeNode parse_tree_node,
                                   TPParseTreeNode *pointer_to_parent_pointer_for_back_patching) {
  assert(parse_tree_node != NULL);
  TPParseTreeNode right_sibbling = parse_tree_node->GetRightMostSibbling();
  assert(right_sibbling != NULL);
  TPParseTreeNode right_left_sibbling = right_sibbling->GetLeftMostSibbling();
  
  parse_tree_node->SetRightMostSibbling(right_left_sibbling);
  right_sibbling->SetLeftMostSibbling(parse_tree_node);
  // Make parent pointer point to right_sibbling
  // instead of parse_tree_node
  *pointer_to_parent_pointer_for_back_patching = right_sibbling;
  Rotate(right_sibbling, pointer_to_parent_pointer_for_back_patching);
}

bool CParseTreeRotator::NeedsRightRotation(TPParseTreeNode parse_tree_node) {
  return false;
}

void CParseTreeRotator::RotateRight(TPParseTreeNode parse_tree_node,
                                    TPParseTreeNode *pointer_to_parent_pointer_for_back_patching) {
}

