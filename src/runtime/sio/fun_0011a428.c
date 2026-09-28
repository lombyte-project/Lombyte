#include "types.h"

/* SIO transmit callback: stores the channel index field at a0+0x14 into the
   array a1+0x1C at slot a0+0x10.  Returns nothing (SIO calls it void). */
void FUN_0011a428(s32 *a0, s32 *a1) __asm__("FUN_0011a428");

void FUN_0011a428(s32 *a0, s32 *a1) {
    s32 *dst;
    s32 index;
    s32 value;

    index = *(s32 *)((u8 *)a0 + 0x10);
    dst = *(s32 **)((u8 *)a1 + 0x1C);
    value = *(s32 *)((u8 *)a0 + 0x14);
    dst[index] = value;
}

extern __typeof__(FUN_0011a428) func_0011A428 __attribute__((alias("FUN_0011a428")));
