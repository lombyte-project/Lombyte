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

#endif /* LOMBYTE_RNC_RENDERING_DMA_TAG_H */
