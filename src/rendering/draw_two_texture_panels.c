#include "types.h"
struct TexturePanelScreen {
    u8 pad_0[0x44];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
};

extern void setup_gif_paging() __asm__("func_001F4280");
extern void do_gif_paging() __asm__("func_001F4398");
extern void draw_textured_quad() __asm__("func_001F5450");
extern s64 func_00204CF0();
s32 draw_two_texture_panels(struct TexturePanelScreen *screen) __asm__("FUN_00220790");

s32 draw_two_texture_panels(struct TexturePanelScreen *screen) {
    s64 modulate_color;

    if (screen->unk44 < 4) {
        return 0;
    }
    modulate_color = (s64)0x80808080;
    setup_gif_paging(0);
    draw_textured_quad(0, 0, 0x100, 0x100, 0, 0, 0x100, 0x100, modulate_color,
                       func_00204CF0(screen->unk48));
    draw_textured_quad(0x100, 0, 0x100, 0x100, 0, 0, 0x100, 0x100, modulate_color,
                       func_00204CF0(screen->unk4C));
    do_gif_paging();
    return 8;
}
