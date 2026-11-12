#include "variables.h"
#include "is_it_fns.h"
#include "movement.h"
#include "critical_scenarios.h"
#include <stdlib.h>
#include <ctype.h>

bool iswhite(char piece)
{
    return piece >= 'a' && piece <= 'z';
}

bool isblack(char piece)
{
    return piece >= 'A' && piece <= 'Z';
}

bool isempty(char place)
{
    return place == WEmpty || place == BEmpty;
}

bool is_path_clear(char board[BOARD_ROWS][BOARD_COLS], int dest_row, int dest_col, int start_row, int start_col)
{
    int row_diff = dest_row - start_row;
    int col_diff = dest_col - start_col;
    int unit_row = 0;
    int unit_col = 0;

    if (col_diff != 0)
        unit_col = col_diff / abs(col_diff);
    if (row_diff != 0)
        unit_row = row_diff / abs(row_diff);

    int current_row = start_row + unit_row;
    int current_col = start_col + unit_col;

    while (current_row != dest_row || current_col != dest_col)
    {
        if (!isempty(board[current_row][current_col]))
            return false;
        current_row += unit_row;
        current_col += unit_col;
    }
    return true;
}

static bool is_valid_pawn_move(char board[BOARD_ROWS][BOARD_COLS], char piece,
                               int start_row, int start_col, int dest_row, int dest_col)
{
    int row_diff = dest_row - start_row;
    int col_diff = dest_col - start_col;
    int forward_dir = (piece == Wpawn) ? -1 : 1;
    int initial_row = (piece == Wpawn) ? 6 : 1;
    int ep_target_row = (piece == Wpawn) ? 2 : 5;

    if (col_diff == 0)
    {
        if (row_diff == forward_dir && isempty(board[dest_row][dest_col]))
            return true;
        if (row_diff == 2 * forward_dir && start_row == initial_row &&
            isempty(board[dest_row][dest_col]) &&
            isempty(board[start_row + forward_dir][start_col]))
            return true;
    }
    else if (abs(col_diff) == 1 && row_diff == forward_dir)
    {
        if (!isempty(board[dest_row][dest_col]))
            return true;
        if (dest_row == ep_target_row && dest_col == current.en_pass_col)
            return true;
    }
    return false;
}

static bool is_valid_castling(char board[BOARD_ROWS][BOARD_COLS], char king_piece,
                              int start_row, int dest_col)
{
    int player = (king_piece == Wking) ? WHITE_PLAYER : BLACK_PLAYER;
    bool king_moved = (player == WHITE_PLAYER) ? current.white_king_moved : current.black_king_moved;
    bool rook_moved_a = (player == WHITE_PLAYER) ? current.white_rook_moved_a : current.black_rook_moved_a;
    bool rook_moved_h = (player == WHITE_PLAYER) ? current.white_rook_moved_h : current.black_rook_moved_h;
    int home_row = (player == WHITE_PLAYER) ? 7 : 0;

    if (start_row != home_row || king_moved)
        return false;

    if (dest_col == 6 && !rook_moved_h && is_path_clear(board, home_row, 7, home_row, 4))
    {
        if (check(board, player))
            return false;
        board[home_row][5] = king_piece;
        board[home_row][4] = EMPTY_SQUARE(home_row, 4);
        bool in_check = check(board, player);
        board[home_row][4] = king_piece;
        board[home_row][5] = EMPTY_SQUARE(home_row, 5);
        return !in_check;
    }

    if (dest_col == 2 && !rook_moved_a && is_path_clear(board, home_row, 0, home_row, 4))
    {
        if (check(board, player))
            return false;
        board[home_row][3] = king_piece;
        board[home_row][4] = EMPTY_SQUARE(home_row, 4);
        bool in_check = check(board, player);
        board[home_row][4] = king_piece;
        board[home_row][3] = EMPTY_SQUARE(home_row, 3);
        return !in_check;
    }

    return false;
}

bool is_valid_move(char board[BOARD_ROWS][BOARD_COLS], char c1, int r1, char c2, int r2, int skip)
{
    if (c1 < 'A' || c1 > 'H' || c2 < 'A' || c2 > 'H')
        return false;
    if (r1 < 1 || r1 > 8 || r2 < 1 || r2 > 8)
        return false;

    int dest_col = c2 - 'A';
    int dest_row = 8 - r2;
    int start_col = c1 - 'A';
    int start_row = 8 - r1;
    char piece = board[start_row][start_col];
    int row_diff = dest_row - start_row;
    int col_diff = dest_col - start_col;

    if (skip == 0 && iswhite(piece) && turn(current.moves_played) == BLACK_PLAYER)
        return false;
    else if (skip == 0 && isblack(piece) && turn(current.moves_played) == WHITE_PLAYER)
        return false;

    if (start_row == dest_row && start_col == dest_col)
        return false;
    if ((iswhite(piece) && iswhite(board[dest_row][dest_col])) || (isblack(piece) && isblack(board[dest_row][dest_col])))
        return false;

    switch (piece)
    {
    case Wpawn:
    case Bpawn:
        return is_valid_pawn_move(board, piece, start_row, start_col, dest_row, dest_col);

    case Wrook:
    case Brook:
        return (row_diff == 0 || col_diff == 0) && is_path_clear(board, dest_row, dest_col, start_row, start_col);

    case Wknight:
    case Bknight:
        return (abs(row_diff) == 2 && abs(col_diff) == 1) || (abs(row_diff) == 1 && abs(col_diff) == 2);

    case Wbishop:
    case Bbishop:
        return (abs(row_diff) == abs(col_diff)) && is_path_clear(board, dest_row, dest_col, start_row, start_col);

    case Wqueen:
    case Bqueen:
        return (row_diff == 0 || col_diff == 0 || abs(row_diff) == abs(col_diff)) &&
               is_path_clear(board, dest_row, dest_col, start_row, start_col);

    case Wking:
    case Bking:
        if (abs(row_diff) <= 1 && abs(col_diff) <= 1)
            return true;
        if (row_diff == 0 && skip == 0)
            return is_valid_castling(board, piece, start_row, dest_col);
        return false;

    default:
        return false;
    }
}

bool is_promotion(char board[BOARD_ROWS][BOARD_COLS], char c2, int r2)
{
    int dest_row = 8 - r2;
    int dest_col = c2 - 'A';
    char piece = board[dest_row][dest_col];

    return (piece == Wpawn && iswhite(piece) && dest_row == 0) || (piece == Bpawn && isblack(piece) && dest_row == 7);
}

bool is_promotion_valid(char board[BOARD_ROWS][BOARD_COLS], char promotion_piece, int start_row, int start_col)
{
    if (board[start_row][start_col] == Wpawn)
        return promotion_piece == Wbishop || promotion_piece == Wknight || promotion_piece == Wqueen || promotion_piece == Wrook;

    if (board[start_row][start_col] == Bpawn)
        return promotion_piece == Bbishop || promotion_piece == Bknight || promotion_piece == Bqueen || promotion_piece == Brook;

    return false;
}
