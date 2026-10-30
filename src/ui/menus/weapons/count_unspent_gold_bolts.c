#include "types.h"

extern s32 count_gold_bolts() __asm__("FUN_00215290");
extern s32 count_gold_weapons() __asm__("FUN_00215300");
s32        count_unspent_gold_bolts(void) __asm__("FUN_00215248");

s32 count_unspent_gold_bolts(void) {
    s32 collected;
    s32 unspent;
    s32 clamped_low;

    collected = count_gold_bolts();
    unspent = collected - (count_gold_weapons() * 4);
    clamped_low = (unspent <= -1) ? 0 : unspent;
    return (clamped_low < 0x29) ? clamped_low : 0x28;
}
