#include "globals.h"
#include "is_it_fns.h"
#include "movement.h"

char input_move[MAX_BUFFER_SIZE];
char eaten_piece;
char promotion_piece;
int undoCount = 0;
game history[MAX_HISTORY];
game current;

bool is_white_turn(int moves_played)
{
    return turn(moves_played) == WHITE_PLAYER;
}

bool is_black_turn(int moves_played)
{
    return turn(moves_played) == BLACK_PLAYER;
}

bool is_white(char piece)
{
    return piece >= 'a' && piece <= 'z';
}

bool is_black(char piece)
{
    return piece >= 'A' && piece <= 'Z';
}

bool isempty(char place)
{
    return place == WEmpty || place == BEmpty;
}
