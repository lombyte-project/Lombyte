#include "types.h"

struct MenuStreamImageScreen {
    u8  pad_0[0x44];
    s32 state;
    s32 left_buffer;
    s32 right_buffer;
};

extern void setup_gif_paging() __asm__("func_001F4280");
extern void do_gif_paging() __asm__("func_001F4398");
extern void draw_textured_quad() __asm__("func_001F5450");
extern s64  load_texture_file() __asm__("func_00204CF0");
s32         draw_menu_stream_image(struct MenuStreamImageScreen *screen) __asm__("FUN_00220790");

s32 draw_menu_stream_image(struct MenuStreamImageScreen *screen) {
    s64 modulate_color;

    if (screen->state < 4) {
        return 0;
    }
    modulate_color = (s64)0x80808080;
    setup_gif_paging(0);
    draw_textured_quad(0,
                       0,
                       0x100,
                       0x100,
                       0,
                       0,
                       0x100,
                       0x100,
                       modulate_color,
                       load_texture_file(screen->left_buffer));
    draw_textured_quad(0x100,
                       0,
                       0x100,
                       0x100,
                       0,
                       0,
                       0x100,
                       0x100,
                       modulate_color,
                       load_texture_file(screen->right_buffer));
    do_gif_paging();
    return 8;
}
