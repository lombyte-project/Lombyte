#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f33b8/FUN_001f33b8.s", FUN_001f33b8);
#else
#include "types.h"
#include "rnc/rendering/screen.h"
#include "rnc/rendering/view.h"

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void update_view_context(void) __asm__("func_001F2D98");
void configure_graphics_projection(s32 viewport_width, s32 viewport_height, f32 horizontal_fov,
                                   f32 fog_near_distance, f32 fog_far_distance,
                                   f32 fog_near_intensity,
                                   f32 fog_far_intensity) __asm__("FUN_001f33b8");
void configure_graphics_projection(s32 viewport_width, s32 viewport_height, f32 horizontal_fov,
                                   f32 fog_near_distance, f32 fog_far_distance,
                                   f32 fog_near_intensity, f32 fog_far_intensity) {
    s32 half_width;
    s32 half_height;
    struct View *context = &view_context;
    f32 projection_half_height;
    s32 *bottom_extent = &D_0013E500.bottom;

    half_height = viewport_height >> 1;
    half_width = viewport_width >> 1;
    D_0013E500.half_width = half_width;
    D_0013E500.half_height = half_height;
    D_0013E500.left = (s32)((u32)(0x800 - half_width) << 4);
    *bottom_extent = (s32)((u32)(half_height + 0x800) << 4);
    D_0013E500.top = (s32)((u32)(0x800 - half_height) << 4);
    D_0013E500.right = (s32)((u32)(half_width + 0x800) << 4);
    context->fov.f[0] = horizontal_fov;
    context->far_clip = 524288.0f;
    context->near_clip = 32.0f;
    D_0013E500.height = viewport_height;
    D_0013E500.width = viewport_width;
    context->half_width = convert_integer_to_float(viewport_width) * 0.5f;
    projection_half_height = convert_integer_to_float(viewport_height) * 0.5f;
    context->half_height = projection_half_height;
    context->scr_x = context->half_width * 4.0f;
    context->scr_y = projection_half_height * 4.0f;
    context->fog_far_int = fog_far_intensity;
    context->fog_near_int = fog_near_intensity;
    context->fog_near_dist = fog_near_distance;
    context->fog_far_dist = fog_far_distance;
    update_view_context();
}

extern __typeof__(configure_graphics_projection) func_001F33B8
    __attribute__((alias("FUN_001f33b8")));

#endif /* NON_MATCHING */
