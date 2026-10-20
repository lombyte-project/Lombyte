#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit sceCdInit; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/sceCdInit/sceCdInit.s", sceCdInit);
#else
#include "types.h"

/* libcdvd: bind the CD/DVD init RPC server, send the init mode and read
   back the drive state. */

typedef struct {
    u8 pad0[0x24];
    void *server;
    u8 pad28[0x10];
} sceSifClientData;

extern s32 D_001312D0[]; /* debug level */
extern s32 D_001312E0[];
extern s32 D_001312E4[];
extern s32 D_001312E8[];
extern s32 D_001312EC[];
extern s32 D_001312F4[];
extern s32 D_001312F8[];
extern s32 D_001312FC[];
extern s32 D_00131300[];
extern s32 D_00131304[];
extern s32 D_00131308[];
extern s32 D_0013130C[];
extern s32 D_00131310[];
extern u8 D_001324C0[];
extern char D_00152F50[];
extern char D_00152F70[];
extern s32 D_00159750[];
extern sceSifClientData D_00159968;
extern s32 D_001599C0[];

extern s32 GetThreadId(void);
extern s32 PowerOffCB(void);
extern void cdvd_exit(void);
extern void cmd_sem_init(void);
extern s32 sceCdSyncS(s32 mode);
extern int scePrintf(const char *format, ...);
extern s32 sceSifBindRpc(sceSifClientData *cd, u32 request, u32 mode);
extern s32 sceSifCallRpc(sceSifClientData *cd, u32 fno, u32 mode, void *send, s32 ssize,
                         void *receive, s32 rsize, void *end_func, void *end_para);
extern void sceSifInitRpc(u32 mode);
extern void sceSifWriteBackDCache(void *addr, s32 size);

int sceCdInit(int init_mode) {
    int ret;
    int bind_result;
    int delay;
    int reply_status;
    int first_version;
    int second_version;
    int owner;
    u8 *receive_buffer;

    if (sceCdSyncS(1) != 0) {
        return 0;
    }
    sceSifInitRpc(0);
    owner = GetThreadId();
    D_00131300[0] = -1;
    *(volatile s32 *)D_001312E4 = 1;
    *(volatile s32 *)D_00159750 = owner;
    D_001312F8[0] = -1;
    D_00131308[0] = -1;
    D_00131304[0] = -1;
    D_0013130C[0] = -1;
    D_001312F4[0] = 0;
    D_001312FC[0] = -1;
    D_00131310[0]++;
    while (1) {
        bind_result = sceSifBindRpc(&D_00159968, 0x80000592, 0);
        if (bind_result < 0) {
            if (D_001312D0[0] > 0) {
                scePrintf(D_00152F50, bind_result, D_00131310[0]);
            }
            delay = 0x100000;
            while (delay-- != 0) {
            }
            continue;
        }
        if (D_00159968.server != 0) {
            D_001599C0[0] = init_mode;
            D_0013130C[0] = 0;
            sceSifWriteBackDCache(D_001599C0, 4);
            receive_buffer = D_001324C0;
            if (sceSifCallRpc(&D_00159968, 0, 0, D_001599C0, 4, receive_buffer, 0x10, 0, 0) < 0) {
                *(volatile s32 *)D_001312E4 = 0;
                return 0;
            }
            break;
        }
        delay = 0x100000;
        while (delay-- != 0) {
        }
    }
    /* Read the RPC result through the EE uncached alias. The signed division
       below preserves the retail rounding for negative response components. */
    first_version = *(s32 *)((u32)(receive_buffer + 4) | 0x20000000);
    second_version = *(s32 *)((u32)(receive_buffer + 8) | 0x20000000);
    reply_status = *(s32 *)((u32)(receive_buffer + 0xC) | 0x20000000);
    ret = 1;
    if (reply_status == 0xFF) {
    } else if (reply_status == 0xFE) {
        D_001312D0[0] = ret;
    } else {
        if (first_version / 256 < 2 || second_version / 256 < 2) {
            ret = 2;
        }
    }
    *(volatile s32 *)D_001312E4 = 0;
    switch (init_mode) {
    case 0:
    case 1:
        break;
    case 5:
        if (D_001312D0[0] > 0) {
            scePrintf(D_00152F70);
        }
        cdvd_exit();
        *(volatile s32 *)D_001312E8 = -1;
        *(volatile s32 *)D_001312EC = -1;
        *(volatile s32 *)D_001312E0 = -1;
        goto done;
    }
    cmd_sem_init();
    PowerOffCB();
done:
    return ret;
}
#endif /* NON_MATCHING */
