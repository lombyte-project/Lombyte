/* Ported from rac1-decomp (src/core/001208E8.c, func_001211B0). */

#include "types.h"
#include "rnc/sdk/libcdvd.h"

/* libcdvd: bind the CD/DVD init RPC server, send the init mode and read
   back the drive state. */

typedef struct {
    u8    pad0[0x24];
    void *server;
    u8    pad28[0x10];
} sceSifClientData;

extern s32              D_001312E4[];
extern s32              D_001312F4[];
extern s32              D_001312F8[];
extern s32              D_001312FC[];
extern s32              D_00131300[];
extern s32              D_00131304[];
extern s32              D_00131308[];
extern s32              cd_init_rpc_bind_state[] __asm__("D_0013130C");
extern s32              cd_initialization_count[] __asm__("D_00131310");
extern char             cd_init_bind_error_format[] __asm__("D_00152F50");
extern char             cd_exit_message[] __asm__("D_00152F70");
extern s32              D_00159750[];
extern sceSifClientData cd_init_rpc_client __asm__("D_00159968");
extern s32              cd_init_mode_request[] __asm__("D_001599C0");

extern s32  GetThreadId(void);
extern s32  PowerOffCB(void);
extern void cdvd_exit(void);
extern void cmd_sem_init(void);
extern s32  sceCdSyncS(s32 mode);
extern int  scePrintf(const char *format, ...);
extern s32  sceSifBindRpc(sceSifClientData *cd, u32 request, u32 mode);
extern s32  sceSifCallRpc(sceSifClientData *cd,
                          u32               fno,
                          u32               mode,
                          void             *send,
                          s32               ssize,
                          void             *receive,
                          s32               rsize,
                          void             *end_func,
                          void             *end_para);
extern void sceSifInitRpc(u32 mode);
extern void sceSifWriteBackDCache(void *addr, s32 size);

int sceCdInit(int init_mode) {
    enum {
        CD_INIT_SHUTDOWN = 5
    };
    int compatibility_result;
    int bind_result;
    int delay;
    int reply_status;
    int first_version;
    int second_version;
    int initializing_thread_id;
    u8 *receive_buffer;

    if (sceCdSyncS(1) != 0) {
        return 0;
    }
    sceSifInitRpc(0);
    initializing_thread_id = GetThreadId();
    *(volatile s32 *)D_001312E4 = 1;
    *(volatile s32 *)D_00159750 = initializing_thread_id;
    {
        s32 *p0 = D_00131300, *p1 = D_001312F8, *p2 = D_00131308, *p3 = D_00131304,
            *init_rpc_bind_state_ptr = cd_init_rpc_bind_state,
            *power_event_handler_state_ptr = D_001312FC, *p5 = D_001312F4;
        /* Fakematch: retail keeps the -1 in $a0. */
        register int m __asm__("$4") = -1;
        *power_event_handler_state_ptr = m;
        *p0 = m;
        *p1 = m;
        *p2 = m;
        *p3 = m;
        *p5 = 0;
        cd_initialization_count[0]++;
        *init_rpc_bind_state_ptr = m;
    }
    while (1) {
        bind_result = sceSifBindRpc(&cd_init_rpc_client, 0x80000592, 0);
        if (bind_result < 0) {
            if (cd_debug > 0) {
                scePrintf(cd_init_bind_error_format, bind_result, cd_initialization_count[0]);
            }
            delay = 0x100000;
            while (delay-- != 0) {
            }
            continue;
        }
        if (cd_init_rpc_client.server != 0) {
            cd_init_mode_request[0] = init_mode;
            cd_init_rpc_bind_state[0] = 0;
            sceSifWriteBackDCache(cd_init_mode_request, 4);
            receive_buffer = cd_scmd_result;
            if (sceSifCallRpc(&cd_init_rpc_client,
                              0,
                              0,
                              cd_init_mode_request,
                              4,
                              receive_buffer,
                              0x10,
                              0,
                              0) < 0) {
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
    compatibility_result = 1;
    if (reply_status == 0xFF) {
    } else if (reply_status == 0xFE) {
        cd_debug = compatibility_result;
    } else {
        if (first_version / 256 < 2 || second_version / 256 < 2) {
            compatibility_result = 2;
        }
    }
    *(volatile s32 *)D_001312E4 = 0;
    switch (init_mode) {
    case 0:
    case 1:
        break;
    case CD_INIT_SHUTDOWN:
        if (cd_debug > 0) {
            scePrintf(cd_exit_message);
        }
        cdvd_exit();
        cd_ncmd_sema = -1;
        cd_scmd_sema = -1;
        cd_callback_sema = -1;
        goto done;
    }
    cmd_sem_init();
    PowerOffCB();
done:
    return compatibility_result;
}
