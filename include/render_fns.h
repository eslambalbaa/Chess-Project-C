#ifndef RENDER_FNS_H
#define RENDER_FNS_H

#include "variables.h"

#ifdef __cplusplus
extern "C" {
#endif

void initposition(char board[BOARD_ROWS][BOARD_COLS]);
void eatenpieces(int eaten_piece);

#ifdef __cplusplus
}
#endif

#endif // RENDER_FNS_H
