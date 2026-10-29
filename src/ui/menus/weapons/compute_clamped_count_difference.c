#include "types.h"
extern s32 count_nonzero_entries_up_to_40() __asm__("FUN_00215290");
extern s32 count_nonzero_entries_up_to_10() __asm__("FUN_00215300");
s32 compute_clamped_count_difference(void) __asm__("FUN_00215248");

s32 compute_clamped_count_difference(void) {
    s32 entry_count;
    s32 unclamped_gold_bolt_balance;
    s32 clamped_low;

    entry_count = count_nonzero_entries_up_to_40();
    unclamped_gold_bolt_balance = entry_count - (count_nonzero_entries_up_to_10() * 4);
    clamped_low = (unclamped_gold_bolt_balance <= -1) ? 0 : unclamped_gold_bolt_balance;
    return (clamped_low < 0x29) ? clamped_low : 0x28;
}
