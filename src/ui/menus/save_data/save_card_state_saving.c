/* FUN_00208f28 exact recovery: retail keeps the `mode_freeze_flags |= 0x40` result
   in v0 and the 0x15 constant in v1; without the pin the allocator swaps them
   (98.67% under the patched profile).  The register-asm pin reproduces
   retail's block, one instruction stream for all 30 instructions. */

#include "types.h"
#include "rnc/ui/menus/save_data/save_card_state.h"
#include "rnc/globals.h"

#include "rnc/storage/memory_card/memory_card_state.h"
extern void mode_freeze_init() __asm__("func_001FBAB8");

void save_card_state_saving(void) __asm__("FUN_00208f28");

void save_card_state_saving(void) {
    s32 flags;

    if ((memory_card_state.state < 3) && (memory_card_state.pending_state < 0)) {
        if (memory_card_state.err != 0) {
            mode_freeze_init(3, 0);
            mode_freeze_state = 0x15;
            flags = mode_freeze_flags | 0x40;
            mode_freeze_flags = flags;
        } else {
            mode_freeze_state = 1;
        }
    }
}
