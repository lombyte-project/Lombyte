#include "types.h"
#include "asm.h"
#include "sda.h"
#include "rnc/ui/map/level_map.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001fd748/FUN_001fd748.s", FUN_001fd748);
#else
#include "types.h"

#include "rnc/ui/map/map_state.h"
#include "rnc/ui/map/level_map.h"

#include "rnc/gameplay/state/level_state.h"
extern s32 pal_mode __asm__("D_0015ED80");
extern s32 game_frame_counter __asm__("D_0015F438");
extern s32 large_font_height __asm__("D_0015F690") __attribute__((sda));
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern s32 measure_text_width_regular(u8 *, s32) __asm__("func_001F6250");
extern void font_print_large(s32, s32, u64, u8 *, s32) __asm__("func_001F6530");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern f32 func_001F9B20(f32 *);
extern u8 *get_help_message_text(s32) __asm__("func_001FDD10");
extern s32 get_icon_frame(s32, s32) __asm__("func_001FF960");
extern void draw_hud_sprite(s32, s32, s32, s32, s32, s32) __asm__("func_001FFC30");
extern void draw_hud_sprite_uv(s32, s32, s32, s32, s32, s32, s32, s32) __asm__("func_00200258");
extern void draw_hud_rect(s32, s32, s32, s32, u64, s32) __asm__("func_00200C80");
extern void append_screen_rect_packet(s32, s32, s32, s32, u64, s32) __asm__("func_00200E08");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");

void draw_level_selection_map(s32 left, s32 right, s32 top, s32 bottom) __asm__("FUN_001fd748");

void draw_level_selection_map(s32 left, s32 right, s32 top, s32 bottom) {
    f32 label_direction[4];
    s32 level_index;
    s32 availability;
    s32 screen_width_subpixels;
    s32 screen_height_subpixels;
    s32 marker_x;
    s32 marker_y;
    s32 label_x;
    s32 label_y;
    s32 line_start_x;
    s32 line_start_y;
    s32 direction_length;
    s32 line_fraction;
    s32 text_width;
    s32 line_end_x;
    s32 text_x;
    u8 *label_text;
    s32 texture_index;

    setup_gif_paging(0);
    vu1_add_g_sregister(0x42, 0x8000000044);
    vu1_add_g_sregister(0x47, 0x4B);
    append_screen_rect_packet(0, 0, 0x200, 0x1C0, 0x80000000, 0);
    texture_index = get_icon_frame(0xE99A, 0xE);
    screen_width_subpixels = (right - left) * 16;
    screen_height_subpixels = (bottom - top) * 16;
    draw_hud_sprite_uv(texture_index, 0, 0, screen_width_subpixels, screen_height_subpixels, 0, 0,
                       0x80);
    vu1_add_g_sregister(8, 0);
    draw_hud_sprite_uv(get_icon_frame(0xE99A, 0xF), 0, 0, screen_width_subpixels,
                       screen_height_subpixels, game_frame_counter & 0xFFF, 0, 0x80);
    vu1_add_g_sregister(8, 5);
    for (level_index = 1; level_index < 20; level_index++) {
        if (*(volatile s32 *)&level_map_markers[level_index].x == 0) {
            continue;
        }
        availability = 3;
        if (level_visit_state[level_index] == 0) {
            availability = 2;
            /* This separate state table gates markers that have not been visited. */
            if (level_available[level_index] == 0) {
                availability = 0;
            }
        }
        if (availability == 0) {
            continue;
        }
        marker_x = level_map_markers[level_index].x;
        marker_y = level_map_markers[level_index].y;
        if (pal_mode != 0) {
            marker_y = marker_y * 0x1C0 / 0x1A0;
        }
        if (availability == 3 ||
            (availability == 2 &&
             game_frame_counter % (scale_game_frames(0x16) + scale_game_frames(8)) <
                 scale_game_frames(0x16))) {
            draw_hud_sprite(get_icon_frame(0xE99A, 0xC), marker_x - 5,
                            marker_y - 5, 10, 10, 0x80);
        }
        if (level_index == level_map_selection.level) {
            label_x = marker_x + level_map_markers[level_index].label_offset_x;
            label_y = marker_y + level_map_markers[level_index].label_offset_y;
            label_direction[1] = level_map_markers[level_index].label_offset_y;
            label_direction[0] = level_map_markers[level_index].label_offset_x;
            direction_length = func_001F9B20(label_direction) * 1000.0f;
            line_fraction = direction_length - 8000;
            line_start_x = label_x + (marker_x - label_x) * line_fraction / direction_length;
            line_start_y = label_y + (marker_y - label_y) * line_fraction / direction_length;
            label_text = get_help_message_text(level_map_labels[level_index].label_text);
            text_width = measure_text_width_regular(label_text, -1);
            if (level_map_markers[level_index].label_offset_x <= -1) {
                line_end_x = label_x - text_width;
            } else {
                line_end_x = label_x + text_width;
            }
            draw_hud_rect(line_start_x + 1, line_start_y + 1, label_x + 1, label_y + 1, 0x80000000,
                          0);
            draw_hud_rect(label_x + 1, label_y + 1, line_end_x + 1, label_y + 1, 0x80000000, 0);
            text_x = (line_end_x < label_x) ? line_end_x : label_x;
            font_print_large(text_x + 1, label_y - large_font_height + 1, 0x80000000, label_text,
                             -1);
            draw_hud_rect(line_start_x, line_start_y, label_x, label_y, 0x80F0F0F0, 0);
            draw_hud_rect(label_x, label_y, line_end_x, label_y, 0x80F0F0F0, 0);
            font_print_large(text_x, label_y - large_font_height, 0x80F0F0F0, label_text, -1);
            draw_hud_sprite(get_icon_frame(0xE99A, 0xD), marker_x - 10,
                            marker_y - 10, 20, 20, 0x80);
        }
    }
    do_gif_paging();
}
#endif /* NON_MATCHING */

LevelMapMarker level_map_markers[20] = {{0}, {0x50, 0x5f, 0x14, -20}, {0x55, 0x82, -16, -8}, {0x5a, 0x41, -16, -16}, {0x6c, 0x6e, -16, 0x28}, {0xa0, 0x6e, 0xa, -16}, {0x78, 0x55, 0x10, 0x12}, {0x96, 0x96, -16, 0x10}, {0xb4, 0x82, 0x20, -12}, {0xc8, 0x6e, 0x10, 0x10}, {0xdc, 0x8c, 0x10, 0x10}, {0x118, 0x64, -16, 3}, {0xe6, 0x50, 0x10, -16}, {0xd2, 0x28, -16, -16}, {0xc8, 0x32, -16, -16}, {0xa0, 0x28, -16, -16}, {0x82, 0x32, -16, -16}, {0x2d, 0x5a, 0x20, -60}, {0x23, 0x50, 0x20, -50}, {0}};
