#include "rnc/rendering_transitions_draw_transition_overlay_types.h"
#include "types.h"

extern struct M2c_D_00151780 D_00151780;
extern u8 D_001A00F0[];
extern void func_001F5450();

s32 draw_transition_overlay(struct M2c_arg0 *arg0) __asm__("FUN_0021fc68");

s32 draw_transition_overlay(struct M2c_arg0 *arg0) {
    if (arg0->unk44 < 2) {
        return 0;
    }
    func_001F5450(0, 0, D_00151780.unk160, D_00151780.unk162,
                  0, 0, arg0->unk38, arg0->unk3C,
                  ((u64)0x8080 << 16) | 0x8080,
                  *(s64 *)(D_001A00F0 + 0x258));
    return 0x10;
}

extern __typeof__(draw_transition_overlay) func_0021FC68 __attribute__((alias("FUN_0021fc68")));
