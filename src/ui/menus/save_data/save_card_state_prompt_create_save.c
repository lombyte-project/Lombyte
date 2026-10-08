#include "sda.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;
extern int mode_freeze_flags __asm__("D_0015EEB4") MACRO_ADDR;
extern int D_0015F5E8 MACRO_ADDR;

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
        if (D_0015F5E8 != 0) {
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
