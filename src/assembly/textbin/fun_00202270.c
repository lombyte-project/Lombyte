#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00202270/FUN_00202270.s", FUN_00202270);
#else
#include "types.h"
#include "eetypes.h"
#include "sda.h"

typedef struct { u128 data[6]; } sceGsLoadImage;

typedef struct {
    u8 pad0[8];
    s32 w;
    s32 h;
    s32 psm;
    s32 cpsm;
    s32 pad18;
    s32 levels;
    u8 data[4];
} TexHeader;

typedef struct {
    s32 clut;
    s32 addr[4];
    s32 clutsize;
    s32 size[4];
    s32 cbp;
    s32 tbp[4];
    s32 tbw[4];
    s32 tw;
    s32 th;
} TexUpload;

extern s32 D_0015EE74 MACRO_ADDR;
extern void FillTransferWords(void *dst, s32 value, s32 size);
extern void FlushCache(s32);
extern s32 sceGsSetDefLoadImage(sceGsLoadImage *, s16, s16, s16, s16, s16, s16, s16);
extern s32 sceGsExecLoadImage(sceGsLoadImage *, u128 *);
extern s32 func_00120558(s32, s32);
extern s32 func_001F97A0(s32);

s32 upload_mip_texture(TexHeader *tex, u64 *regs) __asm__("FUN_00202270");

s32 upload_mip_texture(TexHeader *tex, u64 *regs) {
    TexUpload t;
    sceGsLoadImage li;
    s32 i;
    s32 size;
    s32 *bw;
    u64 v;
    u64 u;
    u64 w;
    u64 x;

    FillTransferWords(&t, 0, sizeof(t));
    bw = t.tbw;
    switch (tex->psm) {
    default:
        break;
    case 0:
    case 2:
        t.clut = 0;
        t.clutsize = 0;
        break;
    case 0x13:
    case 0x14:
        t.clut = (s32)tex->data;
        if (tex->psm == 0x14) {
            if (tex->cpsm == 0) {
                t.clutsize = 0x40;
            } else {
                t.clutsize = 0x20;
            }
        } else {
            if (tex->cpsm != 0) {
                t.clutsize = 0x200;
            } else {
                t.clutsize = 0x400;
            }
        }
        break;
    }
    t.tw = func_001F97A0(tex->w);
    t.th = func_001F97A0(tex->h);
    t.addr[0] = (s32)tex->data + t.clutsize;
    switch (tex->psm) {
    case 0:
        t.size[0] = tex->w * tex->h * 4;
        break;
    case 2:
        t.size[0] = tex->w * tex->h * 2;
        break;
    case 0x13:
        t.size[0] = tex->w * tex->h;
        break;
    case 0x14:
        t.size[0] = (tex->w * tex->h) >> 1;
        break;
    }
    if (tex->psm == 0x13 || tex->psm == 0x14) {
        t.cbp = D_0015EE74 >> 8;
        if (tex->psm == 0x14) {
            D_0015EE74 += 0x100;
            sceGsSetDefLoadImage(&li, t.cbp, 1, tex->cpsm, 0, 0, 8, 2);
        } else {
            D_0015EE74 += t.clutsize;
            sceGsSetDefLoadImage(&li, t.cbp, 1, tex->cpsm, 0, 0, 16, 16);
        }
        FlushCache(0);
        sceGsExecLoadImage(&li, (u128 *)t.clut);
        func_00120558(0, 0);
    }
    for (i = 1; i < tex->levels; i++) {
        t.size[i] = t.size[i - 1] >> 2;
        t.addr[i] = t.addr[i - 1] + t.size[i - 1];
    }
    for (i = 0; i < tex->levels; i++) {
        *bw = tex->w >> (i + 6);
        if (*bw <= 0) {
            *bw = 1;
        }
        t.tbp[i] = D_0015EE74 >> 8;
        sceGsSetDefLoadImage(&li, t.tbp[i], *bw, tex->psm, 0, 0, tex->w >> i, tex->h >> i);
        bw++;
        FlushCache(0);
        sceGsExecLoadImage(&li, (u128 *)t.addr[i]);
        func_00120558(0, 0);
        size = t.size[0] >> (i * 2);
        if (size <= 0xFF) {
            size = 0x100;
        }
        D_0015EE74 += size;
    }
    v = t.tbp[0]; v |= (u64)t.tbw[0] << 14; v |= (u64)tex->psm << 20; v |= (u64)t.tw << 26; v |= (u64)t.th << 30; u = (u64)t.cbp << 37; u |= (u64)1 << 34; v |= u; v |= (u64)tex->cpsm << 51; v |= (u64)1 << 63; regs[0] = v;
    w = t.tbp[1]; w |= (u64)t.tbw[1] << 14; w |= (u64)t.tbp[2] << 20; w |= (u64)t.tbw[2] << 34; w |= (u64)t.tbp[3] << 40; w |= (u64)t.tbw[3] << 54;
    regs[1] = ((u64)(tex->levels - 1) << 2) | 0xFFA0000000E0;
    regs[2] = w;
    return -1;
}
#endif /* NON_MATCHING */
