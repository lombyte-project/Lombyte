#include "types.h"

s32 AddMpegCallback(u8 *decoder, s32 index, s32 callback, s32 data) {
    u8 *base = *(u8 **)(decoder + 0x40);
    u8 *callback_slot = base + 0xC;
    s32 off = index << 3;
    s32 old;

    callback_slot += off;
    base += off;
    ((u32 *)base)[4] = data;
    old = ((u32 *)callback_slot)[0];
    ((u32 *)callback_slot)[0] = callback;
    return old;
}
