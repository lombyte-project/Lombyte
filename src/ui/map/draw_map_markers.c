/* Ported from rac1-decomp (src/game/menu.c: func_00208D38). */
#include "types.h"
#include "sda.h"

#include "rnc/ui/map/map_icon.h"
#include "rnc/ui/map/map_state.h"

extern s32 current_level_index __asm__("D_0015ED84") MACRO_ADDR;
extern s32 marker_half_size __asm__("D_0015FDB0") MACRO_ADDR;
extern char D_0019A3E8_raw[] __asm__("D_0019A3E8");
extern void world_to_map_coords(f32 x, f32 y, f32 *outx, f32 *outy, s32 view) __asm__("func_00208408");
extern s32 resolve_indexed_texture_variant(s32, s32) __asm__("func_001FF960");
extern void append_screen_sprite(s32, s32, s32, s32, s64, s32) __asm__("func_00200E08");
extern void append_indexed_screen_sprite(s32, s32, s32, s32, s32, s32) __asm__("func_00200080");

void draw_map_markers(s32 left, s32 top, s32 right, s32 bottom) __asm__("FUN_00208508");

void draw_map_markers(s32 left, s32 top, s32 right, s32 bottom) {
    f32 normalized_x;
    f32 normalized_y;
    int remaining = level_map_selection.marker_count;
    struct MapMarker *marker = level_map_selection.markers;

    while (remaining-- != 0) {
        s32 screen_x;
        s32 screen_y;

        world_to_map_coords(marker->x, marker->y, &normalized_x, &normalized_y,
                            current_level_index);
        screen_x = left + (s32)((f32)(right - left) * normalized_x);
        screen_y = top + (s32)((f32)(bottom - top) * normalized_y);
        if (screen_x < -0x199 || screen_y < -0x199 || screen_x >= 0x219A || screen_y >= 0x1B9A) {
            marker++;
            continue;
        }
        if (marker->texture_group == -1) {
            append_screen_sprite(screen_x - marker_half_size, screen_y - marker_half_size,
                                 screen_x + marker_half_size, screen_y + marker_half_size,
                                 (u32)marker->texture_variant_or_color, 1);
        } else {
            s32 texture_index = resolve_indexed_texture_variant(
                marker->texture_group, marker->texture_variant_or_color);
            char *hud = D_0019A3E8_raw;
            u8 *texture = (u8 *)(*(char **)(hud + 0x24) +
                                 *(s16 *)(*(char **)(hud + 0x20) + texture_index * 4 + 2) * 8);
            f32 scale = (2.0f * level_map_selection.zoom[level_map_selection.level] + 5.0f) / 13.0f;
            f32 width = scale * (f32)(1 << (texture[6] + 4));

            append_indexed_screen_sprite(
                texture_index, screen_x - (s32)(scale * (f32)(1 << (texture[6] + 3))),
                screen_y - (s32)(scale * (f32)(1 << (texture[7] + 3))), (s32)width,
                (s32)(scale * (f32)(1 << (texture[7] + 4))), 0x80);
        }
        marker++;
    }
}

extern __typeof__(draw_map_markers) func_00208508 __attribute__((alias("FUN_00208508")));
