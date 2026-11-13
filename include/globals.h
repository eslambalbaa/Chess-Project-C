#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_ROWS 8
#define BOARD_COLS 8
#define MAX_HISTORY 1000
#define MAX_EATEN_PIECES 16

#define Wpawn 'p'
#define Wrook 'r'
#define Wknight 'n'
#define Wbishop 'b'
#define Wqueen 'q'
#define Wking 'k'

#define Bpawn 'P'
#define Brook 'R'
#define Bknight 'N'
#define Bbishop 'B'
#define Bqueen 'Q'
#define Bking 'K'

#define WEmpty '-'
#define BEmpty '.'
#define EMPTY_SQUARE(row, col) (((row) + (col)) % 2 ? BEmpty : WEmpty)

#define WHITE_PLAYER 0
#define BLACK_PLAYER 1

#define MAX_BUFFER_SIZE 100

extern char input_move[MAX_BUFFER_SIZE];
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

enum {
  RANK_8 = 0,
  RANK_7 = 1,
  RANK_6 = 2,
  RANK_5 = 3,
  RANK_4 = 4,
  RANK_3 = 5,
  RANK_2 = 6,
  RANK_1 = 7
};

enum { DIR_NORTH = -1, DIR_SOUTH = 1 };

#ifdef __cplusplus
}
#endif

#endif // GLOBALS_H
