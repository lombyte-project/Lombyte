#include "types.h"

struct Menu {
    u8 pad0[0x20];
    s32 w;
    s32 h;
};

extern s32 D_0015ED84;
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern s32 get_help_message_text(s32) __asm__("func_001FDD10");
extern s32 measure_text_width_small(s32, s32) __asm__("func_001F6270");
extern void func_001F61F8(void);
extern void func_001F61E8(void);
extern void font_print_small(s32, s32, u64, s32, s32) __asm__("func_001F65B0");

s32 draw_missions_menu(struct Menu *m) __asm__("FUN_0021f368");

s32 draw_missions_menu(struct Menu *m) {
    s32 w;
    s32 t;
    s32 x;
    s32 y;
    s32 step;

    vu1_add_g_sregister(0x42, 0x44);
    vu1_add_g_sregister(0x47, 0xB);
    setup_gif_paging(0);
    w = measure_text_width_small(get_help_message_text(0x4EEE), -1);
    t = measure_text_width_small(get_help_message_text(0x4EFA), -1);
    if (t >= w) {
        w = t;
    }
    if (D_0015ED84 != 0) {
        t = measure_text_width_small(get_help_message_text(0x4EEF), -1);
        if (t >= w) {
            w = t;
        }
    }
    t = measure_text_width_small(get_help_message_text(0x4EFB), -1);
    if (t >= w) {
        w = t;
    }
    t = measure_text_width_small(get_help_message_text(0x4EFC), -1);
    if (t >= w) {
        w = t;
    }
    t = measure_text_width_small(get_help_message_text(0x4EE0), -1);
    if (t >= w) {
        w = t;
    }
    x = (m->w - w) >> 1;
    if (x < 2) {
        x = 2;
    }
    step = m->h / (D_0015ED84 != 0 ? 7 : 6);
    func_001F61F8();
    y = step - 6;
    font_print_small(x, y, 0x80FFA888, get_help_message_text(0x4EEE), -1);
    if (D_0015ED84 != 0) {
        y += step;
        font_print_small(x, y, 0x80FFA888, get_help_message_text(0x4EEF), -1);
    }
    y += step;
    font_print_small(x, y, 0x80FFA888, get_help_message_text(0x4EFA), -1);
    y += step;
    font_print_small(x, y, 0x80FFA888, get_help_message_text(0x4EFB), -1);
    y += step;
    font_print_small(x, y, 0x80FFA888, get_help_message_text(0x4EFC), -1);
    y += step;
    font_print_small(x, y, 0x80FFA888, get_help_message_text(0x4EE0), -1);
    func_001F61E8();
    do_gif_paging();
    return 2;
}

extern __typeof__(draw_missions_menu) func_0021F368 __attribute__((alias("FUN_0021f368")));
