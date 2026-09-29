/* Ported from rac1-decomp, the PAL decompilation (src/game/map.c, func_00205520). */
#include "sda.h"

extern int FUN_001f97a0(int);
extern int D_0015EE74 MACRO_ADDR;
extern int D_0015F458 MACRO_ADDR;
typedef struct {
    int addr;            /* +0 */
    short unk4;          /* +4 */
    short cbp;           /* +6 */
    int unk8;            /* +8 */
    unsigned char tw;    /* +C */
    unsigned char th;    /* +D */
    short tbp;           /* +E */
} TexSlot;
extern TexSlot D_0018D040[];
typedef struct {
    char *clut;          /* 0x00 */
    char *pix;           /* 0x04 */
    char pad08[0xC];
    int clutSize;        /* 0x14 */
    char pad18[0x34];
    int tw;              /* 0x4C */
    int th;              /* 0x50 */
    char pad54[0xC];
} TexDesc;

/* Loads a 256-colour texture file p (width at +8, height at +0xC, CLUT
   at +0x20, pixels after it): the CLUT and the pixels take the next VRAM
   space at D_0015EE74 (cbp, then tbp 0x400 later, then the allocator
   moves on by 1 << (tw + th)), and the texture is registered and its GS
   TEX0 value returned. */
long FUN_00204cf0(char *p) __asm__("FUN_00204cf0");

long FUN_00204cf0(char *p) {
    TexDesc t;
    int w;
    int cbp;
    int tbp;
    int addr;
    long reg;
    long tex0;
    long *pt;

    t.clut = p + 0x20;
    t.clutSize = 0x400;
    t.tw = FUN_001f97a0(*(int *)(p + 8));
    t.th = FUN_001f97a0(*(int *)(p + 0xC));
    t.pix = p + 0x20 + t.clutSize;
    addr = D_0015EE74;
    cbp = addr >> 8;
    addr += 0x400;
    tbp = addr >> 8;
    D_0015EE74 = addr + (1 << (t.tw + t.th));
    w = t.tw - 6;
    if (w < 0) {
        w = 0;
    }
    w = 1 << w;
    tex0 = (long)tbp | ((long)w << 14) | ((long)0x13 << 20) | ((long)t.tw << 26);
    pt = &tex0;
    tex0 = *pt | ((long)t.th << 30) | ((long)1 << 34) | ((long)cbp << 37);
    reg = *pt | ((long)4 << 61);
    if (D_0015F458 < 0x40) {
        D_0018D040[D_0015F458].addr = (int)t.clut;
        D_0018D040[D_0015F458].cbp = cbp;
        D_0018D040[D_0015F458].unk4 = 0;
        D_0018D040[D_0015F458].unk8 = (int)t.pix;
        D_0018D040[D_0015F458].tw = t.tw;
        D_0018D040[D_0015F458].th = t.th;
        D_0018D040[D_0015F458].tbp = tbp;
        D_0015F458++;
    }
    return reg;
}

extern __typeof__(FUN_00204cf0) func_00204CF0 __attribute__((alias("FUN_00204cf0")));
