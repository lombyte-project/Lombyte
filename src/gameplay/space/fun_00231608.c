#include "types.h"
struct Res { s32 w; s32 h; };
struct DisplayModes { u8 pad0[0x1938]; struct Res ntsc[12]; struct Res pal[12]; };
struct Flags { u8 pad0[0x6B]; u8 flags; };
extern s32 D_0015ED80;
extern s32 D_0015ED88;
extern s32 D_001940D4[];
extern struct DisplayModes D_00137B80;
extern struct Flags D_0013E550;
extern void FUN_0012f1c8();
extern void func_0023A3B8(s32, s32, s32, s32, s32);
extern s32 sceGsSyncV(s32);
extern void FUN_00120558(s32, s32);
extern void *sceGsSyncVCallback(void *);
extern void func_001F4A58(s32);
void FUN_00231608(s32 mode) {
    s32 h;
    s32 w;
    s32 base;

    if (D_0015ED80 != 0) {
        w = D_00137B80.pal[mode].w;
        h = D_00137B80.pal[mode].h;
    } else {
        w = D_00137B80.ntsc[mode].w;
        h = D_00137B80.ntsc[mode].h;
    }
    base = D_001940D4[0];
    D_0013E550.flags |= 8;
    func_0023A3B8(w, h, (base + 0x3F) & ~0x3F, (base + 0x30003F) & ~0x3F, D_0015ED88);
    sceGsSyncV(0);
    FUN_00120558(0, 0);
    sceGsSyncVCallback(FUN_0012f1c8);
    func_001F4A58(4);
    D_0013E550.flags |= 0x10;
}

extern __typeof__(FUN_00231608) func_00231608 __attribute__((alias("FUN_00231608")));
