#include "types.h"

/* The IOP return buffer this file owns: both functions below reach these
   gp-relative, which needs their definitions ahead of them. */
s32 *D_0015EC80 = 0;
s32 D_0015EC84 = 0;
extern u8 D_0015EBC0[];
extern u8 D_00153C98[];
extern void FlushCache();
extern s32 SceSifCheckStatRpc();
extern s32 printf();

s32 snd_got_returns(void) __asm__("FUN_0012de70");

s32 snd_got_returns(void) {
    s32 *p;

    FlushCache(0);
    if (D_0015EC80 == 0) {
        return 1;
    }
    if (SceSifCheckStatRpc(D_0015EBC0) != 0) {
        return 0;
    }
    p = D_0015EC80;
    if (p[0] != 0xFFFFFFFF) {
        printf(D_00153C98);
        return 0;
    }
    if (p[D_0015EC84 + 1] != 0xFFFFFFFF) {
        printf(D_00153C98);
        return 0;
    }
    D_0015EC80 = 0;
    return 1;
}

extern s32 func_0012DE70(void) __attribute__((alias("FUN_0012de70")));
extern s32 snd_GotReturns(void) __attribute__((alias("FUN_0012de70")));

void StoreObjectIndex(s32 *obj, s32 idx) {
    D_0015EC84 = idx;
    D_0015EC80 = obj;
    obj[idx + 1] = 0;
    obj[0] = 0;
}
