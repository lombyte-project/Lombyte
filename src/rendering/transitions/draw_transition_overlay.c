#include "types.h"
#include "rnc/rendering/fs_aa_buffer.h"

struct TransitionState {
    u8 pad_0[0x38];
    s32 unk38;
    s32 unk3C;
    u8 pad_40[0x4];
    s32 unk44;
};
#include "rnc/ui/map/map_state.h"

extern void draw_textured_quad() __asm__("func_001F5450");

s32 draw_transition_overlay(struct TransitionState *transition) __asm__("FUN_0021fc68");

s32 draw_transition_overlay(struct TransitionState *transition) {
    if (transition->unk44 < 2) {
        return 0;
    }
    draw_textured_quad(0, 0, fs_aa_buffer.target_width, fs_aa_buffer.target_height, 0, 0, transition->unk38, transition->unk3C,
                       ((u64)0x8080 << 16) | 0x8080, level_map_selection.tex0);
    return 0x10;
}

extern __typeof__(draw_transition_overlay) func_0021FC68 __attribute__((alias("FUN_0021fc68")));
