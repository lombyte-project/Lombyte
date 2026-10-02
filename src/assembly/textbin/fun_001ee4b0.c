#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001ee4b0/FUN_001ee4b0.s", FUN_001ee4b0);
#else
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

extern struct CameraEnvironmentRegion D_0019ADC0[];
extern u8 D_0015F484;
extern u8 D_0015F485;
extern u8 D_0015F486;
extern f32 D_0015F488;
extern f32 D_0015F48C;
extern f32 D_0015F490;
extern f32 D_0015F494;
extern s32 find_camera_environment_region(void *, f32 *, s32 *) __asm__("func_00212C28");
extern s32 convert_float_to_word(f32) __asm__("func_001FA6D0");

void update_camera_environment_from_regions(void *position) __asm__("FUN_001ee4b0");

void update_camera_environment_from_regions(void *position) {
    f32 blend;
    s32 region_index;
    struct CameraEnvironmentRegion *region;
    u32 positive_weight;
    u32 negative_weight;
    s32 positive_color;
    s32 negative_color;
    f32 inverse_blend;
    s32 red;
    s32 green;

    if (find_camera_environment_region(position, &blend, &region_index) == 0) {
        return;
    }
    region = &D_0019ADC0[region_index];
    if (!(region->flags & 2)) {
        return;
    }
    positive_weight = convert_float_to_word(blend * 255.0f);
    negative_weight = 255 - positive_weight;
    inverse_blend = 1.0f - blend;
    positive_color = region->positive_color;
    negative_color = region->negative_color;
    red = ((positive_color & 0xFF) * positive_weight + (negative_color & 0xFF) * negative_weight) >> 8;
    green = (((positive_color >> 8) & 0xFF) * positive_weight + ((negative_color >> 8) & 0xFF) * negative_weight) >> 8;
    D_0015F486 = (((positive_color >> 16) & 0xFF) * positive_weight + ((negative_color >> 16) & 0xFF) * negative_weight) >> 8;
    D_0015F484 = red;
    D_0015F485 = green;
    D_0015F488 = (region->positive_depth_start * blend + region->negative_depth_start * inverse_blend) * 1024.0f;
    D_0015F48C = (region->positive_depth_end * blend + region->negative_depth_end * inverse_blend) * 1024.0f;
    D_0015F490 = 255.0f - (region->positive_value_start * blend + region->negative_value_start * inverse_blend) * 255.0f;
    D_0015F494 = 255.0f - (region->positive_value_end * blend + region->negative_value_end * inverse_blend) * 255.0f;
}
#endif /* NON_MATCHING */
