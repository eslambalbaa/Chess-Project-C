#ifndef TEMPORAL_OPERATIONS_H
#define TEMPORAL_OPERATIONS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool undo();
bool redo();
bool save_game();
bool load_game();

#ifdef __cplusplus
}
#endif

#endif // TEMPORAL_OPERATIONS_H
