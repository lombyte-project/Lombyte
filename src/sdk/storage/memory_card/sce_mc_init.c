#include "types.h"
#include "kernel.h"
typedef struct {
    u8 pad0[0x24];
    void *server;
} SifClientData;


extern s32 D_00132DAC[];
extern char D_00153450[];
extern char D_00153468[];
extern char D_00153490[];
extern SifClientData D_00159A00;
extern u8 D_00159A80[];
extern s32 D_0015AFC0[];
extern s32 CreateSema(struct SemaParam *);
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern s32 sceMcSync(s32, s32 *, s32 *);
extern void sceSifInitRpc(u32);
extern s32 sceSifBindRpc(SifClientData *, u32, u32);
extern s32 sceSifCallRpc(SifClientData *, u32, u32, void *, s32, void *, s32, void *, void *);
extern s32 scePrintf(const char *, ...);

s32 sceMcInit(void) {
    struct SemaParam param;
    s32 ret;
    s32 i;

    if (D_00132DAC[0] < 0) {
        param.initCount = 1;
        param.maxCount = 1;
        param.option = 0;
        D_00132DAC[0] = CreateSema(&param);
    }
    sceMcSync(0, 0, 0);
    WaitSema(D_00132DAC[0]);
    sceSifInitRpc(0);
    while (1) {
        if (sceSifBindRpc(&D_00159A00, 0x80000400, 0) < 0) {
            scePrintf(D_00153450);
            while (1) {
            }
        }
        if (D_00159A00.server != 0) {
            break;
        }
        i = 0x100000;
        while (--i != 0) {
        }
    }
    ret = sceSifCallRpc(&D_00159A00, 0xFE, 0, D_00159A80, 48, D_0015AFC0, 12, 0, 0);
    SignalSema(D_00132DAC[0]);
    if (ret < 0) {
        D_00159A00.server = 0;
        return ret - 100;
    }
    if (D_0015AFC0[1] < 522) {
        scePrintf(D_00153468);
        D_00159A00.server = 0;
        return -120;
    }
    if (D_0015AFC0[2] < 526) {
        scePrintf(D_00153490);
        D_00159A00.server = 0;
        return -121;
    }
    return D_0015AFC0[0];
}
