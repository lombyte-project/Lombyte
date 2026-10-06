#include "types.h"
#include "asm.h"

#include "types.h"

struct CameraEnvironmentRegion {
    u8 pad0[0x50];
    s32 flags;
    s32 negative_color;
    s32 positive_color;
    f32 negative_depth_start;
    f32 negative_value_start;
    f32 negative_depth_end;
    f32 negative_value_end;
    f32 positive_depth_start;
    f32 positive_value_start;
    f32 positive_depth_end;
    f32 positive_value_end;
    u8 pad7C[4];
};

extern struct CameraEnvironmentRegion camera_environment_regions[] __asm__("D_0019ADC0");
extern u8 fog_color_red __asm__("D_0015F484");
extern u8 fog_color_green __asm__("D_0015F485");
extern u8 fog_color_blue __asm__("D_0015F486");
extern f32 fog_near_distance __asm__("D_0015F488");
extern f32 fog_far_distance __asm__("D_0015F48C");
extern f32 fog_near_intensity __asm__("D_0015F490");
extern f32 fog_far_intensity __asm__("D_0015F494");
extern s32 find_camera_environment_region(void *, f32 *, s32 *) __asm__("func_00212C28");
extern s32 convert_float_to_word(f32) __asm__("func_001FA6D0");

/* The color blend uses weights summing to 255 and shifts the sums by eight. */
void update_camera_environment_from_regions(void *position) __asm__("FUN_001ee4b0");

void update_camera_environment_from_regions(void *position) {
    f32 blend;
    s32 region_index;
    struct CameraEnvironmentRegion *region;
    u32 positive_weight;
    u32 negative_weight;
    s32 negative_color;
    s32 positive_color;
    f32 inverse_blend;
    u32 red;
    u32 green;
    u32 positive_red;
    u32 negative_red;
    u32 positive_green;
    u32 negative_green;
    u32 positive_blue;
    u32 negative_blue;

    if (find_camera_environment_region(position, &blend, &region_index) == 0) {
        return;
    }
    region = &camera_environment_regions[region_index];
    if (!(region->flags & 2)) {
        return;
    }
    positive_weight = convert_float_to_word(blend * 255.0f);
    negative_weight = 255 - positive_weight;
    inverse_blend = 1.0f - blend;
    positive_color = region->positive_color;
    negative_color = region->negative_color;
    positive_red = (positive_color & 0xFF) * positive_weight;
    positive_green = ((positive_color >> 8) & 0xFF) * positive_weight;
    positive_blue = (positive_color >> 16) & 0xFF;
    negative_red = (negative_color & 0xFF) * negative_weight;
    negative_green = ((negative_color >> 8) & 0xFF) * negative_weight;
    negative_blue = (negative_color >> 16) & 0xFF;
    red = (s32)(positive_red + negative_red) >> 8;
    green = (s32)(positive_green + negative_green) >> 8;
    fog_color_blue = (s32)(positive_blue * positive_weight + negative_blue * negative_weight) >> 8;
    fog_color_red = red;
    fog_color_green = green;
    fog_near_distance =
        (region->positive_depth_start * blend + region->negative_depth_start * inverse_blend) *
        1024.0f;
    fog_far_distance =
        (region->positive_depth_end * blend + region->negative_depth_end * inverse_blend) * 1024.0f;
    fog_near_intensity = 255.0f - (region->positive_value_start * blend +
                                   region->negative_value_start * inverse_blend) *
                                      255.0f;
    fog_far_intensity =
        255.0f -
        (region->positive_value_end * blend + region->negative_value_end * inverse_blend) * 255.0f;
}
extern __typeof__(update_camera_environment_from_regions) func_001EE4B0
    __attribute__((alias("FUN_001ee4b0")));
