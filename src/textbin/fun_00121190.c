#include "types.h"
#include "sifrpc.h"

extern s32 D_001312D0[];
extern volatile s32 D_001312EC[];
extern s32 D_00131304[];
extern struct sceSifClientData D_00159990;
extern s32 D_001599D0[];
extern u8 D_001324C0[];
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

/* libcdvd disk-ready query (the sceCdDiskReady pattern): take the S-command
   semaphore, bind the 0x8000059A server once, send mode and return the
   server's status word. Mode 8 reports -1 on failure, every other mode 6. */
s32 FUN_00121190(s32 mode) {
    s32 response;
    s32 spin;

    if (D_001312D0[0] > 0) {
        scePrintf(D_00152F80);
    }
    cmd_sem_init();
    if (D_001312EC[0] != PollSema(D_001312EC[0])) {
        return 6;
    }
    if (sceCdSyncS(1) != 0) {
        SignalSema(D_001312EC[0]);
        if (mode == 8) {
            return -1;
        }
        return 6;
    }
    sceSifInitRpc(0);
    if (D_00131304[0] < 0) {
        for (;;) {
            if (sceSifBindRpc(&D_00159990, 0x8000059A, 0) < 0) {
                if (D_001312D0[0] > 0) {
                    scePrintf(D_00152F90);
                }
                for (spin = 0x100000; spin != -1; spin--) {
                }
                continue;
            }
            if (D_00159990.serve != 0) {
                D_00131304[0] = 0;
                break;
            }
            for (spin = 0x100000; spin != -1; spin--) {
            }
        }
    }
    D_001599D0[0] = mode;
    sceSifWriteBackDCache(D_001599D0, 4);
    if (sceSifCallRpc(&D_00159990, 0, 0, D_001599D0, 4, D_001324C0, 4, 0, 0) < 0) {
        SignalSema(D_001312EC[0]);
        if (mode == 8) {
            return -1;
        }
        return 6;
    }
    if (D_001312D0[0] > 0) {
        scePrintf(D_00152FB0);
    }
    response = *(s32 *)((u32)D_001324C0 | 0x20000000);
    SignalSema(D_001312EC[0]);
    return response;
}

extern __typeof__(FUN_00121190) func_00121190 __attribute__((alias("FUN_00121190")));
