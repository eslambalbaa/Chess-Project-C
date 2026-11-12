#include "variables.h"
#include "critical_scenarios.h"
#include "is_it_fns.h"
#include "movement.h"

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
            if (isempty(under_check))
                continue;

            if ((iswhite(under_check) && is_black_turn(moves_played)) ||
                (isblack(under_check) && is_white_turn(moves_played)))
            {
                if (is_valid_move(board, j + 'A', 8 - i, k_col + 'A', 8 - k_row, 1))
                    return true;
            }
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

bool has_any_legal_move(char board[BOARD_ROWS][BOARD_COLS], int moves_played)
{
    for (int i = 0; i < BOARD_ROWS; i++)
    {
        for (int j = 0; j < BOARD_COLS; j++)
        {
            char piece = board[i][j];
            if (isempty(piece))
                continue;

            if ((iswhite(piece) && is_black_turn(moves_played)) ||
                (isblack(piece) && is_white_turn(moves_played)))
                continue;

            for (int r = 0; r < BOARD_ROWS; r++)
            {
                for (int c = 0; c < BOARD_COLS; c++)
                {
                    if (is_valid_move(board, j + 'A', 8 - i, c + 'A', 8 - r, 1))
                    {
                        char temp = board[r][c];
                        board[r][c] = board[i][j];
                        board[i][j] = EMPTY_SQUARE(i, j);

                        bool is_safe = !check(board, moves_played);

                        board[i][j] = board[r][c];
                        board[r][c] = temp;

                        if (is_safe)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}
