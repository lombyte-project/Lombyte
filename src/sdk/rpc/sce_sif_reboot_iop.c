#include "types.h"
const char D_00152960[] = "rom0:UDNL ";
extern char D_00152970[];
extern s32 scePrintf(const char *, ...);
extern void sceSifInitRpc(u32);
extern void InitializeRpcCommand(void);
extern s32 sceSifResetIop(const char *arg, s32 mode);

s32 sceSifRebootIop(const char *img) {
    const char *prefix = D_00152960;
    char param[80];
    const char *p;
    char *d;

    p = img;
    while (*p) {
        p++;
    }
    if ((u32)(p + 11 - img) > 80) {
        scePrintf(D_00152970, img);
        return 0;
    }
    sceSifInitRpc(0);
    InitializeRpcCommand();
    d = param;
    while (*prefix) {
        *d++ = *prefix++;
    }
    while (*img) {
        *d++ = *img++;
    }
    *d = 0;
    return sceSifResetIop(param, 0);
}
