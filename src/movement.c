#include "variables.h"
#include "critical_scenarios.h"
#include "is_it_fns.h"
#include "movement.h"
#include "render_fns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

bool is_white_turn(int moves_played)
{
    return turn(moves_played) == WHITE_PLAYER;
}

bool is_black_turn(int moves_played)
{
    return turn(moves_played) == BLACK_PLAYER;
}

static void update_castle_flags(char start_piece, int start_row, int start_col, int dest_col)
{
    if (start_piece == Wking)
        current.white_king_moved = 1;
    else if (start_piece == Bking)
        current.black_king_moved = 1;

    if (start_piece == Wrook && start_row == 7 && start_col == 0)
        current.white_rook_moved_a = 1;
    if (start_piece == Wrook && start_row == 7 && start_col == 7)
        current.white_rook_moved_h = 1;
    if (start_piece == Brook && start_row == 0 && start_col == 0)
        current.black_rook_moved_a = 1;
    if (start_piece == Brook && start_row == 0 && start_col == 7)
        current.black_rook_moved_h = 1;

    if ((start_piece == Wking || start_piece == Bking) && abs(start_col - dest_col) == 2)
    {
        int rook_start_col = (dest_col == 6) ? 7 : 0;
        if (rook_start_col == 7 && start_row == 7)
            current.white_rook_moved_h = 1;
        if (rook_start_col == 0 && start_row == 7)
            current.white_rook_moved_a = 1;
        if (rook_start_col == 7 && start_row == 0)
            current.black_rook_moved_h = 1;
        if (rook_start_col == 0 && start_row == 0)
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
    if (start_piece == Wpawn && (dest_row - start_row) == -2) current.en_pass_col = start_col;
    if (start_piece == Bpawn && (dest_row - start_row) == 2) current.en_pass_col = start_col;
    current.moves_played++;
    history[current.moves_played] = current;
    return true;
}

int turn(int moves_played)
{
    return moves_played % 2;
}

bool undo()
{
    if (current.moves_played > 0)
    {
        current = history[current.moves_played - 1];
        undoCount++;
        return true;
    }
    return false;
}

bool redo()
{
    if (undoCount > 0)
    {
        current = history[current.moves_played + 1];
        undoCount--;
        return true;
    }
    return false;
}

bool save_game()
{
    FILE *save = fopen("save.txt", "wb");
    if (save == NULL)
        return false;

    size_t written = 0;
    written += fwrite(&current, sizeof(game), 1, save);
    written += fwrite(history, sizeof(game), MAX_HISTORY, save);
    written += fwrite(&undoCount, sizeof(int), 1, save);
    fclose(save);
    return written == (1 + MAX_HISTORY + 1);
}

bool load_game()
{
    FILE *load = fopen("save.txt", "rb");
    if (load == NULL)
        return false;

    size_t read_cnt = 0;
    read_cnt += fread(&current, sizeof(game), 1, load);
    read_cnt += fread(history, sizeof(game), MAX_HISTORY, load);
    read_cnt += fread(&undoCount, sizeof(int), 1, load);
    fclose(load);
    return read_cnt == (1 + MAX_HISTORY + 1);
}
