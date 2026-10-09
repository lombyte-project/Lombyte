#include "types.h"

#include "rnc/ui/menus/menu_screen.h"

/* Text window passed to font_print_window_regular. */
struct MenuPacket {
    u8 pad_0[2];
    s16 height; /* 0x2: from menu->height */
    u8 pad_4[2];
    s16 width;  /* 0x6: from menu->width */
    s16 x;      /* 0x8: text x */
    s16 y;      /* 0xA: text y */
    u8 pad_C[2];
    u16 end_y;  /* 0xE: y below the printed text; the next line starts 0x10 lower */
    s16 unk10;
    u8 pad_12[6];
};

extern void func_001153FC();
extern void setup_gif_paging() __asm__("func_001F4280");
extern void do_gif_paging() __asm__("func_001F4398");
extern void font_print_window_regular(void *, u64, void *, s32) __asm__("func_001F7580");
extern void *get_help_message_text(s32) __asm__("func_001FDD10");
extern s32 get_icon_frame() __asm__("func_001FF960");
extern void draw_hud_sprite(s32 id, s32 x, s32 y, s32 w, s32 h, s32 alpha) __asm__("FUN_001ffc30");

s32 obtain_all_gold_weapons_menu(struct MenuScreen *menu) __asm__("FUN_00222948");

s32 obtain_all_gold_weapons_menu(struct MenuScreen *menu) {
    struct MenuPacket packet;
    struct MenuPacket tmp;
    void *tex;
    s32 icon;
    s16 end_y;
    s32 first_y = 4;
    s16 pos_x = 0x18;

    func_001153FC(&tmp, 0, 0x18);
    tmp.height = menu->height;
    tmp.width = menu->width;
    tmp.unk10 = 0x10;
    packet = tmp;
    setup_gif_paging(0);
    draw_hud_sprite(get_icon_frame(0xE99A, 6), 4, 0xC, 0x10, 0x10, 0x80);
    packet.y = first_y;
    packet.x = pos_x;
    tex = get_help_message_text(0x5182);
    font_print_window_regular(&packet, ((u64)0x80FF << 16) | 0xA888, tex, -1);
    end_y = packet.end_y;
    first_y = end_y + 0x10;
    packet.y = first_y;
    icon = get_icon_frame(0xE99A, 6);
    draw_hud_sprite(icon, 4, end_y + 0x18, 0x10, 0x10, 0x80);
    font_print_window_regular(&packet, ((u64)0x80FF << 16) | 0xA888, get_help_message_text(0x5183),
                              -1);
    do_gif_paging();
    return 2;
}

extern __typeof__(obtain_all_gold_weapons_menu) func_00222948
    __attribute__((alias("FUN_00222948")));
