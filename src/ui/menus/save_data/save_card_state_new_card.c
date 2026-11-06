#include "sda.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/globals.h"

void save_card_state_new_card(void) __asm__("FUN_00208dd8");

void save_card_state_new_card(void) {
    struct MemoryCardState *card_state;
    int                     initial_freeze_flags = mode_freeze_flags;
    int                     freeze_flags_without_bit_2 = initial_freeze_flags & ~4;
    int                     freeze_flags_without_bits_1_and_2 = freeze_flags_without_bit_2 & ~2;
    mode_freeze_flags = freeze_flags_without_bits_1_and_2;
    if (initial_freeze_flags & 0x80) {
        save_card_state = CS_SAVE_FAILED;
        mode_freeze_flags = (freeze_flags_without_bits_1_and_2 ^ 0x80) | 0x40;
        return;
    }
    if (initial_freeze_flags & 0x100) {
        save_card_state = CS_LOAD_FAILED;
        mode_freeze_flags = (freeze_flags_without_bits_1_and_2 ^ 0x100) | 0x40;
        return;
    }
    card_state = &memory_card_state;
    if (card_state->card[0].sync_result != 0) {
        save_card_state = CS_NOCARD;
        return;
    }
    if (card_state->card_save_active != 0)
        save_card_state = CS_GOOD_SAVE;
}

extern __typeof__(save_card_state_new_card) func_00208DD8 __attribute__((alias("FUN_00208dd8")));
