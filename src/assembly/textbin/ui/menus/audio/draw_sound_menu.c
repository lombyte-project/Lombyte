#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/audio/draw_sound_menu/FUN_0021ce00.s", FUN_0021ce00);
#else
#include "types.h"

typedef struct {
    u8 pad0[0x20];
    s32 w;
    s32 h;
    u8 pad28[0x18];
    s32 sel;
} Menu;

extern s32 D_0015EDE8;
extern s32 D_0015EDEC;
extern s32 D_0015EDF0;

extern void func_001F4280(s32);
extern void func_001F4398(void);
extern void *func_001FDD10(s32);
extern void font_print_right(s32, s32, u64, void *, s32) __asm__("FUN_001f6940");
extern void font_print_large(s32, s32, u64, void *, s32) __asm__("FUN_001f6530");
extern s32 func_001FF960(s32, s32);
extern void draw_hud_sprite_rect(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) __asm__("FUN_00200958");
extern void func_00200E08(s32, s32, s32, s32, u64, s32);

s32 draw_sound_menu(Menu *m) __asm__("FUN_0021ce00");

s32 draw_sound_menu(Menu *m)
{
    s32 x;
    s32 y;
    s32 lx;
    s32 bx;
    s32 fx;
    s32 vx;
    s32 len;

    y = m->h >> 2;
    x = m->w >> 1;
    func_001F4280(0);
    lx = x - 8;
    bx = x + 7;
    fx = x + 9;
    vx = x + 0x4A;

    font_print_right(lx, y - 8, m->sel == 0 ? 0x8020FFFF : 0x80FFA888, func_001FDD10(0x5212), -1);
    func_00200E08(bx, y - 8, m->w - 0x3F, y + 8, 0x80696969, 0);
    func_00200E08(fx, y - 6, m->w - 0x41, y + 6, 0x80383838, 0);
    len = (m->w - vx) * D_0015EDF0 / 1024;
    draw_hud_sprite_rect(func_001FF960(0xE99E, 8), fx << 4, (y - 6) << 4, (x + 8 + len) << 4, (y + 5) << 4, 0, 0xA0, 0x1F0, 0x150, 0x80);

    font_print_right(lx, y * 2 - 8, m->sel == 1 ? 0x8020FFFF : 0x80FFA888, func_001FDD10(0x5213), -1);
    func_00200E08(bx, y * 2 - 8, m->w - 0x3F, y * 2 + 8, 0x80696969, 0);
    func_00200E08(fx, y * 2 - 6, m->w - 0x41, y * 2 + 6, 0x80383838, 0);
    len = (m->w - vx) * D_0015EDEC / 1024;
    draw_hud_sprite_rect(func_001FF960(0xE99E, 9), fx << 4, (y * 2 - 6) << 4, (x + 8 + len) << 4, (y * 2 + 5) << 4, 0, 0xA0, 0x1F0, 0x150, 0x80);

    font_print_right(lx, y * 3 - 8, m->sel == 2 ? 0x8020FFFF : 0x80FFA888, func_001FDD10(0x5214), -1);
    font_print_large(x + 8, y * 3 - 8, 0x80FFA888, func_001FDD10(D_0015EDE8 != 0 ? 0x5216 : 0x5215), -1);
    func_001F4398();
    return 2;
}
#endif /* NON_MATCHING */
