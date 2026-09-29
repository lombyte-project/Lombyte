#include "types.h"

struct GameState {
    u8 pad_0[0x224];
    s32 unk224;
};

struct MissionsMenu {
    u8 pad_0[0x20];
    s32 unk20;
    s32 unk24;
    u8 pad28[0x8];
    s32 sel[1];
};

struct Rect {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
    s16 unk08;
    s16 unk0A;
    s16 unk0C;
    s16 unk0E;
    u16 unk10;
    u16 unk12;
    s16 unk14;
    s16 unk16;
};

extern struct GameState D_001A00F0;
extern void func_001F4280(s32);
extern void func_001F4398();
extern void func_001F61E8();
extern void func_001F61F8();
extern void func_001F6530(s32, s32, u64, s32, s32);
extern s32 func_001F6FD0(s32, s32, s32, s32, s32, s32, s32);
extern void func_001F75F0(void *, u64, void *, s32);
extern u32 func_001FDD10(s32);
extern s32 func_0020BC00();
extern void func_0021F8E8(s32, s32, s32);
extern void func_00233980(s32, s32);

s32 draw_missions_menu2(struct MissionsMenu *arg0) __asm__("FUN_0021f688");

s32 draw_missions_menu2(struct MissionsMenu *arg0) {
    struct Rect dst;
    struct Rect src;
    s32 mask;
    s32 count;
    s32 i;
    s32 off;
    s32 flag;
    s32 col;
    s32 step;
    s32 h;
    s32 bit;
    s32 y;
    u32 *p;

    off = 0x18;
    i = 0;
    mask = 0;
    func_001F4280(0);
    count = func_0020BC00(0x70000000, &mask, 0, 0);
    func_00233980(0x42, 0x44);
    func_00233980(0x47, 0x10B);
    func_001F6530(4, 4, 0x80FFA888, func_001FDD10(0x4F59), -1);
    if (i < count) {
        p = (u32 *)0x70000000;
        do {
            y = off + 0xA;
            flag = arg0->sel[D_001A00F0.unk224] == i;
            col = flag ? 0x8020FFFF : 0x80FFA888;
            if (flag) {
                func_001F61F8();
            }
            h = func_001F6FD0(0x10, off, arg0->unk20 - 0x11, 0x3E8, col, func_001FDD10(*p), -1);
            if (flag) {
                func_001F61E8();
            }
            step = (h >= 0x11) ? 2 : 1;
            if (step == 2) {
                y = off + 0x10;
            }
            bit = (mask >> i) & 1;
            i++;
            func_0021F8E8(9, y, bit);
            p++;
            off += step * 16;
        } while (i < count);
    }
    if (mask < 0) {
        memset(&src, 0, 0x18);
        src.unk02 = arg0->unk24;
        src.unk06 = arg0->unk20;
        src.unk08 = arg0->unk24 >> 1;
        src.unk0A = off + 8;
        src.unk10 = 0x10;
        src.unk12 = 1;
        dst = src;
        func_001F75F0(&dst, 0x8020FFFF, func_001FDD10(0x523D), -1);
    }
    func_001F4398();
    return 2;
}

extern __typeof__(draw_missions_menu2) func_0021F688 __attribute__((alias("FUN_0021f688")));
