#include "types.h"

extern s32
snd_send_iop_command_no_wait(s32 cmd, s32 size, void *buf, s32 a, s32 b) __asm__("func_0012E6E0");
void snd_get_vag_stream_time_remaining_cb(s32 stream_handle,
                                          s32 callback_address,
                                          s32 callback_context) __asm__("FUN_0012ecd0");

void snd_get_vag_stream_time_remaining_cb(s32 stream_handle,
                                          s32 callback_address,

                                          s32 callback_context) {
    u8 sp_slot[0x10];
    *(s32 *)sp_slot = stream_handle;
    snd_send_iop_command_no_wait(50, 4, sp_slot, callback_address, callback_context);
}

/* Recovered original symbol name. */
extern __typeof__(snd_get_vag_stream_time_remaining_cb) snd_GetVAGStreamTimeRemaining_CB
    __attribute__((alias("FUN_0012ecd0")));
