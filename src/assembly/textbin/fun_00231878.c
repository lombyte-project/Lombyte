#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00231878/FUN_00231878.s", FUN_00231878);
#else
#include "types.h"
#include "eetypes.h"

typedef struct { u128 data[6]; } sceGsLoadImage;

struct M2c_D_001940C0 {
    u8 pad_0[0x14];
    s32 unk14;
};

struct M2c_temp_3_12 {
    u8 pad_0[0x1388];
    s32 unk1388;
    s32 unk138C;
};

extern u8 D_00137B80[];
extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern s32 D_0015EE8C;
extern struct M2c_D_001940C0 D_001940C0;
extern void FlushCache(s32 a0);
extern s32 sceCdSync(s32 a0);
extern s32 sceGsExecLoadImage(sceGsLoadImage *img, s32 addr);
extern s32 sceGsSetDefLoadImage(sceGsLoadImage *img, s32 x, s32 y, s32 w, s32 h, s32 a4, s32 a6, s32 a7);
extern s32 func_00120558(s32 a0, s32 a1);
extern s32 func_0020B618(s32 a0, s32 a1);
extern s32 func_00216728(s32 a0, s32 a1, s32 a2, s32 a3);

void FUN_00231878(s32 arg0, s32 arg1, s32 arg2, s64 *arg3, s32 *arg4, s32 *arg5) {
    s32 v[6];
    sceGsLoadImage img;
    s32 *p4;
    s32 *p5;
    s32 i;
    s32 *q;
    s32 size;
    s32 addr;
    s32 t;
    s32 *e1;
    s32 *e2;
    u8 *p8;
    struct M2c_D_001940C0 *st;

    p4 = (s32 *)arg4;
    p5 = (s32 *)arg5;
    st = &D_001940C0;
    p8 = D_00137B80 + arg0 * 8;
    i = 0;
    func_00216728(st->unk14 + 0x100000, ((struct M2c_temp_3_12 *)p8)->unk1388,
        ((struct M2c_temp_3_12 *)p8)->unk138C, st->unk14);
    sceCdSync(0);
    FlushCache(0);
    func_0020B618(st->unk14 + 0x100000, st->unk14);
    FlushCache(0);
    t = st->unk14;
    e1 = (s32 *)((u8 *)t + arg1 * 4);
    e2 = (s32 *)((u8 *)t + arg2 * 4);
    D_0015EE78 = D_0015EE8C;
    D_0015EE74 = D_0015EE8C;
    q = v;
    for (i = 0; i < 6; i++) {
        if (i == 0) {
            sceGsSetDefLoadImage(&img, (D_0015EE74 << 8) >> 16, 1, 0, 0, 0, 0x10, 0x10);
            size = 0x400;
            addr = (s32)((u8 *)t + *(s32 *)((u8 *)t + 4)) + 0x20;
        } else if (i == 1) {
            sceGsSetDefLoadImage(&img, (D_0015EE74 << 8) >> 16, 1, 0x13, 0, 0, 0x40, 0x40);
            size = 0x1000;
            addr = (s32)((u8 *)t + *(s32 *)((u8 *)t + 4)) + 0x420;
        } else if (i == 2) {
            sceGsSetDefLoadImage(&img, (D_0015EE74 << 8) >> 16, 1, 0, 0, 0, 0x10, 0x10);
            size = 0x400;
            addr = (s32)((u8 *)t + e1[2]) + 0x20;
        } else if (i == 3) {
            sceGsSetDefLoadImage(&img, (D_0015EE74 << 8) >> 16, 8, 0x13, 0, 0, 0x200, 0x40);
            size = 0x8000;
            addr = (s32)((u8 *)t + e1[2]) + 0x420;
        } else if (i == 4) {
            sceGsSetDefLoadImage(&img, (D_0015EE74 << 8) >> 16, 1, 0, 0, 0, 0x10, 0x10);
            size = 0x400;
            addr = (s32)((u8 *)t + e2[2]) + 0x20;
        } else {
            sceGsSetDefLoadImage(&img, (D_0015EE74 << 8) >> 16, 8, 0x13, 0, 0, 0x200, 0x40);
            size = 0x8000;
            addr = (s32)((u8 *)t + e2[2]) + 0x420;
        }
        FlushCache(0);
        sceGsExecLoadImage(&img, addr);
        func_00120558(0, 0);
        *q = D_0015EE74 >> 8;
        D_0015EE74 = D_0015EE74 + size;
        q++;
    }
    *arg3 = (v[1] | 0x19304000) | (((s64)v[0] << 37) | ((s64)0xB000 << 19)) | (((s64)-1) << 63);
    *(s64 *)p4 = (v[3] | 0x25320000) | (((s64)v[2] << 37) | ((s64)0xB000 << 19)) | (((s64)-1) << 63);
    *(s64 *)p5 = (v[5] | 0x25320000) | (((s64)v[4] << 37) | ((s64)0xB000 << 19)) | (((s64)-1) << 63);
}
#endif /* NON_MATCHING */
