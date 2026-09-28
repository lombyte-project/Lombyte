/* Ported from rac1-decomp, the PAL decompilation (src/game/movie/disp.c, func_0023C5E0). */
#include "sda.h"
extern int D_0015EE84 MACRO_ADDR;
extern void *D_0015EEB8 MACRO_ADDR;
extern char D_00151DF0[];
/* setImageTag (SDK ezmpeg sample): builds the DMA/GIF chain that uploads a
   decoded frame to VRAM: a header packet (BITBLTBUF from D_0015EE84), one
   0x60-byte packet per 16x16 block (TRXPOS = block x/y, TRXDIR, an IMAGE
   tag and a REF tag to the next 0x400 bytes of `texbuf`), then the closing
   tags. The TRXPOS value is built inside the inner loop and the packet is
   written in address order, so the loop pass hoists the invariants in the
   order retail's scheduler starts from (TRXPOS x part first, the REF tag
   before the address mask). */
void build_image_transfer_tag(void *p, int texbuf, int image_w, int image_h) __asm__("FUN_0023b210");

void build_image_transfer_tag(void *p, int texbuf, int image_w, int image_h) {
    char *out = (char *)p;
    int mbx = image_w >> 4;
    int mby = image_h >> 4;
    int x, y;

    *(long *)(out + 0x0) = 0x10000003L;
    *(long *)(out + 0x8) = 0;
    *(long *)(out + 0x10) = 0x1000000000000002L;
    *(long *)(out + 0x18) = 0xEL;
    *(long *)(out + 0x20) = ((long)(D_0015EE84 >> 8) << 32) | 0x8000000000000L;
    *(long *)(out + 0x28) = 0x50L;
    *(long *)(out + 0x30) = 0x1000000010L;
    *(long *)(out + 0x38) = 0x52L;
    out += 0x40;

    for (x = 0; x < mbx; x++) {
        for (y = 0; y < mby; y++) {
            *(long *)(out + 0x20) = ((long)(x << 4) << 32) | ((long)(y << 4) << 48);
            *(long *)(out + 0x0) = 0x10000004L;
            *(long *)(out + 0x8) = 0;
            *(long *)(out + 0x10) = 0x1000000000000002L;
            *(long *)(out + 0x18) = 0xEL;
            *(long *)(out + 0x28) = 0x51L;
            *(long *)(out + 0x30) = 0;
            *(long *)(out + 0x38) = 0x53L;
            *(long *)(out + 0x40) = 0x800000000000040L;
            *(long *)(out + 0x48) = 0;
            *(int *)(out + 0x50) = 0x30000040;
            *(int *)(out + 0x54) = texbuf & 0xFFFFFFF;
            *(long *)(out + 0x58) = 0;
            texbuf += 0x400;
            out += 0x60;
        }
    }

    *(int *)(out + 0x0) = 0x30000009;
    *(int *)(out + 0x4) = ((int)D_0015EEB8 + 0xC0) & 0xFFFFFFF;
    *(int *)(out + 0x8) = 0;
    *(int *)(out + 0xC) = 0x50000009;
    *(int *)(out + 0x10) = 0x30000025;
    *(int *)(out + 0x14) = (int)D_00151DF0;
    *(int *)(out + 0x18) = 0;
    *(int *)(out + 0x1C) = 0x50000025;
    out += 0x20;
    *(long *)(out + 0x0) = 0x70000000L;
    *(long *)(out + 0x8) = 0;
    out += 0x20;
}

extern __typeof__(build_image_transfer_tag) func_0023B210 __attribute__((alias("FUN_0023b210")));
