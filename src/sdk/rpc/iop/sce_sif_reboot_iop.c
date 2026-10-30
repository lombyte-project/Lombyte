#include "types.h"

const char  D_00152960[] = "rom0:UDNL ";
extern char iop_reboot_parameter_too_long_format[] __asm__("D_00152970");
extern s32  scePrintf(const char *, ...);
extern void sceSifInitRpc(u32);
extern void sceSifExitRpc(void) __asm__("InitializeRpcCommand");
extern s32  sceSifResetIop(const char *arg, s32 mode);

s32 sceSifRebootIop(const char *img) {
    const char *prefix = D_00152960;
    char        param[80];
    const char *image_end;
    char       *command_write_cursor;

    image_end = img;
    while (*image_end) {
        image_end++;
    }
    if ((u32)(image_end + 11 - img) > 80) {
        scePrintf(iop_reboot_parameter_too_long_format, img);
        return 0;
    }
    sceSifInitRpc(0);
    sceSifExitRpc();
    command_write_cursor = param;
    while (*prefix) {
        *command_write_cursor++ = *prefix++;
    }
    while (*img) {
        *command_write_cursor++ = *img++;
    }
    *command_write_cursor = 0;
    return sceSifResetIop(param, 0);
}
