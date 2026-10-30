#include "sda.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"

void save_card_state_checking_save(void) __asm__("FUN_00208c00");

void save_card_state_checking_save(void) {
    struct MemoryCardState *card_state = &memory_card_state;
    int                     save_index;
    if (card_state->card[0].sync_result < -1) {
        mode_freeze_state = 3;
        return;
    }
    save_index = card_state->card[0].save_index;
    if (save_index == -2) {
        if (card_state->card[0].free < 0x15E) {
            mode_freeze_state = 0x13;
        } else {
            mode_freeze_state = 0xC;
        }
        return;
    }
    if (save_index >= -1)
        mode_freeze_state = 0x10;
}

extern __typeof__(save_card_state_checking_save) func_00208C00
    __attribute__((alias("FUN_00208c00")));
