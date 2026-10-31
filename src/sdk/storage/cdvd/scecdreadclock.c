#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/libcdvd.h"

struct unaligned64 {
    u64 v;
} __attribute__((packed));

extern u8    cd_read_clock_before_rpc_debug_message[] __asm__("D_00152FF0");
extern u8    cd_read_clock_after_copy_debug_message[] __asm__("D_00153010");
extern s32   SignalSema();
extern s32   cd_check_scmd() __asm__("func_00120D40");
extern s32   scePrintf();
extern s32   sceSifCallRpc();
extern void *memcpy();

s32 sceCdReadClock(u64 *clock) {
    u32 rpc_response_result;

    if (cd_check_scmd(0xF) == 0) {
        return 0;
    }
    if (cd_debug > 0) {
        scePrintf(cd_read_clock_before_rpc_debug_message);
    }
    if (sceSifCallRpc(&cd_scmd_client, 1, 0, 0, 0, cd_scmd_result, 0x10, 0, 0) < 0) {
        SignalSema(cd_scmd_sema);
        return 0;
    }
    *(struct unaligned64 *)clock = *(struct unaligned64 *)((u32)(cd_scmd_result + 4) | 0x20000000);
    if (cd_debug > 0) {
        scePrintf(cd_read_clock_after_copy_debug_message);
    }
    rpc_response_result = *(u32 *)((u32)cd_scmd_result | 0x20000000);
    SignalSema(cd_scmd_sema);
    return rpc_response_result;
}
