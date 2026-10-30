#include "sda.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"
#include "rnc/game/options.h"

void save_card_state_prompt_create_save(void) __asm__("FUN_00208ca8");

void save_card_state_prompt_create_save(void) {
    int flags;

    if (memory_card_state.card[0].sync_result != 0) {
        mode_freeze_state = 3;
        return;
    }
    flags = mode_freeze_flags;
    if (flags & 0x20) {
        mode_freeze_flags = flags ^ 0x20;
        if (boot_level_active != 0) {
            mode_freeze_state = 0x18;
        } else {
            mode_freeze_state = 0xC;
        }
        return;
    }
    if (flags & 0x10) {
        mode_freeze_flags = flags ^ 0x10;
        mode_freeze_state = 0xE;
    }
}

extern __typeof__(save_card_state_prompt_create_save) func_00208CA8
    __attribute__((alias("FUN_00208ca8")));
