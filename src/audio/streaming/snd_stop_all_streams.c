#include "types.h"

extern s32 snd_send_iop_command_no_wait() __asm__("FUN_0012e6e0");
void       snd_stop_all_streams(void) __asm__("FUN_0012ebd0");

void snd_stop_all_streams(void) {
    snd_send_iop_command_no_wait(0x34, 0, 0, 0, 0);
}

extern void func_0012EBD0(void) __attribute__((alias("FUN_0012ebd0")));
