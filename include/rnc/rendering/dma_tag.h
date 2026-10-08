#ifndef LOMBYTE_RNC_RENDERING_DMA_TAG_H
#define LOMBYTE_RNC_RENDERING_DMA_TAG_H

#include "types.h"

/* One source-chain DMA tag quadword: the tag word (qwc | id << 28), the
   address, then two VIF codes sent with it (usually NOP and DIRECT). */
struct DmaTag {
    u32 tag;
    u32 addr;
    u32 vif0;
    u32 vif1;
};

/* Write cursor into a DMA packet being built. */
struct TagPtr {
    struct DmaTag *p;
};

struct GifTag; /* rnc/sdk/libgraph.h */

/* Write cursor of the packet the frame is building, seen as whatever the
   writer emits next: DMA tags, GIF tags, 32-bit words or raw bytes, or as a
   plain address for the frame setup code that reserves and swaps buffers.
   Writers store through one view and advance the cursor past what they wrote. */
union PacketCursor {
    struct DmaTag *tag;
    struct GifTag *gif;
    s32 *words;
    u8 *bytes;
    s32 addr;
};

/* Files whose code reaches the cursor through an absolute address instead of
   $gp define RENDER_PACKET_CURSOR_ATTR as MACRO_ADDR before including this. */
#ifndef RENDER_PACKET_CURSOR_ATTR
#define RENDER_PACKET_CURSOR_ATTR
#endif
extern union PacketCursor render_packet_cursor __asm__("D_00160F00") RENDER_PACKET_CURSOR_ATTR;

#endif /* LOMBYTE_RNC_RENDERING_DMA_TAG_H */
