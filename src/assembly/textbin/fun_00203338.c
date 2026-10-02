#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00203338/FUN_00203338.s", FUN_00203338);
#else
#include "types.h"
#include "eetypes.h"

typedef struct {
    s32 w0;
    s32 w4;
    u8 pad8[0x8];
    s32 w10;
    s32 w14;
    u8 pad18[0x8];
    s32 w20;
    u8 pad24[0x1C];
} Block;

typedef struct {
    s32 blocks;
    s32 count;
    s32 unk8;
    s32 unkC;
} Chunk;

typedef struct {
    u8 b0;
    u8 pad1[0xB];
    s32 unkC;
} Remap;

typedef struct {
    u8 pad0[0x10];
    u8 count;
    u8 pad11[3];
    s32 unk14;
    u8 pad18[4];
    s32 offs[1];
} Sub;

typedef struct {
    s32 chunks;
    u8 n0;
    u8 n1;
    u8 n2;
    u8 pad7[5];
    u8 nsub;
    u8 padD[3];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 *unk1C;
    s32 unk20;
    u8 pad24[4];
    s32 unk28;
    u8 pad2C[0x1C];
    s32 subs[1];
} Header;

typedef union {
    u128 q;
    u8 b[16];
} Map16;

extern u8 D_001B3AC0[];
extern Map16 D_001B6880[];

extern void func_00202D78(Block *, void *, s32, s32, s32, s32, s32);
extern void set_up_vis_gif_viewer(Block *q, s32 n, s32 prim, s32 a3, s32 t0, s32 mode) __asm__("FUN_00202fd0");

void FUN_00203338(Header *hdr, u8 *tex, u8 *map, s32 cls) {
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 cnt;
    Chunk *c;
    Chunk *c1;
    Remap *r;
    u8 *p;
    Sub *sub;
    s32 *o;
    Map16 *slot;
    Block *blk;
    s32 w;
    s32 lo;
    s32 mode;
    s32 num;
    s32 m;
    s32 q;
    s32 t;

    n = hdr->n0 + hdr->n1 + hdr->n2;
    if (hdr->chunks != 0) {
        hdr->chunks = (s32)hdr + hdr->chunks;
        c1 = (Chunk *)hdr->chunks;
        if (n != 0) {
            k = n;
            do {
                c1->blocks += (s32)hdr;
                c1->unk8 += (s32)hdr;
                k--;
                c1++;
            } while (k != 0);
        }
    }
    if (hdr->unk10 != 0) {
        hdr->unk10 = (s32)hdr + hdr->unk10;
    }
    if (hdr->unk14 != 0) {
        hdr->unk14 = (s32)hdr + hdr->unk14;
    }
    if (hdr->unk18 != 0) {
        hdr->unk18 = (s32)hdr + hdr->unk18;
    }
    if (hdr->unk1C != 0) {
        hdr->unk1C = (s32 *)((u8 *)hdr + (s32)hdr->unk1C);
        num = hdr->unk1C[0];
        for (m = 0; m < num; m++) {
            hdr->unk1C[m + 1] += (s32)hdr;
        }
    }
    if (hdr->unk20 != 0) {
        hdr->unk20 = (s32)hdr + hdr->unk20;
        r = (Remap *)hdr->unk20;
        do {
            r->unkC += (s32)hdr;
            p = &r->b0;
            if (r->b0 != 0xFF) {
                do {
                    *p = map[*p];
                    p++;
                } while (*p != 0xFF);
            }
        } while (r->unkC >= 0 && (r++, 1));
    }
    if (hdr->unk28 != 0) {
        hdr->unk28 = (s32)hdr + hdr->unk28;
    }
    for (q = 0; q < hdr->nsub; q++) {
        if (hdr->subs[q] != 0) {
            sub = (Sub *)((u8 *)hdr + hdr->subs[q]);
            hdr->subs[q] = (s32)sub;
            if (sub->unk14 != 0) {
                sub->unk14 = (s32)hdr + sub->unk14;
            }
            if (sub->count != 0) {
                t = 0;
                o = sub->offs;
                do {
                    *o = (s32)hdr + *o;
                    t++;
                    o++;
                } while (t < sub->count);
            }
        }
    }

    i = D_001B3AC0[cls];
    slot = &D_001B6880[i];
    c = (Chunk *)hdr->chunks;
    slot->q = *(u128 *)map;
    for (i = 0; i < n; i++, c++) {
        w = c->count;
        cnt = w >> 16;
        lo = w & 0xFFFF;
        c->count = lo;
        blk = (Block *)(c->blocks + (lo - cnt) * 16);
        for (j = 0; j < cnt; j += 4) {
            mode = blk->w20;
            if (mode >= 0) {
                mode = slot->b[mode];
            }
            if (tex != NULL) {
                func_00202D78(blk, tex + mode * 16, blk->w0, blk->w4, blk->w10, blk->w14, mode);
            } else {
                set_up_vis_gif_viewer(blk, blk->w0, blk->w4, blk->w10, blk->w14, mode);
            }
            blk++;
        }
    }
}
#endif /* NON_MATCHING */
