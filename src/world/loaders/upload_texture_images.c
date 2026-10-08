#include "types.h"
#include "rnc/sdk/libgraph.h"
#include "eetypes.h"
#include "rnc/globals.h"

struct Ent {
    s32 unk0;
    s32 unk4;
    u8 pad8[4];
    s32 unkC;
};

extern s32 D_0015EE8C;
extern void FlushCache(s32 a0);
extern s32 sceGsSetDefLoadImage(sceGsLoadImage *img, s32 x, s32 y, s32 w, s32 h, s32 a4, s32 a6,
                                s32 a7);
extern s32 sceGsExecLoadImage(sceGsLoadImage *img, s32 addr);
extern s32 FUN_00120558(s32 a0, s32 a1);

void upload_texture_images(s32 arg0, s32 count, struct Ent *p) __asm__("FUN_00203120");

void upload_texture_images(s32 arg0, s32 count, struct Ent *p) {
    sceGsLoadImage img;
    s32 i;
    s32 dst;
    s32 hi;
    s32 lo;
    s32 t;
    s32 prod;
    s32 vram;
    s32 one;
    s32 x;

    gs_texture_allocation_cursor = D_0015EE8C;
    gs_texture_allocation_start = D_0015EE8C;
    if (count > 0) {
        i = count;
        one = 1;
        while (i != 0) {
            x = p->unk4;
            dst = arg0 + p->unkC;
            vram = gs_texture_allocation_cursor;
            lo = (u16)x;
            hi = x >> 16;
            if (p->unk0 == 0x13) {
                t = lo >> 6;
                sceGsSetDefLoadImage(&img, (vram << 8) >> 16, t == 0 ? one : t, 0x13, 0, 0, (s16)lo,
                                     hi);
                prod = lo * hi;
                if (prod <= 0xFF) {
                    prod = 0x100;
                }
                gs_texture_allocation_cursor += prod;
            } else if (p->unk0 == 2) {
                sceGsSetDefLoadImage(&img, (vram << 8) >> 16, 1, 2, 0, 0, 0x10, 0x10);
                gs_texture_allocation_cursor = gs_texture_allocation_cursor + 0x200;
            } else if (p->unk0 == 0) {
                sceGsSetDefLoadImage(&img, (vram << 8) >> 16, 1, 0, 0, 0, 0x10, 0x10);
                gs_texture_allocation_cursor = gs_texture_allocation_cursor + 0x400;
            }
            i--;
            FlushCache(0);
            p++;
            sceGsExecLoadImage(&img, dst);
            FUN_00120558(0, 0);
        }
    }
    gs_texture_allocation_start = gs_texture_allocation_cursor;
}

extern __typeof__(upload_texture_images) func_00203120 __attribute__((alias("FUN_00203120")));
