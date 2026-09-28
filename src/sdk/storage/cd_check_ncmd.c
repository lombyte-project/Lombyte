#include "types.h"

struct M2c_D_00132490 {
    u8 pad_0[0x24];
    s32 unk24;
};

extern s32 D_001312D0[];
extern s32 D_001312DC[];
extern volatile s32 D_001312E8[];
extern s32 D_001312F8[];
extern struct M2c_D_00132490 D_00132490;
extern u8 D_00152EB0[];
extern u8 D_00152ED8[];
extern s32 D_00159750[];
extern u8 D_00159758[];
extern s32 PollSema();
extern void ReferThreadStatus();
extern void SignalSema();
extern void cmd_sem_init();
extern s32 sceCdSync();
extern void scePrintf();
extern s32 sceSifBindRpc();
extern void sceSifInitRpc();

s32 cd_check_ncmd(s32 arg0) __asm__("FUN_00120a28");

s32 cd_check_ncmd(s32 arg0) {
    s32 spin;

    cmd_sem_init();
    if (D_001312E8[0] != PollSema(D_001312E8[0])) {
        if (D_001312D0[0] > 0) {
            scePrintf(D_00152EB0, arg0, D_001312DC[0]);
        }
        return 0;
    }
    D_001312DC[0] = arg0;
    ReferThreadStatus(D_00159750[0], D_00159758);
    if (sceCdSync(1) != 0) {
        SignalSema(D_001312E8[0]);
        return 0;
    }
    sceSifInitRpc(0);
    if (D_001312F8[0] >= 0) {
        return 1;
    }
    for (;;) {
        if (sceSifBindRpc(&D_00132490, 0x80000595, 0) < 0) {
            if (D_001312D0[0] > 0) {
                scePrintf(D_00152ED8);
            }
            for (spin = 0x100000; spin != -1; spin--) {
            }
            continue;
        }
        if (D_00132490.unk24 != 0) {
            break;
        }
        for (spin = 0x100000; spin != -1; spin--) {
        }
    }
    D_001312F8[0] = 0;
    return 1;
}

extern __typeof__(cd_check_ncmd) func_00120A28 __attribute__((alias("FUN_00120a28")));
