#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/draw_localized_three_option_menu/FUN_00222a98.s", FUN_00222a98);
#else
#include "types.h"

struct Menu {
    u8 pad0[0x20];
    s32 w;
    s32 h;
};

extern u8 D_001DF050[];
extern u8 D_001DF3F0[];
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern s32 func_001FDD10(s32);
extern s32 func_00215290(void);
extern s32 func_00215300(void);
extern s32 func_00215348(void);
extern void sprintf(char *, s32, s32, s32);
extern s32 func_001F6250(char *, s32);
extern void func_001F61F8(void);
extern void func_001F61E8(void);
extern void func_001F6AF0(s32, s32, u64, s32, s32);
extern void func_0021F8E8(s32, s32, s32);
extern s32 func_001F44B8(s32);
extern void func_001F62B0(s32, s32, u64, char *, s32, s32, u8 *);

s32 draw_localized_three_option_menu(struct Menu *m) __asm__("FUN_00222a98");

s32 draw_localized_three_option_menu(struct Menu *m) {
    char buf[0x50];
    s32 font;
    u8 *glyphs;
    s32 w;
    s32 t;
    s32 step;
    s32 y;

    font = 1;
    glyphs = 0;
    w = 0;
    func_001F4280(0);
    step = m->h / 5;
    sprintf(buf, func_001FDD10(0x522F), func_00215290(), 0x28);
    t = func_001F6250(buf, -1);
    if (w < t) {
        w = t;
    }
    sprintf(buf, func_001FDD10(0x5230), func_00215300(), 10);
    t = func_001F6250(buf, -1);
    if (w < t) {
        w = t;
    }
    sprintf(buf, func_001FDD10(0x5231), func_00215348(), 0x1E);
    t = func_001F6250(buf, -1);
    if (w < t) {
        w = t;
    }
    glyphs = D_001DF050;
    y = step - 8;
    if (m->w < w + 0x18) {
        font = 2;
        glyphs = D_001DF3F0;
    }
    func_001F61F8();
    func_001F6AF0(m->w >> 1, y, 0x80FFA888, func_001FDD10(0x522E), -1);
    y += step;
    func_0021F8E8(0xB, y + 9, func_00215290() == 0x28);
    sprintf(buf, func_001FDD10(0x522F), func_00215290(), 0x28);
    func_001F62B0(0x14, y, 0x80FFA888, buf, -1, func_001F44B8(font), glyphs);
    y += step;
    func_0021F8E8(0xB, y + 9, func_00215300() == 10);
    sprintf(buf, func_001FDD10(0x5230), func_00215300(), 10);
    func_001F62B0(0x14, y, 0x80FFA888, buf, -1, func_001F44B8(font), glyphs);
    y += step;
    func_0021F8E8(0xB, y + 9, func_00215348() == 0x1E);
    sprintf(buf, func_001FDD10(0x5231), func_00215348(), 0x1E);
    func_001F62B0(0x14, y, 0x80FFA888, buf, -1, func_001F44B8(font), glyphs);
    func_001F61E8();
    func_001F4398();
    return 2;
}
#endif /* NON_MATCHING */
