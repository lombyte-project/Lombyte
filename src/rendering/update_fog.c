#include "types.h"
#include "rnc/rendering/view.h"

/* D_001872D4 is the fog-source selector, a field 0x2D4 into this block. */
struct FogMode {
    u8 pad0[0x2D4];
    s32 use_level_fog;
};

extern struct FogMode view_proj_clip __asm__("D_00187000");

extern u8 D_001610C4;
extern u8 water_fog_g __asm__("D_001610C5");
extern u8 water_fog_b __asm__("D_001610C6");
extern f32 water_fog_near_dist __asm__("D_001610C8");
extern f32 water_fog_far_dist __asm__("D_001610CC");
extern f32 water_fog_near_intensity __asm__("D_001610D0");
extern f32 water_fog_far_intensity __asm__("D_001610D4");
extern u8 D_0015F484;
extern u8 D_0015F485;
extern u8 D_0015F486;
extern f32 D_0015F488;
extern f32 D_0015F48C;
extern f32 D_0015F490;
extern f32 D_0015F494;
extern s32 part_clip_dist __asm__("D_001600BC");
extern s32 level_fog_mode __asm__("D_0015F498");
extern void update_view_context(void) __asm__("func_001F2D98");

void update_fog(void) __asm__("FUN_001f2588");

void update_fog(void) {
    if (view_proj_clip.use_level_fog != 0) {
        view_context.fog_r = D_001610C4;
        view_context.fog_g = water_fog_g;
        view_context.fog_b = water_fog_b;
        view_context.fog_near_dist = water_fog_near_dist;
        view_context.fog_far_dist = water_fog_far_dist;
        view_context.fog_near_int = water_fog_near_intensity;
        view_context.fog_far_int = water_fog_far_intensity;
        part_clip_dist = 0x40000;
    } else {
        view_context.fog_r = D_0015F484;
        view_context.fog_g = D_0015F485;
        view_context.fog_b = D_0015F486;
        view_context.fog_near_dist = D_0015F488;
        view_context.fog_far_dist = D_0015F48C;
        view_context.fog_near_int = D_0015F490;
        view_context.fog_far_int = D_0015F494;
        part_clip_dist = 0x1F4000;
    }
    update_view_context();
    level_fog_mode = 0;
}

extern __typeof__(update_fog) func_001F2588 __attribute__((alias("FUN_001f2588")));
