#ifndef LOMBYTE_RNC_AUDIO_RPC_H
#define LOMBYTE_RNC_AUDIO_RPC_H

#include "types.h"

void snd_prepare_return_buffer(s32 *, s32);
s32  snd_send_iop_command_and_wait(s32, s32, u8 *) __asm__("FUN_0012e548");
void snd_send_iop_command_no_wait(s32, s32, u8 *, s32, s64) __asm__("FUN_0012e6e0");

#endif /* LOMBYTE_RNC_AUDIO_RPC_H */
