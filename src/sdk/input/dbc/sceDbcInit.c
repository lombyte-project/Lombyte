#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sdk_globals.h"

extern struct sceSifClientData dbc_transfer_rpc_client __asm__("D_0015B030");
extern char                    dbc_rpc_bind_failure_message[] __asm__("D_001534D8");
extern char                    dbc_module_version_mismatch_message[] __asm__("D_001534F0");
const char                     D_00153518[] = "[libdbc.a = %d.%d, dbcman.irx = %d.%d]\n";
const char                     D_00153540[] = "SifDmaAddr %08x\n";
extern void                    sceSifInitRpc(u32);
extern s32                     sceSifBindRpc(struct sceSifClientData *, u32, u32);
extern void                    debug_print_stub(const char *fmt, ...) __asm__("func_00124A20");
extern void                    exit(s32);
extern s32                     sceDbcGetModVersion(void);
extern void                    sceDbcSetWorkAddr(void *addr);
extern s32                     scePrintf(const char *, ...);

s32 sceDbcInit(void) {
    s32 ver;
    s32 major;
    s32 i;

    sceSifInitRpc(0);
    while (1) {
        if (sceSifBindRpc(&RpcQueueData, 0x80000900, 0) < 0) {
            debug_print_stub(dbc_rpc_bind_failure_message);
            exit(-1);
        }
        if (RpcQueueData.serve != 0) {
            break;
        }
        i = 0x10000;
        do {
        } while (--i != -1);
    }
    while (1) {
        if (sceSifBindRpc(&dbc_transfer_rpc_client, 0x8000091B, 0) < 0) {
            debug_print_stub(dbc_rpc_bind_failure_message);
            exit(-1);
        }
        if (dbc_transfer_rpc_client.serve != 0) {
            break;
        }
        i = 0x10000;
        do {
        } while (--i != -1);
    }
    ver = sceDbcGetModVersion();
    major = ver >> 8;
    if (major != 2) {
        scePrintf(dbc_module_version_mismatch_message);
        scePrintf(D_00153518, 2, 0, major, ver & 0xFF);
        return 0;
    }
    for (i = 15; i >= 0; i--) {
        D_0015B500[i] = 0;
    }
    debug_print_stub(D_00153540, D_0015B480);
    sceDbcSetWorkAddr(D_0015B480);
    return 1;
}
