#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/audio/draw_sound_menu/FUN_0021ce00.s", FUN_0021ce00);
#else
#include "types.h"

typedef struct {
    u8 pad0[0x20];
    s32 width;
    s32 height;
    u8 pad28[0x18];
    s32 selected_option;
} SoundMenu;

extern s32 playback_mode __asm__("D_0015EDE8");
extern s32 music_volume __asm__("D_0015EDEC");
extern s32 sound_volume __asm__("D_0015EDF0");

extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern void *get_help_message_text(s32) __asm__("func_001FDD10");
extern void font_print_right(s32, s32, u64, void *, s32) __asm__("FUN_001f6940");
extern void font_print_large(s32, s32, u64, void *, s32) __asm__("FUN_001f6530");
extern s32 find_valid_animation_frame_index(s32, s32) __asm__("func_001FF960");
extern void draw_hud_sprite_rect(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_00200958");
extern void append_screen_sprite(s32, s32, s32, s32, u64, s32) __asm__("func_00200E08");

s32 draw_sound_menu(SoundMenu *menu) __asm__("FUN_0021ce00");

s32 draw_sound_menu(SoundMenu *menu)
{
    s32 center_x;
    s32 row_height;
    s32 label_right;
    s32 border_left;
    s32 slider_left;
    s32 slider_padding;
    s32 slider_length;

    row_height = menu->height >> 2;
    center_x = menu->width >> 1;
    setup_gif_paging(0);
    label_right = center_x - 8;
    border_left = center_x + 7;
    slider_left = center_x + 9;
    slider_padding = center_x + 0x4A;

    font_print_right(label_right, row_height - 8, menu->selected_option == 0 ? 0x8020FFFF : 0x80FFA888, get_help_message_text(0x5212), -1);
    append_screen_sprite(border_left, row_height - 8, menu->width - 0x3F, row_height + 8, 0x80696969, 0);
    append_screen_sprite(slider_left, row_height - 6, menu->width - 0x41, row_height + 6, 0x80383838, 0);
    slider_length = (menu->width - slider_padding) * sound_volume / 1024;
    draw_hud_sprite_rect(find_valid_animation_frame_index(0xE99E, 8), slider_left << 4, (row_height - 6) << 4, (center_x + 8 + slider_length) << 4, (row_height + 5) << 4, 0, 0xA0, 0x1F0, 0x150, 0x80);

    font_print_right(label_right, row_height * 2 - 8, menu->selected_option == 1 ? 0x8020FFFF : 0x80FFA888, get_help_message_text(0x5213), -1);
    append_screen_sprite(border_left, row_height * 2 - 8, menu->width - 0x3F, row_height * 2 + 8, 0x80696969, 0);
    append_screen_sprite(slider_left, row_height * 2 - 6, menu->width - 0x41, row_height * 2 + 6, 0x80383838, 0);
    slider_length = (menu->width - slider_padding) * music_volume / 1024;
    draw_hud_sprite_rect(find_valid_animation_frame_index(0xE99E, 9), slider_left << 4, (row_height * 2 - 6) << 4, (center_x + 8 + slider_length) << 4, (row_height * 2 + 5) << 4, 0, 0xA0, 0x1F0, 0x150, 0x80);

    font_print_right(label_right, row_height * 3 - 8, menu->selected_option == 2 ? 0x8020FFFF : 0x80FFA888, get_help_message_text(0x5214), -1);
    font_print_large(center_x + 8, row_height * 3 - 8, 0x80FFA888, get_help_message_text(playback_mode != 0 ? 0x5216 : 0x5215), -1);
    do_gif_paging();
    return 2;
}
#endif /* NON_MATCHING */
