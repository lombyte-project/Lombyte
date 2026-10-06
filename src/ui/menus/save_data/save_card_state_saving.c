/* FUN_00208f28 exact recovery: retail keeps the `D_0015EEB4 |= 0x40` result
   in v0 and the 0x15 constant in v1; without the pin the allocator swaps them
   (98.67% under the patched profile).  The register-asm pin reproduces
   retail's block, one instruction stream for all 30 instructions. */

#include "types.h"
#include "rnc/ui/menus/save_data/save_card_state.h"

extern struct SaveSlotTable D_0013D290;
extern s32 D_0015EEB0;
extern s32 D_0015EEB4;
extern void mode_freeze_init() __asm__("func_001FBAB8");

void save_card_state_saving(void) __asm__("FUN_00208f28");

void save_card_state_saving(void) {
    s32 flags;

    if ((D_0013D290.unkD4 < 3) && (D_0013D290.unkDC < 0)) {
        if (D_0013D290.unkE4 != 0) {
            mode_freeze_init(3, 0);
            D_0015EEB0 = 0x15;
            flags = D_0015EEB4 | 0x40;
            D_0015EEB4 = flags;
        } else {
            D_0015EEB0 = 1;
        }
    }
}
