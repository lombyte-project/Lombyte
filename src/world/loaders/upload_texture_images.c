#include "types.h"
#include "rnc/sdk/libgraph.h"
#include "eetypes.h"
#include "rnc/globals.h"

/* One image of a texture upload list. */
struct TextureImage {
    s32 psm;         /* 0x0: GS pixel format: 0x13 PSMT8, 2 PSMCT16 CLUT, 0 PSMCT32 CLUT */
    s32 size;        /* 0x4: width in the low 16 bits, height in the high 16 */
    u8 pad8[4];
    s32 data_offset; /* 0xC: pixel data offset from the list's base address */
};

extern s32 D_0015EE8C;
extern void FlushCache(s32 a0);
extern s32 sceGsSetDefLoadImage(sceGsLoadImage *img, s32 dbp, s32 dbw, s32 psm, s32 x, s32 y, s32 w,
                                s32 h);
extern s32 sceGsExecLoadImage(sceGsLoadImage *img, s32 src);
extern s32 wait_for_graphics_pipeline_idle(s32 a0, s32 a1) __asm__("FUN_00120558");

void upload_texture_images(s32 base, s32 count, struct TextureImage *p) __asm__("FUN_00203120");

void upload_texture_images(s32 base, s32 count, struct TextureImage *p) {
    sceGsLoadImage img;
    s32 i;
    s32 dst;
    s32 height;
    s32 width;
    s32 t;
    s32 prod;
    s32 vram;
    s32 one;
    s32 size;

    gs_texture_allocation_cursor = D_0015EE8C;
    gs_texture_allocation_start = D_0015EE8C;
    if (count > 0) {
        i = count;
        one = 1;
        while (i != 0) {
            size = p->size;
            dst = base + p->data_offset;
            vram = gs_texture_allocation_cursor;
            width = (u16)size;
            height = size >> 16;
            if (p->psm == 0x13) {
                t = width >> 6;
                sceGsSetDefLoadImage(&img, (vram << 8) >> 16, t == 0 ? one : t, 0x13, 0, 0, (s16)width,
                                     height);
                prod = width * height;
                if (prod <= 0xFF) {
                    prod = 0x100;
                }
                gs_texture_allocation_cursor += prod;
            } else if (p->psm == 2) {
                sceGsSetDefLoadImage(&img, (vram << 8) >> 16, 1, 2, 0, 0, 0x10, 0x10);
                gs_texture_allocation_cursor = gs_texture_allocation_cursor + 0x200;
            } else if (p->psm == 0) {
                sceGsSetDefLoadImage(&img, (vram << 8) >> 16, 1, 0, 0, 0, 0x10, 0x10);
                gs_texture_allocation_cursor = gs_texture_allocation_cursor + 0x400;
            }
            i--;
            FlushCache(0);
            p++;
            sceGsExecLoadImage(&img, dst);
            wait_for_graphics_pipeline_idle(0, 0);
        }
    }
    gs_texture_allocation_start = gs_texture_allocation_cursor;
}

extern __typeof__(upload_texture_images) func_00203120 __attribute__((alias("FUN_00203120")));
