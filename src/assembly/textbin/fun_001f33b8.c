#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f33b8/FUN_001f33b8.s", FUN_001f33b8);
#else
#include "types.h"

struct ProjectionScreenState {
    s32 viewport_width;
    s32 viewport_height;
    s32 half_height;
    s32 half_width;
    s32 left_origin;
    s32 top_origin;
    s32 right_extent;
    s32 bottom_extent;
};

struct ProjectionConfiguration {
    u8 pad_0[0xA0];
    f32 near_clip;
    f32 far_clip;
    u8 pad_A8[0x8];
    f32 horizontal_fov;
    u8 pad_B4[0x14C];
    f32 projection_half_width;
    f32 projection_half_height;
    f32 screen_scale_x;
    f32 screen_scale_y;
    u8 pad_210[0x8];
    f32 fog_near_distance;
    f32 fog_far_distance;
    u8 pad_220[0x8];
    f32 fog_near_intensity;
    f32 fog_far_intensity;
};

extern struct ProjectionScreenState screen_offsets __asm__("D_0013E500");
extern struct ProjectionConfiguration view_context __asm__("D_0018CD00");
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
    struct ProjectionConfiguration *context = &view_context;
    f32 projection_half_height;

    half_height = viewport_height >> 1;
    half_width = viewport_width >> 1;
    screen_offsets.half_height = half_height;
    screen_offsets.half_width = half_width;
    screen_offsets.left_origin = (s32)((u32)(0x800 - half_width) << 4);
    screen_offsets.bottom_extent = (s32)((u32)(half_height + 0x800) << 4);
    screen_offsets.top_origin = (s32)((u32)(0x800 - half_height) << 4);
    screen_offsets.right_extent = (s32)((u32)(half_width + 0x800) << 4);
    context->horizontal_fov = horizontal_fov;
    context->near_clip = 32.0f;
    context->far_clip = 524288.0f;
    screen_offsets.viewport_height = viewport_height;
    screen_offsets.viewport_width = viewport_width;
    context->projection_half_width = convert_integer_to_float(viewport_width) * 0.5f;
    projection_half_height = convert_integer_to_float(viewport_height) * 0.5f;
    context->projection_half_height = projection_half_height;
    context->screen_scale_x = context->projection_half_width * 4.0f;
    context->screen_scale_y = projection_half_height * 4.0f;
    context->fog_far_intensity = fog_far_intensity;
    context->fog_near_distance = fog_near_distance;
    context->fog_far_distance = fog_far_distance;
    context->fog_near_intensity = fog_near_intensity;
    update_view_context();
}

extern __typeof__(configure_graphics_projection) func_001F33B8
    __attribute__((alias("FUN_001f33b8")));

#endif /* NON_MATCHING */
