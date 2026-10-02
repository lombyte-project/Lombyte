#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_002040e0/FUN_002040e0.s", FUN_002040e0);
#else
#include "types.h"
#include "sda.h"

#define SCE_GS_SET_TEX0(tbp, tbw, psm, tw, th, tcc, tfx, cbp, cpsm, csm, csa, cld) \
    ((u64)(tbp) | ((u64)(tbw) << 14) | ((u64)(psm) << 20) | ((u64)(tw) << 26) | \
    ((u64)(th) << 30) | ((u64)(tcc) << 34) | ((u64)(tfx) << 35) | ((u64)(cbp) << 37) | \
    ((u64)(cpsm) << 51) | ((u64)(csm) << 55) | ((u64)(csa) << 56) | ((u64)(cld) << 61))
#define SCE_GS_SET_TEX1(lcm, mxl, mmag, mmin, mtba, l, k) \
    ((u64)(lcm) | ((u64)(mxl) << 2) | ((u64)(mmag) << 5) | ((u64)(mmin) << 6) | \
    ((u64)(mtba) << 9) | ((u64)(l) << 19) | ((u64)(k) << 32))
#define SCE_GS_SET_CLAMP(wms, wmt, minu, maxu, minv, maxv) \
    ((u64)(wms) | ((u64)(wmt) << 2) | ((u64)(minu) << 4) | ((u64)(maxu) << 14) | \
    ((u64)(minv) << 24) | ((u64)(maxv) << 34))
#define SCE_GS_SET_MIPTBP1(tbp1, tbw1, tbp2, tbw2, tbp3, tbw3) \
    ((u64)(tbp1) | ((u64)(tbw1) << 14) | ((u64)(tbp2) << 20) | ((u64)(tbw2) << 34) | \
    ((u64)(tbp3) << 40) | ((u64)(tbw3) << 54))

typedef struct {
    s32 id;
    s16 width;
    s16 height;
    s16 f8;
    s16 clut;
    s16 fC;
    s16 fE;
} TexInfo;

typedef struct {
    u64 data;
    u64 addr;
} GifAD;

typedef struct {
    s32 tex;
    s32 pad4[3];
    s32 f10;
    s32 f14;
    s32 pad18[2];
    s32 f20;
    s32 f24;
    s32 pad28[10];
} Sprite;

typedef struct {
    u8 pad0[0x10];
    u8 *data;
    u8 pad14[8];
    u16 off;
    u8 pad1E[0xA];
    u8 count;
    u8 pad29[0x17];
} SpriteSet;

typedef struct {
    s32 off;
    s32 count;
    f32 scale;
} SpriteHdr;

extern f32 D_00160EA0[3];
extern SpriteSet *D_00160E8C;
typedef struct { s32 v; } Count;
extern Count D_00160E90;
extern s32 D_0015EE8C;

extern s32 func_001F97A0(s32);
extern void func_00233068(f32 *);

void FUN_002040e0(SpriteHdr *h, TexInfo *texs) __asm__("FUN_002040e0");

void FUN_002040e0(SpriteHdr *h, TexInfo *texs)
{
    SpriteSet *set;
    SpriteSet *s;
    GifAD *p;
    TexInfo *t;
    s32 n;
    s32 i;
    s32 j;
    s32 id;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 w;
    s32 tbw;
    s32 tbw2;
    s32 tw;
    s32 th;
    s32 cb;
    f32 f;
    u64 d0;
    u64 d1;
    u64 d2;
    u64 d3;

    D_00160E90.v = h->count;
    f = h->scale;
    D_00160EA0[0] = f * 6.0f;
    D_00160EA0[1] = f * 4.0f;
    D_00160EA0[2] = f + f;
    func_00233068(D_00160EA0);
    set = (SpriteSet *)((u8 *)h + h->off);
    D_00160E8C = set;
    n = D_00160E90.v;
    for (i = 0; i < n; i++) {
        set[i].data = (u8 *)set + (s32)set[i].data;
    }
    for (i = 0; i < D_00160E90.v; i++) {
        for (j = 0; j < D_00160E8C[i].count; j++) {
            p = (GifAD *)(D_00160E8C[i].data + D_00160E8C[i].off + j * 0x50);
            id = ((Sprite *)p)->tex;
            c = ((Sprite *)p)->f20;
            a = ((Sprite *)p)->f10;
            b = ((Sprite *)p)->f14;
            t = &texs[id];
            d = ((Sprite *)p)->f24;
            w = t->width;
            tbw = w >> 6;
            tbw2 = w >> 7;
            if (tbw2 <= 0) {
                tbw2 = 1;
            }
            if (tbw <= 0) {
                tbw = 1;
            }
            tw = func_001F97A0(w);
            th = func_001F97A0(t->height);
            cb = D_0015EE8C >> 8;
            d0 = SCE_GS_SET_TEX0(0, tbw, 0x13, tw, th, 1, 0, t->clut + cb, 0, 0, 0, 4);
            d1 = SCE_GS_SET_TEX1(0, t->f8 - 1, 1, b, 0, 0, a);
            d2 = SCE_GS_SET_CLAMP(c, d, 0, 0, id, 0);
            d3 = SCE_GS_SET_MIPTBP1(0, tbw2, t->fC + cb, 1, t->fE + cb, 1);
            p->data = d0;
            p++;
            p->data = d1;
            p++;
            p->data = d2;
            p++;
            p->data = d3;
            p[1].data = 0;
        }
    }
}
#endif /* NON_MATCHING */
