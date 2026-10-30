/* Ported from rac1-decomp (src/game/movie/disp.c, func_0023C5E0). */
#include "sda.h"

extern int              draw_buffer_address_macro __asm__("D_0015EE84") MACRO_ADDR;
extern void *D_0015EEB8 MACRO_ADDR;

#include "rnc/rendering/fs_aa_packets.h"

/* setImageTag (SDK ezmpeg sample): builds the DMA/GIF chain that uploads a
   decoded frame to VRAM: a header packet (BITBLTBUF from draw_buffer_address_macro), one
   0x60-byte packet per 16x16 block (TRXPOS = block x/y, TRXDIR, an IMAGE
   tag and a REF tag to the next 0x400 bytes of `texbuf`), then the closing
   tags. The TRXPOS value is built inside the inner loop and the packet is
   written in address order, so the loop pass hoists the invariants in the
   order retail's scheduler starts from (TRXPOS x part first, the REF tag
   before the address mask). */
void set_image_tag(void *p, int texbuf, int image_w, int image_h) __asm__("FUN_0023b210");

void set_image_tag(void *tag_buffer, int texbuf, int image_w, int image_h) {
    char *tag_write_cursor = (char *)tag_buffer;
    int   mbx = image_w >> 4;
    int   mby = image_h >> 4;
    int   x, y;

    *(long *)(tag_write_cursor + 0x0) = 0x10000003L;
    *(long *)(tag_write_cursor + 0x8) = 0;
    *(long *)(tag_write_cursor + 0x10) = 0x1000000000000002L;
    *(long *)(tag_write_cursor + 0x18) = 0xEL;
    *(long *)(tag_write_cursor + 0x20) =
        ((long)(draw_buffer_address_macro >> 8) << 32) | 0x8000000000000L;
    *(long *)(tag_write_cursor + 0x28) = 0x50L;
    *(long *)(tag_write_cursor + 0x30) = 0x1000000010L;
    *(long *)(tag_write_cursor + 0x38) = 0x52L;
    tag_write_cursor += 0x40;

    for (x = 0; x < mbx; x++) {
        for (y = 0; y < mby; y++) {
            *(long *)(tag_write_cursor + 0x20) = ((long)(x << 4) << 32) | ((long)(y << 4) << 48);
            *(long *)(tag_write_cursor + 0x0) = 0x10000004L;
            *(long *)(tag_write_cursor + 0x8) = 0;
            *(long *)(tag_write_cursor + 0x10) = 0x1000000000000002L;
            *(long *)(tag_write_cursor + 0x18) = 0xEL;
            *(long *)(tag_write_cursor + 0x28) = 0x51L;
            *(long *)(tag_write_cursor + 0x30) = 0;
            *(long *)(tag_write_cursor + 0x38) = 0x53L;
            *(long *)(tag_write_cursor + 0x40) = 0x800000000000040L;
            *(long *)(tag_write_cursor + 0x48) = 0;
            *(int *)(tag_write_cursor + 0x50) = 0x30000040;
            *(int *)(tag_write_cursor + 0x54) = texbuf & 0xFFFFFFF;
            *(long *)(tag_write_cursor + 0x58) = 0;
            texbuf += 0x400;
            tag_write_cursor += 0x60;
        }
    }

    *(int *)(tag_write_cursor + 0x0) = 0x30000009;
    *(int *)(tag_write_cursor + 0x4) = ((int)D_0015EEB8 + 0xC0) & 0xFFFFFFF;
    *(int *)(tag_write_cursor + 0x8) = 0;
    *(int *)(tag_write_cursor + 0xC) = 0x50000009;
    *(int *)(tag_write_cursor + 0x10) = 0x30000025;
    *(int *)(tag_write_cursor + 0x14) = (int)fs_aa_resample_packet;
    *(int *)(tag_write_cursor + 0x18) = 0;
    *(int *)(tag_write_cursor + 0x1C) = 0x50000025;
    tag_write_cursor += 0x20;
    *(long *)(tag_write_cursor + 0x0) = 0x70000000L;
    *(long *)(tag_write_cursor + 0x8) = 0;
    tag_write_cursor += 0x20;
}

extern __typeof__(set_image_tag) func_0023B210 __attribute__((alias("FUN_0023b210")));
