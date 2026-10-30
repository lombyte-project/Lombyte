#include "sda.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"

void save_card_state_init(void) __asm__("FUN_002088a8");

void save_card_state_init(void) {
    mode_freeze_state = 3;
    memory_card_state.card[0].sync_result = memory_card_state.result;
    memory_card_state.unkF4 = 0;
}

extern __typeof__(save_card_state_init) func_002088A8 __attribute__((alias("FUN_002088a8")));
