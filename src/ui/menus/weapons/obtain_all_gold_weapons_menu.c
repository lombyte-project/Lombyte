#include "types.h"

#include "rnc/ui/menus/menu_screen.h"

struct MenuPacket {
    u8 pad_0[2];
    s16 unk2;
    u8 pad_4[2];
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 pad_C[2];
    u16 unkE;
    s16 unk10;
    u8 pad_12[6];
};

extern void func_001153FC();
extern void setup_gif_paging() __asm__("func_001F4280");
extern void do_gif_paging() __asm__("func_001F4398");
extern void font_print_window_regular(void *, u64, void *, s32) __asm__("func_001F7580");
extern void *get_help_message_text(s32) __asm__("func_001FDD10");
extern s32 get_icon_frame() __asm__("func_001FF960");
extern void draw_hud_sprite() __asm__("FUN_001ffc30");

s32 obtain_all_gold_weapons_menu(struct MenuScreen *menu) __asm__("FUN_00222948");

s32 obtain_all_gold_weapons_menu(struct MenuScreen *menu) {
    struct MenuPacket packet;
    struct MenuPacket tmp;
    void *tex;
    s16 width;
    s32 first_y = 4;
    s16 pos_x = 0x18;

    func_001153FC(&tmp, 0, 0x18);
    tmp.unk2 = menu->height;
    tmp.unk6 = menu->width;
    tmp.unk10 = 0x10;
    packet = tmp;
    setup_gif_paging(0);
    draw_hud_sprite(get_icon_frame(0xE99A, 6), 4, 0xC, 0x10, 0x10, 0x80);
    packet.unkA = first_y;
    packet.unk8 = pos_x;
    tex = get_help_message_text(0x5182);
    font_print_window_regular(&packet, ((u64)0x80FF << 16) | 0xA888, tex, -1);
    width = packet.unkE;
    first_y = width + 0x10;
    packet.unkA = first_y;
    tex = get_icon_frame(0xE99A, 6);
    draw_hud_sprite(tex, 4, width + 0x18, 0x10, 0x10, 0x80);
    font_print_window_regular(&packet, ((u64)0x80FF << 16) | 0xA888, get_help_message_text(0x5183),
                              -1);
    do_gif_paging();
    return 2;
}

extern __typeof__(obtain_all_gold_weapons_menu) func_00222948
    __attribute__((alias("FUN_00222948")));
