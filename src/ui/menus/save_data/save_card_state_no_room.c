#include "sda.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"

void save_card_state_no_room(void) __asm__("FUN_00208eb8");

void save_card_state_no_room(void) {
    if (memory_card_state.card[0].sync_result != 0) {
        mode_freeze_state = 3;
    }
}

extern __typeof__(save_card_state_no_room) func_00208EB8 __attribute__((alias("FUN_00208eb8")));
