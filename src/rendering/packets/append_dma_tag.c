#include "types.h"

extern u32 D_00160F00;

void append_dma_tag(u32 addr) {
    u32 *tag;
    u32 *cursor;
    u32  value;

    cursor = (u32 *)D_00160F00;
    value = addr + 0x90000000;
    tag = cursor;
    cursor += 4;
    D_00160F00 = (u32)cursor;
    tag[0] = value;
    tag[1] = 0;
    tag[2] = 0;
    tag[3] = 0;
}
