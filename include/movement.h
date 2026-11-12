#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "variables.h"
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

bool apply_gui_move(char board[BOARD_ROWS][BOARD_COLS], char c1, int r1,
                    char c2, int r2, char prom_piece);
int turn(int moves_played);
bool is_white_turn(int moves_played);
bool is_black_turn(int moves_played);
bool undo();
bool redo();
bool save_game();
bool load_game();

#ifdef __cplusplus
}
#endif

#endif // MOVEMENT_H
