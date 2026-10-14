#include "types.h"
#include "rnc/rendering/view.h"
#include "rnc/data_labels.h"
#include "rnc/rendering/render_globals.h"

/* D_001872D4 is the fog-source selector, a field 0x2D4 into this block. */
struct FogMode {
    u8 pad0[0x2D4];
    s32 use_level_fog;
};

extern struct FogMode view_proj_clip __asm__("D_00187000");

extern u8 D_001610C4;
extern f32 water_fog_near_dist __asm__("D_001610C8");
extern f32 water_fog_far_dist __asm__("D_001610CC");
extern f32 water_fog_near_intensity __asm__("D_001610D0");
extern f32 water_fog_far_intensity __asm__("D_001610D4");
extern s32 level_fog_mode __asm__("D_0015F498");
extern void update_view_context(void) __asm__("func_001F2D98");

void update_fog(void) __asm__("FUN_001f2588");

extern u8 D_001610C5;
extern u8 D_001610C6;
void update_fog(void) {
    if (view_proj_clip.use_level_fog != 0) {
        view_context.fog_r = D_001610C4;
        view_context.fog_g = D_001610C5;
        view_context.fog_b = D_001610C6;
        view_context.fog_near_dist = water_fog_near_dist;
        view_context.fog_far_dist = water_fog_far_dist;
        view_context.fog_near_int = water_fog_near_intensity;
        view_context.fog_far_int = water_fog_far_intensity;
        part_clip_dist = 0x40000;
    } else {
        view_context.fog_r = fog_color_red;
        view_context.fog_g = fog_color_green;
        view_context.fog_b = fog_color_blue;
        view_context.fog_near_dist = fog_near_distance;
        view_context.fog_far_dist = fog_far_distance;
        view_context.fog_near_int = fog_near_intensity;
        view_context.fog_far_int = fog_far_intensity;
        part_clip_dist = 0x1F4000;
    }
    update_view_context();
    level_fog_mode = 0;
}

extern __typeof__(update_fog) func_001F2588 __attribute__((alias("FUN_001f2588")));
