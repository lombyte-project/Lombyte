#include "types.h"
#include "sda.h"

typedef void (*SndCallback)(s32, long);

struct SndCommand {
    SndCallback fn;
    s32 pad4;
    long arg;
};

extern s32 *snd_current_buffer __asm__("D_0015EC80") __attribute__((sda));          /* pending RPC record */
extern void (*snd_cd_status_callback)(s32) __asm__("D_0015EC90") __attribute__((sda)); /* stop callback */
extern s32 snd_cd_sync_pending __asm__("D_0015EC94") __attribute__((sda));          /* stop requested */
extern s32 snd_cd_stream_end_pending __asm__("D_0015EC98") __attribute__((sda));
extern s32 snd_return_callback_pending __asm__("D_0015EC9C") __attribute__((sda));     /* abort */
extern s32 *snd_batch_command_buffers[2] __asm__("D_0015ECA0") __attribute__((sda)); /* command counts */
extern struct SndCommand *snd_stream_buffers[2] __asm__("D_0015ECB0") __attribute__((sda));
extern s32 *snd_batch_return_buffers[2] __asm__("D_0015ECB8") __attribute__((sda)); /* replies */
extern s32 snd_batch_index __asm__("D_0015ECC0") __attribute__((sda));     /* current buffer */
extern s32 snd_cd_callback_pending __asm__("D_0015ECC8") __attribute__((sda));         /* CD read pending */
extern SndCallback snd_cd_callback_fn __asm__("D_0015ECD0") __attribute__((sda)); /* CD read callback */
extern volatile long snd_cd_callback_data __asm__("D_0015ECD8") MACRO_ADDR;         /* its argument */
extern long snd_return_callback_data __asm__("D_0015ECE8") __attribute__((sda));        /* its argument */
extern u32 snd_cd_callback_arg __asm__("D_0015ED00") MACRO_ADDR;                   /* CD read reply */
extern s32 snd_iop_return_word[] __asm__("D_00133104");
extern s32 snd_got_returns(void) __asm__("func_0012DE70");
extern void snd_send_current_batch(void) __asm__("func_0012E9D8");
extern s32 snd_stream_safe_cd_sync(s32) __asm__("func_0012EE08");
extern void FlushCache(s32);

s32 snd_flush_sound_commands(void) __asm__("FUN_0012dc80");

extern SndCallback snd_return_callback __asm__("D_0015ECE0") __attribute__((sda));
extern s32 GlobalTableStatus __asm__("D_0015ECC4") __attribute__((sda));
/* Once the pending RPC completes, run the callbacks queued with the other
   command buffer (or the abort callback); finish a CD read (FlushCache,
   then its callback with the reply word); send the next batch when idle;
   and poll a stop request. Returns whether an RPC or a CD read is still
   in flight. */
s32 snd_flush_sound_commands(void) {
    s32 i;
    s32 completed_batch_index;
    SndCallback fn;
    long arg;

    if (snd_current_buffer != 0 && snd_got_returns() != 0) {
        if (snd_return_callback_pending != 0) {
            if (snd_return_callback != 0) {
                snd_return_callback(snd_iop_return_word[0], snd_return_callback_data);
            }
            snd_return_callback = 0;
            snd_return_callback_pending = 0;
        } else {
            completed_batch_index = snd_batch_index != 1;
            for (i = 0; i < *snd_batch_command_buffers[completed_batch_index]; i++) {
                if (snd_stream_buffers[completed_batch_index][i].fn != 0) {
                    snd_stream_buffers[completed_batch_index][i].fn(snd_batch_return_buffers[completed_batch_index][i + 1], snd_stream_buffers[completed_batch_index][i].arg);
                }
            }
        }
    }
    if (snd_cd_callback_pending != 0) {
        FlushCache(0);
        if (snd_cd_callback_arg != 0xFFFFFFFF) {
            if (snd_cd_callback_fn != 0) {
                arg = snd_cd_callback_data;
                fn = snd_cd_callback_fn;
                snd_cd_callback_fn = 0;
                snd_cd_callback_data = 0;
                fn(snd_cd_callback_arg, arg);
            }
            snd_cd_callback_arg = 0;
            snd_cd_callback_pending = 0;
        }
    }
    if (snd_current_buffer == 0) {
        if (*snd_batch_command_buffers[snd_batch_index] != 0 && GlobalTableStatus == 0) {
            snd_send_current_batch();
        }
    }
    if (snd_cd_sync_pending != 0) {
        snd_stream_safe_cd_sync(1);
        if (snd_cd_stream_end_pending != 0) {
            snd_cd_sync_pending = 0;
            snd_cd_stream_end_pending = 0;
            if (snd_cd_status_callback != 0) {
                snd_cd_status_callback(1);
            }
        }
    }
    return snd_current_buffer != 0 || snd_cd_callback_pending != 0;
}

extern __typeof__(snd_flush_sound_commands) func_0012DC80 __attribute__((alias("FUN_0012dc80")));
