#include "types.h"
struct ScrollingStatus {
    u8 pad_0[0x2C];
    s32 unk2C;
    u8 pad_30[0x14];
    s32 unk44;
};

extern u8 D_001E5FB8[];
extern struct ScrollingStatus D_001E63C0;
extern s32 draw_framebuffer_rect() __asm__("func_001FB8F0");
extern s32 get_help_message_text() __asm__("func_001FDD10");
extern s32 random_integer_below() __asm__("func_00213260");
extern s32 set_scrolling_status_message() __asm__("func_00237E90");
extern s32 render_capture_scrolling_text(s32, s32, s32, f32) __asm__("func_00238310");
extern s32 strlen();
void update_scrolling_status_message(void) __asm__("FUN_00238520");

void update_scrolling_status_message(void) {
    draw_framebuffer_rect(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    if ((strlen(D_001E63C0.unk2C) - ((s32)D_001E63C0.unk44 / 20)) > 0) {
        D_001E63C0.unk44 = (s32)(D_001E63C0.unk44 + 2);
    } else {
        u8 *base_1e5fb8 = D_001E5FB8;
        set_scrolling_status_message(get_help_message_text(
            *(s32 *)(void *)((random_integer_below(0x18) * 4) + base_1e5fb8)));
    }
    render_capture_scrolling_text(D_001E63C0.unk2C, -D_001E63C0.unk44, 8, 2.0f);
    draw_framebuffer_rect(0, 0, 4, 0x40, 0x200, 0x80, 0);
    draw_framebuffer_rect(0xE2, 0, 0xE6, 0x40, 0x200, 0x80, 0);
}
