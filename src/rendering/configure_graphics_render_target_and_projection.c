#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"

/* Ported from rac1-decomp (src/game/draw.c:func_001F7A50), which matched our retail at the instruction level before the address space was translated. */

/* data paired by order (exact), deltas ['0x100']: D_001519EE->D_001518EE, D_0015EF8C->D_0015EE8C */

extern void setup_render_target(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_001fb440");
extern void configure_graphics_projection(s32 arg0, s32 arg1, f32 fparg0, f32 fparg1, f32 fparg2,
                                          f32 fparg3, f32 fparg4) __asm__("FUN_001f33b8");
extern s16 D_001518EE[];
void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

void configure_graphics_render_target_and_projection(s32 tw, s32 th, s32 fixed, f32 scale) __asm__("FUN_001f7888");

void configure_graphics_render_target_and_projection(s32 tw, s32 th, s32 fixed, f32 scale) {
    s32 base;

    if (fixed != 0) {
        base = D_001518EE[0] << 13;
    } else {
        s32 t = tw + th;
        if (t > 16) {
            t = 16;
        }
        base = gs_texture_allocation_base - (4 << t);
        base = (base >> 13) << 13;
    }
    setup_render_target(tw, th, base);
    configure_graphics_projection(1 << tw, 1 << th, scale, 0.0f, 524288.0f, 255.0f, 0.0f);
    if (fixed != 0) {
        vu1_add_g_sregister(0x47, 0);
    } else {
        vu1_add_g_sregister(0x47, 0x30000);
    }
    vu1_add_g_sregister(0x42, 0x8000000044L);
}

extern __typeof__(configure_graphics_render_target_and_projection) func_001F7888 __attribute__((alias("FUN_001f7888")));
