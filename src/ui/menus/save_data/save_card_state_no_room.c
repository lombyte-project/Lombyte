#include "sda.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;

void save_card_state_no_room(void) __asm__("FUN_00208eb8");

void save_card_state_no_room(void) {
    if (memory_card_state.card[0].sync_result != 0) {
        mode_freeze_state = 3;
    }
}

extern __typeof__(save_card_state_no_room) func_00208EB8 __attribute__((alias("FUN_00208eb8")));
