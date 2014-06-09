//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
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

#ifndef INC_CPARSETREEROTATOR_H
#define INC_CPARSETREEROTATOR_H

#include "CParseTreeNode.h"

class CFunction;

class CParseTreeRotator {
  friend class CFormulaParser;
 public:
  CParseTreeRotator();
  ~CParseTreeRotator();
 public:
  void Rotate(CFunction *function);
 protected:
  void Rotate(TPParseTreeNode parse_tree_node,
    TPParseTreeNode *pointer_to_parent_pointer_for_back_patching);
 private:
  bool NeedsLeftRotation(TPParseTreeNode parse_tree_node);
  bool NeedsRightRotation(TPParseTreeNode parse_tree_node);
  void RotateLeft(TPParseTreeNode parse_tree_node,
    TPParseTreeNode *pointer_to_parent_pointer_for_back_patching);
  void RotateRight(TPParseTreeNode parse_tree_node,
    TPParseTreeNode *pointer_to_parent_pointer_for_back_patching);
};


#endif INC_CPARSETREEROTATOR_H