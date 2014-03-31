#ifndef INC_BOARD_H
#define INC_BOARD_H

#include <afx.h>

void generate_code_for_suited_board(CString board_expression);
void generate_code_for_non_suited_board(CString board_expression);
void generate_code_for_hand_expression(CString hand_expression);
void generate_code_for_hand_expression_with_specific_suits(CString hand_expression);

#endif // INC_BOARD_H