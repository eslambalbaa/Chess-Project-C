#ifndef IS_IT_FNS_H
#define IS_IT_FNS_H

#include "globals.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool is_valid_move(char board[BOARD_ROWS][BOARD_COLS], char c1, int r1, char c2,
                   int r2, int skip);
bool is_path_clear(char board[BOARD_ROWS][BOARD_COLS], int destrow, int destcol,
                   int startrow, int startcol);
bool is_white(char piece);
bool is_black(char piece);
bool isempty(char place);
bool is_promotion(char board[BOARD_ROWS][BOARD_COLS], char c2, int r2);
bool is_promotion_valid(char board[BOARD_ROWS][BOARD_COLS],
                        char promotion_piece, int startrow, int startcol);

#ifdef __cplusplus
}
#endif

#endif // IS_IT_FNS_H
