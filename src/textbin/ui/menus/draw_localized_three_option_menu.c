#include "types.h"

struct ThreeOptionMenu {
    u8 pad0[0x20];
    s32 menu_width;
    s32 height;
    s32 spacing_divisor;
};

extern u8 D_001DF050[];
extern u8 D_001DF3F0[];
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern s32 count_nonzero_entries_up_to_40(void) __asm__("func_00215290");
extern s32 count_nonzero_entries_up_to_10(void) __asm__("func_00215300");
extern s32 count_nonzero_entries_up_to_30(void) __asm__("func_00215348");
extern s32 sprintf(char *, const char *, ...);
extern s32 measure_text_width_regular(char *, s32) __asm__("func_001F6250");
extern void func_001F61F8(void);
extern void func_001F61E8(void);
extern void font_print_center(s32, s32, u64, char *, s32) __asm__("func_001F6AF0");
extern void draw_menu_selection_marker(s32, s32, s32) __asm__("func_0021F8E8");
extern u64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void font_print(s32, s32, u64, char *, s32, s64, u8 *) __asm__("func_001F62B0");

s32 draw_localized_three_option_menu(struct ThreeOptionMenu *menu) __asm__("FUN_00222a98");

s32 draw_localized_three_option_menu(struct ThreeOptionMenu *menu) {
    char text_buffer[0x50];
    s32 font_texture_index;
    u8 *glyphs;
    s32 maximum_text_width;
    s32 measured_width;
    s32 line_spacing;
    s32 draw_y;

    font_texture_index = 1;
    setup_gif_paging(0);
    line_spacing = menu->height / 5;
    draw_y = menu->height / 5 - 8;
    maximum_text_width = 0;
    sprintf(text_buffer, get_help_message_text(0x522F), count_nonzero_entries_up_to_40(), 0x28);
    measured_width = measure_text_width_regular(text_buffer, -1);
    if (maximum_text_width < measured_width) {
        maximum_text_width = measured_width;
    }
    sprintf(text_buffer, get_help_message_text(0x5230), count_nonzero_entries_up_to_10(), 10);
    measured_width = measure_text_width_regular(text_buffer, -1);
    if (maximum_text_width < measured_width) {
        maximum_text_width = measured_width;
    }
    sprintf(text_buffer, get_help_message_text(0x5231), count_nonzero_entries_up_to_30(), 0x1E);
    measured_width = measure_text_width_regular(text_buffer, -1);
    if (maximum_text_width < measured_width) {
        maximum_text_width = measured_width;
    }
    glyphs = D_001DF050;
    if (menu->menu_width < maximum_text_width + 0x18) {
        font_texture_index = 2;
        glyphs = D_001DF3F0;
    }
    func_001F61F8();
    font_print_center(menu->menu_width >> 1, draw_y, 0x80FFA888, get_help_message_text(0x522E), -1);
    draw_y += line_spacing;
    draw_menu_selection_marker(0xB, draw_y + 9, count_nonzero_entries_up_to_40() == 0x28);
    sprintf(text_buffer, get_help_message_text(0x522F), count_nonzero_entries_up_to_40(), 0x28);
    font_print(0x14, draw_y, 0x80FFA888, text_buffer, -1, get_effect_texture(font_texture_index),
               glyphs);
    draw_y += line_spacing;
    draw_menu_selection_marker(0xB, draw_y + 9, count_nonzero_entries_up_to_10() == 10);
    sprintf(text_buffer, get_help_message_text(0x5230), count_nonzero_entries_up_to_10(), 10);
    font_print(0x14, draw_y, 0x80FFA888, text_buffer, -1, get_effect_texture(font_texture_index),
               glyphs);
    draw_y += line_spacing;
    draw_menu_selection_marker(0xB, draw_y + 9, count_nonzero_entries_up_to_30() == 0x1E);
    sprintf(text_buffer, get_help_message_text(0x5231), count_nonzero_entries_up_to_30(), 0x1E);
    font_print(0x14, draw_y, 0x80FFA888, text_buffer, -1, get_effect_texture(font_texture_index),
               glyphs);
    func_001F61E8();
    do_gif_paging();
    return 2;
}
extern __typeof__(draw_localized_three_option_menu) func_00222A98
    __attribute__((alias("FUN_00222a98")));
