#include "types.h"

struct Globals_00151780 {
    u8 pad_0[0x160];
    s16 unk160;
    s16 unk162;
    u8 pad_164[0x2];
};

struct TransitionState {
    u8 pad_0[0x38];
    s32 unk38;
    s32 unk3C;
    u8 pad_40[0x4];
    s32 unk44;
};
#include "rnc/ui/map/map_state.h"

extern struct Globals_00151780 D_00151780;
extern void draw_textured_quad() __asm__("func_001F5450");

s32 draw_transition_overlay(struct TransitionState *transition) __asm__("FUN_0021fc68");

s32 draw_transition_overlay(struct TransitionState *transition) {
    if (transition->unk44 < 2) {
        return 0;
    }
    draw_textured_quad(0, 0, D_00151780.unk160, D_00151780.unk162, 0, 0, transition->unk38, transition->unk3C,
                       ((u64)0x8080 << 16) | 0x8080, D_001A00F0.tex0);
    return 0x10;
}

extern __typeof__(draw_transition_overlay) func_0021FC68 __attribute__((alias("FUN_0021fc68")));
