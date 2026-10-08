#include "types.h"
#include "rnc/storage/stash.h"
extern struct StashEntry D_001DD1D8[];
extern u8 D_001DD1A8[];
extern s32 LookupResourceEntry(s32);
extern s32 sceSifCallRpc(void *, u32, u32, void *, s32, void *, s32, void (*)(void *), void *);
s32 stash_receive_data(u8 *dst, u32 slot, s32 start, s32 count, u32 mode) __asm__("FUN_00232f20");

s32 stash_receive_data(u8 *dst, u32 slot, s32 start, s32 count, u32 mode) {
    s32 n;
    s32 buf[4];

    if (count == -1) {
        count = LookupResourceEntry(slot);
    }
    if (slot >= 64 || D_001DD1D8[slot].count == 0) {
        return -3;
    }
    if (D_001DD1D8[slot].count < start + count) {
        return -1;
    }
    buf[0] = D_001DD1D8[slot].addr + start * 16;
    while (count != 0) {
        n = count > 0xFFFF ? 0xFFFF : count;
        sceSifCallRpc(D_001DD1A8, 1, mode, buf, 16, dst, n * 16, 0, 0);
        count -= n;
        dst += n * 16;
        buf[0] += n * 16;
    }
    return 0;
}

extern __typeof__(stash_receive_data) func_00232F20 __attribute__((alias("FUN_00232f20")));
