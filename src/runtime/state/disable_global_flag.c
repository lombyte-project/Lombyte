#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
/* Disable the global state flag. */

void disable_global_flag(void) __asm__("func_001F61F8");

void disable_global_flag(void) {
    GlobalStateFlag = 0;
}
