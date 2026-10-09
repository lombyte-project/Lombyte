#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00208508/FUN_00208508.s", FUN_00208508);
#else
#include "types.h"

#include "sda.h"

#include "rnc/ui/hud/hud_state.h"
#include "rnc/ui/map/map_state.h"
#include "rnc/ui/map/map_icon.h"

extern struct MapState selected_map_state __asm__("D_001A00F0") __attribute__((section(".data")));
extern s32 current_level_index __asm__("D_0015ED84") MACRO_ADDR;
extern s32 marker_half_size __asm__("D_0015FDB0") MACRO_ADDR;
extern f32 marker_scale_by_level[] __asm__("D_001A01A4");
extern void world_to_map_coords(f32 *, f32 *, s32, f32, f32) __asm__("func_00208408");
extern s32 resolve_indexed_texture_variant(s32, s32) __asm__("func_001FF960");
extern void append_screen_sprite(s32, s32, s32, s32, s64, s64) __asm__("func_00200E08");
extern void append_indexed_screen_sprite(s32, s32, s32, s32, s32, s32) __asm__("func_00200080");

void draw_map_markers(s32 left, s32 top, s32 right, s32 bottom) __asm__("FUN_00208508");

void draw_map_markers(s32 left, s32 top, s32 right, s32 bottom) {
    struct MapMarker *marker;
    f32 normalized_x;
    f32 normalized_y;
    s32 offset_x;
    s32 offset_y;
    s32 screen_x;
    s32 screen_y;
    s32 remaining;
    s32 one;
    s32 texture_index;
    s16 *references;
    u8 *texture;
    u8 width_log2;
    u8 height_log2;

    one = 1;
    remaining = level_map_selection.marker_count - 1;
    marker = level_map_selection.markers;
    for (; remaining != -1; remaining--) {
        world_to_map_coords(&normalized_x, &normalized_y, current_level_index, marker->x,
                            marker->y);
        offset_x = (s32)((f32)(right - left) * normalized_x);
        offset_y = (s32)((f32)(bottom - top) * normalized_y);
        screen_x = left + offset_x;
        screen_y = top + offset_y;
        if (screen_x >= -0x199 && screen_y >= -0x199 && screen_x < 0x219A && screen_y < 0x1B9A) {
            if (marker->texture_group == -1) {
                append_screen_sprite(screen_x - marker_half_size, screen_y - marker_half_size,
                                     screen_x + marker_half_size, screen_y + marker_half_size,
                                     (u32)marker->texture_variant_or_color, 1);
            } else {
                f32 scale;
                f32 full_width_scaled;

                texture_index = resolve_indexed_texture_variant(marker->texture_group,
                                                                marker->texture_variant_or_color);
                references = (s16 *)hud_state.frame_refs;
                texture = (u8 *)hud_state.image_pages + references[texture_index * 2 + 1] * 8;
                width_log2 = texture[6];
                height_log2 = texture[7];
                scale = (2.0f * marker_scale_by_level[selected_map_state.level] + 5.0f) /
                        13.0f;
                full_width_scaled = scale * (f32)(one << (width_log2 + 4));
                append_indexed_screen_sprite(
                    texture_index, screen_x - (s32)(scale * (f32)(one << (width_log2 + 3))),
                    screen_y - (s32)(scale * (f32)(one << (height_log2 + 3))),
                    (s32)full_width_scaled, (s32)(scale * (f32)(one << (height_log2 + 4))), 0x80);
            }
        }
        marker++;
    }
}

extern __typeof__(draw_map_markers) func_00208508 __attribute__((alias("FUN_00208508")));

#endif /* NON_MATCHING */
