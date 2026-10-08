#include "sda.h"
#include "rnc/storage/memory_card/memory_card_state.h"
extern int D_0015EEB4_s[] __asm__("D_0015EEB4") __attribute__((sda));
extern int mode_freeze_flags __asm__("D_0015EEB4");
extern int mode_freeze_state __asm__("D_0015EEB0") MACRO_ADDR;

void save_card_state_prompt_begin_unformatted(void) __asm__("FUN_00208fa0");

void save_card_state_prompt_begin_unformatted(void) {
    struct MemoryCardState *s = &memory_card_state;
    int f;
    if (s->card[0].sync_result != -2) {
        mode_freeze_state = 3;
        return;
    }
    f = D_0015EEB4_s[0];
    if (f & 0x20) {
        mode_freeze_flags = f ^ 0x20;
        mode_freeze_state = 5;
    }
}

extern __typeof__(save_card_state_prompt_begin_unformatted) func_00208FA0
    __attribute__((alias("FUN_00208fa0")));
