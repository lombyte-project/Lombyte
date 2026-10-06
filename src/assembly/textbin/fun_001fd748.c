#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_001fd748/FUN_001fd748.s", FUN_001fd748);
#else
#include "types.h"

typedef struct {
    s32 x;
    s32 y;
    s32 label_offset_x;
    s32 label_offset_y;
} LevelMapMarker;

typedef struct {
    s32 label_text;
    s32 pad4[2];
} LevelMapMarkerText;

typedef struct {
    u8 pad0[0x224];
    s32 selected_level;
} LevelMapSelection;

extern u8 D_0013DD40[];
extern u8 g_abLevelVisitState[] __asm__("D_0013DD58");
extern s32 pal_mode __asm__("D_0015ED80");
extern s32 game_frame_counter __asm__("D_0015F438");
extern s32 large_font_height __asm__("D_0015F690") __attribute__((sda));
extern LevelMapSelection level_map_selection __asm__("D_001A00F0");
extern LevelMapMarkerText level_map_labels[] __asm__("D_001DDD44");
extern LevelMapMarker level_map_markers[] __asm__("D_001DDE28");
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
    u8 visit_state;
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
        visit_state = g_abLevelVisitState[level_index];
        if (visit_state == 0) {
            availability = 2;
            /* This separate state table gates markers that have not been visited. */
            if (D_0013DD40[level_index] == 0) {
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
        if (level_index == level_map_selection.selected_level) {
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
