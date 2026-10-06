#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00121190/FUN_00121190.s", FUN_00121190);
#else
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef volatile s8 vs8;
typedef volatile u8 vu8;
typedef volatile s16 vs16;
typedef volatile u16 vu16;
typedef volatile s32 vs32;
typedef volatile u32 vu32;
typedef volatile s64 vs64;
typedef volatile u64 vu64;
typedef float f32;
typedef double f64;
typedef s32 b32;
struct NativeCdDiskReadyRpcClient {
    u8 pad_0[0x24];
    s32 server;
};
extern s32 D_001312D0[];
extern volatile s32 D_001312EC[];
extern s32 D_00131304[];
extern struct NativeCdDiskReadyRpcClient D_00159990;
extern s32 D_001599D0[];
extern s32 D_001324C0[];
extern u8 D_00152F80[];
extern u8 D_00152F90[];
extern u8 D_00152FB0[];
extern s32 PollSema();
extern void SignalSema();
extern void cmd_sem_init();
extern s32 sceCdSyncS();
extern void scePrintf();
extern s32 sceSifBindRpc();
extern void sceSifInitRpc();
extern void sceSifWriteBackDCache();
extern s32 sceSifCallRpc();
s32 FUN_00121190(s32 mode) {
    s32 response;
    s32 bind_wait_counter;
    s32 poll_result;
    if (D_001312D0[0] > 0) {
        scePrintf(D_00152F80);
    }
    cmd_sem_init();
    poll_result = PollSema(D_001312EC[0]);
    if (D_001312EC[0] != poll_result) {
        return 6;
    }
    if (sceCdSyncS(1) != 0) {
        goto fail;
    }
    sceSifInitRpc(0);
    if (D_00131304[0] < 0) {
        for (;;) {
            if (sceSifBindRpc(&D_00159990, 0x8000059A, 0) < 0) {
                if (D_001312D0[0] > 0) {
                    scePrintf(D_00152F90);
                }
                for (bind_wait_counter = 0x100000; bind_wait_counter != (-1); bind_wait_counter--) {
                }

                continue;
            }
            if (D_00159990.server != 0) {
                break;
            }
            for (bind_wait_counter = 0x100000; bind_wait_counter != (-1); bind_wait_counter--) {
            }
        }

        if (mode) {
            D_00131304[0] = 0;
        } else {
            D_00131304[0] = 0;
        }
    }
    D_001599D0[0] = mode;
    sceSifWriteBackDCache(D_001599D0, 4);
    if (sceSifCallRpc(&D_00159990, 0, 0, D_001599D0, 4, D_001324C0, 4, 0, 0) < 0) {
    fail:
        SignalSema(D_001312EC[0]);

        if (mode == 8) {
            return -1;
        }
        return 6;
    }
    if (D_001312D0[0] > 0) {
        scePrintf(D_00152FB0);
    }
    response = *((s32 *)(((u32)D_001324C0) | 0x20000000));
    SignalSema(D_001312EC[0]);
    return response;
}
#endif /* NON_MATCHING */
