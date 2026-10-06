#include "types.h"
extern s32 snd_send_iop_command_no_wait() __asm__("func_0012E6E0");
void snd_resolve_bank_xrefs(void) __asm__("FUN_0012e1a8");

void snd_resolve_bank_xrefs(void) {
    snd_send_iop_command_no_wait(8, 0, 0, 0, 0);
}

extern void func_0012E1A8(void) __attribute__((alias("FUN_0012e1a8")));
