#include "types.h"

extern s32
snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a, s32 b) __asm__("func_0012E6E0");
void snd_sound_is_still_playing_cb(s32 sound_handle,
                                   s32 callback_address,
                                   s32 context_address) __asm__("FUN_0012e448");

void snd_sound_is_still_playing_cb(s32 sound_handle, s32 callback_address, s32 context_address) {
    u8 sp_slot[0x10];
    *(s32 *)sp_slot = sound_handle;
    snd_send_iop_command_no_wait(25, 4, sp_slot, callback_address, context_address);
}
