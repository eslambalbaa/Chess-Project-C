#include "temporal_operations.h"
#include "globals.h"
#include <stdio.h>

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
