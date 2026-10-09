#include "types.h"
#include "rnc/rendering/view.h"

/* D_001872D4 is the fog-source selector, a field 0x2D4 into this block. */
struct FogMode {
    u8 pad0[0x2D4];
    s32 use_level_fog;
};

extern struct FogMode D_00187000;

extern u8 D_001610C4;
extern u8 D_001610C5;
extern u8 D_001610C6;
extern f32 D_001610C8;
extern f32 D_001610CC;
extern f32 D_001610D0;
extern f32 D_001610D4;
extern u8 D_0015F484;
extern u8 D_0015F485;
extern u8 D_0015F486;
extern f32 D_0015F488;
extern f32 D_0015F48C;
extern f32 D_0015F490;
extern f32 D_0015F494;
extern s32 D_001600BC;
extern s32 D_0015F498;
extern void update_view_context(void) __asm__("func_001F2D98");

void update_fog(void) __asm__("FUN_001f2588");

void update_fog(void) {
    if (D_00187000.use_level_fog != 0) {
        view_context.fog_r = D_001610C4;
        view_context.fog_g = D_001610C5;
        view_context.fog_b = D_001610C6;
        view_context.fog_near_dist = D_001610C8;
        view_context.fog_far_dist = D_001610CC;
        view_context.fog_near_int = D_001610D0;
        view_context.fog_far_int = D_001610D4;
        D_001600BC = 0x40000;
    } else {
        view_context.fog_r = D_0015F484;
        view_context.fog_g = D_0015F485;
        view_context.fog_b = D_0015F486;
        view_context.fog_near_dist = D_0015F488;
        view_context.fog_far_dist = D_0015F48C;
        view_context.fog_near_int = D_0015F490;
        view_context.fog_far_int = D_0015F494;
        D_001600BC = 0x1F4000;
    }
    update_view_context();
    D_0015F498 = 0;
}

extern __typeof__(update_fog) func_001F2588 __attribute__((alias("FUN_001f2588")));
