#include "types.h"
#include "rnc/sdk/libcdvd.h"

extern volatile u32 D_00131314[];
extern s32          SignalSema(s32 semaId);
extern s32          cd_check_scmd(s32 cmd) __asm__("func_00120D40");
extern s32          sceSifCallRpc(void *pkt,
                                  u32   cmd,
                                  s32   mode,
                                  void *send,
                                  s32   sendsize,
                                  void *recv,
                                  s32   recvsize,
                                  void *endfunc,
                                  void *endparam);

s32 sceCdBreak(void) {
    s32 status;

    if (cd_check_scmd(0x1E) == 0) {
        return 0;
    }
    D_00131314[0] = 8;
    if (sceSifCallRpc(&cd_scmd_client, 0x16, 0, 0, 0, cd_scmd_result, 4, 0, 0) < 0) {
        s32 sema = *(volatile u32 *)&cd_scmd_sema;
        SignalSema(sema);
        D_00131314[0] = 0;
        return 0;
    }
    D_00131314[0] = 0;
    status = *(u32 *)((u32)cd_scmd_result | 0x20000000);
    {
        s32 sema2 = *(volatile u32 *)&cd_scmd_sema;
        SignalSema(sema2);
    }
    return status;
}
