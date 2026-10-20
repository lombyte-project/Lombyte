#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/camera/camera_globals.h"

/* Lens flare: 16 sprites along the line from the flare source's screen
   position through the screen centre, faded by distance and by how far
   the source is off centre. */

typedef struct {
    u8 pad_0[0x10];
    f32 pos[4];
    u8 flags;
} FlareMoby;

typedef struct {
    FlareMoby *moby;
    s32 state;
    f32 intensity;
    s32 pad_c;
    s16 alpha[16];
    s16 tex[16];
    s32 color[16];
    f32 dist[16];
    f32 scale[16];
} LensFlare;


#include "rnc/rendering/screen.h"
extern LensFlare effect_ribbon __asm__("D_00187300");

extern f32 fast_vec_dist(void *, void *) __asm__("FUN_001f9b48");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern float absolute_float(float input) __asm__("func_001F99C0");
extern void project_to_screen(f32 *, void *) __asm__("FUN_001f2070");
extern s64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64,
                               s64) __asm__("FUN_001f5450");

void draw_lens_flare(void) __asm__("FUN_001edc50");

void draw_lens_flare(void) {
    f32 scr[4];
    f32 ctr[4];
    f32 screen_center_delta[4];
    f32 off[4];
    f32 at[4];
    f32 dist;
    f32 fade;
    f32 fx;
    f32 fy;
    f32 half;
    s32 i;
    s32 size;
    s64 tex;
    s32 rgba;
    s32 w;
    s32 x;
    s32 y;

    if (effect_ribbon.state != 1 || (effect_ribbon.moby->flags & 0x80)) {
        effect_ribbon.state = 0;
        return;
    }
    if (effect_ribbon.intensity <= 0.0f) {
        return;
    }
    dist = fast_vec_dist(camera_position, effect_ribbon.moby->pos);
    ctr[0] = convert_integer_to_float(screen_extent.half_width);
    ctr[1] = convert_integer_to_float(screen_extent.half_height);
    project_to_screen(scr, effect_ribbon.moby->pos);
    scr[0] = (scr[0] - convert_integer_to_float(screen_extent.left)) * 0.0625f;
    scr[1] = (scr[1] - convert_integer_to_float(screen_extent.top)) * 0.0625f;
    screen_center_delta[0] = ctr[0] - scr[0];
    screen_center_delta[1] = ctr[1] - scr[1];
    for (i = 0; i < 16; i++) {
        tex = get_effect_texture(effect_ribbon.tex[i] + 6);
        size = (i != 0) ? 0x20 : 0x40;
        off[0] = screen_center_delta[0] * effect_ribbon.dist[i];
        off[1] = screen_center_delta[1] * effect_ribbon.dist[i];
        at[0] = ctr[0] + off[0];
        at[1] = ctr[1] + off[1];
        fade = dist - 5.0f;
        if (fade > 1.0f) {
            fade = 1.0f;
        } else if (fade < 0.0f) {
            fade = 0.0f;
        }
        fx = absolute_float(screen_center_delta[0]) / ctr[0] * -5.0f + 5.5f;
        if (fx > 1.0f) {
            fx = 1.0f;
        } else if (fx < 0.0f) {
            fx = 0.0f;
        }
        fy = absolute_float(screen_center_delta[1]) / ctr[1] * -5.0f + 5.5f;
        if (fy > 1.0f) {
            fy = 1.0f;
        } else if (fy < 0.0f) {
            fy = 0.0f;
        }
        fade = fade * (fx * fy);
        rgba = (truncate_float_to_s32(fade * effect_ribbon.alpha[i]) << 24) |
               (effect_ribbon.color[i] & 0xFFFFFF);
        w = truncate_float_to_s32(size * effect_ribbon.scale[i]);
        half = w >> 1;
        x = truncate_float_to_s32(at[0] - half);
        y = truncate_float_to_s32(at[1] - half);
        draw_textured_quad(x, y, w, w, 0, 0, size, size, rgba, tex);
    }
    effect_ribbon.intensity = 0.0f;
}

extern __typeof__(draw_lens_flare) func_001EDC50 __attribute__((alias("FUN_001edc50")));
