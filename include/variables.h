#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_ROWS 8
#define BOARD_COLS 8
#define MAX_HISTORY 1000
#define MAX_EATEN_PIECES 16

/* White pieces */
#define Wpawn 'p'
#define Wrook 'r'
#define Wknight 'n'
#define Wbishop 'b'
#define Wqueen 'q'
#define Wking 'k'

/* Black pieces */
#define Bpawn 'P'
#define Brook 'R'
#define Bknight 'N'
#define Bbishop 'B'
#define Bqueen 'Q'
#define Bking 'K'

/* Board square representation */
#define WEmpty '-'
#define BEmpty '.'
#define EMPTY_SQUARE(row, col) (((row) + (col)) % 2 ? BEmpty : WEmpty)

/* Colors / Turns */
#define WHITE_PLAYER 0
#define BLACK_PLAYER 1

extern char input_move[100];
extern char eaten_piece;
extern char promotion_piece;
extern int undoCount;

typedef struct {
  char board[BOARD_ROWS][BOARD_COLS];
  int moves_played;
  int white_eaten_count;
  int black_eaten_count;
  char white_eaten[MAX_EATEN_PIECES];
  char black_eaten[MAX_EATEN_PIECES];
  int white_win;
  int black_win;
  int en_pass_col;
  int white_king_moved;
  int white_rook_moved_a;
  int white_rook_moved_h;
  int black_king_moved;
  int black_rook_moved_a;
  int black_rook_moved_h;
} game;

extern game history[MAX_HISTORY];
extern game current;

#ifdef __cplusplus
}
#endif

#endif // VARIABLES_H
