#include "sda.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int D_0015EEB4_s[] __asm__("D_0015EEB4") __attribute__((sda));
extern int mode_freeze_flags __asm__("D_0015EEB4");
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;
extern int D_0015F5E8;

void save_card_state_prompt_format(void) __asm__("FUN_00208a78");

void save_card_state_prompt_format(void) {
    struct MemoryCardState *s = &memory_card_state;
    int f;
    if (s->card[0].sync_result != -2) {
        mode_freeze_state = 3;
        return;
    }
    f = D_0015EEB4_s[0];
    if (f & 0x20) {
        D_0015EEB4_s[0] = f ^ 0x20;
        if (D_0015F5E8 != 0) {
            mode_freeze_state = 0x17;
        } else {
            mode_freeze_state = 5;
        }
        return;
    }
    if (f & 8) {
        mode_freeze_flags = f ^ 8;
        mode_freeze_state = 7;
    }
}

extern __typeof__(save_card_state_prompt_format) func_00208A78
    __attribute__((alias("FUN_00208a78")));
