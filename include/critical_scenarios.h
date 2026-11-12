#ifndef CRITICAL_SCENARIOS_H
#define CRITICAL_SCENARIOS_H

#include "variables.h"
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

bool check(char board[BOARD_ROWS][BOARD_COLS], int moves_played);
bool checkmate(char board[BOARD_ROWS][BOARD_COLS]);
bool stalemate(char board[BOARD_ROWS][BOARD_COLS]);
bool endgame(char board[BOARD_ROWS][BOARD_COLS]);
bool has_any_legal_move(char board[BOARD_ROWS][BOARD_COLS], int moves_played);

#ifdef __cplusplus
}
#endif

#endif // CRITICAL_SCENARIOS_H
