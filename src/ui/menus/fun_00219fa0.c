#include "types.h"
#include "rnc/ui/menus/menu_screen.h"
extern s32 D_0015F438;
extern s32 D_001601B0 __attribute__((sda));
extern s32 SubtractIntegerWithClamp();
extern void setup_gif_paging() __asm__("func_001F4280");
extern void do_gif_paging() __asm__("func_001F4398");
extern s32 get_icon_frame() __asm__("func_001FF960");
extern void draw_hud_sprite() __asm__("func_001FFC30");
extern void draw_hud_sprite_flipped() __asm__("FUN_001ffe18");
extern void draw_hud_sprite_subpixel() __asm__("func_00200080");
extern void append_screen_rect_packet() __asm__("func_00200E08");

s32 FUN_00219fa0(struct MenuScreen *menu) {
    s32 new_var2;
    s32 start_y;
    int new_var;
    s32 x;
    s32 height;
    s32 y;
    s32 i;
    int new_var3;
    s32 byte_offset;
    struct MenuIcon *icon;
    i = 0;
    y = menu->data.icons.first_y;
    start_y = y;
    x = ((s32)((menu->width * 0x10) - 0x200)) >> 1;
    setup_gif_paging(0);
    new_var3 = 0;
    new_var = new_var3;
    if (menu->data.icons.count > new_var3) {
        byte_offset = new_var3;
        do {
            icon = (struct MenuIcon *)((u8 *)menu->data.icons.list + byte_offset);
            if (menu->data.icons.cursor == i) {
                append_screen_rect_packet(
                    x - 0x30, y - 0x30, 0x230 + x, y + 0x230,
                    (u64)(u32)(((SubtractIntegerWithClamp(((*(s32 *)0x15F438) & 0x3F) - 0x20) +
                                 0x40) *
                                0x10202) |
                               0x80000000),
                    1);
                append_screen_rect_packet(x - 0x10, y - 0x10, x + 0x210,
                                          y + 0x210, (u64)D_001601B0, 1);
            }
            i = i + 1;
            byte_offset += 0xA;
            draw_hud_sprite_subpixel(
                get_icon_frame(icon->icon, icon->frame),
                new_var2 = x, y, 0x200, 0x200, 0x80);
            y += 0x260;
        } while (i < menu->data.icons.count);
    }
    if (start_y < new_var) {
        append_screen_rect_packet(new_var, new_var, menu->width, 0x14, (u64)D_001601B0, new_var);
        draw_hud_sprite(get_icon_frame(0xE99EU, 6), (x >> 4), 2, 0x20,
                        0x10, 0x80);
    }
    height = menu->height;
    if ((height * 0x10) < y) {
        byte_offset = new_var;
        append_screen_rect_packet(byte_offset, height - 0x14, menu->width, height,
                                  (u64)D_001601B0, byte_offset);
        draw_hud_sprite_flipped(get_icon_frame(0xE99EU, 6), (x >> 4),
                                menu->height - 0x12, 0x20, 0x10, 0x80);
    }
    do_gif_paging();
    return 2;
}
