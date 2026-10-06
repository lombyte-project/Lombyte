#include "types.h"
struct MenuScreen {
    u8 pad_0[0x20];
    s32 unk20;
    s32 unk24;
    u8 pad_28[0x14];
    s32 unk3C;
    s32 unk40;
    u8 pad_44[0x4];
    struct MenuIcon *unk48;
    u8 pad_4C[0x10];
    s32 unk5C;
};
struct MenuIcon {
    u16 unk0;
    s16 unk2;
};
extern s32 D_0015F438;
extern s32 D_001601B0;
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
    y = (*menu).unk5C;
    start_y = y;
    x = ((s32)((menu->unk20 * 0x10) - 0x200)) >> 1;
    setup_gif_paging(0);
    new_var3 = 0;
    new_var = new_var3;
    if (menu->unk40 > new_var3) {
        byte_offset = new_var3;
        do {
            icon = (struct MenuIcon *)((u8 *)menu->unk48 + byte_offset);
            if (menu->unk3C == i) {
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
                get_icon_frame(icon->unk0, icon->unk2),
                new_var2 = x, y, 0x200, 0x200, 0x80);
            y += 0x260;
        } while (i < menu->unk40);
    }
    if (start_y < new_var) {
        append_screen_rect_packet(new_var, new_var, menu->unk20, 0x14, (u64)D_001601B0, new_var);
        draw_hud_sprite(get_icon_frame(0xE99EU, 6), (x >> 4), 2, 0x20,
                        0x10, 0x80);
    }
    height = menu->unk24;
    if ((height * 0x10) < y) {
        byte_offset = new_var;
        append_screen_rect_packet(byte_offset, height - 0x14, menu->unk20, height,
                                  (u64)D_001601B0, byte_offset);
        draw_hud_sprite_flipped(get_icon_frame(0xE99EU, 6), (x >> 4),
                                menu->unk24 - 0x12, 0x20, 0x10, 0x80);
    }
    do_gif_paging();
    return 2;
}
