#include "globals.h"
#include "critical_scenarios.h"
#include "is_it_fns.h"
#include "movement.h"
#include "render_fns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



static void update_castle_flags(char start_piece, int start_row, int start_col, int dest_col)
{
    if (start_piece == Wking)
        current.white_king_moved = 1;
    else if (start_piece == Bking)
        current.black_king_moved = 1;

    if (start_piece == Wrook && start_row == RANK_1 && start_col == 0)
        current.white_rook_moved_a = 1;
    if (start_piece == Wrook && start_row == RANK_1 && start_col == 7)
        current.white_rook_moved_h = 1;
    if (start_piece == Brook && start_row == RANK_8 && start_col == 0)
        current.black_rook_moved_a = 1;
    if (start_piece == Brook && start_row == RANK_8 && start_col == 7)
        current.black_rook_moved_h = 1;

    if ((start_piece == Wking || start_piece == Bking) && abs(start_col - dest_col) == 2)
    {
        int rook_start_col = (dest_col == 6) ? 7 : 0;
        if (rook_start_col == 7 && start_row == RANK_1)
            current.white_rook_moved_h = 1;
        if (rook_start_col == 0 && start_row == RANK_1)
            current.white_rook_moved_a = 1;
        if (rook_start_col == 7 && start_row == RANK_8)
            current.black_rook_moved_h = 1;
        if (rook_start_col == 0 && start_row == RANK_8)
            current.black_rook_moved_a = 1;
    }
}

bool apply_gui_move(char board[BOARD_ROWS][BOARD_COLS], char c1, int r1, char c2, int r2, char prom_piece) {
    if (!is_valid_move(board, c1, r1, c2, r2, 0)) return false;
    int dest_col = c2 - 'A';
    int dest_row = 8 - r2;
    int start_col = c1 - 'A';
    int start_row = 8 - r1;
    char start_piece = board[start_row][start_col];
    int eaten_piece_val = board[dest_row][dest_col];
    int is_en_passant = 0;
    int en_passant_row = -1;

    if ((start_piece == Wpawn || start_piece == Bpawn) && start_col != dest_col && isempty(eaten_piece_val)) {
        is_en_passant = 1;
        en_passant_row = start_row;
        eaten_piece_val = board[en_passant_row][dest_col];
    }

    history[current.moves_played] = current;
    undoCount = 0;

    board[dest_row][dest_col] = start_piece;
    board[start_row][start_col] = EMPTY_SQUARE(start_row, start_col);
    if (is_en_passant) board[en_passant_row][dest_col] = EMPTY_SQUARE(en_passant_row, dest_col);

    if ((start_piece == Wking || start_piece == Bking) && abs(start_col - dest_col) == 2) {
        int rook_start_col = (dest_col == 6) ? 7 : 0;
        int rook_dest_col = (dest_col == 6) ? 5 : 3;
        board[start_row][rook_dest_col] = board[start_row][rook_start_col];
        board[start_row][rook_start_col] = EMPTY_SQUARE(start_row, rook_start_col);
    }

    update_castle_flags(start_piece, start_row, start_col, dest_col);
    if (!isempty(eaten_piece_val)) eatenpieces(eaten_piece_val);

    if (is_promotion(board, c2, r2)) {
        char p = (prom_piece != '\0') ? prom_piece : Wqueen;
        board[dest_row][dest_col] = (turn(current.moves_played) == WHITE_PLAYER) ? tolower(p) : toupper(p);
    }

    if (check(board, current.moves_played)) {
        current = history[current.moves_played];
        return false;
    }

    current.en_pass_col = -1;
    if (start_piece == Wpawn && (dest_row - start_row) == 2 * DIR_NORTH) current.en_pass_col = start_col;
    if (start_piece == Bpawn && (dest_row - start_row) == 2 * DIR_SOUTH) current.en_pass_col = start_col;
    current.moves_played++;
    history[current.moves_played] = current;
    return true;
}

int turn(int moves_played)
{
    return moves_played % 2;
}
