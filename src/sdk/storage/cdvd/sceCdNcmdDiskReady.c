#include "types.h"
#include "rnc/sdk/libcdvd.h"

extern u8  native_cd_drive_state_response[] __asm__("D_00131340");
extern s32 SignalSema();
extern s32 cd_check_ncmd() __asm__("func_00120A28");
extern s32 sceSifCallRpc();

s32 sceCdNcmdDiskReady(void) {
    s32 status;

    if (cd_check_ncmd(2) == 0) {
        return 0;
    }
    if (sceSifCallRpc(&cd_ncmd_client, 0xE, 0, 0, 0, native_cd_drive_state_response, 4, 0, 0) < 0) {
        SignalSema(cd_ncmd_sema);
        return 0;
    }
    status = *(u32 *)((u32)native_cd_drive_state_response | 0x20000000);
    SignalSema(cd_ncmd_sema);
    return status;
}
