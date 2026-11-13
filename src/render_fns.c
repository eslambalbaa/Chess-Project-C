#include "globals.h"
#include "render_fns.h"
#include "is_it_fns.h"
#include "side_notes.h"
#include <stdio.h>
#include <stdlib.h>

void eatenpieces(int eaten_piece)
{
  if (is_white(eaten_piece))
    current.white_eaten[current.white_eaten_count++] = eaten_piece;
  else if (is_black(eaten_piece))
    current.black_eaten[current.black_eaten_count++] = eaten_piece;
}

void initposition(char board[BOARD_ROWS][BOARD_COLS])
{
  char initial_board[BOARD_ROWS][BOARD_COLS] = {
      {Brook, Bknight, Bbishop, Bqueen, Bking, Bbishop, Bknight, Brook},
      {Bpawn, Bpawn, Bpawn, Bpawn, Bpawn, Bpawn, Bpawn, Bpawn},
      {WEmpty, BEmpty, WEmpty, BEmpty, WEmpty, BEmpty, WEmpty, BEmpty},
      {BEmpty, WEmpty, BEmpty, WEmpty, BEmpty, WEmpty, BEmpty, WEmpty},
      {WEmpty, BEmpty, WEmpty, BEmpty, WEmpty, BEmpty, WEmpty, BEmpty},
      {BEmpty, WEmpty, BEmpty, WEmpty, BEmpty, WEmpty, BEmpty, WEmpty},
      {Wpawn, Wpawn, Wpawn, Wpawn, Wpawn, Wpawn, Wpawn, Wpawn},
      {Wrook, Wknight, Wbishop, Wqueen, Wking, Wbishop, Wknight, Wrook}};

  for (int i = 0; i < BOARD_ROWS; i++)
    for (int j = 0; j < BOARD_COLS; j++)
      board[i][j] = initial_board[i][j];
}
