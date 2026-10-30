#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
/* Enable the global state flag. */

void enable_global_flag(void) __asm__("func_001F61E8");

void enable_global_flag(void) {
    GlobalStateFlag = 1;
}
