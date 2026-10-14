#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"
/* Disable the global state flag. */

void disable_global_state_flag(void) __asm__("func_001F61F8");

void disable_global_state_flag(void) {
    GlobalStateFlag = 0;
}
