#include "sda.h"
#include "rnc/save/memory_card_state.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/globals.h"

extern int mode_freeze_flags_gp[] __asm__("D_0015EEB4") __attribute__((sda));

void save_card_state_prompt_format(void) __asm__("FUN_00208a78");

void save_card_state_prompt_format(void) {
    struct MemoryCardState *card_state = &memory_card_state;
    int                     mode_freeze_flags_snapshot;
    if (card_state->card[0].sync_result != -2) {
        card_state = CS_NOCARD;
        return;
    }
    mode_freeze_flags_snapshot = mode_freeze_flags_gp[0];
    if (mode_freeze_flags_snapshot & 0x20) {
        mode_freeze_flags_gp[0] = mode_freeze_flags_snapshot ^ 0x20;
        if (boot_level_active != 0) {
            card_state = CS_PROMPT_BEGIN_UNFORMATTED;
        } else {
            card_state = CS_UNFORMATTED;
        }
        return;
    }
    if (mode_freeze_flags_snapshot & 8) {
        mode_freeze_flags = mode_freeze_flags_snapshot ^ 8;
        card_state = CS_FORMAT_PENDING;
    }
}

extern __typeof__(save_card_state_prompt_format) func_00208A78
    __attribute__((alias("FUN_00208a78")));
