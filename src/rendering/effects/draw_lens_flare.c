#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/view/project_to_screen.h"
#include "rnc/math/fast_math.h"

/* Lens flare: 16 sprites along the line from the flare source's screen
   position through the screen centre, faded by distance and by how far
   the source is off centre. */

#include "rnc/rendering/effects/draw_lens_flare.h"

#include "rnc/rendering/screen.h"

extern s64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void
    draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64) __asm__("FUN_001f5450");

void draw_lens_flare(void) __asm__("FUN_001edc50");

void draw_lens_flare(void) {
    f32 scr[4];
    f32 ctr[4];
    f32 screen_center_delta[4];
    f32 sprite_center_offset[4];
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
    s32 quad_size;
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
        sprite_center_offset[0] = screen_center_delta[0] * effect_ribbon.dist[i];
        sprite_center_offset[1] = screen_center_delta[1] * effect_ribbon.dist[i];
        at[0] = ctr[0] + sprite_center_offset[0];
        at[1] = ctr[1] + sprite_center_offset[1];
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
        quad_size = truncate_float_to_s32(size * effect_ribbon.scale[i]);
        half = quad_size >> 1;
        x = truncate_float_to_s32(at[0] - half);
        y = truncate_float_to_s32(at[1] - half);
        draw_textured_quad(x, y, quad_size, quad_size, 0, 0, size, size, rgba, tex);
    }
    effect_ribbon.intensity = 0.0f;
}

extern __typeof__(draw_lens_flare) func_001EDC50 __attribute__((alias("FUN_001edc50")));

LensFlare effect_ribbon = {0};
