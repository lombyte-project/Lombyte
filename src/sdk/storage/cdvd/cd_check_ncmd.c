#include "types.h"
#include "sifrpc.h"
#include "rnc/sdk/libcdvd.h"

extern s32  D_001312F8[];
const u8    D_00152EB0[] = "Ncmd fail sema cur_cmd:%d keep_cmd:%d\n";
extern u8   cd_ncmd_bind_error_message[] __asm__("D_00152ED8");
extern s32  D_00159750[];
extern u8   D_00159758[];
extern s32  PollSema();
extern void ReferThreadStatus();
extern void SignalSema();
extern void cmd_sem_init();
extern s32  sceCdSync();
extern void scePrintf();
extern s32  sceSifBindRpc();
extern void sceSifInitRpc();

s32 cd_check_ncmd(s32 cmd) __asm__("FUN_00120a28");

s32 cd_check_ncmd(s32 cmd) {
    s32 spin;

    cmd_sem_init();
    if (cd_ncmd_sema != PollSema(cd_ncmd_sema)) {
        if (cd_debug > 0) {
            scePrintf(D_00152EB0, cmd, cd_ncmd_keep_cmd);
        }
        return 0;
    }
    cd_ncmd_keep_cmd = cmd;
    ReferThreadStatus(D_00159750[0], D_00159758);
    if (sceCdSync(1) != 0) {
        SignalSema(cd_ncmd_sema);
        return 0;
    }
    sceSifInitRpc(0);
    if (D_001312F8[0] >= 0) {
        return 1;
    }
    for (;;) {
        if (sceSifBindRpc(&cd_ncmd_client, 0x80000595, 0) < 0) {
            if (cd_debug > 0) {
                scePrintf(cd_ncmd_bind_error_message);
            }
            for (spin = 0x100000; spin != -1; spin--) {
            }
            continue;
        }
        if (cd_ncmd_client.serve != 0) {
            break;
        }
        for (spin = 0x100000; spin != -1; spin--) {
        }
    }
    D_001312F8[0] = 0;
    return 1;
}

extern __typeof__(cd_check_ncmd) func_00120A28 __attribute__((alias("FUN_00120a28")));
