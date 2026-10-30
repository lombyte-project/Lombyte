#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"

void request_space_load(s32 value) {
    space_load = value;
    space_load_in_progress = space_load_pending = 1;
}
