#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/libcdvd.h"

extern s32 SignalSema();
extern s32 cd_check_scmd() __asm__("func_00120D40");
extern s32 sceSifCallRpc();

s32 sceCdGetError(void) {
    s32 error;

    if (cd_check_scmd(3) == 0) {
        return -1;
    }
    if (sceSifCallRpc(&cd_scmd_client, 4, 0, 0, 0, cd_scmd_result, 4, 0, 0) < 0) {
        SignalSema(cd_scmd_sema);
        return -1;
    }
    error = *(u32 *)((u32)cd_scmd_result | 0x20000000);
    SignalSema(cd_scmd_sema);
    return error;
}
