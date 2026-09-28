#include "types.h"
extern s32 D_0015ECC0;
extern s32 *D_0015ECA0[2];
extern s32 D_0015ECA8[2];
extern void *D_0015ECB8[2];
extern u8 D_0015EBC0[];
extern char D_00153D20[];
extern void StoreObjectIndex(void *, s32, s32 *);
extern s32 SceSifCheckStatRpc(void *);
extern s32 printf(const char *, ...);
extern void FlushCache(s32);
extern s32 sceSifCallRpc(void *, u32, u32, void *, s32, void *, s32, void (*)(void *), void *);
void snd_send_current_batch(void) __asm__("FUN_0012e9d8");

void snd_send_current_batch(void) {
    s32 next;

    StoreObjectIndex(D_0015ECB8[D_0015ECC0], *D_0015ECA0[D_0015ECC0], D_0015ECA0[D_0015ECC0]);
    while (SceSifCheckStatRpc(D_0015EBC0) != 0) {
        printf(D_00153D20);
        FlushCache(0);
    }
    sceSifCallRpc(D_0015EBC0, 0x4D, 1, D_0015ECA0[D_0015ECC0], 0x1000 - D_0015ECA8[D_0015ECC0],
                  D_0015ECB8[D_0015ECC0], (*D_0015ECA0[D_0015ECC0] << 2) + 8, 0, 0);
    next = D_0015ECC0 != 1;
    D_0015ECC0 = next;
    *D_0015ECA0[next] = 0;
    D_0015ECA8[next] = 0xFFC;
}

extern __typeof__(snd_send_current_batch) func_0012E9D8 __attribute__((alias("FUN_0012e9d8")));
