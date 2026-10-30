/* Ported from rac1-decomp (src/game/draw.c: SetScreenSize). */
#include "types.h"
#include "rnc/rendering/screen.h"
#include "rnc/rendering/view.h"
#include "rnc/math/fast_math.h"

extern void update_view_context(void) __asm__("func_001F2D98");
void        configure_graphics_projection(s32 viewport_width,
                                          s32 viewport_height,
                                          f32 horizontal_fov,
                                          f32 fog_near_distance,
                                          f32 fog_far_distance,
                                          f32 fog_near_intensity,
                                          f32 fog_far_intensity) __asm__("FUN_001f33b8");

void configure_graphics_projection(s32 viewport_width,
                                   s32 viewport_height,
                                   f32 horizontal_fov,
                                   f32 fog_near_distance,
                                   f32 fog_far_distance,
                                   f32 fog_near_intensity,

                                   f32 fog_far_intensity) {
    struct View *context = &view_context;
    s32          half_width = viewport_width >> 1;
    s32          half_height = viewport_height >> 1;
    f32          projection_half_height;

    screen_extent.width = viewport_width;
    screen_extent.height = viewport_height;
    screen_extent.half_width = half_width;
    screen_extent.half_height = half_height;
    screen_extent.left = (0x800 - half_width) << 4;
    screen_extent.top = (0x800 - half_height) << 4;
    screen_extent.right = (half_width + 0x800) << 4;
    screen_extent.bottom = (half_height + 0x800) << 4;
    context->fov.f[0] = horizontal_fov;
    context->near_clip = 32.0f;
    context->far_clip = 524288.0f;
    context->half_width = convert_integer_to_float(viewport_width) * 0.5f;
    projection_half_height = convert_integer_to_float(viewport_height) * 0.5f;
    context->half_height = projection_half_height;
    context->scr_x = context->half_width * 4.0f;
    context->scr_y = projection_half_height * 4.0f;
    context->fog_near_dist = fog_near_distance;
    context->fog_far_dist = fog_far_distance;
    context->fog_near_int = fog_near_intensity;
    context->fog_far_int = fog_far_intensity;
    update_view_context();
}

extern __typeof__(configure_graphics_projection) func_001F33B8
    __attribute__((alias("FUN_001f33b8")));
