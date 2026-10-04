#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00208508/FUN_00208508.s", FUN_00208508);
#else
#include "types.h"

#include "sda.h"

struct MapMarker {
    f32 x;
    f32 y;
    s32 texture_group;
    s32 texture_variant_or_color;
};
struct MapMarkerState {
    u8 pad0[0x1C];
    struct MapMarker *markers;
    u8 pad20[0x90];
    s32 marker_count;
    u8 padB4[0x170];
    s32 selected_level;
};
struct MapTextureTables {
    u8 pad0[0x20];
    s16 *references;
    u8 *textures;
};
extern struct MapMarkerState map_marker_state __asm__("D_001A00F0");
extern struct MapMarkerState selected_map_state __asm__("D_001A00F0") __attribute__((section(".data")));
extern struct MapTextureTables map_texture_tables __asm__("D_0019A3E8") NOT_SDA;
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
    s32 texture_index;
    s16 *references;
    u8 *texture;
    u8 width_log2;
    u8 height_log2;

    marker = map_marker_state.markers;
    remaining = map_marker_state.marker_count - 1;
    for (; remaining != -1; remaining--) {
        world_to_map_coords(&normalized_x, &normalized_y, current_level_index, marker->x, marker->y);
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

                texture_index = resolve_indexed_texture_variant(marker->texture_group, marker->texture_variant_or_color);
                references = map_texture_tables.references;
                texture = map_texture_tables.textures + references[texture_index * 2 + 1] * 8;
                width_log2 = texture[6];
                height_log2 = texture[7];
                scale = (2.0f * marker_scale_by_level[selected_map_state.selected_level] + 5.0f) / 13.0f;
                append_indexed_screen_sprite(texture_index,
                    screen_x - (s32)(scale * (f32)(1 << (width_log2 + 3))),
                    screen_y - (s32)(scale * (f32)(1 << (height_log2 + 3))),
                    (s32)(scale * (f32)(1 << (width_log2 + 4))),
                    (s32)(scale * (f32)(1 << (height_log2 + 4))), 0x80);
            }
        }
        marker++;
    }
}

extern __typeof__(draw_map_markers) func_00208508 __attribute__((alias("FUN_00208508")));

#endif /* NON_MATCHING */
