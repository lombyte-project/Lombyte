#include "sda.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"
#include "rnc/game/options.h"

void save_card_state_prompt_create_save(void) __asm__("FUN_00208ca8");

void save_card_state_prompt_create_save(void) {
    int freeze_flags_snapshot;

    if (memory_card_state.card[0].sync_result != 0) {
        save_card_state = CS_NOCARD;
        return;
    }
    freeze_flags_snapshot = mode_freeze_flags;
    if (freeze_flags_snapshot & 0x20) {
        mode_freeze_flags = freeze_flags_snapshot ^ 0x20;
        if (boot_level_active != 0) {
            save_card_state = CS_PROMPT_BEGIN_NOSAVE;
        } else {
            save_card_state = CS_NOSAVE;
        }
        return;
    }
    if (freeze_flags_snapshot & 0x10) {
        mode_freeze_flags = freeze_flags_snapshot ^ 0x10;
        save_card_state = CS_CREATE_SAVE_PENDING;
    }
}

extern __typeof__(save_card_state_prompt_create_save) func_00208CA8
    __attribute__((alias("FUN_00208ca8")));
