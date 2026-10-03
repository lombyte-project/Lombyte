#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001f33b8/FUN_001f33b8.s", FUN_001f33b8);
#else
#include "types.h"

struct ProjectionScreenState {
    s32 viewport_width;
    s32 viewport_height;
    s32 half_width;
    s32 half_height;
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
void configure_graphics_projection(s32 viewport_width, s32 viewport_height, f32 horizontal_fov, f32 fog_near_distance, f32 fog_far_distance, f32 fog_near_intensity, f32 fog_far_intensity) __asm__("FUN_001f33b8");

void configure_graphics_projection(s32 viewport_width, s32 viewport_height, f32 horizontal_fov, f32 fog_near_distance, f32 fog_far_distance, f32 fog_near_intensity, f32 fog_far_intensity) {
    struct ProjectionScreenState *screen;
    s64 saved_width;
    s32 half_width;
    s32 half_height;
    s32 left_origin;
    s32 top_origin;
    s32 right_extent;
    s32 bottom_extent;
    f32 half_scale;
    f32 projection_half_width;
    f32 projection_half_height;

    screen = &screen_offsets;
    saved_width = viewport_width;
    half_height = (s32)viewport_height >> 1;
    half_width = (s32)viewport_width >> 1;
    bottom_extent = (half_height + 0x800) << 4;
    top_origin = 0x800 - half_height;
    right_extent = half_width + 0x800;
    left_origin = 0x800 - half_width;
    screen->bottom_extent = bottom_extent;
    view_context.horizontal_fov = horizontal_fov;
    screen->viewport_width = (s32)saved_width;
    screen->left_origin = left_origin << 4;
    screen->top_origin = top_origin << 4;
    screen->right_extent = right_extent << 4;
    view_context.near_clip = 32.0f;
    view_context.far_clip = 524288.0f;
    screen->viewport_height = viewport_height;
    half_scale = 0.5f;
    screen->half_height = half_height;
    screen->half_width = half_width;
    projection_half_width = convert_integer_to_float(viewport_width) * half_scale;
    view_context.projection_half_width = projection_half_width;
    projection_half_height = convert_integer_to_float(viewport_height) * half_scale;
    view_context.fog_far_intensity = fog_far_intensity;
    view_context.fog_near_distance = fog_near_distance;
    view_context.fog_far_distance = fog_far_distance;
    view_context.fog_near_intensity = fog_near_intensity;
    view_context.screen_scale_y = projection_half_height * 4.0f;
    view_context.screen_scale_x = view_context.projection_half_width * 4.0f;
    view_context.projection_half_height = projection_half_height;
    update_view_context();
}

extern __typeof__(configure_graphics_projection) func_001F33B8 __attribute__((alias("FUN_001f33b8")));

#endif /* NON_MATCHING */
