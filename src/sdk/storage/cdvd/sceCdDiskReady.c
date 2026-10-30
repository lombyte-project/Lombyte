#include "types.h"
#include "sifrpc.h"
#include "rnc/sdk/libcdvd.h"

extern s32                     D_00131304[];
extern struct sceSifClientData cd_disk_ready_rpc_client __asm__("D_00159990");
extern s32                     D_001599D0[];
extern u8                      cd_disk_ready_entry_message[] __asm__("D_00152F80");
extern u8                      cd_disk_ready_bind_error_message[] __asm__("D_00152F90");
extern u8                      cd_disk_ready_completion_message[] __asm__("D_00152FB0");
extern s32                     PollSema();
extern void                    SignalSema();
extern void                    cmd_sem_init();
extern s32                     sceCdSyncS();
extern void                    scePrintf();
extern s32                     sceSifBindRpc();
extern void                    sceSifInitRpc();
extern void                    sceSifWriteBackDCache();
extern s32                     sceSifCallRpc();

/* libcdvd disk-ready query (the sceCdDiskReady pattern): take the S-command
   semaphore, bind the 0x8000059A server once, send mode and return the
   server's status word. Mode 8 reports -1 on failure, every other mode 6. */
s32 sceCdDiskReady(s32 mode) __asm__("FUN_00121190");

s32 sceCdDiskReady(s32 mode) {
    s32 response;
    s32 spin;

    if (cd_debug > 0) {
        scePrintf(cd_disk_ready_entry_message);
    }
    cmd_sem_init();
    if (cd_scmd_sema != PollSema(cd_scmd_sema)) {
        return 6;
    }
    if (sceCdSyncS(1) != 0) {
        SignalSema(cd_scmd_sema);
        if (mode == 8) {
            return -1;
        }
        return 6;
    }
    sceSifInitRpc(0);
    if (D_00131304[0] < 0) {
        for (;;) {
            if (sceSifBindRpc(&cd_disk_ready_rpc_client, 0x8000059A, 0) < 0) {
                if (cd_debug > 0) {
                    scePrintf(cd_disk_ready_bind_error_message);
                }
                for (spin = 0x100000; spin != -1; spin--) {
                }
                continue;
            }
            if (cd_disk_ready_rpc_client.serve != 0) {
                D_00131304[0] = 0;
                break;
            }
            for (spin = 0x100000; spin != -1; spin--) {
            }
        }
    }
    D_001599D0[0] = mode;
    sceSifWriteBackDCache(D_001599D0, 4);
    if (sceSifCallRpc(&cd_disk_ready_rpc_client, 0, 0, D_001599D0, 4, cd_scmd_result, 4, 0, 0) <
        0) {
        SignalSema(cd_scmd_sema);
        if (mode == 8) {
            return -1;
        }
        return 6;
    }
    if (cd_debug > 0) {
        scePrintf(cd_disk_ready_completion_message);
    }
    response = *(s32 *)((u32)cd_scmd_result | 0x20000000);
    SignalSema(cd_scmd_sema);
    return response;
}

extern __typeof__(sceCdDiskReady) func_00121190 __attribute__((alias("FUN_00121190")));
