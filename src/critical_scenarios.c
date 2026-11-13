#include "globals.h"
#include "critical_scenarios.h"
#include "is_it_fns.h"
#include "movement.h"

static bool is_own_piece(char piece, int moves_played)
{
    if (isempty(piece))
        return false;

    return is_white(piece) == is_white_turn(moves_played);
}

bool check(char board[BOARD_ROWS][BOARD_COLS], int moves_played)
{
    int k_row = -1, k_col = -1;
    char king = (turn(moves_played) == WHITE_PLAYER) ? Wking : Bking;

    for (int i = 0; i < BOARD_ROWS; i++)
    {
        for (int j = 0; j < BOARD_COLS; j++)
        {
            if (board[i][j] == king)
            {
                k_row = i;
                k_col = j;
                break;
            }
        }
        if (k_row != -1)
            break;
    }

    if (k_row == -1)
        return false;

    for (int i = 0; i < BOARD_ROWS; i++)
    {
        for (int j = 0; j < BOARD_COLS; j++)
        {
            char under_check = board[i][j];
            if (isempty(under_check) || is_own_piece(under_check, moves_played))
                continue;

            if (is_valid_move(board, j + 'A', 8 - i, k_col + 'A', 8 - k_row, 1))
                return true;
        }
    }

    return false;
}

bool checkmate(char board[BOARD_ROWS][BOARD_COLS])
{
    if (!check(board, current.moves_played) || has_any_legal_move(board, current.moves_played))
        return false;

    if (is_black_turn(current.moves_played))
        current.white_win = 1;
    else
        current.black_win = 1;
    return true;
}

bool stalemate(char board[BOARD_ROWS][BOARD_COLS])
{
    return !check(board, current.moves_played) && !has_any_legal_move(board, current.moves_played);
}

bool endgame(char board[BOARD_ROWS][BOARD_COLS])
{
    return checkmate(board) || stalemate(board);
}

static bool try_move(char board[BOARD_ROWS][BOARD_COLS],
                     int src_r, int src_c,
                     int dst_r, int dst_c,
                     int moves_played)
{
    char moving_piece = board[src_r][src_c];
    char captured = board[dst_r][dst_c];

    board[dst_r][dst_c] = moving_piece;
    board[src_r][src_c] = EMPTY_SQUARE(src_r, src_c);

    bool is_safe = !check(board, moves_played);

    board[src_r][src_c] = moving_piece;
    board[dst_r][dst_c] = captured;

    return is_safe;
}

static bool piece_has_legal_move(char board[BOARD_ROWS][BOARD_COLS],
                                 int src_r, int src_c,
                                 int moves_played)
{
    char src_file = src_c + 'A';
    int src_rank = 8 - src_r;

    for (int dst_r = 0; dst_r < BOARD_ROWS; dst_r++)
    {
        for (int dst_c = 0; dst_c < BOARD_COLS; dst_c++)
        {
            if (is_valid_move(board, src_file, src_rank, dst_c + 'A', 8 - dst_r, 1) &&
                try_move(board, src_r, src_c, dst_r, dst_c, moves_played))
            {
                return true;
            }
        }
    }
    return false;
}

bool has_any_legal_move(char board[BOARD_ROWS][BOARD_COLS], int moves_played)
{
    for (int r = 0; r < BOARD_ROWS; r++)
    {
        for (int c = 0; c < BOARD_COLS; c++)
        {
            char piece = board[r][c];
            if (!is_own_piece(piece, moves_played))
                continue;

            if (piece_has_legal_move(board, r, c, moves_played))
                return true;
        }
    }
    return false;
}
