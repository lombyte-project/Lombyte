#include "types.h"
extern s32 D_0015EC80 __attribute__((sda));
extern s32 D_0015ECC0 __attribute__((sda));
extern s32 D_0015ECC4 __attribute__((sda));
extern s32 *D_0015ECA0[2] __attribute__((sda));
extern u8 D_00133140[];
extern s32 D_00133100[4];
extern u8 D_0015EBC0[];
extern s32 printf(const char *, ...);
extern void snd_flush_sound_commands(void) __asm__("func_0012DC80");
extern s32 snd_got_returns(void) __asm__("func_0012DE70");
extern void snd_send_current_batch(void) __asm__("func_0012E9D8");
extern s32 FlushCache(s32);
extern s32 StoreObjectIndex(void *, s32);
extern s32 SceSifCheckStatRpc(void *);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);
s32 snd_send_iop_command_and_wait(s32 cmd, s32 size, u8 *data) __asm__("FUN_0012e548");

s32 snd_send_iop_command_and_wait(s32 cmd, s32 size, u8 *data) {
    s32 i;
    s32 result;

    for (i = 0; i < size; i++) {
        D_00133140[i] = data[i];
    }
    goto check;
    do {
        snd_flush_sound_commands();
        FlushCache(0);
    check:;
    } while (D_0015EC80 != 0);
    StoreObjectIndex(D_00133100, 1);
    while (SceSifCheckStatRpc(D_0015EBC0) != 0) {
        printf("989snd.c: RPC collision!\n");
        snd_flush_sound_commands();
        FlushCache(0);
    }
    if (size != 0) {
        sceSifCallRpc(D_0015EBC0, cmd, 1, D_00133140, size, D_00133100, 0xC, 0, 0);
    } else {
        sceSifCallRpc(D_0015EBC0, cmd, 1, 0, 0, D_00133100, 0xC, 0, 0);
    }
    while (snd_got_returns() == 0) {
    }
    result = D_00133100[1];
    if (*D_0015ECA0[D_0015ECC0] != 0 && D_0015ECC4 == 0) {
        snd_send_current_batch();
    }
    return result;
}

extern __typeof__(snd_send_iop_command_and_wait) func_0012E548
    __attribute__((alias("FUN_0012e548")));
