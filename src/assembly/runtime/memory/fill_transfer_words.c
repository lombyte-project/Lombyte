#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact low-cost entry recovered with target symbolic relocations. */
INCLUDE_ASM(
    "config/us/expected/asm/assembly/runtime/memory/fill_transfer_words/FillTransferWords.s",
    FillTransferWords);
#else
#include "types.h"

/* size is a byte count. The retail routine writes the first word even when
 * size is zero or negative, then advances in four-byte steps. Callers supply
 * word-aligned destinations and normally use sizes divisible by four. */
void FillTransferWords(void *dst, s32 value, s32 size) {
    volatile u32 *words = (volatile u32 *)dst;
    u32 remaining = (u32)size;
    do {
        *words = (u32)value;
        remaining -= 4;
        words++;
        if ((s32)remaining > 0) {
            *words = (u32)value;
            remaining -= 4;
            words++;
        } else {
            return;
        }
    } while ((s32)remaining > 0);
}
#endif /* NON_MATCHING */
