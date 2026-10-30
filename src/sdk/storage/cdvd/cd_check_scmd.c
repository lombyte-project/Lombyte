#include "types.h"
#include "sifrpc.h"
#include "rnc/sdk/libcdvd.h"

extern s32  D_00131308[];
const u8    D_00152F10[] = "Scmd fail sema cur_cmd:%d keep_cmd:%d\n";
extern u8   cd_s_command_bind_error_message[] __asm__("D_00152F38");
extern s32  D_00159750[];
extern u8   D_00159758[];
extern s32  PollSema();
extern void ReferThreadStatus();
extern void SignalSema();
extern void cmd_sem_init();
extern s32  sceCdSyncS();
extern void scePrintf();
extern s32  sceSifBindRpc();
extern void sceSifInitRpc();

s32 cd_check_scmd(s32 cmd) __asm__("FUN_00120d40");

s32 cd_check_scmd(s32 cmd) {
    s32 spin;

    cmd_sem_init();
    if (cd_scmd_sema != PollSema(cd_scmd_sema)) {
        if (cd_debug > 0) {
            scePrintf(D_00152F10, cmd, cd_scmd_keep_cmd);
        }
        return 0;
    }
    cd_scmd_keep_cmd = cmd;
    ReferThreadStatus(D_00159750[0], D_00159758);
    if (sceCdSyncS(1) != 0) {
        SignalSema(cd_scmd_sema);
        return 0;
    }
    sceSifInitRpc(0);
    if (D_00131308[0] >= 0) {
        return 1;
    }
    for (;;) {
        if (sceSifBindRpc(&cd_scmd_client, 0x80000593, 0) < 0) {
            if (cd_debug > 0) {
                scePrintf(cd_s_command_bind_error_message);
            }
            for (spin = 0x100000; spin != -1; spin--) {
            }
            continue;
        }
        if (cd_scmd_client.serve != 0) {
            break;
        }
        for (spin = 0x100000; spin != -1; spin--) {
        }
    }
    D_00131308[0] = 0;
    return 1;
}

extern __typeof__(cd_check_scmd) func_00120D40 __attribute__((alias("FUN_00120d40")));
