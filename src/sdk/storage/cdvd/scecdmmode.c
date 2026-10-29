#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/libcdvd.h"

extern u8   cd_media_mode_request[] __asm__("D_00132900");
extern s32  SignalSema();
extern s32  cd_check_scmd() __asm__("func_00120D40");
extern s32  sceSifCallRpc();
extern void sceSifWriteBackDCache();

s32 sceCdMmode(s32 media_mode) {
    register u8 *state;
    register s32 mode;
    s32          media_mode_response;

    state = cd_media_mode_request;
    mode = media_mode;
    if (cd_check_scmd(0x22) == 0) {
        return 0;
    }
    *(s32 *)cd_media_mode_request = mode;
    sceSifWriteBackDCache(state, 4);
    if (sceSifCallRpc(&cd_scmd_client, 0x22, 0, state, 4, cd_scmd_result, 4, 0, 0) < 0) {
        SignalSema(cd_scmd_sema);
        return 0;
    }
    media_mode_response = *(u32 *)((u32)cd_scmd_result | 0x20000000);
    SignalSema(cd_scmd_sema);
    return media_mode_response;
}
